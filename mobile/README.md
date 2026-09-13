# JagX Mobile

This directory contains everything specific to making **JagX on phones and tablets** excellent.

## Goal

Build one of the best mobile operating systems in the world — measured by:

- Input latency and smoothness
- Battery efficiency
- Privacy and user control
- Visual and interaction quality
- Long-term maintainability

## Contents

- `ARCHITECTURE.md` — High-level mobile architecture
- `UX_PRINCIPLES.md` — Design and interaction principles
- `POWER.md` — Power management philosophy
- `TOUCH.md` — Touch & gesture pipeline goals
- `SECURITY.md` — Mobile security model ideas

## Relationship to the Kernel

The core kernel lives in `/kernel` and is shared.  
Mobile-specific drivers, HALs, and policy will live here or under `kernel/arch/aarch64` and platform layers.

We deliberately keep the mobile design documents early so that every kernel and userspace decision can be evaluated against mobile excellence.
