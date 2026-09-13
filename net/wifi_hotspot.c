#include "wifi_hotspot.h"
#include "../mobile/hal/wifi.h"
#include "../kernel/arch/x86_64/console.h"

static enum wifi_mode mode = WIFI_MODE_OFF;
static char cur_ssid[WIFI_SSID_MAX];
static int connected = 0;

/* Simulated scan list (driver fills this on real hardware) */
static struct wifi_network scan_cache[WIFI_SCAN_MAX];
static int scan_n = 0;

void wifi_stack_init(void) {
    mode = WIFI_MODE_OFF;
    cur_ssid[0] = 0;
    connected = 0;
    scan_n = 3;
    /* typical phone hotspot names users see */
    scan_cache[0].ssid[0]='A';scan_cache[0].ssid[1]='n';scan_cache[0].ssid[2]='d';
    scan_cache[0].ssid[3]='r';scan_cache[0].ssid[4]='o';scan_cache[0].ssid[5]='i';
    scan_cache[0].ssid[6]='d';scan_cache[0].ssid[7]='A';scan_cache[0].ssid[8]='P';
    scan_cache[0].ssid[9]='_';scan_cache[0].ssid[10]='1';scan_cache[0].ssid[11]='2';
    scan_cache[0].ssid[12]='3';scan_cache[0].ssid[13]='4';scan_cache[0].ssid[14]=0;
    scan_cache[0].signal = 80; scan_cache[0].secure = 1;

    scan_cache[1].ssid[0]='i';scan_cache[1].ssid[1]='P';scan_cache[1].ssid[2]='h';
    scan_cache[1].ssid[3]='o';scan_cache[1].ssid[4]='n';scan_cache[1].ssid[5]='e';
    scan_cache[1].ssid[6]=0;
    scan_cache[1].signal = 65; scan_cache[1].secure = 1;

    scan_cache[2].ssid[0]='J';scan_cache[2].ssid[1]='a';scan_cache[2].ssid[2]='g';
    scan_cache[2].ssid[3]='X';scan_cache[2].ssid[4]='-';scan_cache[2].ssid[5]='L';
    scan_cache[2].ssid[6]='a';scan_cache[2].ssid[7]='b';scan_cache[2].ssid[8]=0;
    scan_cache[2].signal = 90; scan_cache[2].secure = 1;

    console_write("[WIFI] Hotspot join + soft-AP stack ready\n");
}

int wifi_scan(struct wifi_network* out, int max) {
    if (!out || max <= 0) return 0;
    int n = scan_n < max ? scan_n : max;
    for (int i = 0; i < n; i++) out[i] = scan_cache[i];
    console_write("[WIFI] Scan complete\n");
    return n;
}

static void set_ssid(const char* ssid) {
    int i = 0;
    if (!ssid) { cur_ssid[0] = 0; return; }
    while (ssid[i] && i < WIFI_SSID_MAX - 1) { cur_ssid[i] = ssid[i]; i++; }
    cur_ssid[i] = 0;
}

int wifi_join(const char* ssid, const char* psk) {
    if (!ssid || !ssid[0]) return -1;
    (void)psk; /* driver validates PSK on real NIC */
    if (mode == WIFI_MODE_AP) wifi_stop_hotspot();
    mode = WIFI_MODE_STA;
    set_ssid(ssid);
    connected = 1;
    jagx_wifi_set(1);
    console_write("[WIFI] Joined network/hotspot: ");
    console_write(cur_ssid);
    console_write("\n");
    console_write("[WIFI] Use phone hotspot SSID+password from phone Settings\n");
    return 0;
}

int wifi_disconnect(void) {
    connected = 0;
    mode = WIFI_MODE_OFF;
    cur_ssid[0] = 0;
    jagx_wifi_set(0);
    console_write("[WIFI] Disconnected\n");
    return 0;
}

int wifi_start_hotspot(const char* ssid, const char* psk) {
    if (!ssid || !ssid[0]) return -1;
    (void)psk;
    if (mode == WIFI_MODE_STA) wifi_disconnect();
    mode = WIFI_MODE_AP;
    set_ssid(ssid);
    connected = 1;
    console_write("[WIFI] Hotspot ON: ");
    console_write(cur_ssid);
    console_write("\n");
    console_write("[WIFI] Other phones can join this SSID when Wi-Fi driver supports AP\n");
    return 0;
}

int wifi_stop_hotspot(void) {
    if (mode != WIFI_MODE_AP) return -1;
    mode = WIFI_MODE_OFF;
    connected = 0;
    cur_ssid[0] = 0;
    console_write("[WIFI] Hotspot OFF\n");
    return 0;
}

enum wifi_mode wifi_get_mode(void) { return mode; }
const char* wifi_current_ssid(void) { return cur_ssid; }
int wifi_is_connected(void) { return connected; }
