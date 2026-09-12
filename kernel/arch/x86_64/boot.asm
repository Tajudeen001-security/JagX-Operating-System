; JagX OS – Minimal Multiboot-compatible header + entry point
; This is a starting skeleton only.

section .multiboot
align 4
    dd 0x1BADB002              ; Magic
    dd 0x00                    ; Flags
    dd -(0x1BADB002 + 0x00)    ; Checksum

section .text
global _start
_start:
    ; Set up a simple stack
    mov esp, stack_top

    ; Call the C kernel
    extern kernel_main
    call kernel_main

    ; Hang if kernel returns
.hang:
    cli
    hlt
    jmp .hang

section .bss
align 16
stack_bottom:
    resb 16384                 ; 16 KiB stack
stack_top:
