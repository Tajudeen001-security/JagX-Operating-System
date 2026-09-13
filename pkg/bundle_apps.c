/* Install core apps as .jagx packages */
#include "jagxpkg.h"
#include "../fs/ramfs.h"
#include "../kernel/arch/x86_64/console.h"

static int install_one(const char* manifest, const char* mark, const char* path) {
    uint8_t pkg[2048];
    uint32_t plen = 0;
    while (mark[plen]) plen++;
    uint32_t n = pkg_build(manifest, (const uint8_t*)mark, plen, pkg, sizeof(pkg));
    if (!n) return -1;
    ramfs_write(path, pkg, n);
    return pkg_install_buffer(pkg, n);
}

void pkg_bundle_core_apps(void) {
    install_one(
        "name=Notepad\npackage=com.jagx.notepad\nversion=1.0.0\nvendor=JagX\narch=any\nentry=notepad.main\nmin_os=0.1.0\nperm=storage\n",
        "NOTEPAD_APP", "/packages/notepad.jagx");
    install_one(
        "name=Paint\npackage=com.jagx.paint\nversion=1.0.0\nvendor=JagX\narch=any\nentry=paint.main\nmin_os=0.1.0\nperm=storage\n",
        "PAINT_APP", "/packages/paint.jagx");
    install_one(
        "name=JagSheet\npackage=com.jagx.sheet\nversion=1.0.0\nvendor=JagX\narch=any\nentry=sheet.main\nmin_os=0.1.0\nperm=storage\n",
        "SHEET_APP", "/packages/sheet.jagx");
    install_one(
        "name=JagBase\npackage=com.jagx.database\nversion=1.0.0\nvendor=JagX\narch=any\nentry=db.main\nmin_os=0.1.0\nperm=storage\n",
        "DB_APP", "/packages/database.jagx");
    install_one(
        "name=Terminal\npackage=com.jagx.terminal\nversion=1.0.0\nvendor=JagX\narch=any\nentry=term.main\nmin_os=0.1.0\nperm=\n",
        "TERM_APP", "/packages/terminal.jagx");
    install_one(
        "name=Calculator\npackage=com.jagx.calc\nversion=1.0.0\nvendor=JagX\narch=any\nentry=calc.main\nmin_os=0.1.0\nperm=\n",
        "CALC_APP", "/packages/calc.jagx");
    install_one(
        "name=Forms\npackage=com.jagx.forms\nversion=1.0.0\nvendor=JagX\narch=any\nentry=forms.main\nmin_os=0.1.0\nperm=storage\n",
        "FORMS_APP", "/packages/forms.jagx");
    install_one(
        "name=Contacts\npackage=com.jagx.contacts\nversion=1.0.0\nvendor=JagX\narch=any\nentry=contacts.main\nmin_os=0.1.0\nperm=contacts\n",
        "CONTACTS_APP", "/packages/contacts.jagx");
    install_one(
        "name=Gallery\npackage=com.jagx.gallery\nversion=1.0.0\nvendor=JagX\narch=any\nentry=gallery.main\nmin_os=0.1.0\nperm=storage,camera\n",
        "GALLERY_APP", "/packages/gallery.jagx");
    install_one(
        "name=Settings\npackage=com.jagx.settings\nversion=1.0.0\nvendor=JagX\narch=any\nentry=settings.main\nmin_os=0.1.0\nperm=\n",
        "SETTINGS_APP", "/packages/settings.jagx");
    install_one(
        "name=Phone\npackage=com.jagx.phone\nversion=1.0.0\nvendor=JagX\narch=any\nentry=phone.main\nmin_os=0.1.0\nperm=telephony,contacts\n",
        "PHONE_APP", "/packages/phone.jagx");
    install_one(
        "name=Messages\npackage=com.jagx.messages\nversion=1.0.0\nvendor=JagX\narch=any\nentry=messages.main\nmin_os=0.1.0\nperm=telephony,contacts\n",
        "MESSAGES_APP", "/packages/messages.jagx");
    install_one(
        "name=JagCircle\npackage=com.jagx.social\nversion=1.0.0\nvendor=JagX\narch=any\nentry=social.main\nmin_os=0.1.0\nperm=social,network\n",
        "SOCIAL_APP", "/packages/social.jagx");
    install_one(
        "name=JagX Connect\npackage=com.jagx.connect\nversion=1.0.0\nvendor=JagX & JRILICENSE\narch=any\nentry=connect.main\nmin_os=0.1.0\nperm=social,network,storage,camera\n",
        "CONNECT_APP", "/packages/connect.jagx");
    install_one(
        "name=JagBrowser\npackage=com.jagx.browser\nversion=1.0.0\nvendor=JagX\narch=any\nentry=browser.main\nmin_os=0.1.0\nperm=network\n",
        "BROWSER_APP", "/packages/browser.jagx");
    install_one(
        "name=JagStore\npackage=com.jagx.store\nversion=1.0.0\nvendor=JagX\narch=any\nentry=store.main\nmin_os=0.1.0\nperm=storage\n",
        "STORE_APP", "/packages/store.jagx");
    console_write("[PKG] Core apps packaged as .jagx including Connect (APK refused)\n");
}
