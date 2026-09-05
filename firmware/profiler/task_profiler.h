#ifndef TASK_PROFILER_H
#define TASK_PROFILER_H

#include <tk/tkernel.h>

#define PROFILER_INTERVAL_MS 5
#define PROFILER_PRIORITY 7
#define TASK_COUNT 5

/**
 * @brief Initialize the task profiler.
 * 
 * @param flg_id The event flag ID used to signal the inference engine.
 * @return ER E_OK on success, error code otherwise.
 */
ER sentinel_profiler_init(ID flg_id);

/**
 * @brief Get the latest feature vector.
 * 
 * @param vector Pointer to a float array of size 26.
 */
void sentinel_profiler_get_features(float* vector);

#endif // TASK_PROFILER_H
