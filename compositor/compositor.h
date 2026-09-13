#ifndef JAGX_COMPOSITOR_H
#define JAGX_COMPOSITOR_H

#include <stdint.h>

#define MAX_WINDOWS 16

struct jagx_window {
    int id, x, y, width, height;
    uint32_t color, title_color;
    int visible, dragging;
    char title[48];
};

struct compositor {
    struct jagx_window windows[MAX_WINDOWS];
    int window_count, focused_id;
    int last_cursor_x, last_cursor_y;
    int drag_offset_x, drag_offset_y;
    int show_control_center;
    int show_notification_shade;
};

void compositor_init(struct compositor* c);
int  compositor_create_window(struct compositor* c, int x, int y, int w, int h, const char* title);
void compositor_destroy_window(struct compositor* c, int id);
void compositor_render(struct compositor* c);
void compositor_draw_cursor(int x, int y);
void compositor_handle_mouse(struct compositor* c, int x, int y, uint8_t buttons);
void compositor_toggle_control_center(struct compositor* c);
void compositor_toggle_notification_shade(struct compositor* c);
void compositor_draw_control_center(struct compositor* c);
void compositor_draw_notification_shade(struct compositor* c);

#endif
