# JagX Kernel Progress

## v0.0.1
- GDT, IDT, PIC, Timer, basic keyboard, VGA console

## v0.0.2 (current)
Implemented the next 6 items:

1. **Better keyboard** – Left/Right Shift + Caps Lock
2. **Physical Memory Manager** – Bitmap page frame allocator (up to 128 MB)
3. **Kernel Heap** – Bump-style `kmalloc`
4. **Paging** – Basic 32-bit paging with identity mapping of the first 4 MB
5. **RamFS** – Simple in-memory file store with demo files
6. **Syscall interface** – Numbered dispatcher ready for userspace later

## Testing checklist

- [x] Boots and shows all init messages
- [x] Timer dots appear
- [x] Keyboard works with Shift / Caps
- [x] PMM reports free pages
- [x] Paging enabled without crashing
- [x] RamFS lists welcome.txt and version
- [x] Demo syscall prints a message

## Known limitations (expected at this stage)

- Heap cannot free individual blocks yet
- Only first 4 MB is mapped
- No Multiboot memory map used (hard-coded 128 MB assumption)
- No real userspace yet (syscalls are callable from kernel only)
- Still 32-bit protected mode

These will be addressed in future iterations.
