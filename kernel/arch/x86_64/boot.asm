; ============================================================
; JagX OS - Boot entry point (Multiboot + protected mode)
; ============================================================
; Loaded by GRUB in 32-bit protected mode with flat segments.
; We set up our own GDT, then jump to C kernel.

section .multiboot
align 4
    dd 0x1BADB002              ; Multiboot magic
    dd 0x00                    ; Flags (none for now)
    dd -(0x1BADB002 + 0x00)    ; Checksum

section .text
global _start
global gdt_flush
global idt_load
global irq_common_stub

extern kernel_main
extern irq_handler

_start:
    ; Set up a temporary stack
    mov esp, stack_top

    ; Jump into the C kernel (GDT/IDT will be set up there)
    call kernel_main

.hang:
    cli
    hlt
    jmp .hang

; ------------------------------------------------------------
; gdt_flush - load new GDT and reload segment registers
; void gdt_flush(uint32_t gdt_ptr)
; ------------------------------------------------------------
gdt_flush:
    mov eax, [esp + 4]         ; Get pointer to GDT descriptor
    lgdt [eax]

    mov ax, 0x10               ; Data segment selector (index 2)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Far jump to reload CS (code segment = index 1 = 0x08)
    jmp 0x08:.flush
.flush:
    ret

; ------------------------------------------------------------
; idt_load - load IDT
; void idt_load(uint32_t idt_ptr)
; ------------------------------------------------------------
idt_load:
    mov eax, [esp + 4]
    lidt [eax]
    ret

; ------------------------------------------------------------
; Common IRQ stub - saves state, calls C handler, restores
; ------------------------------------------------------------
%macro IRQ_STUB 1
global irq%1
irq%1:
    cli
    push byte 0                ; Dummy error code
    push byte %1               ; IRQ number
    jmp irq_common_stub
%endmacro

IRQ_STUB 0
IRQ_STUB 1
IRQ_STUB 2
IRQ_STUB 3
IRQ_STUB 4
IRQ_STUB 5
IRQ_STUB 6
IRQ_STUB 7
IRQ_STUB 8
IRQ_STUB 9
IRQ_STUB 10
IRQ_STUB 11
IRQ_STUB 12
IRQ_STUB 13
IRQ_STUB 14
IRQ_STUB 15

irq_common_stub:
    pusha                      ; Save all general purpose registers

    mov ax, ds
    push eax                   ; Save data segment

    mov ax, 0x10               ; Load kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call irq_handler           ; Call C handler

    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa
    add esp, 8                 ; Clean up error code + IRQ number
    sti
    iret

; ------------------------------------------------------------
; Exception stubs (some have error codes, some don't)
; ------------------------------------------------------------
%macro ISR_NOERR 1
global isr%1
isr%1:
    cli
    push byte 0
    push byte %1
    jmp isr_common_stub
%endmacro

%macro ISR_ERR 1
global isr%1
isr%1:
    cli
    push byte %1
    jmp isr_common_stub
%endmacro

ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR   8
ISR_NOERR 9
ISR_ERR   10
ISR_ERR   11
ISR_ERR   12
ISR_ERR   13
ISR_ERR   14
ISR_NOERR 15
ISR_NOERR 16
ISR_ERR   17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_ERR   30
ISR_NOERR 31

extern isr_handler

isr_common_stub:
    pusha

    mov ax, ds
    push eax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call isr_handler

    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa
    add esp, 8
    sti
    iret

section .bss
align 16
stack_bottom:
    resb 32768                 ; 32 KiB stack
stack_top:
