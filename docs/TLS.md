# TLS / HTTPS on JagX

## Status

- TCP path can carry bytes
- `tls_client_hello()` builds a minimal TLS 1.2 ClientHello (with optional SNI) and sends it
- **Not yet:** ServerHello parse, certificate verification, ECDHE/RSA key exchange, Finished, application_data encryption

## Next implementation order

1. Parse ServerHello + Certificate
2. Verify cert chain (or TOFU dev mode)
3. Key exchange + traffic keys (prefer TLS 1.3 later)
4. Encrypt HTTP GET as TLS application_data
5. Wire into JagBrowser as `https://`

Until then, HTTP on port 80 is the testable path; HTTPS is scaffolded only.
