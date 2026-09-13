/* JagX Phone — native dialer. Not Android Telephony / not APK. */
#include "phone.h"
#include "../mobile/hal/radio.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

static enum jagx_call_state state = JAGX_CALL_IDLE;
static char peer[24];
static int muted;
static uint32_t ticks;

static void copy_str(char* dst, const char* src, int max) {
    int i = 0;
    if (src) while (src[i] && i < max - 1) { dst[i] = src[i]; i++; }
    dst[i] = 0;
}

void phone_init(void) {
    state = JAGX_CALL_IDLE;
    peer[0] = 0;
    muted = 0;
    ticks = 0;
    console_write("[PHONE] Native JagX dialer ready (.jagx, not APK)\n");
}

int phone_dial(const char* number) {
    if (!number || !number[0]) return -1;
    if (state == JAGX_CALL_ACTIVE || state == JAGX_CALL_DIALING) return -1;
    copy_str(peer, number, 24);
    if (jagx_call_dial(number) != 0) {
        /* HAL stub still lets lab UI proceed so the shell can be tested */
        console_write("[PHONE] Modem HAL stub — lab call session\n");
    }
    state = JAGX_CALL_DIALING;
    ticks = 0;
    console_write("[PHONE] Dial ");
    console_write(peer);
    console_write("\n");
    return 0;
}

int phone_answer(void) {
    if (state != JAGX_CALL_RINGING) return -1;
    jagx_call_answer();
    state = JAGX_CALL_ACTIVE;
    return 0;
}

int phone_hangup(void) {
    jagx_call_hangup();
    state = JAGX_CALL_ENDED;
    console_write("[PHONE] Call ended\n");
    return 0;
}

int phone_mute(int on) {
    muted = on ? 1 : 0;
    return 0;
}

enum jagx_call_state phone_state(void) { return state; }
const char* phone_peer(void) { return peer; }

void phone_draw(int x, int y, int w, int h) {
    if (!fb_is_ready()) return;
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, (uint32_t)h, 0xFF0B0D12);
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, 36, 0xFF1EC9BC);
    /* keypad grid */
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 3; c++)
            fb_fill_rect((uint32_t)(x + 16 + c * ((w - 40) / 3)),
                         (uint32_t)(y + 80 + r * 48),
                         (uint32_t)((w - 52) / 3), 40, 0xFF1A1D26);
    uint32_t call_col = state == JAGX_CALL_ACTIVE ? 0xFFE24A4A : 0xFF1EC9BC;
    fb_fill_rect((uint32_t)(x + w / 2 - 28), (uint32_t)(y + h - 56), 56, 40, call_col);
}
