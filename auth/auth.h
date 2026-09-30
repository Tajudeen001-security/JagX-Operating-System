#ifndef JAGX_AUTH_H
#define JAGX_AUTH_H

#include <stdint.h>

enum auth_method {
    AUTH_NONE = 0,
    AUTH_PIN,
    AUTH_PASSWORD,
    AUTH_FINGERPRINT,
    AUTH_FACE
};

enum auth_result {
    AUTH_OK = 0,
    AUTH_FAIL,
    AUTH_LOCKED_OUT,
    AUTH_SENSOR_MISSING
};

void auth_init(void);
void auth_set_pin(const char* pin4_to_8);
void auth_set_password(const char* pw);
int  auth_pin_enabled(void);
int  auth_password_enabled(void);
int  auth_fingerprint_enabled(void);
int  auth_face_enabled(void);
void auth_enable_fingerprint(int on); /* needs sensor HAL on device */
void auth_enable_face(int on);        /* needs camera HAL on device */

enum auth_result auth_try_pin(const char* pin);
enum auth_result auth_try_password(const char* pw);
enum auth_result auth_try_fingerprint(void); /* HAL callback or lab simulate */
enum auth_result auth_try_face(void);

void auth_on_key(char c); /* build PIN/password entry */
void auth_clear_entry(void);
const char* auth_entry_masked(void);
enum auth_method auth_active_method(void);
void auth_set_method(enum auth_method m);
int  auth_is_authenticated(void);
void auth_logout(void);

#endif
