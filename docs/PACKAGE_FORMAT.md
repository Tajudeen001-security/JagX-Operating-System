# JagX application package format

## What other systems use

| OS | Extension | Notes |
|----|-----------|--------|
| Android | `.apk` | ZIP + Dalvik/ART bytecode |
| iOS | `.ipa` | ZIP + Mach-O, signed |
| Windows | `.exe` / `.msi` | PE binaries / installers |
| Linux | `.deb` / `.rpm` / AppImage | native ELF + metadata |
| macOS | `.app` / `.dmg` | bundles |
| **JagX** | **`.jagx`** | Magic `JAGX` + manifest + native payload |

**JagX does not run APK or IPA.** The package manager checks magic bytes and refuses `.apk` `.ipa` `.exe` `.dex`.

## JagX package: `.jagx`

| Item | Value |
|------|--------|
| Extension | **`.jagx`** |
| Magic | `JAGX` (4 bytes LE `0x5847414A`) then version `u16` |
| Layout | header (24 bytes) + manifest + payload |
| CPU | `x86` / `x86_64` / `aarch64` / `any` in manifest |
| Entry | Native JagX binary or bytecode id |

Header (`pkg/jagxpkg.h`):

```
u32 magic
u16 version
u16 flags
u32 manifest_len
u32 payload_len
u32 checksum   (sum of payload bytes)
```

### Manifest

```
name=Noder
package=com.jagx.noder
version=1.0.0
vendor=JagX & JRILICENSE
arch=any
entry=noder.main
min_os=0.1.0
perm=storage
```

Permissions: `telephony`, `contacts`, `network`, `camera`, `storage`, `social`.

### Install path

`/apps/<package>/` on the system volume (RamFS today).

### Host packager

```bash
python3 tools/pack_jagx.py --manifest manifest.txt --payload app.bin --out app.jagx
```

## Converting an Android APK

You **cannot** rename `.apk` → `.jagx` and run it.

1. Keep the product design
2. Implement as JagX-native (`sdk/jagx.h`)
3. Package with `tools/pack_jagx.py`
4. Install with JagStore or `pkg_install_path("noder.jagx")`
