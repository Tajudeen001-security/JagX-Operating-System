# JagX v0.0.12

## TCP
- SYN emit with checksum
- SYN-ACK handling → ESTABLISHED + ACK
- Data send (PSH+ACK)
- Data recv path via virtio RX + tcp_input
- FIN/ACK close path

## HTTP
- `http_get`: DNS → TCP connect → wait ESTABLISHED → GET → read response bytes

## Docs
- Full README: install, ISO, QEMU, networking, secure boot, mobile, company guide
- SETUP_PC.md / SETUP_MOBILE.md
