#ifndef JAGX_SOCIAL_H
#define JAGX_SOCIAL_H

#include <stdint.h>

#define JAGX_POST_MAX 24
#define JAGX_POST_BODY 280

struct jagx_post {
    char author[32];
    char body[JAGX_POST_BODY];
    uint32_t likes;
    int used;
};

void social_init(void);
int  social_post(const char* author, const char* body);
int  social_like(int index);
int  social_count(void);
const struct jagx_post* social_at(int index);
void social_draw(int x, int y, int w, int h);

#endif
