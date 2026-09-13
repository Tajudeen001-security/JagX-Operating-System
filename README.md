# JagX Operating System

From-scratch OS for **PC** and **Mobile**. No Linux, Android, or iOS base.

## v0.0.7

- Live mouse cursor + **window dragging**
- Virtio-net real init sequence
- Stronger aarch64 (MMU + caches + timer + GIC start)
- Security architecture document (capability-first direction)

## Design language
Dark · Electric teal · Soft purple · Built for excellent mobile experience

## Build

```bash
cd kernel
make
make iso                    # GRUB ISO for real framebuffer
qemu-system-i386 -cdrom ../boot/jagx.iso -m 128M -serial stdio
```

https://github.com/Tajudeen001-security/JagX-Operating-System

MIT License
