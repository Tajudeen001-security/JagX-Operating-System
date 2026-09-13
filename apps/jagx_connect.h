#ifndef JAGX_CONNECT_H
#define JAGX_CONNECT_H

/* Native JagX Connect for JagX OS. Package: com.jagx.connect (.jagx only). */

void jagx_connect_init(void);
void jagx_connect_draw(int x, int y, int w, int h);
int  jagx_connect_post(const char* author, const char* body);
int  jagx_connect_count(void);

#endif
