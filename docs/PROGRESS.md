# JagX Progress - v0.0.5

## Implemented in this version

### Graphics (Multiboot2 path)
- Switched boot header to Multiboot2
- Request 1024×768×32 framebuffer tag
- Parse Multiboot2 tags and activate linear framebuffer when present
- Real `fb_putpixel` / `fb_fill_rect` / `fb_clear`
- Demo drawing of teal + purple rectangles

### Compositor
- Windows are now rendered as colored rectangles with title bars when FB is active

### aarch64 / Mobile
- Exception vector table installed
- Basic exception stubs
- MMU preparation point reached

### Networking
- Stub interface + loopback concept added (`net/`)

## Notes

- Under plain `qemu -kernel` the Multiboot2 framebuffer tag may not always be supplied. Using GRUB or a Multiboot2-aware loader gives the best graphical result.
- Text mode remains the reliable fallback.
