/**
 * @file sentinel_rt.c
 * @brief Master initialization for Sentinel-RT
 */
#include "sentinel_rt.h"
#include "usart.h"

ID sentinel_event_flg = 0;
SentinelFeatureVector current_features;
SentinelDecision current_decision;

/**
 * @brief Master init that starts all 3 components
 */
void sentinel_rt_init(void) {
    /* Initialize shared resources */
    T_CFLG cflg = {0};
    cflg.flgatr = TA_TFIFO | TA_WMUL;
    cflg.iflgptn = 0;
    sentinel_event_flg = tk_cre_flg(&cflg);

    /* Start components in order */
    sentinel_profiler_init();
    sentinel_inference_init();
    sentinel_decision_init();

    /* Safe UART print — no heap allocation, no printf */
    extern UART_HandleTypeDef huart3;
    HAL_UART_Transmit(&huart3,
        (uint8_t*)"[Sentinel-RT v0.1.0] AI-DRS Ready\r\n", 37, 200);
}

/**
 * @brief Registers an application task for monitoring
 */
void sentinel_rt_register_task(ID task_id, const char *name) {
    (void)name; /* Name unused for now, kept for API compatibility */
    for (int i = 0; i < MAX_TASKS; ++i) {
        if (monitored_tasks[i] == 0) {
            monitored_tasks[i] = task_id;
            break;
        }
    }
}
