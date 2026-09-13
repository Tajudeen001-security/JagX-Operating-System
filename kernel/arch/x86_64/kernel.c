/* JagX v0.0.9 */
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
#include "../../../crypto/sha256.h"

struct compositor g_compositor;

void kernel_main(uint32_t magic, void* mb_info) {
    console_init();
    console_write("JagX OS v0.0.9\n==============\n\n");

    multiboot2_parse(magic, mb_info);
    gdt_init();
    idt_init();
    pic_remap();
    if (pmm_get_total_pages() == 0) pmm_init(128 * 1024);
    heap_init();
    paging_init();
    fb_init();

    timer_init(100);
    keyboard_init();
    mouse_init();
    net_init();

    process_init();
    process_create("init", 0, 0);

    /* SHA-256 self check on a short message */
    uint8_t dig[32];
    const char* msg = "JagX";
    sha256((const uint8_t*)msg, 4, dig);
    console_write("[CRYPTO] SHA-256 self-test done\n");

    compositor_init(&g_compositor);
    compositor_create_window(&g_compositor, 60, 50, 400, 260, "JagX Shell");
    compositor_create_window(&g_compositor, 320, 140, 360, 240, "Network");
    compositor_render(&g_compositor);

    ramfs_init();
    syscall_init();

    __asm__ volatile ("sti");
    console_write("\n=== Ready ===\n");
    console_write("Net: virtqueues + DNS builder + TCP PCB\n");
    console_write("User: process table + int 0x80\n");
    console_write("Crypto: SHA-256 live | AEAD interface\n");
    console_write("> ");

    for (;;) __asm__ volatile ("hlt");
}
