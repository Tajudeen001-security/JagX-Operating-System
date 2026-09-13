/* ChaCha20 + full Poly1305 (RFC 8439 style) */
#include "aead.h"
#include <stddef.h>

static uint32_t rotl32(uint32_t x, int n) { return (x << n) | (x >> (32 - n)); }

static void quarter(uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d) {
    *a += *b; *d ^= *a; *d = rotl32(*d, 16);
    *c += *d; *b ^= *c; *b = rotl32(*b, 12);
    *a += *b; *d ^= *a; *d = rotl32(*d, 8);
    *c += *d; *b ^= *c; *b = rotl32(*b, 7);
}

static void chacha20_block(uint32_t out[16], const uint32_t in[16]) {
    for (int i = 0; i < 16; i++) out[i] = in[i];
    for (int i = 0; i < 10; i++) {
        quarter(&out[0], &out[4], &out[8], &out[12]);
        quarter(&out[1], &out[5], &out[9], &out[13]);
        quarter(&out[2], &out[6], &out[10], &out[14]);
        quarter(&out[3], &out[7], &out[11], &out[15]);
        quarter(&out[0], &out[5], &out[10], &out[15]);
        quarter(&out[1], &out[6], &out[11], &out[12]);
        quarter(&out[2], &out[7], &out[8], &out[13]);
        quarter(&out[3], &out[4], &out[9], &out[14]);
    }
    for (int i = 0; i < 16; i++) out[i] += in[i];
}

static void chacha20_xor(const uint8_t key[32], const uint8_t nonce[12],
                         uint32_t counter, const uint8_t* in, uint8_t* out, size_t len) {
    uint32_t state[16] = {
        0x61707865, 0x3320646e, 0x79622d32, 0x6b206574
    };
    for (int i = 0; i < 8; i++)
        state[4+i] = (uint32_t)key[i*4] | ((uint32_t)key[i*4+1]<<8) |
                     ((uint32_t)key[i*4+2]<<16) | ((uint32_t)key[i*4+3]<<24);
    state[12] = counter;
    state[13] = (uint32_t)nonce[0] | ((uint32_t)nonce[1]<<8) |
                ((uint32_t)nonce[2]<<16) | ((uint32_t)nonce[3]<<24);
    state[14] = (uint32_t)nonce[4] | ((uint32_t)nonce[5]<<8) |
                ((uint32_t)nonce[6]<<16) | ((uint32_t)nonce[7]<<24);
    state[15] = (uint32_t)nonce[8] | ((uint32_t)nonce[9]<<8) |
                ((uint32_t)nonce[10]<<16) | ((uint32_t)nonce[11]<<24);

    size_t off = 0;
    while (off < len) {
        uint32_t block[16];
        chacha20_block(block, state);
        state[12]++;
        uint8_t* kb = (uint8_t*)block;
        size_t n = len - off; if (n > 64) n = 64;
        for (size_t i = 0; i < n; i++) out[off+i] = in[off+i] ^ kb[i];
        off += n;
    }
}

