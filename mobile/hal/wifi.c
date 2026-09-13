#include "wifi.h"
#include "../../kernel/arch/x86_64/console.h"

static const struct jagx_wifi_ops* wifi_ops = 0;

void jagx_wifi_register(const struct jagx_wifi_ops* ops) {
    wifi_ops = ops;
    console_write("[HAL] Wi-Fi registered\n");
}

int jagx_wifi_set(int on) {
    if (!wifi_ops || !wifi_ops->set_enabled) {
        console_write(on ? "[HAL] Wi-Fi ON (stub)\n" : "[HAL] Wi-Fi OFF (stub)\n");
        return 0;
    }
    return wifi_ops->set_enabled(on);
}
