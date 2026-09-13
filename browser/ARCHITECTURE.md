# JagBrowser Architecture

JagBrowser is the **native** web browser and search interface for JagX.

## User goals (target behavior)

- Type a URL (`youtube.com`, `github.com`) → navigate there in real time
- Type a search (`who's elon musk`, `who is the founder of JagX`) → show results from the network
- Support both desktop and mobile layouts with the JagX design language

## Reality check (current stage of the OS)

A working live web browser requires:

1. Stable network driver (virtio-net rings complete)
2. TCP/IP + DNS
3. TLS (HTTPS)
4. HTTP client
5. HTML/CSS layout and rendering into the compositor
6. Optional JS engine

JagX does **not** yet have items 2–6. Therefore **live internet browsing is not available in the running kernel today**. This document and the code stubs define the path so that when networking matures, the browser can attach cleanly.

## Planned components

| Layer | Role |
|-------|------|
| Navigation bar / Search box | User enters URL or query |
| URL vs Search classifier | If looks like host → navigate; else → search |
| DNS + HTTP(S) client | Fetch resources (needs networking) |
| HTML parser + layout | Build render tree |
| Compositor surfaces | Draw pages as windows / tabs |
| Security | Per-origin capabilities, no ambient file/network rights |

## Local / offline answers (temporary)

Until the network stack can fetch live data, JagBrowser may answer a small set of **project-local** queries from in-tree data (e.g. founder of JagX). All other queries will report that network search is not yet available.

## Security model for the browser

- Each tab/origin runs with limited capabilities
- No silent access to camera, mic, filesystem, or arbitrary devices
- TLS required for sensitive navigation once networking exists
- Aligns with mobile-specific security (see `docs/MOBILE_SECURITY.md`)
