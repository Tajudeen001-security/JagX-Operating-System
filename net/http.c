#include "http.h"
#include "dns.h"
#include "tcp.h"
#include "virtio_net.h"
#include "../kernel/arch/x86_64/console.h"

static void append(char* dst, uint32_t* pos, uint32_t max, const char* s) {
    while (*s && *pos + 1 < max) dst[(*pos)++] = *s++;
    dst[*pos] = 0;
}

int http_get(const char* host, const char* path, uint8_t* out, uint32_t out_len, uint32_t* out_got) {
    if (!host || !path) return -1;
    if (out_got) *out_got = 0;

    uint32_t ip = 0;
    if (dns_resolve_a(host, &ip) != 0) {
        console_write("[HTTP] DNS failed\n");
        return -1;
    }

    struct tcp_pcb pcb;
    if (tcp_connect(&pcb, ip, 80) != 0) {
        console_write("[HTTP] SYN failed\n");
        return -1;
    }

    /* Wait for ESTABLISHED via RX */
    uint8_t frame[1600];
    for (int i = 0; i < 40; i++) {
        uint32_t n = virtio_net_receive(frame, sizeof(frame));
        if (n) tcp_input(frame, n);
        if (pcb.state == TCP_ESTABLISHED) break;
        for (volatile int d = 0; d < 100000; d++);
    }

    if (pcb.state != TCP_ESTABLISHED) {
        console_write("[HTTP] TCP not established (no SYN-ACK)\n");
        return -2;
    }

    char req[512];
    uint32_t pos = 0;
    append(req, &pos, sizeof(req), "GET ");
    append(req, &pos, sizeof(req), path);
    append(req, &pos, sizeof(req), " HTTP/1.0\r\nHost: ");
    append(req, &pos, sizeof(req), host);
    append(req, &pos, sizeof(req), "\r\nUser-Agent: JagBrowser/0.0.12\r\nConnection: close\r\n\r\n");

    if (tcp_send(&pcb, req, pos) != 0) {
        console_write("[HTTP] Send failed\n");
        return -3;
    }

    uint32_t total = 0;
    if (out && out_len) {
        for (int i = 0; i < 20 && total < out_len; i++) {
            int r = tcp_recv(&pcb, out + total, out_len - total);
            if (r > 0) total += (uint32_t)r;
            else break;
        }
    }
    if (out_got) *out_got = total;

    tcp_close(&pcb);
    console_write("[HTTP] Done, bytes=");
    console_write_dec(total);
    console_write("\n");
    return total > 0 ? 0 : -4;
}
