# JagX v0.0.8

## Web / JagBrowser

- Architecture for URL navigation + web search documented honestly
- `jagx_navigate_or_search()` API added
- Local answer for “founder of JagX” → Gbadamosi Tajudeen Olajide + project link
- Live youtube.com / Google-scale search **requires** unfinished networking (TCP/TLS/HTTP) — not claimed as working yet

## Security & system

- ENCRYPTION.md — algorithms and policy direction
- SECURE_BOOT.md — verification chain notes
- MOBILE_SECURITY.md — phone-specific rules
- Userspace directory + init stub
- Founder attribution file (FOUNDER.md)

## Still required for real web browsing

1. Complete virtio-net RX/TX rings
2. DNS + TCP
3. TLS 1.3
4. HTTP client
5. HTML renderer into compositor
