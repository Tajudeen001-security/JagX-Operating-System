# JagX Operating System

**Founder:** Gbadamosi Tajudeen Olajide  
**License:** MIT  
**Repo:** https://github.com/Tajudeen001-security/JagX-Operating-System

Original OS for PC + Mobile — **not** Linux/Android/iOS based.

## Replace Android on Tecno / Itel / etc.?

Not a one-click ROM. Camera, SMS, SIM data, Control Center, flashlight need a **device port**.

**Guide:** [docs/REPLACE_ANDROID.md](docs/REPLACE_ANDROID.md)

**HAL stubs now in tree** (`mobile/hal/`): camera · lights/torch · radio/SMS/data · Wi‑Fi · Control Center toggles. Real phones register drivers into these interfaces.

## PC quick start

```bash
git clone https://github.com/Tajudeen001-security/JagX-Operating-System.git
cd JagX-Operating-System/kernel && make && make iso
qemu-system-i386 -cdrom ../boot/jagx.iso -m 128M -serial stdio \
  -netdev user,id=net0 -device virtio-net-pci,netdev=net0
```

## Mobile QEMU (aarch64)

See [docs/SETUP_MOBILE.md](docs/SETUP_MOBILE.md).

## v0.0.15

TLS record/handshake parsing · Mobile Control Center + HALs
