#include "sentinel_rt.h"
#include "profiler/task_profiler.h"
#include "inference/inference_engine.h"
#include "decision/decision_engine.h"

// TODO: Include actual hardware headers for UART
// #include "stm32n6xx_hal.h"

/**
 * @brief Stub for HAL_UART_Transmit (TODO: hardware stub)
 */
static void uart_transmit(const char* msg) {
    // TODO: HAL_UART_Transmit(...)
}

/**
 * @brief Top-level orchestrator initialization for Sentinel-RT.
 * @return ER E_OK on success, error code otherwise.
 */
ER sentinel_rt_init(void) {
    // 1. Create an event flag for inter-component signaling
    T_CFLG cflg = {0};
    cflg.flgatr = TA_TFIFO | TA_WMUL;
    cflg.iflgptn = 0;
    ID flg_id = tk_cre_flg(&cflg);
    if (flg_id < E_OK) {
        return flg_id;
    }

    // 2. Initialize the Task Profiler
    ER err = sentinel_profiler_init(flg_id);
    if (err < E_OK) return err;

    // 3. Initialize the Inference Engine
    err = sentinel_inference_init(flg_id);
    if (err < E_OK) return err;

    // 4. Initialize the Decision Engine
    err = sentinel_decision_init(flg_id);
    if (err < E_OK) return err;

    // 5. Print initialization message
    uart_transmit("[Sentinel-RT] All systems GO!\r\n");

    return E_OK;
}
