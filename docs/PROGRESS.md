# JagX v0.0.7

## Real implementations added

### Input & Windowing
- Live mouse cursor redraw on every packet
- Window dragging by title bar (left button)
- Proper focus + chrome (shadow, title, close, focus line)

### Networking
- Virtio-net: real PCI BAR discovery, status machine (ACK → DRIVER → FEATURES_OK → DRIVER_OK)
- Transmit entry point ready for ring completion

### aarch64
- MMU + I/D caches enabled
- Two 1GB identity blocks
- Generic timer (CNTP) enabled
- GIC distributor minimal bring-up

### Security
- Dedicated SECURITY.md with capability-oriented roadmap
- Principles locked in for all future work

## Boot

Use the ISO path for reliable Multiboot2 framebuffer:

```bash
cd kernel && make && make iso
qemu-system-i386 -cdrom ../boot/jagx.iso -m 128M -serial stdio \
  -netdev user,id=net0 -device virtio-net-pci,netdev=net0
```
