#ifndef JAGX_DNS_H
#define JAGX_DNS_H

#include <stdint.h>

/* Build a DNS query packet (A record). Returns length or 0 on failure. */
uint32_t dns_build_query(const char* hostname, uint8_t* buf, uint32_t buf_len, uint16_t id);

/* Very early resolver status — full UDP send needs IP stack */
int dns_resolve_a(const char* hostname, uint32_t* out_ipv4);

#endif
