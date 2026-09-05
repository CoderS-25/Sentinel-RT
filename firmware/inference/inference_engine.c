#include "inference_engine.h"
#include "../profiler/task_profiler.h"
#include <string.h>

// Placeholder for Neural-ART delegate
// TODO: Include actual Neural-ART headers
// #include <neural_art_delegate.h>

// TODO: run: xxd -i sentinel_model.tflite > model_data.c
extern const unsigned char sentinel_model_tflite[];
extern const unsigned int sentinel_model_tflite_len;

static SentinelDecision latest_decision;
static ID sync_flg_id;

/**
 * @brief The inference engine task entry point.
 * 
 * @param stacd Task start code.
 * @param exinf Extended information.
 */
static void inference_task(INT stacd, void *exinf) {
    UINT flgptn;
    float features[26];

    // TODO: Initialize TFLite interpreter with Neural-ART delegate

    while (1) {
        // Wait for profiler signal (bit 0x01)
        tk_wai_flg(sync_flg_id, 0x01, TW_CLR | TW_AND, &flgptn, TMO_FEVR);

        // Get features
        sentinel_profiler_get_features(features);

        // TODO: Copy 26 floats to TFLite input tensor
        // TODO: Run inference
        // TODO: Copy outputs to latest_decision

        // Mock outputs
        memset(&latest_decision, 0, sizeof(SentinelDecision));
        latest_decision.power_state[0] = 1.0f; // Active

        // Signal Decision Engine (flag bit 0x02)
        tk_set_flg(sync_flg_id, 0x02);
    }
    tk_ext_tsk();
}

/**
 * @brief Initialize the inference engine.
 * 
 * @param flg_id The event flag ID used to synchronize components.
 * @return ER E_OK on success, error code otherwise.
 */
ER sentinel_inference_init(ID flg_id) {
    sync_flg_id = flg_id;
    T_CTSK ctsk = {0};
    ctsk.tskatr = TA_HLNG | TA_RNG3;
    ctsk.task = inference_task;
    ctsk.itskpri = INFERENCE_PRIORITY;
    ctsk.stksz = 1024;
    ID tsk_id = tk_cre_tsk(&ctsk);
    if (tsk_id < E_OK) return tsk_id;
    return tk_sta_tsk(tsk_id, 0);
}

/**
 * @brief Get the latest decision output from the inference engine.
 * 
 * @param decision Pointer to store the decision.
 */
void sentinel_inference_get_decision(SentinelDecision* decision) {
    memcpy(decision, &latest_decision, sizeof(SentinelDecision));
}
