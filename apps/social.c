/* JagCircle — native JagX social. Not Facebook/X/WhatsApp APKs. */
#include "social.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

static struct jagx_post feed[JAGX_POST_MAX];
static int n = 0;

static void copy_str(char* dst, const char* src, int max) {
    int i = 0;
    if (src) while (src[i] && i < max - 1) { dst[i] = src[i]; i++; }
    dst[i] = 0;
}

static void seed(const char* a, const char* b, uint32_t likes) {
    if (n >= JAGX_POST_MAX) return;
    copy_str(feed[n].author, a, 32);
    copy_str(feed[n].body, b, JAGX_POST_BODY);
    feed[n].likes = likes;
    feed[n].used = 1;
    n++;
}

void social_init(void) {
    n = 0;
    seed("JagX", "JagCircle is native to JagX OS. No APK. Package: com.jagx.social", 128);
    seed("Lagos Lab", "Testing calls, SMS, and JagBrowser on JagX Mobile.", 44);
    seed("Abuja Pilot", "Ministries can evaluate JagX without Android lock-in.", 61);
    console_write("[SOCIAL] JagCircle native feed online\n");
}

int social_post(const char* author, const char* body) {
    if (!body || n >= JAGX_POST_MAX) return -1;
    copy_str(feed[n].author, author ? author : "me", 32);
    copy_str(feed[n].body, body, JAGX_POST_BODY);
    feed[n].likes = 0;
    feed[n].used = 1;
    n++;
    return n - 1;
}

int social_like(int index) {
    if (index < 0 || index >= n) return -1;
    feed[index].likes++;
    return 0;
}

int social_count(void) { return n; }

const struct jagx_post* social_at(int index) {
    if (index < 0 || index >= n) return 0;
    return &feed[index];
}

void social_draw(int x, int y, int w, int h) {
    if (!fb_is_ready()) return;
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, (uint32_t)h, 0xFF0B0D12);
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, 36, 0xFF1EC9BC);
    int show = n < 5 ? n : 5;
    for (int i = 0; i < show; i++)
        fb_fill_rect((uint32_t)(x + 12), (uint32_t)(y + 52 + i * 56),
                     (uint32_t)(w - 24), 48, 0xFF1A1D26);
}
