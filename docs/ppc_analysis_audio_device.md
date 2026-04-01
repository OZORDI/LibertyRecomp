# PPC Analysis: Audio Device Initialization Functions

**Parent**: `sub_82478AF8` — see `ppc_analysis_sub_82478AF8.md` for the full call graph.
**Focus**: Functions called during audio device/manager construction:
`sub_8294BD68`, `sub_8294BE28`, `sub_8294BE20`, `sub_8294BEA0`, `sub_8293EA08`, `sub_8284D220`

---

## Availability in Decompiled Output

None of these six functions appear in the Hex-Rays pseudo-code (`default (1).xex.c`). They fall in the
gaps between functions IDA successfully decompiled:

| Address | Gap surroundings |
|-|-|
| `sub_8294BD68` | Between `sub_8294B8C8` and `sub_8294BE20` |
| `sub_8294BE28` | Between `sub_8294BE20` and `sub_8294BEE8` |
| `sub_8294BEA0` | Between `sub_8294BE20` and `sub_8294BEE8` (same gap) |
| `sub_8293EA08` | Between `sub_8293E900` and `sub_8293EAA8` |
| `sub_8284D220` | Entire `sub_8284xxxx` mid-range absent from pseudo-code |

All are present in recompiled form in the generated `.cpp` files, but those files use obfuscated
address placeholders (`n`), so they cannot be cross-referenced by address alone.

The nearest decompiled neighbor, `sub_8294BE20` (pseudo-code line 2879815), is a VMX-heavy
function performing reciprocal-square-root on a float4 (audio distance/attenuation calculation)
that ultimately calls `sub_82966670`.

---

## Role in sub_82478AF8 Call Sequence

From the existing analysis of `sub_82478AF8` (Phase 2–4):

```
Phase 2 (line ~83772):
  sub_821B3510(176)       -> allocate 176-byte audio manager struct
  sub_8294BD68(alloc)     -> construct audio manager (if alloc != NULL)
  global[0x831CC944] = r30  (audio manager pointer)

Phase 4 (line ~83894):
  sub_8294BE28(audio_mgr, stack_structs, stack)  -> set listener position
  sub_8294BE20(audio_mgr, 6000.0f)               -> set audio budget / distance scale
  sub_8294BEA0(audio_mgr, 1500.0f)               -> set a second audio parameter
```

---

## sub_8294BD68 — Audio Manager Constructor

**Signature (inferred)**: `void sub_8294BD68(_DWORD *a1)`
**Role**: Constructs the 176-byte audio manager object at `a1`.

Based on the pattern of `sub_8293EAA8` (the destructor for the same struct, decompiled at
pseudo-code line 2867284), the audio manager layout is:

### Audio Manager Struct Layout (176 bytes)

| Offset | Size | Type | Description |
|-|-|-|-|
| 0 | 4 | ptr | vtable → `off_820959B4` (set by destructor, inferred same vtable in ctor) |
| 128 | 4 | ptr | Array of sub-device pointers (freed in destructor) |
| 132 | 2 | u16 | Sub-device count (word) |
| 134 | 2 | u16 | Flags or secondary count |
| 136 | 4 | ptr | Sub-buffer A (freed in destructor) |
| 140 | 4 | ptr | Sub-buffer B (freed in destructor) |
| 144 | 4 | ptr | Sub-buffer C |
| 148 | 4 | ptr | Sub-buffer D |
| 152 | 4 | ptr | Sub-buffer E |
| 156 | 4 | ptr | Sub-buffer F |
| 160 | 4 | ptr | Sub-buffer G |
| 164 | 4 | ptr | Sub-buffer H |
| 168 | 4 | ptr | Sub-buffer I |
| 172 | 4 | ptr | Sub-buffer J |
| 176 | 4 | ptr | Sub-buffer K |
| 184 | 4 | i32 | Sub-device active count (checked against `dword_83130670`) |

The destructor (`sub_8293EAA8`) iterates `a1[184]` sub-devices stored in the pointer array at
`a1[128]`, calling `(**vtable)(obj, 1)` on each (vtable slot 0 — shutdown/release), then frees
all sub-buffers and calls `sub_828CA2F0` (likely placement-delete or parent destructor).

The constructor `sub_8294BD68` is the mirror: it zeros all pointer fields, sets the vtable, and
calls `sub_8293EA08` for hardware device enumeration.

---

## sub_8293EA08 — Audio Device Enumeration / Hardware Init

