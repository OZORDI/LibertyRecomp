# EDRAM handling in LibertyRecomp — validation of the 0xC000 hypothesis

Agent 11/15 — post-fix crash research. RESEARCH ONLY.

## TL;DR — hypothesis REJECTED

LibertyRecomp has **no EDRAM memory-emulation layer** (no byte-addressable
backing buffer, no Lock/Unlock path that maps an EDRAM offset to host
memory, no `0xC000` mapping in any physical allocator). But **the crash
target 0xC000 is not an EDRAM offset either** — it's a null-adjacent
guest pointer, as covered by agent 2. The "Unknown format 0x8" warning
seen before the crash is a **texture format**, not an EDRAM-surface
format, and it returns a safe `R8G8B8A8` fallback without any allocation
side-effect. There is no EDRAM proxy that "should have caught" a write
to 0xC000 because `sub_828D9AC8 -> sub_82A00DC0` is an ordinary guest
`memcpy` into what should have been a real texture upload buffer, not
an EDRAM tile.

**Conclusion**: agent 10's EDRAM hypothesis is an architectural dead-end
for this crash. The architectural gap (no EDRAM emulation) is real but
orthogonal — GTA IV avoids raw EDRAM CPU access through the `BeginTiling`
/ `EndTiling` + `SurfaceSize=0` workarounds, none of which involves
`0xC000` or CPU-side memcpy.

## LibertyRecomp's "EDRAM" layer — what exists

| component | what it does | is it a memory proxy? |
|-|-|-|
| `gtaiv_render_state.h` `DeviceOffsetExt::EdramBase` (offset 1784) | Reads `edramBase`/`edramPitch`/`SurfaceFormat` fields from the GuestDevice for diagnostic/log purposes only | **No** — pure read of guest-register-state copy |
| `GetEDRAMFormat(device)` (`gtaiv_render_state.cpp:166`) | Returns `XenosSurfaceFormat` enum — used to decide HDR passthrough path | No |
| `IsHDR10Bit()` / `IsHDR16Bit()` | Classifies current surface format for rendering hints | No |
| `CreateSurface(w,h,fmt,ms,params)` (`video.cpp:4101`) | Creates a host render-target texture; comment at line 4099 acknowledges "same memory location in EDRAM for HDR and FB0" but **only uses the base as a cache key** (`g_surfaceCache.emplace_back(surface, baseValue)`) | No — EDRAM base is a dictionary key, not a real address |
| `SurfaceSize(...)` at `video.cpp:10176` | Always returns 0 — defeats the 1024-threshold EDRAM tiling check in GTA IV | No — it's a lie to skip tiling |
| `D3DDevice_BeginTiling` / `D3DDevice_EndTiling` (`video.cpp:10196,10205`) | **Hooks disabled** ("Sonic 06 address"); game calls fall through to recompiled guest code | No — no host implementation at all |
| Pending MSAA-resolve pipelines (`g_resolveMsaaColorShaders`, `g_resolveMsaaDepthPipelines`) | Host shader-based MSAA resolve when hardware resolve isn't available | No — GPU-side, no EDRAM bytes exposed |

## What LibertyRecomp does **NOT** have

Grepped across `LibertyRecomp/**` (all .cpp, .h, .mm):

- No `EDRAM_BASE`, `EDRAM_SIZE`, `edram_backing`, `tile_backing` constants.
- No 10 MiB scratch buffer reserved at any fixed guest address for EDRAM
  tile mirroring.
- No `VdRetrainEDRAM` hook (MarathonRecomp has one at `imports.cpp:934`
  — returns 0 — but LibertyRecomp's `kernel/` has **no VdRetrainEDRAM
  entry at all**; the import either goes through rexglue's default
  stub or is unresolved).
- No Lock/Unlock pair for EDRAM surfaces — only `LockTextureRect` /
  `LockBuffer` which allocate from `g_userHeap.AllocPhysical` (real
  host-backed physical memory). These never involve a 0xC000 region.

### Xenia reference (tools/xenia-master-1/src/xenia/gpu/xenos.h)

Xenia has a **full** EDRAM model: 10 MiB opaque block, 2048 tiles of
80x16 32bpp, circular tile addressing, format codes
`k_16_16_EDRAM`/`k_16_16_16_16_EDRAM`/`k_2_10_10_10_FLOAT_EDRAM`, etc.
See `xenos.h:225-271` and `xenos.h:483,497,538,539`. Xenia also
implements resolve copy-from-EDRAM to main memory (`xenos.h:911-971`).
**LibertyRecomp has none of this** — by design. The recomp only
emulates what GTA IV actually hits.

### UnleashedRecomp / MarathonRecomp

- UnleashedRecomp-main `UnleashedRecomp/gpu/` — **zero** matches for
  "EDRAM"/"edram". Only kernel-level stub for `VdRetrainEDRAM`
  (`imports.cpp`) that returns 0.
- MarathonRecomp-main `MarathonRecomp/gpu/video.cpp:3545-3549` has the
  **exact same** "TODO: Singleplayer uses same EDRAM memory location"
  comment as LibertyRecomp — this is **inherited Sonic06-era code**,
  not Liberty-specific EDRAM emulation.

