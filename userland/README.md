# Userspace (early)

JagX will load userspace processes after the kernel is stable enough.

## Planned first steps

1. Simple process structure (pid, address space, capabilities)
2. ELF loader (or a minimal custom format at first)
3. `init` process
4. System call transition from kernel stubs to real user mode

## Current status

Directory and roadmap only. Kernel still runs everything in kernel mode.  
Real ring-3 / EL0 processes are a major milestone after memory isolation is solid.
