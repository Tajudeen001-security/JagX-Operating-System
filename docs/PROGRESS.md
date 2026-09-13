# JagX v0.0.9

## Networking
- Virtio-net: real virtqueue allocation (desc/avail/used), PFN register, TX notify path
- DNS: real query packet builder (A record)
- TCP: PCB + SYN-SENT state machine stub
- Live URLs still need IPv4/UDP glue + RX path completion

## Userspace
- Process table + create
- int 0x80 gate at DPL=3 + dispatcher
- Full ring-3 switch still needs TSS + per-process address spaces

## Crypto
- **SHA-256 full implementation** (used for secure boot hashing)
- AEAD encrypt/decrypt API reserved for volume crypto / TLS

## Secure boot
- `boot/sign-kernel.sh` — hash + optional openssl detached sign/verify
