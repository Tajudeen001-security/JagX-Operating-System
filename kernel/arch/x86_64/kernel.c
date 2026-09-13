/* JagX Kernel v0.0.5 */

#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "timer.h"
#include "keyboard.h"
#include "console.h"
#include "framebuffer.h"
#include "multiboot.h"

#include "../../mm/pmm.h"
#include "../../mm/heap.h"
#include "../../mm/paging.h"
#include "../../fs/ramfs.h"
#include "../../syscall/syscall.h"
#include "../../../compositor/compositor.h"

static struct compositor comp;

void kernel_main(uint32_t magic, void* mb_info) {
    console_init();
    console_write("JagX OS v0.0.5\n");
    console_write("==============\n\n");

    multiboot2_parse(magic, mb_info);

    gdt_init();
    idt_init();
    pic_remap();

    if (pmm_get_total_pages() == 0)
        pmm_init(128 * 1024);

    heap_init();
    paging_init();

    fb_init();
    /* Try to draw if Multiboot2 gave us a framebuffer */
    fb_draw_demo();

    /* Compositor demo */
    compositor_init(&comp);
    int w1 = compositor_create_window(&comp, 100, 100, 320, 200, "Welcome");
    int w2 = compositor_create_window(&comp, 280, 180, 360, 240, "JagX Desktop");
    (void)w1; (void)w2;
    compositor_render(&comp);

    ramfs_init();
    syscall_init();

    timer_init(100);
    keyboard_init();
    __asm__ volatile ("sti");

    console_write("\n=== System Ready ===\n");
    if (fb_is_ready())
        console_write("Graphical framebuffer is active.\n");
    else
        console_write("Running in text mode (FB not provided by loader).\n");

    console_write("Compositor windows created.\n");
    console_write("Type on keyboard. Timer dots appear.\n\n> ");

    for (;;) {
        __asm__ volatile ("hlt");
    }
}
