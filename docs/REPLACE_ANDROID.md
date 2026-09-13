# Replacing Android with JagX on a phone (Tecno, Itel, Infinix, Samsung, …)

## Short answer

**You cannot install JagX today the way you install a GSI or custom ROM and expect camera, SIM, SMS, mobile data, and flashlight to work.**

JagX is a **from-scratch OS**. Android phones ship with:

- A locked or vendor bootloader
- A Linux kernel + proprietary drivers (camera ISP, modem/RIL, GPU, sensors)
- Vendor HALs and firmware blobs

JagX does **not** reuse that Android stack. So “override Android → JagX” is a **full device port**, not a one-click installer.

This document explains the **real path** for individuals and companies who want a Tecno/Itel/other device to run JagX with working hardware features.

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

---

## Realistic stages

### Stage 0 — Lab only (current project status)
- QEMU aarch64 / x86
- No consumer phone installer
- No Tecno/Itel flash package

### Stage 1 — Bring-up on one unlocked device
Pick **one** model with:
- Unlockable bootloader
- Public or obtainable schematics / kernel sources where possible
- Willingness to brick and recover

Work items:
1. Unlock bootloader (OEM policy; often voids warranty)
2. Find a working recovery / fastboot
3. Port JagX aarch64 kernel to that board (UART first)
4. Display (framebuffer / DRM-style driver)
5. Touch input
6. Basic charging / battery status

### Stage 2 — “Daily usability” hardware
7. Wi‑Fi / Bluetooth (often needs firmware blobs + legal redistribution rights)
8. Audio
9. Sensors (IMU, ALS, proximity)
10. Flashlight LED
11. Camera (complex: sensor + ISP + userspace HAL)

### Stage 3 — Telephony (hardest for most teams)
12. Baseband / modem interface (proprietary on almost all phones)
13. SIM detection, signal, mobile data on/off
14. SMS send/receive
15. Voice calls

**Without modem vendor documentation or a reverse-engineered RIL, SMS and cellular data will not work** — this is true for almost every non-Android OS on commercial phones.

### Stage 4 — Product polish
- Control center UI (JagX Design System)
- Settings app
- Permissions / capability security
- OTA updates + verified boot
- Regulatory certification (RF, safety) if sold commercially

---

## Why “just flash JagX over Android” fails

| Android piece | If you remove it | Effect |
|---------------|------------------|--------|
| Vendor kernel modules | Gone | Camera, GPU, touch may die |
| `vendor` / `odm` partitions | Gone | Firmware for modem/camera missing |
| RIL / modem daemon | Gone | No SIM, SMS, mobile data |
| Camera HAL + ISP libs | Gone | Black screen camera |

JagX must **reimplement or legally integrate** each of these for that exact chipset (MediaTek / Unisoc / etc. common on Tecno/Itel).

---

## Control center features (design target)

When the mobile shell exists, Control Center should include:

- Mobile data toggle (needs modem stack)
- Wi‑Fi / Bluetooth / Airplane mode
- Flashlight (LED class driver)
- Brightness / rotation
- Quiet / focus modes
- Quick Settings tiles aligned with `mobile/UX_PRINCIPLES.md` and `docs/MOBILE_SECURITY.md`

These are **UI + HAL + driver** work, documented as goals, not available as a flashable ROM today.

---

## For a company (Tecno OEM, carrier, or startup)

1. **Choose one reference device** and fund a BSP team (kernel + drivers + QA).
2. Negotiate **firmware redistribution** rights for Wi‑Fi/modem/camera blobs if required.
3. Implement JagX HALs:
   - `camera`, `lights` (torch), `radio`/`modem`, `wifi`, `sensors`
4. Build Settings + Messaging + Dialer + Control Center on the JagX compositor.
5. Ship with **verified boot** and a recovery that can restore a working image.
6. Run field tests for SIM, SMS, data, camera, thermal, battery.

Estimated effort for a single phone model with working telephony + camera: **large multi-person, multi-month/year project**, not a weekend install.

---

## What you can do today

```bash
# Experience JagX safely in QEMU (PC or aarch64)
cd kernel && make && make iso
qemu-system-i386 -cdrom ../boot/jagx.iso -m 128M -serial stdio \
  -netdev user,id=net0 -device virtio-net-pci,netdev=net0

# Mobile architecture stub
cd kernel/arch/aarch64
# build & run under qemu-system-aarch64 (see README)
```

Read also:
- `mobile/README.md` — mobile vision
- `docs/SETUP_MOBILE.md` — aarch64 QEMU steps
- `docs/MOBILE_SECURITY.md` — permissions model
- Root `README.md` — full PC/Mobile setup

---

## Bottom line

| Question | Answer |
|----------|--------|
| Can I turn my Tecno/Itel into JagX this week like a custom ROM? | **No** |
| Can JagX become that OS with proper engineering? | **Yes, as a long-term port** |
| Will camera, SMS, SIM data, flashlight work automatically? | **Only after drivers + HALs for that device** |
| Is there an official “override Android” installer? | **Not yet — documented path only** |

Honesty protects users from bricking phones and protects the project from false claims.
