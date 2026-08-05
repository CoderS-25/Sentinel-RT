# Sentinel-RT

AI-driven real-time scheduler middleware for μT-Kernel 3.0 on STM32N657. Entry ID 45302, TRON 2026.

## Architecture

```text
+-------------------+      +------------------+      +------------------+
|       Eyes        |      |      Brain       |      |      Hands       |
|   Task Profiler   | ---> |  Neural-ART NPU  | ---> | Decision Engine  |
+-------------------+      +------------------+      +------------------+
```

## Folder Structure

- `firmware/profiler/`: Task profiling, CPU load & wait time metrics (Eyes).
- `firmware/inference/`: NN inference layer wrapping STM32Cube.AI (Brain).
- `firmware/decision/`: Output applying engine for task priorities and sleep modes (Hands).
- `firmware/`: Main `sentinel_rt` entry point for initialization and task registration.

## Build Instructions

*(Placeholder)*
Make sure the STM32Cube.AI toolchain is configured for the STM32N657 target.
Integration with μT-Kernel 3.0 is required. Ensure you include standard `tk/tkernel.h` paths.

## License

MIT License

## Team

*(Placeholder)*