**Signature (inferred)**: `int sub_8293EA08(_DWORD *a1)`
**Role**: Called by the audio manager constructor to enumerate and initialize Xbox 360 audio render
devices.

This function falls between `sub_8293E900` (a 24-parameter audio voice submit function) and
`sub_8293EAA8` (the manager destructor). On Xbox 360 it drives:

1. **XAudioRenderDriverInitialize** — registered in `xboxkrnl_audio.cpp` as a stub
   (`XBOXKRNL_EXPORT_STUB(__imp__XAudioRenderDriverInitialize)`). The game calls this to bind
   the hardware render driver. In LibertyRecomp this is a no-op.

2. **XAudioRegisterRenderDriverClient** — maps to `AudioSystem::RegisterClient()`:
   - Allocates a client slot (up to 8 clients)
   - Creates the `AudioDriver` (SDL-backed on macOS/Linux, `SDLAudioDriver`)
   - Returns a synthetic driver handle: `0x41550000 | index`
   - Stored at `audio_mgr + 24` (confirmed from `sub_82194708` generated code, field `r31+24`)

3. **XAudioGetSpeakerConfig** — returns `0x00010001` (stereo, `XAUDIO_SPEAKER_STEREO`)

---

## sub_8294BE28, sub_8294BE20, sub_8294BEA0 — Audio Parameter Setters

These three functions write into the audio manager struct at offsets determined by IDA's analysis
of the VMX-using `sub_8294BE20` (which is decompiled):

### sub_8294BE28 — Set Listener Position
**Signature**: `void sub_8294BE28(_DWORD *mgr, float *position_struct, void *stack)`
Stores the 3D listener position (float4 via VMX `lvx128`/`stvx128`) into the audio manager.
Internally calls `sub_82966670` with the manager's vtable pointer at `*mgr[0]`.

### sub_8294BE20 — Set Audio Budget / Distance Scale
**Signature**: `BOOL sub_8294BE20(int mgr, float *_R4, int a3, int a4, int a5)`
Confirmed decompiled (pseudo-code line 2879816). Performs VMX reciprocal-square-root on a float4
representing audio distance parameters, then calls `sub_82966670(*(_DWORD**)mgr, ...)`.
Called with `r4 = 6000.0f` (max audible distance, in GTA IV world units).
Fields accessed: `mgr+24` (f32 distance scale), `mgr+32` (float4 near params), `mgr+48` (float4
far params).

### sub_8294BEA0 — Set Secondary Distance Parameter
**Signature** (inferred): `void sub_8294BEA0(int mgr, float param)`
Called with `r4 = 1500.0f` immediately after `sub_8294BE20`. Likely sets a secondary rolloff
distance (possibly the "near" clamp distance, since 1500 < 6000).

---

## sub_8284D220 — Audio Format Descriptor Builder

**Signature (inferred)**: `void sub_8284D220(void *out_struct, void *callback_ptr, int zero, void *stack_buf, int four)`
**Called**: 7 times in Phase 7, then 7 more times in Phase 12 of `sub_82478AF8`.

Builds a streaming audio format descriptor (likely an `XAUDIO2_BUFFER` or GTA IV internal
equivalent). Parameters:
- `r3` = destination struct on stack
- `r4` = callback function pointer (for XAudio2 voice callback)
- `r5` = 0 (flags)
- `r6` = stack scratch buffer
- `r7` = 4 (format stride or channel count)

Output structs are committed to the audio device via `sub_829530B0`.

---

## Kernel APIs Called (XAudio)

All XAudio kernel exports are implemented in
`glue/rexglue-sdk-main/src/kernel/xboxkrnl/xboxkrnl_audio.cpp`:

| Import address | Symbol | Status | Host implementation |
|-|-|-|-|
| `0x82A75804` | `XAudioRegisterRenderDriverClient` | Active | `AudioSystem::RegisterClient()` → `SDLAudioDriver` |
| `0x82A757F4` | `XAudioUnregisterRenderDriverClient` | Active | `AudioSystem::UnregisterClient()` |
| `0x82A757E4` | `XAudioSubmitRenderDriverFrame` | Active | `AudioSystem::SubmitFrame()` |
| `0x82A74DB4` | `XAudioGetSpeakerConfig` | Active | Returns `0x00010001` (stereo) |
| `0x82A75774` | `XAudioGetVoiceCategoryVolumeChangeMask` | Active | Returns `0` |
| `0x82A75794` | `XAudioGetVoiceCategoryVolume` | Active | Returns `1.0f` |
| N/A | `XAudioRenderDriverInitialize` | **Stub** | No-op |
| N/A | `XAudioRenderDriverLock` | **Stub** | No-op |
| N/A | `XAudioQueryDriverPerformance` | **Stub** | No-op |
| N/A | `XAudioGetRenderDriverThread` | **Stub** | No-op |
| N/A | `XAudioSuspendRenderDriverClients` | **Stub** | No-op |

