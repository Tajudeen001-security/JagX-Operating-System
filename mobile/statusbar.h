#ifndef JAGX_STATUSBAR_H
#define JAGX_STATUSBAR_H

#include <stdint.h>

void statusbar_init(void);
void statusbar_set_battery(int percent); /* 0-100 */
void statusbar_set_time(int hours, int mins);
void statusbar_tick(void); /* advance clock from timer */
void statusbar_draw(void);
int  statusbar_battery(void);

#endif
