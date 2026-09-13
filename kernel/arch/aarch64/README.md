# aarch64 (ARM64) – JagX Mobile Foundation

## Current Status

- Early boot stub for QEMU `virt` machine
- Prints a banner over UART
- Parks secondary cores

## How to test (later)

```bash
aarch64-elf-as boot.S -o boot.o
aarch64-elf-ld -T linker.ld boot.o -o jagx-aarch64.elf
qemu-system-aarch64 -M virt -cpu cortex-a53 -kernel jagx-aarch64.elf -nographic
```

## Next steps for Mobile

1. Exception vectors
2. Early MMU
3. GIC + timer
4. Framebuffer / display for mobile UI
5. Touch input path

This architecture is the foundation for the high-quality mobile experience we are targeting.
