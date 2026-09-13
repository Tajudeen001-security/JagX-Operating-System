#include "multiboot.h"
#include "console.h"
#include "framebuffer.h"
#include "../../mm/pmm.h"

void multiboot2_parse(uint32_t magic, void* info) {
    if (magic != MULTIBOOT2_MAGIC) {
        console_write("[MB2] Not Multiboot2 (falling back)\n");
        pmm_init(128 * 1024);
        return;
    }

    console_write("[MB2] Multiboot2 detected\n");

    /* Multiboot2 info: first 8 bytes are total_size + reserved */
    uint32_t total_size = *(uint32_t*)info;
    uint8_t* ptr = (uint8_t*)info + 8;
    uint8_t* end = (uint8_t*)info + total_size;

    while (ptr < end) {
        struct multiboot_tag* tag = (struct multiboot_tag*)ptr;
        if (tag->type == 0) break;          /* End tag */

        if (tag->type == 8) {               /* Framebuffer tag */
            struct multiboot_tag_framebuffer* fb =
                (struct multiboot_tag_framebuffer*)tag;

            console_write("[MB2] Framebuffer: ");
            console_write_dec(fb->framebuffer_width);
            console_write("x");
            console_write_dec(fb->framebuffer_height);
            console_write("x");
            console_write_dec(fb->framebuffer_bpp);
            console_write("\n");

            fb_set((uint32_t*)(uint32_t)fb->framebuffer_addr,
                   fb->framebuffer_width,
                   fb->framebuffer_height,
                   fb->framebuffer_pitch,
                   fb->framebuffer_bpp);
        }

        /* Next tag is 8-byte aligned */
        ptr += (tag->size + 7) & ~7;
    }

    /* Conservative fallback memory size */
    if (pmm_get_total_pages() == 0) {
        pmm_init(128 * 1024);
    }
}
