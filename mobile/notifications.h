#ifndef JAGX_NOTIFICATIONS_H
#define JAGX_NOTIFICATIONS_H

#include <stdint.h>

#define JAGX_NOTIF_MAX 16

struct jagx_notification {
    int used;
    char title[40];
    char body[80];
    uint32_t id;
};

void notifications_init(void);
int  notifications_push(const char* title, const char* body);
void notifications_clear(void);
int  notifications_count(void);
const struct jagx_notification* notifications_get(int index);

#endif
