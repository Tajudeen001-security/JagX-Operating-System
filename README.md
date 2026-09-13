# JagX Operating System

**Founder:** Gbadamosi Tajudeen Olajide  
**License:** MIT  
**Repository:** https://github.com/Tajudeen001-security/JagX-Operating-System

JagX is an **original** operating system for **PC** and **Mobile**, built from scratch — **not** based on Linux, Android, or iOS.

---

## Want to replace Android on a Tecno / Itel / other phone?

**Short answer: not with a one-click installer today.**

Turning a commercial Android phone into JagX so that **camera, Settings, SMS, SIM/mobile data, Control Center (data on/off, flashlight, etc.)** all work means building a **full board support package** for that exact device (drivers + modem + HALs + apps).

👉 **Read the full honest guide:** [`docs/REPLACE_ANDROID.md`](docs/REPLACE_ANDROID.md)

That document covers:
- Why flashing over Android does not keep camera/modem working by magic
- Stages from UART bring-up → display/touch → Wi‑Fi → camera → **telephony (SIM/SMS/data)**
- Control Center feature targets (mobile data, torch, airplane mode, …)
- What **companies / OEMs** must fund to ship a real device

---

## Quick start — PC (QEMU)

```bash
git clone https://github.com/Tajudeen001-security/JagX-Operating-System.git
cd JagX-Operating-System/kernel
make
make iso
qemu-system-i386 -cdrom ../boot/jagx.iso -m 128M -serial stdio \
  -netdev user,id=net0 -device virtio-net-pci,netdev=net0
```

### Dependencies (Debian/Ubuntu)

```bash
sudo apt install build-essential gcc-multilib qemu-system-x86 \
  grub-pc-bin xorriso mtools openssl
```

### Optional: sign kernel

```bash
cd boot && bash sign-kernel.sh ../kernel/jagx.kernel .
```

---

## Quick start — Mobile architecture (aarch64 QEMU)

```bash
sudo apt install gcc-aarch64-linux-gnu qemu-system-arm
cd kernel/arch/aarch64
aarch64-linux-gnu-as -c boot.S -o boot.o
aarch64-linux-gnu-ld -Ttext=0x40000000 boot.o -o jagx-a64.elf
qemu-system-aarch64 -M virt -cpu cortex-a72 -kernel jagx-a64.elf -nographic
```

This is **emulator bring-up**, not a flashable Tecno/Itel image.

More: [`docs/SETUP_MOBILE.md`](docs/SETUP_MOBILE.md) · [`mobile/`](mobile/)

---

## Feature goals on a future JagX phone

| Area | Target |
|------|--------|
| Camera | Working photo/video via JagX camera HAL |
| Settings | Full system settings |
| Messages | SMS (needs modem stack) |
| SIM / network | Signal, mobile data toggle |
| Control Center | Data, Wi‑Fi, Bluetooth, flashlight, airplane mode |
| Security | Capabilities, encryption, verified boot |

**Status:** designed and documented; hardware ports are per-device projects.

---

## Networking status (PC / QEMU)

Virtio-net · ARP · DHCP · IPv4 · UDP · DNS · TCP · HTTP · TLS ClientHello + ServerHello detection scaffold

HTTPS is **not** complete until full TLS keys/certs exist.

---

## For companies & OEMs

1. Evaluate in QEMU using this repo  
2. Read `docs/SECURITY.md`, `docs/REPLACE_ANDROID.md`  
3. Select **one** reference phone with unlockable bootloader  
4. Assign BSP + telephony + camera teams  
5. Do not promise end users a ROM-style install until drivers exist  

Keep **MIT license** and founder attribution: **Gbadamosi Tajudeen Olajide**.

---

## Project layout

```
kernel/   net/   crypto/   compositor/   browser/
userland/ mobile/   boot/   docs/
```

## Version

**v0.0.14** — TLS handshake scaffold · Android→JagX replacement guide

---

**JagX** — Own your computer. Own your phone.
