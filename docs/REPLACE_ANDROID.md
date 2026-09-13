# Replacing Android with JagX on a phone (Tecno, Itel, Infinix, Samsung, …)

## Short answer

**You cannot install JagX today the way you install a GSI or custom ROM and expect camera, SIM, SMS, mobile data, and flashlight to work.**

JagX is a **from-scratch OS**. Android phones ship with:

- A locked or vendor bootloader
- A Linux kernel + proprietary drivers (camera ISP, modem/RIL, GPU, sensors)
- Vendor HALs and firmware blobs

JagX does **not** reuse that Android stack. So “override Android → JagX” is a **full device port**, not a one-click installer.

**For testing without bricking:** use the JagX Test Shell (Add to Home screen). See [`TEST_ANDROID.md`](TEST_ANDROID.md).

---

## What users want (target product)

| Feature | Meaning on JagX |
|---------|------------------|
| Camera | Photo/video via JagX camera HAL + ISP/sensor drivers |
| Settings | System settings app (display, sound, network, security) |
| Messages / SMS | Telephony stack + SIM + modem firmware interface |
| SIM / mobile network | Radio interface (baseband), not only Wi‑Fi |
| Control center | Quick toggles: mobile data, Wi‑Fi, Bluetooth, flashlight, airplane mode |
| Flashlight / torch | LED driver through a stable HAL |
| Touch | Low-latency touch + gestures |
| Calls | Audio path + modem call control |

All of that must be **implemented for each SoC/board**, not inherited automatically from Android.

Native apps already in tree: Phone, Messages, JagCircle, JagBrowser, JagStore (`.jagx` only).

---

## Realistic stages

### Stage 0 — Lab only (current)
- QEMU aarch64 / x86
- JagX Test Shell (browser / PWA) for UX
- No consumer phone installer
- No Tecno/Itel flash package

### Stage 1 — Bring-up on one unlocked device
Pick **one** model with unlockable bootloader.

1. Unlock bootloader (OEM policy; often voids warranty)
2. Working recovery / fastboot
3. Port JagX aarch64 kernel (UART first)
4. Display (framebuffer)
5. Touch
6. Charging / battery

### Stage 2 — Daily usability hardware
Wi‑Fi / Bluetooth, audio, sensors, flashlight, camera.

### Stage 3 — Telephony
Baseband / modem, SIM, SMS, voice calls. **Hardest.** Without vendor docs, SMS and cellular data will not work.

### Stage 4 — Product polish
Control Center, Settings, permissions, OTA, verified boot, RF certification.

---

## Why “just flash JagX over Android” fails

| Android piece | If you remove it | Effect |
|---------------|------------------|--------|
| Vendor kernel modules | Gone | Camera, GPU, touch may die |
| `vendor` / `odm` partitions | Gone | Firmware for modem/camera missing |
| RIL / modem daemon | Gone | No SIM, SMS, mobile data |
| Camera HAL + ISP libs | Gone | Black screen camera |

---

## For a company (OEM, carrier, ministry, startup)

1. Choose **one** reference device and fund a BSP team
2. Negotiate firmware redistribution for Wi‑Fi/modem/camera blobs
3. Implement JagX HALs: camera, lights, radio, wifi, sensors
4. Build Settings + Messaging + Dialer + Control Center
5. Ship verified boot + recovery
6. Field-test SIM, SMS, data, camera, thermal, battery

---

## What you can do today

```bash
cd kernel && make && make iso
qemu-system-i386 -cdrom ../boot/jagx.iso -m 256M -serial stdio
```

On a phone: Chrome → JagX Test Shell → Add to Home screen. This **overrides your testing home icon**, not the Android system partition.

Read: `docs/TEST_ANDROID.md`, `mobile/README.md`, `docs/SETUP_MOBILE.md`.

---

## Bottom line

| Question | Answer |
|----------|--------|
| Can I turn my Tecno/Itel into JagX this week like a custom ROM? | **No** |
| Can I test JagX UX on that phone this week? | **Yes — Test Shell** |
| Can JagX become the phone OS with proper engineering? | **Yes, as a long-term port** |
| Will camera, SMS, SIM data, flashlight work automatically? | **Only after drivers + HALs** |
| Official “override Android” installer? | **Not yet — documented path only** |
