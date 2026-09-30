/**
 * @file    inference_engine.c
 * @brief   Sentinel-RT Inference Engine — COMPLETE X-CUBE-AI implementation.
 *
 * Replaces the original scaffold that had TODO placeholders and mock outputs.
 * This version performs real neural inference using the X-CUBE-AI runtime
 * (ai_platform.h API), which targets the Neural-ART NPU on the STM32N6.
 *
 * Data flow:
 *   1. Profiler signals feature vector ready (flag bit 0x01).
 *   2. This engine quantizes 26 floats → INT8, runs inference, dequantizes.
 *   3. Signals the Decision Engine (flag bit 0x02).
 *
 * Reference:
 *   - STM32N6-GettingStarted-ImageClassification (GitHub, STMicroelectronics)
 *   - X-CUBE-AI 10.2.1 API documentation
 *   - ST EdgeAI-Core Tutorial (YouTube: usQ_f6Swpok)
 *   - .ai/network_sentinel_model.tflite_c_info.json (quantization params)
 */

#include "inference_engine.h"
#include "../profiler/task_profiler.h"
#include "../ai_model_config.h"
#include <string.h>

/* ── X-CUBE-AI generated headers ─────────────────────────────────────────
 * These are produced by:
 *   stedgeai generate --model sentinel_model.tflite --target stm32n6
 *
 * After running that command, the following headers will exist in your
 * CubeIDE project under the X-CUBE-AI middleware folder.
 *
 * If they are not present, you need to run the stedgeai generate step
 * first (see firmware_v2/README.md).
 * ──────────────────────────────────────────────────────────────────────── */
#include "ai_platform.h"
#include "network.h"
#include "network_data.h"

/* ── External model weights (from model_data.c) ─────────────────────────
 * These are the raw INT8 TFLite bytes embedded as a C array.
 * They are referenced by the X-CUBE-AI runtime when weights are
 * stored in internal memory rather than external flash.
 * ──────────────────────────────────────────────────────────────────────── */
extern const unsigned char sentinel_model_tflite[];
extern const unsigned int  sentinel_model_tflite_len;

/* ── Module state ────────────────────────────────────────────────────── */
static SentinelDecision latest_decision;
static ID               sync_flg_id;

/* X-CUBE-AI runtime handles */
static ai_handle  network_handle = AI_HANDLE_NULL;
static ai_buffer *ai_input  = NULL;
static ai_buffer *ai_output = NULL;

/*
 * Activation buffer — this is the "arena" used by the network for
 * intermediate tensor storage during inference.
 * Size from ai_model_config.h (2048 bytes, rounded up from ~1100).
 * Aligned to 8 bytes for ARM Cortex-M55 SIMD requirements.
 */
AI_ALIGNED(8)
static ai_u8 activation_buffer[SENTINEL_ACTIVATION_BUFFER_SIZE];

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
 * @brief Create and initialize the X-CUBE-AI network.
 * @return 0 on success, -1 on failure.
 */
static int ai_network_setup(void)
{
    ai_error err;

    /*
     * Create and initialize the network in one call.
     * The activation_buffer provides workspace for intermediate tensors.
     * The second parameter (NULL) means weights are already linked
     * into the binary (via model_data.c / network_data.c).
     */
    err = ai_network_create_and_init(&network_handle,
                                     (const ai_handle)activation_buffer,
                                     NULL);
    if (err.type != AI_ERROR_NONE) {
        /* Initialization failed — network_handle remains AI_HANDLE_NULL */
        return -1;
    }

    /* Get pointers to the input and output buffer descriptors */
    ai_input  = ai_network_inputs_get(network_handle, NULL);
    ai_output = ai_network_outputs_get(network_handle, NULL);

    if (ai_input == NULL || ai_output == NULL) {
        ai_network_destroy(network_handle);
        network_handle = AI_HANDLE_NULL;
        return -1;
    }

    return 0;
}

/* ── Core inference function ─────────────────────────────────────────── */

/**
 * @brief Run one inference cycle.
 *
 * @param features  Array of 26 float feature values from the profiler.
 * @param decision  Output: filled with dequantized priority + power predictions.
 * @return 0 on success, -1 on inference error.
 */
static int run_inference(const float features[SENTINEL_INPUT_SIZE],
                         SentinelDecision *decision)
{
    int8_t input_quantized[SENTINEL_INPUT_SIZE];
    int8_t output_pri[SENTINEL_PRI_OUTPUT_SIZE];
    int8_t output_pwr[SENTINEL_PWR_OUTPUT_SIZE];

    /* ── Step 1: Quantize input features (float → INT8) ──────────── */
    for (int i = 0; i < SENTINEL_INPUT_SIZE; i++) {
        input_quantized[i] = quantize_float_to_int8(
            features[i],
            SENTINEL_INPUT_SCALE,
            SENTINEL_INPUT_ZERO_POINT
        );
    }

    /* ── Step 2: Point the AI input buffer to our quantized data ──── */
    ai_input[0].data = AI_HANDLE_PTR(input_quantized);

    /*
     * The network has two output heads.  X-CUBE-AI stores them
     * as separate ai_buffer entries:
     *   ai_output[0] → task priorities [1, 5]
     *   ai_output[1] → power state     [1, 3]
     *
     * We point them to our local output arrays.
     */
    ai_output[0].data = AI_HANDLE_PTR(output_pri);
    ai_output[1].data = AI_HANDLE_PTR(output_pwr);

    /* ── Step 3: Execute inference on the Neural-ART NPU ─────────── */
    ai_i32 batch = ai_network_run(network_handle, ai_input, ai_output);
    if (batch != 1) {
        /* Inference failed */
        return -1;
    }

    /* ── Step 4: Dequantize outputs (INT8 → float) ───────────────── */

    /* Priority head */
    for (int i = 0; i < SENTINEL_PRI_OUTPUT_SIZE; i++) {
        decision->task_priorities[i] = dequantize_int8_to_float(
            output_pri[i],
            SENTINEL_PRI_SCALE,
            SENTINEL_PRI_ZERO_POINT
        );
    }

    /* Power state head (softmax probabilities) */
    for (int i = 0; i < SENTINEL_PWR_OUTPUT_SIZE; i++) {
        decision->power_state[i] = dequantize_int8_to_float(
            output_pwr[i],
            SENTINEL_PWR_SCALE,
            SENTINEL_PWR_ZERO_POINT
        );
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

    /* Initialize the X-CUBE-AI network (one-time setup) */
    if (ai_network_setup() != 0) {
        /*
         * CRITICAL: Network init failed.
         * This should not happen if stedgeai generate was run correctly
         * and the model weights are linked.  We fall through to a
         * safe-mode loop that keeps the RTOS alive but does not
         * produce inference results.
         */
        while (1) {
            /* Signal decision engine with zeroed-out (safe) decision */
            memset(&latest_decision, 0, sizeof(SentinelDecision));
            latest_decision.power_state[0] = 1.0f; /* Active = safe default */
            tk_set_flg(sync_flg_id, 0x02);
            tk_dly_tsk(100); /* Don't spin-loop */
        }
    }

    /* Main inference loop */
    while (1) {
        /* Wait for profiler signal (bit 0x01) */
        tk_wai_flg(sync_flg_id, 0x01, TW_CLR | TW_AND, &flgptn, TMO_FEVR);

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
