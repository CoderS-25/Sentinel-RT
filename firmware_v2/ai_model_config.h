#ifndef AI_MODEL_CONFIG_H
#define AI_MODEL_CONFIG_H

/**
 * @file    ai_model_config.h
 * @brief   X-CUBE-AI / Neural-ART model configuration for Sentinel-RT.
 *
 * This header bridges the Sentinel-RT middleware with the X-CUBE-AI
 * generated network interface.  It provides quantization parameters
 * (derived from the .ai/network_sentinel_model.tflite_c_info.json)
 * and memory-pool sizing constants.
 *
 * Reference:
 *   - STMicroelectronics X-CUBE-AI 10.2.1 API
 *   - STM32N6-GettingStarted-ImageClassification (GitHub)
 *   - ST EdgeAI-Core Tutorial (YouTube: usQ_f6Swpok)
 */

/* ────────────────────────────────────────────────────────────────────────── */
/*  Network geometry (must match stedgeai generate output)                   */
/* ────────────────────────────────────────────────────────────────────────── */

/** Input tensor: [1, 26] INT8 — 26-float feature vector, quantized */
#define SENTINEL_INPUT_SIZE         26
#define SENTINEL_INPUT_BATCHES      1

/** Output 0 — Task priority head: [1, 5] INT8 */
#define SENTINEL_PRI_OUTPUT_SIZE    5

/** Output 1 — Power state head: [1, 3] INT8 */
#define SENTINEL_PWR_OUTPUT_SIZE    3

/* ────────────────────────────────────────────────────────────────────────── */
/*  INT8 Quantization parameters                                            */
/*  Extracted from .ai/network_sentinel_model.tflite_c_info.json             */
/*  input:   scale=0.00392156886  zero_point=-128  (STAI_FORMAT_S8)          */
/*  output0: scale=0.0257617515   zero_point=-128  (task priorities)         */
/*  output1: scale=0.00390625     zero_point=-128  (power state softmax)     */
/* ────────────────────────────────────────────────────────────────────────── */

/** Input quantization */
#define SENTINEL_INPUT_SCALE        0.00392156886f
#define SENTINEL_INPUT_ZERO_POINT   (-128)

/** Output 0 (priorities) quantization */
#define SENTINEL_PRI_SCALE          0.0257617515f
#define SENTINEL_PRI_ZERO_POINT     (-128)

/** Output 1 (power state) quantization */
#define SENTINEL_PWR_SCALE          0.00390625f
#define SENTINEL_PWR_ZERO_POINT     (-128)

/* ────────────────────────────────────────────────────────────────────────── */
/*  Memory budget                                                            */
/*  Model weights:   22,560 bytes  (const, in .rodata / external flash)      */
/*  Activation arena: ~1,100 bytes (from c_info: largest mpool buffer)       */
/* ────────────────────────────────────────────────────────────────────────── */

/** Activation buffer size in bytes — must be >= the arena reported by
 *  stedgeai analyze.  We round up to 2048 for alignment headroom.          */
#define SENTINEL_ACTIVATION_BUFFER_SIZE  2048

/** Model weights size (matches model_data.c sentinel_model_tflite_len) */
#define SENTINEL_WEIGHTS_SIZE            23048

/* ────────────────────────────────────────────────────────────────────────── */
/*  Neural-ART / ll_aton platform defines                                    */
/*  Set these in your project preprocessor or here:                          */
/*    LL_ATON_PLATFORM  = LL_ATON_PLAT_STM32N6                              */
/*    LL_ATON_OSAL      = LL_ATON_OSAL_BARE_METAL  (or FREERTOS)            */
/* ────────────────────────────────────────────────────────────────────────── */

#ifndef LL_ATON_PLATFORM
#define LL_ATON_PLATFORM    2   /* LL_ATON_PLAT_STM32N6 */
#endif

#ifndef LL_ATON_OSAL
#define LL_ATON_OSAL        0   /* LL_ATON_OSAL_BARE_METAL */
#endif

#endif /* AI_MODEL_CONFIG_H */
