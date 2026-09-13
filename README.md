# JagX Operating System

**Founder:** Gbadamosi Tajudeen Olajide  
**License:** MIT  
https://github.com/Tajudeen001-security/JagX-Operating-System

From-scratch OS for **PC** and **Mobile** (not Linux/Android/iOS).

## Replace Android on Tecno / Itel?

See [docs/REPLACE_ANDROID.md](docs/REPLACE_ANDROID.md) — full device port required for camera, SMS, SIM, Control Center.

## Quick start (PC)

```bash
git clone https://github.com/Tajudeen001-security/JagX-Operating-System.git
cd JagX-Operating-System/kernel && make && make iso
qemu-system-i386 -cdrom ../boot/jagx.iso -m 128M -serial stdio \
  -netdev user,id=net0 -device virtio-net-pci,netdev=net0
```

## v0.0.16 highlights

- TLS PRF / key_block scaffold (HMAC-SHA256)
- **Control Center UI** tiles (Wi‑Fi, mobile data, airplane, torch)
- **Settings** skeleton (Network, Display, Sound, Security, About)
- Mobile HALs for future Tecno/Itel-class ports

## Mobile QEMU

[docs/SETUP_MOBILE.md](docs/SETUP_MOBILE.md)
