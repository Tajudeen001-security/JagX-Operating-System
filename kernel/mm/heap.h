#ifndef JAGX_HEAP_H
#define JAGX_HEAP_H

#include <stdint.h>
#include <stddef.h>

void  heap_init(void);
void* kmalloc(size_t size);
void  kfree(void* ptr);
void* kmalloc_a(size_t size);   /* page-aligned */

#endif
