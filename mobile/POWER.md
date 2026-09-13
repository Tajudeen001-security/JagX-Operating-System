# Power Management Philosophy

Battery life is a core feature of JagX Mobile, not an optimization phase at the end.

## Goals

- Competitive or better battery life than current flagship mobile OSes on comparable hardware
- Predictable power behavior (users can understand why battery drains)
- Fine-grained control for advanced users without complexity for normal users

## Approach

1. **Aggressive idle** — CPUs, GPU, radios, and sensors enter deep idle states quickly
2. **Coordinated wakeups** — Avoid timer fragmentation; batch work when possible
3. **Display intelligence** — Adaptive refresh rate, content-aware brightness, efficient composition
4. **App policy** — Background execution is limited and visible to the user
5. **Hardware cooperation** — Use modern idle states, frequency scaling, and sensor hubs effectively

## Metrics We Will Track

- Screen-on time
- Suspend/resume latency
- Idle current draw
- Wakeup frequency and sources
- Per-subsystem power attribution (as far as hardware allows)
