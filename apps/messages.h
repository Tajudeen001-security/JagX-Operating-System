#ifndef JAGX_MESSAGES_H
#define JAGX_MESSAGES_H
void messages_init(void);
int  messages_send(const char* to, const char* body);
void messages_draw(int x, int y, int w, int h);
#endif
