# JagX Kernel (Advanced Starter)

This is a significantly improved hobby-OS style kernel skeleton.

## Features included

- Multiboot-compliant entry point
- Global Descriptor Table (GDT)
- Interrupt Descriptor Table (IDT)
- Exception handlers (0-31) with messages
- PIC remapping (IRQs moved to 32-47)
- Programmable Interval Timer (PIT) at 100 Hz
- Basic PS/2 keyboard driver (US QWERTY)
- VGA text-mode console with scrolling, backspace, tabs
- Interrupt-driven main loop (`hlt`)

## Building

You need a 32-bit capable toolchain.

### Option A – Cross compiler (recommended)
```bash
# Install or build i686-elf-gcc / i686-elf-as / i686-elf-ld
make
```

### Option B – Host compiler with -m32 (Linux)
```bash
sudo apt install gcc-multilib   # or equivalent
make
```

## Running with QEMU
```bash
make run
# or manually:
qemu-system-i386 -kernel jagx.kernel
```

You should see the welcome messages, then dots appearing every second (timer),
and any keys you type will be echoed on the screen.

## Next steps (suggested)

1. Better keyboard (shift, caps, special keys)
2. Heap / physical memory manager
3. Paging
4. Move to long mode (true x86_64)
5. Simple filesystem / ramdisk
6. Userspace processes & syscalls

## Notes

This kernel currently runs in **32-bit protected mode** for simplicity and educational value.
A future milestone will switch to long mode (64-bit) while keeping the same overall design.
