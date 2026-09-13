/* JagX Connect — native social client for JagX OS.
 * Not an APK. Install as connect.jagx via JagStore / pkg_install_path.
 */
#include "jagx_connect.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

#define JC_MAX 24
#define JC_BODY 160

struct jc_post {
    char author[32];
    char body[JC_BODY];
    char region[48];
    unsigned likes;
    int used;
};

static struct jc_post feed[JC_MAX];
static int n = 0;

static void copy_str(char* dst, const char* src, int max) {
    int i = 0;
    if (src) while (src[i] && i < max - 1) { dst[i] = src[i]; i++; }
    dst[i] = 0;
}

static void seed(const char* a, const char* b, const char* r, unsigned likes) {
    if (n >= JC_MAX) return;
    copy_str(feed[n].author, a, 32);
    copy_str(feed[n].body, b, JC_BODY);
    copy_str(feed[n].region, r, 48);
    feed[n].likes = likes;
    feed[n].used = 1;
    n++;
}

void jagx_connect_init(void) {
    n = 0;
    seed("JagX", "JagX Connect is native on JagX OS. Package com.jagx.connect", "Lagos, Nigeria", 240);
    seed("Abuja Night", "Club live + events RSVP work on Connect. No APK.", "Abuja, Nigeria", 91);
    seed("Cotonou", "West Africa feed: Nigeria, Benin, Burkina Faso locked to region.", "Cotonou, Benin", 54);
    seed("Ouaga", "Switch profiles. Ban a country from Admin Desk.", "Ouagadougou, Burkina Faso", 33);
    console_write("[CONNECT] JagX Connect native app online (.jagx)\n");
}

int jagx_connect_post(const char* author, const char* body) {
    if (!body || n >= JC_MAX) return -1;
    copy_str(feed[n].author, author ? author : "me", 32);
    copy_str(feed[n].body, body, JC_BODY);
    copy_str(feed[n].region, "detected", 48);
    feed[n].likes = 0;
    feed[n].used = 1;
    n++;
    return n - 1;
}

int jagx_connect_count(void) { return n; }

void jagx_connect_draw(int x, int y, int w, int h) {
    if (!fb_is_ready()) return;
    fb_fill_rect((unsigned)x, (unsigned)y, (unsigned)w, (unsigned)h, 0xFF0B0D12);
    fb_fill_rect((unsigned)x, (unsigned)y, (unsigned)w, 40, 0xFF1EC9BC);
    int show = n < 6 ? n : 6;
    for (int i = 0; i < show; i++)
        fb_fill_rect((unsigned)(x + 12), (unsigned)(y + 56 + i * 58),
                     (unsigned)(w - 24), 50, 0xFF16181F);
}
