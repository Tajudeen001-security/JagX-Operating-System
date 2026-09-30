#include "lockscreen.h"
#include "../auth/auth.h"
#include "statusbar.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

static int locked = 1;
static int unlock_progress = 0;
static uint32_t pad_ox, pad_oy; /* keypad origin for hit tests */

void lockscreen_init(void) {
    locked = 1;
    unlock_progress = 0;
    console_write("[LOCK] PIN keypad clickable + biometrics\n");
}

void lockscreen_show(void) {
    locked = 1;
    unlock_progress = 0;
    auth_logout();
    auth_clear_entry();
}

void lockscreen_hide(void) {
    locked = 0;
    unlock_progress = 0;
}

int lockscreen_is_locked(void) {
    if (auth_is_authenticated()) locked = 0;
    return locked && !auth_is_authenticated();
}

void lockscreen_on_drag(int y) {
    if (!lockscreen_is_locked()) return;
    if (y < 0) unlock_progress += -y;
}

void lockscreen_on_key(char c) {
    if (!lockscreen_is_locked()) return;
    auth_on_key(c);
    if (auth_is_authenticated()) locked = 0;
}

void lockscreen_try_fingerprint(void) {
    if (auth_try_fingerprint() == AUTH_OK) locked = 0;
}

void lockscreen_try_face(void) {
    if (auth_try_face() == AUTH_OK) locked = 0;
}

void lockscreen_on_click(int x, int y) {
    if (!lockscreen_is_locked() || !fb_is_ready()) return;
    uint32_t W = fb_width() ? fb_width() : 800;
    uint32_t H = fb_height() ? fb_height() : 600;
    uint32_t cx = W / 2;

    /* Fingerprint button */
    if (x >= (int)(cx - 80) && x < (int)(cx - 10) &&
        y >= (int)(H/2 + 60) && y < (int)(H/2 + 110)) {
        lockscreen_try_fingerprint();
        return;
    }
    /* Face button */
    if (x >= (int)(cx + 10) && x < (int)(cx + 80) &&
        y >= (int)(H/2 + 60) && y < (int)(H/2 + 110)) {
        lockscreen_try_face();
        return;
    }

    /* Keypad 1-9 grid: 3x3 */
    int key_w = 42, key_h = 38, gap = 50;
    int base_x = (int)(cx - 70);
    int base_y = (int)(H/2 + 130);
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            int kx = base_x + c * gap;
            int ky = base_y + r * 45;
            if (x >= kx && x < kx + key_w && y >= ky && y < ky + key_h) {
                char dig = (char)('1' + r * 3 + c);
                auth_on_key(dig);
                if (auth_is_authenticated()) locked = 0;
                return;
            }
        }
    }
    /* Row: backspace | 0 | OK(enter) */
    int row_y = base_y + 3 * 45;
    if (x >= base_x && x < base_x + key_w && y >= row_y && y < row_y + key_h) {
        auth_on_key('\b');
        return;
    }
    if (x >= base_x + gap && x < base_x + gap + key_w && y >= row_y && y < row_y + key_h) {
        auth_on_key('0');
        if (auth_is_authenticated()) locked = 0;
        return;
    }
    if (x >= base_x + 2 * gap && x < base_x + 2 * gap + key_w && y >= row_y && y < row_y + key_h) {
        auth_on_key('\n');
        if (auth_is_authenticated()) locked = 0;
        return;
    }
}

void lockscreen_draw(void) {
    if (!fb_is_ready() || !lockscreen_is_locked()) return;
    uint32_t W = fb_width() ? fb_width() : 800;
    uint32_t H = fb_height() ? fb_height() : 600;
    uint32_t cx = W / 2;
    fb_fill_rect(0, 0, W, H, 0xFF0A0A18);
    fb_fill_rect(cx - 90, H/5, 180, 90, 0xFF1E2A3A);
    fb_fill_rect(cx - 70, H/5 + 20, 140, 50, 0xFF00D4C8);
    /* PIN field */
    fb_fill_rect(cx - 100, H/2, 200, 36, 0xFF2A2A35);
    const char* m = auth_entry_masked();
    int n = 0; while (m[n]) n++;
    for (int i = 0; i < n && i < 8; i++)
        fb_fill_rect(cx - 70 + (uint32_t)(i * 20), H/2 + 10, 12, 12, 0xFF00D4C8);
    /* Biometrics */
    fb_fill_rect(cx - 80, H/2 + 60, 70, 50, 0xFF2A3A4A);
    fb_fill_rect(cx + 10, H/2 + 60, 70, 50, 0xFF2A3A4A);
    fb_fill_rect(cx - 40, H/2 + 70, 30, 30, 0xFF7B5EA7);
    fb_fill_rect(cx + 30, H/2 + 70, 30, 30, 0xFF00D4C8);
    /* Keys 1-9 */
    pad_ox = cx - 70;
    pad_oy = H/2 + 130;
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            fb_fill_rect(pad_ox + (uint32_t)(c * 50), pad_oy + (uint32_t)(r * 45), 42, 38, 0xFF1E2A3A);
    /* Backspace, 0, OK */
    fb_fill_rect(pad_ox, pad_oy + 135, 42, 38, 0xFF3A2A2A);       /* backspace */
    fb_fill_rect(pad_ox + 50, pad_oy + 135, 42, 38, 0xFF1E2A3A);  /* 0 */
    fb_fill_rect(pad_ox + 100, pad_oy + 135, 42, 38, 0xFF1A4A3A); /* OK */
    statusbar_draw();
}
