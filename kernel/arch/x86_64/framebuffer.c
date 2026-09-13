/* Early Framebuffer layer for JagX
 *
 * Right now this is mostly a stub that can be expanded
 * once we parse Multiboot framebuffer tags or use VBE/GOP.
 *
 * For the current QEMU text-mode kernel we keep VGA text
 * as the primary console, but this file prepares the path
 * toward the graphical bootscreen and UI you liked.
 */

#include "framebuffer.h"
#include "console.h"

static struct framebuffer fb = {0};

void fb_init(void) {
    /* Placeholder: In a real Multiboot2 kernel we would
     * receive framebuffer info from the bootloader.
     * For now we just record that the layer exists.
     */
    fb.addr   = 0;
    fb.width  = 0;
    fb.height = 0;
    fb.pitch  = 0;
    fb.bpp    = 32;

    console_write("[FB] Framebuffer layer ready (awaiting bootloader info)\n");
}

void fb_clear(uint32_t color) {
    if (!fb.addr) return;
    for (uint32_t y = 0; y < fb.height; y++) {
        for (uint32_t x = 0; x < fb.width; x++) {
            fb_putpixel(x, y, color);
        }
    }
}

void fb_putpixel(uint32_t x, uint32_t y, uint32_t color) {
    if (!fb.addr || x >= fb.width || y >= fb.height) return;
    uint32_t* pixel = (uint32_t*)((uint8_t*)fb.addr + y * fb.pitch + x * 4);
    *pixel = color;
}

void fb_fill_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) {
    for (uint32_t dy = 0; dy < h; dy++) {
        for (uint32_t dx = 0; dx < w; dx++) {
            fb_putpixel(x + dx, y + dy, color);
        }
    }
}
