// ============================================================
// JagX aarch64 - Real early boot with identity-mapped MMU
// Target: QEMU virt (cortex-a72 / a53)
// ============================================================

.section ".text.boot"
.global _start

.equ UART_DR,   0x09000000
.equ UART_FR,   0x09000018

_start:
    // Only core 0 continues
    mrs     x0, mpidr_el1
    and     x0, x0, #0xFF
    cbnz    x0, park

    // Stack
    ldr     x0, =stack_top
    mov     sp, x0

    // Install exception vectors early
    ldr     x0, =vectors
    msr     vbar_el1, x0
    isb

    // Print before MMU
    adr     x0, msg_boot
    bl      uart_puts

    // -------------------------------------------------------
    // Basic identity map MMU setup (4K granule, 1:1 mapping)
    // This is a minimal real enable sequence for early bring-up.
    // -------------------------------------------------------
    bl      mmu_init

    adr     x0, msg_mmu
    bl      uart_puts

    // Stay alive
1:  wfe
    b       1b

park:
    wfe
    b       park

// ------------------------------------------------------------
// UART (PL011-style on QEMU virt) - real polling TX
// ------------------------------------------------------------
uart_putchar:
    ldr     x1, =UART_FR
1:  ldr     w2, [x1]
    and     w2, w2, #0x20          // TXFF
    cbnz    w2, 1b
    ldr     x1, =UART_DR
    strb    w0, [x1]
    ret

uart_puts:
    mov     x19, x0                // save pointer
1:  ldrb    w0, [x19], #1
    cbz     w0, 2f
    bl      uart_putchar
    b       1b
2:  ret

// ------------------------------------------------------------
// Minimal MMU identity map (TTBR0, 4K, 39-bit VA for simplicity)
// Maps first 1GB 1:1 as block descriptors (very early style)
// ------------------------------------------------------------
mmu_init:
    // Use a simple page table in .bss
    ldr     x0, =ttbr0_base

    // Clear table (512 entries * 8 bytes)
    mov     x1, #0
    mov     x2, #512
1:  str     x1, [x0], #8
    subs    x2, x2, #1
    b.ne    1b

    ldr     x0, =ttbr0_base

    // Level 1 block entry for 0x00000000 - 0x40000000 (1GB)
    // Attr: AF=1, AP=00 (RW), AttrIdx=0, Block=1
    // Valid block descriptor bits for 1GB at level 1
    mov     x1, #0x00000701        // basic valid block + AF
    str     x1, [x0]

    // MAIR_EL1: Attr0 = normal memory (0xFF), Attr1 = device (0x00)
    ldr     x1, =0x000000000000FF
    msr     mair_el1, x1

    // TCR_EL1: 39-bit, 4K granule, inner/outer WB WA
    ldr     x1, =0x00000015B5103510
    msr     tcr_el1, x1

    // TTBR0_EL1
    ldr     x0, =ttbr0_base
    msr     ttbr0_el1, x0

    // Invalidate TLBs
    tlbi    vmalle1
    dsb     sy
    isb

    // Enable MMU (SCTLR_EL1.M = 1) + caches later
    mrs     x0, sctlr_el1
    orr     x0, x0, #1             // M bit
    msr     sctlr_el1, x0
    isb

    ret

// ------------------------------------------------------------
// Exception vectors
// ------------------------------------------------------------
.align 11
vectors:
    .align 7
    b       exc_handler
    .align 7
    b       exc_handler
    .align 7
    b       exc_handler
    .align 7
    b       exc_handler

    .align 7
    b       exc_handler
    .align 7
    b       exc_handler
    .align 7
    b       exc_handler
    .align 7
    b       exc_handler

    .align 7
    b       exc_handler
    .align 7
    b       exc_handler
    .align 7
    b       exc_handler
    .align 7
    b       exc_handler

    .align 7
    b       exc_handler
    .align 7
    b       exc_handler
    .align 7
    b       exc_handler
    .align 7
    b       exc_handler

exc_handler:
    adr     x0, msg_exc
    bl      uart_puts
1:  wfe
    b       1b

msg_boot:
    .asciz  "\r\n[JagX aarch64] Boot core 0 - UART live\r\n"
msg_mmu:
    .asciz  "[JagX aarch64] MMU identity map enabled\r\n"
msg_exc:
    .asciz  "[JagX aarch64] Exception taken\r\n"

.section .bss
.align 12                          // 4K aligned page tables
ttbr0_base:
    .skip 4096
.align 16
stack_bottom:
    .skip 16384
stack_top:
