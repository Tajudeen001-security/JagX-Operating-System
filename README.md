# JagX Operating System

**Founder:** Gbadamosi Tajudeen Olajide  
**License:** MIT  
https://github.com/Tajudeen001-security/JagX-Operating-System

Original OS for PC + Mobile (not Linux/Android/iOS).

## Gallery, screenshots & screen record

- **Gallery** — see photos, screenshots, and recordings
- **Screenshot** — Control Center button, or **triple-tap** the screen
- **Screen record** — start/stop from Control Center; saved into Gallery
- Camera captures also go to Gallery when the camera HAL is connected

Details: [docs/FEATURES.md](docs/FEATURES.md)

## Replace Android on Tecno / Itel?

[docs/REPLACE_ANDROID.md](docs/REPLACE_ANDROID.md)

## Quick start

```bash
git clone https://github.com/Tajudeen001-security/JagX-Operating-System.git
cd JagX-Operating-System/kernel && make && make iso
qemu-system-i386 -cdrom ../boot/jagx.iso -m 128M -serial stdio \
  -netdev user,id=net0 -device virtio-net-pci,netdev=net0
```

## v0.0.17

Gallery · Screenshot · Triple-tap · Screen record · DND · Battery saver · Clipboard
