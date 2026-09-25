#ifndef JAGX_CONTROL_CENTER_H
#define JAGX_CONTROL_CENTER_H

#include <stdint.h>

struct jagx_cc_state {
    int wifi;
    int mobile_data;
    int bluetooth;
    int airplane;
    int torch;
    int rotation_lock;
    int hotspot; /* soft-AP on this device */
};

void control_center_init(void);
void control_center_toggle_wifi(void);
void control_center_toggle_hotspot(void);
void control_center_join_hotspot(const char* ssid, const char* psk);
void control_center_toggle_data(void);
void control_center_toggle_airplane(void);
void control_center_toggle_torch(void);
void control_center_screenshot(void);
void control_center_toggle_record(void);
struct jagx_cc_state control_center_get(void);

#endif
