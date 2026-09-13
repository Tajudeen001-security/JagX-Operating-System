/* Real PS/2 mouse + live cursor support */
#include "mouse.h"
#include "io.h"
#include "pic.h"
#include "console.h"
#include "../../../compositor/compositor.h"

static struct mouse_state mouse = {200, 150, 0};
static uint8_t mouse_cycle = 0;
static int8_t  mouse_bytes[3];
static int cursor_visible = 1;

extern struct compositor g_compositor;   // defined in kernel.c

static void mouse_wait(uint8_t type) {
    uint32_t timeout = 100000;
    if (type == 0) {
        while (timeout-- && (inb(0x64) & 1) == 0);
    } else {
        while (timeout-- && (inb(0x64) & 2) != 0);
    }
}

static void mouse_write(uint8_t val) {
    mouse_wait(1);
    outb(0x64, 0xD4);
    mouse_wait(1);
    outb(0x60, val);
}

static uint8_t mouse_read(void) {
    mouse_wait(0);
    return inb(0x60);
}

void mouse_handler(void) {
    uint8_t data = inb(0x60);

    switch (mouse_cycle) {
        case 0:
            if ((data & 0x08) == 0) break;
            mouse_bytes[0] = data;
            mouse_cycle = 1;
            break;
        case 1:
            mouse_bytes[1] = data;
            mouse_cycle = 2;
            break;
        case 2: {
            mouse_bytes[2] = data;
            mouse.buttons = mouse_bytes[0] & 0x07;

            int32_t dx = mouse_bytes[1];
            int32_t dy = -mouse_bytes[2];

            mouse.x += dx;
            mouse.y += dy;
            if (mouse.x < 0) mouse.x = 0;
            if (mouse.y < 0) mouse.y = 0;
            if (mouse.x > 1000) mouse.x = 1000;
            if (mouse.y > 700) mouse.y = 700;

            // Live cursor redraw + simple window drag
            if (cursor_visible) {
                compositor_handle_mouse(&g_compositor, mouse.x, mouse.y, mouse.buttons);
            }

            mouse_cycle = 0;
            break;
        }
    }
}

void mouse_init(void) {
    mouse_wait(1);
    outb(0x64, 0xA8);

    mouse_wait(1);
    outb(0x64, 0x20);
    mouse_wait(0);
    uint8_t status = inb(0x60) | 2;
    mouse_wait(1);
    outb(0x64, 0x60);
    mouse_wait(1);
    outb(0x60, status);

    mouse_write(0xF6);
    mouse_read();
    mouse_write(0xF4);
    mouse_read();

    pic_clear_mask(12);
    console_write("[MOUSE] PS/2 mouse live (cursor + drag enabled)\n");
}

struct mouse_state mouse_get_state(void) {
    return mouse;
}
