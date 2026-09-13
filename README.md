# JagX Operating System

**JagX** is an original, open-source operating system built **from scratch** — no Linux, no Android, no iOS base.

It targets both **PC** and **Mobile** as first-class citizens, with a special ambition:

> **Make the JagX Mobile experience one of the best in the world** — smooth, private, efficient, beautiful, and under the user’s full control.

## Core Goals

- Completely independent kernel and system software
- Dual architecture support: x86_64 (PC) + aarch64 (Mobile & modern PC)
- Own native web browser (**JagBrowser**)
- Security and privacy by design
- Excellent mobile experience as a top priority (not an afterthought)

## Why JagX Mobile aims to be among the best

Most mobile operating systems today are either closed (iOS) or heavy Linux-based (Android).  
JagX Mobile is designed from day one with these principles:

- **Extremely low input latency** (touch and gesture pipeline optimized)
- **Predictable performance** and smooth 120 Hz+ UI when hardware allows
- **Aggressive but intelligent power management**
- **Strong privacy defaults** and transparent permission model
- **Lightweight base** — no unnecessary background services
- **Beautiful, modern, consistent design language**
- **Full user ownership** — no forced accounts, no telemetry by default
- **Long-term updateability** even on older devices

## Current Status (Early)

| Area              | Status                          |
|-------------------|---------------------------------|
| x86 Kernel        | Bootable, interrupts, memory, basic FS & syscalls |
| aarch64 / Mobile  | Architecture skeleton + design docs |
| Graphics          | VGA text only (framebuffer later) |
| Touch / Gestures  | Design phase                    |
| Power Management  | Design phase                    |
| JagBrowser        | Placeholder                     |
| Userspace         | Very early stubs                |

## Repository Layout

```
JagX-Operating-System/
├── kernel/           # Shared + arch-specific kernel code
│   ├── arch/
│   │   ├── x86_64/
│   │   └── aarch64/  # Mobile & modern ARM PC
│   ├── mm/
│   ├── fs/
│   └── syscall/
├── mobile/           # Mobile-specific design, UX, HAL ideas
├── browser/          # JagBrowser
├── docs/             # Architecture, vision, roadmaps
└── ...
```

## Building the current (x86) kernel

```bash
cd kernel
make
make run          # QEMU
```

## Contributing

This is a long-term, ambitious project. Low-level systems programmers, mobile UI/UX people, and security-minded developers are especially welcome.

## License

MIT License — fully open source.

---

**JagX** — Own your computer. Own your phone.
