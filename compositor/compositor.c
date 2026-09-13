#include "compositor.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../mobile/control_center.h"
#include "../mobile/screenshot.h"
#include "../mobile/screencast.h"
#include "../mobile/notifications.h"
#include "../mobile/statusbar.h"
#include "../mobile/lockscreen.h"

static void jagx_strncpy(char* dst, const char* src, int n) {
    int i = 0;
    while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; }
    dst[i] = 0;
}

void compositor_init(struct compositor* c) {
    c->window_count = 0;
    c->focused_id = -1;
    c->last_cursor_x = 200;
    c->last_cursor_y = 150;
    c->drag_offset_x = c->drag_offset_y = 0;
    c->show_control_center = 0;
    c->show_notification_shade = 0;
    for (int i = 0; i < MAX_WINDOWS; i++) {
        c->windows[i].id = -1;
        c->windows[i].visible = 0;
        c->windows[i].dragging = 0;
    }
}

int compositor_create_window(struct compositor* c, int x, int y, int w, int h, const char* title) {
    if (c->window_count >= MAX_WINDOWS) return -1;
    for (int i = 0; i < MAX_WINDOWS; i++) {
        if (c->windows[i].id == -1) {
            c->windows[i].id = i;
            c->windows[i].x = x; c->windows[i].y = y;
            c->windows[i].width = w; c->windows[i].height = h;
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
    if (id < 0 || id >= MAX_WINDOWS || c->windows[id].id == -1) return;
    c->windows[id].id = -1;
    c->windows[id].visible = 0;
    c->window_count--;
}

void compositor_draw_notification_shade(struct compositor* c) {
    if (!fb_is_ready() || !c->show_notification_shade) return;
    int x = 200, y = 28, w = 400, h = 280;
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, (uint32_t)h, 0xF0101018);
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, 28, 0xFF7B5EA7);
    int n = notifications_count();
    for (int i = 0; i < n && i < 5; i++)
        fb_fill_rect((uint32_t)(x + 12), (uint32_t)(y + 40 + i * 44), (uint32_t)(w - 24), 36, 0xFF1E2A3A);
}

void compositor_draw_control_center(struct compositor* c) {
    if (!fb_is_ready() || !c->show_control_center) return;
    int panel_x = 40, panel_y = 40, panel_w = 300, panel_h = 380;
    fb_fill_rect((uint32_t)(panel_x + 4), (uint32_t)(panel_y + 4), (uint32_t)panel_w, (uint32_t)panel_h, 0xFF000000);
    fb_fill_rect((uint32_t)panel_x, (uint32_t)panel_y, (uint32_t)panel_w, (uint32_t)panel_h, 0xEE12121A);
    fb_fill_rect((uint32_t)panel_x, (uint32_t)panel_y, (uint32_t)panel_w, 36, 0xFF00D4C8);
    struct jagx_cc_state st = control_center_get();
    int tx = panel_x + 16, ty = panel_y + 52, tw = 120, th = 56;
    uint32_t on = 0xFF00D4C8, off = 0xFF2A2A35;
    fb_fill_rect((uint32_t)tx, (uint32_t)ty, (uint32_t)tw, (uint32_t)th, st.wifi ? on : off);
    fb_fill_rect((uint32_t)(tx + tw + 16), (uint32_t)ty, (uint32_t)tw, (uint32_t)th, st.mobile_data ? on : off);
    fb_fill_rect((uint32_t)tx, (uint32_t)(ty + th + 12), (uint32_t)tw, (uint32_t)th, st.airplane ? on : off);
    fb_fill_rect((uint32_t)(tx + tw + 16), (uint32_t)(ty + th + 12), (uint32_t)tw, (uint32_t)th, st.torch ? on : off);
    fb_fill_rect((uint32_t)tx, (uint32_t)(ty + 2 * (th + 12)), (uint32_t)tw, (uint32_t)th, 0xFF7B5EA7);
    fb_fill_rect((uint32_t)(tx + tw + 16), (uint32_t)(ty + 2 * (th + 12)), (uint32_t)tw, (uint32_t)th,
                 screencast_is_recording() ? 0xFFE74C3C : 0xFF2A2A35);
}

void compositor_toggle_control_center(struct compositor* c) {
    c->show_control_center = !c->show_control_center;
}
void compositor_toggle_notification_shade(struct compositor* c) {
    c->show_notification_shade = !c->show_notification_shade;
}

