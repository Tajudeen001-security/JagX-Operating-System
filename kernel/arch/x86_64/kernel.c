/* JagX v0.0.18 */
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
#include "../../../userland/process.h"
#include "../../../mobile/control_center.h"
#include "../../../mobile/settings.h"
#include "../../../mobile/gallery.h"
#include "../../../mobile/screenshot.h"
#include "../../../mobile/notifications.h"
#include "../../../mobile/filemanager.h"
#include "../../../mobile/features.h"

extern int boot_verify_marker(void);
struct compositor g_compositor;
static uint8_t user_stack[8192] __attribute__((aligned(16)));

static void user_program(void) {
    const char* s = "[USER] ring3\n";
    __asm__ volatile ("mov $1,%%eax; mov %0,%%ebx; int $0x80" : : "r"(s) : "eax","ebx");
    for (;;) __asm__ volatile ("hlt");
}

void kernel_main(uint32_t magic, void* mb_info) {
    console_init();
    console_write("JagX OS v0.0.18\n===============\n\n");
    if (boot_verify_marker() != 0) for (;;) __asm__ volatile ("hlt");

    multiboot2_parse(magic, mb_info);
    gdt_init(); idt_init(); pic_remap();
    if (pmm_get_total_pages() == 0) pmm_init(128 * 1024);
    heap_init(); paging_init(); fb_init();
    timer_init(100); keyboard_init(); mouse_init();
    net_init();

    ramfs_init();
    gallery_init();
    notifications_init();
    filemanager_init();
    features_init();
    control_center_init();
    settings_init();

    notifications_push("Welcome", "JagX is running");
    notifications_push("Gallery", "Screenshots save here");

    screenshot_set_triple_tap_enabled(1);
    gallery_add(JAGX_MEDIA_PHOTO, "camera-demo.jpg", 1280, 720, 200000);
    screenshot_take();
    gallery_list_console();
    filemanager_list_root();

    process_init();
    uint32_t ustack = (uint32_t)(user_stack + sizeof(user_stack));
    process_create("userdemo", (uint32_t)user_program, ustack);
    tss_set_stack((uint32_t)&user_stack[0] + 0x10000);

    compositor_init(&g_compositor);
    compositor_create_window(&g_compositor, 40, 40, 320, 220, "Gallery");
    compositor_create_window(&g_compositor, 280, 80, 320, 220, "Files");
    compositor_create_window(&g_compositor, 160, 200, 320, 200, "Settings");
    compositor_toggle_control_center(&g_compositor);
    compositor_toggle_notification_shade(&g_compositor);
    compositor_render(&g_compositor);

    syscall_init();
    __asm__ volatile ("sti");
    console_write("Notif shade + Files + FB screenshot dump live\n");
    enter_user_mode((uint32_t)user_program, ustack);
    for (;;) __asm__ volatile ("hlt");
}
