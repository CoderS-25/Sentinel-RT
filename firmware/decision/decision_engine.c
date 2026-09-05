#include "decision_engine.h"
#include "../inference/inference_engine.h"

// TODO: Include actual hardware headers for power modes
// #include "stm32n6xx_hal.h"

/**
 * @brief Enter Light Sleep Mode (TODO: hardware stub)
 */
static void enter_light_sleep(void) {
    // TODO: HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

/**
 * @brief Enter Deep Sleep Mode (TODO: hardware stub)
 */
static void enter_deep_sleep(void) {
    // TODO: HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
}

/**
 * @brief The decision engine task entry point.
 * 
 * @param stacd Task start code.
 * @param exinf Extended information.
 */
static void decision_task(INT stacd, void *exinf) {
    UINT flgptn;
    SentinelDecision decision;
    
    while (1) {
        // Wait for inference engine signal (bit 0x02)
        tk_wai_flg(sync_flg_id, 0x02, TW_CLR | TW_AND, &flgptn, TMO_FEVR);

        // Get latest decision
        sentinel_inference_get_decision(&decision);
        
        // Apply task priorities (assuming task IDs 1 to 5 for the 5 tasks)
        for (int i = 0; i < 5; i++) {
            // Priority is expected to be a valid tkernel priority (e.g., 1 to 140)
            // Scale or map the float output to actual priority
            PRI pri = (PRI)decision.task_priorities[i];
            if (pri > 0) {
                // TODO: Call tk_chg_pri for the specific task ID
                // tk_chg_pri(task_id_array[i], pri);
            }
        }
        
        // Determine power state from softmax
        int best_pwr = 0;
        float max_p = decision.power_state[0];
        for (int i = 1; i < 3; i++) {
            if (decision.power_state[i] > max_p) {
                max_p = decision.power_state[i];
                best_pwr = i;
            }
        }
        
        // Apply power state
        if (best_pwr == 1) {
            enter_light_sleep();
        } else if (best_pwr == 2) {
            enter_deep_sleep();
        }
        // Active state (0) requires no action
    }
    tk_ext_tsk();
}

static ID sync_flg_id;

/**
 * @brief Initialize the decision engine.
 * 
 * @param flg_id The event flag ID used to synchronize components.
 * @return ER E_OK on success, error code otherwise.
 */
ER sentinel_decision_init(ID flg_id) {
    sync_flg_id = flg_id;
    T_CTSK ctsk = {0};
    ctsk.tskatr = TA_HLNG | TA_RNG3;
    ctsk.task = decision_task;
    ctsk.itskpri = DECISION_PRIORITY;
    ctsk.stksz = 1024;
    ID tsk_id = tk_cre_tsk(&ctsk);
    if (tsk_id < E_OK) return tsk_id;
    return tk_sta_tsk(tsk_id, 0);
}
