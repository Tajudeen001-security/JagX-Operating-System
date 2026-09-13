#ifndef JAGX_AEAD_H
#define JAGX_AEAD_H

#include <stdint.h>
#include <stddef.h>

/* AEAD interface — implementations fill in (ChaCha20-Poly1305 / AES-GCM) */

int aead_encrypt(const uint8_t* key, size_t key_len,
                 const uint8_t* nonce, size_t nonce_len,
                 const uint8_t* aad, size_t aad_len,
                 const uint8_t* plain, size_t plain_len,
                 uint8_t* cipher, uint8_t* tag, size_t tag_len);

int aead_decrypt(const uint8_t* key, size_t key_len,
                 const uint8_t* nonce, size_t nonce_len,
                 const uint8_t* aad, size_t aad_len,
                 const uint8_t* cipher, size_t cipher_len,
                 const uint8_t* tag, size_t tag_len,
                 uint8_t* plain);

#endif
