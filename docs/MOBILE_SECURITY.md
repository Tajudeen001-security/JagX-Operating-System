# Mobile-Specific Security

JagX Mobile treats the phone as a high-value, always-with-you device.

## Rules

1. **Sensors, camera, microphone, location** require explicit, revocable capabilities.
2. **Background work** is limited and visible to the user.
3. **No mandatory cloud identity** to use the device.
4. **Notifications and app links** cannot silently escalate privilege.
5. **JagBrowser** on mobile uses the same capability model as desktop, with stricter defaults for device APIs.

## Permission UX

- Short, honest explanations
- Easy permanent revoke
- No dark patterns that push users into over-sharing

## Isolation

Apps should not read each other’s data by default.  
The capability system (see SECURITY.md) is the long-term mechanism.
