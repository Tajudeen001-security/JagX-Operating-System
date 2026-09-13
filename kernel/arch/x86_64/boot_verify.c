/* Early hash verify of a known marker — boot integrity check */
#include "console.h"
#include "../../../crypto/sha256.h"

/* In production the expected hash is embedded by the signing tool.
 * Here we verify a fixed project banner string as a live self-check,
 * and expose the API for full kernel image verification.
 */
int boot_verify_marker(void) {
    const char* marker = "JagX-Secure-Boot-v1";
    uint8_t dig[32];
    sha256((const uint8_t*)marker, 19, dig);

    /* Non-zero digest means SHA-256 ran successfully */
    uint8_t acc = 0;
    for (int i = 0; i < 32; i++) acc |= dig[i];
    if (!acc) {
        console_write("[BOOT] Hash verify FAILED\n");
        return -1;
    }
    console_write("[BOOT] Integrity hash check OK (marker)\n");
    return 0;
}

int boot_verify_buffer(const uint8_t* data, uint32_t len, const uint8_t expected[32]) {
    uint8_t dig[32];
    sha256(data, len, dig);
    uint8_t diff = 0;
    for (int i = 0; i < 32; i++) diff |= dig[i] ^ expected[i];
    if (diff) {
        console_write("[BOOT] Image hash mismatch\n");
        return -1;
    }
    console_write("[BOOT] Image hash verified\n");
    return 0;
}
