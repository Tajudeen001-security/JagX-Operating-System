/* ============================================================
 * JagX Kernel - Main entry point (v0.0.2)
 * ============================================================
 * New in this version:
 *   1. Improved keyboard (Shift + Caps Lock)
 *   2. Physical Memory Manager (bitmap)
 *   3. Simple kernel heap (kmalloc)
 *   4. Basic paging (identity map)
 *   5. RamFS (in-memory filesystem)
 *   6. Syscall dispatcher stub
 * ============================================================
 */

#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "timer.h"
#include "keyboard.h"
#include "console.h"

#include "../../mm/pmm.h"
#include "../../mm/heap.h"
#include "../../mm/paging.h"
#include "../../fs/ramfs.h"
#include "../../syscall/syscall.h"

void kernel_main(void) {
    console_init();
    console_write("JagX Operating System v0.0.2\n");
    console_write("=============================\n\n");

    console_write("[*] GDT...\n");
    gdt_init();

    console_write("[*] IDT...\n");
    idt_init();

    console_write("[*] PIC...\n");
    pic_remap();

    /* Memory management */
    pmm_init(128 * 1024);          /* Assume 128 MB for QEMU default */
    heap_init();
    paging_init();

    /* Higher level services */
    ramfs_init();
    syscall_init();

    console_write("[*] Timer (100 Hz)...\n");
    timer_init(100);

    console_write("[*] Keyboard (Shift + Caps Lock supported)...\n");
    keyboard_init();

    /* Enable interrupts */
    __asm__ volatile ("sti");

    console_write("\n=== System Ready ===\n");
    console_write("Type on the keyboard. Timer dots appear every second.\n");
    console_write("RamFS demo files available.\n\n");

    /* Show RamFS contents */
    ramfs_list();

    console_write("\n> ");

    /* Demo a syscall */
    syscall_handler(SYS_WRITE, (uint32_t)"[DEMO] Syscall write works!\n", 0, 0);

    /* Main loop */
    for (;;) {
        __asm__ volatile ("hlt");
    }
}
