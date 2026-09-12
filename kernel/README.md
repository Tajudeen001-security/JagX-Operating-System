# JagX Kernel

This directory contains the core of the JagX operating system.

## Architecture

- `arch/` – architecture-specific code (x86_64, aarch64, ...)
- `core/` – architecture-independent kernel code
- `drivers/` – device drivers

## Current Goal

Get a minimal kernel that can boot under QEMU and print a message.
