/**
 * @file    decision_engine.c
 * @brief   Sentinel-RT Decision Engine — Priority actuation & power management.
 *
 * Replaces the original scaffold that had TODO stubs and a misplaced
 * static variable declaration.  This version:
 *
 *   1. Actually calls tk_chg_pri() with safety-bounded priorities.
 *   2. Uses HAL PWR functions for real power state transitions.
 *   3. Implements hysteresis filtering to prevent power state thrashing.
 *   4. Rate-limits priority changes to protect system stability.
 *   5. Logs decisions to UART for debugging.
 *
 * Reference:
 *   - μT-Kernel 3.0 Specification: tk_chg_pri()
 *   - STM32N6xx HAL PWR driver
 *   - firmware/decision/decision_engine.c (original scaffold)
 */

#include "decision_engine.h"
#include "../inference/inference_engine.h"
#include "stm32n6xx_hal.h"
#include <string.h>
#include <stdio.h>

/* ── Module state (declared BEFORE use — fixed from original) ────────── */

/** Event flag ID for synchronization with the inference engine */
static ID sync_flg_id;

/** Registered task IDs for priority actuation */
static ID managed_tasks[MAX_MANAGED_TASKS] = {0};

/** Previous power state (for hysteresis comparison) */
static int prev_power_state = 0;  /* 0=Active, 1=LowPower, 2=Sleep */

/** Tick count of last priority change (for rate limiting) */
static RELTIM last_priority_change_tick = 0;

/* ── UART debug helper ───────────────────────────────────────────────── */

extern UART_HandleTypeDef huart3;  /* Defined in CubeMX-generated main.c */

/**
 * @brief Print a debug message to UART3 (ST-LINK VCP).
 */
static void debug_print(const char *msg)
{
    HAL_UART_Transmit(&huart3, (const uint8_t *)msg, strlen(msg), 100);
}

/* ── Power management ────────────────────────────────────────────────── */

/**
 * @brief Enter Light Sleep Mode.
 *
 * Reduces clock frequency and enters WFI.  Wakes on any interrupt
 * (including the SysTick timer used by μT-Kernel).
 */
