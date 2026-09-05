/**
 * @file inference_engine.h
 * @brief NN Inference Engine component (Brain) of Sentinel-RT
 * 
 * Runs a neural network on the Neural-ART NPU.
 */
#ifndef SENTINEL_INFERENCE_ENGINE_H
#define SENTINEL_INFERENCE_ENGINE_H

#include <stdint.h>
/* Placeholder for uT-Kernel definitions */
#include <tk/tkernel.h>
#include "../profiler/task_profiler.h"

#define INFERENCE_PRIORITY 6
#define INFERENCE_STACK_SIZE 2048

/**
 * @struct SentinelDecision
 * @brief Holds the output decision from the NPU inference
 */
typedef struct {
    int8_t task_priorities[MAX_TASKS];
    uint8_t power_state; /* 0=Active, 1=LightSleep, 2=DeepSleep */
    float confidence;
} SentinelDecision;

/**
 * @brief Initializes the inference engine and loads the model on NPU
 */
void sentinel_inference_init(void);

/**
 * @brief Runs inference
 * @param input Feature vector input
 * @param output Decision vector output
 */
void sentinel_inference_run(const SentinelFeatureVector *input, SentinelDecision *output);

/**
 * @brief uT-Kernel task entry function
 * @param stacd Task start code
 * @param exinf Extended information
 */
void sentinel_inference_task_entry(INT stacd, void *exinf);

#endif /* SENTINEL_INFERENCE_ENGINE_H */
