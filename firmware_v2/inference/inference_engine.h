#ifndef INFERENCE_ENGINE_H
#define INFERENCE_ENGINE_H

/**
 * @file    inference_engine.h
 * @brief   Sentinel-RT Inference Engine — X-CUBE-AI / Neural-ART integration.
 *
 * Provides the interface between the Task Profiler (which produces 26-float
 * feature vectors) and the Decision Engine (which consumes priority and
 * power-state predictions).
 *
 * The inference runs on the Neural-ART NPU via the X-CUBE-AI runtime,
 * with quantized INT8 tensors.
 */

#include <tk/tkernel.h>

/** μT-Kernel task priority for the inference engine task */
#define INFERENCE_PRIORITY 6

/** Stack size for inference task — needs room for activation arena */
#define INFERENCE_STACK_SIZE  4096

/**
 * @brief Output of the dual-head neural network.
 *
 * task_priorities[5]:  Predicted optimal priority weights for 5 tasks.
 *                      Higher value = higher urgency.
 * power_state[3]:      Softmax probabilities for [Active, LowPower, Sleep].
 */
typedef struct {
    float task_priorities[5];
    float power_state[3];
} SentinelDecision;

/**
 * @brief Initialize the inference engine.
 *
 * This function:
 *   1. Creates the X-CUBE-AI network instance.
 *   2. Allocates the activation buffer (arena).
 *   3. Spawns the inference RTOS task.
 *
 * @param flg_id  Event flag ID shared with the profiler and decision engine.
 * @return ER  E_OK on success, error code otherwise.
 */
ER sentinel_inference_init(ID flg_id);

/**
 * @brief Get the latest decision output from the inference engine.
 *
 * Thread-safe: copies the most recent inference result into the caller's buffer.
 *
 * @param decision  Pointer to store the decision.
 */
void sentinel_inference_get_decision(SentinelDecision* decision);

#endif /* INFERENCE_ENGINE_H */
