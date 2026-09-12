/* Physical Memory Manager - simple bitmap allocator */
#include "pmm.h"
#include "../arch/x86_64/console.h"

/* We use a fixed bitmap for the first 128 MB of RAM (plenty for a starter kernel).
 * Each bit represents one 4 KiB page.
 * 128 MB / 4 KB = 32768 pages → 32768 / 8 = 4096 bytes bitmap.
 */
#define MAX_PAGES 32768
#define BITMAP_SIZE (MAX_PAGES / 8)

static uint8_t bitmap[BITMAP_SIZE];
static uint32_t total_pages = 0;
static uint32_t used_pages  = 0;

static inline void set_bit(uint32_t bit) {
    bitmap[bit / 8] |= (1 << (bit % 8));
}

static inline void clear_bit(uint32_t bit) {
    bitmap[bit / 8] &= ~(1 << (bit % 8));
}

static inline int test_bit(uint32_t bit) {
    return bitmap[bit / 8] & (1 << (bit % 8));
}

void pmm_init(uint32_t mem_size_kb) {
    total_pages = (mem_size_kb * 1024) / PAGE_SIZE;
    if (total_pages > MAX_PAGES) total_pages = MAX_PAGES;

    /* Mark everything free first */
    for (uint32_t i = 0; i < BITMAP_SIZE; i++) {
        bitmap[i] = 0;
    }

    /* Reserve the first 1 MB (BIOS, kernel, etc.) - very conservative */
    uint32_t reserved = (1024 * 1024) / PAGE_SIZE;  /* 256 pages */
    for (uint32_t i = 0; i < reserved && i < total_pages; i++) {
        set_bit(i);
        used_pages++;
    }

    console_write("[PMM] Initialized. Total pages: ");
    console_write_dec(total_pages);
    console_write(", Free: ");
    console_write_dec(total_pages - used_pages);
    console_write("\n");
}

void* pmm_alloc_page(void) {
    for (uint32_t i = 0; i < total_pages; i++) {
        if (!test_bit(i)) {
            set_bit(i);
            used_pages++;
            return (void*)(i * PAGE_SIZE);
        }
    }
    return NULL;  /* Out of memory */
}

void pmm_free_page(void* page) {
    uint32_t addr = (uint32_t)page;
    if (addr % PAGE_SIZE != 0) return;  /* Must be page-aligned */

    uint32_t bit = addr / PAGE_SIZE;
    if (bit >= total_pages) return;

    if (test_bit(bit)) {
        clear_bit(bit);
        used_pages--;
    }
}

uint32_t pmm_get_free_pages(void) {
    return total_pages - used_pages;
}

uint32_t pmm_get_total_pages(void) {
    return total_pages;
}
