#include "idt.h"
#include "pic.h"
#include "timer.h"
#include "keyboard.h"
#include "console.h"

/* Main IRQ dispatcher called from assembly stubs */
void irq_handler(struct registers* r) {
    /* The IRQ number is in r->int_no (0-15 after our stub) */
    uint8_t irq = r->int_no;

    switch (irq) {
        case 0:
            timer_handler();
            break;
        case 1:
            keyboard_handler();
            break;
        default:
            /* Other IRQs ignored for now */
            break;
    }

    /* Send End-Of-Interrupt to PIC */
    pic_send_eoi(irq);
}
