#ifndef JAGX_LOCKSCREEN_H
#define JAGX_LOCKSCREEN_H

void lockscreen_init(void);
void lockscreen_show(void);
void lockscreen_hide(void);
int  lockscreen_is_locked(void);
void lockscreen_draw(void);
/* Simple swipe-up unlock: call with y delta */
void lockscreen_on_drag(int y);

#endif
