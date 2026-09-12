#ifndef JAGX_SYSCALL_H
#define JAGX_SYSCALL_H

#include <stdint.h>

/* System call numbers */
#define SYS_WRITE   1
#define SYS_READ    2
#define SYS_EXIT    3
#define SYS_GETPID  4
#define SYS_UPTIME  5

void syscall_init(void);
int  syscall_handler(uint32_t num, uint32_t arg1, uint32_t arg2, uint32_t arg3);

#endif
