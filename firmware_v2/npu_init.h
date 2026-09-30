#ifndef NPU_INIT_H
#define NPU_INIT_H

/**
 * @file    npu_init.h
 * @brief   Neural-ART NPU hardware initialization for STM32N6570-DK.
 *
 * Enables the NPU clocks and verifies the accelerator is responsive
 * before the inference engine attempts to load a model.
 *
 * Reference:
 *   - STM32N6570-DK Data Brief (DB5351)
 *   - STM32N6-GettingStarted-ImageClassification (GitHub)
 */

#include <stdint.h>

/**
 * @brief  Initialize Neural-ART NPU clocks and verify readiness.
 * @retval 0  Success
 * @retval -1 NPU not responding (clock or power issue)
 */
int npu_hardware_init(void);

/**
 * @brief  De-initialize NPU (disable clocks to save power).
 */
void npu_hardware_deinit(void);

#endif /* NPU_INIT_H */
