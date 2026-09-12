#include "keyboard.h"
#include "io.h"
#include "pic.h"
#include "console.h"

/* Simple US QWERTY scancode set 1 (make codes only) */
static const char scancode_to_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,  'a','s','d','f','g','h','j','k','l',';','\'', '`',
    0,  '\\','z','x','c','v','b','n','m',',','.','/', 0,
    '*', 0,  ' ', 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

void keyboard_handler(void) {
    uint8_t scancode = inb(0x60);

    /* Only handle key press (ignore break codes for now) */
    if (scancode < 128) {
        char c = scancode_to_ascii[scancode];
        if (c) {
            char buf[2] = {c, 0};
            console_write(buf);
        }
    }
}

void keyboard_init(void) {
    pic_clear_mask(1);                /* Enable IRQ1 (keyboard) */
}
