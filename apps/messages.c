#include "messages.h"
#include "../mobile/hal/radio.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

void messages_init(void) {
    console_write("[SMS] Messages app ready\n");
}

int messages_send(const char* to, const char* body) {
    console_write("[SMS] To ");
    if (to) console_write(to);
    console_write(": ");
    if (body) console_write(body);
    console_write("\n");
    return jagx_sms_send(to, body);
}

void messages_draw(int x, int y, int w, int h) {
    if (!fb_is_ready()) return;
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, (uint32_t)h, 0xFF12121A);
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, 36, 0xFF9B59B6);
    for (int i = 0; i < 5; i++)
        fb_fill_rect((uint32_t)(x+10), (uint32_t)(y+50+i*40), (uint32_t)(w-20), 32, 0xFF1E2A3A);
}
