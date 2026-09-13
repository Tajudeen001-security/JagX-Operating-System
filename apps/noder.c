#include "noder.h"
#include "../pkg/jagxpkg.h"
#include "../fs/ramfs.h"
#include "../kernel/arch/x86_64/framebuffer.h"
#include "../kernel/arch/x86_64/console.h"

struct noder_tab {
    int used;
    char name[NODER_NAME_LEN];
    char path[64];
    char text[NODER_BUF_SIZE];
    uint32_t len;
    uint32_t cursor;
    int dirty;
    int lang; /* 0=text 1=c 2=h 3=py 4=js 5=md */
};

static struct noder_tab tabs[NODER_MAX_TABS];
static int active = 0;
static int focused = 1;
static int show_explorer = 1;

static int detect_lang(const char* name) {
    int n = 0;
    while (name[n]) n++;
    if (n >= 2 && name[n-2]=='.' && name[n-1]=='c') return 1;
    if (n >= 2 && name[n-2]=='.' && name[n-1]=='h') return 2;
    if (n >= 3 && name[n-3]=='.' && name[n-2]=='p' && name[n-1]=='y') return 3;
    if (n >= 3 && name[n-3]=='.' && name[n-2]=='j' && name[n-1]=='s') return 4;
    if (n >= 3 && name[n-3]=='.' && name[n-2]=='m' && name[n-1]=='d') return 5;
    return 0;
}

static void copy_str(char* dst, const char* src, int max) {
    int i = 0;
    if (src) while (src[i] && i < max - 1) { dst[i] = src[i]; i++; }
    dst[i] = 0;
}

void noder_init(void) {
    for (int i = 0; i < NODER_MAX_TABS; i++) {
        tabs[i].used = 0;
        tabs[i].len = 0;
        tabs[i].cursor = 0;
        tabs[i].dirty = 0;
        tabs[i].text[0] = 0;
        tabs[i].name[0] = 0;
        tabs[i].path[0] = 0;
    }
    active = 0;
    focused = 1;

    /* Default workspace files */
    noder_new_file("main.c");
    const char* seed =
        "/* Noder on JagX OS — code here */\n"
        "/* JagX & JRILICENSE */\n\n"
        "#include <stdint.h>\n\n"
        "int main(void) {\n"
        "    /* write your program */\n"
        "    return 0;\n"
        "}\n";
    uint32_t i = 0;
    while (seed[i] && i < NODER_BUF_SIZE - 1) {
        tabs[0].text[i] = seed[i];
        i++;
    }
    tabs[0].text[i] = 0;
    tabs[0].len = i;
    tabs[0].cursor = i;
    tabs[0].lang = 1;
    copy_str(tabs[0].path, "/workspace/main.c", 64);

    noder_new_file("readme.md");
    const char* md =
        "# JagX workspace\n\n"
        "Edit files in Noder on JagX OS.\n"
        "Save with Ctrl path via noder_save_current().\n";
    i = 0;
    while (md[i] && i < NODER_BUF_SIZE - 1) {
        tabs[1].text[i] = md[i];
        i++;
    }
    tabs[1].text[i] = 0;
    tabs[1].len = i;
    tabs[1].cursor = i;
    tabs[1].lang = 5;
    copy_str(tabs[1].path, "/workspace/readme.md", 64);

    active = 0;
    console_write("[NODER] Native IDE ready on JagX (tabs + keyboard + save)\n");
}

void noder_focus(int on) { focused = on ? 1 : 0; }
int noder_is_focused(void) { return focused; }

int noder_new_file(const char* name) {
    for (int t = 0; t < NODER_MAX_TABS; t++) {
        if (tabs[t].used) continue;
        tabs[t].used = 1;
        copy_str(tabs[t].name, name ? name : "untitled.txt", NODER_NAME_LEN);
        tabs[t].path[0] = 0;
        tabs[t].text[0] = 0;
        tabs[t].len = 0;
        tabs[t].cursor = 0;
        tabs[t].dirty = 0;
        tabs[t].lang = detect_lang(tabs[t].name);
        active = t;
        return t;
    }
    return -1;
}

int noder_open_file(const char* path) {
    uint8_t tmp[NODER_BUF_SIZE];
    uint32_t n = 0;
    if (ramfs_read(path, tmp, NODER_BUF_SIZE - 1, &n) != 0) return -1;
    int t = noder_new_file(path);
    if (t < 0) {
        /* reuse active */
        t = active;
    }
    for (uint32_t i = 0; i < n; i++) tabs[t].text[i] = (char)tmp[i];
    tabs[t].text[n] = 0;
    tabs[t].len = n;
    tabs[t].cursor = n;
    copy_str(tabs[t].path, path, 64);
    /* basename into name */
    const char* base = path;
    for (int i = 0; path[i]; i++) if (path[i] == '/') base = path + i + 1;
    copy_str(tabs[t].name, base, NODER_NAME_LEN);
    tabs[t].lang = detect_lang(tabs[t].name);
    tabs[t].dirty = 0;
    active = t;
    return t;
}

int noder_save_current(void) {
    if (active < 0 || !tabs[active].used) return -1;
    char path[64];
    if (tabs[active].path[0]) copy_str(path, tabs[active].path, 64);
    else {
        copy_str(path, "/workspace/", 64);
        int pi = 11;
        for (int i = 0; tabs[active].name[i] && pi < 63; i++)
            path[pi++] = tabs[active].name[i];
        path[pi] = 0;
        copy_str(tabs[active].path, path, 64);
    }
    int r = ramfs_write(path, (const uint8_t*)tabs[active].text, tabs[active].len);
    if (r == 0) {
        tabs[active].dirty = 0;
        console_write("[NODER] Saved ");
        console_write(path);
        console_write("\n");
    }
    return r;
}

