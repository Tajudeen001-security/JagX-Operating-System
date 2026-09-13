#ifndef JAGX_PKG_H
#define JAGX_PKG_H

#include <stdint.h>

#define JAGX_PKG_MAGIC 0x5847414Au /* 'JAGX' little-endian bytes J A G X */
#define JAGX_PKG_VER   1
#define JAGX_PKG_NAME  64
#define JAGX_PKG_MAX   32
#define JAGX_PKG_PERM  96

struct jagx_pkg_header {
    uint32_t magic;
    uint16_t version;
    uint16_t flags;
    uint32_t manifest_len;
    uint32_t payload_len;
    uint32_t checksum; /* simple sum of payload bytes */
};

struct jagx_pkg_info {
    char name[JAGX_PKG_NAME];
    char package[JAGX_PKG_NAME];
    char version[16];
    char vendor[JAGX_PKG_NAME];
    char entry[JAGX_PKG_NAME];
    char arch[16];
    char perm[JAGX_PKG_PERM];
    char min_os[16];
    int installed;
};

void pkg_init(void);
int  pkg_parse_manifest(const char* text, struct jagx_pkg_info* out);
int  pkg_install_buffer(const uint8_t* data, uint32_t len);
int  pkg_install_path(const char* path);
int  pkg_list_count(void);
const struct jagx_pkg_info* pkg_list_at(int index);
int  pkg_is_installed(const char* package_id);

/* Build a minimal .jagx in memory for bundling */
uint32_t pkg_build(const char* manifest, const uint8_t* payload, uint32_t plen,
                   uint8_t* out, uint32_t out_max);

#endif
