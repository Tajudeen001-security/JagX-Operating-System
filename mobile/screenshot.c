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

/* Full BMP write for small buffers; for large FB write header + sample rows into RamFS chunks */
static int write_bmp_to_ramfs(const char* path) {
    if (!fb_is_ready()) return -1;
    uint32_t w = fb_width();
    uint32_t h = fb_height();
    if (!w || !h) { w = 320; h = 200; }
    /* Limit dump size to RamFS capacity: max ~1K width strip stored */
    uint32_t dw = w > 160 ? 160 : w;
    uint32_t dh = h > 100 ? 100 : h;
    uint32_t row_bytes = dw * 3;
    uint32_t pad = (4 - (row_bytes % 4)) % 4;
    uint32_t img_size = (row_bytes + pad) * dh;
    uint32_t file_size = 54 + img_size;
    if (file_size > 4096) {
        dh = (4096 - 54) / (row_bytes + pad);
        if (dh < 1) dh = 1;
        img_size = (row_bytes + pad) * dh;
        file_size = 54 + img_size;
    }

    uint8_t bmp[4096];
    for (uint32_t i = 0; i < file_size; i++) bmp[i] = 0;
    /* BITMAPFILEHEADER */
    bmp[0] = 'B'; bmp[1] = 'M';
    bmp[2] = file_size & 0xFF; bmp[3] = (file_size >> 8) & 0xFF;
    bmp[4] = (file_size >> 16) & 0xFF; bmp[5] = (file_size >> 24) & 0xFF;
    bmp[10] = 54;
    /* BITMAPINFOHEADER */
    bmp[14] = 40;
    bmp[18] = dw & 0xFF; bmp[19] = (dw >> 8) & 0xFF;
    bmp[22] = dh & 0xFF; bmp[23] = (dh >> 8) & 0xFF;
    bmp[26] = 1; bmp[28] = 24;

    uint32_t* fb = fb_addr();
    uint32_t pitch = fb_pitch() ? fb_pitch() / 4 : w;
    uint32_t off = 54;
    for (int y = (int)dh - 1; y >= 0; y--) {
        for (uint32_t x = 0; x < dw; x++) {
            uint32_t color = 0xFF00D4C8;
            if (fb && (uint32_t)y < h && x < w)
                color = fb[(uint32_t)y * pitch + x];
            bmp[off++] = (uint8_t)(color & 0xFF);
            bmp[off++] = (uint8_t)((color >> 8) & 0xFF);
            bmp[off++] = (uint8_t)((color >> 16) & 0xFF);
        }
        for (uint32_t p = 0; p < pad; p++) bmp[off++] = 0;
    }
    return ramfs_write(path, bmp, file_size);
}

int screenshot_take(void) {
    char name[48];
    name[0]='s';name[1]='c';name[2]='r';name[3]='e';name[4]='e';name[5]='n';
    name[6]='s';name[7]='h';name[8]='o';name[9]='t';name[10]='-';
    int n = shot_seq++, pos = 11;
    if (n >= 10) name[pos++] = (char)('0' + (n / 10) % 10);
    name[pos++] = (char)('0' + (n % 10));
    name[pos++] = '.'; name[pos++] = 'b'; name[pos++] = 'm'; name[pos++] = 'p';
    name[pos] = 0;

    char path[64];
    int pi = 0;
    const char* pref = "/screens/";
    while (pref[pi]) { path[pi] = pref[pi]; pi++; }
    for (int i = 0; name[i] && pi < 63; i++) path[pi++] = name[i];
    path[pi] = 0;

    write_bmp_to_ramfs(path);
    uint32_t w = fb_is_ready() ? fb_width() : 1024;
    uint32_t h = fb_is_ready() ? fb_height() : 768;
    gallery_add(JAGX_MEDIA_SCREENSHOT, name, w, h, w * h * 3);
    console_write("[SHOT] BMP saved ");
    console_write(path);
    console_write("\n");
    return 0;
}

void screenshot_set_triple_tap_enabled(int enabled) { triple_enabled = enabled ? 1 : 0; }
int screenshot_triple_tap_enabled(void) { return triple_enabled; }

void screenshot_on_tap(int x, int y) {
    if (!triple_enabled) return;
    fake_tick++;
    int near = (x - last_x) * (x - last_x) + (y - last_y) * (y - last_y) < 1600;
    if (near && (fake_tick - last_tap_tick) < 30) tap_count++;
    else tap_count = 1;
    last_x = x; last_y = y; last_tap_tick = fake_tick;
    if (tap_count >= 3) {
        tap_count = 0;
        console_write("[SHOT] Triple-tap\n");
        screenshot_take();
    }
}
