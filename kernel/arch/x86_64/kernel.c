/* JagX v0.1.2 — PIN keypad clicks, power menu, Messages+Gallery polish */
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
#include "../../../net/wifi_hotspot.h"
#include "../../../userland/process.h"
#include "../../../mobile/control_center.h"
#include "../../../mobile/notifications.h"
#include "../../../mobile/statusbar.h"
#include "../../../mobile/lockscreen.h"
#include "../../../mobile/launcher.h"
#include "../../../mobile/settings.h"
#include "../../../mobile/gallery.h"
#include "../../../apps/noder.h"
#include "../../../apps/phone.h"
#include "../../../apps/messages.h"
#include "../../../apps/social.h"
#include "../../../apps/browser_app.h"
#include "../../../apps/store.h"
#include "../../../apps/contacts.h"
#include "../../../pkg/jagxpkg.h"
#include "../../../i18n/lang.h"
#include "../../../drivers/driver.h"
#include "../../../auth/auth.h"
#include "../../../system/power.h"
#include "../../../system/power_menu.h"

extern void drivers_register_builtins(void);
extern void pkg_bundle_core_apps(void);
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
    console_write("JagX OS v0.1.2\n==============\n\n");
    if (boot_verify_marker() != 0) for (;;) __asm__ volatile ("hlt");

    multiboot2_parse(magic, mb_info);
    gdt_init(); idt_init(); pic_remap();
    if (pmm_get_total_pages() == 0) pmm_init(256 * 1024);
    heap_init(); paging_init(); fb_init();

    power_init();
    power_menu_init();
    auth_init();
    auth_enable_fingerprint(1);
    auth_enable_face(1);

    timer_init(100); keyboard_init(); mouse_init();

    lang_init();
    drivers_init();
    drivers_register_builtins();
    drivers_probe_all();
    net_init();
    wifi_stack_init();
    ramfs_init();
    pkg_init();
    notifications_init();
    control_center_init();
    statusbar_init();
    settings_init();
    gallery_init();
    lockscreen_init();
    launcher_init();
    contacts_init();
    phone_init();
    messages_init();
    social_init();
    browser_app_init();
    store_init();

    if (fb_is_ready()) {
        for (int i = 0; i < 45; i++) { power_tick(); power_draw_screen(); }
        power_boot_complete();
    } else power_boot_complete();

    lockscreen_show();
    console_write("[AUTH] Click PIN keys or type 1234+Enter | F1/F2 bio\n");
    console_write("[POWER] Ctrl+Shift+Q menu | Control Center power tile\n");

    noder_init();
    noder_package_install();
    pkg_bundle_core_apps();

    notifications_push("JagX", "Click PIN keypad to unlock");

    process_init();
    uint32_t ustack = (uint32_t)(user_stack + sizeof(user_stack));
    process_create("userdemo", (uint32_t)user_program, ustack);
    tss_set_stack((uint32_t)&user_stack[0] + 0x10000);

    compositor_init(&g_compositor);
    compositor_create_window(&g_compositor, 16, 28, 400, 300, "Noder");
    compositor_create_window(&g_compositor, 430, 28, 200, 300, "Phone");
    compositor_create_window(&g_compositor, 16, 340, 280, 200, "Messages");
    compositor_create_window(&g_compositor, 310, 340, 320, 200, "Gallery");

    if (fb_is_ready()) {
        if (lockscreen_is_locked()) lockscreen_draw();
        else {
            compositor_render(&g_compositor);
            noder_draw(24, 60, 380, 260);
            phone_draw(438, 60, 180, 260);
            messages_draw(24, 372, 260, 160);
            gallery_draw(318, 372, 300, 160);
        }
    }

    syscall_init();
    __asm__ volatile ("sti");
    console_write("v0.1.2 ready — PIN clicks, power menu, Messages, Gallery\n");

    for (;;) {
        if (power_blocks_ui()) {
            power_tick();
            power_draw_screen();
        }
        __asm__ volatile ("hlt");
    }
}
