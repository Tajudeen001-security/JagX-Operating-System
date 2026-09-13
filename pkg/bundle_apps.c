/* Install core apps as .jagx packages */
#include "jagxpkg.h"
#include "../fs/ramfs.h"
#include "../kernel/arch/x86_64/console.h"

static int install_one(const char* manifest, const char* mark, const char* path) {
    uint8_t pkg[1024];
    uint32_t plen = 0;
    while (mark[plen]) plen++;
    uint32_t n = pkg_build(manifest, (const uint8_t*)mark, plen, pkg, sizeof(pkg));
    if (!n) return -1;
    ramfs_write(path, pkg, n);
    return pkg_install_buffer(pkg, n);
}

void pkg_bundle_core_apps(void) {
    install_one(
        "name=Notepad\npackage=com.jagx.notepad\nversion=1.0.0\nvendor=JagX\narch=x86\nentry=notepad.main\n",
        "NOTEPAD_APP", "/packages/notepad.jagx");
    install_one(
        "name=Paint\npackage=com.jagx.paint\nversion=1.0.0\nvendor=JagX\narch=x86\nentry=paint.main\n",
        "PAINT_APP", "/packages/paint.jagx");
    install_one(
        "name=JagSheet\npackage=com.jagx.sheet\nversion=1.0.0\nvendor=JagX\narch=x86\nentry=sheet.main\n",
        "SHEET_APP", "/packages/sheet.jagx");
    install_one(
        "name=JagBase\npackage=com.jagx.database\nversion=1.0.0\nvendor=JagX\narch=x86\nentry=db.main\n",
        "DB_APP", "/packages/database.jagx");
    install_one(
        "name=Terminal\npackage=com.jagx.terminal\nversion=1.0.0\nvendor=JagX\narch=x86\nentry=term.main\n",
        "TERM_APP", "/packages/terminal.jagx");
    install_one(
        "name=Calculator\npackage=com.jagx.calc\nversion=1.0.0\nvendor=JagX\narch=x86\nentry=calc.main\n",
        "CALC_APP", "/packages/calc.jagx");
    install_one(
        "name=Forms\npackage=com.jagx.forms\nversion=1.0.0\nvendor=JagX\narch=x86\nentry=forms.main\n",
        "FORMS_APP", "/packages/forms.jagx");
    install_one(
        "name=Contacts\npackage=com.jagx.contacts\nversion=1.0.0\nvendor=JagX\narch=x86\nentry=contacts.main\n",
        "CONTACTS_APP", "/packages/contacts.jagx");
    install_one(
        "name=Gallery\npackage=com.jagx.gallery\nversion=1.0.0\nvendor=JagX\narch=x86\nentry=gallery.main\n",
        "GALLERY_APP", "/packages/gallery.jagx");
    install_one(
        "name=Settings\npackage=com.jagx.settings\nversion=1.0.0\nvendor=JagX\narch=x86\nentry=settings.main\n",
        "SETTINGS_APP", "/packages/settings.jagx");
    console_write("[PKG] Core apps packaged as .jagx\n");
}
