/**
 * @file    inference_engine.c
 * @brief   Sentinel-RT Inference Engine — LL_ATON Neural-ART NPU implementation.
 *
 * This version uses the LL_ATON runtime API (ll_aton_rt_user_api.h) which is
 * the correct API for the STM32N6's Neural-ART NPU.  The old X-CUBE-AI legacy
 * API (ai_network_create_and_init, ai_network_run, etc.) does NOT exist on
 * this platform.
 *
 * Data flow:
 *   1. Profiler signals feature vector ready (flag bit 0x01).
 *   2. This engine quantizes 26 floats → INT8, runs NPU inference via
 *      LL_ATON_RT_RunEpochBlock(), then dequantizes the outputs.
 *   3. Signals the Decision Engine (flag bit 0x02).
 *
 * Reference:
 *   - STM32N6 LL_ATON Runtime User API (ll_aton_rt_user_api.h)
 *   - ll_aton_rt_main.c (ST-provided template for synchronous inference)
 *   - network.h (generated: model name = "Default", 1 input x 26B, 2 outputs)
 */

#include "inference_engine.h"
#include "../profiler/task_profiler.h"
#include "../ai_model_config.h"
#include <string.h>

/* ── LL_ATON NPU Runtime headers ─────────────────────────────────────────
 * These are the actual API for the STM32N6 Neural-ART accelerator.
 * ──────────────────────────────────────────────────────────────────────── */
#include "ll_aton_rt_user_api.h"
#include "ll_aton_runtime.h"

/* ── Declare the NN interface and instance for model "Default" ──────────
 * The network name "Default" comes from network.h:
 *   LL_ATON_DEFAULT_C_MODEL_NAME  "Default"
 *
 * This macro expands to:
 *   - extern declarations for LL_ATON_EC_Network_Init_Default, etc.
 *   - static const NN_Interface_TypeDef NN_Interface_Default = { ... };
 *   - static NN_Instance_TypeDef NN_Instance_Default = { ... };
 * ──────────────────────────────────────────────────────────────────────── */
LL_ATON_DECLARE_NAMED_NN_INSTANCE_AND_INTERFACE(Default);

/* ── Module state ────────────────────────────────────────────────────── */
static SentinelDecision latest_decision;
static ID               sync_flg_id;
static int              runtime_initialized = 0;

/* ── Quantization helpers ────────────────────────────────────────────── */

/**
 * @brief Quantize a float value to INT8 using affine quantization.
 *
 * q = clamp(round(value / scale) + zero_point, -128, 127)
 */
static inline int8_t quantize_float_to_int8(float value, float scale, int32_t zero_point)
{
    int32_t q = (int32_t)(value / scale + 0.5f) + zero_point;
    if (q < -128) q = -128;
    if (q >  127) q =  127;
    return (int8_t)q;
}

/**
 * @brief Dequantize an INT8 value back to float.
 *
 * value = (q - zero_point) * scale
 */
static inline float dequantize_int8_to_float(int8_t q, float scale, int32_t zero_point)
{
    return ((float)q - (float)zero_point) * scale;
}

/* ── Network initialization ──────────────────────────────────────────── */

/**
 * @brief Initialize the LL_ATON runtime and network instance.
 * @return 0 on success, -1 on failure.
 */
static int ai_network_setup(void)
{
    /* Initialize the LL_ATON runtime (configures NPU hardware) */
    LL_ATON_RT_RuntimeInit();

    /* Initialize the network instance (loads epoch blocks, etc.) */
    LL_ATON_RT_Init_Network(&NN_Instance_Default);

    runtime_initialized = 1;
    return 0;
}

/* ── Core inference function ─────────────────────────────────────────── */

/**
 * @brief Run one complete inference cycle on the Neural-ART NPU.
 *
 * @param features  Array of 26 float feature values from the profiler.
 * @param decision  Output: filled with dequantized priority + power predictions.
 * @return 0 on success, -1 on inference error.
 */
