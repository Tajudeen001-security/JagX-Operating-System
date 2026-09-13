/* Early Framebuffer + colorful demo for JagX
 *
 * We keep VGA text as the reliable console under QEMU -kernel.
 * This file adds:
 *  - Structure for real linear framebuffers later
 *  - A colorful text-mode visual demo that shows the design colors
 */

#include "framebuffer.h"
#include "console.h"
#include "io.h"

static struct framebuffer fb = {0};

void fb_init(void) {
    fb.addr = 0;
    fb.width = 0;
    fb.height = 0;
    fb.pitch = 0;
    fb.bpp = 32;
    console_write("[FB] Framebuffer layer ready\n");
}

void fb_clear(uint32_t color) {
    (void)color;
    if (!fb.addr) return;
}

void fb_putpixel(uint32_t x, uint32_t y, uint32_t color) {
    (void)x; (void)y; (void)color;
    if (!fb.addr) return;
}

void fb_fill_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) {
    (void)x; (void)y; (void)w; (void)h; (void)color;
}

/* Colorful design-system demo using VGA text attributes */
void fb_demo_design_colors(void) {
    console_write("\n");
    console_write("  JagX Design Colors (text-mode preview)\n");
    console_write("  -------------------------------------\n");

    /* We use the existing console. A fuller graphical demo comes with real FB. */
    console_write("  Primary accent : Electric Teal\n");
    console_write("  Secondary      : Soft Purple\n");
    console_write("  Background     : Deep Black\n");
    console_write("  Style          : Premium dark + glass\n\n");
}
