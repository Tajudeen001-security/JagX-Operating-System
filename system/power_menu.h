#ifndef JAGX_POWER_MENU_H
#define JAGX_POWER_MENU_H

void power_menu_init(void);
void power_menu_show(void);
void power_menu_hide(void);
int  power_menu_is_open(void);
void power_menu_draw(void);
void power_menu_on_click(int x, int y);

#endif
