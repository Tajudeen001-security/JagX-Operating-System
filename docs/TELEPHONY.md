# Calls and SMS on JagX

JagX includes a **native Phone and Messages stack**. It is not Google Dialer, not an APK.

## Apps

| App | Package | Source |
|-----|---------|--------|
| Phone | `com.jagx.phone` | `apps/phone.c` |
| Messages | `com.jagx.messages` | `apps/messages.c` |
| Contacts | `com.jagx.contacts` | `apps/contacts.c` |

## HAL

`mobile/hal/radio.h` is the modem contract:

- SIM ready / signal
- Mobile data on/off
- Airplane mode
- `sms_send` / `sms_poll`
- `call_dial` / `call_answer` / `call_hangup`

Device ports **register** `struct jagx_radio_ops`. Until a baseband driver exists, the HAL returns stub errors and the Phone UI can still run a **lab call session** so the shell can be tested.

## Real calls on a phone

Need:

1. Unlocked device + JagX BSP
2. Vendor modem firmware (legal redistribution)
3. A JagX RIL talking to that firmware
4. Audio path (earpiece, mic, speaker, BT)

This is the same reason non-Android phone OSes struggle on Tecno/Itel: the modem is proprietary.

## Testing without a modem

- Test Shell: full dialer, in-call timer, mute, contacts, SMS threads
- QEMU: Phone app draws; serial logs `[PHONE] Dial …`

## Emergency calls

Not certified. Do not rely on JagX for emergency services until a national lab signs off RF + audio + SIM.
