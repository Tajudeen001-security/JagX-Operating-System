#include "jagxpkg.h"
#include "../fs/ramfs.h"
#include "../kernel/arch/x86_64/console.h"

static struct jagx_pkg_info installed[JAGX_PKG_MAX];
static int n_inst = 0;

void pkg_init(void) {
    n_inst = 0;
    for (int i = 0; i < JAGX_PKG_MAX; i++) installed[i].installed = 0;
    console_write("[PKG] .jagx package manager online\n");
}

static int key_eq(const char* line, const char* key) {
    int i = 0;
    while (key[i] && line[i] == key[i]) i++;
    return key[i] == 0 && line[i] == '=';
}

static void copy_val(const char* line, char* dst, int max) {
    int i = 0;
    while (line[i] && line[i] != '=') i++;
    if (line[i] == '=') i++;
    int j = 0;
    while (line[i] && line[i] != '\n' && line[i] != '\r' && j < max - 1)
        dst[j++] = line[i++];
    dst[j] = 0;
}

int pkg_parse_manifest(const char* text, struct jagx_pkg_info* out) {
    if (!text || !out) return -1;
    out->name[0] = out->package[0] = out->version[0] = 0;
    out->vendor[0] = out->entry[0] = out->arch[0] = 0;
    out->installed = 0;
    const char* p = text;
    while (*p) {
        if (key_eq(p, "name")) copy_val(p, out->name, JAGX_PKG_NAME);
        else if (key_eq(p, "package")) copy_val(p, out->package, JAGX_PKG_NAME);
        else if (key_eq(p, "version")) copy_val(p, out->version, 16);
        else if (key_eq(p, "vendor")) copy_val(p, out->vendor, JAGX_PKG_NAME);
        else if (key_eq(p, "entry")) copy_val(p, out->entry, JAGX_PKG_NAME);
        else if (key_eq(p, "arch")) copy_val(p, out->arch, 16);
        while (*p && *p != '\n') p++;
        if (*p == '\n') p++;
    }
    return out->package[0] ? 0 : -1;
}

static uint32_t checksum(const uint8_t* p, uint32_t n) {
    uint32_t s = 0;
    for (uint32_t i = 0; i < n; i++) s += p[i];
    return s;
}

uint32_t pkg_build(const char* manifest, const uint8_t* payload, uint32_t plen,
                   uint8_t* out, uint32_t out_max) {
    uint32_t mlen = 0;
    while (manifest[mlen]) mlen++;
    uint32_t total = 16 + mlen + plen; /* sizeof header fields we write */
    if (total > out_max) return 0;

    struct jagx_pkg_header h;
    h.magic = JAGX_PKG_MAGIC;
    h.version = JAGX_PKG_VER;
    h.flags = 0;
    h.manifest_len = mlen;
    h.payload_len = plen;
    h.checksum = checksum(payload, plen);

    uint32_t o = 0;
    out[o++] = (uint8_t)(h.magic); out[o++] = (uint8_t)(h.magic >> 8);
    out[o++] = (uint8_t)(h.magic >> 16); out[o++] = (uint8_t)(h.magic >> 24);
    out[o++] = (uint8_t)(h.version); out[o++] = (uint8_t)(h.version >> 8);
    out[o++] = (uint8_t)(h.flags); out[o++] = (uint8_t)(h.flags >> 8);
    out[o++] = (uint8_t)(h.manifest_len); out[o++] = (uint8_t)(h.manifest_len >> 8);
    out[o++] = (uint8_t)(h.manifest_len >> 16); out[o++] = (uint8_t)(h.manifest_len >> 24);
    out[o++] = (uint8_t)(h.payload_len); out[o++] = (uint8_t)(h.payload_len >> 8);
    out[o++] = (uint8_t)(h.payload_len >> 16); out[o++] = (uint8_t)(h.payload_len >> 24);
    out[o++] = (uint8_t)(h.checksum); out[o++] = (uint8_t)(h.checksum >> 8);
    out[o++] = (uint8_t)(h.checksum >> 16); out[o++] = (uint8_t)(h.checksum >> 24);

    for (uint32_t i = 0; i < mlen; i++) out[o++] = (uint8_t)manifest[i];
    for (uint32_t i = 0; i < plen; i++) out[o++] = payload[i];
    return o;
}

