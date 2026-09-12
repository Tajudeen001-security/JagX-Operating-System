#include "timer.h"
#include "io.h"
#include "pic.h"
#include "console.h"

static volatile uint32_t timer_ticks = 0;

void timer_handler(void) {
    timer_ticks++;

    /* Print a tick every second (assuming 100 Hz) */
    if (timer_ticks % 100 == 0) {
        console_write(".");
    }
}

uint32_t timer_get_ticks(void) {
    return timer_ticks;
}

void timer_init(uint32_t frequency) {
    /* PIT runs at 1193180 Hz */
    uint32_t divisor = 1193180 / frequency;

    outb(0x43, 0x36);                 /* Command: channel 0, lobyte/hibyte, rate generator */
    outb(0x40, divisor & 0xFF);       /* Low byte */
    outb(0x40, (divisor >> 8) & 0xFF);/* High byte */

    pic_clear_mask(0);                /* Enable IRQ0 (timer) */
}
