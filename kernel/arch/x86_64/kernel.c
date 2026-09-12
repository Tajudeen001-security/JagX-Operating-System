/* JagX Kernel – Minimal entry point
 *
 * This is intentionally extremely simple.
 * Real memory management, interrupts, drivers, etc. come later.
 */

void kernel_main(void) {
    /* VGA text mode buffer */
    volatile char* video = (volatile char*)0xB8000;

    const char* msg = "Welcome to JagX Operating System!";
    int i = 0;

    /* Clear screen (black background) */
    for (int j = 0; j < 80 * 25 * 2; j += 2) {
        video[j] = ' ';
        video[j + 1] = 0x07;   /* light grey on black */
    }

    /* Print message */
    while (msg[i]) {
        video[i * 2] = msg[i];
        video[i * 2 + 1] = 0x0A;  /* light green */
        i++;
    }

    /* Infinite loop */
    for (;;);
}
