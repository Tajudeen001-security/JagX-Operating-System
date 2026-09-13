#ifndef JAGX_SETTINGS_H
#define JAGX_SETTINGS_H

/* Settings app skeleton — categories for mobile + PC */

enum settings_category {
    SETTINGS_NETWORK = 0,
    SETTINGS_DISPLAY,
    SETTINGS_SOUND,
    SETTINGS_SECURITY,
    SETTINGS_ABOUT,
    SETTINGS_COUNT
};

struct settings_item {
    const char* title;
    const char* subtitle;
};

void settings_init(void);
const char* settings_category_name(enum settings_category c);
int settings_category_count(enum settings_category c);
const struct settings_item* settings_item_at(enum settings_category c, int index);
void settings_draw(void); /* draws into compositor window if FB ready */

#endif
