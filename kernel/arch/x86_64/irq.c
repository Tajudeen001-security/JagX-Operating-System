#include "idt.h"
#include "pic.h"
#include "timer.h"
#include "keyboard.h"
#include "mouse.h"
#include "console.h"

void irq_handler(struct registers* r) {
    uint8_t irq = r->int_no;

    switch (irq) {
        case 0:  timer_handler();    break;
        case 1:  keyboard_handler(); break;
        case 12: mouse_handler();    break;
        default: break;
    }

    pic_send_eoi(irq);
}
