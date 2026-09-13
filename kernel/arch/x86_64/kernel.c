/* JagX v0.0.22 — real .jagx packages + Noder */
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
#include "../../../mobile/notifications.h"
#include "../../../mobile/statusbar.h"
#include "../../../mobile/lockscreen.h"
#include "../../../apps/office.h"
#include "../../../apps/noder.h"
#include "../../../apps/terminal.h"
#include "../../../pkg/jagxpkg.h"
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
    console_write("JagX OS v0.0.22\n===============\n\n");
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

    net_init();
    ramfs_init();
    pkg_init();
    gallery_init();
    notifications_init();
    control_center_init();
    settings_init();
    statusbar_init();
    lockscreen_init();
    office_init();
    terminal_init();
    noder_init();

    /* Real package install: Noder as .jagx */
    if (noder_package_install() == 0)
        console_write("[PKG] Noder.jagx installed\n");

    console_write("[PKG] Installed packages: ");
    console_write_dec((uint32_t)pkg_list_count());
    console_write("\n");
    for (int i = 0; i < pkg_list_count(); i++) {
        const struct jagx_pkg_info* p = pkg_list_at(i);
        if (p) { console_write("  - "); console_write(p->name); console_write("\n"); }
    }

    notifications_push("Noder", "JagX IDE packaged as .jagx");

    process_init();
    uint32_t ustack = (uint32_t)(user_stack + sizeof(user_stack));
    process_create("userdemo", (uint32_t)user_program, ustack);
    tss_set_stack((uint32_t)&user_stack[0] + 0x10000);

    compositor_init(&g_compositor);
    compositor_create_window(&g_compositor, 40, 40, 500, 320, "Noder");
    compositor_create_window(&g_compositor, 560, 40, 220, 160, "Terminal");
    compositor_render(&g_compositor);
    if (fb_is_ready()) noder_draw(48, 72, 480, 280);

    syscall_init();
    __asm__ volatile ("sti");
    console_write("Package format: .jagx (not APK/IPA)\n");
    enter_user_mode((uint32_t)user_program, ustack);
    for (;;) __asm__ volatile ("hlt");
}
