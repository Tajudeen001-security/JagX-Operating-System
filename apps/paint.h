#ifndef JAGX_PAINT_H
#define JAGX_PAINT_H

#include <stdint.h>

#define PAINT_W 160
#define PAINT_H 100

void paint_init(void);
void paint_clear(uint32_t color);
void paint_set_color(uint32_t color);
void paint_plot(int x, int y);
void paint_line(int x0, int y0, int x1, int y1);
void paint_draw_ui(int ox, int oy);
void paint_on_click(int sx, int sy, int ox, int oy);

#endif
