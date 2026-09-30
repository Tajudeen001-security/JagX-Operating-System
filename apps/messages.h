#ifndef JAGX_MESSAGES_H
#define JAGX_MESSAGES_H

#define MSG_MAX_THREADS 16
#define MSG_NAME_LEN 32
#define MSG_BODY_LEN 96

struct msg_thread {
    int used;
    char from[MSG_NAME_LEN];
    char preview[MSG_BODY_LEN];
    int unread;
};

void messages_init(void);
int  messages_send(const char* to, const char* body);
int  messages_thread_count(void);
const struct msg_thread* messages_thread(int i);
void messages_draw(int x, int y, int w, int h);
void messages_on_click(int x, int y, int ox, int oy, int w, int h);

#endif
