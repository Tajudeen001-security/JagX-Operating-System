# Building a `.jagx` package (host side)

See `docs/PACKAGE_FORMAT.md`, `docs/APP_DEVELOPMENT.md`, and `pkg/jagxpkg.c`.

```bash
python3 tools/pack_jagx.py \
  --manifest sdk/examples/hello/manifest.txt \
  --payload sdk/examples/hello/hello.c \
  --out hello.jagx
```

The tool **refuses** `.apk` `.ipa` `.exe` `.dex` payloads.

On JagX: `pkg_install_path("/packages/hello.jagx")` or JagStore.

## Noder

```
name=Noder
package=com.jagx.noder
vendor=JagX & JRILICENSE
```

Installed at boot via `noder_package_install()`.

## APK

Do **not** ship apps as `.apk` for JagX. Rebuild as `.jagx`.
