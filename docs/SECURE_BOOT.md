# Secure Boot — JagX

## Development signing flow

```bash
cd kernel && make
cd ../boot
bash sign-kernel.sh ../kernel/jagx.kernel .
```

Produces:
- `jagx.kernel.sha256` — raw hash (also computable in-kernel via SHA-256)
- `jagx.kernel.sig` + `dev-signing.pub` — openssl detached signature (dev only)

## Runtime verification (direction)

1. Bootloader or early kernel computes SHA-256 of the loaded image
2. Compare to expected hash or verify signature with embedded public key
3. On failure: halt with clear message (no silent untrusted boot)

## Production

- Replace dev RSA with proper key hierarchy / platform keys
- Prefer hardware root of trust on mobile when available
- Keep recovery path so devices are not bricked during bring-up
