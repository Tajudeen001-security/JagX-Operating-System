#include "paint.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

static uint32_t canvas[PAINT_H][PAINT_W];
static uint32_t brush = 0xFF000000;
static int last_x = -1, last_y = -1;

void paint_init(void) {
    paint_clear(0xFFFFFFFF);
    brush = 0xFF000000;
    console_write("[PAINT] Canvas ready\n");
}

void paint_clear(uint32_t color) {
    for (int y = 0; y < PAINT_H; y++)
        for (int x = 0; x < PAINT_W; x++)
            canvas[y][x] = color;
}

void paint_set_color(uint32_t color) { brush = color; }

void paint_plot(int x, int y) {
    if (x < 0 || y < 0 || x >= PAINT_W || y >= PAINT_H) return;
    canvas[y][x] = brush;
    if (x + 1 < PAINT_W) canvas[y][x + 1] = brush;
    if (y + 1 < PAINT_H) canvas[y + 1][x] = brush;
}

void paint_line(int x0, int y0, int x1, int y1) {
    int dx = x1 - x0; if (dx < 0) dx = -dx;
    int dy = y1 - y0; if (dy < 0) dy = -dy;
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;
    for (;;) {
        paint_plot(x0, y0);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
    }
}

void paint_draw_ui(int ox, int oy) {
    if (!fb_is_ready()) return;
    fb_fill_rect((uint32_t)ox, (uint32_t)oy, PAINT_W + 40, PAINT_H + 50, 0xFF2A2A35);
    fb_fill_rect((uint32_t)ox, (uint32_t)oy, PAINT_W + 40, 24, 0xFF00D4C8);
    for (int y = 0; y < PAINT_H; y++)
        for (int x = 0; x < PAINT_W; x++)
            fb_putpixel((uint32_t)(ox + 8 + x), (uint32_t)(oy + 32 + y), canvas[y][x]);
    /* palette */
    fb_fill_rect((uint32_t)(ox + 8), (uint32_t)(oy + 32 + PAINT_H + 4), 20, 12, 0xFF000000);
    fb_fill_rect((uint32_t)(ox + 32), (uint32_t)(oy + 32 + PAINT_H + 4), 20, 12, 0xFFE74C3C);
    fb_fill_rect((uint32_t)(ox + 56), (uint32_t)(oy + 32 + PAINT_H + 4), 20, 12, 0xFF3498DB);
    fb_fill_rect((uint32_t)(ox + 80), (uint32_t)(oy + 32 + PAINT_H + 4), 20, 12, 0xFF2ECC71);
}

void paint_on_click(int sx, int sy, int ox, int oy) {
    int px = sx - (ox + 8);
    int py = sy - (oy + 32);
    int py_palette = oy + 32 + PAINT_H + 4;
    if (sy >= py_palette && sy < py_palette + 12) {
        int cx = sx - (ox + 8);
        if (cx < 20) brush = 0xFF000000;
        else if (cx < 44) brush = 0xFFE74C3C;
        else if (cx < 68) brush = 0xFF3498DB;
        else if (cx < 92) brush = 0xFF2ECC71;
        return;
    }
    if (px < 0 || py < 0 || px >= PAINT_W || py >= PAINT_H) return;
    if (last_x >= 0) paint_line(last_x, last_y, px, py);
    else paint_plot(px, py);
    last_x = px; last_y = py;
}
