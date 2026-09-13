# Touch & Gesture Pipeline

## Primary Goal

**Lowest practical touch-to-photon latency** while remaining power-efficient and reliable.

## Design Targets

- Motion-to-photon as close to the display refresh period as possible
- Reliable multi-touch (10+ contacts)
- Clean gesture recognition (swipe, pinch, long-press, edge gestures) without fighting the application
- Palm rejection and accidental touch mitigation
- Support for high report-rate touch controllers

## Pipeline Stages (Conceptual)

1. Hardware interrupt / firmware report
2. Kernel driver → normalized events
3. Input dispatcher (with priority for interactive threads)
4. Gesture recognizer (system + app level)
5. Application / compositor response
6. Composition and display scanout

Every stage will be examined for unnecessary latency, copies, and lock contention.

## Future Extensions

- Stylus support with pressure and tilt
- External touchpads and precise pointer modes
