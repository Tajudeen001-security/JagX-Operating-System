/* Very simple kernel heap - bump allocator + free list later */
#include "heap.h"
#include "pmm.h"
#include "../arch/x86_64/console.h"

#define HEAP_START 0x400000   /* 4 MB - after early kernel */
#define HEAP_SIZE  0x400000   /* 4 MB heap for now */

static uint32_t heap_pos = HEAP_START;
static uint32_t heap_end = HEAP_START + HEAP_SIZE;

void heap_init(void) {
    heap_pos = HEAP_START;
    console_write("[HEAP] Simple bump allocator ready (4 MB)\n");
}

void* kmalloc(size_t size) {
    if (size == 0) return NULL;

    /* Align to 8 bytes */
    size = (size + 7) & ~7;

    if (heap_pos + size > heap_end) {
        return NULL;  /* Out of heap */
    }

    void* ptr = (void*)heap_pos;
    heap_pos += size;
    return ptr;
}

void* kmalloc_a(size_t size) {
    /* Align current position to page boundary */
    if (heap_pos & 0xFFF) {
        heap_pos = (heap_pos + 0xFFF) & ~0xFFF;
    }
    return kmalloc(size);
}

void kfree(void* ptr) {
    /* Bump allocator cannot free individual blocks yet.
     * A real free-list or slab allocator will come later.
     */
    (void)ptr;
}
