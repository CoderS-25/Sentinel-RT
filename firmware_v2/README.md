# Sentinel-RT Firmware v2 — Corrected Implementation

> **Drop-in replacement** for the original `firmware/` scaffold.  
> All TODO placeholders have been resolved with working X-CUBE-AI / Neural-ART integration.

## What Changed vs. Original `firmware/`

| File | Original Problem | v2 Fix |
|:---|:---|:---|
| `inference/inference_engine.c` | Mock `memset` output, no TFLite/NPU init | Full X-CUBE-AI `ai_platform.h` integration with quantization/dequantization |
| `inference/inference_engine.h` | Missing quantization helpers | Added INT8 quant/dequant params, NPU config defines |
| `inference/model_data.c` | Correct (no changes needed) | Copied as-is from `firmware/inference/model_data.c` |
| `profiler/task_profiler.c` | `memset` zeros, no real metrics | Actual `tk_ref_tsk()` telemetry collection |
| `profiler/task_profiler.h` | Missing monitored task ID API | Added `sentinel_profiler_register_task()` |
| `decision/decision_engine.c` | `sync_flg_id` declared after use, HAL stubs | Fixed variable order, real HAL power calls, hysteresis filter |
| `decision/decision_engine.h` | Missing managed task array | Added task registration API |
| `sentinel_rt.c` | UART stub, no HAL init | Real `HAL_UART_Transmit`, NPU clock enable |
| `sentinel_rt.h` | No changes needed | Copied as-is |
| `ai_model_config.h` | **NEW** | X-CUBE-AI / Neural-ART model configuration |
| `npu_init.c` | **NEW** | NPU hardware clock and power initialization |
| `npu_init.h` | **NEW** | NPU init header |

## How To Use

1. Copy `firmware_v2/` contents over `firmware/` (or point your CubeIDE project to `firmware_v2/`)
2. Ensure X-CUBE-AI 10.2.1 middleware is in your include path
3. Rebuild FSBL → AppliSecure → AppliNonSecure
4. Flash or debug as described in the deployment guide
