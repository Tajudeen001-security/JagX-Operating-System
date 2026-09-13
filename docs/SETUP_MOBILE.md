# JagX Mobile setup

## QEMU aarch64 (today)

```bash
cd kernel/arch/aarch64
aarch64-linux-gnu-as -c boot.S -o boot.o
aarch64-linux-gnu-ld -Ttext=0x40000000 boot.o -o jagx-a64.elf
qemu-system-aarch64 -M virt -cpu cortex-a72 -kernel jagx-a64.elf -nographic
```

## Replacing Android on a real phone (Tecno, Itel, …)

There is **no** consumer “install JagX over Android” package yet.

For camera, SMS, SIM data, control center, flashlight, and settings to work on a specific phone, you need a full device port.

**Read:** [`REPLACE_ANDROID.md`](REPLACE_ANDROID.md)
