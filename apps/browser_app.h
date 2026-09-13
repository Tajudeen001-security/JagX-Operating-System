#ifndef JAGX_BROWSER_APP_H
#define JAGX_BROWSER_APP_H

void browser_app_init(void);
void browser_app_go(const char* input);
void browser_app_draw(int x, int y, int w, int h);
const char* browser_app_title(void);
const char* browser_app_url(void);

#endif