int noder_switch_tab(int index) {
    if (index < 0 || index >= NODER_MAX_TABS || !tabs[index].used) return -1;
    active = index;
    return 0;
}

int noder_tab_count(void) {
    int c = 0;
    for (int i = 0; i < NODER_MAX_TABS; i++) if (tabs[i].used) c++;
    return c;
}

const char* noder_tab_name(int index) {
    if (index < 0 || index >= NODER_MAX_TABS || !tabs[index].used) return "";
    return tabs[index].name;
}

const char* noder_current_text(void) {
    if (!tabs[active].used) return "";
    return tabs[active].text;
}

uint32_t noder_current_len(void) {
    if (!tabs[active].used) return 0;
    return tabs[active].len;
}

void noder_on_key(char c) {
    if (!focused || !tabs[active].used) return;
    struct noder_tab* t = &tabs[active];

    if (c == 27) { /* Esc — unfocus */
        focused = 0;
        return;
    }
    if (c == '\b') {
        if (t->cursor == 0 || t->len == 0) return;
        /* delete before cursor */
        for (uint32_t i = t->cursor - 1; i < t->len; i++)
            t->text[i] = t->text[i + 1];
        t->cursor--;
        t->len--;
        t->text[t->len] = 0;
        t->dirty = 1;
        return;
    }
    if (c == 19) { /* Ctrl+S style if host sends 0x13 */
        noder_save_current();
        return;
    }
    if (t->len + 1 >= NODER_BUF_SIZE) return;
    /* insert at cursor */
    for (uint32_t i = t->len + 1; i > t->cursor; i--)
        t->text[i] = t->text[i - 1];
    t->text[t->cursor] = c;
    t->cursor++;
    t->len++;
    t->text[t->len] = 0;
    t->dirty = 1;
}

void noder_draw(int x, int y, int w, int h) {
    if (!fb_is_ready()) return;

    /* chrome */
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, (uint32_t)h, 0xFF1E1E1E);
    fb_fill_rect((uint32_t)x, (uint32_t)y, (uint32_t)w, 28, 0xFF007ACC);

    int ex_w = show_explorer ? 100 : 0;
    if (show_explorer) {
        fb_fill_rect((uint32_t)x, (uint32_t)(y + 28), (uint32_t)ex_w, (uint32_t)(h - 28), 0xFF252526);
        for (int i = 0; i < NODER_MAX_TABS; i++) {
            if (!tabs[i].used) continue;
            uint32_t col = (i == active) ? 0xFF37373D : 0xFF2D2D30;
            fb_fill_rect((uint32_t)(x + 4), (uint32_t)(y + 36 + i * 28), (uint32_t)(ex_w - 8), 24, col);
        }
    }

    /* tab bar */
    int tx = x + ex_w + 4;
    for (int i = 0; i < NODER_MAX_TABS; i++) {
        if (!tabs[i].used) continue;
        uint32_t col = (i == active) ? 0xFF1E1E1E : 0xFF2D2D30;
        fb_fill_rect((uint32_t)tx, (uint32_t)(y + 28), 90, 22, col);
        if (tabs[i].dirty)
            fb_fill_rect((uint32_t)(tx + 80), (uint32_t)(y + 34), 6, 6, 0xFFCCCCCC);
        tx += 94;
    }

    /* editor body — line gutters */
    int body_x = x + ex_w + 8;
    int body_y = y + 54;
    int body_w = w - ex_w - 16;
    int body_h = h - 70;
    fb_fill_rect((uint32_t)body_x, (uint32_t)body_y, (uint32_t)body_w, (uint32_t)body_h, 0xFF1E1E1E);
    fb_fill_rect((uint32_t)body_x, (uint32_t)body_y, 28, (uint32_t)body_h, 0xFF1E1E1E);

    if (tabs[active].used) {
        int lines = 1;
        for (uint32_t i = 0; i < tabs[active].len; i++)
            if (tabs[active].text[i] == '\n') lines++;
        if (lines > 24) lines = 24;
        for (int L = 0; L < lines; L++) {
            fb_fill_rect((uint32_t)(body_x + 32), (uint32_t)(body_y + 4 + L * 12),
                         (uint32_t)(body_w - 40), 2, 0xFF404040);
        }
        /* language color strip */
        uint32_t lc = 0xFF569CD6;
        if (tabs[active].lang == 3) lc = 0xFF4EC9B0;
        if (tabs[active].lang == 5) lc = 0xFFC586C0;
        fb_fill_rect((uint32_t)(x + w - 8), (uint32_t)(y + 28), 4, (uint32_t)(h - 28), lc);
    }

    /* status bar */
    fb_fill_rect((uint32_t)x, (uint32_t)(y + h - 18), (uint32_t)w, 18, 0xFF007ACC);
    if (focused)
        fb_fill_rect((uint32_t)(x + 4), (uint32_t)(y + h - 14), 12, 10, 0xFF3FB950);
}

int noder_package_install(void) {
    static const char manifest[] =
        "name=Noder\n"
        "package=com.jagx.noder\n"
        "version=0.2.0\n"
        "vendor=JagX & JRILICENSE\n"
        "arch=x86\n"
        "entry=noder.main\n"
        "min_os=0.0.24\n";
    const uint8_t* payload = (const uint8_t*)"NODER_JAGX_NATIVE_IDE";
    uint8_t pkg[2048];
    uint32_t n = pkg_build(manifest, payload, 21, pkg, sizeof(pkg));
    if (!n) return -1;
    ramfs_write("/packages/noder.jagx", pkg, n);
    return pkg_install_buffer(pkg, n);
}
