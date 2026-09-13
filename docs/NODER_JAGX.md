# Code on JagX OS (native Noder)

You do **not** need Windows to write code on JagX.

## Two products, same brand

| Product | Where | Status |
|---------|--------|--------|
| **Noder (Electron)** | Windows / Mac / Linux | Full IDE — [JagX-JRILICENSE/Noder](https://github.com/JagX-JRILICENSE/Noder) |
| **Noder (native)** | **JagX OS** | Built into the OS — `apps/noder.c` + `noder.jagx` |

## Features on JagX (v0.0.24)

- Multi-tab buffers (up to 4)
- Keyboard typing into the editor when focused
- Backspace, Enter, Esc (unfocus)
- **Ctrl+S** — save current file to RamFS (`/workspace/...`)
- **Ctrl+1..4** — switch tab
- Open / new file API
- Explorer sidebar + tab bar + status bar UI
- Language hint by extension (`.c` `.h` `.py` `.js` `.md`)
- Default files: `main.c`, `readme.md`

## How to use in QEMU

1. Build and boot JagX ISO
2. Unlock lock screen if shown
3. Noder window is focused — type on the keyboard
4. Ctrl+S saves to RamFS
5. Files appear under `/workspace/` in the file manager / RamFS list

## Roadmap (still to grow toward desktop Noder)

- Font glyph rendering (real characters on FB, not line bars only)
- Full terminal panel inside Noder
- Git operations
- AI panel (optional)
- Larger buffers / more tabs

Native Noder is the path for **coding on the JagX operating system itself**.
