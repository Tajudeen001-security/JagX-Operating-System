#include "lang.h"
#include "../kernel/arch/x86_64/console.h"

static enum jagx_lang current = LANG_EN;

struct pair { const char* key; const char* en; const char* ha; const char* yo; const char* ig; const char* fr; };

static const struct pair table[] = {
    {"app.settings", "Settings", "Saiti", "Eto", "Ntọala", "Paramètres"},
    {"app.files", "Files", "Fayiloli", "Awọn faili", "Faịlụ", "Fichiers"},
    {"app.gallery", "Gallery", "Gallo", "Ile aworan", "Galarị", "Galerie"},
    {"app.notepad", "Notepad", "Allura", "Akọ̀wé", "Notepad", "Bloc-notes"},
    {"app.paint", "Paint", "Fenti", "Kun", "Agba", "Peinture"},
    {"app.sheet", "Spreadsheet", "Sheet", "Sheet", "Sheet", "Tableur"},
    {"app.database", "Database", "Bayanan", "Database", "Database", "Base de données"},
    {"app.terminal", "Terminal", "Terminal", "Terminal", "Terminal", "Terminal"},
    {"app.calc", "Calculator", "Kalkuleta", "Iṣiro", "Kalkuleta", "Calculatrice"},
    {"app.calendar", "Calendar", "Kalanda", "Kalẹnda", "Kalenda", "Calendrier"},
    {"app.contacts", "Contacts", "Lambobi", "Awọn olubasọrọ", "Kọntaktị", "Contacts"},
    {"app.browser", "Browser", "Browser", "Browser", "Browser", "Navigateur"},
    {"cc.wifi", "Wi-Fi", "Wi-Fi", "Wi-Fi", "Wi-Fi", "Wi-Fi"},
    {"cc.data", "Mobile data", "Data", "Data", "Data", "Données mobiles"},
    {"cc.airplane", "Airplane", "Jirgin sama", "Ọkọ ofurufu", "Ụgbọelu", "Mode avion"},
    {"cc.torch", "Torch", "Tocila", "Tọọṣi", "Torch", "Lampe"},
    {"lock.unlock", "Swipe up to unlock", "Ja sama don buɗe", "Fa si oke lati ṣii", "Bugharia elu", "Glissez pour déverrouiller"},
    {"welcome", "Welcome to JagX", "Barka da zuwa JagX", "Kaabo si JagX", "Nnọọ na JagX", "Bienvenue sur JagX"},
    {0,0,0,0,0,0}
};

void lang_init(void) {
    current = LANG_EN;
    console_write("[i18n] EN/HA/YO/IG/FR ready\n");
}

void lang_set(enum jagx_lang lang) {
    if (lang >= 0 && lang < LANG_COUNT) current = lang;
}

enum jagx_lang lang_get(void) { return current; }

const char* lang_name(enum jagx_lang lang) {
    switch (lang) {
        case LANG_EN: return "English";
        case LANG_HA: return "Hausa";
        case LANG_YO: return "Yoruba";
        case LANG_IG: return "Igbo";
        case LANG_FR: return "Français";
        default: return "?";
    }
}

const char* L(const char* key) {
    for (int i = 0; table[i].key; i++) {
        const char* k = table[i].key;
        int j = 0;
        while (k[j] && key[j] && k[j] == key[j]) j++;
        if (k[j] == 0 && key[j] == 0) {
            switch (current) {
                case LANG_HA: return table[i].ha;
                case LANG_YO: return table[i].yo;
                case LANG_IG: return table[i].ig;
                case LANG_FR: return table[i].fr;
                default: return table[i].en;
            }
        }
    }
    return key;
}
