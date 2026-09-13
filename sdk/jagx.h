#ifndef JAGX_SDK_H
#define JAGX_SDK_H

/*
 * JagX Application SDK (public headers)
 *
 * Apps ship as .jagx packages — never APK, IPA, EXE, or DEB.
 * Link against JagX syscalls / compositor. No Android NDK.
 *
 * Manifest required keys:
 *   name, package, version, vendor, arch, entry, min_os
 * Optional:
 *   perm=telephony,contacts,network,camera,storage,social
 */

#include <stdint.h>

#define JAGX_SDK_VERSION "0.1.0"

/* Package manager */
int jagx_pkg_install(const char* path);          /* must end in .jagx */
int jagx_pkg_is_installed(const char* package_id);

/* Phone / SMS (need perm=telephony) */
int jagx_dial(const char* number);
int jagx_sms(const char* number, const char* text);

/* UI */
void jagx_window_create(int x, int y, int w, int h, const char* title);
void jagx_notify(const char* title, const char* body);

/* Storage under /apps/<package>/ */
int jagx_fs_write(const char* path, const uint8_t* data, uint32_t len);
int jagx_fs_read(const char* path, uint8_t* buf, uint32_t max, uint32_t* out_len);

/* Social (JagCircle, perm=social) */
int jagx_social_post(const char* body);

#endif
