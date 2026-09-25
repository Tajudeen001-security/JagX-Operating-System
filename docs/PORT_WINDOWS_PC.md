# JagX on PC (Windows-class machines)

**Target:** PCs that normally run **Windows** (and can also dual-boot or run from USB/ISO).

JagX PC build is **x86 / QEMU-first** today — not a Windows program; it is a separate OS image.

## Device identity

| Field | Value |
|-------|--------|
| **PC type** | Desktop / Laptop |
| **Maker / model** | _________________ |
| **CPU** | _________________ |
| **Firmware** | BIOS / UEFI |
| **Secure Boot** | On / Off |

## How to use JagX on a Windows PC (lab)

1. On Windows: install QEMU or use a spare machine
2. Build ISO: `cd kernel && make && make iso`
3. Boot ISO in QEMU **or** USB (only if you accept risk; backup first)
4. Do **not** wipe the only Windows install until dual-boot is tested

## Checklist

- [ ] Boots under QEMU on this CPU
- [ ] Multiboot2 / GRUB framebuffer works
- [ ] Keyboard + mouse
- [ ] virtio-net or Ethernet driver path
- [ ] Optional: real hardware boot from USB
- [ ] Optional: dual-boot menu next to Windows

## Government / office PCs

Prefer **lab VMs** and dedicated pilot machines. Keep Windows for production until L2 certification (see `docs/CERTIFICATION.md`).

**Pilot PC name:** _______________  
**Owner:** _______________
