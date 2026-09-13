#include "tls_crypto.h"
#include "../crypto/sha256.h"

static void hmac_sha256(const uint8_t* key, size_t key_len,
                        const uint8_t* msg, size_t msg_len,
                        uint8_t out[32]) {
    uint8_t k[64];
    for (int i = 0; i < 64; i++) k[i] = 0;
    if (key_len > 64) sha256(key, key_len, k);
    else for (size_t i = 0; i < key_len; i++) k[i] = key[i];
    uint8_t ipad[64], opad[64];
    for (int i = 0; i < 64; i++) { ipad[i] = k[i] ^ 0x36; opad[i] = k[i] ^ 0x5c; }
    uint8_t inner[64 + 256];
    for (int i = 0; i < 64; i++) inner[i] = ipad[i];
    for (size_t i = 0; i < msg_len && i < 256; i++) inner[64 + i] = msg[i];
    uint8_t ih[32];
    sha256(inner, 64 + (msg_len < 256 ? msg_len : 256), ih);
    uint8_t outer[96];
    for (int i = 0; i < 64; i++) outer[i] = opad[i];
    for (int i = 0; i < 32; i++) outer[64 + i] = ih[i];
    sha256(outer, 96, out);
}

void tls_prf_sha256(const uint8_t* secret, size_t secret_len,
                    const char* label,
                    const uint8_t* seed, size_t seed_len,
                    uint8_t* out, size_t out_len) {
    uint8_t a[32];
    uint8_t seed_full[128];
    size_t label_len = 0;
    while (label[label_len]) label_len++;
    size_t sl = 0;
    for (size_t i = 0; i < label_len && sl < 128; i++) seed_full[sl++] = (uint8_t)label[i];
    for (size_t i = 0; i < seed_len && sl < 128; i++) seed_full[sl++] = seed[i];
    hmac_sha256(secret, secret_len, seed_full, sl, a);
    size_t produced = 0;
    while (produced < out_len) {
        uint8_t buf[160];
        size_t bp = 0;
        for (int i = 0; i < 32; i++) buf[bp++] = a[i];
        for (size_t i = 0; i < sl; i++) buf[bp++] = seed_full[i];
        uint8_t block[32];
        hmac_sha256(secret, secret_len, buf, bp, block);
        for (int i = 0; i < 32 && produced < out_len; i++) out[produced++] = block[i];
        hmac_sha256(secret, secret_len, a, 32, a);
    }
}

void tls_master_secret(const uint8_t* pms, size_t pms_len,
                       const uint8_t client_random[32],
                       const uint8_t server_random[32],
                       uint8_t master[48]) {
    uint8_t seed[64];
    for (int i = 0; i < 32; i++) seed[i] = client_random[i];
    for (int i = 0; i < 32; i++) seed[32 + i] = server_random[i];
    tls_prf_sha256(pms, pms_len, "master secret", seed, 64, master, 48);
}

void tls_key_block(const uint8_t master[48],
                   const uint8_t client_random[32],
                   const uint8_t server_random[32],
                   uint8_t* key_block, size_t key_block_len) {
    uint8_t seed[64];
    for (int i = 0; i < 32; i++) seed[i] = server_random[i];
    for (int i = 0; i < 32; i++) seed[32 + i] = client_random[i];
    tls_prf_sha256(master, 48, "key expansion", seed, 64, key_block, key_block_len);
}
