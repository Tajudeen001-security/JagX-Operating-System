/* JagX v0.0.20 — Office suite for gov demos */
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
#include "../../../mobile/statusbar.h"
#include "../../../mobile/lockscreen.h"
#include "../../../apps/office.h"
#include "../../../apps/sheet.h"
#include "../../../apps/database.h"

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
    console_write("JagX OS v0.0.20\n===============\n\n");
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
    statusbar_init();
    lockscreen_init();
    office_init();

    notifications_push("Office", "Notepad Paint Sheet Base ready");
    notifications_push("Gov pilot", "See docs/GOVERNMENT.md");

    console_write("[SHEET] Budget col sum=");
    console_write_dec((uint32_t)sheet_sum_column(1));
    console_write("\n");
    db_list_console();

    process_init();
    uint32_t ustack = (uint32_t)(user_stack + sizeof(user_stack));
    process_create("userdemo", (uint32_t)user_program, ustack);
    tss_set_stack((uint32_t)&user_stack[0] + 0x10000);

    compositor_init(&g_compositor);
    compositor_create_window(&g_compositor, 30, 36, 300, 180, "Notepad");
    compositor_create_window(&g_compositor, 350, 36, 280, 180, "Paint");
    compositor_create_window(&g_compositor, 30, 230, 320, 200, "JagSheet");
    compositor_create_window(&g_compositor, 370, 230, 320, 200, "JagBase");
    compositor_render(&g_compositor);

    if (fb_is_ready() && !lockscreen_is_locked())
        office_draw_all();

    syscall_init();
    __asm__ volatile ("sti");
    console_write("Productivity suite live — gov demo path open\n");
    enter_user_mode((uint32_t)user_program, ustack);
    for (;;) __asm__ volatile ("hlt");
}
