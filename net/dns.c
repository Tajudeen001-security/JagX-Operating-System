#include "dns.h"
#include "udp.h"
#include "../kernel/arch/x86_64/console.h"

uint32_t dns_build_query(const char* hostname, uint8_t* buf, uint32_t buf_len, uint16_t id) {
    if (!hostname || !buf || buf_len < 64) return 0;
    uint32_t pos = 0;
    buf[pos++] = (id >> 8) & 0xFF; buf[pos++] = id & 0xFF;
    buf[pos++] = 0x01; buf[pos++] = 0x00;
    buf[pos++] = 0x00; buf[pos++] = 0x01;
    buf[pos++] = 0x00; buf[pos++] = 0x00;
    buf[pos++] = 0x00; buf[pos++] = 0x00;
    buf[pos++] = 0x00; buf[pos++] = 0x00;

    const char* p = hostname;
    while (*p) {
        const char* dot = p;
        while (*dot && *dot != '.') dot++;
        uint8_t lablen = (uint8_t)(dot - p);
        if (lablen == 0 || pos + lablen + 1 >= buf_len) return 0;
        buf[pos++] = lablen;
        for (uint8_t i = 0; i < lablen; i++) buf[pos++] = (uint8_t)p[i];
        p = *dot ? dot + 1 : dot;
    }
    buf[pos++] = 0;
    buf[pos++] = 0x00; buf[pos++] = 0x01;
    buf[pos++] = 0x00; buf[pos++] = 0x01;
    return pos;
}

int dns_resolve_a(const char* hostname, uint32_t* out_ipv4) {
    uint8_t q[512];
    uint32_t len = dns_build_query(hostname, q, sizeof(q), 0x4A47);
    if (!len) return -1;

    console_write("[DNS] On-wire query -> 8.8.8.8:53 for ");
    console_write(hostname);
    console_write("\n");

    uint32_t src = (10u << 24) | (0u << 16) | (2u << 8) | 15u;
    uint32_t dst = (8u << 24) | (8u << 16) | (8u << 8) | 8u;

    int rc = udp_send(src, 53000, dst, 53, q, len);
    if (out_ipv4) *out_ipv4 = 0;
    return rc;
}
