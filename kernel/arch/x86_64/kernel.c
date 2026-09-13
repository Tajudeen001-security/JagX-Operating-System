/* JagX v0.1.0 — Independence: native phone, SMS, social, store, launcher */
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
    console_write("JagX OS v0.1.0 Independence\n===========================\n\n");
    if (boot_verify_marker() != 0) for (;;) __asm__ volatile ("hlt");

    multiboot2_parse(magic, mb_info);
    gdt_init(); idt_init(); pic_remap();
    if (pmm_get_total_pages() == 0) pmm_init(256 * 1024);
    heap_init(); paging_init(); fb_init();
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
    lockscreen_init();
    launcher_init();
    contacts_init();
    phone_init();
    messages_init();
    social_init();
    browser_app_init();
    store_init();

    /* Unlock so user can type into Noder immediately in lab */
    lockscreen_hide();

    noder_init();
    noder_focus(1);
    noder_package_install();
    pkg_bundle_core_apps();

    noder_save_current();
    console_write("[NODER] Type to edit. Ctrl+S save. Ctrl+1-4 tabs.\n");
    console_write("[APPS] Phone, Messages, JagCircle, JagBrowser, JagStore (.jagx)\n");

    notifications_push("JagX", "Independence — native apps, no APK");

    process_init();
    uint32_t ustack = (uint32_t)(user_stack + sizeof(user_stack));
    process_create("userdemo", (uint32_t)user_program, ustack);
    tss_set_stack((uint32_t)&user_stack[0] + 0x10000);

    compositor_init(&g_compositor);
    compositor_create_window(&g_compositor, 20, 30, 700, 400, "Noder — JagX IDE");
    compositor_create_window(&g_compositor, 40, 80, 280, 360, "Phone");
    compositor_render(&g_compositor);
    noder_draw(28, 62, 680, 360);
    phone_draw(48, 112, 260, 320);

    syscall_init();
    __asm__ volatile ("sti");
    console_write("JagX v0.1.0 ready — .jagx only, never APK\n");
    enter_user_mode((uint32_t)user_program, ustack);
    for (;;) __asm__ volatile ("hlt");
}
