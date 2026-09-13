#include "driver.h"
#include "../kernel/arch/x86_64/console.h"

#define MAX_DRIVERS 32
static const struct jagx_driver* table[MAX_DRIVERS];
static int count = 0;

const char* driver_class_name(enum driver_class c) {
    switch (c) {
        case DRV_DISPLAY: return "display";
        case DRV_INPUT: return "input";
        case DRV_STORAGE: return "storage";
        case DRV_NETWORK: return "network";
        case DRV_AUDIO: return "audio";
        case DRV_CAMERA: return "camera";
        case DRV_MODEM: return "modem";
        case DRV_POWER: return "power";
        case DRV_SENSOR: return "sensor";
        default: return "?";
    }
}

void drivers_init(void) {
    count = 0;
    console_write("[DRV] Framework ready\n");
}

int driver_register(const struct jagx_driver* d) {
    if (!d || count >= MAX_DRIVERS) return -1;
    table[count++] = d;
    console_write("[DRV] Registered: ");
    console_write(d->name);
    console_write("\n");
    return 0;
}

int drivers_probe_all(void) {
    int ok = 0;
    for (int i = 0; i < count; i++) {
        if (table[i]->probe && table[i]->probe() == 0) {
            if (table[i]->init) table[i]->init();
            ok++;
        }
    }
    return ok;
}

void drivers_list_console(void) {
    for (int i = 0; i < count; i++) {
        console_write("  ");
        console_write(driver_class_name(table[i]->cls));
        console_write(": ");
        console_write(table[i]->name);
        console_write("\n");
    }
}
