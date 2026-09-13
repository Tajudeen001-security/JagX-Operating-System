// JagX aarch64 early boot stub (QEMU virt)
// This is a minimal placeholder that will be expanded.

.section ".text.boot"
.global _start

_start:
    // For now just park the core.
    // Real code will:
    // - Set up stack
    // - Clear BSS
    // - Set exception vectors
    // - Enable MMU later
    // - Jump to kernel_main

1:  wfe
    b   1b
