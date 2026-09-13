#include "screenshot.h"
#include "gallery.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

static int triple_enabled = 1;
static int tap_count = 0;
static int last_x = -1000, last_y = -1000;
static uint32_t last_tap_tick = 0;
static uint32_t fake_tick = 0;
static int shot_seq = 1;

int screenshot_take(void) {
    char name[48];
    /* build name screenshot-N */
    name[0] = 's'; name[1] = 'c'; name[2] = 'r'; name[3] = 'e';
    name[4] = 'e'; name[5] = 'n'; name[6] = 's'; name[7] = 'h';
    name[8] = 'o'; name[9] = 't'; name[10] = '-';
    int n = shot_seq++;
    int pos = 11;
    if (n >= 10) { name[pos++] = (char)('0' + (n / 10) % 10); }
    name[pos++] = (char)('0' + (n % 10));
    name[pos++] = '.'; name[pos++] = 'b'; name[pos++] = 'm'; name[pos++] = 'p';
    name[pos] = 0;

    uint32_t w = 1024, h = 768;
    if (fb_is_ready()) {
        /* dimensions would come from fb_info — use defaults if unknown */
        w = 1024; h = 768;
    }
    uint32_t size = w * h * 4;
    int id = gallery_add(JAGX_MEDIA_SCREENSHOT, name, w, h, size);
    console_write("[SHOT] Screenshot saved to Gallery\n");
    return id;
}

void screenshot_set_triple_tap_enabled(int enabled) {
    triple_enabled = enabled ? 1 : 0;
}

int screenshot_triple_tap_enabled(void) {
    return triple_enabled;
}

void screenshot_on_tap(int x, int y) {
    if (!triple_enabled) return;
    fake_tick++;
    int near = (x - last_x) * (x - last_x) + (y - last_y) * (y - last_y) < 40 * 40;
    if (near && (fake_tick - last_tap_tick) < 30)
        tap_count++;
    else
        tap_count = 1;
    last_x = x;
    last_y = y;
    last_tap_tick = fake_tick;
    if (tap_count >= 3) {
        tap_count = 0;
        console_write("[SHOT] Triple-tap detected\n");
        screenshot_take();
    }
}
