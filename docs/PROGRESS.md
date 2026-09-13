# JagX v0.0.13

## Networking
- **ARP** table, request, reply learning; IPv4 uses ARP before send
- **DHCP** DISCOVER + offer parsing / QEMU default lease fallback
- **TLS** foundation: ClientHello builder + send over TCP (full handshake still open)

## Toward HTTPS
1. ~~ARP/DHCP~~
2. TLS ServerHello + key schedule + cert verify
3. Application data records for HTTPS GET
