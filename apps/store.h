#ifndef JAGX_STORE_H
#define JAGX_STORE_H

void store_init(void);
int  store_install_path(const char* jagx_path);
void store_list_console(void);
void store_draw(int x, int y, int w, int h);

#endif
