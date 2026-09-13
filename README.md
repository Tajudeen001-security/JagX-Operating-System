# JagX OS v0.0.19

**Founder:** Gbadamosi Tajudeen Olajide  
https://github.com/Tajudeen001-security/JagX-Operating-System

## New
- TLS master secret derivation (PRF from pre-master)
- Richer Settings UI
- BMP screenshots to RamFS + Gallery
- Status bar (clock / battery)
- Lock screen (swipe up to unlock)
- Notification shade · Control Center · Gallery · Files

## Run
```bash
cd kernel && make && make iso
qemu-system-i386 -cdrom ../boot/jagx.iso -m 128M -serial stdio
```

Phone ports: docs/REPLACE_ANDROID.md
