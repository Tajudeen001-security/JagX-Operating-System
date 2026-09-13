# aarch64 / Mobile Kernel Progress

## v0.0.5 Status

- Exception vector table installed (`vbar_el1`)
- Basic exception stubs (currently park)
- UART banner confirms boot path
- MMU preparation point marked (full identity mapping comes next)

## Test (when cross tools available)

```bash
aarch64-linux-gnu-as boot.S -o boot.o
aarch64-linux-gnu-ld -Ttext=0x40000000 boot.o -o jagx-a64.elf
qemu-system-aarch64 -M virt -cpu cortex-a72 -kernel jagx-a64.elf -nographic
```

Next mobile milestones: real MMU enable, GIC, timer, then display path.
