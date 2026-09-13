#include "framebuffer.h"
#include "console.h"

static struct framebuffer fb;

void fb_init(void) {
    fb.addr = 0;
    fb.width = 0;
    fb.height = 0;
    fb.pitch = 0;
    fb.bpp = 0;
    fb.ready = 0;
}

void fb_set(uint32_t* addr, uint32_t w, uint32_t h, uint32_t pitch, uint32_t bpp) {
    fb.addr = addr;
    fb.width = w;
    fb.height = h;
    fb.pitch = pitch;
    fb.bpp = bpp;
    fb.ready = (addr && w && h) ? 1 : 0;
    if (fb.ready) console_write("[FB] Framebuffer active\n");
}

int fb_is_ready(void) { return fb.ready; }
uint32_t fb_width(void) { return fb.width; }
uint32_t fb_height(void) { return fb.height; }
uint32_t* fb_addr(void) { return fb.addr; }
uint32_t fb_pitch(void) { return fb.pitch; }
uint32_t fb_bpp(void) { return fb.bpp; }

void fb_putpixel(uint32_t x, uint32_t y, uint32_t color) {
    if (!fb.ready || x >= fb.width || y >= fb.height) return;
    uint8_t* row = (uint8_t*)fb.addr + y * fb.pitch;
    ((uint32_t*)row)[x] = color;
}

void fb_fill_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) {
    for (uint32_t j = 0; j < h; j++)
        for (uint32_t i = 0; i < w; i++)
            fb_putpixel(x + i, y + j, color);
}

void fb_clear(uint32_t color) {
    if (!fb.ready) return;
    fb_fill_rect(0, 0, fb.width, fb.height, color);
}

void fb_draw_demo(void) {
    if (!fb.ready) return;
    fb_clear(0xFF0A0A12);
    fb_fill_rect(40, 40, 200, 120, 0xFF00D4C8);
}
