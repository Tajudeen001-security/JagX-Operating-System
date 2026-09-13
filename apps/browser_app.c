/* JagBrowser shell — native JagX web client, not Chromium/WebView APK. */
#include "browser_app.h"
#include "../browser/search.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

static struct jagx_nav_result last;
static char last_url[128];

static void copy_str(char* dst, const char* src, int max) {
    int i = 0;
    if (src) while (src[i] && i < max - 1) { dst[i] = src[i]; i++; }
    dst[i] = 0;
}

void browser_app_init(void) {
    last.kind = JAGX_NAV_LOCAL;
    last.title = "JagStart";
    last.body = "JagBrowser — native engine. Search or enter a URL.";
    last.url = "jagx://start";
    copy_str(last_url, "jagx://start", 128);
    console_write("[BROWSER] JagBrowser ready\n");
}

void browser_app_go(const char* input) {
    if (!input) return;
    jagx_navigate_or_search(input, &last);
    if (last.url) copy_str(last_url, last.url, 128);
    console_write("[BROWSER] nav\n");
}

const char* browser_app_title(void) { return last.title ? last.title : "JagBrowser"; }
const char* browser_app_url(void) { return last_url; }

void browser_app_draw(int x, int y, int w, int h) {
    if (!fb_is_ready()) return;
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, (uint32_t)h, 0xFF0B0D12);
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, 32, 0xFF12141A);
    fb_fill_rect((uint32_t)(x + 48), (uint32_t)(y + 6), (uint32_t)(w - 96), 20, 0xFF1A1D26);
    fb_fill_rect((uint32_t)(x + 8), (uint32_t)(y + 44), (uint32_t)(w - 16), (uint32_t)(h - 56), 0xFF16181F);
}
