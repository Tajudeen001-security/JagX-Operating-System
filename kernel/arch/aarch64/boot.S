// JagX aarch64 v0.0.7 - Stronger early kernel
// Real: UART, exception vectors, identity MMU, timer stub, GIC notes

.section ".text.boot"
.global _start

.equ UART_DR,  0x09000000
.equ UART_FR,  0x09000018
.equ GICD_BASE, 0x08000000
.equ GICC_BASE, 0x08010000
.equ TIMER_BASE, 0x08000000   // placeholder - use generic timer

_start:
    mrs     x0, mpidr_el1
    and     x0, x0, #0xFF
    cbnz    x0, park

    ldr     x0, =stack_top
    mov     sp, x0

    ldr     x0, =vectors
    msr     vbar_el1, x0
    isb

    adr     x0, msg_boot
    bl      uart_puts

    bl      mmu_init
    adr     x0, msg_mmu
    bl      uart_puts

    // Enable generic timer (CNTP) - basic
    bl      timer_init_el1
    adr     x0, msg_timer
    bl      uart_puts

    // GIC distributor minimal init note
    bl      gic_init_minimal
    adr     x0, msg_gic
    bl      uart_puts

1:  wfe
    b       1b

park:
    wfe
    b       park

uart_putchar:
    ldr     x1, =UART_FR
1:  ldr     w2, [x1]
    and     w2, w2, #0x20
    cbnz    w2, 1b
    ldr     x1, =UART_DR
    strb    w0, [x1]
    ret

uart_puts:
    mov     x19, x0
1:  ldrb    w0, [x19], #1
    cbz     w0, 2f
    bl      uart_putchar
    b       1b
2:  ret

mmu_init:
    ldr     x0, =ttbr0_base
    mov     x1, #0
    mov     x2, #512
1:  str     x1, [x0], #8
    subs    x2, x2, #1
    b.ne    1b

    ldr     x0, =ttbr0_base
    // 1GB block, AF=1, valid
    mov     x1, #0x701
    str     x1, [x0]

    // Second 1GB
    mov     x1, #0x40000701
    str     x1, [x0, #8]

    ldr     x1, =0xFF
    msr     mair_el1, x1

    // TCR: 40-bit IPA, 4K, T0SZ=25 (approx)
    ldr     x1, =0x00000015B5103519
    msr     tcr_el1, x1

    ldr     x0, =ttbr0_base
    msr     ttbr0_el1, x0

    tlbi    vmalle1is
    dsb     sy
    isb

    mrs     x0, sctlr_el1
    orr     x0, x0, #(1 << 0)   // M
    orr     x0, x0, #(1 << 2)   // C
    orr     x0, x0, #(1 << 12)  // I
    msr     sctlr_el1, x0
    isb
    ret

timer_init_el1:
    // Enable EL1 physical timer interrupts later; for now just program frequency view
    mrs     x0, cntfrq_el0
    // Store or print frequency in future
    // Enable timer (CNTP_CTL_EL0 bit 0) - safe minimal
    mov     x0, #1
    msr     cntp_ctl_el0, x0
    ret

gic_init_minimal:
    // GIC distributor: enable forward group0/1 (very early)
    ldr     x0, =GICD_BASE
    mov     w1, #3              // enable groups
    str     w1, [x0]            // GICD_CTLR
    ret

.align 11
vectors:
    .rept 16
    .align 7
    b       exc_handler
    .endr

exc_handler:
    adr     x0, msg_exc
    bl      uart_puts
1:  wfe
    b       1b

msg_boot:  .asciz "\r\n[JagX a64] Boot\r\n"
msg_mmu:   .asciz "[JagX a64] MMU + caches enabled (identity)\r\n"
msg_timer: .asciz "[JagX a64] Generic timer enabled\r\n"
msg_gic:   .asciz "[JagX a64] GIC distributor basic init\r\n"
msg_exc:   .asciz "[JagX a64] Exception\r\n"

.section .bss
.align 12
ttbr0_base:
    .skip 4096
.align 16
stack_bottom:
    .skip 16384
stack_top:
