#include "contacts.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

#define MAX_C 16
static char names[MAX_C][32];
static char phones[MAX_C][24];
static int n = 0;

void contacts_init(void) {
    n = 0;
    contacts_add("Ministry IT", "+234-800-000-0001");
    contacts_add("Support Desk", "+234-800-000-0002");
    console_write("[CONTACTS] Ready\n");
}

int contacts_add(const char* name, const char* phone) {
    if (n >= MAX_C) return -1;
    int i = 0;
    if (name) while (name[i] && i < 31) { names[n][i] = name[i]; i++; }
    names[n][i] = 0;
    i = 0;
    if (phone) while (phone[i] && i < 23) { phones[n][i] = phone[i]; i++; }
    phones[n][i] = 0;
    n++;
    return n - 1;
}

void contacts_list_console(void) {
    for (int i = 0; i < n; i++) {
        console_write(names[i]); console_write(" — ");
        console_write(phones[i]); console_write("\n");
    }
}

void contacts_draw(int x, int y) {
    if (!fb_is_ready()) return;
    fb_fill_rect((uint32_t)x, (uint32_t)y, 260, 180, 0xFF1A1A24);
    fb_fill_rect((uint32_t)x, (uint32_t)y, 260, 28, 0xFF3498DB);
    for (int i = 0; i < n && i < 5; i++)
        fb_fill_rect((uint32_t)(x+8), (uint32_t)(y+36+i*28), 244, 24, 0xFF2A2A35);
}
