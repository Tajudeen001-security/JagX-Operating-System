# JagX OS v0.1.0 — Independence

**An original operating system for PC and Mobile.**  
Not Linux. Not Android. Not iOS. Not Windows.  
Apps are **`.jagx` — never APK.**

**Founder:** Gbadamosi Tajudeen Olajide · JagX & JRILICENSE  
https://github.com/Tajudeen001-security/JagX-Operating-System

Built for people, companies, and **government agencies in Nigeria and Africa** who want a sovereign OS they can inspect, fork, and run without Google or Microsoft lock-in.

---

## What JagX is

| Layer | JagX | Not |
|-------|------|-----|
| Kernel | Own kernel (x86 + aarch64) | Linux |
| Packages | `.jagx` magic `JAGX` | `.apk` `.ipa` `.exe` |
| Phone | Native dialer + radio HAL | Google Dialer |
| Social | JagCircle native | WhatsApp/Facebook APK |
| Browser | JagBrowser | Chrome WebView |
| Office | Notepad, JagSheet, JagBase, Paint | Microsoft Office lock-in |
| IDE | Noder, on-device | Windows-only editors |

---

## Quick start (safe — does not touch Windows or Android)

```bash
git clone https://github.com/Tajudeen001-security/JagX-Operating-System.git
cd JagX-Operating-System/kernel
make && make iso
qemu-system-i386 -cdrom ../boot/jagx.iso -m 256M -serial stdio \
  -netdev user,id=net0 -device virtio-net-pci,netdev=net0
```

Type in Noder. **Ctrl+S** saves. **Ctrl+1–4** switch tabs.

Mobile kernel (QEMU):

```bash
cd kernel/arch/aarch64
aarch64-linux-gnu-as -c boot.S -o boot.o
aarch64-linux-gnu-ld -Ttext=0x40000000 boot.o -o jagx-a64.elf
qemu-system-aarch64 -M virt -cpu cortex-a72 -kernel jagx-a64.elf -nographic
```

---

## Test on your Android phone (does not delete Android)

**Do this first. Do not fastboot-flash random files.**

1. Open the JagX Test Shell (live JagX OS in the browser) in **Chrome**
2. Menu → **Add to Home screen**
3. Open the JagX icon
4. Try **Phone** (lab call), **Messages**, **JagCircle**, **JagBrowser**, **JagStore** (`.jagx` only)

That is the supported way to **override your home screen for testing** without bricking a Tecno, Itel, Infinix, or Samsung.

Full flashing path (dangerous, BSP required): [`docs/TEST_ANDROID.md`](docs/TEST_ANDROID.md) · [`docs/REPLACE_ANDROID.md`](docs/REPLACE_ANDROID.md)

---

## Test on a Windows PC (does not delete Windows)

| Method | What happens to Windows |
|--------|-------------------------|
| QEMU / VirtualBox + `boot/jagx.iso` | Untouched |
| Rufus/Ventoy USB boot | Untouched if you do not format the disk |
| Dual-boot GRUB | Windows remains, JagX is another boot entry |
| Wipe disk | **Destroys Windows** — not recommended before v1.0 |

Step-by-step: [`docs/TEST_WINDOWS.md`](docs/TEST_WINDOWS.md) · [`docs/INSTALL.md`](docs/INSTALL.md)

Firmware boot menu: Dell **F12**, HP **Esc then F9**, Lenovo **F12**.

---

## How people build apps (`.jagx` only)

Android Studio produces APKs. JagX **refuses** them.

```bash
python3 tools/pack_jagx.py \
  --manifest sdk/examples/hello/manifest.txt \
  --payload sdk/examples/hello/hello.c \
  --out hello.jagx
```

On JagX: **JagStore → Install** or `pkg_install_path("/packages/hello.jagx")`.

Manifest:

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

- SDK: [`sdk/README.md`](sdk/README.md) · header [`sdk/jagx.h`](sdk/jagx.h)
- Guide: [`docs/APP_DEVELOPMENT.md`](docs/APP_DEVELOPMENT.md)
- Format: [`docs/PACKAGE_FORMAT.md`](docs/PACKAGE_FORMAT.md)

**You cannot rename `app.apk` → `app.jagx`.** Rebuild native.

---

## Built-in apps (all `.jagx`)

**Mobile:** Phone, Messages, JagCircle, JagBrowser, JagStore, Camera, Gallery, Contacts, Settings, Control Center  
**PC:** Noder IDE, Terminal, Notepad, Paint, JagSheet, JagBase, Calculator, Files, Calendar  
**Shared kernel** for both.

Calls / SMS architecture: [`docs/TELEPHONY.md`](docs/TELEPHONY.md)  
Social: [`docs/SOCIAL.md`](docs/SOCIAL.md)

---

## For companies and governments (Nigeria & Africa)

JagX is **early but real**: kernel, compositor, package manager, native office, telephony HAL, mobile launcher.

**Ready now:** lab evaluation, training, `.jagx` app design, QEMU / Test Shell pilots.  
**Not ready:** replacing production Windows/Android fleets, emergency calling certification, every phone SoC.

Suggested path:

1. IT lab on QEMU + Test Shell (this week)
2. One unused PC via USB ISO
3. Requirements: EN + Hausa / Yoruba / Igbo / French
4. Internal JagStore (no Google account)
5. Fund **one** reference phone BSP (Tecno/Infinix/Itel are the volume devices)
6. Security review: [`docs/SECURITY.md`](docs/SECURITY.md)

Details: [`docs/GOVERNMENT.md`](docs/GOVERNMENT.md) · [`docs/TRAINING.md`](docs/TRAINING.md)

MIT license — ministries may fork.

---

## Honesty (status)

| Claim | Status |
|-------|--------|
| Original kernel, not a Linux distro | Yes |
| `.jagx` package manager refuses APK | Yes |
| Boots under QEMU (x86 ISO + aarch64 UART) | Yes |
| Native apps in tree (phone, SMS, social, office, IDE) | Yes |
| Daily driver replacing Android on a Tecno this week | **No** — needs a device BSP |
| Daily driver replacing Windows in a ministry this week | **No** — lab only |

Standing on its own means: own kernel, own apps, own package format, own installer docs. Hardware bring-up is the remaining mountain — documented, not pretended.

---

## Layout

```
apps/          native programs (phone, messages, social, noder, office…)
browser/       JagBrowser engine
boot/          ISO / GRUB / signing
compositor/    windows (PC) + overlays (mobile)
crypto/        SHA-256, ChaCha20-Poly1305
docs/          architecture, install, government, testing
drivers/       driver framework
fs/            RamFS
kernel/        x86 + aarch64
mobile/        launcher, HAL (radio, wifi, camera, lights)
net/           IPv4, TCP, TLS, virtio-net
pkg/           .jagx pack/install
sdk/           public API + hello example
tools/         pack_jagx.py
userland/      processes / init
```

---

## License

MIT. Founder: **Gbadamosi Tajudeen Olajide**.
