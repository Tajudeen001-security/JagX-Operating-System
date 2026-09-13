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
            /* Alternate teal / purple for demo */
            c->windows[i].color = (i % 2 == 0) ? 0xFF00D4C8 : 0xFF7B5EA7;
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

/* Draw all visible windows as colored rectangles */
void compositor_render(struct compositor* c) {
    if (!fb_is_ready()) return;

    for (int i = 0; i < MAX_WINDOWS; i++) {
        if (c->windows[i].visible && c->windows[i].id != -1) {
            struct jagx_window* w = &c->windows[i];
            /* Window body */
            fb_fill_rect(w->x, w->y, w->width, w->height, w->color);
            /* Simple darker title bar */
            fb_fill_rect(w->x, w->y, w->width, 28, 0xFF1A1A2E);
        }
    }
}