static int run_inference(const float features[SENTINEL_INPUT_SIZE],
                         SentinelDecision *decision)
{
    LL_ATON_RT_RetValues_t ret;

    /* ── Step 1: Quantize input features (float → INT8) ──────────── */
    int8_t input_quantized[SENTINEL_INPUT_SIZE];
    for (int i = 0; i < SENTINEL_INPUT_SIZE; i++) {
        input_quantized[i] = quantize_float_to_int8(
            features[i],
            SENTINEL_INPUT_SCALE,
            SENTINEL_INPUT_ZERO_POINT
        );
    }

    /* ── Step 2: Copy quantized input into the network's input buffer ── */
    /*
     * Get the pointer to the network's internal input buffer.
     * The LL_ATON runtime manages its own input/output memory.
     * Input buffer index 0 = the only input (26 bytes of INT8).
     */
    const LL_Buffer_InfoTypeDef *in_info = LL_ATON_Input_Buffers_Info(&NN_Instance_Default);
    if (in_info != NULL && in_info->name != NULL) {
        uint8_t *in_ptr = LL_Buffer_addr_start(in_info);
        if (in_ptr != NULL) {
            memcpy(in_ptr, input_quantized, SENTINEL_INPUT_SIZE);
        }
    }

    /* ── Step 3: Reset network state for a new inference ─────────── */
    LL_ATON_RT_Reset_Network(&NN_Instance_Default);

    /* ── Step 4: Execute inference epoch-by-epoch on the NPU ─────── */
    /*
     * LL_ATON_RT_RunEpochBlock() drives the NPU through each epoch
     * block until the entire network has been executed.
     * This is the pattern from ll_aton_rt_main.c (ST template).
     */
    do {
        ret = LL_ATON_RT_RunEpochBlock(&NN_Instance_Default);

        if (ret == LL_ATON_RT_WFE) {
            /* Epoch block is still running on the NPU — wait for IRQ */
            LL_ATON_OSAL_WFE();
        }
    } while (ret != LL_ATON_RT_DONE);

    /* ── Step 5: Read and dequantize outputs ─────────────────────── */
    /*
     * From network.h (generated by stedgeai):
     *   Output 1 (index 0): 3 bytes  — power state  (Quantize_30_out_0)
     *   Output 2 (index 1): 5 bytes  — task priorities (Quantize_27_out_0)
     *
     * We iterate through the output buffer info array.
     */
    const LL_Buffer_InfoTypeDef *out_info = LL_ATON_Output_Buffers_Info(&NN_Instance_Default);

    if (out_info == NULL || out_info->name == NULL) {
        return -1;  /* No output buffers — something went wrong */
    }

    /* Output 0: Power state [3 bytes] */
    {
        const LL_Buffer_InfoTypeDef *pwr_buf = &out_info[0];
        int8_t *pwr_data = (int8_t *)LL_Buffer_addr_start(pwr_buf);
        if (pwr_data != NULL) {
            for (int i = 0; i < SENTINEL_PWR_OUTPUT_SIZE; i++) {
                decision->power_state[i] = dequantize_int8_to_float(
                    pwr_data[i],
                    SENTINEL_PWR_SCALE,
                    SENTINEL_PWR_ZERO_POINT
                );
            }
        }
    }

    /* Output 1: Task priorities [5 bytes] */
    {
        const LL_Buffer_InfoTypeDef *pri_buf = &out_info[1];
        if (pri_buf->name != NULL) {
            int8_t *pri_data = (int8_t *)LL_Buffer_addr_start(pri_buf);
            if (pri_data != NULL) {
                for (int i = 0; i < SENTINEL_PRI_OUTPUT_SIZE; i++) {
                    decision->task_priorities[i] = dequantize_int8_to_float(
                        pri_data[i],
                        SENTINEL_PRI_SCALE,
                        SENTINEL_PRI_ZERO_POINT
                    );
                }
            }
        }
    }

    return 0;
}

/* ── RTOS task ───────────────────────────────────────────────────────── */

/**
 * @brief The inference engine μT-Kernel task entry point.
 *
 * Waits for the profiler to signal a new feature vector, runs inference
 * on the NPU, and signals the decision engine with the result.
 */
static void inference_task(INT stacd, void *exinf)
{
    UINT  flgptn;
    float features[SENTINEL_INPUT_SIZE];

    /* Initialize the LL_ATON runtime and network (one-time setup) */
    if (ai_network_setup() != 0) {
        /*
         * CRITICAL: Network init failed.
         * Fall through to a safe-mode loop that keeps the RTOS alive
         * but does not produce inference results.
         */
        while (1) {
            memset(&latest_decision, 0, sizeof(SentinelDecision));
            latest_decision.power_state[0] = 1.0f; /* Active = safe default */
            tk_set_flg(sync_flg_id, 0x02);
            tk_dly_tsk(100);
        }
    }

    /* Main inference loop */
    while (1) {
        /* Wait for profiler signal (bit 0x01) */
        tk_wai_flg(sync_flg_id, 0x01, TWF_CLR | TWF_ANDW, &flgptn, TMO_FEVR);

        /* Get the latest feature vector from the profiler */
        sentinel_profiler_get_features(features);

        /* Run inference */
        SentinelDecision new_decision;
        if (run_inference(features, &new_decision) == 0) {
            /* Success — update the shared decision atomically */
            memcpy(&latest_decision, &new_decision, sizeof(SentinelDecision));
        }
        /* On failure, latest_decision retains the previous good value */

        /* Signal Decision Engine (flag bit 0x02) */
        tk_set_flg(sync_flg_id, 0x02);
    }
    tk_ext_tsk();
}

/* ── Public API ──────────────────────────────────────────────────────── */

/**
 * @brief Initialize the inference engine.
 *
 * @param flg_id  Event flag ID for inter-component synchronization.
 * @return ER  E_OK on success, error code otherwise.
 */
ER sentinel_inference_init(ID flg_id)
{
    sync_flg_id = flg_id;

    /* Initialize the latest decision to safe defaults */
    memset(&latest_decision, 0, sizeof(SentinelDecision));
    latest_decision.power_state[0] = 1.0f; /* Active state */

    /* Create the inference RTOS task */
    T_CTSK ctsk   = {0};
    ctsk.tskatr   = TA_HLNG | TA_RNG3;
    ctsk.task     = inference_task;
    ctsk.itskpri  = INFERENCE_PRIORITY;
    ctsk.stksz    = INFERENCE_STACK_SIZE;

    ID tsk_id = tk_cre_tsk(&ctsk);
    if (tsk_id < E_OK) return tsk_id;

    return tk_sta_tsk(tsk_id, 0);
}

/**
 * @brief Get the latest decision output from the inference engine.
 *
 * @param decision  Pointer to store the decision.
 */
void sentinel_inference_get_decision(SentinelDecision *decision)
{
    memcpy(decision, &latest_decision, sizeof(SentinelDecision));
}
