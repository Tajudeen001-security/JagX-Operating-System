# JagX Mobile Architecture

## Design Principles for Mobile

1. **Shared Core, Specialized Edges**  
   Same kernel, same syscall interface, same security model as PC.  
   Mobile-specific code is isolated in HALs, compositors, and policy.

2. **Latency First**  
   Touch → processing → display path is treated as a critical path.  
   Minimize copies, context switches, and scheduling latency.

3. **Power as a First-Class Resource**  
   Every subsystem must be able to report and respond to power state.

4. **Capability Security**  
   Apps receive only the capabilities they need.  
   Sensors, location, camera, microphone are tightly controlled.

5. **Composable UI**  
   A modern compositor that supports smooth animations, variable refresh rates, and efficient damage tracking.

## Major Layers (Target)

```
┌─────────────────────────────────────────┐
│           Applications + JagBrowser     │
├─────────────────────────────────────────┤
│     Shell / Launcher / System UI        │
├─────────────────────────────────────────┤
│   Compositor + Window Management        │
├─────────────────────────────────────────┤
│  Input (Touch/Gesture) + Power + Sensors│
├─────────────────────────────────────────┤
│              Kernel + Drivers           │
└─────────────────────────────────────────┘
```

## Hardware Abstraction

We will define clean interfaces for:
- Display (including high refresh rate and adaptive sync)
- Touch controllers
- Battery / charging
- Radios (Wi-Fi, cellular, Bluetooth) — with strong permission gates
- Sensors (IMU, ambient light, proximity, etc.)
- Cameras

These interfaces must be usable on both real hardware and high-quality emulators/QEMU.
