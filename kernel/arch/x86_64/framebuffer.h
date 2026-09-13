#ifndef JAGX_FRAMEBUFFER_H
#define JAGX_FRAMEBUFFER_H

#include <stdint.h>

/* Very early linear framebuffer support.
 * Currently we still primarily use VGA text mode.
 * This is preparation for graphical boot and UI.
 */

struct framebuffer {
    uint32_t* addr;
    uint32_t  width;
    uint32_t  height;
    uint32_t  pitch;
    uint32_t  bpp;
};

void fb_init(void);
void fb_clear(uint32_t color);
void fb_putpixel(uint32_t x, uint32_t y, uint32_t color);
void fb_fill_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);

#endif
