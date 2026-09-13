#include "framebuffer.h"
#include "console.h"

static struct framebuffer fb = {0};

void fb_init(void) {
    fb.ready = 0;
    console_write("[FB] Waiting for Multiboot2 framebuffer tag...\n");
}

void fb_set(uint32_t* addr, uint32_t w, uint32_t h, uint32_t pitch, uint32_t bpp) {
    fb.addr = addr;
    fb.width = w;
    fb.height = h;
    fb.pitch = pitch;
    fb.bpp = bpp;
    fb.ready = (addr != 0 && w > 0 && h > 0);
    if (fb.ready) {
        console_write("[FB] Linear framebuffer active!\n");
    }
}

int fb_is_ready(void) {
    return fb.ready;
}

void fb_putpixel(uint32_t x, uint32_t y, uint32_t color) {
    if (!fb.ready || x >= fb.width || y >= fb.height) return;
    uint8_t* base = (uint8_t*)fb.addr + y * fb.pitch + x * 4;
    *(uint32_t*)base = color;
}

void fb_fill_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) {
    if (!fb.ready) return;
    for (uint32_t dy = 0; dy < h; dy++) {
        for (uint32_t dx = 0; dx < w; dx++) {
            fb_putpixel(x + dx, y + dy, color);
        }
    }
}

void fb_clear(uint32_t color) {
    if (!fb.ready) return;
    fb_fill_rect(0, 0, fb.width, fb.height, color);
}

/* Draw a simple demo matching JagX design colors */
void fb_draw_demo(void) {
    if (!fb.ready) {
        console_write("[FB] No framebuffer - text mode only\n");
        return;
    }

    /* Deep black background */
    fb_clear(0x00000000);

    /* Teal header bar */
    fb_fill_rect(0, 0, fb.width, 48, 0xFF00D4C8);

    /* Soft purple accent rectangle */
    fb_fill_rect(80, 120, 400, 200, 0xFF7B5EA7);

    /* Another teal card */
    fb_fill_rect(520, 120, 300, 200, 0xFF00B7A8);

    console_write("[FB] Demo rectangles drawn (teal + purple)\n");
}
