# JagX SDK — build apps as `.jagx` (never APK)

JagX is **not Android**. Google Play APKs, iOS IPAs, and Windows EXEs **will not run**.

Your product ships as a **`.jagx` package** signed for JagX.

## 1. Write a native app

```c
#include "jagx.h"

int main(void) {
    jagx_notify("Hello", "Running on JagX");
    return 0;
}
```

Place sources under your project. Link with the JagX syscall ABI (`kernel/syscall`).

## 2. Write `manifest.txt`

```
name=Hello JagX
package=ng.example.hello
version=1.0.0
vendor=Your Organisation
arch=any
entry=hello.main
min_os=0.1.0
perm=network
```

`package` is reverse-domain (like Android) **but that is the only similarity**. The payload is JagX-native, not DEX.

## 3. Pack

```bash
python3 tools/pack_jagx.py \
  --manifest manifest.txt \
  --payload hello.bin \
  --out hello.jagx
```

The file starts with magic `JAGX`. If `pack_jagx.py` prints `Refused: APK`, you pointed it at Android.

## 4. Install on JagX

- **On-device:** JagStore → Install, or `pkg_install_path("/packages/hello.jagx")`
- **Refused:** `.apk` `.ipa` `.exe` `.dex`

## Permissions

| perm | What it unlocks |
|------|-----------------|
| `telephony` | Dialer / SMS |
| `contacts` | Address book |
| `network` | JagBrowser / TCP |
| `camera` | Camera HAL |
| `storage` | `/apps/<package>/` plus shared files |
| `social` | JagCircle posts |

The kernel capability model denies undeclared perms.

## Examples

- `sdk/examples/hello/` — smallest package
- `apps/phone.c` — native dialer
- `apps/social.c` — JagCircle
- `apps/noder.c` — IDE that runs **on** JagX

## What you cannot do

- Rename `app.apk` → `app.jagx` and expect it to launch
- Ship Play Store builds
- Call Android APIs (`android.*`, JNI, ART)

Rebuild the product as JagX-native. Same idea, new package.
