#ifndef JAGX_LANG_H
#define JAGX_LANG_H

/* Bilingual / multilingual UI — EN + Hausa + Yoruba + Igbo + French */

enum jagx_lang {
    LANG_EN = 0,
    LANG_HA,  /* Hausa */
    LANG_YO,  /* Yoruba */
    LANG_IG,  /* Igbo */
    LANG_FR,  /* French (West/Central Africa) */
    LANG_COUNT
};

void lang_init(void);
void lang_set(enum jagx_lang lang);
enum jagx_lang lang_get(void);
const char* lang_name(enum jagx_lang lang);

/* Common UI strings */
const char* L(const char* key);

#endif
