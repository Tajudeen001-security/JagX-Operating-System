# JagX OS v0.0.25

**Founder:** Gbadamosi Tajudeen Olajide · JagX & JRILICENSE  
https://github.com/Tajudeen001-security/JagX-Operating-System

## Targets

| | |
|--|--|
| **Phones** | Itel → Tecno / Infinix → other Android |
| **PC** | Windows-class machines (QEMU lab first) |

See [docs/TARGETS.md](docs/TARGETS.md) and port checklists under `docs/PORT_*.md`.

## Control Center networking

- **Wi‑Fi** → join network / phone hotspot (`wifi_join`)
- **Hotspot** → share from this device (`wifi_start_hotspot`)

## Code on JagX

Native Noder IDE + desktop Noder: https://github.com/JagX-JRILICENSE/Noder

```bash
cd kernel && make && make iso
qemu-system-i386 -cdrom ../boot/jagx.iso -m 128M -serial stdio
```
