#include "search.h"
#include <stddef.h>

/* Minimal helpers (no libc) */
static int jagx_strlen(const char* s) {
    int n = 0;
    while (s[n]) n++;
    return n;
}

static int jagx_str_contains(const char* hay, const char* needle) {
    int hl = jagx_strlen(hay), nl = jagx_strlen(needle);
    if (nl == 0) return 1;
    for (int i = 0; i <= hl - nl; i++) {
        int j = 0;
        while (j < nl && hay[i + j] == needle[j]) j++;
        if (j == nl) return 1;
    }
    return 0;
}

static int looks_like_url(const char* s) {
    if (jagx_str_contains(s, ".com") || jagx_str_contains(s, ".org") ||
        jagx_str_contains(s, ".net") || jagx_str_contains(s, ".io") ||
        jagx_str_contains(s, "www.")) return 1;
    return 0;
}

void jagx_navigate_or_search(const char* input, struct jagx_nav_result* out) {
    if (!input || !out) return;

    /* Local knowledge: founder of JagX */
    if (jagx_str_contains(input, "founder") && jagx_str_contains(input, "jagx")) {
        out->kind = JAGX_NAV_LOCAL;
        out->title = "Founder of JagX";
        out->body =
            "Gbadamosi Tajudeen Olajide — Founder of the JagX Operating System. "
            "JagX is an original OS for PC and Mobile, built from scratch "
            "(no Linux/Android/iOS base), with its own browser (JagBrowser) "
            "and a strong security and mobile-first vision. "
            "Project: github.com/Tajudeen001-security/JagX-Operating-System";
        out->url = "https://github.com/Tajudeen001-security/JagX-Operating-System";
        return;
    }

    if (looks_like_url(input)) {
        out->kind = JAGX_NAV_URL;
        out->title = "Navigate";
        out->body =
            "URL navigation is designed. Live fetch requires TCP/IP + TLS + HTTP "
            "(not yet available in this kernel build). Target: open the site in real time.";
        out->url = input;
        return;
    }

    /* Generic search */
    out->kind = JAGX_NAV_SEARCH;
    out->title = "Web search";
    out->body =
        "Live web search is a primary goal of JagBrowser. "
        "It will use the system network stack once DNS/TCP/TLS/HTTP are ready. "
        "Today the stack is still under construction — no live results yet.";
    out->url = 0;
}
