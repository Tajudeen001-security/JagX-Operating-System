#ifndef JAGX_TLS_CRYPTO_H
#define JAGX_TLS_CRYPTO_H

#include <stdint.h>
#include <stddef.h>

/* TLS 1.2 PRF (SHA-256 based) scaffold for key expansion */
void tls_prf_sha256(const uint8_t* secret, size_t secret_len,
                    const char* label,
                    const uint8_t* seed, size_t seed_len,
                    uint8_t* out, size_t out_len);

void tls_key_block(const uint8_t master[48],
                   const uint8_t client_random[32],
                   const uint8_t server_random[32],
                   uint8_t* key_block, size_t key_block_len);

#endif
