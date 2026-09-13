#include "tls.h"
#include "tcp.h"
#include "virtio_net.h"
#include "../kernel/arch/x86_64/console.h"

void tls_init(void) {
    console_write("[TLS] Handshake scaffold ready\n");
}

static uint32_t build_client_hello(uint8_t* out, uint32_t max, const char* sni) {
    if (max < 120) return 0;
    uint32_t p = 0;
    out[p++] = 22;
    out[p++] = 0x03; out[p++] = 0x01;
    uint32_t rec_len_at = p; p += 2;

    uint32_t hs_start = p;
    out[p++] = 1;
    uint32_t hs_len_at = p; p += 3;
    out[p++] = 0x03; out[p++] = 0x03;
    for (int i = 0; i < 32; i++) out[p++] = (uint8_t)(0xA0 + i);
    out[p++] = 0;
    out[p++] = 0x00; out[p++] = 0x04;
    out[p++] = 0x00; out[p++] = 0x2F;
    out[p++] = 0x00; out[p++] = 0x0A;
    out[p++] = 0x01; out[p++] = 0x00;

    uint32_t ext_len_at = p; p += 2;
    uint32_t ext_start = p;
    if (sni && sni[0]) {
        out[p++] = 0x00; out[p++] = 0x00;
        uint32_t sni_len_at = p; p += 2;
        uint32_t sni_start = p;
        out[p++] = 0x00; out[p++] = 0x00;
        uint32_t list_at = p - 2;
        out[p++] = 0x00;
        uint32_t host_len = 0;
        while (sni[host_len]) host_len++;
        out[p++] = (host_len >> 8) & 0xFF;
        out[p++] = host_len & 0xFF;
        for (uint32_t i = 0; i < host_len; i++) out[p++] = (uint8_t)sni[i];
        uint16_t list_len = (uint16_t)(p - (list_at + 2));
        out[list_at] = (list_len >> 8) & 0xFF;
        out[list_at+1] = list_len & 0xFF;
        uint16_t sni_len = (uint16_t)(p - sni_start);
        out[sni_len_at] = (sni_len >> 8) & 0xFF;
        out[sni_len_at+1] = sni_len & 0xFF;
    }
    uint16_t ext_len = (uint16_t)(p - ext_start);
    out[ext_len_at] = (ext_len >> 8) & 0xFF;
    out[ext_len_at+1] = ext_len & 0xFF;

    uint32_t hs_len = p - hs_start - 4;
    out[hs_len_at] = (hs_len >> 16) & 0xFF;
    out[hs_len_at+1] = (hs_len >> 8) & 0xFF;
    out[hs_len_at+2] = hs_len & 0xFF;
    uint16_t rec_len = (uint16_t)(p - 5);
    out[rec_len_at] = (rec_len >> 8) & 0xFF;
    out[rec_len_at+1] = rec_len & 0xFF;
    return p;
}

/* Parse first TLS record; detect ServerHello (type 2) */
static int tls_try_parse_server_hello(const uint8_t* data, uint32_t len) {
    if (len < 6) return -1;
    /* May be inside TCP stream after Ethernet/IP/UDP or raw TCP payload */
    for (uint32_t i = 0; i + 5 < len; i++) {
        if (data[i] == 22 && data[i+1] == 0x03 &&
            (data[i+2] == 0x01 || data[i+2] == 0x03 || data[i+2] == 0x03)) {
            if (i + 5 < len && data[i+5] == 2) {
                console_write("[TLS] ServerHello detected\n");
                return 0;
            }
        }
        if (data[i] == 22 && i + 5 < len && data[i+5] == 2) {
            console_write("[TLS] ServerHello detected\n");
            return 0;
        }
    }
    return -1;
}

int tls_client_hello(struct tls_conn* c, uint32_t ip, uint16_t port, const char* sni_host) {
    if (!c) return -1;
    c->remote_ip = ip;
    c->remote_port = port;
    c->state = TLS_CLIENT_HELLO_SENT;

    struct tcp_pcb pcb;
    if (tcp_connect(&pcb, ip, port) != 0) return -1;

    uint8_t frame[1600];
    for (int i = 0; i < 40; i++) {
        uint32_t n = virtio_net_receive(frame, sizeof(frame));
        if (n) tcp_input(frame, n);
        if (pcb.state == TCP_ESTABLISHED) break;
        for (volatile int d = 0; d < 80000; d++);
    }
    if (pcb.state != TCP_ESTABLISHED) {
        console_write("[TLS] TCP not established\n");
        return -2;
    }

    uint8_t hello[512];
    uint32_t hlen = build_client_hello(hello, sizeof(hello), sni_host);
    if (!hlen) return -3;
    if (tcp_send(&pcb, hello, hlen) != 0) return -4;
    console_write("[TLS] ClientHello sent\n");
    c->state = TLS_HANDSHAKING;

    for (int i = 0; i < 30; i++) {
        int r = tcp_recv(&pcb, frame, sizeof(frame));
        if (r > 0) {
            if (tls_try_parse_server_hello(frame, (uint32_t)r) == 0) {
                c->state = TLS_HANDSHAKING;
                console_write("[TLS] Handshake in progress (keys/certs next)\n");
                return 0;
            }
        }
        for (volatile int d = 0; d < 80000; d++);
    }
    console_write("[TLS] No ServerHello yet\n");
    return -5;
}

int tls_send_appdata(struct tls_conn* c, const uint8_t* data, uint32_t len) {
    if (!c || c->state != TLS_APP_DATA) return -1;
    (void)data; (void)len;
    return -1;
}
