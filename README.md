# JagX Operating System

**JagX** is an original, open-source operating system designed for both **PC** and **Mobile** devices.

It is **not** based on Linux, Android, iOS, Windows, or any existing operating system.  
We are building our own kernel, drivers, userland, and applications from the ground up.

## Vision

- Fully independent kernel written primarily in C and Assembly
- Dual target: desktop (x86_64 + ARM64) and mobile (ARM64)
- Own graphical user interface (or TUI first)
- **Own web browser** (JagBrowser) built into the system
- Secure by design, modern, and lightweight
- Completely free and open source

## Current Status

This is the **very beginning**. The repository currently contains:

- Project structure
- Vision & roadmap
- Minimal bootloader + kernel skeleton (x86)
- Placeholder for the JagBrowser
- Documentation

Building a real OS is a multi-year effort. Contributions, ideas, and serious developers are welcome.

## Repository Structure

```
JagX-Operating-System/
├── boot/               # Bootloaders (BIOS/UEFI, mobile boot)
├── kernel/             # Core kernel (architecture independent + arch specific)
│   ├── arch/
│   │   ├── x86_64/
│   │   └── aarch64/
│   ├── core/           # Scheduler, memory management, etc.
│   └── drivers/
├── browser/            # JagBrowser – native web browser for JagX
├── userland/           # Future userspace programs & libraries
├── docs/               # Design documents, architecture, roadmap
├── tools/              # Build tools, cross compilers helpers, QEMU scripts
├── LICENSE
└── README.md
```

## Building (Very Early)

Currently only a minimal x86 kernel skeleton exists.  
You will need a cross-compiler (`x86_64-elf-gcc`) and QEMU.

```bash
# Example (will be improved)
cd kernel
make
qemu-system-x86_64 -kernel jagx.kernel
```

Detailed build instructions will be added as the code matures.

## Roadmap (High Level)

### Phase 0 – Foundation (Current)
- [x] Repository & structure
- [ ] Minimal bootable kernel (x86)
- [ ] Basic memory management
- [ ] Interrupt handling

### Phase 1 – Core Kernel
- Process/thread management
- Virtual memory
- Device drivers (basic)
- Filesystem (simple)

### Phase 2 – Desktop & Mobile Foundations
- Graphical subsystem
- Input (keyboard/mouse/touch)
- Windowing system
- Mobile-specific power & touch stack

### Phase 3 – User Experience
- JagBrowser (own web browser)
- App framework
- Settings, file manager, etc.

### Phase 4 – Production Ready
- Security model
- Networking
- Package system
- Stable releases for PC and Mobile

## Contributing

This is a serious long-term project.  
If you know low-level systems programming (C, Assembly, OS development), feel free to open issues or pull requests.

Please read the docs/ folder before contributing large changes.

## License

JagX is released under the **MIT License** (see LICENSE file).

---

**Made with ambition by the community.**  
Let's build something truly original.
