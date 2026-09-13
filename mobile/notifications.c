#include "notifications.h"
#include "../kernel/arch/x86_64/console.h"

static struct jagx_notification list[JAGX_NOTIF_MAX];
static uint32_t next_id = 1;
static int count = 0;

void notifications_init(void) {
    for (int i = 0; i < JAGX_NOTIF_MAX; i++) list[i].used = 0;
    count = 0;
    console_write("[NOTIF] Shade ready\n");
}

int notifications_push(const char* title, const char* body) {
    for (int i = 0; i < JAGX_NOTIF_MAX; i++) {
        if (list[i].used) continue;
        list[i].used = 1;
        list[i].id = next_id++;
        int j = 0;
        if (title) while (title[j] && j < 39) { list[i].title[j] = title[j]; j++; }
        list[i].title[j] = 0;
        j = 0;
        if (body) while (body[j] && j < 79) { list[i].body[j] = body[j]; j++; }
        list[i].body[j] = 0;
        count++;
        console_write("[NOTIF] ");
        console_write(list[i].title);
        console_write("\n");
        return (int)list[i].id;
    }
    return -1;
}

void notifications_clear(void) {
    for (int i = 0; i < JAGX_NOTIF_MAX; i++) list[i].used = 0;
    count = 0;
}

int notifications_count(void) { return count; }

const struct jagx_notification* notifications_get(int index) {
    int seen = 0;
    for (int i = 0; i < JAGX_NOTIF_MAX; i++) {
        if (!list[i].used) continue;
        if (seen == index) return &list[i];
        seen++;
    }
    return 0;
}
