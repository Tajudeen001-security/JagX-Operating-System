#ifndef JAGX_PHONE_H
#define JAGX_PHONE_H

void phone_init(void);
int  phone_contact_add(const char* name, const char* number);
int  phone_contact_count(void);
const char* phone_contact_name(int i);
const char* phone_contact_number(int i);
void phone_dial_digit(char d);
void phone_dial_clear(void);
const char* phone_dial_buffer(void);
int  phone_call(const char* number);
int  phone_call_contact(int index);
void phone_hangup(void);
int  phone_is_in_call(void);
void phone_draw_contacts(int x, int y, int w, int h);
void phone_draw_dialer(int x, int y, int w, int h);
void phone_draw(int x, int y, int w, int h); /* contacts + dialer stacked */
void phone_list_console(void);

#endif
