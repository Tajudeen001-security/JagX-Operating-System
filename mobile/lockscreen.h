#ifndef JAGX_LOCKSCREEN_H
#define JAGX_LOCKSCREEN_H

void lockscreen_init(void);
void lockscreen_show(void);
void lockscreen_hide(void);
int  lockscreen_is_locked(void);
void lockscreen_draw(void);
void lockscreen_on_drag(int y);
void lockscreen_on_key(char c);
void lockscreen_try_fingerprint(void);
void lockscreen_try_face(void);

#endif
