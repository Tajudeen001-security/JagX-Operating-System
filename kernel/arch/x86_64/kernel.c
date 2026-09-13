/* JagX v0.0.12 */
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
#include "../../../net/http.h"
#include "../../../userland/process.h"
#include "../../../crypto/aead.h"

extern int boot_verify_marker(void);
struct compositor g_compositor;
static uint8_t user_stack[8192] __attribute__((aligned(16)));

static void user_program(void) {
    const char* s = "[USER] Hello from ring 3\n";
    __asm__ volatile (
        "mov $1, %%eax\n"
        "mov %0, %%ebx\n"
        "int $0x80\n"
        : : "r"(s) : "eax", "ebx"
    );
    for (;;) __asm__ volatile ("hlt");
}

void kernel_main(uint32_t magic, void* mb_info) {
    console_init();
    console_write("JagX OS v0.0.12\n===============\n\n");

    if (boot_verify_marker() != 0)
        for (;;) __asm__ volatile ("hlt");

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
    uint32_t ustack = (uint32_t)(user_stack + sizeof(user_stack));
    process_create("userdemo", (uint32_t)user_program, ustack);
    tss_set_stack((uint32_t)&user_stack[0] + 0x10000);

    uint8_t key[32], nonce[12], pt[16], ct[16], tag[16], out[16];
    for (int i = 0; i < 32; i++) key[i] = (uint8_t)i;
    for (int i = 0; i < 12; i++) nonce[i] = (uint8_t)(i+1);
    for (int i = 0; i < 16; i++) pt[i] = (uint8_t)('A'+i);
    if (aead_encrypt(key,32,nonce,12,0,0,pt,16,ct,tag,16)==0 &&
        aead_decrypt(key,32,nonce,12,0,0,ct,16,tag,16,out)==0)
        console_write("[CRYPTO] AEAD OK\n");

    uint8_t http_buf[512];
    uint32_t got = 0;
    http_get("example.com", "/", http_buf, sizeof(http_buf), &got);

    compositor_init(&g_compositor);
    compositor_create_window(&g_compositor, 60, 50, 400, 260, "JagX");
    compositor_render(&g_compositor);

    ramfs_init();
    syscall_init();
    __asm__ volatile ("sti");

    console_write("\nSee README for install/setup (PC + Mobile).\n");
    console_write("Entering user mode...\n");
    enter_user_mode((uint32_t)user_program, ustack);

    for (;;) __asm__ volatile ("hlt");
}
