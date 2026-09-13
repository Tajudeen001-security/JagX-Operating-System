#include "terminal.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

#define TBUF 512
static char hist[TBUF];
static int hlen = 0;

void terminal_init(void) {
    const char* s = "JagX Terminal v0.0.21\n$ ";
    hlen = 0;
    while (s[hlen] && hlen < TBUF-1) { hist[hlen] = s[hlen]; hlen++; }
    hist[hlen] = 0;
    console_write("[TERM] Developer terminal ready\n");
}

void terminal_puts(const char* s) {
    while (*s && hlen < TBUF-1) hist[hlen++] = *s++;
    hist[hlen] = 0;
}

void terminal_draw(int x, int y, int w, int h) {
    if (!fb_is_ready()) return;
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, (uint32_t)h, 0xFF0D1117);
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, 24, 0xFF21262D);
    for (int i = 0; i < 8; i++)
        fb_fill_rect((uint32_t)(x+8), (uint32_t)(y+32+i*14), (uint32_t)(w-16), 3, 0xFF3FB950);
}
