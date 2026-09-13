#include "process.h"
#include "../kernel/arch/x86_64/console.h"

static struct process procs[MAX_PROCESSES];
static int current_pid = -1;
static int next_pid = 1;

void process_init(void) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        procs[i].state = PROC_UNUSED;
        procs[i].pid = -1;
    }
    console_write("[PROC] Process table ready\n");
}

int process_create(const char* name, uint32_t entry, uint32_t stack_top) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (procs[i].state == PROC_UNUSED) {
            procs[i].pid = next_pid++;
            procs[i].state = PROC_RUNNABLE;
            procs[i].entry = entry;
            procs[i].stack_top = stack_top;
            int j = 0;
            while (name[j] && j < 31) { procs[i].name[j] = name[j]; j++; }
            procs[i].name[j] = 0;
            console_write("[PROC] Created: ");
            console_write(procs[i].name);
            console_write("\n");
            return procs[i].pid;
        }
    }
    return -1;
}

void process_yield(void) {
    /* Full context switch + ring3 jump comes with per-process page tables */
}
