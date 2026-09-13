#include "calc.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

void calc_init(void) { console_write("[CALC] Calculator ready\n"); }
int calc_eval_add(int a, int b) { return a + b; }
int calc_eval_mul(int a, int b) { return a * b; }

void calc_draw(int x, int y) {
    if (!fb_is_ready()) return;
    fb_fill_rect((uint32_t)x, (uint32_t)y, 200, 260, 0xFF1E1E28);
    fb_fill_rect((uint32_t)x, (uint32_t)y, 200, 40, 0xFF00D4C8);
    fb_fill_rect((uint32_t)(x+8), (uint32_t)(y+48), 184, 36, 0xFF0A0A12);
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            fb_fill_rect((uint32_t)(x+8+c*46), (uint32_t)(y+96+r*40), 42, 36, 0xFF2A2A35);
}
