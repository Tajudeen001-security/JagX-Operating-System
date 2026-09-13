# JagX Operating System

**Founder:** Gbadamosi Tajudeen Olajide  
https://github.com/Tajudeen001-security/JagX-Operating-System

## v0.0.18

- Gallery · Screenshots (triple-tap) · Screen record
- Screenshots stored in **RamFS** + Gallery
- **Notification shade**
- **File manager** (RamFS)
- Control Center (Wi‑Fi, data, airplane, torch, shot, record)

## Quick start

```bash
git clone https://github.com/Tajudeen001-security/JagX-Operating-System.git
cd JagX-Operating-System/kernel && make && make iso
qemu-system-i386 -cdrom ../boot/jagx.iso -m 128M -serial stdio \
  -netdev user,id=net0 -device virtio-net-pci,netdev=net0
```

Android phone ports: [docs/REPLACE_ANDROID.md](docs/REPLACE_ANDROID.md)
