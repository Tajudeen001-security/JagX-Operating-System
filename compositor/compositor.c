#include "compositor.h"
#include "../kernel/arch/x86_64/framebuffer.h"

static void jagx_strncpy(char* dst, const char* src, int n) {
    int i = 0;
    while (i < n - 1 && src[i]) {
        dst[i] = src[i];
        i++;
    }
    dst[i] = 0;
}

void compositor_init(struct compositor* c) {
    c->window_count = 0;
    c->focused_id = -1;
    c->last_cursor_x = 200;
    c->last_cursor_y = 150;
    c->drag_offset_x = 0;
    c->drag_offset_y = 0;
    for (int i = 0; i < MAX_WINDOWS; i++) {
        c->windows[i].id = -1;
        c->windows[i].visible = 0;
        c->windows[i].dragging = 0;
    }
}

int compositor_create_window(struct compositor* c, int x, int y,
                             int w, int h, const char* title) {
    if (c->window_count >= MAX_WINDOWS) return -1;

    for (int i = 0; i < MAX_WINDOWS; i++) {
        if (c->windows[i].id == -1) {
            c->windows[i].id = i;
            c->windows[i].x = x;
            c->windows[i].y = y;
            c->windows[i].width = w;
            c->windows[i].height = h;
            c->windows[i].color = (i % 2 == 0) ? 0xFF1E2A3A : 0xFF2A1E3A;
            c->windows[i].title_color = 0xFF00D4C8;
            c->windows[i].visible = 1;
            c->windows[i].dragging = 0;
            jagx_strncpy(c->windows[i].title, title, 48);
            c->window_count++;
            c->focused_id = i;
            return i;
        }
    }
    return -1;
}

void compositor_destroy_window(struct compositor* c, int id) {
    if (id < 0 || id >= MAX_WINDOWS) return;
    if (c->windows[id].id == -1) return;
    c->windows[id].id = -1;
    c->windows[id].visible = 0;
    c->window_count--;
}

void compositor_render(struct compositor* c) {
    if (!fb_is_ready()) return;

    fb_clear(0xFF0A0A12);

    for (int i = 0; i < MAX_WINDOWS; i++) {
        if (!c->windows[i].visible || c->windows[i].id == -1) continue;
        struct jagx_window* w = &c->windows[i];

        // Shadow
        fb_fill_rect(w->x + 5, w->y + 5, w->width, w->height, 0xFF050508);
        // Body
        fb_fill_rect(w->x, w->y, w->width, w->height, w->color);
        // Title bar
        fb_fill_rect(w->x, w->y, w->width, 32, w->title_color);
        // Close button
        fb_fill_rect(w->x + w->width - 26, w->y + 6, 18, 18, 0xFFE74C3C);
        // Focus line
        if (i == c->focused_id)
            fb_fill_rect(w->x, w->y + 32, w->width, 3, 0xFF7B5EA7);
    }

    compositor_draw_cursor(c->last_cursor_x, c->last_cursor_y);
}

void compositor_draw_cursor(int x, int y) {
    if (!fb_is_ready()) return;
    fb_fill_rect(x, y, 14, 14, 0xFFFFFFFF);
    fb_fill_rect(x + 2, y + 2, 10, 10, 0xFF00D4C8);
}

/* Live mouse handling: cursor redraw + title-bar dragging */
void compositor_handle_mouse(struct compositor* c, int x, int y, uint8_t buttons) {
    if (!fb_is_ready()) return;

    int left_down = buttons & 1;

    // Start drag if clicking on focused window title bar
    if (left_down && c->focused_id >= 0) {
        struct jagx_window* w = &c->windows[c->focused_id];
        if (x >= w->x && x < w->x + w->width &&
            y >= w->y && y < w->y + 32) {
            if (!w->dragging) {
                w->dragging = 1;
                c->drag_offset_x = x - w->x;
                c->drag_offset_y = y - w->y;
            }
        }
    }

    // Perform drag
    if (c->focused_id >= 0 && c->windows[c->focused_id].dragging) {
        if (left_down) {
            c->windows[c->focused_id].x = x - c->drag_offset_x;
            c->windows[c->focused_id].y = y - c->drag_offset_y;
            if (c->windows[c->focused_id].x < 0) c->windows[c->focused_id].x = 0;
            if (c->windows[c->focused_id].y < 0) c->windows[c->focused_id].y = 0;
        } else {
            c->windows[c->focused_id].dragging = 0;
        }
    }

    c->last_cursor_x = x;
    c->last_cursor_y = y;

    // Full redraw for now (later: damage tracking)
    compositor_render(c);
}
