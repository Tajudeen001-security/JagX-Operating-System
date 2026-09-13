# JagX Kernel v0.0.3

## Current Features

- GDT + IDT + PIC + Timer + Keyboard (Shift/Caps)
- Physical Memory Manager + simple Heap
- Basic Paging
- RamFS
- Syscall dispatcher stub
- **Early Framebuffer layer** (preparation for graphical boot & UI)

## Design Direction

We are building toward the dark + teal/purple premium look you liked:

- Graphical bootscreen
- Modern mobile home & multitasking
- Clean settings, notifications, file manager
- Own browser (JagBrowser)

Text mode is still the current runtime console.  
The framebuffer code is the bridge to the visual OS.

## Build

```bash
cd kernel
make
make run
```
