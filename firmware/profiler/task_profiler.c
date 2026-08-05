/**
 * @file task_profiler.c
 * @brief Implementation of the Task Profiler component (Eyes)
 */
#include "task_profiler.h"
#include <string.h>

/* Assuming event flag ID is globally known or returned. Placeholder for event flag. */
extern ID sentinel_event_flg;
extern ID monitored_tasks[MAX_TASKS];

/**
 * @brief Initializes the profiler as a uT-Kernel task
 */
void sentinel_profiler_init(void) {
    T_CTSK ctsk = {0};
    ctsk.tskatr = TA_HLNG | TA_RNG0;
    ctsk.task = sentinel_profiler_task_entry;
    ctsk.itskpri = PROFILER_PRIORITY;
    ctsk.stksz = PROFILER_STACK_SIZE;

    ID tskid = tk_cre_tsk(&ctsk);
    if (tskid > 0) {
        tk_sta_tsk(tskid, 0);
    }
}

/**
 * @brief uT-Kernel task entry function
 */
void sentinel_profiler_task_entry(INT stacd, void *exinf) {
    (void)stacd; 
    (void)exinf;
    SentinelFeatureVector features;

    while (1) {
        /* Periodic delay */
        tk_dly_tsk(PROFILER_INTERVAL_MS);

        sentinel_profiler_get_features(&features);

        /* Signal the inference engine that features are ready */
        tk_set_flg(sentinel_event_flg, 0x01);
    }
}

/**
 * @brief Fills the feature vector
 */
void sentinel_profiler_get_features(SentinelFeatureVector *out) {
    if (!out) return;
    memset(out, 0, sizeof(SentinelFeatureVector));

    /* TODO: Helium/MVE acceleration for feature vector normalization and aggregation */
    float total_load = 0.0f;

    for (int i = 0; i < MAX_TASKS; ++i) {
        if (monitored_tasks[i] != 0) {
            T_RTSK rtsk;
            tk_ref_tsk(monitored_tasks[i], &rtsk);

            /* Compute features based on task state */
            out->tasks[i].cpu_load = 0.0f; /* Compute from execution time counters */
            out->tasks[i].wait_time_ms = 0.0f; /* Compute from task wait state */
            out->tasks[i].deadline_proximity = 0.0f; /* TODO: Hardware-specific timer lookup */
            out->tasks[i].context_switch_rate = 0.0f; /* TODO: Hook into context switch */
            out->tasks[i].is_blocked = (rtsk.tskstat == TTS_WAI) ? 1.0f : 0.0f;

            total_load += out->tasks[i].cpu_load;
        }
    }
    out->total_system_cpu_load = total_load;
}
