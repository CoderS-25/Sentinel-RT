/**
 * @file sentinel_rt.h
 * @brief Main public header for Sentinel-RT AI-DRS Middleware
 */
#ifndef SENTINEL_RT_H
#define SENTINEL_RT_H

#include "profiler/task_profiler.h"
#include "inference/inference_engine.h"
#include "decision/decision_engine.h"

#define SENTINEL_RT_VERSION_MAJOR 0
#define SENTINEL_RT_VERSION_MINOR 1
#define SENTINEL_RT_VERSION_PATCH 0

/**
 * @brief Master init that starts all 3 components
 */
void sentinel_rt_init(void);

/**
 * @brief Registers an application task for monitoring
 * @param task_id The uT-Kernel task ID
 * @param name Task name for logging/debugging
 */
void sentinel_rt_register_task(ID task_id, const char *name);

#endif /* SENTINEL_RT_H */
