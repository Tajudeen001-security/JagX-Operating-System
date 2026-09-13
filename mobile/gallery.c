#include "gallery.h"
#include "../kernel/arch/x86_64/console.h"

static struct jagx_media_item items[JAGX_GALLERY_MAX];
static int count = 0;
static uint32_t tick = 1;

void gallery_init(void) {
    for (int i = 0; i < JAGX_GALLERY_MAX; i++) items[i].used = 0;
    count = 0;
    console_write("[GALLERY] Media library ready\n");
}

int gallery_add(enum jagx_media_type type, const char* name,
                uint32_t w, uint32_t h, uint32_t size) {
    for (int i = 0; i < JAGX_GALLERY_MAX; i++) {
        if (items[i].used) continue;
        items[i].used = 1;
        items[i].type = type;
        items[i].width = w;
        items[i].height = h;
        items[i].size_bytes = size;
        items[i].timestamp = tick++;
        int j = 0;
        if (name) while (name[j] && j < JAGX_GALLERY_NAME - 1) {
            items[i].name[j] = name[j]; j++;
        }
        items[i].name[j] = 0;
        count++;
        console_write("[GALLERY] Added: ");
        console_write(items[i].name);
        console_write("\n");
        return i;
    }
    return -1;
}

int gallery_count(void) { return count; }

const struct jagx_media_item* gallery_get(int index) {
    int seen = 0;
    for (int i = 0; i < JAGX_GALLERY_MAX; i++) {
        if (!items[i].used) continue;
        if (seen == index) return &items[i];
        seen++;
    }
    return 0;
}

void gallery_list_console(void) {
    console_write("[GALLERY] Items: ");
    console_write_dec((uint32_t)count);
    console_write("\n");
    for (int i = 0; i < count; i++) {
        const struct jagx_media_item* m = gallery_get(i);
        if (!m) continue;
        console_write("  - ");
        console_write(m->name);
        if (m->type == JAGX_MEDIA_SCREENSHOT) console_write(" [screenshot]");
        else if (m->type == JAGX_MEDIA_RECORDING) console_write(" [recording]");
        else console_write(" [photo]");
        console_write("\n");
    }
}
