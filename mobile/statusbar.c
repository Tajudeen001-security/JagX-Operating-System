#include "statusbar.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/timer.h"

static int battery = 87;
static int hours = 14;
static int mins = 40;
static uint32_t last_ticks = 0;

void statusbar_init(void) {
    battery = 87;
    hours = 14;
    mins = 40;
    last_ticks = timer_get_ticks();
}

void statusbar_set_battery(int percent) {
    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;
    battery = percent;
}

void statusbar_set_time(int h, int m) {
    hours = h % 24;
    mins = m % 60;
}

int statusbar_battery(void) { return battery; }

void statusbar_tick(void) {
    uint32_t t = timer_get_ticks();
    if (t - last_ticks > 6000) { /* ~60s at 100Hz */
        last_ticks = t;
        mins++;
        if (mins >= 60) { mins = 0; hours = (hours + 1) % 24; }
        if (battery > 5 && (mins % 15) == 0) battery--;
    }
}

void statusbar_draw(void) {
    if (!fb_is_ready()) return;
    uint32_t W = fb_width();
    if (W < 100) W = 800;
    fb_fill_rect(0, 0, W, 28, 0xF00A0A12);
    /* clock block */
    fb_fill_rect(12, 6, 52, 16, 0xFF2A2A35);
    /* battery body */
    int bx = (int)W - 60;
    fb_fill_rect((uint32_t)bx, 8, 40, 14, 0xFF2A2A35);
    int fill = battery * 36 / 100;
    uint32_t col = battery < 20 ? 0xFFE74C3C : 0xFF00D4C8;
    fb_fill_rect((uint32_t)(bx + 2), 10, (uint32_t)fill, 10, col);
    fb_fill_rect((uint32_t)(bx + 40), 12, 4, 6, 0xFF2A2A35); /* nub */
}
