# ============================================================
# JagX Kernel Makefile
# ============================================================

CC      ?= i686-elf-gcc
AS      ?= i686-elf-as
LD      ?= i686-elf-ld

ifeq (,$(shell which i686-elf-gcc 2>/dev/null))
  CC = gcc -m32
  AS = as --32
  LD = ld -m elf_i386
endif

CFLAGS  = -std=gnu99 -ffreestanding -O2 -Wall -Wextra \
          -fno-stack-protector -fno-pic -mno-red-zone \
          -I arch/x86_64 -I mm -I fs -I syscall

ASFLAGS =
LDFLAGS = -T linker.ld -nostdlib

OBJS = arch/x86_64/boot.o \
       arch/x86_64/gdt.o \
       arch/x86_64/idt.o \
       arch/x86_64/pic.o \
       arch/x86_64/timer.o \
       arch/x86_64/keyboard.o \
       arch/x86_64/irq.o \
       arch/x86_64/console.o \
       arch/x86_64/framebuffer.o \
       arch/x86_64/kernel.o \
       mm/pmm.o \
       mm/heap.o \
       mm/paging.o \
       fs/ramfs.o \
       syscall/syscall.o

.PHONY: all clean run

all: jagx.kernel

jagx.kernel: $(OBJS) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJS)
	@echo "Kernel built: jagx.kernel"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -f $(OBJS) jagx.kernel

run: jagx.kernel
	qemu-system-i386 -kernel jagx.kernel -m 128M -serial stdio
