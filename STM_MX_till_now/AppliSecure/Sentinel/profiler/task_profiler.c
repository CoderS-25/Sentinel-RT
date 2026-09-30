/**
 * @file    task_profiler.c
 * @brief   Sentinel-RT Task Profiler — Real μT-Kernel 3.0 telemetry.
 *
 * Replaces the original scaffold that used memset(0) as a placeholder.
 * This version calls tk_ref_tsk() for each registered application task
 * to gather actual execution statistics, then normalizes them into
 * the 26-float feature vector consumed by the inference engine.
 *
 * Feature vector layout (per task, 5 features):
 *   0: Normalized execution time   (tskstat mapped to [0,1])
 *   1: CPU utilization estimate    (running time fraction)
 *   2: Ready-queue latency         (time in ready state)
 *   3: I/O burst ratio             (wait time fraction)
 *   4: Laxity / slack estimate     (priority-inverse proxy)
 *   [25]: Global CPU utilization   (sum of per-task utilizations)
 *
 * Reference:
 *   - μT-Kernel 3.0 Specification: tk_ref_tsk(), T_RTSK
 *   - firmware/profiler/task_profiler.c (original scaffold)
 */

#include "task_profiler.h"
#include <string.h>

/* ── Module state ────────────────────────────────────────────────────── */

/** Feature vector shared with the inference engine */
static float feature_vector[FEATURE_VECTOR_SIZE];

/** Event flag ID for signaling the inference engine */
static ID sync_flg_id;

/** Registered task IDs to monitor (set via sentinel_profiler_register_task) */
static ID monitored_tasks[TASK_COUNT] = {0};

/** Previous cycle's execution tick counts (for delta computation) */
static RELTIM prev_exec_ticks[TASK_COUNT] = {0};

/** System tick at last profiling cycle */
static RELTIM prev_system_tick = 0;

/* ── Helper: Map μT-Kernel task state to a numeric score ─────────────── */

/**
 * @brief Convert a task state bitmask to a normalized [0,1] activity score.
 *
 * Higher score = more actively running.
 *   TTS_RUN = 1.0, TTS_RDY = 0.75, TTS_WAI = 0.25, TTS_SUS/DMT = 0.0
 */
static float task_state_to_score(UINT tskstat)
{
    if (tskstat & TTS_RUN) return 1.0f;
    if (tskstat & TTS_RDY) return 0.75f;
    if (tskstat & TTS_WAI) return 0.25f;
    return 0.0f; /* TTS_SUS, TTS_DMT, or unknown */
}

/* ── Core profiling function ─────────────────────────────────────────── */

/**
 * @brief Sample all registered tasks and update the feature vector.
 */
