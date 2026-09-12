/* ============================================================
 * JagX Kernel - Main entry point
 * ============================================================
 * Features in this starter:
 *   - GDT setup
 *   - IDT + exception handlers
 *   - PIC remapping
 *   - PIT timer (IRQ0)
 *   - PS/2 keyboard (IRQ1)
 *   - VGA text console with scrolling
 * ============================================================
 */

#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "timer.h"
#include "keyboard.h"
#include "console.h"

void kernel_main(void) {
    /* Initialize core subsystems */
    console_init();
    console_write("JagX Operating System\n");
    console_write("=====================\n\n");

    console_write("[*] Setting up GDT...\n");
    gdt_init();

    console_write("[*] Setting up IDT...\n");
    idt_init();

    console_write("[*] Remapping PIC...\n");
    pic_remap();

    console_write("[*] Initializing timer (100 Hz)...\n");
    timer_init(100);

    console_write("[*] Initializing keyboard...\n");
    keyboard_init();

    /* Enable interrupts */
    __asm__ volatile ("sti");

    console_write("\nSystem ready. Type on the keyboard.\n");
    console_write("Timer ticks will appear as dots.\n\n");
    console_write("> ");

    /* Main kernel loop - interrupts do the work */
    for (;;) {
        __asm__ volatile ("hlt");  /* Halt until next interrupt */
    }
}
