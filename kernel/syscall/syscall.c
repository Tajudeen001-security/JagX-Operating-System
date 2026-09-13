#include "syscall.h"
#include "../arch/x86_64/console.h"
#include "../arch/x86_64/timer.h"
#include "../arch/x86_64/idt.h"

extern void syscall_entry(void);

int syscall_dispatcher(uint32_t num, uint32_t a1, uint32_t a2, uint32_t a3) {
    (void)a2; (void)a3;
    switch (num) {
        case SYS_WRITE:
            console_write((const char*)a1);
            return 0;
        case SYS_UPTIME:
            return (int)timer_get_ticks();
        case SYS_GETPID:
            return 1;
        case SYS_EXIT:
            console_write("[SYS] exit\n");
            return 0;
        default:
            return -1;
    }
}

int syscall_handler(uint32_t num, uint32_t arg1, uint32_t arg2, uint32_t arg3) {
    return syscall_dispatcher(num, arg1, arg2, arg3);
}

void syscall_init(void) {
    /* Install int 0x80 as DPL=3 interrupt gate so userspace can call it */
    idt_set_gate(0x80, (uint32_t)syscall_entry, 0x08, 0xEE);
    console_write("[SYSCALL] int 0x80 gate installed (DPL=3)\n");
}
