# JagX port checklist — Itel (first phone target)

Fill blanks for **your** first device. One model only until it boots.

## Device identity

| Field | Value |
|-------|--------|
| **Brand** | Itel |
| **Model name** | _________________ |
| **Model number / codename** | _________________ |
| **Chipset (SoC)** | _________________ (often MediaTek) |
| **Android version shipped** | _________________ |
| **RAM / storage** | _________________ |
| **Bootloader unlock possible?** | Yes / No / Unknown |

## Boot path

- [ ] Official or community unlock method documented
- [ ] Fastboot / SP Flash Tool access confirmed
- [ ] Backup of stock firmware taken
- [ ] JagX aarch64 kernel builds for this CPU
- [ ] First stage bootloader loads JagX image

## Drivers (minimum to replace Android UI)

- [ ] Display + backlight
- [ ] Touchscreen
- [ ] Storage (eMMC/UFS)
- [ ] **Wi‑Fi** (scan / join / hotspot AP if supported)
- [ ] Power / battery
- [ ] Buttons (power, volume)

## Later (phone features)

- [ ] Modem / SIM / SMS / mobile data
- [ ] Camera
- [ ] Audio / speaker / mic
- [ ] Bluetooth
- [ ] Sensors

## HAL hooks in JagX tree

Register into: `mobile/hal/wifi`, `radio`, `camera`, `lights` + `drivers/` framework.

## Status

**Port status:** Not started / In progress / Boots to UI  
**Owner:** _________________  
**Date started:** _________________
