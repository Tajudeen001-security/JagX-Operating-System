#ifndef JAGX_TLS_H
#define JAGX_TLS_H

#include <stdint.h>

/* TLS foundation for HTTPS — records + ClientHello scaffold.
 * Full handshake (cert verify, key exchange) is the next crypto milestone.
 */

enum tls_state {
    TLS_CLOSED = 0,
    TLS_CLIENT_HELLO_SENT,
    TLS_HANDSHAKING,
    TLS_APP_DATA
};

struct tls_conn {
    enum tls_state state;
    uint16_t remote_port;
    uint32_t remote_ip;
};

void tls_init(void);
int  tls_client_hello(struct tls_conn* c, uint32_t ip, uint16_t port, const char* sni_host);
int  tls_send_appdata(struct tls_conn* c, const uint8_t* data, uint32_t len);

#endif
