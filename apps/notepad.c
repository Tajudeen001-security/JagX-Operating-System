#include "notepad.h"
#include "../fs/ramfs.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

static char buf[NOTEPAD_MAX];
static uint32_t len = 0;

void notepad_init(void) {
    len = 0; buf[0] = 0;
    const char* intro = "JagX Notepad\nReady for notes.\n";
    while (intro[len] && len < NOTEPAD_MAX - 1) { buf[len] = intro[len]; len++; }
    buf[len] = 0;
    console_write("[NOTEPAD] Ready\n");
}

void notepad_clear(void) { len = 0; buf[0] = 0; }

void notepad_insert_char(char c) {
    if (len + 1 >= NOTEPAD_MAX) return;
    buf[len++] = c; buf[len] = 0;
}

void notepad_backspace(void) {
    if (len == 0) return;
    buf[--len] = 0;
}

const char* notepad_text(void) { return buf; }
uint32_t notepad_length(void) { return len; }

int notepad_save(const char* path) {
    return ramfs_write(path, (const uint8_t*)buf, len);
}

int notepad_load(const char* path) {
    uint32_t n = 0;
    if (ramfs_read(path, (uint8_t*)buf, NOTEPAD_MAX - 1, &n) != 0) return -1;
    len = n; buf[len] = 0;
    return 0;
}

void notepad_draw(int x, int y, int w, int h) {
    if (!fb_is_ready()) return;
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, (uint32_t)h, 0xFFF5F5F0);
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, 28, 0xFF00D4C8);
    /* text lines as bars proportional to content */
    int rows = (int)(len / 40) + 1;
    if (rows > 12) rows = 12;
    for (int i = 0; i < rows; i++)
        fb_fill_rect((uint32_t)(x + 8), (uint32_t)(y + 36 + i * 16), (uint32_t)(w - 16), 4, 0xFF333333);
}
