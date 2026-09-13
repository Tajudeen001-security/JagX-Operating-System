#include "tls.h"
#include "tcp.h"
#include "virtio_net.h"
#include "../kernel/arch/x86_64/console.h"

void tls_init(void) {
    console_write("[TLS] Record parser + handshake state machine\n");
}

static uint32_t build_client_hello(uint8_t* out, uint32_t max, const char* sni) {
    if (max < 120) return 0;
    uint32_t p = 0;
    out[p++] = 22; out[p++] = 0x03; out[p++] = 0x01;
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
        uint32_t hl = 0; while (sni[hl]) hl++;
        out[p++] = (hl >> 8) & 0xFF; out[p++] = hl & 0xFF;
        for (uint32_t i = 0; i < hl; i++) out[p++] = (uint8_t)sni[i];
        uint16_t list_len = (uint16_t)(p - (list_at + 2));
        out[list_at] = (list_len >> 8) & 0xFF; out[list_at+1] = list_len & 0xFF;
        uint16_t sni_len = (uint16_t)(p - sni_start);
        out[sni_len_at] = (sni_len >> 8) & 0xFF; out[sni_len_at+1] = sni_len & 0xFF;
    }
    uint16_t ext_len = (uint16_t)(p - ext_start);
    out[ext_len_at] = (ext_len >> 8) & 0xFF; out[ext_len_at+1] = ext_len & 0xFF;
    uint32_t hs_len = p - hs_start - 4;
    out[hs_len_at] = (hs_len >> 16) & 0xFF;
    out[hs_len_at+1] = (hs_len >> 8) & 0xFF;
    out[hs_len_at+2] = hs_len & 0xFF;
    uint16_t rec_len = (uint16_t)(p - 5);
    out[rec_len_at] = (rec_len >> 8) & 0xFF; out[rec_len_at+1] = rec_len & 0xFF;
    return p;
}

int tls_process_records(struct tls_conn* c, const uint8_t* data, uint32_t len) {
    if (!c || !data || len < 5) return -1;
    uint32_t off = 0;
    while (off + 5 <= len) {
        uint8_t ctype = data[off];
        uint16_t rlen = ((uint16_t)data[off+3] << 8) | data[off+4];
        if (off + 5 + rlen > len) break;
        const uint8_t* body = data + off + 5;

        if (ctype == 22 && rlen >= 4) { /* Handshake */
            uint8_t htype = body[0];
            if (htype == 2) { /* ServerHello */
                c->got_server_hello = 1;
                c->state = TLS_SERVER_HELLO_RCVD;
                if (rlen >= 38) {
                    for (int i = 0; i < 32; i++) c->server_random[i] = body[6 + i];
                    uint8_t sid_len = body[38];
                    uint32_t cs_at = 39 + sid_len;
                    if (cs_at + 1 < rlen)
                        c->cipher_suite = ((uint16_t)body[cs_at] << 8) | body[cs_at+1];
                }
                console_write("[TLS] ServerHello parsed\n");
            } else if (htype == 11) { /* Certificate */
                c->got_certificate = 1;
                console_write("[TLS] Certificate message seen (verify TBD)\n");
            } else if (htype == 14) {
                console_write("[TLS] ServerHelloDone\n");
                c->state = TLS_HANDSHAKING;
            }
        } else if (ctype == 20) {
            console_write("[TLS] ChangeCipherSpec\n");
        } else if (ctype == 21) {
            console_write("[TLS] Alert\n");
        } else if (ctype == 23) {
            console_write("[TLS] Application data (needs keys)\n");
        }
        off += 5 + rlen;
    }
    return 0;
}

int tls_client_hello(struct tls_conn* c, uint32_t ip, uint16_t port, const char* sni_host) {
    if (!c) return -1;
    c->remote_ip = ip;
    c->remote_port = port;
    c->state = TLS_CLIENT_HELLO_SENT;
    c->got_server_hello = 0;
    c->got_certificate = 0;
    c->cipher_suite = 0;

    struct tcp_pcb pcb;
    if (tcp_connect(&pcb, ip, port) != 0) return -1;

    uint8_t frame[1600];
    for (int i = 0; i < 40; i++) {
        uint32_t n = virtio_net_receive(frame, sizeof(frame));
        if (n) tcp_input(frame, n);
        if (pcb.state == TCP_ESTABLISHED) break;
        for (volatile int d = 0; d < 80000; d++);
    }
    if (pcb.state != TCP_ESTABLISHED) return -2;

    uint8_t hello[512];
    uint32_t hlen = build_client_hello(hello, sizeof(hello), sni_host);
    if (!hlen || tcp_send(&pcb, hello, hlen) != 0) return -3;
    console_write("[TLS] ClientHello sent\n");

    for (int i = 0; i < 30; i++) {
        int r = tcp_recv(&pcb, frame, sizeof(frame));
        if (r > 0) {
            tls_process_records(c, frame, (uint32_t)r);
            if (c->got_server_hello) return 0;
        }
        for (volatile int d = 0; d < 80000; d++);
    }
    return c->got_server_hello ? 0 : -5;
}

int tls_send_appdata(struct tls_conn* c, const uint8_t* data, uint32_t len) {
    if (!c || c->state != TLS_APP_DATA) return -1;
    (void)data; (void)len;
    return -1; /* needs traffic keys */
}
