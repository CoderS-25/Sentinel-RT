#ifndef DECISION_ENGINE_H
#define DECISION_ENGINE_H

/**
 * @file    decision_engine.h
 * @brief   Sentinel-RT Decision Engine — Priority actuation & power management.
 *
 * Consumes the inference output and applies priority changes via tk_chg_pri()
 * and power state transitions via the STM32 HAL PWR driver.
 * Includes safety filters (hysteresis, rate limiting, priority ceilings).
 */

#include <tk/tkernel.h>

/** μT-Kernel task priority for the decision engine task */
#define DECISION_PRIORITY  5

/** Maximum number of managed application tasks */
#define MAX_MANAGED_TASKS  5

/** Minimum interval between consecutive priority changes (ms) */
#define PRIORITY_CHANGE_COOLDOWN_MS  50

/** Hysteresis threshold: power state won't change unless the
 *  confidence delta exceeds this value (prevents thrashing) */
#define POWER_HYSTERESIS_THRESHOLD   0.15f

/** Minimum allowed task priority (highest urgency) — safety ceiling */
#define PRIORITY_CEILING   3

/** Maximum allowed task priority (lowest urgency) — safety floor */
#define PRIORITY_FLOOR     120

/**
 * @brief Register a managed task for priority actuation.
 *
 * @param index   Slot index (0 to MAX_MANAGED_TASKS-1).
 * @param tsk_id  μT-Kernel task ID.
 */
void sentinel_decision_register_task(int index, ID tsk_id);

/**
 * @brief Initialize the decision engine.
 *
 * @param flg_id  Event flag ID for synchronization with the inference engine.
 * @return ER  E_OK on success, error code otherwise.
 */
ER sentinel_decision_init(ID flg_id);

#endif /* DECISION_ENGINE_H */