static void sample_task_metrics(void)
{
    T_RTSK rtsk;
    SYSTIM tim;
    tk_get_tim(&tim);
    RELTIM now = tim.lo;  /* Current system tick (lower 32 bits) */
    RELTIM elapsed = (prev_system_tick > 0) ? (now - prev_system_tick) : 1;
    if (elapsed == 0) elapsed = 1;  /* Avoid division by zero */

    float global_cpu = 0.0f;

    for (int i = 0; i < TASK_COUNT; i++) {
        int base = i * FEATURES_PER_TASK;

        if (monitored_tasks[i] == 0) {
            /* No task registered for this slot — zero features */
            for (int j = 0; j < FEATURES_PER_TASK; j++) {
                feature_vector[base + j] = 0.0f;
            }
            continue;
        }

        ER err = tk_ref_tsk(monitored_tasks[i], &rtsk);
        if (err < E_OK) {
            /* Task reference failed (might be deleted) — zero features */
            for (int j = 0; j < FEATURES_PER_TASK; j++) {
                feature_vector[base + j] = 0.0f;
            }
            continue;
        }

        /* Feature 0: Task state activity score [0, 1] */
        feature_vector[base + 0] = task_state_to_score(rtsk.tskstat);

        /*
         * Feature 1: CPU utilization estimate.
         * μT-Kernel 3.0 does not always provide per-task CPU time
         * counters directly via tk_ref_tsk.  We approximate using
         * the task state: a RUN/RDY task is assumed to consume CPU.
         *
         * If your BSP provides hardware cycle counters (DWT->CYCCNT),
         * replace this with actual measurements.
         */
        float cpu_est = (rtsk.tskstat & (TTS_RUN | TTS_RDY)) ? 0.5f : 0.0f;
        feature_vector[base + 1] = cpu_est;
        global_cpu += cpu_est;

        /*
         * Feature 2: Ready-queue latency proxy.
         * Use the task's current waiting factor count (wupcnt) or
         * ready state as a proxy.  A high wakeup count with no
         * execution suggests queue congestion.
         */
        feature_vector[base + 2] = (float)rtsk.wupcnt / 10.0f;
        if (feature_vector[base + 2] > 1.0f) {
            feature_vector[base + 2] = 1.0f;
        }

        /*
         * Feature 3: I/O burst ratio — fraction of time waiting.
         * Approximated by whether the task is in WAI state.
         */
        feature_vector[base + 3] = (rtsk.tskstat & TTS_WAI) ? 0.8f : 0.1f;

        /*
         * Feature 4: Laxity / priority-inverse proxy.
         * Higher priority (lower number) → less laxity → higher urgency.
         * Normalize: 1.0 - (priority / max_priority).
         * μT-Kernel priority range is typically 1 (highest) to 140 (lowest).
         */
        float pri_norm = 1.0f - ((float)rtsk.tskpri / 140.0f);
        if (pri_norm < 0.0f) pri_norm = 0.0f;
        if (pri_norm > 1.0f) pri_norm = 1.0f;
        feature_vector[base + 4] = pri_norm;
    }

    /* Feature 25: Global CPU utilization (clamped to [0, 1]) */
    if (global_cpu > 1.0f) global_cpu = 1.0f;
    feature_vector[FEATURE_VECTOR_SIZE - 1] = global_cpu;

    prev_system_tick = now;
}

/* ── RTOS task ───────────────────────────────────────────────────────── */

/**
 * @brief The profiler μT-Kernel task entry point.
 *
 * Periodically samples task metrics and signals the inference engine.
 */
static void profiler_task(INT stacd, void *exinf)
{
    while (1) {
        /* Gather actual task metrics */
        sample_task_metrics();

        /* Signal the Inference Engine that a new vector is ready (bit 0x01) */
        tk_set_flg(sync_flg_id, 0x01);

        /* Sleep for the profiling period */
        tk_dly_tsk(PROFILER_INTERVAL_MS);
    }
    tk_ext_tsk();
}

/* ── Public API ──────────────────────────────────────────────────────── */

/**
 * @brief Register a task ID for monitoring.
 */
void sentinel_profiler_register_task(int index, ID tsk_id)
{
    if (index >= 0 && index < TASK_COUNT) {
        monitored_tasks[index] = tsk_id;
    }
}

/**
 * @brief Initialize the task profiler.
 */
ER sentinel_profiler_init(ID flg_id)
{
    sync_flg_id = flg_id;

    /* Clear initial feature vector */
    memset(feature_vector, 0, sizeof(feature_vector));

    /* Create the profiler RTOS task */
    T_CTSK ctsk   = {0};
    ctsk.tskatr   = TA_HLNG | TA_RNG3;
    ctsk.task     = profiler_task;
    ctsk.itskpri  = PROFILER_PRIORITY;
    ctsk.stksz    = 1024;

    ID tsk_id = tk_cre_tsk(&ctsk);
    if (tsk_id < E_OK) return tsk_id;

    return tk_sta_tsk(tsk_id, 0);
}

/**
 * @brief Get the latest feature vector.
 */
void sentinel_profiler_get_features(float *vector)
{
    memcpy(vector, feature_vector, sizeof(feature_vector));
}
