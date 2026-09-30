#include "power.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

static enum power_state state = POWER_BOOTING;
static int anim = 0;
static int hold = 0;

void power_init(void) {
    state = POWER_BOOTING;
    anim = 0;
    hold = 0;
    console_write("[POWER] Boot sequence\n");
}

enum power_state power_get(void) { return state; }

int power_blocks_ui(void) {
    return state == POWER_BOOTING || state == POWER_SHUTTING_DOWN ||
           state == POWER_RESTARTING || state == POWER_OFF;
}

void power_boot_complete(void) {
    if (state == POWER_BOOTING) {
        state = POWER_ON;
        console_write("[POWER] Boot complete\n");
    }
}

void power_request_shutdown(void) {
    state = POWER_SHUTTING_DOWN;
    anim = 0;
    hold = 0;
    console_write("[POWER] Shutting down...\n");
}

void power_request_restart(void) {
    state = POWER_RESTARTING;
    anim = 0;
    hold = 0;
    console_write("[POWER] Restarting...\n");
}

void power_request_sleep(void) {
    state = POWER_SLEEP;
    console_write("[POWER] Sleep\n");
}

void power_tick(void) {
    if (state == POWER_BOOTING) {
        anim++;
        if (anim > 40) power_boot_complete();
    } else if (state == POWER_SHUTTING_DOWN) {
        anim++;
        if (anim > 30) {
            state = POWER_OFF;
            console_write("[POWER] System halted\n");
        }
    } else if (state == POWER_RESTARTING) {
        anim++;
        if (anim > 25) {
            state = POWER_BOOTING;
            anim = 0;
            console_write("[POWER] Reboot...\n");
        }
    }
}

void power_draw_screen(void) {
    if (!fb_is_ready()) return;
    uint32_t W = fb_width() ? fb_width() : 800;
    uint32_t H = fb_height() ? fb_height() : 600;

    if (state == POWER_BOOTING) {
        fb_clear(0xFF0A0A12);
        /* JagX logo mark */
        fb_fill_rect(W/2 - 60, H/2 - 80, 120, 120, 0xFF00D4C8);
        fb_fill_rect(W/2 - 40, H/2 - 60, 80, 80, 0xFF0A0A12);
        fb_fill_rect(W/2 - 20, H/2 - 40, 40, 40, 0xFF7B5EA7);
        /* progress bar */
        fb_fill_rect(W/2 - 100, H/2 + 80, 200, 8, 0xFF2A2A35);
        uint32_t prog = (uint32_t)(anim * 5);
        if (prog > 200) prog = 200;
        fb_fill_rect(W/2 - 100, H/2 + 80, prog, 8, 0xFF00D4C8);
    } else if (state == POWER_SHUTTING_DOWN) {
        fb_clear(0xFF050508);
        fb_fill_rect(W/2 - 80, H/2 - 40, 160, 80, 0xFF1E2A3A);
        fb_fill_rect(W/2 - 50, H/2 - 10, 100, 8, 0xFFE74C3C);
        fb_fill_rect(W/2 - 100, H/2 + 60, 200, 6, 0xFF2A2A35);
        uint32_t prog = (uint32_t)(anim * 7);
        if (prog > 200) prog = 200;
        fb_fill_rect(W/2 - 100, H/2 + 60, prog, 6, 0xFFE74C3C);
    } else if (state == POWER_RESTARTING) {
        fb_clear(0xFF0A0A18);
        fb_fill_rect(W/2 - 50, H/2 - 50, 100, 100, 0xFF7B5EA7);
        fb_fill_rect(W/2 - 30, H/2 - 30, 60, 60, 0xFF0A0A18);
        fb_fill_rect(W/2 - 100, H/2 + 70, 200, 6, 0xFF2A2A35);
        uint32_t prog = (uint32_t)(anim * 8);
        if (prog > 200) prog = 200;
        fb_fill_rect(W/2 - 100, H/2 + 70, prog, 6, 0xFF7B5EA7);
    } else if (state == POWER_OFF) {
        fb_clear(0xFF000000);
    } else if (state == POWER_SLEEP) {
        fb_clear(0xFF000008);
        fb_fill_rect(W/2 - 30, H/2 - 30, 60, 60, 0xFF1A1A24);
    }
}
