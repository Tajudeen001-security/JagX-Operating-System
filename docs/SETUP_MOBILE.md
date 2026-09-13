# JagX — Mobile setup (detailed)

## Today

Mobile is **aarch64 early bring-up**, not a flashable phone image.

```bash
cd kernel/arch/aarch64
aarch64-linux-gnu-as -c boot.S -o boot.o
aarch64-linux-gnu-ld -Ttext=0x40000000 boot.o -o jagx-a64.elf
qemu-system-aarch64 -M virt -cpu cortex-a72 -kernel jagx-a64.elf -nographic
```

## Future device bring-up (companies)

1. Obtain SoC BSP requirements (boot ROM, trust zone, display, touch, PMIC)
2. Port `kernel/arch/aarch64` to the board
3. Implement verified boot with device fuses
4. Integrate `mobile/` UX and security policies
5. Certify radio stacks only with proper legal/regulatory process

Do not sideload experimental kernels onto consumer phones without recovery.
