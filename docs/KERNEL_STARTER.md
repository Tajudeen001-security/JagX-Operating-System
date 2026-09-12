# JagX Kernel Starter Documentation

## What was added in this update

### GDT (Global Descriptor Table)
- Null, Code, and Data segments
- Flat 4 GB model (typical for hobby kernels)
- `gdt_flush` reloads all segment registers

### IDT (Interrupt Descriptor Table)
- Full 256 entries
- Exception handlers (ISR 0-31) with human-readable messages
- IRQ handlers (32-47 after PIC remap)

### PIC (8259 Programmable Interrupt Controller)
- Remapped so IRQs start at vector 32 (avoids collision with CPU exceptions)
- EOI (End Of Interrupt) support
- Individual IRQ mask/unmask helpers

### Timer (PIT)
- Configured to 100 Hz
- Increments a tick counter
- Prints a `.` every second so you can see it is alive

### Keyboard
- Basic PS/2 scancode → ASCII translation (US layout)
- Echoes typed characters to the console

### Console
- 80×25 VGA text mode
- Scrolling, newline, backspace, tab
- Simple write / hex / decimal helpers

## How interrupts flow

1. Hardware raises IRQ → PIC → CPU
2. CPU looks up handler in IDT
3. Assembly stub (`irqN` / `isrN`) saves registers and calls C
4. C handler (`irq_handler` / `isr_handler`) does the work
5. EOI is sent to the PIC
6. `iret` restores state and returns

## Testing checklist

- [ ] Kernel boots under QEMU and shows the banner
- [ ] Dots appear roughly once per second
- [ ] Typing on the keyboard produces characters
- [ ] Backspace works
- [ ] No immediate triple-fault or exception

Enjoy building JagX!
