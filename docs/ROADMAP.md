# JagX Roadmap

## Phase 0 – Foundation (Done / In Progress)
- [x] Project structure & vision
- [x] x86 bootable kernel with interrupts, memory, basic FS, syscalls
- [x] Strong mobile vision and architecture documents
- [ ] aarch64 kernel skeleton boots under QEMU

## Phase 1 – Core Kernel Maturity
- Proper Multiboot / device tree memory maps
- Better physical & virtual memory management
- Process/thread model
- Basic framebuffer graphics
- Early input stack (keyboard + future touch)

## Phase 2 – Mobile Foundations
- aarch64 support solid
- Display & compositor design
- Touch and gesture pipeline (low latency focus)
- Power management framework (CPU idle, frequency scaling, display, radios)
- Sensor HAL design
- Mobile-specific security policy

## Phase 3 – User Experience
- Windowing / compositor (shared PC + Mobile concepts)
- JagX Design Language (mobile-first, scales to desktop)
- Core apps (launcher, settings, notifications, keyboard)
- JagBrowser (native engine)

## Phase 4 – Excellence & Polish
- Performance and latency tuning (especially mobile)
- Battery life optimization
- Privacy features and permission UX
- App sandboxing & capability system
- Stable releases for both PC and Mobile

The mobile experience is treated as a first-class goal throughout every phase — never an afterthought.
