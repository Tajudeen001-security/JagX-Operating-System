#include "keyboard.h"
#include "io.h"
#include "pic.h"
#include "console.h"
#include "../../../apps/noder.h"

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
static int ctrl_pressed = 0;

void keyboard_handler(void) {
    uint8_t scancode = inb(0x60);

    if (scancode & 0x80) {
        scancode &= 0x7F;
        if (scancode == 0x2A || scancode == 0x36) shift_pressed = 0;
        if (scancode == 0x1D) ctrl_pressed = 0;
        return;
    }

    if (scancode == 0x2A || scancode == 0x36) { shift_pressed = 1; return; }
    if (scancode == 0x1D) { ctrl_pressed = 1; return; }
    if (scancode == 0x3A) { caps_lock = !caps_lock; return; }

    /* Ctrl+S save in Noder */
    if (ctrl_pressed && (scancode == 0x1F)) { /* S */
        if (noder_is_focused()) noder_save_current();
        return;
    }
    /* Ctrl+1..4 switch tab */
    if (ctrl_pressed && scancode >= 0x02 && scancode <= 0x05) {
        noder_switch_tab(scancode - 0x02);
        return;
    }

    char c = 0;
    if (scancode < 128) {
        c = shift_pressed ? scancode_ascii_shift[scancode] : scancode_ascii[scancode];
        if (caps_lock && c >= 'a' && c <= 'z') c = (char)(c - 'a' + 'A');
        else if (caps_lock && c >= 'A' && c <= 'Z' && !shift_pressed)
            c = (char)(c - 'A' + 'a');
    }

    if (c) {
        if (noder_is_focused()) {
            noder_on_key(c);
        } else {
            char buf[2] = {c, 0};
            console_write(buf);
        }
    }
}

void keyboard_init(void) {
    while (inb(0x64) & 1) inb(0x60);
    pic_clear_mask(1);
}
