CC ?= i686-elf-gcc
AS ?= i686-elf-as
LD ?= i686-elf-ld
ifeq (,$(shell which i686-elf-gcc 2>/dev/null))
  CC = gcc -m32
  AS = as --32
  LD = ld -m elf_i386
endif
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fno-stack-protector -fno-pic -mno-red-zone \
  -I arch/x86_64 -I mm -I fs -I syscall -I ../compositor -I ../net -I ../userland -I ../crypto -I ../mobile -I ../mobile/hal
LDFLAGS = -T linker.ld -nostdlib
OBJS = arch/x86_64/boot.o arch/x86_64/gdt.o arch/x86_64/idt.o arch/x86_64/pic.o \
  arch/x86_64/timer.o arch/x86_64/keyboard.o arch/x86_64/mouse.o arch/x86_64/irq.o \
  arch/x86_64/console.o arch/x86_64/framebuffer.o arch/x86_64/multiboot.o \
  arch/x86_64/boot_verify.o arch/x86_64/kernel.o \
  mm/pmm.o mm/heap.o mm/paging.o fs/ramfs.o syscall/syscall.o \
  ../compositor/compositor.o ../net/net.o ../net/virtio_net.o ../net/dns.o ../net/tcp.o \
  ../net/ipv4.o ../net/udp.o ../net/http.o ../net/arp.o ../net/dhcp.o ../net/tls.o ../net/tls_crypto.o \
  ../userland/process.o ../crypto/sha256.o ../crypto/aead.o ../crypto/chacha20poly1305.o \
  ../mobile/control_center.o ../mobile/settings.o ../mobile/gallery.o \
  ../mobile/screenshot.o ../mobile/screencast.o ../mobile/features.o \
  ../mobile/notifications.o ../mobile/filemanager.o \
  ../mobile/hal/camera.o ../mobile/hal/lights.o ../mobile/hal/radio.o ../mobile/hal/wifi.o
.PHONY: all clean run iso
all: jagx.kernel
jagx.kernel: $(OBJS) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJS)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.asm
	$(AS) $< -o $@
clean:
	rm -f $(OBJS) jagx.kernel
run: jagx.kernel
	qemu-system-i386 -kernel jagx.kernel -m 128M -serial stdio \
	  -netdev user,id=n0 -device virtio-net-pci,netdev=n0
iso: jagx.kernel
	cd ../boot && bash build-iso.sh
