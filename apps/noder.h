#ifndef JAGX_NODER_H
#define JAGX_NODER_H

/* Noder for JagX OS — native code editor so people can code on JagX
 * (Desktop Electron Noder remains for Windows: JagX-JRILICENSE/Noder)
 */

#include <stdint.h>

#define NODER_MAX_TABS  4
#define NODER_BUF_SIZE  8192
#define NODER_NAME_LEN  48

void noder_init(void);
void noder_focus(int on);              /* 1 = keyboard goes to editor */
int  noder_is_focused(void);
void noder_on_key(char c);             /* from keyboard IRQ */

int  noder_new_file(const char* name);
int  noder_open_file(const char* path);
int  noder_save_current(void);
int  noder_switch_tab(int index);
int  noder_tab_count(void);
const char* noder_tab_name(int index);
const char* noder_current_text(void);
uint32_t noder_current_len(void);

void noder_draw(int x, int y, int w, int h);
int  noder_package_install(void);

#endif
