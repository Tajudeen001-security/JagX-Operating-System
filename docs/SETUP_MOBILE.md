# JagX Mobile setup

## Test on a real Android phone (safe)

Chrome → JagX Test Shell → **Add to Home screen**.  
This does not replace Android. Details: [TEST_ANDROID.md](TEST_ANDROID.md).

## QEMU aarch64 (kernel)

```bash
cd kernel/arch/aarch64
aarch64-linux-gnu-as -c boot.S -o boot.o
aarch64-linux-gnu-ld -Ttext=0x40000000 boot.o -o jagx-a64.elf
qemu-system-aarch64 -M virt -cpu cortex-a72 -kernel jagx-a64.elf -nographic
```

## Replacing Android on a real phone

There is **no** consumer “install JagX over Android” package yet.  
[REPLACE_ANDROID.md](REPLACE_ANDROID.md)
