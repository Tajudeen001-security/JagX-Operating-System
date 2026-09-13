#ifndef JAGX_MESSAGES_H
#define JAGX_MESSAGES_H

#include <stdint.h>

#define JAGX_SMS_MAX 32
#define JAGX_SMS_BODY 160

struct jagx_sms {
    char from[24];
    char to[24];
    char body[JAGX_SMS_BODY];
    int inbound;
    int used;
};

void messages_init(void);
int  messages_send(const char* number, const char* text);
int  messages_count(void);
const struct jagx_sms* messages_at(int index);
void messages_draw(int x, int y, int w, int h);

#endif
