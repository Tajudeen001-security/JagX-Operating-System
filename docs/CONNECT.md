# JagX Connect on JagX OS

JagX Connect ships as **`connect.jagx`**. It is not an APK.

## Install

```
JagStore → Open connect.jagx
# or from kernel
pkg_install_path("/packages/connect.jagx")
```

## Build the package (host)

```bash
python3 tools/pack_jagx.py \
  --manifest packages/connect/manifest.txt \
  --payload apps/jagx_connect.c \
  --out dist/connect.jagx
```

Magic bytes must be `JAGX`. The package manager refuses `.apk` `.ipa` `.exe` `.dex`.
