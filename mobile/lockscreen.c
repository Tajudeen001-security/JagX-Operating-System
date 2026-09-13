#include "lockscreen.h"
#include "statusbar.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

static int locked = 1;
static int unlock_progress = 0;

void lockscreen_init(void) {
    locked = 1;
    unlock_progress = 0;
    console_write("[LOCK] Lock screen active\n");
}

void lockscreen_show(void) { locked = 1; unlock_progress = 0; }
void lockscreen_hide(void) { locked = 0; unlock_progress = 0; }
int  lockscreen_is_locked(void) { return locked; }

void lockscreen_on_drag(int y) {
    if (!locked) return;
    if (y < 0) unlock_progress += -y;
    if (unlock_progress > 120) {
        locked = 0;
        console_write("[LOCK] Unlocked\n");
    }
}

void lockscreen_draw(void) {
    if (!fb_is_ready() || !locked) return;
    uint32_t W = fb_width() ? fb_width() : 800;
    uint32_t H = fb_height() ? fb_height() : 600;
    fb_fill_rect(0, 0, W, H, 0xFF0A0A18);
    /* clock area */
    fb_fill_rect(W / 2 - 80, H / 3 - 40, 160, 80, 0xFF1E2A3A);
    fb_fill_rect(W / 2 - 60, H / 3 - 20, 120, 40, 0xFF00D4C8);
    /* unlock hint bar */
    fb_fill_rect(W / 2 - 50, H - 80, 100, 6, 0xFF7B5EA7);
    statusbar_draw();
}
