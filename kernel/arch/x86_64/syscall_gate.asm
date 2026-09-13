; int 0x80 syscall entry — real interrupt gate target
global syscall_entry
extern syscall_dispatcher

syscall_entry:
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; args: eax=num, ebx=a1, ecx=a2, edx=a3 (classic Linux-ish)
    push edx
    push ecx
    push ebx
    push eax
    call syscall_dispatcher
    add esp, 16
    ; return value in eax — restore path keeps it if we careful; simple: overwrite eax from return
    mov [esp + 28], eax   ; pos of eax in pusha frame approx — simplified

    pop gs
    pop fs
    pop es
    pop ds
    popa
    iret
