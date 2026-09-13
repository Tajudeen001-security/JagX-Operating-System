#ifndef JAGX_CONTACTS_H
#define JAGX_CONTACTS_H
void contacts_init(void);
int  contacts_add(const char* name, const char* phone);
void contacts_list_console(void);
void contacts_draw(int x, int y);
#endif
