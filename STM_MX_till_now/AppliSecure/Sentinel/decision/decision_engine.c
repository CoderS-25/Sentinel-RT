/**
 * @file decision_engine.c
 * @brief Implementation of the Decision Engine component (Hands)
 */
#include "decision_engine.h"
#include <stdio.h>
#include "stm32n6xx_hal.h"

extern ID sentinel_event_flg;
extern SentinelDecision current_decision;
ID monitored_tasks[MAX_TASKS] = {0};

/**
 * @brief Initializes the decision engine
 */
void sentinel_decision_init(void) {
    T_CTSK ctsk = {0};
    ctsk.tskatr = TA_HLNG | TA_RNG0;
    ctsk.task = sentinel_decision_task_entry;
    ctsk.itskpri = DECISION_PRIORITY;
    ctsk.stksz = DECISION_STACK_SIZE;

    ID tskid = tk_cre_tsk(&ctsk);
    if (tskid > 0) {
        tk_sta_tsk(tskid, 0);
    }
}

/**
 * @brief uT-Kernel task entry function
 */
void sentinel_decision_task_entry(INT stacd, void *exinf) {
    (void)stacd;
    (void)exinf;
    UINT flgptn;

    while (1) {
        /* Wait on event flag from inference engine */
        tk_wai_flg(sentinel_event_flg, 0x02, TWF_ANDW | TWF_BITCLR, &flgptn, TMO_FEVR);

        /* Apply decisions */
        sentinel_decision_apply(&current_decision);
    }
}

/**
 * @brief Applies the decision from the inference engine
 */
void sentinel_decision_apply(const SentinelDecision *decision) {
    if (!decision) return;

    for (int i = 0; i < MAX_TASKS; ++i) {
        if (monitored_tasks[i] != 0 && decision->task_priorities[i] > 0) {
            tk_chg_pri(monitored_tasks[i], decision->task_priorities[i]);
            printf("[Sentinel-RT] Task %d: priority -> %d\n", monitored_tasks[i], decision->task_priorities[i]);
        }
    }

    switch (decision->power_state) {
        case 0:
            /* Stay in Run mode (do nothing) */
            break;
        case 1:
            HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
            break;
        case 2:
            HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
            break;
        default:
            break;
    }
}
