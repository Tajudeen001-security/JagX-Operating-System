#include "control_center.h"
#include "hal/wifi.h"
#include "hal/radio.h"
#include "hal/lights.h"
#include "screencast.h"
#include "screenshot.h"
#include "features.h"
#include "../net/wifi_hotspot.h"
#include "../kernel/arch/x86_64/console.h"

static struct jagx_cc_state cc;

/* Default join target — user/device settings should override */
static char join_ssid[32] = "AndroidAP";
static char join_psk[64] = "password";
static char ap_ssid[32] = "JagX-Share";
static char ap_psk[64] = "jagx1234";

void control_center_init(void) {
    cc.wifi = 0;
    cc.mobile_data = 0;
    cc.bluetooth = 0;
    cc.airplane = 0;
    cc.torch = 0;
    cc.rotation_lock = 0;
    cc.hotspot = 0;
    console_write("[CC] Wi-Fi/Hotspot wired to wifi_join / wifi_start_hotspot\n");
}

void control_center_toggle_wifi(void) {
    if (cc.airplane) return;
    if (cc.hotspot) {
        /* turning Wi-Fi station on stops AP mode */
        control_center_toggle_hotspot();
    }
    cc.wifi = !cc.wifi;
    if (cc.wifi) {
        jagx_wifi_set(1);
        /* Join known network / phone hotspot */
        if (wifi_join(join_ssid, join_psk) == 0)
            console_write("[CC] Wi-Fi ON → joined network/hotspot\n");
        else
            console_write("[CC] Wi-Fi ON (join failed — set SSID)\n");
    } else {
        wifi_disconnect();
        jagx_wifi_set(0);
        console_write("[CC] Wi-Fi OFF\n");
    }
}

void control_center_toggle_hotspot(void) {
    if (cc.airplane) return;
    cc.hotspot = !cc.hotspot;
    if (cc.hotspot) {
        if (cc.wifi) {
            wifi_disconnect();
            cc.wifi = 0;
        }
        if (wifi_start_hotspot(ap_ssid, ap_psk) == 0)
            console_write("[CC] Hotspot ON — others can join JagX-Share\n");
        else {
            cc.hotspot = 0;
            console_write("[CC] Hotspot start failed\n");
        }
    } else {
        wifi_stop_hotspot();
        console_write("[CC] Hotspot OFF\n");
    }
}

void control_center_join_hotspot(const char* ssid, const char* psk) {
    int i = 0;
    if (ssid) while (ssid[i] && i < 31) { join_ssid[i] = ssid[i]; i++; }
    join_ssid[i] = 0;
    i = 0;
    if (psk) while (psk[i] && i < 63) { join_psk[i] = psk[i]; i++; }
    join_psk[i] = 0;
    cc.wifi = 1;
    cc.hotspot = 0;
    jagx_wifi_set(1);
    wifi_join(join_ssid, join_psk);
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
        if (cc.wifi) { wifi_disconnect(); cc.wifi = 0; }
        if (cc.hotspot) { wifi_stop_hotspot(); cc.hotspot = 0; }
        cc.mobile_data = 0;
        jagx_wifi_set(0);
    }
}

void control_center_toggle_torch(void) {
    cc.torch = !cc.torch;
    jagx_torch_set(cc.torch ? 1 : 0);
}

struct jagx_cc_state control_center_get(void) {
    return cc;
}

void control_center_screenshot(void) {
    screenshot_take();
}

void control_center_toggle_record(void) {
    if (screencast_is_recording()) screencast_stop();
    else screencast_start();
}
