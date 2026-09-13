#ifndef JAGX_MOUSE_H
#define JAGX_MOUSE_H

#include <stdint.h>

struct mouse_state {
    int32_t x;
    int32_t y;
    uint8_t buttons;   // bit0=left, bit1=right, bit2=middle
};

void mouse_init(void);
void mouse_handler(void);
struct mouse_state mouse_get_state(void);

#endif
