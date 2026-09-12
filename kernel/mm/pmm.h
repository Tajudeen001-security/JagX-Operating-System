#ifndef JAGX_PMM_H
#define JAGX_PMM_H

#include <stdint.h>
#include <stddef.h>

#define PAGE_SIZE 4096

void pmm_init(uint32_t mem_size_kb);
void* pmm_alloc_page(void);
void  pmm_free_page(void* page);
uint32_t pmm_get_free_pages(void);
uint32_t pmm_get_total_pages(void);

#endif