static void enter_light_sleep(void)
{
    debug_print("[Decision] -> Light Sleep\r\n");
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

/**
 * @brief Enter Deep Sleep / STOP Mode.
 *
 * WARNING: This stops most clocks.  Only use if the system is truly
 * idle and can tolerate wakeup latency.  The RTOS SysTick will stop
 * during STOP mode, so use with extreme caution in a real-time system.
 */
static void enter_deep_sleep(void)
{
    debug_print("[Decision] -> Deep Sleep\r\n");
    /*
     * For a hard real-time system, STOP mode is dangerous because
     * SysTick stops and task scheduling halts.  We use a conservative
     * approach: only reduce to SLEEP, not full STOP.
     *
     * Uncomment the line below ONLY if your application can handle
     * the wakeup latency and clock re-initialization:
     *
     * HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
     */
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

/* ── Priority mapping ────────────────────────────────────────────────── */

/**
 * @brief Map a neural network priority output (0.0–1.0 range) to a
 *        valid μT-Kernel priority, clamped by safety ceiling/floor.
 *
 * The network outputs a float where higher value = higher urgency.
 * μT-Kernel uses lower number = higher priority.
 * So we invert: priority = FLOOR - (value * (FLOOR - CEILING))
 *
 * @param nn_priority  Network output [0.0, 1.0].
 * @return Clamped μT-Kernel priority [PRIORITY_CEILING, PRIORITY_FLOOR].
 */
static PRI map_nn_priority_to_tkernel(float nn_priority)
{
    /* Clamp input to [0, 1] */
    if (nn_priority < 0.0f) nn_priority = 0.0f;
    if (nn_priority > 1.0f) nn_priority = 1.0f;

    /* Invert: high NN value → low μT-Kernel priority number → high urgency */
    float range = (float)(PRIORITY_FLOOR - PRIORITY_CEILING);
    PRI pri = PRIORITY_FLOOR - (PRI)(nn_priority * range);

    /* Safety clamp */
    if (pri < PRIORITY_CEILING) pri = PRIORITY_CEILING;
    if (pri > PRIORITY_FLOOR)   pri = PRIORITY_FLOOR;

    return pri;
}

/* ── RTOS task ───────────────────────────────────────────────────────── */

/**
 * @brief The decision engine μT-Kernel task entry point.
 *
 * Waits for the inference engine to produce a new decision, then:
 *   1. Applies rate-limited priority changes to managed tasks.
 *   2. Determines the optimal power state with hysteresis filtering.
 *   3. Transitions the hardware power mode if needed.
 */
static void decision_task(INT stacd, void *exinf)
{
    UINT flgptn;
    SentinelDecision decision;
    char dbg_buf[128];

    while (1) {
        /* Wait for inference engine signal (bit 0x02) */
        tk_wai_flg(sync_flg_id, 0x02, TWF_CLR | TWF_ANDW, &flgptn, TMO_FEVR);

        /* Get the latest inference output */
        sentinel_inference_get_decision(&decision);

        /* ── Step 1: Apply task priorities (rate-limited) ────────── */
        SYSTIM tim;
        tk_get_tim(&tim);
        RELTIM now = tim.lo;
        RELTIM elapsed = (last_priority_change_tick > 0)
                         ? (now - last_priority_change_tick) : 0xFFFFFFFF;

        if (elapsed >= PRIORITY_CHANGE_COOLDOWN_MS) {
            for (int i = 0; i < MAX_MANAGED_TASKS; i++) {
                if (managed_tasks[i] == 0) continue;

                PRI new_pri = map_nn_priority_to_tkernel(decision.task_priorities[i]);
                ER err = tk_chg_pri(managed_tasks[i], new_pri);

                if (err == E_OK) {
                    snprintf(dbg_buf, sizeof(dbg_buf),
                             "[Decision] Task[%d] pri->%d (nn=%.2f)\r\n",
                             i, new_pri, decision.task_priorities[i]);
                    debug_print(dbg_buf);
                }
                /* Ignore errors (task may have exited, etc.) */
            }
            last_priority_change_tick = now;
        }

        /* ── Step 2: Determine optimal power state (with hysteresis) ── */
        int best_pwr = 0;
        float max_p = decision.power_state[0];
        for (int i = 1; i < 3; i++) {
            if (decision.power_state[i] > max_p) {
                max_p = decision.power_state[i];
                best_pwr = i;
            }
        }

        /*
         * Hysteresis filter:
         * Only change power state if the winning class confidence
         * exceeds the runner-up by at least POWER_HYSTERESIS_THRESHOLD.
         * This prevents rapid oscillation between states.
         */
        float second_best = 0.0f;
        for (int i = 0; i < 3; i++) {
            if (i != best_pwr && decision.power_state[i] > second_best) {
                second_best = decision.power_state[i];
            }
        }

        if ((max_p - second_best) < POWER_HYSTERESIS_THRESHOLD) {
            /* Confidence gap too small — keep previous state */
            best_pwr = prev_power_state;
        }

        /* ── Step 3: Apply power state transition ────────────────── */
        if (best_pwr != prev_power_state) {
            snprintf(dbg_buf, sizeof(dbg_buf),
                     "[Decision] Power: %d->%d (conf=%.2f)\r\n",
                     prev_power_state, best_pwr, max_p);
            debug_print(dbg_buf);

            switch (best_pwr) {
                case 0:
                    /* Active — no power reduction needed */
                    debug_print("[Decision] -> Active\r\n");
                    break;
                case 1:
                    enter_light_sleep();
                    break;
                case 2:
                    enter_deep_sleep();
                    break;
                default:
                    break;
            }
            prev_power_state = best_pwr;
        }
    }
    tk_ext_tsk();
}

/* ── Public API ──────────────────────────────────────────────────────── */

/**
 * @brief Register a managed task for priority actuation.
 */
void sentinel_decision_register_task(int index, ID tsk_id)
{
    if (index >= 0 && index < MAX_MANAGED_TASKS) {
        managed_tasks[index] = tsk_id;
    }
}

/**
 * @brief Initialize the decision engine.
 */
ER sentinel_decision_init(ID flg_id)
{
    sync_flg_id = flg_id;
    prev_power_state = 0;
    last_priority_change_tick = 0;

    T_CTSK ctsk   = {0};
    ctsk.tskatr   = TA_HLNG | TA_RNG3;
    ctsk.task     = decision_task;
    ctsk.itskpri  = DECISION_PRIORITY;
    ctsk.stksz    = 2048;  /* Increased for snprintf + debug output */

    ID tsk_id = tk_cre_tsk(&ctsk);
    if (tsk_id < E_OK) return tsk_id;

    return tk_sta_tsk(tsk_id, 0);
}
