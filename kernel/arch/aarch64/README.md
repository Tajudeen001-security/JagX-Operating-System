# aarch64 (ARM64) Support for JagX

This directory will contain the ARM64 port of the JagX kernel.

Mobile devices almost exclusively use ARM64 (aarch64).  
High-quality aarch64 support is therefore mandatory for the mobile vision.

## Current Status

Skeleton only.  
The x86_64 kernel is currently the active development target.

## Planned Contents

- Boot (UEFI + Linux boot protocol / custom)
- Exception levels and exception vectors
- MMU / paging (4K and 16K pages consideration)
- GIC (Generic Interrupt Controller) support
- Timer (ARM Generic Timer)
- Early serial / framebuffer console
- SMP bring-up
- Device tree parsing

## Development Strategy

1. Get a minimal aarch64 kernel printing to UART under QEMU `virt` machine
2. Port memory management and interrupt handling
3. Unify with the existing x86 code where possible (shared core in `kernel/core` later)
4. Add mobile-relevant drivers (display, touch, power)

Contributions from people experienced with ARM64 bare-metal or OS development are highly welcome.
