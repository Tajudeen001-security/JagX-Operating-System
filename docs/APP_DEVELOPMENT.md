# How to build apps for JagX

**JagX apps are `.jagx` files.** They are not APKs. Android Studio, Gradle, and the Play Store are the wrong toolchain.

## Why not APK?

APK packages Dalvik/ART bytecode and Android framework APIs. JagX has its own kernel, compositor, syscalls, and package manager. An APK would need the entire Android runtime — which JagX deliberately does not ship.

| Want | Use |
|------|-----|
| Install on JagX | `myapp.jagx` |
| Install on Android | `myapp.apk` (not JagX) |
| Install on Windows | `myapp.exe` (not JagX) |

## Toolchain

1. C (freestanding, same style as `apps/` in this tree)
2. JagX SDK header: [`sdk/jagx.h`](../sdk/jagx.h)
3. Packager: [`tools/pack_jagx.py`](../tools/pack_jagx.py)

```bash
# 1. Write sources (see sdk/examples/hello)
# 2. Build payload (cross to i686/aarch64 JagX ABI when the full libc lands)
# 3. Pack
python3 tools/pack_jagx.py \
  --manifest sdk/examples/hello/manifest.txt \
  --payload sdk/examples/hello/hello.c \
  --out hello.jagx

# 4. On JagX
# JagStore → Install hello.jagx
# or kernel: pkg_install_path("/packages/hello.jagx")
```

Today the kernel installs packaged **markers** at boot (`pkg/bundle_apps.c`) so the OS has a working app set without a host toolchain. As the userland linker matures, payloads become real binaries.

## Manifest

```
name=Clinic Records
package=ng.gov.health.clinic
version=1.2.0
vendor=Federal Ministry of Health
arch=any
entry=clinic.main
min_os=0.1.0
perm=storage,network,contacts
```

## Permissions

Declare only what you need. Telephony is `perm=telephony`. Camera is `perm=camera`. Undeclared access is denied.

## UI

- Mobile: full-screen activity on the JagX launcher
- PC: compositor window (`jagx_window_create`)
- Follow [`DESIGN_SYSTEM.md`](DESIGN_SYSTEM.md) — dark OLED, teal accent `#1EC9BC`

## Distribution

- Side-load `.jagx` (USB, files app, JagStore)
- Government agencies can host an internal JagStore (no Google account)
- Do not publish to Google Play and expect it to appear on JagX

## Porting an existing Android app

You cannot convert APK → JagX. Re-implement:

1. Keep product UX
2. Rewrite against `sdk/jagx.h`
3. Pack `.jagx`
4. Test in the JagX shell (QEMU / Test Shell) then on device
