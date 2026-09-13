#ifndef JAGX_SEARCH_H
#define JAGX_SEARCH_H

/* JagBrowser search / navigation interface */

typedef enum {
    JAGX_NAV_URL,       /* user typed something that looks like a URL */
    JAGX_NAV_SEARCH,    /* free-text search query */
    JAGX_NAV_LOCAL      /* answered from local project knowledge */
} jagx_nav_kind_t;

struct jagx_nav_result {
    jagx_nav_kind_t kind;
    const char* title;
    const char* body;       /* short text result or status message */
    const char* url;        /* if applicable */
};

/* Classify input and produce a result.
 * When networking is ready, SEARCH/URL paths will hit the real stack.
 * Today: local knowledge for founder query; honest status otherwise.
 */
void jagx_navigate_or_search(const char* input, struct jagx_nav_result* out);

#endif
