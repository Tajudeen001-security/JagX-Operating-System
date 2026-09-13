# Noder on JagX

## Upstream product (desktop)

**https://github.com/JagX-JRILICENSE/Noder**

- Real-time collaborative IDE by **JagX & JRILICENSE**
- Electron + Vite; builds Windows/macOS/Linux via GitHub Actions
- Workflows: [Actions](https://github.com/JagX-JRILICENSE/Noder/actions)
  - **Build All Platforms**
  - **Build Windows App**
- Latest line: v0.8.0 (frameless chrome, language detect, AI agent, GitHub clone/push, …)

That repo is the **full Noder for Windows/laptop**. It does **not** run as an Android APK inside JagX OS.

## On JagX OS

| Item | Value |
|------|--------|
| Package | `com.jagx.noder` → **`noder.jagx`** |
| Code | `apps/noder.c` |
| Role | Native editor surface + package identity until a full port of Electron Noder exists |

Use desktop Noder today on Windows from the upstream repo. Use `noder.jagx` on JagX OS package manager.
