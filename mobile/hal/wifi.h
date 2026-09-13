#ifndef JAGX_HAL_WIFI_H
#define JAGX_HAL_WIFI_H

#include <stdint.h>

struct jagx_wifi_ops {
    int (*set_enabled)(int on);
    int (*connect)(const char* ssid, const char* psk);
    int (*disconnect)(void);
};

void jagx_wifi_register(const struct jagx_wifi_ops* ops);
int  jagx_wifi_set(int on);

#endif
