#ifndef TASK_PROFILER_H
#define TASK_PROFILER_H

/**
 * @file    task_profiler.h
 * @brief   Sentinel-RT Task Profiler — RTOS telemetry extraction.
 *
 * Collects per-task execution metrics from μT-Kernel 3.0 and composes
 * a 26-float feature vector for the inference engine.
 *
 * Feature vector layout (5 tasks × 5 features + 1 global):
 *   [0..4]   Task 0: exec_time, cpu_util, ready_latency, io_ratio, laxity
 *   [5..9]   Task 1: ...
 *   [10..14] Task 2: ...
 *   [15..19] Task 3: ...
 *   [20..24] Task 4: ...
 *   [25]     Global: overall CPU utilization
 */

#include <tk/tkernel.h>

/** Profiler sampling period in milliseconds */
#define PROFILER_INTERVAL_MS  5

/** μT-Kernel task priority for the profiler task */
#define PROFILER_PRIORITY     7

/** Number of application tasks being monitored */
#define TASK_COUNT            5

/** Total feature vector dimension */
#define FEATURE_VECTOR_SIZE   26

/** Number of per-task features */
#define FEATURES_PER_TASK     5

/**
 * @brief Register a task ID for monitoring.
 *
 * Call this during system init for each application task you want
 * the profiler to track (up to TASK_COUNT tasks).
 *
 * @param index  Slot index (0 to TASK_COUNT-1).
 * @param tsk_id μT-Kernel task ID returned by tk_cre_tsk().
 */
void sentinel_profiler_register_task(int index, ID tsk_id);

/**
 * @brief Initialize the task profiler.
 *
 * Creates and starts the profiler RTOS task, which periodically
 * samples task metrics and signals the inference engine.
 *
 * @param flg_id  Event flag ID used to signal the inference engine.
 * @return ER  E_OK on success, error code otherwise.
 */
ER sentinel_profiler_init(ID flg_id);

/**
 * @brief Get the latest feature vector.
 *
 * Thread-safe copy of the most recently computed 26-float vector.
 *
 * @param vector  Pointer to a float array of size FEATURE_VECTOR_SIZE (26).
 */
void sentinel_profiler_get_features(float *vector);

#endif /* TASK_PROFILER_H */
