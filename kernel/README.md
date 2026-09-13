# JagX Kernel v0.0.5

## New in this version

- Multiboot2 + real framebuffer pixel drawing
- Compositor can draw simple colored windows
- aarch64 exception vectors + MMU prep
- Networking stub (loopback)

## Build

```bash
cd kernel
make
make run
```

For best graphical results later, boot via GRUB with Multiboot2 support.
