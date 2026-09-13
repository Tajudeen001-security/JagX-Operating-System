#include "noder.h"
#include "../pkg/jagxpkg.h"
#include "../fs/ramfs.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

#define NODER_BUF 4096
static char buffer[NODER_BUF];
static uint32_t blen = 0;
static char title[64];

void noder_init(void) {
    blen = 0;
    buffer[0] = 0;
    title[0] = 'u'; title[1] = 'n'; title[2] = 't'; title[3] = 'i';
    title[4] = 't'; title[5] = 'l'; title[6] = 'e'; title[7] = 'd';
    title[8] = '.'; title[9] = 'c'; title[10] = 0;
    const char* seed =
        "/* Noder — JagX & JRILICENSE */\n"
        "/* Code editor for JagX OS (.jagx native) */\n"
        "#include <stdio.h>\n\n"
        "int main(void) {\n"
        "    return 0;\n"
        "}\n";
    while (seed[blen] && blen < NODER_BUF - 1) {
        buffer[blen] = seed[blen];
        blen++;
    }
    buffer[blen] = 0;
    console_write("[NODER] Editor ready (JagX native, not APK)\n");
}

void noder_open_buffer(const char* t) {
    int i = 0;
    if (t) while (t[i] && i < 63) { title[i] = t[i]; i++; }
    title[i] = 0;
}

void noder_insert(char c) {
    if (blen + 1 >= NODER_BUF) return;
    buffer[blen++] = c;
    buffer[blen] = 0;
}

void noder_backspace(void) {
    if (blen == 0) return;
    buffer[--blen] = 0;
}

const char* noder_buffer(void) { return buffer; }

int noder_save(const char* path) {
    return ramfs_write(path, (const uint8_t*)buffer, blen);
}

void noder_draw(int x, int y, int w, int h) {
    if (!fb_is_ready()) return;
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, (uint32_t)h, 0xFF1E1E1E);
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, 28, 0xFF007ACC); /* VS-like accent */
    fb_fill_rect((uint32_t)x, (uint32_t)(y + 28), 48, (uint32_t)(h - 28), 0xFF252526);
    int lines = (int)(blen / 48) + 1;
    if (lines > 20) lines = 20;
    for (int i = 0; i < lines; i++)
        fb_fill_rect((uint32_t)(x + 56), (uint32_t)(y + 36 + i * 14),
                     (uint32_t)(w - 64), 3, 0xFF3C3C3C);
}

int noder_package_install(void) {
    static const char manifest[] =
        "name=Noder\n"
        "package=com.jagx.noder\n"
        "version=1.0.0\n"
        "vendor=JagX & JRILICENSE\n"
        "arch=x86\n"
        "entry=noder.main\n"
        "min_os=0.0.22\n";
    /* payload = editor buffer snapshot / app marker */
    const uint8_t* payload = (const uint8_t*)"NODER_JAGX_APP";
    uint32_t plen = 14;
    uint8_t pkg[2048];
    uint32_t n = pkg_build(manifest, payload, plen, pkg, sizeof(pkg));
    if (!n) return -1;
    ramfs_write("/packages/noder.jagx", pkg, n);
    return pkg_install_buffer(pkg, n);
}
