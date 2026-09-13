#include "forms.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

void forms_init(void) { console_write("[FORMS] Gov form templates ready\n"); }

void forms_draw(int x, int y) {
    if (!fb_is_ready()) return;
    fb_fill_rect((uint32_t)x, (uint32_t)y, 320, 240, 0xFFF5F5F0);
    fb_fill_rect((uint32_t)x, (uint32_t)y, 320, 32, 0xFF008751); /* NG green accent */
    for (int i = 0; i < 5; i++)
        fb_fill_rect((uint32_t)(x+16), (uint32_t)(y+48+i*36), 288, 28, 0xFFE8E8E0);
}
