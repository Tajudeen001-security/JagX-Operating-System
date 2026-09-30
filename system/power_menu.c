#include "power_menu.h"
#include "power.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

static int open = 0;

void power_menu_init(void) {
    open = 0;
    console_write("[POWER] On-screen power menu ready\n");
}

void power_menu_show(void) { open = 1; }
void power_menu_hide(void) { open = 0; }
int  power_menu_is_open(void) { return open; }

void power_menu_draw(void) {
    if (!open || !fb_is_ready()) return;
    uint32_t W = fb_width() ? fb_width() : 800;
    uint32_t H = fb_height() ? fb_height() : 600;
    /* Dim overlay */
    fb_fill_rect(0, 0, W, H, 0x88000000);
    /* Card */
    uint32_t cx = W / 2 - 120, cy = H / 2 - 100;
    fb_fill_rect(cx + 4, cy + 4, 240, 200, 0xFF000000);
    fb_fill_rect(cx, cy, 240, 200, 0xFF1A1A24);
    fb_fill_rect(cx, cy, 240, 36, 0xFF00D4C8);
    /* Power off */
    fb_fill_rect(cx + 20, cy + 50, 200, 40, 0xFFE74C3C);
    /* Restart */
    fb_fill_rect(cx + 20, cy + 100, 200, 40, 0xFF7B5EA7);
    /* Sleep / cancel */
    fb_fill_rect(cx + 20, cy + 150, 95, 36, 0xFF2A2A35);
    fb_fill_rect(cx + 125, cy + 150, 95, 36, 0xFF2A2A35);
}

void power_menu_on_click(int x, int y) {
    if (!open || !fb_is_ready()) return;
    uint32_t W = fb_width() ? fb_width() : 800;
    uint32_t H = fb_height() ? fb_height() : 600;
    int cx = (int)(W / 2 - 120), cy = (int)(H / 2 - 100);

    if (x >= cx + 20 && x < cx + 220 && y >= cy + 50 && y < cy + 90) {
        open = 0;
        power_request_shutdown();
        return;
    }
    if (x >= cx + 20 && x < cx + 220 && y >= cy + 100 && y < cy + 140) {
        open = 0;
        power_request_restart();
        return;
    }
    if (x >= cx + 20 && x < cx + 115 && y >= cy + 150 && y < cy + 186) {
        open = 0;
        power_request_sleep();
        return;
    }
    if (x >= cx + 125 && x < cx + 220 && y >= cy + 150 && y < cy + 186) {
        open = 0; /* cancel */
        return;
    }
    /* click outside closes */
    if (x < cx || x >= cx + 240 || y < cy || y >= cy + 200)
        open = 0;
}
