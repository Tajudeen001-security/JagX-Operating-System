# JagX Security Architecture

Goal: Make JagX one of the most secure general-purpose operating systems by design, not by afterthought.

## Core Principles

1. **Least Privilege** – Every component runs with the minimum rights required.
2. **Capability-based security** – Access to resources is via unforgeable tokens, not ambient authority.
3. **Strong isolation** – Processes, drivers, and the browser are isolated by default.
4. **Minimal trusted computing base** – Small kernel, audited critical paths.
5. **No silent data collection** – Privacy is a security property.
6. **Defense in depth** – W^X, ASLR, stack protection, CFI where feasible, verified boot later.

## Current Foundations (v0.0.7)

- Clear separation of kernel vs future userspace
- Syscall surface kept small and explicit
- Design for capability tokens (structure reserved)
- Mobile security model already documented (permissions, sensors, etc.)
- No network stack that accepts arbitrary traffic yet (reduces attack surface while under construction)

## Roadmap toward high security

| Phase | Item |
|-------|------|
| Near  | W^X enforcement, basic ASLR for userspace, stack canaries |
| Mid   | Capability system for files, network, devices |
| Mid   | JagBrowser site isolation + capability-restricted rendering |
| Later | Verified boot chain, optional measured boot, secure element support |
| Later | Formal-ish verification of critical kernel paths where practical |

## Mobile-specific

- Sensors, camera, mic, location require explicit capabilities
- Background execution tightly limited
- Permission UX designed to be honest and revocable

Security is a permanent constraint on every new feature.
