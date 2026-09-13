/* JagX Kernel v0.0.6 - Real drivers focus */

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

static struct compositor comp;

void kernel_main(uint32_t magic, void* mb_info) {
    console_init();
    console_write("JagX OS v0.0.6\n");
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

    // Real input devices
    timer_init(100);
    keyboard_init();
    mouse_init();

    // Networking (loopback + virtio probe)
    net_init();

    // Compositor with real chrome
    compositor_init(&comp);
    compositor_create_window(&comp, 80, 60, 420, 280, "JagX Shell");
    compositor_create_window(&comp, 360, 160, 380, 260, "System");
    compositor_render(&comp);

    // Draw cursor at initial mouse position
    struct mouse_state ms = mouse_get_state();
    compositor_draw_cursor(ms.x + 200, ms.y + 150);

    ramfs_init();
    syscall_init();

    __asm__ volatile ("sti");

    console_write("\n=== JagX Ready ===\n");
    if (fb_is_ready())
        console_write("Framebuffer active - windows + cursor drawn\n");
    else
        console_write("Text mode (use ISO/GRUB for reliable FB)\n");

    console_write("PS/2 mouse live | Virtio-net probed | MMU path on aarch64\n");
    console_write("> ");

    for (;;) {
        __asm__ volatile ("hlt");
        // Future: redraw cursor on mouse movement
    }
}
