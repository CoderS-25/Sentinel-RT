#include "task_profiler.h"
#include <string.h>

// 5 tasks * 5 features + 1 global = 26 floats
static float feature_vector[26];
static ID sync_flg_id;

/**
 * @brief The profiler task entry point.
 * 
 * @param stacd Task start code.
 * @param exinf Extended information.
 */
static void profiler_task(INT stacd, void *exinf) {
    while (1) {
        // TODO: Gather actual task metrics using tk_ref_tsk
        // Placeholder for gathering 26 float features
        memset(feature_vector, 0, sizeof(feature_vector));
        
        // Signal Inference Engine (flag bit 0x01)
        tk_set_flg(sync_flg_id, 0x01);
        
        tk_dly_tsk(PROFILER_INTERVAL_MS);
    }
    tk_ext_tsk();
}

/**
 * @brief Initialize the task profiler.
 * 
 * @param flg_id The event flag ID used to signal the inference engine.
 * @return ER E_OK on success, error code otherwise.
 */
ER sentinel_profiler_init(ID flg_id) {
    sync_flg_id = flg_id;
    T_CTSK ctsk = {0};
    ctsk.tskatr = TA_HLNG | TA_RNG3;
    ctsk.task = profiler_task;
    ctsk.itskpri = PROFILER_PRIORITY;
    ctsk.stksz = 1024;
    ID tsk_id = tk_cre_tsk(&ctsk);
    if (tsk_id < E_OK) return tsk_id;
    return tk_sta_tsk(tsk_id, 0);
}

/**
 * @brief Get the latest feature vector.
 * 
 * @param vector Pointer to a float array of size 26.
 */
void sentinel_profiler_get_features(float* vector) {
    memcpy(vector, feature_vector, sizeof(feature_vector));
}
