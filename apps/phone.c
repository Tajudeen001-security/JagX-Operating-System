#include "phone.h"
#include "../mobile/hal/radio.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

#define MAX_CONTACTS 64
#define NAME_LEN 40
#define NUM_LEN  24

static char names[MAX_CONTACTS][NAME_LEN];
static char numbers[MAX_CONTACTS][NUM_LEN];
static int n_contacts = 0;
static char dial[NUM_LEN];
static int dial_len = 0;
static int in_call = 0;

void phone_init(void) {
    n_contacts = 0;
    dial_len = 0;
    dial[0] = 0;
    in_call = 0;
    phone_contact_add("Emergency", "112");
    phone_contact_add("Ministry IT", "+2348000000001");
    phone_contact_add("Support", "+2348000000002");
    console_write("[PHONE] Contacts + dialer ready\n");
}

int phone_contact_add(const char* name, const char* number) {
    if (n_contacts >= MAX_CONTACTS) return -1;
    int i = 0;
    if (name) while (name[i] && i < NAME_LEN-1) { names[n_contacts][i] = name[i]; i++; }
    names[n_contacts][i] = 0;
    i = 0;
    if (number) while (number[i] && i < NUM_LEN-1) { numbers[n_contacts][i] = number[i]; i++; }
    numbers[n_contacts][i] = 0;
    n_contacts++;
    return n_contacts - 1;
}

int phone_contact_count(void) { return n_contacts; }
const char* phone_contact_name(int i) {
    if (i < 0 || i >= n_contacts) return "";
    return names[i];
}
const char* phone_contact_number(int i) {
    if (i < 0 || i >= n_contacts) return "";
    return numbers[i];
}

void phone_dial_digit(char d) {
    if (dial_len >= NUM_LEN-1) return;
    if ((d >= '0' && d <= '9') || d == '+' || d == '*' || d == '#') {
        dial[dial_len++] = d;
        dial[dial_len] = 0;
    }
}

void phone_dial_clear(void) {
    if (dial_len > 0) { dial[--dial_len] = 0; }
}

const char* phone_dial_buffer(void) { return dial; }

int phone_call(const char* number) {
    if (!number || !number[0]) return -1;
    in_call = 1;
    console_write("[PHONE] Calling ");
    console_write(number);
    console_write(" (modem HAL on device)\n");
    /* Real call: radio HAL / modem driver */
    return 0;
}

int phone_call_contact(int index) {
    if (index < 0 || index >= n_contacts) return -1;
    return phone_call(numbers[index]);
}

void phone_hangup(void) {
    in_call = 0;
    console_write("[PHONE] Call ended\n");
}

int phone_is_in_call(void) { return in_call; }

void phone_list_console(void) {
    for (int i = 0; i < n_contacts; i++) {
        console_write(names[i]);
        console_write(" — ");
        console_write(numbers[i]);
        console_write("\n");
    }
}

void phone_draw_contacts(int x, int y, int w, int h) {
    if (!fb_is_ready()) return;
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, (uint32_t)h, 0xFF14141E);
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, 36, 0xFF3498DB);
    for (int i = 0; i < n_contacts && i < 8; i++)
        fb_fill_rect((uint32_t)(x+8), (uint32_t)(y+48+i*36), (uint32_t)(w-16), 30, 0xFF1E2A3A);
}

void phone_draw_dialer(int x, int y, int w, int h) {
    if (!fb_is_ready()) return;
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, (uint32_t)h, 0xFF0D1117);
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, 40, 0xFF21262D);
    fb_fill_rect((uint32_t)(x+12), (uint32_t)(y+52), (uint32_t)(w-24), 36, 0xFF1E2A3A);
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 3; c++)
            fb_fill_rect((uint32_t)(x+20+c*((w-50)/3)), (uint32_t)(y+100+r*48),
                         (uint32_t)((w-60)/3), 40, 0xFF2A2A35);
    fb_fill_rect((uint32_t)(x+w/2-40), (uint32_t)(y+h-50), 80, 36,
                 in_call ? 0xFFE74C3C : 0xFF2ECC71);
}
