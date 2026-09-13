#ifndef JAGX_WIFI_HOTSPOT_H
#define JAGX_WIFI_HOTSPOT_H

#include <stdint.h>

/* Join another phone's hotspot OR run a soft AP (tethering).
 * Real RF needs a Wi-Fi driver; this is the system API + state machine.
 */

#define WIFI_SSID_MAX 32
#define WIFI_PSK_MAX  64
#define WIFI_SCAN_MAX 8

enum wifi_mode {
    WIFI_MODE_OFF = 0,
    WIFI_MODE_STA,      /* join network / phone hotspot */
    WIFI_MODE_AP        /* this device is the hotspot */
};

struct wifi_network {
    char ssid[WIFI_SSID_MAX];
    int  signal;   /* 0-100 */
    int  secure;
};

void wifi_stack_init(void);
int  wifi_scan(struct wifi_network* out, int max);
int  wifi_join(const char* ssid, const char* psk); /* connect to phone hotspot or router */
int  wifi_disconnect(void);
int  wifi_start_hotspot(const char* ssid, const char* psk); /* share this device */
int  wifi_stop_hotspot(void);
enum wifi_mode wifi_get_mode(void);
const char* wifi_current_ssid(void);
int  wifi_is_connected(void);

#endif
