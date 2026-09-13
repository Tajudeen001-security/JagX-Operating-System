#ifndef JAGX_GALLERY_H
#define JAGX_GALLERY_H

#include <stdint.h>

#define JAGX_GALLERY_MAX 32
#define JAGX_GALLERY_NAME 48

enum jagx_media_type {
    JAGX_MEDIA_PHOTO = 0,
    JAGX_MEDIA_SCREENSHOT = 1,
    JAGX_MEDIA_RECORDING = 2
};

struct jagx_media_item {
    int used;
    enum jagx_media_type type;
    char name[JAGX_GALLERY_NAME];
    uint32_t width;
    uint32_t height;
    uint32_t size_bytes;
    uint32_t timestamp;
};

void gallery_init(void);
int  gallery_add(enum jagx_media_type type, const char* name,
                 uint32_t w, uint32_t h, uint32_t size);
int  gallery_count(void);
const struct jagx_media_item* gallery_get(int index);
void gallery_list_console(void);

#endif
