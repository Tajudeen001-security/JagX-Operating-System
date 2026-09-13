# JagX for government evaluation (Nigeria & Africa)

## Status — honest

JagX is an **early but sovereign operating system**. It is **not** yet ready to replace Windows/Linux desktops in ministries as a production daily driver.

Governments can **evaluate**, pilot in labs, and fund localization — starting **now** — without claiming full national rollout readiness.

Why it matters: Android and Windows are foreign stacks. JagX is original, MIT-licensed, and inspectable. Apps are `.jagx`, not Play Store APKs — no Google account required for an internal app catalog.

## Productivity and communication (v0.1.0)

| App | Similar to | Purpose |
|-----|------------|--------|
| **Notepad** | Windows Notepad | Memos, circulars |
| **Paint** | MS Paint | Diagrams |
| **JagSheet** | Excel | Tables, budgets, column sums |
| **JagBase** | Access | Staff / dept / state records |
| **Phone / Messages** | Dialer / SMS | Native telephony (HAL; real modem needs BSP) |
| **JagCircle** | Internal social | Agency feed without Facebook |
| **JagBrowser** | Chrome | Native web client |
| **Noder** | VS Code | Code on JagX itself |
| **JagStore** | Play Store | **`.jagx` only** |

## Suggested pilot path (Nigeria / AU members)

1. **Lab pilot** — QEMU / Test Shell / dedicated PCs, no critical data
2. **Training** — IT staff build/run from source (see README)
3. **Requirements** — drivers, bilingual UI (EN/Hausa/Yoruba/Igbo/French)
4. **Security review** — `docs/SECURITY.md`, secure boot path
5. **Phased hardware** — one ministry department, offline-first
6. **One phone BSP** — pick a high-volume Tecno/Infinix/Itel, fund port

## Contact / project

- Founder: **Gbadamosi Tajudeen Olajide**
- Repo: https://github.com/Tajudeen001-security/JagX-Operating-System
- License: MIT (public-sector forks welcome)

## What governments should not do yet

- Deploy as sole OS on national ID / payroll production systems
- Expect Android phone replacement without a funded BSP
- Assume Microsoft-file compatibility (`.docx` / `.xlsx` import not built)
- Fastboot-flash phones without a stock restore image

## What is available for demos today

```bash
cd kernel && make && make iso
qemu-system-i386 -cdrom ../boot/jagx.iso -m 256M -serial stdio
```

Or the JagX Test Shell (interactive phone + PC) for non-technical stakeholders.
