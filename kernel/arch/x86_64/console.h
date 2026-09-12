#ifndef JAGX_CONSOLE_H
#define JAGX_CONSOLE_H

void console_init(void);
void console_clear(void);
void console_write(const char* str);
void console_write_hex(uint32_t n);
void console_write_dec(uint32_t n);

#endif
