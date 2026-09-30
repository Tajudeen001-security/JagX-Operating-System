# JagX OS v0.1.1

**Founder:** Gbadamosi Tajudeen Olajide · JagX & JRILICENSE  
https://github.com/Tajudeen001-security/JagX-Operating-System

## Boot flow

1. **Power on / boot splash** (logo + progress)
2. **Lock screen** — PIN / fingerprint (F1) / face (F2)
3. Desktop apps: Noder, Phone (Contacts+Dialer), Settings, Messages

### Lab keys

| Key | Action |
|-----|--------|
| PIN **1234** + Enter | Unlock |
| **F1** | Fingerprint try |
| **F2** | Face unlock try |
| **Ctrl+Shift+Q** | Shutdown screen |
| **Ctrl+Shift+R** | Restart screen |

```bash
cd kernel && make && make iso
qemu-system-i386 -cdrom ../boot/jagx.iso -m 256M -serial stdio
```