/* Full Poly1305 (26-bit limb style) */
static void poly1305_mac(const uint8_t key[32], const uint8_t* m, size_t mlen,
                         const uint8_t* aad, size_t aad_len, uint8_t tag[16]) {
    uint32_t r0, r1, r2, r3, r4;
    uint32_t h0 = 0, h1 = 0, h2 = 0, h3 = 0, h4 = 0;
    uint32_t t0 = (uint32_t)key[0] | ((uint32_t)key[1]<<8) | ((uint32_t)key[2]<<16) | ((uint32_t)key[3]<<24);
    uint32_t t1 = (uint32_t)key[4] | ((uint32_t)key[5]<<8) | ((uint32_t)key[6]<<16) | ((uint32_t)key[7]<<24);
    uint32_t t2 = (uint32_t)key[8] | ((uint32_t)key[9]<<8) | ((uint32_t)key[10]<<16) | ((uint32_t)key[11]<<24);
    uint32_t t3 = (uint32_t)key[12] | ((uint32_t)key[13]<<8) | ((uint32_t)key[14]<<16) | ((uint32_t)key[15]<<24);
    r0 = t0 & 0x3ffffff; t0 >>= 26; t0 |= t1 << 6;
    r1 = t0 & 0x3ffff03; t1 >>= 20; t1 |= t2 << 12;
    r2 = t1 & 0x3ffc0ff; t2 >>= 14; t2 |= t3 << 18;
    r3 = t2 & 0x3f03fff; t3 >>= 8;
    r4 = t3 & 0x00fffff;

    uint8_t block[16];
    size_t total = aad_len + mlen;
    /* Process AAD then message in 16-byte blocks with padding bit */
    const uint8_t* parts[2] = { aad, m };
    size_t lens[2] = { aad_len, mlen };
    for (int part = 0; part < 2; part++) {
        const uint8_t* p = parts[part];
        size_t left = lens[part];
        while (left > 0) {
            size_t n = left < 16 ? left : 16;
            for (size_t i = 0; i < 16; i++) block[i] = 0;
            for (size_t i = 0; i < n; i++) block[i] = p[i];
            if (n < 16) block[n] = 1;
            else {
                /* full block: add 2^128 */
            }
            uint32_t b0 = (uint32_t)block[0] | ((uint32_t)block[1]<<8) | ((uint32_t)block[2]<<16) | ((uint32_t)block[3]<<24);
            uint32_t b1 = (uint32_t)block[4] | ((uint32_t)block[5]<<8) | ((uint32_t)block[6]<<16) | ((uint32_t)block[7]<<24);
            uint32_t b2 = (uint32_t)block[8] | ((uint32_t)block[9]<<8) | ((uint32_t)block[10]<<16) | ((uint32_t)block[11]<<24);
            uint32_t b3 = (uint32_t)block[12] | ((uint32_t)block[13]<<8) | ((uint32_t)block[14]<<16) | ((uint32_t)block[15]<<24);
            h0 += b0 & 0x3ffffff;
            h1 += (uint32_t)((b0 >> 26) | (b1 << 6)) & 0x3ffffff;
            h2 += (uint32_t)((b1 >> 20) | (b2 << 12)) & 0x3ffffff;
            h3 += (uint32_t)((b2 >> 14) | (b3 << 18)) & 0x3ffffff;
            h4 += (uint32_t)(b3 >> 8) | (n == 16 ? (1u << 24) : 0);

            uint64_t s0 = (uint64_t)h0*r0 + (uint64_t)h1*5*r4 + (uint64_t)h2*5*r3 + (uint64_t)h3*5*r2 + (uint64_t)h4*5*r1;
            uint64_t s1 = (uint64_t)h0*r1 + (uint64_t)h1*r0 + (uint64_t)h2*5*r4 + (uint64_t)h3*5*r3 + (uint64_t)h4*5*r2;
            uint64_t s2 = (uint64_t)h0*r2 + (uint64_t)h1*r1 + (uint64_t)h2*r0 + (uint64_t)h3*5*r4 + (uint64_t)h4*5*r3;
            uint64_t s3 = (uint64_t)h0*r3 + (uint64_t)h1*r2 + (uint64_t)h2*r1 + (uint64_t)h3*r0 + (uint64_t)h4*5*r4;
            uint64_t s4 = (uint64_t)h0*r4 + (uint64_t)h1*r3 + (uint64_t)h2*r2 + (uint64_t)h3*r1 + (uint64_t)h4*r0;
            h0 = (uint32_t)s0 & 0x3ffffff; s1 += s0 >> 26;
            h1 = (uint32_t)s1 & 0x3ffffff; s2 += s1 >> 26;
            h2 = (uint32_t)s2 & 0x3ffffff; s3 += s2 >> 26;
            h3 = (uint32_t)s3 & 0x3ffffff; s4 += s3 >> 26;
            h4 = (uint32_t)s4 & 0x3ffffff; h0 += (uint32_t)(s4 >> 26) * 5;
            h1 += h0 >> 26; h0 &= 0x3ffffff;

            p += n; left -= n;
        }
    }
    (void)total;
    uint32_t g0 = h0 + 5; uint32_t c = g0 >> 26; g0 &= 0x3ffffff;
    uint32_t g1 = h1 + c; c = g1 >> 26; g1 &= 0x3ffffff;
    uint32_t g2 = h2 + c; c = g2 >> 26; g2 &= 0x3ffffff;
    uint32_t g3 = h3 + c; c = g3 >> 26; g3 &= 0x3ffffff;
    uint32_t g4 = h4 + c - (1u << 26);
    uint32_t mask = (g4 >> 31) - 1;
    g0 &= mask; g1 &= mask; g2 &= mask; g3 &= mask; g4 &= mask;
    mask = ~mask;
    h0 = (h0 & mask) | g0; h1 = (h1 & mask) | g1; h2 = (h2 & mask) | g2;
    h3 = (h3 & mask) | g3; h4 = (h4 & mask) | g4;

    uint32_t s0 = (uint32_t)key[16] | ((uint32_t)key[17]<<8) | ((uint32_t)key[18]<<16) | ((uint32_t)key[19]<<24);
    uint32_t s1 = (uint32_t)key[20] | ((uint32_t)key[21]<<8) | ((uint32_t)key[22]<<16) | ((uint32_t)key[23]<<24);
    uint32_t s2 = (uint32_t)key[24] | ((uint32_t)key[25]<<8) | ((uint32_t)key[26]<<16) | ((uint32_t)key[27]<<24);
    uint32_t s3 = (uint32_t)key[28] | ((uint32_t)key[29]<<8) | ((uint32_t)key[30]<<16) | ((uint32_t)key[31]<<24);

    uint64_t f0 = (uint64_t)h0 | ((uint64_t)h1 << 26);
    uint64_t f1 = ((uint64_t)h1 >> 6) | ((uint64_t)h2 << 20);
    uint64_t f2 = ((uint64_t)h2 >> 12) | ((uint64_t)h3 << 14);
    uint64_t f3 = ((uint64_t)h3 >> 18) | ((uint64_t)h4 << 8);
    f0 += s0; f1 += s1 + (f0 >> 32); f2 += s2 + (f1 >> 32); f3 += s3 + (f2 >> 32);
    uint32_t o0 = (uint32_t)f0, o1 = (uint32_t)f1, o2 = (uint32_t)f2, o3 = (uint32_t)f3;
    tag[0]=o0; tag[1]=o0>>8; tag[2]=o0>>16; tag[3]=o0>>24;
    tag[4]=o1; tag[5]=o1>>8; tag[6]=o1>>16; tag[7]=o1>>24;
    tag[8]=o2; tag[9]=o2>>8; tag[10]=o2>>16; tag[11]=o2>>24;
    tag[12]=o3; tag[13]=o3>>8; tag[14]=o3>>16; tag[15]=o3>>24;
}

