#include "gdt.h"
#include "console.h"

/* null, kcode, kdata, ucode, udata, tss */
struct gdt_entry gdt[6];
struct gdt_ptr   gp;
struct tss_entry tss;

extern void gdt_flush(uint32_t);
extern void tss_flush(void);

static void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low = base & 0xFFFF;
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;
    gdt[num].limit_low = limit & 0xFFFF;
    gdt[num].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt[num].access = access;
}

static void write_tss(int num, uint16_t ss0, uint32_t esp0) {
    uint32_t base = (uint32_t)&tss;
    uint32_t limit = sizeof(tss) - 1;
    gdt_set_gate(num, base, limit, 0xE9, 0x00);

    for (uint32_t i = 0; i < sizeof(tss); i++)
        ((uint8_t*)&tss)[i] = 0;

    tss.ss0 = ss0;
    tss.esp0 = esp0;
    tss.cs = 0x0B;  /* kernel code still used on syscall entry path variants */
    tss.ss = tss.ds = tss.es = tss.fs = tss.gs = 0x13;
    tss.iomap_base = sizeof(tss);
}

void tss_set_stack(uint32_t kernel_stack) {
    tss.esp0 = kernel_stack;
}

void gdt_init(void) {
    gp.limit = sizeof(gdt) - 1;
    gp.base = (uint32_t)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0);
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); /* kernel code */
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); /* kernel data */
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); /* user code */
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); /* user data */
    write_tss(5, 0x10, 0);

    gdt_flush((uint32_t)&gp);
    tss_flush();
    console_write("[GDT] Kernel+User segments + TSS loaded\n");
}

/* Jump to ring 3 at entry with user stack */
void enter_user_mode(uint32_t entry, uint32_t user_stack) {
    console_write("[USER] Entering ring 3\n");
    __asm__ volatile (
        "cli\n"
        "mov $0x23, %%ax\n"   /* user data selector RPL=3 */
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "pushl $0x23\n"       /* SS */
        "pushl %0\n"          /* ESP */
        "pushf\n"
        "popl %%eax\n"
        "orl $0x200, %%eax\n" /* IF */
        "pushl %%eax\n"
        "pushl $0x1B\n"       /* CS user code RPL=3 */
        "pushl %1\n"          /* EIP */
        "iret\n"
        :
        : "r"(user_stack), "r"(entry)
        : "eax", "memory"
    );
}
