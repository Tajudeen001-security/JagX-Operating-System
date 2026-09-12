# JagX Roadmap

## Phase 0 – Bootstrap (Now)
- Project structure & documentation
- Minimal x86_64 bootloader + kernel that prints to screen
- Basic build system
- QEMU testing setup

## Phase 1 – Kernel Foundations
- Physical & virtual memory management
- Interrupt / exception handling
- Simple scheduler (cooperative first, then preemptive)
- Basic device drivers (keyboard, timer, serial)
- Simple filesystem (ramfs or custom)

## Phase 2 – Userspace & GUI Basics
- System calls
- Userspace process loading
- Framebuffer / early graphics
- Simple window manager or compositor
- Touch and mobile input support design

## Phase 3 – Applications & Browser
- JagBrowser (HTML/CSS/JS engine – start very simple)
- Core apps (file manager, settings, terminal)
- Networking stack

## Phase 4 – Maturity
- Security model (capabilities / sandboxing)
- Package management
- Stable API
- Mobile hardware support (display, power, sensors)
- First public alpha releases

This roadmap will evolve. Contributions that move us forward are welcome.
