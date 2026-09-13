/* JagX v0.0.21 — bilingual, drivers, full app suite */
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
#include "../../../apps/terminal.h"
#include "../../../apps/calc.h"
#include "../../../apps/calendar.h"
#include "../../../apps/contacts.h"
#include "../../../apps/ide.h"
#include "../../../apps/forms.h"
#include "../../../i18n/lang.h"
#include "../../../drivers/driver.h"

extern void drivers_register_builtins(void);
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
    console_write("JagX OS v0.0.21\n===============\n\n");
    if (boot_verify_marker() != 0) for (;;) __asm__ volatile ("hlt");

    multiboot2_parse(magic, mb_info);
    gdt_init(); idt_init(); pic_remap();
    if (pmm_get_total_pages() == 0) pmm_init(128 * 1024);
    heap_init(); paging_init(); fb_init();
    timer_init(100); keyboard_init(); mouse_init();

    lang_init();
    drivers_init();
    drivers_register_builtins();
    drivers_probe_all();
    drivers_list_console();

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
    terminal_init();
    calc_init();
    calendar_init();
    contacts_init();
    ide_init();
    forms_init();

    console_write(L("welcome"));
    console_write("\n");
    notifications_push("JagX", L("welcome"));
    notifications_push("Apps", "Office Dev Gov User suite");

    process_init();
    uint32_t ustack = (uint32_t)(user_stack + sizeof(user_stack));
    process_create("userdemo", (uint32_t)user_program, ustack);
    tss_set_stack((uint32_t)&user_stack[0] + 0x10000);

    compositor_init(&g_compositor);
    compositor_create_window(&g_compositor, 20, 32, 260, 150, "Notepad");
    compositor_create_window(&g_compositor, 300, 32, 240, 150, "Paint");
    compositor_create_window(&g_compositor, 560, 32, 220, 150, "Terminal");
    compositor_create_window(&g_compositor, 20, 200, 280, 160, "JagSheet");
    compositor_create_window(&g_compositor, 320, 200, 240, 160, "JagBase");
    compositor_create_window(&g_compositor, 580, 200, 200, 160, "Forms");
    compositor_render(&g_compositor);

    syscall_init();
    __asm__ volatile ("sti");
    console_write("v0.0.21: i18n + drivers + full app set\n");
    enter_user_mode((uint32_t)user_program, ustack);
    for (;;) __asm__ volatile ("hlt");
}
