#include "settings.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

static const struct settings_item network_items[] = {
    {"Wi-Fi", "Networks and hotspot"},
    {"Mobile data", "SIM data"},
    {"Airplane mode", "Disable radios"},
    {"Hotspot", "Share connection"},
};
static const struct settings_item display_items[] = {
    {"Brightness", "Screen level"},
    {"Wallpaper", "Lock and home"},
    {"Dark theme", "JagX default"},
};
static const struct settings_item sound_items[] = {
    {"Volume", "Media and calls"},
    {"Ringtone", "Incoming calls"},
};
static const struct settings_item security_items[] = {
    {"PIN", "4–8 digit unlock"},
    {"Password", "Alphanumeric unlock"},
    {"Fingerprint", "Sensor when available"},
    {"Face unlock", "Camera when available"},
};
static const struct settings_item apps_items[] = {
    {"Noder", "Code editor"},
    {"Phone", "Contacts and dialer"},
    {"Messages", "SMS"},
    {"Gallery", "Photos and shots"},
};
static const struct settings_item about_items[] = {
    {"JagX OS", "v0.0.26"},
    {"Founder", "Gbadamosi Tajudeen Olajide"},
    {"Vendor", "JagX & JRILICENSE"},
};

void settings_init(void) {
    console_write("[SETTINGS] Full categories including Security biometrics\n");
}

const char* settings_category_name(enum settings_category c) {
    switch (c) {
        case SETTINGS_NETWORK: return "Network";
        case SETTINGS_DISPLAY: return "Display";
        case SETTINGS_SOUND: return "Sound";
        case SETTINGS_SECURITY: return "Security";
        case SETTINGS_APPS: return "Apps";
        case SETTINGS_ABOUT: return "About";
        default: return "?";
    }
}

int settings_category_count(enum settings_category c) {
    switch (c) {
        case SETTINGS_NETWORK: return 4;
        case SETTINGS_DISPLAY: return 3;
        case SETTINGS_SOUND: return 2;
        case SETTINGS_SECURITY: return 4;
        case SETTINGS_APPS: return 4;
        case SETTINGS_ABOUT: return 3;
        default: return 0;
    }
}

const struct settings_item* settings_item_at(enum settings_category c, int index) {
    if (index < 0 || index >= settings_category_count(c)) return 0;
    switch (c) {
        case SETTINGS_NETWORK: return &network_items[index];
        case SETTINGS_DISPLAY: return &display_items[index];
        case SETTINGS_SOUND: return &sound_items[index];
        case SETTINGS_SECURITY: return &security_items[index];
        case SETTINGS_APPS: return &apps_items[index];
        case SETTINGS_ABOUT: return &about_items[index];
        default: return 0;
    }
}

void settings_draw(void) { settings_draw_at(80, 50, 520, 400); }

void settings_draw_at(int x, int y, int w, int h) {
    if (!fb_is_ready()) return;
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, (uint32_t)h, 0xFF14141E);
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, 40, 0xFF00D4C8);
    for (int i = 0; i < SETTINGS_COUNT; i++)
        fb_fill_rect((uint32_t)(x+8), (uint32_t)(y+52+i*44), 130, 38,
                     i == 3 ? 0xFF7B5EA7 : 0xFF1E2A3A);
    for (int i = 0; i < 4; i++) {
        fb_fill_rect((uint32_t)(x+150), (uint32_t)(y+52+i*52), (uint32_t)(w-165), 44, 0xFF1E2A3A);
        fb_fill_rect((uint32_t)(x+160), (uint32_t)(y+62+i*52), 180, 10, 0xFF2A3A4A);
    }
}
