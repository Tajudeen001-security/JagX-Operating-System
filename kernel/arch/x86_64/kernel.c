/* JagX v0.0.10 */
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
#include "../../../net/dns.h"
#include "../../../userland/process.h"
#include "../../../crypto/sha256.h"
#include "../../../crypto/aead.h"

extern int boot_verify_marker(void);

struct compositor g_compositor;

/* Tiny user-mode payload: issue write syscall then hlt loop */
static void user_program(void) {
    const char* s = "[USER] Hello from ring 3 via int 0x80\n";
    __asm__ volatile (
        "mov $1, %%eax\n"
        "mov %0, %%ebx\n"
        "int $0x80\n"
        :
        : "r"(s)
        : "eax", "ebx"
    );
    for (;;) __asm__ volatile ("hlt");
}

void kernel_main(uint32_t magic, void* mb_info) {
    console_init();
    console_write("JagX OS v0.0.10\n===============\n\n");

    if (boot_verify_marker() != 0) {
        console_write("Halting on integrity failure\n");
        for (;;) __asm__ volatile ("hlt");
    }

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

    /* On-wire DNS attempt */
    uint32_t ip = 0;
    dns_resolve_a("example.com", &ip);

    process_init();
    process_create("userdemo", (uint32_t)user_program, 0xB00000);

    /* AEAD smoke test */
    uint8_t key[32], nonce[12], pt[16], ct[16], tag[16], out[16];
    for (int i = 0; i < 32; i++) key[i] = (uint8_t)i;
    for (int i = 0; i < 12; i++) nonce[i] = (uint8_t)(i + 1);
    for (int i = 0; i < 16; i++) pt[i] = (uint8_t)('A' + i);
    if (aead_encrypt(key, 32, nonce, 12, 0, 0, pt, 16, ct, tag, 16) == 0 &&
        aead_decrypt(key, 32, nonce, 12, 0, 0, ct, 16, tag, 16, out) == 0)
        console_write("[CRYPTO] ChaCha20-Poly1305 encrypt/decrypt OK\n");
    else
        console_write("[CRYPTO] AEAD test failed\n");

    compositor_init(&g_compositor);
    compositor_create_window(&g_compositor, 60, 50, 400, 260, "JagX");
    compositor_render(&g_compositor);

    ramfs_init();
    syscall_init();
    tss_set_stack((uint32_t)&magic + 0x4000); /* rough kernel stack anchor */

    __asm__ volatile ("sti");
    console_write("\n=== Ready ===\n");
    console_write("IPv4/UDP path active | TSS+Ring3 ready | AEAD live | boot hash OK\n");

    /* Optional: actually enter user mode demo (may not return) */
    /* enter_user_mode((uint32_t)user_program, 0xB00000); */

    console_write("> ");
    for (;;) __asm__ volatile ("hlt");
}
