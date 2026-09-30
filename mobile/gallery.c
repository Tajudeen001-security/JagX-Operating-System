#include "gallery.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

static struct jagx_media_item items[JAGX_GALLERY_MAX];
static int n_items = 0;
static int selected = 0;

void gallery_init(void) {
    n_items = 0;
    selected = 0;
    gallery_add(JAGX_MEDIA_PHOTO, "photo-001.bmp", 640, 480, 120000);
    gallery_add(JAGX_MEDIA_SCREENSHOT, "screenshot-1.bmp", 800, 600, 90000);
    gallery_add(JAGX_MEDIA_SCREENSHOT, "screenshot-2.bmp", 800, 600, 88000);
    gallery_add(JAGX_MEDIA_RECORDING, "record-1.jagrec", 640, 360, 500000);
    gallery_add(JAGX_MEDIA_PHOTO, "camera-shot.bmp", 1280, 720, 200000);
    console_write("[GALLERY] Media library ready\n");
}

int gallery_add(enum jagx_media_type type, const char* name,
                uint32_t w, uint32_t h, uint32_t size) {
    if (n_items >= JAGX_GALLERY_MAX) return -1;
    struct jagx_media_item* it = &items[n_items];
    it->used = 1;
    it->type = type;
    it->width = w;
    it->height = h;
    it->size_bytes = size;
    it->timestamp = (uint32_t)n_items * 1000;
    int i = 0;
    if (name) while (name[i] && i < JAGX_GALLERY_NAME-1) { it->name[i] = name[i]; i++; }
    it->name[i] = 0;
    if (type == JAGX_MEDIA_PHOTO) it->color = 0xFF3498DB;
    else if (type == JAGX_MEDIA_SCREENSHOT) it->color = 0xFF00D4C8;
    else it->color = 0xFFE74C3C;
    n_items++;
    return n_items - 1;
}

int gallery_count(void) { return n_items; }

const struct jagx_media_item* gallery_get(int index) {
    if (index < 0 || index >= n_items) return 0;
    return &items[index];
}

int gallery_selected(void) { return selected; }
void gallery_select(int index) {
    if (index >= 0 && index < n_items) selected = index;
}

void gallery_list_console(void) {
    for (int i = 0; i < n_items; i++) {
        console_write(items[i].name);
        console_write("\n");
    }
}

void gallery_draw(int x, int y, int w, int h) {
    if (!fb_is_ready()) return;
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, (uint32_t)h, 0xFF0D1117);
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, 36, 0xFF00D4C8);
    /* grid of thumbnails 3 columns */
    int cols = 3;
    int pad = 8;
    int cell = (w - pad * (cols + 1)) / cols;
    if (cell < 20) cell = 20;
    int rows = (h - 48) / (cell + pad);
    int idx = 0;
    for (int r = 0; r < rows && idx < n_items; r++) {
        for (int c = 0; c < cols && idx < n_items; c++, idx++) {
            int tx = x + pad + c * (cell + pad);
            int ty = y + 44 + r * (cell + pad);
            fb_fill_rect((uint32_t)tx, (uint32_t)ty, (uint32_t)cell, (uint32_t)cell, items[idx].color);
            fb_fill_rect((uint32_t)(tx + 4), (uint32_t)(ty + 4), (uint32_t)(cell - 8), (uint32_t)(cell - 8), 0xFF1A1A24);
            if (idx == selected)
                fb_fill_rect((uint32_t)tx, (uint32_t)ty, (uint32_t)cell, 4, 0xFFFFFFFF);
            /* type stripe */
            fb_fill_rect((uint32_t)tx, (uint32_t)(ty + cell - 6), (uint32_t)cell, 6, items[idx].color);
        }
    }
    /* detail strip bottom */
    if (n_items > 0 && selected < n_items) {
        fb_fill_rect((uint32_t)(x + 8), (uint32_t)(y + h - 28), (uint32_t)(w - 16), 20, 0xFF21262D);
    }
}

void gallery_on_click(int x, int y, int ox, int oy, int w, int h) {
    if (x < ox || y < oy || x >= ox + w || y >= oy + h) return;
    int cols = 3;
    int pad = 8;
    int cell = (w - pad * (cols + 1)) / cols;
    if (cell < 20) return;
    int lx = x - ox - pad;
    int ly = y - oy - 44;
    if (lx < 0 || ly < 0) return;
    int c = lx / (cell + pad);
    int r = ly / (cell + pad);
    if (c < 0 || c >= cols) return;
    int idx = r * cols + c;
    if (idx >= 0 && idx < n_items) selected = idx;
}
