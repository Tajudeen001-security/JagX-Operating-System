#include "calendar.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

void calendar_init(void) { console_write("[CAL] Calendar ready\n"); }

void calendar_draw(int x, int y) {
    if (!fb_is_ready()) return;
    fb_fill_rect((uint32_t)x, (uint32_t)y, 280, 220, 0xFF1A1A24);
    fb_fill_rect((uint32_t)x, (uint32_t)y, 280, 32, 0xFF7B5EA7);
    for (int r = 0; r < 5; r++)
        for (int c = 0; c < 7; c++)
            fb_fill_rect((uint32_t)(x+8+c*38), (uint32_t)(y+44+r*32), 34, 28, 0xFF2A2A35);
}
