# Encryption in JagX

## Goals

- Protect data at rest and in transit
- Prefer modern, audited algorithms
- Keep cryptographic policy visible and under user/system control

## Planned use cases

| Area | Approach |
|------|----------|
| Disk / filesystem | Optional volume encryption (AES-GCM or XChaCha20-Poly1305 style AEAD) |
| Network (HTTPS) | TLS 1.3 once TCP stack exists |
| Secure boot measurements | Hash chain (SHA-256/SHA-3 family) |
| Capabilities / tokens | MAC or signature so tokens are unforgeable |
| Password handling | Memory-hard KDF (e.g. Argon2 direction) — never store plaintext |

## Principles

- No home-grown ciphers for production crypto
- Keys never logged; minimize time in RAM
- Clear separation: kernel provides primitives; policy lives in well-defined services
- Mobile: hardware-backed keys when a TEE/secure element is available

## Current status

Design only. Implementation begins after basic userspace and storage are stable enough to host keys safely.
