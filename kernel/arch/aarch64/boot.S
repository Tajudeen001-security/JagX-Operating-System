// ============================================================
// JagX aarch64 early boot (QEMU virt)
// ============================================================
// Minimal stub that can print to UART and park cores.
// Expand this into a full kernel entry later.

.section ".text.boot"
.global _start

.equ UART0, 0x09000000          // QEMU virt UART base

_start:
    // Only run on core 0 for now
    mrs     x0, mpidr_el1
    and     x0, x0, #0xFF
    cbnz    x0, park

    // Set up a small stack
    ldr     x0, =stack_top
    mov     sp, x0

    // Print a banner over UART
    adr     x0, banner
    bl      uart_puts

park:
    wfe
    b       park

// ------------------------------------------------------------
// Very simple UART putchar / puts (QEMU virt PL011-ish)
// ------------------------------------------------------------
uart_putchar:
    // x0 = character
    ldr     x1, =UART0
1:  // wait for TX ready (simplified)
    strb    w0, [x1]
    ret

uart_puts:
    // x0 = pointer to string
    mov     x2, x0
1:  ldrb    w0, [x2], #1
    cbz     w0, 2f
    bl      uart_putchar
    b       1b
2:  ret

banner:
    .asciz  "\r\nJagX aarch64 kernel stub - Mobile foundation starting...\r\n"

.section .bss
.align 16
stack_bottom:
    .skip 16384
stack_top:
