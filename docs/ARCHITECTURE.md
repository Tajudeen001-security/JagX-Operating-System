# JagX Architecture Overview

## High-level Goals

- Monolithic or hybrid kernel (decision still open – start simple)
- Support for multiple architectures from day one (x86_64 first, then aarch64)
- Clean separation between architecture-specific and portable code
- Capability-based security model (long-term goal)
- Shared core for PC and Mobile, with platform-specific layers

## Kernel Layers (planned)

1. **Hardware Abstraction Layer (HAL)**
2. **Memory Manager**
3. **Process / Thread Scheduler**
4. **Interrupt & Exception Handling**
5. **Device Driver Framework**
6. **Filesystem**
7. **Networking**
8. **Graphics / Display Subsystem**

## Userspace

- System call interface
- Standard libraries (JagX libc)
- Windowing system / compositor
- Applications (including JagBrowser)

This document will be expanded as design decisions are made.