int pkg_install_buffer(const uint8_t* data, uint32_t len) {
    if (!data || len < 24) return -1;
    uint32_t magic = (uint32_t)data[0] | ((uint32_t)data[1] << 8) |
                     ((uint32_t)data[2] << 16) | ((uint32_t)data[3] << 24);
    if (magic != JAGX_PKG_MAGIC) {
        console_write("[PKG] Not a .jagx package (bad magic)\n");
        return -1;
    }
    uint32_t mlen = (uint32_t)data[8] | ((uint32_t)data[9] << 8) |
                    ((uint32_t)data[10] << 16) | ((uint32_t)data[11] << 24);
    uint32_t plen = (uint32_t)data[12] | ((uint32_t)data[13] << 8) |
                    ((uint32_t)data[14] << 16) | ((uint32_t)data[15] << 24);
    uint32_t csum = (uint32_t)data[16] | ((uint32_t)data[17] << 8) |
                    ((uint32_t)data[18] << 16) | ((uint32_t)data[19] << 24);
    if (24 + mlen + plen > len) return -1;
    const uint8_t* man = data + 24;
    const uint8_t* pay = data + 24 + mlen;
    if (checksum(pay, plen) != csum) {
        console_write("[PKG] Checksum failed\n");
        return -1;
    }

    char mtxt[512];
    uint32_t copy = mlen < 511 ? mlen : 511;
    for (uint32_t i = 0; i < copy; i++) mtxt[i] = (char)man[i];
    mtxt[copy] = 0;

    struct jagx_pkg_info info;
    if (pkg_parse_manifest(mtxt, &info) != 0) return -1;

    /* store payload under /apps/<package> */
    char path[96];
    int pi = 0;
    const char* pref = "/apps/";
    while (pref[pi]) { path[pi] = pref[pi]; pi++; }
    for (int i = 0; info.package[i] && pi < 90; i++) path[pi++] = info.package[i];
    path[pi++] = '.'; path[pi++] = 'b'; path[pi++] = 'i'; path[pi++] = 'n'; path[pi] = 0;
    ramfs_write(path, pay, plen);

    char mpath[96];
    pi = 0;
    while (pref[pi]) { mpath[pi] = pref[pi]; pi++; }
    for (int i = 0; info.package[i] && pi < 80; i++) mpath[pi++] = info.package[i];
    mpath[pi++] = '.'; mpath[pi++] = 'm'; mpath[pi++] = 'f'; mpath[pi] = 0;
    ramfs_write(mpath, (const uint8_t*)mtxt, copy);

    if (n_inst < JAGX_PKG_MAX) {
        installed[n_inst] = info;
        installed[n_inst].installed = 1;
        n_inst++;
    }
    console_write("[PKG] Installed ");
    console_write(info.name);
    console_write(" (");
    console_write(info.package);
    console_write(")\n");
    return 0;
}

int pkg_install_path(const char* path) {
    uint8_t buf[4096];
    uint32_t n = 0;
    if (ramfs_read(path, buf, sizeof(buf), &n) != 0) return -1;
    return pkg_install_buffer(buf, n);
}

int pkg_list_count(void) { return n_inst; }

const struct jagx_pkg_info* pkg_list_at(int index) {
    if (index < 0 || index >= n_inst) return 0;
    return &installed[index];
}

int pkg_is_installed(const char* package_id) {
    for (int i = 0; i < n_inst; i++) {
        int j = 0;
        while (package_id[j] && installed[i].package[j] == package_id[j]) j++;
        if (package_id[j] == 0 && installed[i].package[j] == 0) return 1;
    }
    return 0;
}
