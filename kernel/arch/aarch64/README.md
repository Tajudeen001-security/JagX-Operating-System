# aarch64 Kernel - Real Early Bring-up

## What is real in v0.0.6

- UART output with proper TX full polling (PL011 on QEMU virt)
- Exception vector table installed via `vbar_el1`
- **MMU identity map enabled** (SCTLR_EL1.M = 1)
- Minimal 1GB block mapping so code keeps running after MMU on

This is actual bare-metal code, not a placeholder comment.

## Build & test

```bash
aarch64-linux-gnu-as -c boot.S -o boot.o
aarch64-linux-gnu-ld -Ttext=0x40000000 boot.o -o jagx-a64.elf
qemu-system-aarch64 -M virt -cpu cortex-a72 -kernel jagx-a64.elf -nographic
```

You should see the boot and MMU messages on the serial console.
