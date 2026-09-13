#ifndef JAGX_COMPOSITOR_H
#define JAGX_COMPOSITOR_H

#include <stdint.h>

#define MAX_WINDOWS 16

struct jagx_window {
    int      id;
    int      x, y;
    int      width, height;
    uint32_t color;
    uint32_t title_color;
    int      visible;
    int      dragging;
    char     title[48];
};

struct compositor {
    struct jagx_window windows[MAX_WINDOWS];
    int window_count;
    int focused_id;
    int last_cursor_x;
    int last_cursor_y;
    int drag_offset_x;
    int drag_offset_y;
    int show_control_center;
};

void compositor_init(struct compositor* c);
int  compositor_create_window(struct compositor* c, int x, int y,
                              int w, int h, const char* title);
void compositor_destroy_window(struct compositor* c, int id);
void compositor_render(struct compositor* c);
void compositor_draw_cursor(int x, int y);
void compositor_handle_mouse(struct compositor* c, int x, int y, uint8_t buttons);
void compositor_toggle_control_center(struct compositor* c);
void compositor_draw_control_center(struct compositor* c);

#endif
