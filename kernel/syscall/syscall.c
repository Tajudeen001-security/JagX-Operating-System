#include "syscall.h"
#include "../arch/x86_64/console.h"
#include "../arch/x86_64/timer.h"
#include "../fs/ramfs.h"

/* Very early syscall interface.
 * Later this will be triggered by int 0x80 or sysenter/syscall instruction.
 * For now we just provide the dispatcher that can be called from kernel or future userspace.
 */

int syscall_handler(uint32_t num, uint32_t arg1, uint32_t arg2, uint32_t arg3) {
    (void)arg3;

    switch (num) {
        case SYS_WRITE: {
            /* arg1 = pointer to string (kernel address for now) */
            console_write((const char*)arg1);
            return 0;
        }
        case SYS_UPTIME: {
            return (int)timer_get_ticks();
        }
        case SYS_GETPID: {
            return 1;  /* Only kernel "process" for now */
        }
        case SYS_EXIT: {
            console_write("[SYS] Process exited\n");
            return 0;
        }
        default:
            console_write("[SYS] Unknown syscall\n");
            return -1;
    }
}

void syscall_init(void) {
    console_write("[SYSCALL] Dispatcher ready (numbers 1-5)\n");
    /* Future: install int 0x80 gate in IDT here */
}
