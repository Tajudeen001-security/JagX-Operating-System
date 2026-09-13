# Install JagX on a device

JagX is a **from-scratch OS**. Installing it is closer to installing a new operating system than installing an app.

## Choose a path

| Who | Path | Risk |
|-----|------|------|
| Anyone, today | QEMU (PC or phone-sized aarch64) | None |
| Testers | JagX Test Shell (this live OS in a browser / PWA) | None |
| PC lab | Boot `jagx.iso` from USB (Rufus / Ventoy) | Low if you do not overwrite the disk |
| Replace Windows | Dual-boot or full disk install | High — back up first |
| Replace Android | Device port + fastboot (not a consumer ROM yet) | Very high — can brick |

## 1. Safest — QEMU on a PC (Windows, Linux, macOS)

```bash
git clone https://github.com/Tajudeen001-security/JagX-Operating-System.git
cd JagX-Operating-System/kernel
make
make iso
qemu-system-i386 -cdrom ../boot/jagx.iso -m 256M -serial stdio \
  -netdev user,id=net0 -device virtio-net-pci,netdev=net0
```

Windows users: install [QEMU](https://www.qemu.org/download/#windows) or VirtualBox and attach `boot/jagx.iso` as a CD.

## 2. Test Shell — phone or PC browser

Open the JagX Test Shell (the interactive OS in this project’s live preview / PWA).

- **Android Chrome:** menu → **Add to Home screen**. You get a JagX icon. This does **not** delete Android. It is how you test calls, SMS, JagCircle, JagBrowser, and `.jagx` install **without flashing**.
- **Windows Edge/Chrome:** install as app, or run QEMU for the real kernel.

## 3. USB boot on a Windows PC (lab)

See [`TEST_WINDOWS.md`](TEST_WINDOWS.md).

1. Build `boot/jagx.iso`
2. Rufus → DD mode → write ISO to USB
3. Reboot → firmware boot menu (F12 / F10 / Esc) → USB
4. JagX kernel starts. Windows is still on disk unless you format it.

## 4. Replace Android on a phone

There is **no** one-tap “override Android” installer for Tecno / Itel / Infinix / Samsung today.

Read [`REPLACE_ANDROID.md`](REPLACE_ANDROID.md) and [`TEST_ANDROID.md`](TEST_ANDROID.md).

Short version:

1. Unlock bootloader (voids warranty, OEM dependent)
2. Port JagX aarch64 + display + touch for **that exact model**
3. `fastboot flash boot` / `fastboot boot` a JagX image
4. Camera, SIM, SMS, calls need a modem/camera HAL for that SoC

Until a named device has a BSP, **use the Test Shell + QEMU**.

## 5. Companies and ministries

1. Lab: QEMU + Test Shell (this week)
2. Dedicated PCs: USB/ISO, no production data
3. Fund one reference phone BSP (recommended: a popular Unisoc/MediaTek Tecno or Infinix)
4. Internal JagStore for `.jagx` line-of-business apps
5. Security review: [`SECURITY.md`](SECURITY.md), [`GOVERNMENT.md`](GOVERNMENT.md)

## Uninstall / recover

- **QEMU:** close the window
- **USB:** unplug, boot Windows as usual
- **PWA Test Shell:** uninstall the home-screen shortcut
- **Fastboot flash:** re-flash the vendor Android image from the OEM (if you kept it). If you did not, the phone may be unrecoverable — this is why consumer flash is not offered yet
