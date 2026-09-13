# JagX OS v0.0.24

**Founder:** Gbadamosi Tajudeen Olajide · JagX & JRILICENSE  
https://github.com/Tajudeen001-security/JagX-Operating-System

## Code on JagX (no Windows required)

**Native Noder IDE** runs **on JagX OS**:

- Multi-tab editor, keyboard input, Ctrl+S save, explorer UI
- Workspace files under `/workspace/`
- Package: `noder.jagx`

Guide: [docs/NODER_JAGX.md](docs/NODER_JAGX.md)

Full desktop Noder (Windows): https://github.com/JagX-JRILICENSE/Noder

## Run

```bash
cd kernel && make && make iso
qemu-system-i386 -cdrom ../boot/jagx.iso -m 128M -serial stdio
```

Type in the Noder window. **Ctrl+S** saves. **Ctrl+1–4** switches tabs.
