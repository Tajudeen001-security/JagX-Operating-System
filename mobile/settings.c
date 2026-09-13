#include "settings.h"
#include "../compositor/compositor.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

extern struct compositor g_compositor;

static const struct settings_item network_items[] = {
    {"Wi-Fi", "Turn wireless on or off"},
    {"Mobile data", "Cellular data toggle"},
    {"Airplane mode", "Disable radios"},
    {"SIM & network", "Signal and carrier (needs modem HAL)"},
};
static const struct settings_item display_items[] = {
    {"Brightness", "Screen brightness"},
    {"Wallpaper", "Home background"},
    {"Dark theme", "JagX default"},
};
static const struct settings_item sound_items[] = {
    {"Volume", "Media and calls"},
    {"Ringtone", "Incoming calls"},
};
static const struct settings_item security_items[] = {
    {"Lock screen", "PIN / future biometrics"},
    {"Permissions", "Capability manager"},
    {"Encryption", "Device encryption policy"},
};
static const struct settings_item about_items[] = {
    {"JagX OS", "v0.0.16"},
    {"Founder", "Gbadamosi Tajudeen Olajide"},
    {"Build", "From-scratch PC + Mobile"},
};

void settings_init(void) {
    console_write("[SETTINGS] App skeleton ready\n");
}

const char* settings_category_name(enum settings_category c) {
    switch (c) {
        case SETTINGS_NETWORK: return "Network";
        case SETTINGS_DISPLAY: return "Display";
        case SETTINGS_SOUND: return "Sound";
        case SETTINGS_SECURITY: return "Security";
        case SETTINGS_ABOUT: return "About";
        default: return "?";
    }
}

int settings_category_count(enum settings_category c) {
    switch (c) {
        case SETTINGS_NETWORK: return 4;
        case SETTINGS_DISPLAY: return 3;
        case SETTINGS_SOUND: return 2;
        case SETTINGS_SECURITY: return 3;
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
        case SETTINGS_ABOUT: return &about_items[index];
        default: return 0;
    }
}

void settings_draw(void) {
    if (!fb_is_ready()) {
        console_write("[SETTINGS] Network / Display / Sound / Security / About\n");
        return;
    }
    /* Settings content drawn as a dedicated window by caller */
}
