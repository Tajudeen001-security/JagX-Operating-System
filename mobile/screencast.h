#ifndef JAGX_SCREENCAST_H
#define JAGX_SCREENCAST_H

#include <stdint.h>

int  screencast_start(void);
int  screencast_stop(void);
int  screencast_is_recording(void);
void screencast_tick(void); /* call periodically while recording */

#endif
