# JagX Operating System

**Founder:** Gbadamosi Tajudeen Olajide  
**License:** MIT  
**Repository:** https://github.com/Tajudeen001-security/JagX-Operating-System

JagX is an **original** operating system for **PC** and **Mobile**, built from scratch — **not** based on Linux, Android, or iOS.

It aims for:
- Independent kernel and drivers
- Strong security (capabilities, encryption, secure boot path)
- Excellent mobile experience
- Native browser (**JagBrowser**) with networking path (DNS / TCP / HTTP in progress)

---

## Table of contents

1. [What you need](#1-what-you-need)
2. [Get the source](#2-get-the-source)
3. [Build the kernel (PC / x86)](#3-build-the-kernel-pc--x86)
4. [Run in QEMU (quick test)](#4-run-in-qemu-quick-test)
5. [Build a bootable ISO (recommended)](#5-build-a-bootable-iso-recommended)
6. [Networking in QEMU](#6-networking-in-qemu)
7. [Secure boot / sign the kernel](#7-secure-boot--sign-the-kernel)
8. [Mobile / aarch64 path](#8-mobile--aarch64-path)
9. [Using the system (current features)](#9-using-the-system-current-features)
10. [For companies / integrators](#10-for-companies--integrators)
11. [Project layout](#11-project-layout)
12. [Roadmap & limitations](#12-roadmap--limitations)
13. [Contributing & license](#13-contributing--license)

---

## 1. What you need

### On a Linux development PC (recommended)

```bash
# Debian / Ubuntu style
sudo apt update
sudo apt install build-essential gcc-multilib nasm qemu-system-x86 \
  grub-pc-bin grub-common xorriso mtools openssl

# Optional cross tools for cleaner builds
# sudo apt install gcc-i686-linux-gnu binutils-i686-linux-gnu
```

### Tools used
| Tool | Purpose |
|------|---------|
| `gcc` / `i686-elf-gcc` | Compile kernel (32-bit freestanding) |
| `as` / `ld` | Assemble & link |
| `qemu-system-i386` | Emulate PC |
| `grub-mkrescue` | Bootable ISO with Multiboot2 |
| `openssl` | Optional kernel signing |
| `qemu-system-aarch64` | Mobile/ARM64 early tests |

---

## 2. Get the source

```bash
git clone https://github.com/Tajudeen001-security/JagX-Operating-System.git
cd JagX-Operating-System
```

---

## 3. Build the kernel (PC / x86)

```bash
cd kernel
make clean
make
```

Output: `kernel/jagx.kernel`

If you have an `i686-elf` cross-compiler in `PATH`, the Makefile prefers it; otherwise it uses `gcc -m32`.

---

## 4. Run in QEMU (quick test)

```bash
cd kernel
make run
```

Or manually:

```bash
qemu-system-i386 -kernel jagx.kernel -m 128M -serial stdio
```

**Note:** Plain `-kernel` may not always provide a Multiboot2 framebuffer. Text mode still works. For graphics, use the ISO method below.

---

## 5. Build a bootable ISO (recommended)

This uses **GRUB + Multiboot2** so the framebuffer request is handled properly.

```bash
cd kernel
make
make iso
```

That runs `boot/build-iso.sh` and produces `boot/jagx.iso`.

Boot it:

```bash
qemu-system-i386 -cdrom ../boot/jagx.iso -m 128M -serial stdio
```

### On real PC hardware (advanced / experimental)

> Early development only. Do **not** wipe production disks. Use a spare machine or USB.

1. Build `jagx.iso` as above.
2. Flash to USB (example — **double-check the device name**):
   ```bash
   sudo dd if=boot/jagx.iso of=/dev/sdX bs=4M status=progress conv=fsync
   ```
3. Boot from USB in the BIOS/UEFI boot menu.
4. Expect limited hardware support (keyboard/mouse/PCI virtio paths were developed primarily under QEMU).

Companies evaluating JagX should start in **QEMU or a dedicated lab PC**, not production fleets.

---

## 6. Networking in QEMU

```bash
qemu-system-i386 -cdrom boot/jagx.iso -m 128M -serial stdio \
  -netdev user,id=net0 -device virtio-net-pci,netdev=net0
```

This enables:
- Virtio-net TX/RX
- IPv4 + UDP
- DNS queries
- TCP SYN / SYN-ACK / data path
- HTTP GET attempts (port 80)

**Still evolving:** DHCP/ARP polish, TLS (HTTPS), full browser rendering.

---

## 7. Secure boot / sign the kernel

```bash
cd kernel && make
cd ../boot
bash sign-kernel.sh ../kernel/jagx.kernel .
```

Produces:
- `jagx.kernel.sha256` — content hash
- Optional `jagx.kernel.sig` + dev keys via OpenSSL

The kernel also runs an early **SHA-256 integrity check** at boot (`boot_verify_marker`).

Production secure boot (platform keys, measured boot) is documented in `docs/SECURE_BOOT.md` and not finished for mass devices yet.

---

## 8. Mobile / aarch64 path

Mobile targets **ARM64 (aarch64)**. Early bring-up lives in `kernel/arch/aarch64/`.

### Build & run the aarch64 stub (QEMU virt)

```bash
# Toolchain example (Debian/Ubuntu)
sudo apt install gcc-aarch64-linux-gnu qemu-system-arm

cd kernel/arch/aarch64
aarch64-linux-gnu-as -c boot.S -o boot.o
aarch64-linux-gnu-ld -Ttext=0x40000000 boot.o -o jagx-a64.elf

qemu-system-aarch64 -M virt -cpu cortex-a72 -kernel jagx-a64.elf -nographic
```

You should see UART messages for boot, MMU, timer, and GIC init.

### Real phones / tablets

Bringing JagX up on physical mobile hardware requires:
- Device-specific bootloader / unlock model
- Display, touch, power, modem drivers
- Verified boot aligned with the device SoC

That work is **not** product-ready. Partners should contact the project and plan a **BSP (board support package)** effort per SoC.

Design docs for mobile UX, power, touch, and security are under `mobile/` and `docs/`.

---

## 9. Using the system (current features)

After boot you should see serial/console messages such as:
- Multiboot / memory init
- GDT + TSS + user segments
- Virtio-net, DNS, TCP
- Crypto self-tests
- Optional graphical windows (if framebuffer is active)
- Transition toward ring-3 user mode

**Keyboard:** type in the console (Shift / Caps supported).  
**Mouse:** PS/2 driver; window drag when framebuffer is active.  
**HTTP:** kernel can attempt `http_get("example.com", "/")` after DNS + TCP.

JagBrowser live UI search is **architected** but full web rendering is not finished.

---

## 10. For companies / integrators

### Evaluation checklist
1. Clone and build on Linux CI
2. Run ISO under QEMU with virtio-net
3. Review `docs/SECURITY.md`, `docs/ENCRYPTION.md`, `docs/SECURE_BOOT.md`
4. Review mobile design under `mobile/`
5. Do **not** deploy to end users until drivers, update system, and verified boot are complete

### Integration options
| Goal | Approach |
|------|----------|
| Research / OS education | QEMU + source tree as-is |
| Embedded PC appliance | Port drivers for your board; keep MIT attribution |
| Mobile product | Fund BSP + certification; aarch64 kernel is the starting point |
| Cloud / CI testing | Script `make iso` + QEMU smoke tests |

### Support expectations
This is an **early open-source OS**. There is no SLA. Companies should fork, assign maintainers, and contribute drivers upstream when possible.

### Branding & founder
Please keep attribution to **Gbadamosi Tajudeen Olajide** and the MIT license notice when redistributing.

---

## 11. Project layout

```
JagX-Operating-System/
├── kernel/           # x86 kernel, mm, fs, syscalls
│   └── arch/
│       ├── x86_64/   # PC boot, GDT/TSS, drivers
│       └── aarch64/  # Mobile early boot
├── net/              # virtio-net, IPv4, UDP, DNS, TCP, HTTP
├── crypto/           # SHA-256, ChaCha20-Poly1305
├── compositor/       # Early window system
├── browser/          # JagBrowser architecture + search API
├── userland/         # Process table / future init
├── mobile/           # Mobile design principles
├── boot/             # GRUB config, ISO script, signing
└── docs/             # Security, encryption, progress, founder
```

---

## 12. Roadmap & limitations

**Working / in progress**
- Boot (Multiboot2), interrupts, memory, paging basics
- Keyboard, mouse, framebuffer path
- Virtio-net, IPv4/UDP/DNS/TCP/HTTP path
- Crypto primitives, secure boot hashing
- Ring-3 entry path

**Not ready for production**
- Full desktop/mobile UI product
- HTTPS / TLS
- Complete driver set for arbitrary PCs/phones
- App store, updater, carrier certification

---

## 13. Contributing & license

- Open issues / PRs on GitHub
- Prefer small, testable changes with QEMU steps
- MIT License — see `LICENSE`

**JagX** — Own your computer. Own your phone.

*— Gbadamosi Tajudeen Olajide, Founder*
