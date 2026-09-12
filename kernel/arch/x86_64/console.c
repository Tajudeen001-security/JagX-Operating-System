#include "console.h"
#include <stdint.h>

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

static volatile uint16_t* vga = (volatile uint16_t*)VGA_MEMORY;
static int cursor_x = 0;
static int cursor_y = 0;
static uint8_t color = 0x0A;  /* light green on black */

static void update_cursor(void) {
    /* Hardware cursor update (optional) */
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    /* outb(0x3D4, 0x0F); outb(0x3D5, (uint8_t)(pos & 0xFF)); ... */
    (void)pos;
}

static void scroll(void) {
    if (cursor_y >= VGA_HEIGHT) {
        for (int i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++) {
            vga[i] = vga[i + VGA_WIDTH];
        }
        for (int i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++) {
            vga[i] = (color << 8) | ' ';
        }
        cursor_y = VGA_HEIGHT - 1;
    }
}

void console_clear(void) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga[i] = (color << 8) | ' ';
    }
    cursor_x = 0;
    cursor_y = 0;
    update_cursor();
}

void console_init(void) {
    console_clear();
}

void console_putchar(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\b') {
        if (cursor_x > 0) {
            cursor_x--;
            vga[cursor_y * VGA_WIDTH + cursor_x] = (color << 8) | ' ';
        }
    } else if (c == '\t') {
        cursor_x = (cursor_x + 4) & ~3;
    } else {
        vga[cursor_y * VGA_WIDTH + cursor_x] = (color << 8) | c;
        cursor_x++;
    }

    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    scroll();
    update_cursor();
}

void console_write(const char* str) {
    while (*str) {
        console_putchar(*str++);
    }
}

void console_write_hex(uint32_t n) {
    console_write("0x");
    for (int i = 28; i >= 0; i -= 4) {
        uint8_t nibble = (n >> i) & 0xF;
        char c = nibble < 10 ? ('0' + nibble) : ('A' + nibble - 10);
        console_putchar(c);
    }
}

void console_write_dec(uint32_t n) {
    if (n == 0) {
        console_putchar('0');
        return;
    }
    char buf[12];
    int i = 0;
    while (n > 0) {
        buf[i++] = '0' + (n % 10);
        n /= 10;
    }
    while (--i >= 0) {
        console_putchar(buf[i]);
    }
}
