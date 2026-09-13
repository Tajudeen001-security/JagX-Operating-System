/* JagStore — installs .jagx packages only. Refuses APK/IPA/EXE. */
#include "store.h"
#include "../pkg/jagxpkg.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

void store_init(void) {
    console_write("[STORE] JagStore — .jagx only (no APK)\n");
}

int store_install_path(const char* jagx_path) {
    if (!jagx_path) return -1;
    /* Reject Android / iOS / Windows payloads by extension */
    int n = 0;
    while (jagx_path[n]) n++;
    if (n >= 4) {
        const char* e = jagx_path + n - 4;
        if ((e[0]=='.' && e[1]=='a' && e[2]=='p' && e[3]=='k') ||
            (e[0]=='.' && e[1]=='i' && e[2]=='p' && e[3]=='a') ||
            (e[0]=='.' && e[1]=='e' && e[2]=='x' && e[3]=='e')) {
            console_write("[STORE] Refused: not a .jagx package\n");
            return -1;
        }
    }
    return pkg_install_path(jagx_path);
}

void store_list_console(void) {
    int c = pkg_list_count();
    for (int i = 0; i < c; i++) {
        const struct jagx_pkg_info* p = pkg_list_at(i);
        if (!p) continue;
        console_write(p->name);
        console_write(" ");
        console_write(p->package);
        console_write("\n");
    }
}

void store_draw(int x, int y, int w, int h) {
    if (!fb_is_ready()) return;
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, (uint32_t)h, 0xFF0B0D12);
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, 36, 0xFF1EC9BC);
    int c = pkg_list_count();
    if (c > 8) c = 8;
    for (int i = 0; i < c; i++)
        fb_fill_rect((uint32_t)(x + 12), (uint32_t)(y + 52 + i * 36),
                     (uint32_t)(w - 24), 28, 0xFF1A1D26);
}
