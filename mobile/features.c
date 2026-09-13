#include "features.h"
#include "hal/lights.h"
#include "../kernel/arch/x86_64/console.h"

static int dnd = 0;
static int battery_saver = 0;
static char clip[256];
static int torch = 0;

void features_init(void) {
    clip[0] = 0;
    console_write("[FEATURES] Flashlight, DND, Battery saver, Clipboard ready\n");
}

void feature_flashlight_toggle(void) {
    torch = !torch;
    jagx_torch_set(torch ? 1 : 0);
}

void feature_do_not_disturb_toggle(void) {
    dnd = !dnd;
    console_write(dnd ? "[FEATURES] Do Not Disturb ON\n" : "[FEATURES] Do Not Disturb OFF\n");
}

int feature_do_not_disturb(void) { return dnd; }

void feature_battery_saver_toggle(void) {
    battery_saver = !battery_saver;
    console_write(battery_saver ? "[FEATURES] Battery saver ON\n" : "[FEATURES] Battery saver OFF\n");
}

int feature_battery_saver(void) { return battery_saver; }

void clipboard_set(const char* text) {
    int i = 0;
    if (!text) { clip[0] = 0; return; }
    while (text[i] && i < 255) { clip[i] = text[i]; i++; }
    clip[i] = 0;
}

const char* clipboard_get(void) { return clip; }

int feature_calc_add(int a, int b) { return a + b; }
