#ifndef JAGX_HAL_RADIO_H
#define JAGX_HAL_RADIO_H

#include <stdint.h>

/* Cellular modem / SIM — requires real baseband driver per device */

enum jagx_radio_state {
    JAGX_RADIO_OFF = 0,
    JAGX_RADIO_ON,
    JAGX_RADIO_AIRPLANE
};

struct jagx_radio_ops {
    int (*set_power)(enum jagx_radio_state st);
    int (*set_data_enabled)(int enabled);
    int (*get_signal)(int* dbm);
    int (*sim_ready)(void);
    int (*sms_send)(const char* number, const char* text);
    int (*sms_poll)(char* out_from, char* out_body, uint32_t max);
};

void jagx_radio_register(const struct jagx_radio_ops* ops);
int  jagx_mobile_data_set(int enabled);  /* Control Center data toggle */
int  jagx_airplane_set(int enabled);
int  jagx_sms_send(const char* number, const char* text);

#endif
