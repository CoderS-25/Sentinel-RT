/**
 * @file task_profiler.h
 * @brief Task Profiler component (Eyes) of Sentinel-RT
 * 
 * Monitors all running tasks and builds a feature vector.
 */
#ifndef SENTINEL_TASK_PROFILER_H
#define SENTINEL_TASK_PROFILER_H

#include <stdint.h>
/* Placeholder for uT-Kernel definitions */
#include <tk/tkernel.h>

#define MAX_TASKS 5
#define FEATURE_COUNT 26
#define PROFILER_INTERVAL_MS 5
#define PROFILER_PRIORITY 7
#define PROFILER_STACK_SIZE 512

/**
 * @struct SentinelFeatureVector
 * @brief Holds feature extraction data for the AI inference engine
 */
typedef struct {
    struct {
        float cpu_load;
        float wait_time_ms;
        float deadline_proximity;
        float context_switch_rate;
        float is_blocked;
    } tasks[MAX_TASKS];
    float total_system_cpu_load;
} SentinelFeatureVector;

/**
 * @brief Initializes the profiler as a uT-Kernel task
 */
void sentinel_profiler_init(void);

/**
 * @brief Fills the feature vector
 * @param out Pointer to the feature vector to populate
 */
void sentinel_profiler_get_features(SentinelFeatureVector *out);

/**
 * @brief uT-Kernel task entry function
 * @param stacd Task start code
 * @param exinf Extended information
 */
void sentinel_profiler_task_entry(INT stacd, void *exinf);

#endif /* SENTINEL_TASK_PROFILER_H */
