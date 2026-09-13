#include "multiboot.h"
#include "console.h"
#include "../../mm/pmm.h"

void multiboot_parse(uint32_t magic, struct multiboot_info* info) {
    if (magic != MULTIBOOT_MAGIC) {
        console_write("[MB] Warning: Invalid Multiboot magic\n");
        return;
    }

    console_write("[MB] Multiboot info valid\n");

    if (info->flags & 1) {
        console_write("[MB] Lower memory: ");
        console_write_dec(info->mem_lower);
        console_write(" KB, Upper: ");
        console_write_dec(info->mem_upper);
        console_write(" KB\n");

        /* Re-init PMM with real upper memory if available */
        uint32_t total_kb = info->mem_lower + info->mem_upper;
        if (total_kb > 1024) {
            pmm_init(total_kb);
        }
    }

    if (info->flags & (1 << 6)) {
        console_write("[MB] Memory map present\n");
        /* Future: walk mmap_addr / mmap_length for exact usable regions */
    }
}
