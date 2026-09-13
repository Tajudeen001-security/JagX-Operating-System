# Secure Boot Notes (JagX)

## Goal

Ensure that only trusted JagX boot components run, from firmware/bootloader through the kernel.

## Intended chain

1. Firmware / UEFI (or platform boot ROM on mobile)
2. Signed bootloader (or GRUB with signature verification where applicable)
3. Signed kernel image
4. Optional measured init / first userspace

## Design notes

- Use a platform key hierarchy (or project-controlled keys for development devices)
- Kernel image signature verified before jump to entry point
- On failure: clear error state, no silent fallback to untrusted payload
- Development builds may use a separate “dev” key enrolled only on test machines

## Mobile

- Align with device fuse / OTP keys where hardware allows
- Verified boot must not brick devices during early bring-up; recovery path required

## Current status

Documented intent. Implementation depends on stable boot path (GRUB/ISO already started on x86; aarch64 boot still early).
