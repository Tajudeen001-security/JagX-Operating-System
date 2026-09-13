#ifndef JAGX_CONTROL_CENTER_H
#define JAGX_CONTROL_CENTER_H

#include <stdint.h>

struct jagx_cc_state {
    int wifi;
    int mobile_data;
    int bluetooth;   /* stub until BT HAL */
    int airplane;
    int torch;
    int rotation_lock;
};

void control_center_init(void);
void control_center_toggle_wifi(void);
void control_center_toggle_data(void);
void control_center_toggle_airplane(void);
void control_center_toggle_torch(void);
struct jagx_cc_state control_center_get(void);

#endif
