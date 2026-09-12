# Very early Makefile for JagX kernel (x86_64)
# Requires a cross-compiler: x86_64-elf-gcc / x86_64-elf-as / x86_64-elf-ld

AS = x86_64-elf-as
CC = x86_64-elf-gcc
LD = x86_64-elf-ld

CFLAGS = -ffreestanding -O2 -Wall -Wextra -mno-red-zone -mno-mmx -mno-sse -mno-sse2

OBJS = arch/x86_64/boot.o arch/x86_64/kernel.o

all: jagx.kernel

jagx.kernel: $(OBJS) linker.ld
	$(LD) -T linker.ld -o $@ $(OBJS)

arch/x86_64/boot.o: arch/x86_64/boot.asm
	$(AS) $< -o $@

arch/x86_64/kernel.o: arch/x86_64/kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) jagx.kernel

.PHONY: all clean
