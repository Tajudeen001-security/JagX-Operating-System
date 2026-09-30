#include "auth.h"
#include "../kernel/arch/x86_64/console.h"

static char stored_pin[16];
static char stored_pw[64];
static char entry[64];
static int entry_len = 0;
static int pin_on = 1;
static int pw_on = 0;
static int fp_on = 0;
static int face_on = 0;
static int authenticated = 0;
static int fails = 0;
static enum auth_method method = AUTH_PIN;

static int streq(const char* a, const char* b) {
    int i = 0;
    while (a[i] && b[i] && a[i] == b[i]) i++;
    return a[i] == 0 && b[i] == 0;
}

void auth_init(void) {
    stored_pin[0] = '1'; stored_pin[1] = '2'; stored_pin[2] = '3';
    stored_pin[3] = '4'; stored_pin[4] = 0;
    stored_pw[0] = 0;
    entry[0] = 0;
    entry_len = 0;
    pin_on = 1;
    authenticated = 0;
    fails = 0;
    method = AUTH_PIN;
    console_write("[AUTH] PIN/password/fingerprint/face framework ready\n");
    console_write("[AUTH] Default PIN for lab: 1234\n");
}

void auth_set_pin(const char* pin) {
    int i = 0;
    if (!pin) return;
    while (pin[i] && i < 15) { stored_pin[i] = pin[i]; i++; }
    stored_pin[i] = 0;
    pin_on = 1;
}

void auth_set_password(const char* pw) {
    int i = 0;
    if (!pw) return;
    while (pw[i] && i < 63) { stored_pw[i] = pw[i]; i++; }
    stored_pw[i] = 0;
    pw_on = 1;
}

int auth_pin_enabled(void) { return pin_on; }
int auth_password_enabled(void) { return pw_on; }
int auth_fingerprint_enabled(void) { return fp_on; }
int auth_face_enabled(void) { return face_on; }

void auth_enable_fingerprint(int on) {
    fp_on = on ? 1 : 0;
    console_write(fp_on ? "[AUTH] Fingerprint enabled (needs sensor HAL)\n"
                        : "[AUTH] Fingerprint disabled\n");
}

void auth_enable_face(int on) {
    face_on = on ? 1 : 0;
    console_write(face_on ? "[AUTH] Face unlock enabled (needs camera HAL)\n"
                          : "[AUTH] Face unlock disabled\n");
}

static enum auth_result after_try(int ok) {
    if (ok) {
        authenticated = 1;
        fails = 0;
        entry_len = 0;
        entry[0] = 0;
        console_write("[AUTH] Unlocked\n");
        return AUTH_OK;
    }
    fails++;
    entry_len = 0;
    entry[0] = 0;
    if (fails >= 5) {
        console_write("[AUTH] Too many attempts\n");
        return AUTH_LOCKED_OUT;
    }
    console_write("[AUTH] Failed\n");
    return AUTH_FAIL;
}

enum auth_result auth_try_pin(const char* pin) {
    if (!pin_on) return AUTH_FAIL;
    return after_try(streq(pin, stored_pin));
}

enum auth_result auth_try_password(const char* pw) {
    if (!pw_on || !stored_pw[0]) return AUTH_FAIL;
    return after_try(streq(pw, stored_pw));
}

enum auth_result auth_try_fingerprint(void) {
    if (!fp_on) return AUTH_SENSOR_MISSING;
    /* Lab: accept simulated match when sensor HAL not present */
    console_write("[AUTH] Fingerprint sample (HAL stub OK in lab)\n");
    return after_try(1);
}

enum auth_result auth_try_face(void) {
    if (!face_on) return AUTH_SENSOR_MISSING;
    console_write("[AUTH] Face sample (camera HAL stub OK in lab)\n");
    return after_try(1);
}

void auth_on_key(char c) {
    if (c == '\n' || c == '\r') {
        entry[entry_len] = 0;
        if (method == AUTH_PIN) auth_try_pin(entry);
        else if (method == AUTH_PASSWORD) auth_try_password(entry);
        return;
    }
    if (c == '\b') {
        if (entry_len > 0) entry[--entry_len] = 0;
        return;
    }
    if (entry_len < 63 && c >= 32 && c < 127) {
        entry[entry_len++] = c;
        entry[entry_len] = 0;
    }
}

void auth_clear_entry(void) { entry_len = 0; entry[0] = 0; }

const char* auth_entry_masked(void) {
    static char mask[64];
    int i;
    for (i = 0; i < entry_len && i < 63; i++) mask[i] = '*';
    mask[i] = 0;
    return mask;
}

enum auth_method auth_active_method(void) { return method; }
void auth_set_method(enum auth_method m) { method = m; auth_clear_entry(); }
int auth_is_authenticated(void) { return authenticated; }
void auth_logout(void) { authenticated = 0; fails = 0; auth_clear_entry(); }
