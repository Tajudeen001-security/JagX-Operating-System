/* ============================================================
 * JagX Kernel v0.0.4
 * ============================================================
 */

#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "timer.h"
#include "keyboard.h"
#include "console.h"
#include "framebuffer.h"
#include "multiboot.h"

#include "../../mm/pmm.h"
#include "../../mm/heap.h"
#include "../../mm/paging.h"
#include "../../fs/ramfs.h"
#include "../../syscall/syscall.h"

void kernel_main(uint32_t magic, struct multiboot_info* mb_info) {
    console_init();
    console_write("JagX Operating System v0.0.4\n");
    console_write("=============================\n\n");

    /* Multiboot + memory */
    multiboot_parse(magic, mb_info);

    console_write("[*] GDT...\n");
    gdt_init();

    console_write("[*] IDT...\n");
    idt_init();

    console_write("[*] PIC...\n");
    pic_remap();

    /* Fallback PMM if multiboot did not re-init */
    if (pmm_get_total_pages() == 0) {
        pmm_init(128 * 1024);
    }

    heap_init();
    paging_init();

    fb_init();
    fb_demo_design_colors();

    ramfs_init();
    syscall_init();

    console_write("[*] Timer...\n");
    timer_init(100);

    console_write("[*] Keyboard...\n");
    keyboard_init();

    __asm__ volatile ("sti");

    console_write("\n=== JagX Ready ===\n");
    console_write("Multiboot memory info parsed.\n");
    console_write("Framebuffer layer + design colors ready.\n");
    console_write("Type to test keyboard. Dots = timer.\n\n");

    ramfs_list();
    console_write("\n> ");

    syscall_handler(1, (uint32_t)"[DEMO] All core systems online\n", 0, 0);

    for (;;) {
        __asm__ volatile ("hlt");
    }
}
