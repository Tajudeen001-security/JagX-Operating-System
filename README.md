# JagX Operating System

**Founder:** Gbadamosi Tajudeen Olajide  
**License:** MIT  
**Repo:** https://github.com/Tajudeen001-security/JagX-Operating-System

Original OS for **PC** and **Mobile** (not Linux/Android/iOS based).

## Quick start (PC)

```bash
git clone https://github.com/Tajudeen001-security/JagX-Operating-System.git
cd JagX-Operating-System/kernel
make
make iso
qemu-system-i386 -cdrom ../boot/jagx.iso -m 128M -serial stdio \
  -netdev user,id=net0 -device virtio-net-pci,netdev=net0
```

## Full install / setup guide

See the detailed sections below (and `docs/SETUP_PC.md`, `docs/SETUP_MOBILE.md`).

### 1. Dependencies (Linux)

```bash
sudo apt install build-essential gcc-multilib qemu-system-x86 \
  grub-pc-bin xorriso mtools openssl
```

### 2. Build

```bash
cd kernel && make
```

### 3. ISO (recommended)

```bash
make iso
# → boot/jagx.iso
```

### 4. Run with networking

```bash
qemu-system-i386 -cdrom ../boot/jagx.iso -m 128M -serial stdio \
  -netdev user,id=net0 -device virtio-net-pci,netdev=net0
```

### 5. Sign kernel (optional)

```bash
cd boot && bash sign-kernel.sh ../kernel/jagx.kernel .
```

### 6. Mobile (aarch64 QEMU)

```bash
cd kernel/arch/aarch64
aarch64-linux-gnu-as -c boot.S -o boot.o
aarch64-linux-gnu-ld -Ttext=0x40000000 boot.o -o jagx-a64.elf
qemu-system-aarch64 -M virt -cpu cortex-a72 -kernel jagx-a64.elf -nographic
```

### 7. Real hardware

Experimental only. Use spare machines. Flash ISO to USB with `dd` only if you understand the risk. Mobile phones need a full BSP — not a consumer installer yet.

### 8. Companies

Evaluate in QEMU/lab. Review `docs/SECURITY.md`. Do not ship to end users until drivers, updates, and verified boot are complete. Keep MIT attribution and founder credit.

## Current networking stack

Virtio-net · ARP · DHCP · IPv4 · UDP · DNS · TCP · HTTP · TLS ClientHello foundation

## Version

v0.0.13 — ARP/DHCP + TLS foundation
