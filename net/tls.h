#ifndef JAGX_TLS_H
#define JAGX_TLS_H

#include <stdint.h>

enum tls_state {
    TLS_CLOSED = 0,
    TLS_CLIENT_HELLO_SENT,
    TLS_SERVER_HELLO_RCVD,
    TLS_HANDSHAKING,
    TLS_APP_DATA
};

struct tls_conn {
    enum tls_state state;
    uint16_t remote_port;
    uint32_t remote_ip;
    uint8_t  client_random[32];
    uint8_t  server_random[32];
    uint8_t  master_secret[48];
    uint8_t  key_block[128];
    uint16_t cipher_suite;
    int      got_server_hello;
    int      got_certificate;
    int      has_master;
};

void tls_init(void);
int  tls_client_hello(struct tls_conn* c, uint32_t ip, uint16_t port, const char* sni_host);
int  tls_process_records(struct tls_conn* c, const uint8_t* data, uint32_t len);
int  tls_derive_master_from_pms(struct tls_conn* c, const uint8_t* pms, uint32_t pms_len);
int  tls_send_appdata(struct tls_conn* c, const uint8_t* data, uint32_t len);

#endif
