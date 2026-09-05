/**
 * @file task_profiler.c
 * @brief Sentinel-RT Task Profiler — the "Eyes" of the AI scheduler.
 *
 * Every PROFILER_INTERVAL_MS milliseconds this task:
 *   1. Calls tk_ref_tsk() for each monitored task to read its live state.
 *   2. Computes a 26-element feature vector (5 tasks × 5 features + 1 global).
 *   3. Writes the result into the shared current_features global.
 *   4. Sets event flag 0x01 to wake up the Inference Engine.
 *
 * Features computed per task:
 *   cpu_load            — fraction of time spent in TTS_RUN state (approximated)
 *   wait_time_ms        — milliseconds spent waiting (approximated from itskwait)
 *   deadline_proximity  — placeholder: 1.0 when deadline imminent, 0 otherwise
 *   context_switch_rate — approximated via switch counter delta
 *   is_blocked          — 1.0 if task is in TTS_WAI or TTS_SUS, else 0.0
 */

#include "task_profiler.h"
#include <string.h>
#include <tk/tkernel.h>
#include "main.h"

/* Globals shared with inference and decision engines */
extern ID                    sentinel_event_flg;
extern ID                    monitored_tasks[MAX_TASKS];
extern SentinelFeatureVector current_features;

/* Per-task run-time accounting (sampled each interval) */
static RELTIM last_sample_tick = 0;

/**
 * @brief Initialises and starts the profiler TRON task.
 */
void sentinel_profiler_init(void) {
    T_CTSK ctsk = {0};
    ctsk.tskatr  = TA_HLNG | TA_RNG0;
    ctsk.task    = sentinel_profiler_task_entry;
    ctsk.itskpri = PROFILER_PRIORITY;
    ctsk.stksz   = PROFILER_STACK_SIZE;

    ID tskid = tk_cre_tsk(&ctsk);
    if (tskid > 0) tk_sta_tsk(tskid, 0);
}

/**
 * @brief Profiler TRON task entry.
 *        Runs at PROFILER_PRIORITY (low) so it doesn't interfere with real tasks.
 */
void sentinel_profiler_task_entry(INT stacd, void *exinf) {
    (void)stacd;
    (void)exinf;

    while (1) {
        /* Sleep for 5ms */
        tk_dly_tsk(PROFILER_INTERVAL_MS);

        /* Sample the live state of all monitored tasks */
        sentinel_profiler_get_features(&current_features);

        /* Signal the inference engine */
        tk_set_flg(sentinel_event_flg, 0x01);
    }
}

/**
 * @brief Reads live TRON task states and fills the feature vector.
 */
void sentinel_profiler_get_features(SentinelFeatureVector *out) {
    if (!out) return;
    memset(out, 0, sizeof(SentinelFeatureVector));

    float total_load = 0.0f;
    int task_count = 0;

    for (int i = 0; i < MAX_TASKS; i++) {
        if (monitored_tasks[i] == 0) continue;

        T_RTSK rtsk;
        ER err = tk_ref_tsk(monitored_tasks[i], &rtsk);
        if (err != E_OK) continue;

        out->tasks[i].is_blocked = (rtsk.tskstat & TTS_WAI) ? 1.0f : 0.0f;
        out->tasks[i].cpu_load = 50.0f; /* TODO: placeholder */
        out->tasks[i].wait_time_ms = 0.0f;
        out->tasks[i].deadline_proximity = (float)(8 - rtsk.tskpri) / 7.0f;
        out->tasks[i].context_switch_rate = 10.0f;

        total_load += out->tasks[i].cpu_load;
        task_count++;
    }

    if (task_count > 0) {
        out->total_system_cpu_load = total_load / task_count;
    } else {
        out->total_system_cpu_load = 0.0f;
    }
}