---

## Audio Device Render Client Struct (sub_82194708 context)

From analysis of `sub_82194708` in `gta4_recomp.2.cpp` (lines 10010–10075):

| Offset | Value at init | Description |
|-|-|-|
| +0 | `0x820BE5EC` | Primary vtable pointer |
| +4 | `0x820BE5D0` | Secondary interface vtable |
| +8 | from parent obj+4 | Inherited callback arg |
| +12 | callback arg | Passed to `XAudioRegisterRenderDriverClient` as `callback_ptr[1]` |
| +16 | 0 | Last submitted sample tic |
| +20 | 0 | Submission counter |
| +24 | `0x4155xxxx` | Driver handle (returned by `XAudioRegisterRenderDriverClient`) |

The render client is a ~28-byte struct. The driver handle encoding `0x41550000 | index` allows
up to 65535 audio clients; `0x4155` is ASCII `"AU"` — a Xenia-originated convention.

---

## Global State Written During Audio Device Init

| Address | Written by | Value | Meaning |
|-|-|-|-|
| `0x831CC944` | `sub_82478AF8` phase 2 | audio_mgr ptr | Global audio manager object |
| `0x831EA404` | `sub_821945C0` (via `sub_82194770`) | 6144 | Frame tic counter (incremented every audio frame, stride 6144) |
| `0x82B29484` | `sub_82194770` | 6144 | Audio render frame counter global |
| `0x831D53EC` + 304 | read by voice category loop | ptr | Speaker/voice configuration object |
| `0x831D52DC` | read by `KeReleaseSemaphore` path | semaphore | Audio worker thread semaphore |
| `0x82FF5360` | `sub_82478AF8` phase 5 | audio_mgr ptr | Second global audio manager copy |
| `0x82FF5364` | `sub_82478AF8` phase 5 | audio_device ptr | Audio device/router (192 bytes) |

---

## String References

No embedded ASCII/UTF-16 string literals for `"XAudio"`, `"DirectSound"`, `"WASAPI"`, or
`"audio renderer"` were found in the pseudo-code for these specific functions. The pseudo-code
file has no string references in the `0x8294xxxx` or `0x8293xxxx` ranges at all.

The audio backend selection in `sub_82478AF8` (Phase 4, lines 83940–84004) uses `rexcrt__stricmp`
to compare a config string against backend names. Those string addresses are in the
`0x82FF53C0` area (config/ini storage). The exact string literals are stored as global data in
the XEX `.rodata` section and would require XEX binary extraction to read directly.

---

## Xbox 360 Hardware Dependencies

| Dependency | On Xbox 360 | In LibertyRecomp |
|-|-|-|
| XMA hardware decoder | Required for compressed audio | XmaDecoder disabled on macOS (`#ifndef __APPLE__`) |
| XAudio render driver thread | Kernel-managed, 64-frame ring | Replaced by `AudioSystem::WorkerThreadMain()` (SDL) |
| `XAudioRenderDriverInitialize` | Binds HW audio engine | Stubbed — no-op |
| `XAudioRenderDriverLock` | Serializes render thread | Stubbed — no-op |
| PCM frame submission | 256 samples/frame stereo at 48kHz | `SDLAudioDriver::SubmitFrame()` → SDL2 callback |
| Voice category volumes | Per-category HW mixer | Emulated: returns 1.0f, mask 0 |
| Speaker config query | HW HDMI/analog detection | Returns constant `0x00010001` (stereo) |

---

## Relationship to the Hang (sub_8285D610)

The audio device init functions documented here (`sub_8294BD68` through `sub_8294BEA0`) complete
successfully in LibertyRecomp — all their kernel calls are implemented or stubbed.

The hang identified in `ppc_analysis_sub_82478AF8.md` occurs later (Phase 13), inside
`sub_8285D610` → `sub_82849778` (XMA source context creation). That is downstream of this
audio manager/device construction and is a separate issue from the functions analyzed here.
