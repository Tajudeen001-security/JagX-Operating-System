#ifndef JAGX_MULTIBOOT_H
#define JAGX_MULTIBOOT_H

#include <stdint.h>

#define MULTIBOOT2_MAGIC 0x36D76289

/* We keep a simplified parser focused on framebuffer + basic memory */
struct multiboot_tag {
    uint32_t type;
    uint32_t size;
};

struct multiboot_tag_framebuffer {
    uint32_t type;
    uint32_t size;
    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t  framebuffer_bpp;
    uint8_t  framebuffer_type;
    uint16_t reserved;
};

void multiboot2_parse(uint32_t magic, void* info);

#endif
