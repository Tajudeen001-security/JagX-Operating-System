#ifndef JAGX_FEATURES_H
#define JAGX_FEATURES_H

/* Extra useful system features */

void features_init(void);

/* Quick actions */
void feature_flashlight_toggle(void);
void feature_do_not_disturb_toggle(void);
int  feature_do_not_disturb(void);

/* Battery saver (policy flag until power HAL exists) */
void feature_battery_saver_toggle(void);
int  feature_battery_saver(void);

/* Clipboard */
void clipboard_set(const char* text);
const char* clipboard_get(void);

/* Calculator stub result log */
int feature_calc_add(int a, int b);

#endif
