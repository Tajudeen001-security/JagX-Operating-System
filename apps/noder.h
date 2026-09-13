#ifndef JAGX_NODER_H
#define JAGX_NODER_H

/* Noder — JagX & JRILICENSE code editor (VS Code–class product on JagX)
 * Native .jagx app — NOT an Android APK.
 */

void noder_init(void);
void noder_open_buffer(const char* title);
void noder_insert(char c);
void noder_backspace(void);
const char* noder_buffer(void);
void noder_draw(int x, int y, int w, int h);
int  noder_save(const char* path);
int  noder_package_install(void); /* build+install noder.jagx into package mgr */

#endif
