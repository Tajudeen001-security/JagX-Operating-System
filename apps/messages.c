#include "messages.h"
#include "../mobile/hal/radio.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

static struct msg_thread threads[MSG_MAX_THREADS];
static int n_threads = 0;
static int selected = 0;

static void add_thread(const char* from, const char* preview, int unread) {
    if (n_threads >= MSG_MAX_THREADS) return;
    int i = 0;
    while (from && from[i] && i < MSG_NAME_LEN-1) { threads[n_threads].from[i] = from[i]; i++; }
    threads[n_threads].from[i] = 0;
    i = 0;
    while (preview && preview[i] && i < MSG_BODY_LEN-1) { threads[n_threads].preview[i] = preview[i]; i++; }
    threads[n_threads].preview[i] = 0;
    threads[n_threads].unread = unread;
    threads[n_threads].used = 1;
    n_threads++;
}

void messages_init(void) {
    n_threads = 0;
    selected = 0;
    add_thread("Ministry IT", "Meeting at 10am confirmed", 1);
    add_thread("Support", "Your ticket #442 is open", 1);
    add_thread("Lagos Office", "Documents received, thanks", 0);
    add_thread("Emergency", "Test SMS path OK", 0);
    console_write("[SMS] Messages threads ready\n");
}

int messages_send(const char* to, const char* body) {
    console_write("[SMS] To ");
    if (to) console_write(to);
    console_write(": ");
    if (body) console_write(body);
    console_write("\n");
    if (to && body) add_thread(to, body, 0);
    return jagx_sms_send(to, body);
}

int messages_thread_count(void) { return n_threads; }

const struct msg_thread* messages_thread(int i) {
    if (i < 0 || i >= n_threads || !threads[i].used) return 0;
    return &threads[i];
}

void messages_draw(int x, int y, int w, int h) {
    if (!fb_is_ready()) return;
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, (uint32_t)h, 0xFF12121A);
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, 36, 0xFF9B59B6);
    /* compose bar */
    fb_fill_rect((uint32_t)(x + 8), (uint32_t)(y + h - 40), (uint32_t)(w - 16), 28, 0xFF1E2A3A);
    fb_fill_rect((uint32_t)(x + w - 50), (uint32_t)(y + h - 38), 36, 24, 0xFF9B59B6);
    int rows = (h - 90) / 40;
    if (rows > n_threads) rows = n_threads;
    if (rows > 8) rows = 8;
    for (int i = 0; i < rows; i++) {
        uint32_t col = (i == selected) ? 0xFF2A1E3A : 0xFF1E2A3A;
        fb_fill_rect((uint32_t)(x + 10), (uint32_t)(y + 48 + i * 40), (uint32_t)(w - 20), 34, col);
        if (threads[i].unread)
            fb_fill_rect((uint32_t)(x + 14), (uint32_t)(y + 56 + i * 40), 8, 8, 0xFF9B59B6);
        fb_fill_rect((uint32_t)(x + 28), (uint32_t)(y + 54 + i * 40), 100, 8, 0xFF3A4A5A);
        fb_fill_rect((uint32_t)(x + 28), (uint32_t)(y + 68 + i * 40), (uint32_t)(w - 50), 6, 0xFF2A3A4A);
    }
}

void messages_on_click(int x, int y, int ox, int oy, int w, int h) {
    if (x < ox || y < oy || x >= ox + w || y >= oy + h) return;
    int ly = y - oy - 48;
    if (ly >= 0 && ly < 8 * 40) {
        int idx = ly / 40;
        if (idx < n_threads) {
            selected = idx;
            threads[idx].unread = 0;
        }
    }
    /* Send button */
    if (x >= ox + w - 50 && x < ox + w - 14 && y >= oy + h - 38 && y < oy + h - 14) {
        messages_send("Support", "Hello from JagX Messages");
    }
}
