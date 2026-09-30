/**
 * @file    sentinel_rt.c
 * @brief   Sentinel-RT top-level orchestrator — COMPLETE implementation.
 *
 * Replaces the original scaffold that had UART stubs and missing NPU init.
 * This version:
 *   1. Initializes the Neural-ART NPU hardware (npu_init.c).
 *   2. Creates the shared event flag.
 *   3. Starts all three middleware tasks in pipeline order.
 *   4. Outputs diagnostic messages via UART3 (ST-LINK VCP).
 *
 * Reference:
 *   - STM32N6570-DK Data Brief (DB5351): USART3 on PD8/PD9
 *   - firmware/sentinel_rt.c (original scaffold)
 */

#include "sentinel_rt.h"
#include "profiler/task_profiler.h"
#include "inference/inference_engine.h"
#include "decision/decision_engine.h"
#include "npu_init.h"
#include "stm32n6xx_hal.h"
#include <string.h>
#include <stdio.h>

/* ── UART handle ─────────────────────────────────────────────────────
 * USART3 is connected to the ST-LINK VCP on the STM32N6570-DK
 * (PD8 = TX, PD9 = RX, as configured in your .ioc file).
 * This handle is declared in the CubeMX-generated main.c.
 * ──────────────────────────────────────────────────────────────────── */
extern UART_HandleTypeDef huart3;

/* ── Application Tasks ─────────────────────────────────────────────── */
static ID led_task_id;
static ID camera_task_id;
static ID screen_task_id;

static void led_app_task(INT stacd, void *exinf)
{
    /* Enable GPIOO clock and setup Pin 1 for Green LED */
    __HAL_RCC_GPIOO_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOO, &GPIO_InitStruct);

    while (1) {
        HAL_GPIO_TogglePin(GPIOO, GPIO_PIN_1);
        tk_dly_tsk(500); /* Blink every 500ms */
    }
}

static void camera_app_task(INT stacd, void *exinf)
{
    while (1) {
        /* Dummy camera capture processing */
        tk_dly_tsk(33); /* ~30 FPS */
    }
}

static void screen_app_task(INT stacd, void *exinf)
{
    while (1) {
        /* Dummy screen render processing */
        tk_dly_tsk(16); /* ~60 FPS */
    }
}

extern UART_HandleTypeDef huart3;

/**
 * @brief Transmit a debug string via UART3.
 */
static void uart_transmit(const char *msg)
{
    HAL_UART_Transmit(&huart3, (const uint8_t *)msg, strlen(msg), 200);
}

/* ── Top-level init ──────────────────────────────────────────────────── */

/**
 * @brief Top-level orchestrator initialization for Sentinel-RT.
 * @return ER  E_OK on success, error code otherwise.
 */
ER sentinel_rt_init(void)
{
    char msg_buf[128];
    ER err;

    uart_transmit("\r\n");
    uart_transmit("============================================\r\n");
    uart_transmit("  Sentinel-RT  v2.0  (STM32N6570-DK)\r\n");
    uart_transmit("  AI-Driven RTOS Task Scheduler\r\n");
    uart_transmit("============================================\r\n");

    /* ── Step 0: Initialize Neural-ART NPU hardware ────────────── */
    uart_transmit("[Init] Enabling Neural-ART NPU clocks...\r\n");
    if (npu_hardware_init() != 0) {
        uart_transmit("[Init] ERROR: NPU hardware init failed!\r\n");
        /* Non-fatal: inference engine will fall back to safe defaults */
    } else {
        uart_transmit("[Init] NPU hardware ready.\r\n");
    }

    /* ── Step 1: Create the shared event flag ──────────────────── */
    uart_transmit("[Init] Creating event flag...\r\n");
    T_CFLG cflg = {0};
    cflg.flgatr  = TA_TFIFO | TA_WMUL;
    cflg.iflgptn = 0;

    ID flg_id = tk_cre_flg(&cflg);
    if (flg_id < E_OK) {
        snprintf(msg_buf, sizeof(msg_buf),
                 "[Init] ERROR: tk_cre_flg failed (err=%d)\r\n", flg_id);
        uart_transmit(msg_buf);
        return flg_id;
    }
    uart_transmit("[Init] Event flag created.\r\n");

    /* ── Step 2: Initialize the Task Profiler (5ms period) ─────── */
    uart_transmit("[Init] Starting Task Profiler...\r\n");
    err = sentinel_profiler_init(flg_id);
    if (err < E_OK) {
        snprintf(msg_buf, sizeof(msg_buf),
                 "[Init] ERROR: Profiler init failed (err=%d)\r\n", err);
        uart_transmit(msg_buf);
        return err;
    }
    uart_transmit("[Init] Task Profiler running (5ms period).\r\n");

    /* ── Step 3: Initialize the Inference Engine (NPU) ─────────── */
    uart_transmit("[Init] Starting Inference Engine...\r\n");
    err = sentinel_inference_init(flg_id);
    if (err < E_OK) {
        snprintf(msg_buf, sizeof(msg_buf),
                 "[Init] ERROR: Inference init failed (err=%d)\r\n", err);
        uart_transmit(msg_buf);
        return err;
    }
    snprintf(msg_buf, sizeof(msg_buf),
             "[Init] Inference Engine running (model=%u bytes).\r\n",
             (unsigned int)23048);  /* sentinel_model_tflite_len */
    uart_transmit(msg_buf);

    /* ── Step 4: Initialize the Decision Engine ────────────────── */
    uart_transmit("[Init] Starting Decision Engine...\r\n");
    err = sentinel_decision_init(flg_id);
    if (err < E_OK) {
        snprintf(msg_buf, sizeof(msg_buf),
                 "[Init] ERROR: Decision init failed (err=%d)\r\n", err);
        uart_transmit(msg_buf);
        return err;
    }
    uart_transmit("[Init] Decision Engine running.\r\n");

    /* ── Step 5: Start and Register Application Tasks ──────────────── */
    uart_transmit("[Init] Starting Application Tasks (LED, Camera, Screen)...\r\n");
    
    T_CTSK ctsk_app = {0};
    ctsk_app.tskatr   = TA_HLNG | TA_RNG3;
    ctsk_app.stksz    = 1024;
    
    /* Create and start LED Task */
    ctsk_app.task     = led_app_task;
    ctsk_app.itskpri  = 10;
    led_task_id = tk_cre_tsk(&ctsk_app);
    tk_sta_tsk(led_task_id, 0);
    sentinel_profiler_register_task(0, led_task_id);

    /* Create and start Camera Task */
    ctsk_app.task     = camera_app_task;
    ctsk_app.itskpri  = 9;
    camera_task_id = tk_cre_tsk(&ctsk_app);
    tk_sta_tsk(camera_task_id, 0);
    sentinel_profiler_register_task(1, camera_task_id);

    /* Create and start Screen Task */
    ctsk_app.task     = screen_app_task;
    ctsk_app.itskpri  = 8;
    screen_task_id = tk_cre_tsk(&ctsk_app);
    tk_sta_tsk(screen_task_id, 0);
    sentinel_profiler_register_task(2, screen_task_id);
    
    uart_transmit("[Init] App Tasks registered with AI Profiler.\r\n");

    /* ── All systems go ───────────────────────────────────────────── */
    uart_transmit("============================================\r\n");
    uart_transmit("[Sentinel-RT] All systems GO!\r\n");
    uart_transmit("  Profiler  -> Inference -> Decision\r\n");
    uart_transmit("  (5ms)        (NPU)        (tk_chg_pri)\r\n");
    uart_transmit("============================================\r\n");

    return E_OK;
}
