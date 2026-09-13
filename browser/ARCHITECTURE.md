# JagBrowser Architecture (Early)

JagBrowser is the **native** web browser for JagX. It will not be a port of Chromium or Firefox.

## High-level Goals

- Fast and lightweight
- Strong integration with JagX security model (capabilities)
- Excellent mobile experience (touch scrolling, reading mode, low power)
- Clean, modern UI matching the JagX design system

## Major Components (Planned)

1. **Network stack interface** – HTTP/HTTPS (will use system networking later)
2. **HTML parser** – Start simple, grow carefully
3. **CSS engine** – Layout and styling
4. **Rendering backend** – Talks to the JagX compositor / graphics
5. **JavaScript engine** – Decision pending (embed a small engine vs write subset)
6. **UI chrome** – Address bar, tabs, bookmarks, matching design language

## Security Direction

- Each origin / tab runs with limited capabilities
- No unnecessary access to the rest of the system
- Clear permission prompts that match JagX mobile privacy goals

## Current Status

Architecture document only. Implementation begins after the compositor and networking foundations exist.
