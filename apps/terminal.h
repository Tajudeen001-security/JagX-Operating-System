#ifndef JAGX_TERMINAL_H
#define JAGX_TERMINAL_H
void terminal_init(void);
void terminal_puts(const char* s);
void terminal_draw(int x, int y, int w, int h);
#endif
