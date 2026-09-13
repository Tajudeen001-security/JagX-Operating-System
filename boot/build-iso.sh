#!/bin/bash
# Build a bootable ISO with GRUB so Multiboot2 + framebuffer works reliably

set -e

KERNEL=../kernel/jagx.kernel
ISO_DIR=isodir
ISO_NAME=jagx.iso

if [ ! -f "$KERNEL" ]; then
    echo "Kernel not found. Build it first: cd ../kernel && make"
    exit 1
fi

rm -rf "$ISO_DIR"
mkdir -p "$ISO_DIR/boot/grub"

cp "$KERNEL" "$ISO_DIR/boot/jagx.kernel"
cp grub.cfg "$ISO_DIR/boot/grub/grub.cfg"

grub-mkrescue -o "$ISO_NAME" "$ISO_DIR"

echo "Created $ISO_NAME"
echo "Run with:"
echo "  qemu-system-i386 -cdrom jagx.iso -m 128M -serial stdio"
echo "For networking add: -netdev user,id=net0 -device virtio-net-pci,netdev=net0"
