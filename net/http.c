#include "http.h"
#include "dns.h"
#include "tcp.h"
#include "../kernel/arch/x86_64/console.h"

static void append(char* dst, uint32_t* pos, uint32_t max, const char* s) {
    while (*s && *pos + 1 < max) dst[(*pos)++] = *s++;
    dst[*pos] = 0;
}

int http_get(const char* host, const char* path, uint8_t* out, uint32_t out_len, uint32_t* out_got) {
    if (!host || !path) return -1;
    uint32_t ip = 0;
    int dr = dns_resolve_a(host, &ip);
    if (dr != 0) {
        console_write("[HTTP] DNS failed — cannot GET\n");
        return -1;
    }

    char req[512];
    uint32_t pos = 0;
    append(req, &pos, sizeof(req), "GET ");
    append(req, &pos, sizeof(req), path);
    append(req, &pos, sizeof(req), " HTTP/1.0\r\nHost: ");
    append(req, &pos, sizeof(req), host);
    append(req, &pos, sizeof(req), "\r\nConnection: close\r\n\r\n");

    console_write("[HTTP] Request built for ");
    console_write(host);
    console_write("\n");

    struct tcp_pcb pcb;
    tcp_connect(&pcb, ip, 80);
    /* Full TCP handshake + payload TX still incomplete;
     * when ESTABLISHED, tcp_send(req) would go here. */
    (void)out; (void)out_len;
    if (out_got) *out_got = 0;

    console_write("[HTTP] Waiting on TCP ESTABLISHED path\n");
    return -2;
}
