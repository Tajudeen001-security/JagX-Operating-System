# JagX OS v0.0.22

**Founder:** Gbadamosi Tajudeen Olajide · **JagX & JRILICENSE**  
https://github.com/Tajudeen001-security/JagX-Operating-System

## App packages = `.jagx` (not APK / IPA)

| Platform | Package file |
|----------|----------------|
| Android | `.apk` |
| iOS | `.ipa` |
| Windows | `.exe` / `.msi` |
| **JagX** | **`.jagx`** |

APK will **not** run on JagX. Rebuild apps as `.jagx`.

### Noder (VS Code–class editor)

- Product: **Noder** by **JagX & JRILICENSE**
- On JagX: native editor + **`noder.jagx`** installed by package manager
- Docs: [PACKAGE_FORMAT.md](docs/PACKAGE_FORMAT.md)

## Build

```bash
cd kernel && make && make iso
qemu-system-i386 -cdrom ../boot/jagx.iso -m 128M -serial stdio
```
