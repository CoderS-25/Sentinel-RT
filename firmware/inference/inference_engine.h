#ifndef INFERENCE_ENGINE_H
#define INFERENCE_ENGINE_H

#include <tk/tkernel.h>

#define INFERENCE_PRIORITY 6

typedef struct {
    float task_priorities[5];
    float power_state[3];
} SentinelDecision;

/**
 * @brief Initialize the inference engine.
 * 
 * @param flg_id The event flag ID used to synchronize with the profiler and decision engine.
 * @return ER E_OK on success, error code otherwise.
 */
ER sentinel_inference_init(ID flg_id);

/**
 * @brief Get the latest decision output from the inference engine.
 * 
 * @param decision Pointer to store the decision.
 */
void sentinel_inference_get_decision(SentinelDecision* decision);

#endif // INFERENCE_ENGINE_H
