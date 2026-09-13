#ifndef JAGX_PROCESS_H
#define JAGX_PROCESS_H

#include <stdint.h>

#define MAX_PROCESSES 8

enum proc_state { PROC_UNUSED = 0, PROC_RUNNABLE, PROC_RUNNING, PROC_ZOMBIE };

struct process {
    int pid;
    enum proc_state state;
    uint32_t entry;
    uint32_t stack_top;
    char name[32];
};

void process_init(void);
int  process_create(const char* name, uint32_t entry, uint32_t stack_top);
void process_yield(void);

#endif
