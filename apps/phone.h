#ifndef JAGX_PHONE_H
#define JAGX_PHONE_H

#include <stdint.h>

enum jagx_call_state {
    JAGX_CALL_IDLE = 0,
    JAGX_CALL_DIALING,
    JAGX_CALL_RINGING,
    JAGX_CALL_ACTIVE,
    JAGX_CALL_HELD,
    JAGX_CALL_ENDED
};

void phone_init(void);
int  phone_dial(const char* number);
int  phone_answer(void);
int  phone_hangup(void);
int  phone_mute(int on);
enum jagx_call_state phone_state(void);
const char* phone_peer(void);
void phone_draw(int x, int y, int w, int h);

#endif
