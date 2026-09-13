#ifndef JAGX_DNS_H
#define JAGX_DNS_H

#include <stdint.h>

uint32_t dns_build_query(const char* hostname, uint8_t* buf, uint32_t buf_len, uint16_t id);
int dns_resolve_a(const char* hostname, uint32_t* out_ipv4);
int dns_parse_a_response(const uint8_t* pkt, uint32_t len, uint16_t expect_id, uint32_t* out_ipv4);

#endif
