#include "lockscreen.h"
#include "../auth/auth.h"
#include "statusbar.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

static int locked = 1;
static int unlock_progress = 0;

void lockscreen_init(void) {
    locked = 1;
    unlock_progress = 0;
    console_write("[LOCK] Auth lock screen (PIN / biometric)\n");
}

void lockscreen_show(void) {
    locked = 1;
    unlock_progress = 0;
    auth_logout();
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
    /* swipe only reveals auth UI; still need PIN/bio */
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

void lockscreen_draw(void) {
    if (!fb_is_ready() || !lockscreen_is_locked()) return;
    uint32_t W = fb_width() ? fb_width() : 800;
    uint32_t H = fb_height() ? fb_height() : 600;
    fb_fill_rect(0, 0, W, H, 0xFF0A0A18);
    /* clock block */
    fb_fill_rect(W/2 - 90, H/5, 180, 90, 0xFF1E2A3A);
    fb_fill_rect(W/2 - 70, H/5 + 20, 140, 50, 0xFF00D4C8);
    /* PIN pad area */
    fb_fill_rect(W/2 - 100, H/2, 200, 36, 0xFF2A2A35);
    /* dots for entry length */
    const char* m = auth_entry_masked();
    int n = 0; while (m[n]) n++;
    for (int i = 0; i < n && i < 8; i++)
        fb_fill_rect(W/2 - 70 + (uint32_t)(i * 20), H/2 + 10, 12, 12, 0xFF00D4C8);
    /* biometric buttons */
    fb_fill_rect(W/2 - 80, H/2 + 60, 70, 50, 0xFF2A3A4A); /* fingerprint */
    fb_fill_rect(W/2 + 10, H/2 + 60, 70, 50, 0xFF2A3A4A); /* face */
    fb_fill_rect(W/2 - 40, H/2 + 70, 30, 30, 0xFF7B5EA7);
    fb_fill_rect(W/2 + 30, H/2 + 70, 30, 30, 0xFF00D4C8);
    /* method: PIN keypad grid */
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            fb_fill_rect(W/2 - 70 + (uint32_t)(c * 50), H/2 + 130 + (uint32_t)(r * 45), 42, 38, 0xFF1E2A3A);
    statusbar_draw();
}
