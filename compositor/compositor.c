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
    for (int i = 0; i < MAX_WINDOWS; i++) {
        c->windows[i].id = -1;
        c->windows[i].visible = 0;
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
            c->windows[i].title_color = 0xFF00D4C8;  // teal title bar
            c->windows[i].visible = 1;
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

    // Background
    fb_clear(0xFF0A0A12);

    for (int i = 0; i < MAX_WINDOWS; i++) {
        if (!c->windows[i].visible || c->windows[i].id == -1) continue;

        struct jagx_window* w = &c->windows[i];

        // Drop shadow
        fb_fill_rect(w->x + 6, w->y + 6, w->width, w->height, 0xFF000000);

        // Window body
        fb_fill_rect(w->x, w->y, w->width, w->height, w->color);

        // Title bar (real chrome)
        fb_fill_rect(w->x, w->y, w->width, 32, w->title_color);

        // Close button (simple red square)
        fb_fill_rect(w->x + w->width - 28, w->y + 6, 20, 20, 0xFFE74C3C);

        // Focus indicator line
        if (i == c->focused_id) {
            fb_fill_rect(w->x, w->y + 32, w->width, 2, 0xFF7B5EA7);
        }
    }
}

void compositor_draw_cursor(int x, int y) {
    if (!fb_is_ready()) return;
    // Simple 12x12 cursor
    fb_fill_rect(x, y, 12, 12, 0xFFFFFFFF);
    fb_fill_rect(x + 2, y + 2, 8, 8, 0xFF00D4C8);
}
