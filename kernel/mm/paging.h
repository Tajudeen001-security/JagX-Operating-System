#ifndef JAGX_PAGING_H
#define JAGX_PAGING_H

#include <stdint.h>

void paging_init(void);
void paging_map_page(uint32_t virt, uint32_t phys, uint32_t flags);

#endif
