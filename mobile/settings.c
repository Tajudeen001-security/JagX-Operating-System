#include "settings.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

static const struct settings_item network_items[] = {
    {"Wi-Fi", "Wireless networks"},
    {"Mobile data", "Cellular toggle"},
    {"Airplane mode", "Disable radios"},
    {"SIM & network", "Needs modem HAL"},
};
static const struct settings_item display_items[] = {
    {"Brightness", "Screen level"},
    {"Wallpaper", "Home background"},
    {"Dark theme", "JagX default"},
};
static const struct settings_item sound_items[] = {
    {"Volume", "Media and calls"},
    {"Ringtone", "Incoming calls"},
};
static const struct settings_item security_items[] = {
    {"Lock screen", "Swipe unlock"},
    {"Permissions", "Capabilities"},
    {"Encryption", "Device policy"},
};
static const struct settings_item about_items[] = {
    {"JagX OS", "v0.0.19"},
    {"Founder", "Gbadamosi Tajudeen Olajide"},
    {"Build", "PC + Mobile"},
};

void settings_init(void) {
    console_write("[SETTINGS] UI ready\n");
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
        console_write("[SETTINGS] categories listed on console\n");
        return;
    }
    /* Richer settings panel */
    int x = 120, y = 60, w = 480, h = 400;
    fb_fill_rect(x + 6, y + 6, (uint32_t)w, (uint32_t)h, 0xFF000000);
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, (uint32_t)h, 0xFF14141E);
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, 40, 0xFF00D4C8);
    /* sidebar categories */
    for (int i = 0; i < SETTINGS_COUNT; i++) {
        uint32_t col = (i == 0) ? 0xFF7B5EA7 : 0xFF1E2A3A;
        fb_fill_rect((uint32_t)(x + 8), (uint32_t)(y + 52 + i * 48), 120, 40, col);
    }
    /* content rows */
    for (int i = 0; i < 4; i++) {
        fb_fill_rect((uint32_t)(x + 140), (uint32_t)(y + 52 + i * 52), 320, 44, 0xFF1E2A3A);
        fb_fill_rect((uint32_t)(x + 150), (uint32_t)(y + 60 + i * 52), 200, 12, 0xFF2A3A4A);
        fb_fill_rect((uint32_t)(x + 150), (uint32_t)(y + 78 + i * 52), 140, 8, 0xFF2A2A35);
    }
}
