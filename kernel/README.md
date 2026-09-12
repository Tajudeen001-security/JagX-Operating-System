# JagX Kernel v0.0.2

## Features now included

1. **Improved keyboard** – Shift and Caps Lock support
2. **Physical Memory Manager (PMM)** – Bitmap-based page allocator
3. **Kernel Heap** – Simple bump allocator (`kmalloc` / `kfree`)
4. **Basic Paging** – Identity-mapped first 4 MB, paging enabled
5. **RamFS** – In-memory filesystem with create / read / list
6. **Syscall stub** – Dispatcher for future userspace (SYS_WRITE, SYS_UPTIME, etc.)

Plus everything from v0.0.1 (GDT, IDT, PIC, timer, console).

## Build & Run

```bash
cd kernel
make
make run
```

You should see initialization messages for all new subsystems, a list of RamFS files, and a demo syscall message.

## Directory layout (new)

```
kernel/
├── arch/x86_64/     # Architecture specific (boot, GDT, IDT, drivers)
├── mm/              # Memory management (PMM, heap, paging)
├── fs/              # Filesystems (RamFS for now)
├── syscall/         # System call interface
└── ...
```

## Next major goals

- Proper Multiboot memory map parsing
- Full free-list / slab allocator
- Expand paging (more than 4 MB, user/kernel separation)
- Long mode (true 64-bit)
- Real userspace processes + `int 0x80` / syscall instruction
- ELF loader
- Better VFS layer