## The "Unknown format 0x8" warning — is it an EDRAM surface?

`video.cpp:3974-3977`:

```cpp
default:
    // GTA IV uses some non-standard format codes, return a safe default
    LOGF_WARNING("Unknown format {:#x} - using RGBA8 fallback\n", format);
    return RenderFormat::R8G8B8A8_UNORM;
```

Defined format constants (from `video.h:142-162`):

| name | value | category |
|-|-|-|
| D3DFMT_INDEX16 | 1 | index buffer |
| D3DFMT_INDEX32 | 6 | index buffer |
| D3DFMT_DXT1 | 0x1A200152 | texture |
| D3DFMT_A8R8G8B8 | 0x18280186 | texture |
| D3DFMT_A8 | 0x4900102 | texture |
| ... | ... | ... |

`0x8` is **not** one of the Xenos EDRAM surface-format enum values —
those are `0x06`, `0x1A`, `0x1F`, `0x28`, `0x29` per
`gtaiv_render_state.h:46-50`. `0x8` is a tiny value that falls through
the `ConvertFormat` switch because none of the `D3DFMT_*` enums match;
it's almost certainly a **garbage/uninitialized format byte** extracted
from a corrupted `grcTextureXenon` that agent 2 already identified as
the crash trigger. The fallback to `R8G8B8A8_UNORM` is a **safe,
non-allocating code path** — it just returns a RenderFormat enum value.
It does **not** create a surface, does **not** allocate memory, and
does **not** write to 0xC000. The warning is a **symptom** of the
same underlying corruption (a stale/freed texture whose format field
is now junk), not a cause.

## Why no proxy "catches" a write to 0xC000

1. `sub_828D9AC8` (grcTextureXenon::vfunc[23]) is the caller (per agent 2/3).
2. It reaches the guest inline `__memcpy` at `sub_82A00DC0` with a
   corrupt dst pointer.
3. `sub_82A00DC0` is **plain recompiled code** — not a hook, not
   proxied through any EDRAM translator.
4. The guest store instruction (`std`) goes through the standard
   recomp memory path (`PPC_STORE_U64`), which does `MapVirtual(0xC000)`
   and SIGSEGVs because guest VA 0xC000 isn't mapped in the host
   `g_memory` arena.
5. **There is no intermediate "EDRAM proxy" layer** that could
   intercept this store, for two reasons:
   - LibertyRecomp doesn't emulate EDRAM byte-addressably.
   - 0xC000 was never an EDRAM target anyway — the Xenon exposes
     EDRAM *only through the RB*, never as a CPU VA.

## Cross-reference

- Agent 2: sub_82A00DC0 is the culprit memcpy, crash is in
  `loc_82A00F5C` unrolled hot-loop — dst pointer comes in **already
  corrupt** from the caller.
- Agent 3: `grcTextureXenon::vfunc[23]` / `sub_828D9AC8` — texture
  finalizer calling memcpy with a freed/stale texture.
- This agent (11): **EDRAM emulation is absent by design** in
  LibertyRecomp/UnleashedRecomp/MarathonRecomp, but that's NOT the
  root cause — the root cause is use-after-free or format-field
  corruption on a grcTextureXenon upstream of the memcpy.

## Recommendation

Do not pursue EDRAM emulation as a fix for this crash. The correct
investigation path continues in agents 2/3/4's directions:
grcTextureXenon lifetime, vtable-slot-23 contract, and which code
path produced a texture with `format=0x8`. Adding an EDRAM memory
proxy would be a large architectural change with no bearing on
`0xC000` — because 0xC000 is a null-adjacent guest pointer, not
an EDRAM tile offset.

## Files referenced

- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/gpu/video.cpp`
  (lines 3939-3978 ConvertFormat, 4099-4166 CreateSurface, 10173-10215
  SurfaceSize/BeginTiling/EndTiling, 2697-2735 LockTextureRect)
- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/gpu/gtaiv_render_state.h`
  (lines 14-19 DeviceOffsetExt, 45-51 XenosSurfaceFormat)
- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/gpu/gtaiv_render_state.cpp`
  (lines 74-77 Extract, 146-169 HDR/EDRAM format helpers)
- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/gpu/video.h`
  (lines 142-162 D3DFMT constants)
- `/Users/Ozordi/Downloads/LibertyRecomp/tools/xenia-master-1/src/xenia/gpu/xenos.h`
  (lines 225-271 EDRAM model, 483-539 EDRAM formats, 911-971 resolve)
- `/Users/Ozordi/Downloads/LibertyRecomp/Reference Projects/MarathonRecomp-main/MarathonRecomp/kernel/imports.cpp`
  (line 934 VdRetrainEDRAM stub)
- `/Users/Ozordi/Downloads/LibertyRecomp/Reference Projects/UnleashedRecomp-main/UnleashedRecomp/kernel/imports.cpp`
  (line 934 VdRetrainEDRAM stub)
