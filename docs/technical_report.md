# Sentinel-RT: AI Task Scheduler for STM32N657 + μT-Kernel 3.0

## 1. Problem Statement
In modern IoT and embedded systems, static priority scheduling often falls short. Traditional Real-Time Operating Systems (RTOS) rely on fixed priorities assigned by developers at compile time. However, complex workloads—such as sensor fusion, edge ML inference, and network communications—exhibit dynamic CPU demands. When a low-priority task unexpectedly requires significant computation, it can lead to priority inversion, missed deadlines, and inefficient power management. A static approach fails to adapt to these runtime variations, resulting in suboptimal performance and excessive energy consumption.

## 2. Proposed Solution
Sentinel-RT introduces an AI-driven middleware pipeline that seamlessly integrates with μT-Kernel 3.0 to provide dynamic task scheduling. Our solution uses a 3-task middleware architecture:
1. **Monitor Task**: Gathers telemetry data (CPU usage, wait times, deadline proximity) from the RTOS.
2. **AI Inference Task**: Feeds the telemetry data into an embedded Neural Network to predict optimal task priorities and power states.
3. **Actuator Task**: Applies the new priorities to the μT-Kernel scheduler and transitions the CPU into the recommended power state.

By dynamically adjusting priorities, Sentinel-RT ensures critical tasks meet their deadlines while aggressively saving power during idle periods.

## 3. Neural Network Architecture
The core of Sentinel-RT is a 1D Convolutional Neural Network (1D-CNN) designed for edge deployment. The architecture features:
- **Input Layer**: Accepts temporal telemetry features (total CPU load, task-specific metrics).
- **1D Convolutional Layers**: Extracts temporal patterns from the task execution history.
- **Dense Layers**: Fully connected layers to process the extracted features.
- **Dual Output Heads**:
  - *Priority Head*: Outputs a continuous score (0-1) for each task, representing its dynamic priority.
  - *Power State Head*: A softmax output classifying the optimal system power state (Active, LightSleep, DeepSleep).

## 4. Training Methodology
Given the lack of large-scale RTOS telemetry datasets, we utilized a synthetic data generation approach. We simulated various RTOS workloads (normal load, deadline crises, idle states) to generate over 100,000 telemetry samples. 
The dataset was split 80/20 for training and validation. We trained the model using TensorFlow/Keras with the Adam optimizer and Mean Squared Error (MSE) for the priority head, and Categorical Crossentropy for the power state head. Early stopping was employed to prevent overfitting, halting training when validation loss plateaued.

## 5. Results
Sentinel-RT achieves remarkable performance while maintaining a tiny memory footprint suitable for microcontrollers:

| Metric | Value |
|--------|-------|
| Power State Accuracy | 93.15% |
| Priority MAE | 0.042 |
| Model Size (INT8) | 22.51 KB |
| Compression Ratio | 7.76x |
| Inference Latency | < 1 ms |

Through post-training quantization to INT8, the model size was reduced to just 22.51KB, well below the 50KB target, enabling deployment on resource-constrained STM32 devices.

## 6. Integration
Sentinel-RT plugs directly into μT-Kernel 3.0. The middleware is initialized during the system boot sequence in `usermain()`. It utilizes standard T-Kernel API calls to monitor task status and adjust priorities dynamically.

```c
#include <tk/tkernel.h>
#include "sentinel_rt.h"

EXPORT INT usermain( void )
{
    /* Initialize standard μT-Kernel subsystems */
    tk_sys_init();
    
    /* Initialize Sentinel-RT AI Scheduler */
    ERR err = sentinel_rt_init();
    if (err < E_OK) {
        // Handle initialization error
        return err;
    }
    
    /* Start application tasks */
    app_task_start();
    
    /* Enter idle loop */
    tk_slp_tsk(TMO_FEVR);
    return 0;
}
```

## 7. Hardware
The system is heavily optimized for the STM32N657 microcontroller. By leveraging the built-in Neural-ART NPU acceleration, Sentinel-RT offloads the 1D-CNN inference from the main Cortex-M core. This hardware acceleration ensures that the AI scheduler itself does not become a bottleneck, allowing inference to complete in under 1 millisecond and freeing the main CPU for actual application workloads.

## 8. Future Work
While Sentinel-RT already demonstrates the viability of AI in RTOS scheduling, future work will focus on:
- **Camera Integration**: Using visual inputs to preemptively adjust scheduling (e.g., waking up processing tasks before a subject fully enters the frame).
- **Online Learning**: Allowing the model to fine-tune itself on the device to adapt to unique, environment-specific workloads.
- **Formal Verification**: Mathematically proving that the AI scheduler will never violate hard real-time constraints for safety-critical tasks.
