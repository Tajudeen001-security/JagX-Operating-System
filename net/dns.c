#include "dns.h"
#include "udp.h"
#include "virtio_net.h"
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

/* Parse Ethernet+IP+UDP+DNS for first A record */
int dns_parse_a_response(const uint8_t* pkt, uint32_t len, uint16_t expect_id, uint32_t* out_ipv4) {
    if (!pkt || len < 14 + 20 + 8 + 12 || !out_ipv4) return -1;
    /* Skip Ethernet */
    const uint8_t* ip = pkt + 14;
    if (ip[0] != 0x45) return -1;
    if (ip[9] != 17) return -1; /* UDP */
    uint8_t ihl = (ip[0] & 0xF) * 4;
    const uint8_t* udp = ip + ihl;
    const uint8_t* dns = udp + 8;
    uint32_t dns_len = len - (uint32_t)(dns - pkt);
    if (dns_len < 12) return -1;

    uint16_t id = ((uint16_t)dns[0] << 8) | dns[1];
    if (id != expect_id) return -1;
    uint16_t ancount = ((uint16_t)dns[6] << 8) | dns[7];
    if (ancount == 0) return -1;

    /* Skip question */
    uint32_t pos = 12;
    while (pos < dns_len && dns[pos] != 0) {
        uint8_t l = dns[pos];
        if (l >= 0xC0) { pos += 2; break; }
        pos += l + 1;
    }
    if (pos < dns_len && dns[pos] == 0) pos++;
    pos += 4; /* qtype qclass */

    /* First answer */
    if (pos >= dns_len) return -1;
    if (dns[pos] >= 0xC0) pos += 2;
    else {
        while (pos < dns_len && dns[pos]) pos += dns[pos] + 1;
        pos++;
    }
    if (pos + 10 > dns_len) return -1;
    uint16_t atype = ((uint16_t)dns[pos] << 8) | dns[pos+1];
    pos += 8; /* type class ttl */
    uint16_t rdlen = ((uint16_t)dns[pos] << 8) | dns[pos+1];
    pos += 2;
    if (atype == 1 && rdlen == 4 && pos + 4 <= dns_len) {
        *out_ipv4 = ((uint32_t)dns[pos] << 24) | ((uint32_t)dns[pos+1] << 16) |
                    ((uint32_t)dns[pos+2] << 8) | dns[pos+3];
        return 0;
    }
    return -1;
}

int dns_resolve_a(const char* hostname, uint32_t* out_ipv4) {
    uint8_t q[512];
    uint16_t id = 0x4A47;
    uint32_t len = dns_build_query(hostname, q, sizeof(q), id);
    if (!len) return -1;

    console_write("[DNS] Query on wire: ");
    console_write(hostname);
    console_write("\n");

    uint32_t src = (10u << 24) | (0u << 16) | (2u << 8) | 15u;
    uint32_t dst = (8u << 24) | (8u << 16) | (8u << 8) | 8u;
    if (udp_send(src, 53000, dst, 53, q, len) != 0) return -1;

    /* Poll RX briefly for answer */
    uint8_t rx[1600];
    for (int tries = 0; tries < 50; tries++) {
        uint32_t n = virtio_net_receive(rx, sizeof(rx));
        if (n > 0) {
            uint32_t ip = 0;
            if (dns_parse_a_response(rx, n, id, &ip) == 0) {
                if (out_ipv4) *out_ipv4 = ip;
                console_write("[DNS] Resolved A record\n");
                return 0;
            }
        }
        for (volatile int d = 0; d < 100000; d++);
    }
    console_write("[DNS] No answer yet (RX/timeout)\n");
    if (out_ipv4) *out_ipv4 = 0;
    return -2;
}
