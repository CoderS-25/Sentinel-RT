#ifndef SENTINEL_RT_H
#define SENTINEL_RT_H

/**
 * @file    sentinel_rt.h
 * @brief   Sentinel-RT top-level API — AI-driven RTOS middleware.
 *
 * Single entry point to initialize the complete Sentinel-RT pipeline:
 *   NPU Hardware → Profiler → Inference Engine → Decision Engine
 */

#include <tk/tkernel.h>

/**
 * @brief Top-level orchestrator initialization for Sentinel-RT.
 *
 * Call this from your application's main() or usermain() after
 * μT-Kernel and HAL initialization are complete.
 *
 * This function:
 *   1. Initializes the Neural-ART NPU hardware clocks.
 *   2. Creates the inter-component event flag.
 *   3. Starts the Task Profiler (5ms periodic).
 *   4. Starts the Inference Engine (NPU-accelerated).
 *   5. Starts the Decision Engine (priority + power actuation).
 *   6. Prints a diagnostic message on UART3.
 *
 * @return ER  E_OK on success, error code otherwise.
 */
ER sentinel_rt_init(void);

#endif /* SENTINEL_RT_H */
