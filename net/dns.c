#include "dns.h"
#include "../kernel/arch/x86_64/console.h"

static uint32_t jagx_strlen(const char* s) {
    uint32_t n = 0; while (s[n]) n++; return n;
}

uint32_t dns_build_query(const char* hostname, uint8_t* buf, uint32_t buf_len, uint16_t id) {
    if (!hostname || !buf || buf_len < 64) return 0;

    uint32_t pos = 0;
    /* Header */
    buf[pos++] = (id >> 8) & 0xFF; buf[pos++] = id & 0xFF;
    buf[pos++] = 0x01; buf[pos++] = 0x00; /* RD */
    buf[pos++] = 0x00; buf[pos++] = 0x01; /* QDCOUNT = 1 */
    buf[pos++] = 0x00; buf[pos++] = 0x00;
    buf[pos++] = 0x00; buf[pos++] = 0x00;
    buf[pos++] = 0x00; buf[pos++] = 0x00;

    /* QNAME */
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
    /* QTYPE A = 1, QCLASS IN = 1 */
    buf[pos++] = 0x00; buf[pos++] = 0x01;
    buf[pos++] = 0x00; buf[pos++] = 0x01;
    return pos;
}

int dns_resolve_a(const char* hostname, uint32_t* out_ipv4) {
    uint8_t q[512];
    uint32_t len = dns_build_query(hostname, q, sizeof(q), 0x1234);
    if (!len) return -1;

    console_write("[DNS] Query built for: ");
    console_write(hostname);
    console_write(" (");
    console_write_dec(len);
    console_write(" bytes). UDP/IP send pending full IP stack.\n");

    /* When UDP/IP exists: send to 8.8.8.8:53 and parse answer */
    if (out_ipv4) *out_ipv4 = 0;
    return -2; /* not sent yet */
}
