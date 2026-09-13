#ifndef JAGX_NOTEPAD_H
#define JAGX_NOTEPAD_H

#include <stdint.h>

#define NOTEPAD_MAX 2048

void notepad_init(void);
void notepad_clear(void);
void notepad_insert_char(char c);
void notepad_backspace(void);
const char* notepad_text(void);
uint32_t notepad_length(void);
int  notepad_save(const char* path);
int  notepad_load(const char* path);
void notepad_draw(int x, int y, int w, int h);

#endif
