/* JagX Messages — native SMS. Uses radio HAL. Never an APK. */
#include "messages.h"
#include "../mobile/hal/radio.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

static struct jagx_sms box[JAGX_SMS_MAX];
static int n = 0;

static void copy_str(char* dst, const char* src, int max) {
    int i = 0;
    if (src) while (src[i] && i < max - 1) { dst[i] = src[i]; i++; }
    dst[i] = 0;
}

static void seed(const char* from, const char* body, int inbound) {
    if (n >= JAGX_SMS_MAX) return;
    copy_str(box[n].from, from, 24);
    copy_str(box[n].to, inbound ? "me" : from, 24);
    copy_str(box[n].body, body, JAGX_SMS_BODY);
    box[n].inbound = inbound;
    box[n].used = 1;
    n++;
}

void messages_init(void) {
    n = 0;
    seed("+234-800-JAGX", "Welcome to JagX Messages. Native SMS, not Android.", 1);
    seed("Ministry IT", "Pilot lab online. Use .jagx apps only.", 1);
    console_write("[SMS] Native JagX messaging ready\n");
}

int messages_send(const char* number, const char* text) {
    if (!number || !text || n >= JAGX_SMS_MAX) return -1;
    copy_str(box[n].from, "me", 24);
    copy_str(box[n].to, number, 24);
    copy_str(box[n].body, text, JAGX_SMS_BODY);
    box[n].inbound = 0;
    box[n].used = 1;
    n++;
    if (jagx_sms_send(number, text) != 0)
        console_write("[SMS] Queued locally (modem HAL stub)\n");
    return 0;
}

int messages_count(void) { return n; }

const struct jagx_sms* messages_at(int index) {
    if (index < 0 || index >= n) return 0;
    return &box[index];
}

void messages_draw(int x, int y, int w, int h) {
    if (!fb_is_ready()) return;
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, (uint32_t)h, 0xFF0B0D12);
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, 36, 0xFF1EC9BC);
    int show = n < 6 ? n : 6;
    for (int i = 0; i < show; i++) {
        uint32_t col = box[n - 1 - i].inbound ? 0xFF1A1D26 : 0xFF143C38;
        fb_fill_rect((uint32_t)(x + 10), (uint32_t)(y + 48 + i * 42),
                     (uint32_t)(w - 20), 36, col);
    }
}
