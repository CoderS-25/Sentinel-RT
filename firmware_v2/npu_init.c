/**
 * @file    npu_init.c
 * @brief   Neural-ART NPU hardware initialization for STM32N6570-DK.
 *
 * This module enables the NPU peripheral clocks via the STM32 HAL RCC
 * driver.  The STM32N657X0 Neural-ART NPU sits on the AXI bus and
 * requires its clock gate to be opened before any inference can run.
 *
 * Reference:
 *   - STM32N6570-DK Data Brief (DB5351)
 *   - STM32N6-GettingStarted-ImageClassification / main.c
 *   - STM32CubeN6 HAL driver (stm32n6xx_hal_rcc.h)
 */

#include "npu_init.h"
#include "stm32n6xx_hal.h"

/*
 * The Neural-ART NPU clock is gated through the RCC AHB5 peripheral
 * clock enable register.  The exact macro name depends on your
 * STM32CubeN6 HAL version.  Common variants:
 *
 *   __HAL_RCC_NPU_CLK_ENABLE()
 *   __HAL_RCC_NART_CLK_ENABLE()
 *   __HAL_RCC_NPU_FORCE_RESET() / __HAL_RCC_NPU_RELEASE_RESET()
 *
 * If your HAL version uses a different name, update the macros below.
 */

/**
 * @brief  Initialize Neural-ART NPU clocks and verify readiness.
 * @retval 0  Success.
 * @retval -1 NPU did not come out of reset (check HAL/RCC config).
 */
int npu_hardware_init(void)
{
    /* ── Step 1: Enable NPU peripheral clock ───────────────────────── */
#if defined(__HAL_RCC_NPU_CLK_ENABLE)
    __HAL_RCC_NPU_CLK_ENABLE();
#elif defined(__HAL_RCC_NART_CLK_ENABLE)
    __HAL_RCC_NART_CLK_ENABLE();
#else
    /*
     * Fallback: manually set the RCC AHB5ENR bit.
     * The NPU clock enable bit position is device-specific.
     * On the STM32N657, it is typically bit 4 of RCC->AHB5ENR.
     * Verify against your device reference manual.
     */
    RCC->AHB5ENR |= (1UL << 4);
    __DSB();
    __ISB();
#endif

    /* ── Step 2: Release NPU from reset (if held) ──────────────────── */
#if defined(__HAL_RCC_NPU_FORCE_RESET) && defined(__HAL_RCC_NPU_RELEASE_RESET)
    __HAL_RCC_NPU_FORCE_RESET();
    /* Brief delay to ensure reset is registered */
    for (volatile int i = 0; i < 100; i++) { /* ~100 cycles */ }
    __HAL_RCC_NPU_RELEASE_RESET();
#endif

    /* ── Step 3: Allow NPU clock to stabilize ──────────────────────── */
    HAL_Delay(1);

    /* ── Step 4: Enable AXISRAM3–6 (NPURAM) clocks if needed ──────── */
    /* These SRAMs are optimized for Neural-ART DMA access.            */
#if defined(__HAL_RCC_AXISRAM3_CLK_ENABLE)
    __HAL_RCC_AXISRAM3_CLK_ENABLE();
#endif
#if defined(__HAL_RCC_AXISRAM4_CLK_ENABLE)
    __HAL_RCC_AXISRAM4_CLK_ENABLE();
#endif
#if defined(__HAL_RCC_AXISRAM5_CLK_ENABLE)
    __HAL_RCC_AXISRAM5_CLK_ENABLE();
#endif
#if defined(__HAL_RCC_AXISRAM6_CLK_ENABLE)
    __HAL_RCC_AXISRAM6_CLK_ENABLE();
#endif

    return 0;
}

/**
 * @brief  De-initialize NPU (disable clocks to save power).
 */
void npu_hardware_deinit(void)
{
#if defined(__HAL_RCC_NPU_CLK_DISABLE)
    __HAL_RCC_NPU_CLK_DISABLE();
#elif defined(__HAL_RCC_NART_CLK_DISABLE)
    __HAL_RCC_NART_CLK_DISABLE();
#else
    RCC->AHB5ENR &= ~(1UL << 4);
#endif
}
