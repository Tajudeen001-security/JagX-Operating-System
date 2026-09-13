# Mobile Security Model (Early Ideas)

## Principles

- Least privilege by default
- Explicit, understandable user consent
- Strong isolation between apps
- Minimal trusted computing base
- No silent data collection

## Planned Direction

- Capability-based security (apps hold unforgeable tokens for resources)
- Permission groups that are actually meaningful to users
- Runtime revocation that works reliably
- Secure element / TEE integration where available (for keys, payments, etc.)
- Verified boot chain (when we reach hardware bring-up)
- Exploit mitigations: W^X, ASLR, stack protection, control-flow integrity where feasible

Mobile devices are high-value targets. Security is therefore a permanent design constraint, not a feature to add later.
