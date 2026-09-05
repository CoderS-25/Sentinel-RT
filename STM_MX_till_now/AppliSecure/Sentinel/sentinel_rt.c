/**
 * @file sentinel_rt.c
 * @brief Master initialization for Sentinel-RT
 */
#include "sentinel_rt.h"
#include <stdio.h>

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

    printf("[Sentinel-RT v%d.%d.%d] AI-DRS Middleware Initialized\n",
           SENTINEL_RT_VERSION_MAJOR,
           SENTINEL_RT_VERSION_MINOR,
           SENTINEL_RT_VERSION_PATCH);
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
