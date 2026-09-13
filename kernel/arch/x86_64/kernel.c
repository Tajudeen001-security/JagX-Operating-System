/* ============================================================
 * JagX Kernel - Main entry point (v0.0.3)
 * ============================================================
 * Continuing development toward the graphical vision.
 * ============================================================
 */

#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "timer.h"
#include "keyboard.h"
#include "console.h"
#include "framebuffer.h"

#include "../../mm/pmm.h"
#include "../../mm/heap.h"
#include "../../mm/paging.h"
#include "../../fs/ramfs.h"
#include "../../syscall/syscall.h"

void kernel_main(void) {
    console_init();
    console_write("JagX Operating System v0.0.3\n");
    console_write("=============================\n\n");

    console_write("[*] GDT...\n");
    gdt_init();

    console_write("[*] IDT...\n");
    idt_init();

    console_write("[*] PIC...\n");
    pic_remap();

    /* Memory management */
    pmm_init(128 * 1024);
    heap_init();
    paging_init();

    /* Graphics preparation */
    fb_init();

    /* Higher level services */
    ramfs_init();
    syscall_init();

    console_write("[*] Timer (100 Hz)...\n");
    timer_init(100);

    console_write("[*] Keyboard...\n");
    keyboard_init();

    __asm__ volatile ("sti");

    console_write("\n=== JagX Kernel Ready ===\n");
    console_write("Text mode active. Graphical framebuffer layer prepared.\n");
    console_write("Design direction: dark + teal/purple premium UI.\n\n");

    ramfs_list();

    console_write("\n> ");

    /* Demo syscall */
    syscall_handler(1, (uint32_t)"[DEMO] Syscall + design system online\n", 0, 0);

    for (;;) {
        __asm__ volatile ("hlt");
    }
}
