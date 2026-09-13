#include "control_center.h"
#include "hal/wifi.h"
#include "hal/radio.h"
#include "hal/lights.h"
#include "../kernel/arch/x86_64/console.h"

static struct jagx_cc_state cc;

void control_center_init(void) {
    cc.wifi = 0;
    cc.mobile_data = 0;
    cc.bluetooth = 0;
    cc.airplane = 0;
    cc.torch = 0;
    cc.rotation_lock = 0;
    console_write("[CC] Control Center ready\n");
}

void control_center_toggle_wifi(void) {
    cc.wifi = !cc.wifi;
    jagx_wifi_set(cc.wifi);
}

void control_center_toggle_data(void) {
    if (cc.airplane) return;
    cc.mobile_data = !cc.mobile_data;
    jagx_mobile_data_set(cc.mobile_data);
}

void control_center_toggle_airplane(void) {
    cc.airplane = !cc.airplane;
    jagx_airplane_set(cc.airplane);
    if (cc.airplane) {
        cc.mobile_data = 0;
        cc.wifi = 0;
    }
}

void control_center_toggle_torch(void) {
    cc.torch = !cc.torch;
    jagx_torch_set(cc.torch ? 1 : 0);
}

struct jagx_cc_state control_center_get(void) {
    return cc;
}
