/**
 * @file decision_engine.h
 * @brief Decision Engine component (Hands) of Sentinel-RT
 * 
 * Applies the NN output via tk_chg_pri() and power mode switching.
 */
#ifndef SENTINEL_DECISION_ENGINE_H
#define SENTINEL_DECISION_ENGINE_H

#include <stdint.h>
/* Placeholder for uT-Kernel definitions */
#include <tk/tkernel.h>
#include "../inference/inference_engine.h"

#define DECISION_PRIORITY 6
#define DECISION_STACK_SIZE 256

/* Maps index to actual uT-Kernel task IDs */
extern ID monitored_tasks[MAX_TASKS];

/**
 * @brief Initializes the decision engine
 */
void sentinel_decision_init(void);

/**
 * @brief Applies the decision from the inference engine
 * @param decision Pointer to the decision object
 */
void sentinel_decision_apply(const SentinelDecision *decision);

/**
 * @brief uT-Kernel task entry function
 * @param stacd Task start code
 * @param exinf Extended information
 */
void sentinel_decision_task_entry(INT stacd, void *exinf);

#endif /* SENTINEL_DECISION_ENGINE_H */
