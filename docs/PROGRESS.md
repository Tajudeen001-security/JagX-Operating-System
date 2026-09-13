# JagX v0.0.11

## Networking
- Virtio **RX** path (`virtio_net_receive`)
- DNS response parser for A records
- `dns_resolve_a` polls RX after query
- Minimal **HTTP GET** builder + DNS dependency (TCP ESTABLISHED still required for full fetch)

## Userspace
- Mapped `user_stack[8192]`
- **`enter_user_mode()` is actually called** with entry + stack

## Crypto
- **Full Poly1305** limb implementation + ChaCha20 AEAD

## Remaining for live web pages
- Complete TCP handshake + payload
- TLS for HTTPS sites
- HTML renderer in compositor
