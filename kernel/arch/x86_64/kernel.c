/* JagX Kernel v0.0.7 */

#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "timer.h"
#include "keyboard.h"
#include "mouse.h"
#include "console.h"
#include "framebuffer.h"
#include "multiboot.h"

#include "../../mm/pmm.h"
#include "../../mm/heap.h"
#include "../../mm/paging.h"
#include "../../fs/ramfs.h"
#include "../../syscall/syscall.h"
#include "../../../compositor/compositor.h"
#include "../../../net/net.h"

struct compositor g_compositor;

void kernel_main(uint32_t magic, void* mb_info) {
    console_init();
    console_write("JagX OS v0.0.7\n");
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

    timer_init(100);
    keyboard_init();
    mouse_init();

    net_init();

    compositor_init(&g_compositor);
    compositor_create_window(&g_compositor, 60, 50, 400, 260, "JagX Shell");
    compositor_create_window(&g_compositor, 320, 140, 360, 240, "Security");
    compositor_render(&g_compositor);

    ramfs_init();
    syscall_init();

    __asm__ volatile ("sti");

    console_write("\n=== JagX Ready ===\n");
    console_write("Mouse: live cursor + window dragging\n");
    console_write("Virtio-net: probed + basic init\n");
    console_write("Security foundations active\n");
    if (fb_is_ready())
        console_write("Framebuffer active\n");
    console_write("> ");

    for (;;) {
        __asm__ volatile ("hlt");
    }
}
