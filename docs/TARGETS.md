# JagX hardware targets

| Class | Brands / scope | Primary docs |
|-------|----------------|--------------|
| **Phones** | **Itel**, **Tecno**, **Infinix**, then other Android | PORT_ITEL.md, PORT_TECNO_INFINIX.md, PORT_ANDROID_GENERIC.md |
| **PC / system** | Machines that run **Windows** (lab → dual-boot) | PORT_WINDOWS_PC.md |

## Priority order

1. **PC (QEMU + Windows PCs)** — demos, government lab, Noder on OS  
2. **One Itel model** — first Android override pilot  
3. **One Tecno or Infinix** — same-family reuse  
4. **Other Android** — only after a successful Transsion pilot  

## Control Center networking

- **Wi‑Fi tile** → `wifi_join()` (join router or **phone hotspot**)
- **Hotspot tile** → `wifi_start_hotspot()` (this device shares)

See `docs/HOTSPOT.md`.