void compositor_render(struct compositor* c) {
    if (!fb_is_ready()) return;
    statusbar_tick();
    if (screencast_is_recording()) screencast_tick();

    if (lockscreen_is_locked()) {
        lockscreen_draw();
        compositor_draw_cursor(c->last_cursor_x, c->last_cursor_y);
        return;
    }

    fb_clear(0xFF0A0A12);
    for (int i = 0; i < MAX_WINDOWS; i++) {
        if (!c->windows[i].visible || c->windows[i].id == -1) continue;
        struct jagx_window* w = &c->windows[i];
        fb_fill_rect((uint32_t)(w->x + 5), (uint32_t)(w->y + 5), (uint32_t)w->width, (uint32_t)w->height, 0xFF050508);
        fb_fill_rect((uint32_t)w->x, (uint32_t)w->y, (uint32_t)w->width, (uint32_t)w->height, w->color);
        fb_fill_rect((uint32_t)w->x, (uint32_t)w->y, (uint32_t)w->width, 32, w->title_color);
        fb_fill_rect((uint32_t)(w->x + w->width - 26), (uint32_t)(w->y + 6), 18, 18, 0xFFE74C3C);
        if (i == c->focused_id)
            fb_fill_rect((uint32_t)w->x, (uint32_t)(w->y + 32), (uint32_t)w->width, 3, 0xFF7B5EA7);
    }
    compositor_draw_control_center(c);
    compositor_draw_notification_shade(c);
    statusbar_draw();
    if (screencast_is_recording()) fb_fill_rect(16, 32, 18, 18, 0xFFE74C3C);
    if (notifications_count() > 0) fb_fill_rect(fb_width() > 20 ? fb_width() - 20 : 780, 8, 10, 10, 0xFFE74C3C);
    compositor_draw_cursor(c->last_cursor_x, c->last_cursor_y);
}

void compositor_draw_cursor(int x, int y) {
    if (!fb_is_ready()) return;
    fb_fill_rect((uint32_t)x, (uint32_t)y, 14, 14, 0xFFFFFFFF);
    fb_fill_rect((uint32_t)(x + 2), (uint32_t)(y + 2), 10, 10, 0xFF00D4C8);
}

void compositor_handle_mouse(struct compositor* c, int x, int y, uint8_t buttons) {
    if (!fb_is_ready()) return;
    int left_down = buttons & 1;

    if (lockscreen_is_locked()) {
        static int prev_y = -1;
        if (left_down) {
            if (prev_y >= 0) lockscreen_on_drag(y - prev_y);
            prev_y = y;
        } else prev_y = -1;
        c->last_cursor_x = x;
        c->last_cursor_y = y;
        compositor_render(c);
        return;
    }

    if (left_down) screenshot_on_tap(x, y);
    if (left_down && y < 28) c->show_notification_shade = 1;

    if (left_down && c->focused_id >= 0) {
        struct jagx_window* w = &c->windows[c->focused_id];
        if (x >= w->x && x < w->x + w->width && y >= w->y && y < w->y + 32) {
            if (!w->dragging) {
                w->dragging = 1;
                c->drag_offset_x = x - w->x;
                c->drag_offset_y = y - w->y;
            }
        }
        if (c->show_control_center) {
            int panel_x = 40, panel_y = 40, tx = panel_x + 16, ty = panel_y + 52, tw = 120, th = 56;
            if (x >= tx && x < tx + tw && y >= ty && y < ty + th) control_center_toggle_wifi();
            else if (x >= tx + tw + 16 && x < tx + 2 * tw + 16 && y >= ty && y < ty + th) control_center_toggle_data();
            else if (x >= tx && x < tx + tw && y >= ty + th + 12 && y < ty + 2 * th + 12) control_center_toggle_airplane();
            else if (x >= tx + tw + 16 && x < tx + 2 * tw + 16 && y >= ty + th + 12 && y < ty + 2 * th + 12) control_center_toggle_torch();
            else if (x >= tx && x < tx + tw && y >= ty + 2 * (th + 12) && y < ty + 3 * th + 24) control_center_screenshot();
            else if (x >= tx + tw + 16 && x < tx + 2 * tw + 16 && y >= ty + 2 * (th + 12) && y < ty + 3 * th + 24)
                control_center_toggle_record();
        }
    }
    if (c->focused_id >= 0 && c->windows[c->focused_id].dragging) {
        if (left_down) {
            c->windows[c->focused_id].x = x - c->drag_offset_x;
            c->windows[c->focused_id].y = y - c->drag_offset_y;
            if (c->windows[c->focused_id].x < 0) c->windows[c->focused_id].x = 0;
            if (c->windows[c->focused_id].y < 0) c->windows[c->focused_id].y = 0;
        } else c->windows[c->focused_id].dragging = 0;
    }
    c->last_cursor_x = x;
    c->last_cursor_y = y;
    compositor_render(c);
}
