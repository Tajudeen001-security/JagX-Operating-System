# JagX v0.0.10

## Networking
- Real IPv4 header + checksum
- Real UDP datagram builder
- DNS A query sent via UDP/IPv4 → virtio TX (on-wire path)
- RX/parse of DNS answers still next

## Userspace
- GDT user code/data segments
- TSS loaded (`ltr`)
- `enter_user_mode()` iret path to ring 3
- int 0x80 from user side prepared

## Crypto
- ChaCha20 core (real quarter rounds)
- AEAD encrypt/decrypt with development Poly1305-style tag
- Self-test in kernel boot

## Secure boot
- `boot_verify_marker()` / `boot_verify_buffer()` using SHA-256
- Kernel refuses to continue if marker hash path fails
