#include "compositor.h"

/* Minimal string helper */
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

int compositor_create_window(struct compositor* c, int x, int y, int w, int h, const char* title) {
    if (c->window_count >= MAX_WINDOWS) return -1;

    for (int i = 0; i < MAX_WINDOWS; i++) {
        if (c->windows[i].id == -1) {
            c->windows[i].id = i;
            c->windows[i].x = x;
            c->windows[i].y = y;
            c->windows[i].width = w;
            c->windows[i].height = h;
            c->windows[i].color = 0xFF00D4C8; /* teal-ish */
            c->windows[i].visible = 1;
            jagx_strncpy(c->windows[i].title, title, 64);
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
