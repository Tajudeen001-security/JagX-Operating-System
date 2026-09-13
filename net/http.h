#ifndef JAGX_HTTP_H
#define JAGX_HTTP_H

#include <stdint.h>

/* Minimal HTTP/1.0 GET over raw TCP-ish path (needs working TCP later).
 * For now builds request and attempts send after DNS.
 */
int http_get(const char* host, const char* path, uint8_t* out, uint32_t out_len, uint32_t* out_got);

#endif
