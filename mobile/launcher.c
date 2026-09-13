/* JagX Mobile home launcher — first-class mobile shell. */
#include "launcher.h"
#include "../pkg/jagxpkg.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

void launcher_init(void) {
    console_write("[LAUNCHER] JagX home ready\n");
}

void launcher_draw(int x, int y, int w, int h) {
    if (!fb_is_ready()) return;
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, (uint32_t)h, 0xFF07080A);
    int c = pkg_list_count();
    int i = 0;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (i >= c) break;
            int ix = x + 16 + col * ((w - 24) / 4);
            int iy = y + 48 + row * 72;
            fb_fill_rect((uint32_t)ix, (uint32_t)iy, 48, 48, 0xFF1A1D26);
            i++;
        }
    }
    /* dock */
    fb_fill_rect((uint32_t)(x + 20), (uint32_t)(y + h - 64), (uint32_t)(w - 40), 48, 0xFF12141A);
}
