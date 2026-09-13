#ifndef JAGX_DRIVER_H
#define JAGX_DRIVER_H

#include <stdint.h>

/* Generic driver registration framework for PC + Mobile ports */

enum driver_class {
    DRV_DISPLAY = 0,
    DRV_INPUT,
    DRV_STORAGE,
    DRV_NETWORK,
    DRV_AUDIO,
    DRV_CAMERA,
    DRV_MODEM,
    DRV_POWER,
    DRV_SENSOR,
    DRV_COUNT
};

struct jagx_driver {
    const char* name;
    enum driver_class cls;
    int (*probe)(void);
    int (*init)(void);
    int (*shutdown)(void);
    void* priv;
};

void drivers_init(void);
int  driver_register(const struct jagx_driver* d);
int  drivers_probe_all(void);
const char* driver_class_name(enum driver_class c);
void drivers_list_console(void);

#endif
