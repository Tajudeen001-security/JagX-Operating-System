# Test JagX on an Android phone (without destroying Android)

You asked to **override Android for testing**. There are three levels. Only the last one actually replaces Android.

## Level A — Test Shell (recommended, cannot brick)

Use this for UX, `.jagx` apps, calls UI, SMS, JagCircle, JagBrowser.

1. On the phone, open Chrome
2. Open the JagX Test Shell URL (the live JagX OS)
3. Chrome menu → **Add to Home screen** → Add
4. Open the JagX icon. Use Phone, Messages, Social, Browser, Store
5. Optional: Chrome → Site settings → Notifications, Microphone, Camera for a fuller test

**This does not uninstall Android.** It is the correct way to test JagX *today*.

You cannot set a website as the system launcher without an APK. JagX will not ship an APK. When the native image exists for your model, the launcher *is* JagX.

## Level B — QEMU aarch64 from a PC, phone-shaped

Tests the **real kernel**, not the web shell.

```bash
cd kernel/arch/aarch64
aarch64-linux-gnu-as -c boot.S -o boot.o
aarch64-linux-gnu-ld -Ttext=0x40000000 boot.o -o jagx-a64.elf
qemu-system-aarch64 -M virt -cpu cortex-a72 -kernel jagx-a64.elf -nographic
```

UART should print `[JagX a64] Boot`.

## Level C — Actually replace Android (dangerous)

This is a **full OS port**, not Magisk, not a GSI, not a custom recovery ROM.

### What “override” really means

Android phones boot:

```
Boot ROM → bootloader (abl/LK) → Android kernel + vendor + system
```

To run JagX you replace the kernel and userspace. Vendor firmware (modem, camera ISP, GPU) does **not** automatically work.

### Checklist (per model, e.g. Tecno Spark / Itel / Infinix)

- [ ] OEM unlock allowed (`fastboot flashing unlock`)
- [ ] You have the **stock** Android image to restore
- [ ] UART or `fastboot boot` (ramdisk) so a bad image is recoverable
- [ ] JagX aarch64 kernel linked for that SoC memory map
- [ ] Framebuffer / DRM bring-up
- [ ] Touch IRQ
- [ ] Power / charger
- [ ] Wi-Fi firmware license
- [ ] Modem RIL for calls/SMS/data (hardest)

### Commands (once an image exists)

```bash
adb reboot bootloader
fastboot flashing unlock          # erases userdata
fastboot boot jagx-boot.img       # try RAM first — do not flash yet
# if display+touch work:
fastboot flash boot jagx-boot.img
fastboot reboot
```

**Do not run these with a fake/placeholder image.** There is no official `jagx-boot.img` for consumer Tecno/Itel yet. Using a random file will hard-brick many devices.

### If you brick

- Download mode / SP Flash (MediaTek) or Unisoc ResearchDownload with the **official** scatter + firmware
- Keep those files **before** you unlock

## What works in Test Shell vs on a flashed phone

| Feature | Test Shell | Flashed JagX (after BSP) |
|---------|------------|---------------------------|
| Home, dock, settings | Yes | Yes |
| `.jagx` install (no APK) | Yes (lab) | Yes |
| Call UI | Yes (lab session) | Real modem |
| SMS UI | Yes (lab) | Real SMSC |
| JagCircle | Yes | Yes |
| JagBrowser | Wikipedia + native pages | Full TCP/TLS |
| Camera | Device camera if permitted | HAL + ISP |
| SIM / mobile data | Simulated | Radio HAL |

## Bottom line

For testing **now**: Level A.  
For kernel bring-up: Level B.  
For replacing Android: Level C **after** a funded BSP for one named model.
