#ifndef JAGX_SCREENSHOT_H
#define JAGX_SCREENSHOT_H

#include <stdint.h>

/* Capture current framebuffer into gallery */
int screenshot_take(void);

/* Triple-tap gesture: call from touch/mouse path */
void screenshot_on_tap(int x, int y);
void screenshot_set_triple_tap_enabled(int enabled);
int  screenshot_triple_tap_enabled(void);

#endif