int aead_encrypt(const uint8_t* key, size_t key_len,
                 const uint8_t* nonce, size_t nonce_len,
                 const uint8_t* aad, size_t aad_len,
                 const uint8_t* plain, size_t plain_len,
                 uint8_t* cipher, uint8_t* tag, size_t tag_len) {
    if (key_len != 32 || nonce_len != 12 || tag_len < 16) return -1;
    uint8_t poly_key[32];
    uint8_t zero[64] = {0};
    chacha20_xor(key, nonce, 0, zero, poly_key, 32);
    chacha20_xor(key, nonce, 1, plain, cipher, plain_len);
    poly1305_mac(poly_key, cipher, plain_len, aad ? aad : zero, aad_len, tag);
    return 0;
}

int aead_decrypt(const uint8_t* key, size_t key_len,
                 const uint8_t* nonce, size_t nonce_len,
                 const uint8_t* aad, size_t aad_len,
                 const uint8_t* cipher, size_t cipher_len,
                 const uint8_t* tag, size_t tag_len,
                 uint8_t* plain) {
    if (key_len != 32 || nonce_len != 12 || tag_len < 16) return -1;
    uint8_t poly_key[32], expect[16], zero[1] = {0};
    uint8_t z[64] = {0};
    chacha20_xor(key, nonce, 0, z, poly_key, 32);
    poly1305_mac(poly_key, cipher, cipher_len, aad ? aad : zero, aad_len, expect);
    uint8_t diff = 0;
    for (int i = 0; i < 16; i++) diff |= expect[i] ^ tag[i];
    if (diff) return -1;
    chacha20_xor(key, nonce, 1, cipher, plain, cipher_len);
    return 0;
}
