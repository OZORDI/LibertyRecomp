# PPC Analysis: sub_821412B8 — Main Subsystem Init Chain

**Function**: `sub_821412B8` (68 sequential calls)
**Called from**: `sub_82140000` (game entry) → `sub_82120FB8` → here
**Source**: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.0.cpp`, line 2860
**Current hook**: `kernel/imports.cpp` line 1677 (diagnostic printf wrapper + INIT_PROBE binary search)

---

## Prologue (before call #1)

Before the first call, the function writes two zero bytes:
- `PPC_STORE_U8(0x82A74A84 - 0x82A74A84 + offset, 0)` — global flags cleared (from lis/stb pattern at lines 2879–2884).

---

## Complete Ordered Call List

| # | Address | Identification | File (recomp) | Hang Risk | INIT_PROBE |
|-|-|-|-|-|-|
| 1 | `__imp__XNotifyPositionUI` | XAM import: positions UI notification element; Xbox dashboard overlay | import | LOW | no |
| 2 | `sub_82302308` | Streaming pool bootstrap; calls sub_821B5xxx group (streaming pool init) + CRT | recomp.10.cpp | LOW | PROBE #2 "early init" |
| 3 | `sub_826CA440` | Platform SDK / engine mid-level; calls sub_8223C810 (game timer/clock) | recomp.39.cpp | LOW | PROBE #3 "engine mid-level" |
| 4 | `sub_82299500` | **Xenos GPU renderer pipeline init** (calls sub_82902AF8→sub_82852FB0 shader chain); **already stubbed** in imports.cpp | recomp.8.cpp | STUBBED | PROBE #4 "renderer init" |
| 5 | `sub_8284D220` (1st) | Platform locale/string table constructor; calls XDK sub_82A00DC0 + CRT sub_829FF840 | recomp.55.cpp | LOW | no |
| 6 | `sub_8284D220` (2nd) | Second locale/string table registration (different args: 0x82274038 + 0x822781A8) | recomp.55.cpp | LOW | no |
| 7 | `sub_821B4768` | World/scene graph init; calls GPU sub_828CACE8, render sub_8220xxxx, sub_822BCA90 barrier | recomp.2.cpp | LOW | PROBE #7 "player/controller" |
| 8 | `sub_822CE1C0` | Camera/viewport init; calls GPU sub_828BF740, sub_8284E060, sub_82148030 (object registry) | recomp.9.cpp | LOW | no |
| 9 | `sub_822BCA90` | **Sync barrier** (no-op stub — `return;`) | recomp.9.cpp | NONE | SYNC log |
| 10 | `sub_82214E00` | String/locale CRT table init; calls only CRT sub_829FFD48 + sub_829FFE18 | recomp.5.cpp | LOW | PROBE #10 "game systems" |
| 11 | `sub_82266EA8` | Physics system init; calls streaming pool sub_821B3510, physics/collision sub_82507368, draw sub_821D0488 | recomp.7.cpp | LOW | PROBE #11 "subsystem" |
| 12 | `sub_822BCA90` | Sync barrier | recomp.9.cpp | NONE | SYNC log |
| 13 | `sub_8233C480` | Streaming config init; r3=2000 stored as config value (not a sleep — direct writes to 0x82254xxx globals) | recomp.12.cpp | LOW | PROBE #13 "subsystem r3=2000" |
| 14 | `sub_821FBC20` | Secondary physics/collision init; same streaming+collision callee pattern as #11 | recomp.5.cpp | LOW | no |
| 15 | `sub_82140F38` | Animation + collision system init; calls sub_82280300 (anim), sub_82294250 (collision) | recomp.0.cpp | LOW | PROBE #15 "subsystem" |
| 16 | `sub_821D6140` | Render pipeline init; calls 0x825xxxxx (render) + 0x824xxxxx (spatial) + 0x822D (draw calls) | recomp.4.cpp | LOW | no |
| 17 | `sub_8232CA80` | Streaming resource manager init; calls sub_82329DC8, sub_821D0488, sub_8250D080 | recomp.12.cpp | LOW | no |
| 18 | `sub_821D0498` | Draw command buffer init; calls sub_822D30B8 (draw), sub_82514500 (render), sub_824E26E0 (spatial) | recomp.4.cpp | LOW | no |
| 19 | `sub_821CFF28` | Object pool constructor with count=50; direct memory writes only (r3=global ptr, r4=50) | recomp.3.cpp | LOW | no |
| 20 | `sub_821E34C0` | GPU resource + physics combined init; calls GPU sub_82832848, sub_82836680, physics sub_8247D7C8 | recomp.4.cpp | LOW | PROBE #20 "subsystem" |
| 21 | `sub_821EC2A0` | LOD/visibility system init; same core triad (stream pool + physics + draw calls) | recomp.4.cpp | LOW | no |
| 22 | `sub_821C13F0` | Small memory pool init; calls allocator sub_824FFF28 only | recomp.3.cpp | LOW | no |
| 23 | `sub_821F9F78` | Small allocator init; calls sub_824FFF28 only | recomp.4.cpp | LOW | no |
| 24 | `sub_822BCA90` | Sync barrier | recomp.9.cpp | NONE | SYNC log |
| 25 | `sub_82206BB8` | Entity/object manager init; streaming pool + physics + allocator | recomp.5.cpp | LOW | PROBE #25 "subsystem" |
| 26 | `sub_822BCA90` | Sync barrier | recomp.9.cpp | NONE | SYNC log |
| 27 | `sub_8223F458` | Locale/text system init; calls XDK sub_82A11FB0 (string) + CRT sub_829DB688 | recomp.6.cpp | LOW | PROBE #26 "between 25-35" |
| 28 | `sub_8223C848` | Flag/counter init; no sub_ callees, very small (29 lines) | recomp.6.cpp | LOW | PROBE #27 "between 25-35" |
| **29** | **`sub_821FC1F8`** | ***** CONFIRMED HANG ***** — Render device / world type registration; 40 internal calls all in 0x825x render range; internal binary search probes at sub_8251BA08 (call 1) through sub_825030B8 (call 45) | recomp.5.cpp | **HANG** | PROBE #28 "between 25-35" + 10 internal probes |
| 30 | `sub_822BCA90` | Sync barrier | recomp.9.cpp | NONE | SYNC log |
| 31 | `sub_822BCA90` | Sync barrier | recomp.9.cpp | NONE | SYNC log |
| 32 | `sub_82267948` | Streaming platform bridge init; calls sub_8236C2E0 (streaming) + **sub_82655CD0** (XAM-range — possible platform I/O) | recomp.7.cpp | MEDIUM (XAM call) | PROBE #31 "between 25-35" |
| 33 | `sub_822BCA90` | Sync barrier | recomp.9.cpp | NONE | SYNC log |
| 34 | `sub_822BCA90` | Sync barrier | recomp.9.cpp | NONE | SYNC log |
| 35 | `sub_822446A8` | Global flag reset; 13 lines, no callees | recomp.6.cpp | LOW | PROBE #35 "subsystem" |
| 36 | `sub_821434B0` | Script/mission system init; calls sub_8222E5F8 (game logic), GPU sub_828BDBE8, **sub_8265CB30** (XAM ×3) | recomp.0.cpp | MEDIUM (XAM calls) | no |
| 37 | `sub_822148A0` | Shader/material system init; calls 0x8253xxxx (render), sub_825EDF40 (shader), sub_821ED518 | recomp.5.cpp | LOW | no |
| 38 | `sub_82283E90` | GPU/EDRAM texture cache init; calls GPU sub_828D1CC0 + sub_828D1A50 | recomp.8.cpp | LOW | no |
| 39 | `sub_8214A4C8` | Draw call submission init; calls sub_82258EC0, sub_8214D588, sub_8224FA68, sub_82257BC0 | recomp.0.cpp | LOW | no |
| 40 | `sub_821C61D8` | Occlusion system init; calls sub_8239A5E8 (spatial), sub_8235C450 (transform), GPU sub_828C3F68 | recomp.3.cpp | LOW | PROBE #40 "subsystem" |
| 41 | `sub_82163A68` | Core object registry / RTTI init; calls sub_8235C370, sub_82146690, sub_82148030 | recomp.0.cpp | LOW | no |
| 42 | `sub_82163270` | String table / type-info registration; single callee sub_821C4B90 | recomp.0.cpp | LOW | no |
| 43 | `sub_821480A8` | Streaming object type registration; calls sub_8233xxxx (streaming), sub_8228xxxx (game objects) | recomp.0.cpp | LOW | no |
| 44 | `sub_8224DE10` | Config/globals init; 73 lines, direct memory writes only, no sub_ callees | recomp.7.cpp | LOW | no |
| 45 | `sub_822BCA90` | Sync barrier | recomp.9.cpp | NONE | SYNC log |
| 46 | `sub_82220FC8` | Small subsystem allocator; calls sub_824FFF28 only | recomp.6.cpp | LOW | PROBE #46 "subsystem" |
| 47 | `sub_822134D0` | Audio/sound system init; calls sub_82210AA8, CRT sub_829DB8D0, GPU sub_82849778 | recomp.5.cpp | LOW | no |
| 48 | `sub_822A0968` | Multiplayer/network system init; calls XAM sub_826714F0, sub_82675F28, sub_82673D48, sub_82674FF0, sub_826729B8 | recomp.8.cpp | MEDIUM (5 XAM calls) | no |
| 49 | `sub_822BCA90` | Sync barrier | recomp.9.cpp | NONE | SYNC log |
| 50 | `sub_822A1028` | File system / VFS I/O thread init; calls sub_827B×7 functions (file I/O — 0x827B range = multiple I/O thread setup) | recomp.8.cpp | LOW | PROBE #50 "subsystem" |
| 51 | `sub_822637D0` | Game profile/difficulty init; single callee sub_821F1268 | recomp.7.cpp | LOW | no |
| 52 | `sub_823A5328` | Streaming scheduler init; thin wrapper over sub_823A5158 | recomp.14.cpp | LOW | no |
| 53 | `sub_822BCA90` | Sync barrier | recomp.9.cpp | NONE | SYNC log |
| 54 | `sub_822BCA90` | Sync barrier | recomp.9.cpp | NONE | SYNC log |
| 55 | `sub_822653B0` | Platform achievements/stats init; single callee sub_82264270 (0x8226 = platform) | recomp.7.cpp | LOW | no |
| 56 | `sub_82204BA8` | Post-processing / effect system init; calls GPU sub_8284xxxx, shader sub_825Exxxx | recomp.5.cpp | LOW | no |
| 57 | `sub_8229B870` | Flag/counter init; 21 lines, no callees | recomp.8.cpp | LOW | no |
| 58 | `sub_822778D8` | Pointer/vtable setter; 11 lines, r3=global ptr param | recomp.7.cpp | LOW | no |
| 59 | `sub_82146A68` | Transform/matrix subsystem init; calls sub_8235C450, sub_8235C378 (transform), GPU sub_828C3F68 | recomp.0.cpp | LOW | PROBE #59 "late init" |
| 60 | `sub_829FFA48` | **Conditional** — CRT string/locale init; only called if bit flag at 0x82F4F864 not set | recomp.68.cpp | LOW | no |
| 61 | `sub_82854120` | GPU command buffer / render context registration; calls GPU sub_8284F0C0, sub_82855460 | recomp.55.cpp | LOW | no |
| 62 | `sub_828541E8` | GPU render target / surface registration; calls GPU sub_82855A18 | recomp.55.cpp | LOW | no |
| 63 | `sub_82227D50` | Map/world area allocator init; calls allocator sub_824FFF28 | recomp.6.cpp | LOW | PROBE #63 "late init" |
| 64 | `sub_822BCA90` | Sync barrier | recomp.9.cpp | NONE | SYNC log |
| 65 | `sub_82270558` | Platform HID/input system init; calls allocator sub_824FFF28 + platform sub_8226F9E0 | recomp.7.cpp | LOW | no |
| 66 | `sub_821D5768` | Asset streaming final init; calls allocator sub_824FFF28 + streaming pool sub_821B3560 | recomp.4.cpp | LOW | no |
| 67 | `sub_823227B0` | Streaming memory allocator init; calls allocator sub_824FFF28 | recomp.12.cpp | LOW | no |
| 68 | `sub_82329C90` | Second streaming allocator / FIFO queue init; calls allocator sub_824FFF28 | recomp.12.cpp | LOW | PROBE #68 "final init" |

---

## Hang Analysis: Call #29 — sub_821FC1F8

**Address**: `0x821FC1F8`
**Type**: World/render-type registration dispatcher
**Internal calls**: 40 sequential calls, all in `0x825xxxxx` (render) + `0x824xxxxx` (spatial) ranges
**Why it hangs**: One or more internal callees dispatches through the Xenos GPU device vtable (same family as `sub_8285B088` / `sub_8285A8B0`). The binary search in imports.cpp (lines 1735–1744) probes at calls 1, 5, 10, 15, 20, 25, 30, 35, 40, 45 to identify the specific blocking callee.

**Known related stubs already in place**: `sub_8285B088` (ShaderFinalise), `sub_8285A8B0` (GPU buffer flush), `sub_8285D018`, `sub_8285C648`, `sub_8285CF98`.

**Fix approach**: Identify the exact callee inside sub_821FC1F8 that reaches a GPU fence spin-wait and add a targeted stub. The internal probes will reveal which of the 40 calls is last observed before the hang.

---

## Hang Risk Assessment: Calls 30–68

No call in positions 30–68 directly references `sub_8285D948` or `sub_8285D610`.

Three calls make XAM-range (0x826xxxxx) subcalls:

| Call # | Function | XAM Callees | Risk |
|-|-|-|-|
| 32 | `sub_82267948` | `sub_82655CD0` (1×) | MEDIUM — streaming platform I/O, may touch STFS/VFS |
| 36 | `sub_821434B0` | `sub_8265CB30` (3×) | MEDIUM — script system, likely XContent query |
| 48 | `sub_822A0968` | `sub_826714F0`, `sub_82675F28`, `sub_82673D48`, `sub_82674FF0`, `sub_826729B8` (5×) | MEDIUM — multiplayer/session, XAM network init |

All other calls 30–68 call only game-side (0x821–0x824) or CRT/allocator (0x824FFF28) functions and are LOW risk.

---

## Existing INIT_PROBE Hooks in imports.cpp

From `kernel/imports.cpp` lines 1686–1744:

**Outer chain probes** (confirm which outer call completes before hang):

| PROBE | Function | Call # |
|-|-|-|
| #2 | `sub_82302308` | 2 |
| #3 | `sub_826CA440` | 3 |
| #4 | `sub_82299500` | 4 |
| #7 | `sub_821B4768` | 7 |
| #10 | `sub_82214E00` | 10 |
| #11 | `sub_82266EA8` | 11 |
| #13 | `sub_8233C480` | 13 |
| #15 | `sub_82140F38` | 15 |
| #20 | `sub_821E34C0` | 20 |
| #25 | `sub_82206BB8` | 25 |
| #26 | `sub_8223F458` | 27 |
| #27 | `sub_8223C848` | 28 |
| #28 | `sub_821FC1F8` | **29 (HANG)** |
| #31 | `sub_82267948` | 32 |
| #35 | `sub_822446A8` | 35 |
| #40 | `sub_821C61D8` | 40 |
| #46 | `sub_82220FC8` | 46 |
| #50 | `sub_822A1028` | 50 |
| #59 | `sub_82146A68` | 59 |
| #63 | `sub_82227D50` | 63 |
| #68 | `sub_82329C90` | 68 |

**Internal probes for sub_821FC1F8** (binary search within the hang function):

| PROBE | Internal callee | Internal call position |
|-|-|-|
| #2801 | `sub_8251BA08` | 1 |
| #2805 | `sub_82504318` | 5 |
| #2810 | `sub_82446BA8` | 10 |
| #2815 | `sub_82446DB0` | 15 |
| #2820 | `sub_8254A610` | 20 |
| #2825 | `sub_82163F38` | 25 |
| #2830 | `sub_82478AF8` | 30 |
| #2835 | `sub_822B2010` | 35 |
| #2840 | `sub_823A2108` | 40 |
| #2845 | `sub_825030B8` | 45 |

**sub_822BCA90** (sync barrier): full call log with caller LR for first 5 invocations, then every 10,000.

---

## Expected Init Sequence After Hang Fix

Once sub_821FC1F8 is unblocked (GPU fence spin-wait stubbed), the remaining 39 calls (30–68) should complete without hangs. The expected sequence:

1. Calls 30–34 (barriers + streaming platform bridge + more barriers)
2. Call 35: global flag reset
3. Calls 36–44: script/mission, shader/material, GPU texture cache, draw submission, occlusion, RTTI, streaming types, config globals
4. Calls 45–55: more barriers, allocators, audio, multiplayer, VFS I/O threads, profile, streaming scheduler
5. Calls 55–68: achievements, post-processing, transforms, conditional CRT, GPU registration, allocators, input, streaming finalizers

After the last call (#68, `sub_82329C90`), `sub_821412B8` returns to `sub_82120FB8` which then returns to the game's main loop entry.

---

## Notes

- `sub_822BCA90` is a **no-op stub** in the recomp (`blr` only) — represents an `lwsync` or `sync` instruction that needs no emulation.
- `sub_8284D220` is called **twice** with different arguments (calls 5 and 6) — both are locale/string table registration, different table entries.
- Call #4 (`sub_82299500`) is the Xenos GPU renderer pipeline and is already completely stubbed.
- The `sub_824FFF28` allocator appears as the sole callee in many small init functions — this is the RAGE pool/heap allocator.
- `sub_821B3510` + `sub_82507368` appearing together consistently signals physics+streaming subsystem pairing.
