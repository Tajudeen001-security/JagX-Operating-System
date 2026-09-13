# Social media on JagX

JagX does **not** run Facebook, WhatsApp, Instagram, or X as APKs.

## Native: JagCircle (`com.jagx.social`)

A first-party social app:

- Local/regional feed
- Compose + like
- No Google/Facebook account required
- Packaged as `social.jagx`

Source: `apps/social.c`.

Agencies can fork JagCircle into an **internal network** (civil service circulars, campus, cooperative) with no US cloud vendor.

## Visiting global social networks

JagBrowser (`com.jagx.browser`) is the native web engine. When TCP/TLS is up, mobile web versions of public sites can load **as websites**, not as Android apps.

Until the network stack is complete:

- Test Shell JagBrowser uses live Wikipedia (CORS) plus bookmarks
- Kernel JagBrowser uses `browser/search.c` (local knowledge + URL classification)

## Policy for governments

| Approach | Recommendation |
|----------|----------------|
| WhatsApp APK | Not supported — would reintroduce Android |
| JagCircle internal | Preferred for official comms |
| Web social in JagBrowser | Acceptable when TLS + policy allow |
| Unofficial APK sideload | Blocked by package manager magic `JAGX` |
