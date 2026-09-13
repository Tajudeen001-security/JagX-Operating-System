# JagX v0.0.6 - Real implementations

## What is actually implemented (not demo comments)

### aarch64
- Real UART TX with flag polling
- Exception vectors live in `vbar_el1`
- MMU turned on with identity mapping (SCTLR_EL1.M=1)
- Confirmed by serial messages after MMU enable

### x86 Graphics + Input
- Multiboot2 framebuffer tag request + parser
- Real pixel plot / fill when FB is present
- PS/2 mouse driver (IRQ12) with packet assembly and position tracking
- Compositor draws real window chrome: shadow, title bar, close button, focus line, cursor

### Networking
- PCI scan for virtio-net (real config space reads)
- Loopback interface
- Detection message when device is present under QEMU

### Boot
- GRUB config + ISO build script so Multiboot2 framebuffer can be reliably provided

## How to get graphical FB reliably

```bash
cd kernel && make
make iso          # requires grub-mkrescue
qemu-system-i386 -cdrom ../boot/jagx.iso -m 128M -serial stdio
```
