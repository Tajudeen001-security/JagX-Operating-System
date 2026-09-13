#include "filemanager.h"
#include "../fs/ramfs.h"
#include "../kernel/arch/x86_64/console.h"
#include "../kernel/arch/x86_64/framebuffer.h"

void filemanager_init(void) {
    console_write("[FILES] File manager ready (RamFS)\n");
}

void filemanager_list_root(void) {
    console_write("[FILES] /\n");
    ramfs_list("/");
}

void filemanager_draw(void) {
    if (!fb_is_ready()) {
        filemanager_list_root();
        return;
    }
    /* Visual shell: panel representing file list */
    fb_fill_rect(100, 100, 400, 280, 0xFF1A1A24);
    fb_fill_rect(100, 100, 400, 32, 0xFF00D4C8);
    /* rows as placeholders for file entries */
    for (int i = 0; i < 5; i++)
        fb_fill_rect(110, 140 + i * 36, 380, 28, 0xFF2A2A35);
}
