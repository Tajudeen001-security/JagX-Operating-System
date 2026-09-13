// JagX aarch64 - Early boot with exception vectors + basic MMU prep
// Target: QEMU virt machine

.section ".text.boot"
.global _start

.equ UART0, 0x09000000

_start:
    mrs     x0, mpidr_el1
    and     x0, x0, #0xFF
    cbnz    x0, park_core

    // Set stack
    ldr     x0, =stack_top
    mov     sp, x0

    // Install exception vectors
    ldr     x0, =vectors
    msr     vbar_el1, x0

    // Print banner
    adr     x0, banner
    bl      uart_puts

    // Very early MMU note (full enable later)
    adr     x0, mmu_msg
    bl      uart_puts

park_core:
    wfe
    b       park_core

// ------------------------------------------------------------
uart_putchar:
    ldr     x1, =UART0
    strb    w0, [x1]
    ret

uart_puts:
    mov     x2, x0
1:  ldrb    w0, [x2], #1
    cbz     w0, 2f
    bl      uart_putchar
    b       1b
2:  ret

// ------------------------------------------------------------
// Exception vector table (simplified - each entry 0x80 bytes)
// ------------------------------------------------------------
.align 11
vectors:
    // Current EL with SP0
    .align 7
    b       exc_stub
    .align 7
    b       exc_stub
    .align 7
    b       exc_stub
    .align 7
    b       exc_stub

    // Current EL with SPx
    .align 7
    b       exc_stub
    .align 7
    b       exc_stub
    .align 7
    b       exc_stub
    .align 7
    b       exc_stub

    // Lower EL using AArch64
    .align 7
    b       exc_stub
    .align 7
    b       exc_stub
    .align 7
    b       exc_stub
    .align 7
    b       exc_stub

    // Lower EL using AArch32
    .align 7
    b       exc_stub
    .align 7
    b       exc_stub
    .align 7
    b       exc_stub
    .align 7
    b       exc_stub

exc_stub:
    // For now just hang on any exception
    wfe
    b       exc_stub

banner:
    .asciz  "\r\nJagX aarch64 - exceptions vector installed\r\n"
mmu_msg:
    .asciz  "MMU preparation point reached (identity map next)\r\n"

.section .bss
.align 16
stack_bottom:
    .skip 16384
stack_top:
