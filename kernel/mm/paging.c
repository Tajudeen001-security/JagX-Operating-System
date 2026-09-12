/* Basic 32-bit paging - identity map first 16 MB */
#include "paging.h"
#include "pmm.h"
#include "../arch/x86_64/console.h"

/* Page Directory and Tables must be page-aligned */
static uint32_t page_directory[1024] __attribute__((aligned(4096)));
static uint32_t first_page_table[1024] __attribute__((aligned(4096)));

void paging_map_page(uint32_t virt, uint32_t phys, uint32_t flags) {
    uint32_t pd_index = virt >> 22;
    uint32_t pt_index = (virt >> 12) & 0x3FF;

    if (!(page_directory[pd_index] & 1)) {
        /* Allocate a new page table if needed (simplified - only using first for now) */
        return;
    }

    uint32_t* pt = (uint32_t*)(page_directory[pd_index] & ~0xFFF);
    pt[pt_index] = (phys & ~0xFFF) | (flags & 0xFFF) | 1;  /* Present */
}

void paging_init(void) {
    /* Clear page directory */
    for (int i = 0; i < 1024; i++) {
        page_directory[i] = 0;
    }

    /* Identity map the first 4 MB using one page table */
    for (int i = 0; i < 1024; i++) {
        first_page_table[i] = (i * 4096) | 3;  /* Present + R/W */
    }

    page_directory[0] = ((uint32_t)first_page_table) | 3;

    /* Load page directory into CR3 */
    __asm__ volatile ("mov %0, %%cr3" : : "r"(page_directory));

    /* Enable paging (set PG bit in CR0) */
    uint32_t cr0;
    __asm__ volatile ("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ volatile ("mov %0, %%cr0" : : "r"(cr0));

    console_write("[PAGING] Enabled (identity mapped first 4 MB)\n");
}
