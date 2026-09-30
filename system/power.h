#ifndef JAGX_POWER_H
#define JAGX_POWER_H

enum power_state {
    POWER_OFF = 0,
    POWER_BOOTING,
    POWER_ON,
    POWER_SHUTTING_DOWN,
    POWER_RESTARTING,
    POWER_SLEEP
};

void power_init(void);
enum power_state power_get(void);
void power_request_shutdown(void);
void power_request_restart(void);
void power_request_sleep(void);
void power_boot_complete(void);
void power_tick(void); /* advance animated transitions */
void power_draw_screen(void); /* boot / shutdown / restart UI */
int  power_blocks_ui(void); /* 1 while boot/shutdown animation */

#endif
