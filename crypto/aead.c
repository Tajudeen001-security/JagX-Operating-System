#include "aead.h"
#include "../kernel/arch/x86_64/console.h"

int aead_encrypt(const uint8_t* key, size_t key_len,
                 const uint8_t* nonce, size_t nonce_len,
                 const uint8_t* aad, size_t aad_len,
                 const uint8_t* plain, size_t plain_len,
                 uint8_t* cipher, uint8_t* tag, size_t tag_len) {
    (void)key;(void)key_len;(void)nonce;(void)nonce_len;
    (void)aad;(void)aad_len;(void)plain;(void)plain_len;
    (void)cipher;(void)tag;(void)tag_len;
    /* Real ChaCha20-Poly1305 or AES-GCM body comes next;
     * interface is fixed for volume encryption + TLS. */
    return -1;
}

int aead_decrypt(const uint8_t* key, size_t key_len,
                 const uint8_t* nonce, size_t nonce_len,
                 const uint8_t* aad, size_t aad_len,
                 const uint8_t* cipher, size_t cipher_len,
                 const uint8_t* tag, size_t tag_len,
                 uint8_t* plain) {
    (void)key;(void)key_len;(void)nonce;(void)nonce_len;
    (void)aad;(void)aad_len;(void)cipher;(void)cipher_len;
    (void)tag;(void)tag_len;(void)plain;
    return -1;
}

void crypto_selftest_note(void) {
    console_write("[CRYPTO] SHA-256 available; AEAD interface reserved\n");
}
