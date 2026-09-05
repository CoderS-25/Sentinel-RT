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
        /* Sleep for the sampling interval — use HAL_GetTick() based delay
         * to avoid dependence on the TRON timer interrupt during bring-up.  */
        RELTIM now = (RELTIM)HAL_GetTick();
        if ((now - last_sample_tick) < PROFILER_INTERVAL_MS) {
            /* Not time yet — yield so other tasks can run */
            tk_slp_tsk(PROFILER_INTERVAL_MS - (now - last_sample_tick));
        }
        last_sample_tick = (RELTIM)HAL_GetTick();

        /* Sample the live state of all monitored tasks */
        sentinel_profiler_get_features(&current_features);

        /* Signal the inference engine */
        tk_set_flg(sentinel_event_flg, 0x01);
    }
}

/**
 * @brief Reads live TRON task states and fills the feature vector.
 *
 * tk_ref_tsk() returns a T_RTSK struct which contains:
 *   tskstat  — TTS_RUN / TTS_RDY / TTS_WAI / TTS_SUS / TTS_DMT
 *   tskpri   — current priority
 *   wupcnt   — pending wakeup count
 *   suscnt   — suspend nesting count
 *   itskwait — internal wait factor (what it's waiting for)
 *   exinf    — extended info set at task creation
 *   texstat  — task exception status
 */
void sentinel_profiler_get_features(SentinelFeatureVector *out) {
    if (!out) return;
    memset(out, 0, sizeof(SentinelFeatureVector));

    float total_load = 0.0f;

    for (int i = 0; i < MAX_TASKS; i++) {
        if (monitored_tasks[i] == 0) continue;   /* slot empty */

        T_RTSK rtsk;
        ER err = tk_ref_tsk(monitored_tasks[i], &rtsk);
        if (err != E_OK) continue;               /* task may have exited */

        /* --- is_blocked -------------------------------------------- */
        float blocked = ((rtsk.tskstat == TTS_WAI) ||
                         (rtsk.tskstat == TTS_SUS) ||
                         (rtsk.tskstat == TTS_WAS)) ? 1.0f : 0.0f;
        out->tasks[i].is_blocked = blocked;

        /* --- cpu_load (approximation) ------------------------------ */
        /* We approximate load as the complement of blocked time.
         * A running/ready task contributes to load; a waiting one does not.
         * For a proper measurement you'd hook into the context-switch handler. */
        float load = (rtsk.tskstat == TTS_RUN || rtsk.tskstat == TTS_RDY)
                     ? (1.0f - (float)rtsk.suscnt * 0.1f)
                     : 0.0f;
        if (load < 0.0f) load = 0.0f;
        if (load > 1.0f) load = 1.0f;
        out->tasks[i].cpu_load = load;

        /* --- wait_time_ms (approximation) -------------------------- */
        /* itskwait contains bit flags for what the task is waiting on.
         * We use wupcnt as a proxy — high wupcnt means it was woken often
         * (low wait time); zero means it's been sleeping a while.           */
        out->tasks[i].wait_time_ms = (rtsk.tskstat == TTS_WAI)
                                     ? (float)(PROFILER_INTERVAL_MS)
                                     : 0.0f;

        /* --- deadline_proximity (placeholder) ---------------------- */
        /* A real implementation would look up a per-task deadline stored
         * in a registration table. For now we use priority inversion:
         * tasks at priority 1 (highest) are assumed near their deadline.    */
        out->tasks[i].deadline_proximity = (float)(8 - rtsk.tskpri) / 7.0f;

        /* --- context_switch_rate (placeholder) --------------------- */
        /* Without a kernel hook we cannot count switches directly.
         * We set a synthetic value based on task readiness.                 */
        out->tasks[i].context_switch_rate = (rtsk.tskstat == TTS_RDY) ? 50.0f : 10.0f;

        total_load += out->tasks[i].cpu_load;
    }

    /* Normalise total load to [0,1] */
    out->total_system_cpu_load = (total_load > 1.0f) ? 1.0f : total_load;
}



