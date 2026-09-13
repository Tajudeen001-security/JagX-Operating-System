#include "ide.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

void ide_init(void) { console_write("[IDE] Developer editor surface ready\n"); }

void ide_draw(int x, int y, int w, int h) {
    if (!fb_is_ready()) return;
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, (uint32_t)h, 0xFF0D1117);
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, 28, 0xFF161B22);
    fb_fill_rect((uint32_t)x, (uint32_t)(y+28), 48, (uint32_t)(h-28), 0xFF010409);
    for (int i = 0; i < 10; i++)
        fb_fill_rect((uint32_t)(x+56), (uint32_t)(y+40+i*16), (uint32_t)(w-64), 2, 0xFF21262D);
}
