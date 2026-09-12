#include "keyboard.h"
#include "io.h"
#include "pic.h"
#include "console.h"

/* Scancode set 1 - make codes */
static const char scancode_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,  'a','s','d','f','g','h','j','k','l',';','\'', '`',
    0,  '\\','z','x','c','v','b','n','m',',','.','/', 0,
    '*', 0,  ' '
};

static const char scancode_ascii_shift[128] = {
    0,  27, '!','@','#','$','%','^','&','*','(',')','_','+', '\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0,  'A','S','D','F','G','H','J','K','L',':','\"','~',
    0,  '|','Z','X','C','V','B','N','M','<','>','?', 0,
    '*', 0,  ' '
};

static int shift_pressed = 0;
static int caps_lock = 0;

void keyboard_handler(void) {
    uint8_t scancode = inb(0x60);

    /* Key release (break code) */
    if (scancode & 0x80) {
        scancode &= 0x7F;
        if (scancode == 0x2A || scancode == 0x36) {  /* Left/Right Shift */
            shift_pressed = 0;
        }
        return;
    }

    /* Key press */
    if (scancode == 0x2A || scancode == 0x36) {      /* Shift */
        shift_pressed = 1;
        return;
    }
    if (scancode == 0x3A) {                          /* Caps Lock */
        caps_lock = !caps_lock;
        return;
    }

    char c = 0;
    if (scancode < 128) {
        if (shift_pressed) {
            c = scancode_ascii_shift[scancode];
        } else {
            c = scancode_ascii[scancode];
        }

        /* Apply Caps Lock to letters */
        if (caps_lock && c >= 'a' && c <= 'z') {
            c = c - 'a' + 'A';
        } else if (caps_lock && c >= 'A' && c <= 'Z' && !shift_pressed) {
            c = c - 'A' + 'a';
        }
    }

    if (c) {
        char buf[2] = {c, 0};
        console_write(buf);
    }
}

void keyboard_init(void) {
    /* Clear any pending data */
    while (inb(0x64) & 1) {
        inb(0x60);
    }
    pic_clear_mask(1);  /* Enable IRQ1 */
}
