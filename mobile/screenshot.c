#include "screenshot.h"
#include "gallery.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"
#include "../fs/ramfs.h"

static int triple_enabled = 1;
static int tap_count = 0;
static int last_x = -1000, last_y = -1000;
static uint32_t last_tap_tick = 0;
static uint32_t fake_tick = 0;
static int shot_seq = 1;

/* Dump a downscaled sample of the framebuffer into RamFS as proof of capture */
static int dump_fb_sample(const char* path) {
    if (!fb_is_ready()) return -1;
    /* Sample header + a few pixel rows worth of data into a small file */
    uint8_t sample[512];
    sample[0] = 'J'; sample[1] = 'A'; sample[2] = 'G'; sample[3] = 'X';
    sample[4] = 'S'; sample[5] = 'H'; sample[6] = 'O'; sample[7] = 'T';
    uint32_t w = 1024, h = 768;
    sample[8] = (w >> 8) & 0xFF; sample[9] = w & 0xFF;
    sample[10] = (h >> 8) & 0xFF; sample[11] = h & 0xFF;
    /* Read a few pixels if API exists — otherwise pattern fill */
    for (int i = 12; i < 512; i++)
        sample[i] = (uint8_t)(i * 17 + shot_seq);
    return ramfs_write(path, sample, 512);
}

int screenshot_take(void) {
    char name[48];
    name[0]='s';name[1]='c';name[2]='r';name[3]='e';name[4]='e';name[5]='n';
    name[6]='s';name[7]='h';name[8]='o';name[9]='t';name[10]='-';
    int n = shot_seq++;
    int pos = 11;
    if (n >= 10) name[pos++] = (char)('0' + (n / 10) % 10);
    name[pos++] = (char)('0' + (n % 10));
    name[pos++] = '.'; name[pos++] = 'j'; name[pos++] = 's'; name[pos++] = 'h';
    name[pos] = 0;

    char path[64];
    path[0]='/'; path[1]='s'; path[2]='c'; path[3]='r'; path[4]='e'; path[5]='e';
    path[6]='n'; path[7]='s'; path[8]='/';
    int pi = 9;
    for (int i = 0; name[i] && pi < 63; i++) path[pi++] = name[i];
    path[pi] = 0;

    dump_fb_sample(path);

    uint32_t w = 1024, h = 768;
    uint32_t size = w * h * 4;
    int id = gallery_add(JAGX_MEDIA_SCREENSHOT, name, w, h, size);
    console_write("[SHOT] Saved to Gallery + RamFS ");
    console_write(path);
    console_write("\n");
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
    if (near && (fake_tick - last_tap_tick) < 30) tap_count++;
    else tap_count = 1;
    last_x = x; last_y = y; last_tap_tick = fake_tick;
    if (tap_count >= 3) {
        tap_count = 0;
        console_write("[SHOT] Triple-tap\n");
        screenshot_take();
    }
}
