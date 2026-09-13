# JagX Operating System

Original from-scratch OS for PC and Mobile (no Linux/Android/iOS base).

## v0.0.6 Highlights (Real code)

- **aarch64**: MMU identity map actually enabled + UART
- **PS/2 mouse** driver (position + buttons)
- **Compositor** draws real window chrome (title bar, close button, shadow, cursor)
- **Virtio-net** PCI detection
- **GRUB + ISO** build path for reliable Multiboot2 framebuffer

## Quick start

```bash
cd kernel
make
make run          # text / limited FB

# For proper graphical boot:
make iso
qemu-system-i386 -cdrom ../boot/jagx.iso -m 128M -serial stdio
```

## Design
Dark theme · Electric teal · Soft purple · Built for both desktop and excellent mobile experience.

https://github.com/Tajudeen001-security/JagX-Operating-System
