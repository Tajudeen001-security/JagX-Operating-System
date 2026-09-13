# Building a `.jagx` package (host side)

## Format

See `docs/PACKAGE_FORMAT.md` and `pkg/jagxpkg.c`.

## Steps

1. Write `manifest.txt` with name, package, version, vendor, arch, entry
2. Build native payload (object code or resource blob)
3. Call `pkg_build(manifest, payload, plen, out, out_max)` or host script that mirrors the header
4. Write bytes to `myapp.jagx`
5. On JagX: `pkg_install_path("/packages/myapp.jagx")`

## Noder

```
name=Noder
package=com.jagx.noder
vendor=JagX & JRILICENSE
```

Installed at boot via `noder_package_install()`.

## APK

Do **not** ship Noder as `.apk` for JagX. Rebuild as `.jagx`.
