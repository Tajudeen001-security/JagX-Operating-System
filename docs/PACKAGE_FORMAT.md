# JagX application package format

## What other systems use

| OS | Extension | Notes |
|----|-----------|--------|
| Android | `.apk` | ZIP + Dalvik/ART bytecode |
| iOS | `.ipa` | ZIP + Mach-O, signed |
| Windows | `.exe` / `.msi` | PE binaries / installers |
| Linux | `.deb` / `.rpm` / AppImage | native ELF + metadata |
| macOS | `.app` / `.dmg` | bundles |

**JagX does not run APK or IPA.** Those need Android/iOS runtimes. JagX is a separate OS.

## JagX package: `.jagx`

| Item | Value |
|------|--------|
| Extension | **`.jagx`** |
| Magic | `JAGX` (4 bytes) then version `u16` |
| Layout | Custom archive: header + manifest + payload blobs |
| CPU | `x86` / `x86_64` / `aarch64` tags in manifest |
| Entry | Native JagX binary or bytecode id in manifest |

### Manifest (text, inside package)

```
name=Noder
package=com.jagx.noder
version=1.0.0
vendor=JagX & JRILICENSE
arch=x86
entry=noder.main
min_os=0.0.22
```

### Install path

`/apps/<package>/` on the system volume (RamFS today).

## Converting an Android APK (e.g. Noder APK)

You **cannot** rename `.apk` → `.jagx` and run it.

Correct path:
1. Keep product design (VS Code–like editor = Noder)
2. Implement as JagX-native code (this repo: `apps/noder/`)
3. Package with `tools/pack_jagx` → `noder.jagx`
4. Install with package manager API `pkg_install("noder.jagx")`

If you only have an APK binary and no source, you must re-implement the UI/logic for JagX (same product name, native package).
