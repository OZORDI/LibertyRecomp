# Agent 6 — CreateRAGERT Hook Analysis

Research target: `LibertyRecomp/gpu/video.cpp` L9212-9299; log emitter L9227-9229.
Date: 2026-04-17. After-fix log: `/tmp/liberty_AFTER.log`.

## 1. Hook location and guest function

- **Guest function**: `sub_828BEC78` — RAGE's `CreateRenderTarget` factory.
- **Mechanism**: `PPC_FUNC_HOOK(sub_828BEC78)` at video.cpp:9212 (NOT `GUEST_FUNCTION_HOOK`
  because we use `PPC_FUNC_HOOK` to read `sp+84` from the caller frame before the
  callee prologue consumes sp).
- **Log site**: video.cpp:9227-9229 (`LOGF_WARNING("[CreateRAGERT] ...")`).

Calling-convention (per comment block L9201-9210):
- `r3` = device context (ignored)
- `r4` = width; `r6` = height
- `r8` = Xbox D3DFMT-packed constant; `r9` = MSAA count; `r10` = type
  (1 = 2D texture, 3 = cube, else = render surface)
- `sp+84` (caller frame) = output pointer — hook writes `MapVirtual(surface)` there.

## 2. Host implementation

Two branches:

- **type == 1 || 3** (texture RT): `g_userHeap.AllocPhysical<GuestTexture>(Texture)`,
  `createTexture` via `g_device`, 2D (or arraySize=6 for cube), `createTextureView`,
  allocates a shader-read descriptor (`g_textureDescriptorAllocator`), registers via
  `GTAIV::RegisterTexture`. Writes guest handle to `*outPtr`, returns 0.
- **else** (plain RT/DS surface — **this is the path all 5 log entries take, type=0**):
  falls through to `CreateSurface(width, height, fmt, msaa, nullptr)` at L4101
  (allocates `GuestSurface`, wraps `RenderTextureFlag::RENDER_TARGET` or `DEPTH_TARGET`
  if depth fmt, registers with `GTAIV::RegisterSurface`). Fmt=0 is forced to
  `0x1A200186` (RGBA8); compressed fmts get MSAA forced to 0.

## 3. Format decode — Xenos GPUTEXTUREFORMAT low-6 bits

|fmt|texfmt|meaning|gamma|upper|
|-|-|-|-|-|
|`0x1A220197`|0x17|`16_16_16_16_FLOAT` (FP16×4)|1|0x1A|
|`0x18280186`|0x06|`8_8_8_8` (RGBA8)|1|0x18|
|`0x1A200186`|0x06|`8_8_8_8` fallback for fmt=0|1|0x1A|
|`0x1A200152`|0x12|DXT1|1|0x1A|

All 5 requests are SHADER-READABLE RENDER TARGETS (RT surfaces, not depth). Upper byte
`0x18`/`0x1A` encodes size class + resource-type bits; bit-21 gamma flag on every one.

## 4. Size sanity — these are LUTs, not real RTs

1×1, 8×1, 32×1 are **NOT** viewport render targets. Verdict: RAGE "render-to-LUT"
technique from SCO post-FX:

- **1×1 fmt=0 then 1×1 FP16×4 (#1,#2)** — single-pixel solid-color/scene-probe
  sampler (ambient capture / env-map prefill).
- **32×1 RGBA8 MSAA=1 (#3,#4)** — 32-entry 1D curve (tone-map or colour-grading
  spline).
- **8×1 RGBA8 MSAA=1 (#5)** — 8-entry vignette / shadow-kernel ramp.

MSAA=1 on a 32×1 curve is nonsense on real GPU too — this is the Xbox MSAA count enum
(0/1/2/3 → 1/2/4/8× samples); value 1 = 2× which still makes no physical sense for 1D
LUTs. The `IsCompressedFmt` filter doesn't strip MSAA from RGBA8 — they pass through as
`RenderSampleCount::COUNT_2`. Likely benign on most backends, but a **subtle footgun
for any backend that refuses MSAA on 1×N render-target dimensions**.

## 5. Pointer space: guest VA in AllocPhysical heap

- `out=0xd9xxxxxx` / `0xd906a4xx` are **guest virtual addresses** (output of
  `g_memory.MapVirtual(surface)`) written back into `*(sp+84)`.
- Consecutive stride between `#3→#4` and `#4→#5` is `0x40` (64 B) — matches
  `sizeof(GuestSurface)` in the physical heap slab.
- `#1..#2` live in region `0xD906A3xx..D906A4xx` (probe surfaces in a SEPARATE slab).
- The crash register `r4=0xD906A5C8` sits **0x190 (400 B)** past out#2 and
  **`r11=0xD906A5C0`** is **0x188 (392 B)** past out#2 — so the wrapper being
  dereferenced is in the SAME physical-heap neighborhood as our just-created
  RAGE render-target handles, but **it is NOT one of our GuestSurface objects**
  (stride wrong, alignment wrong).

## 6. Connection to the crash

- Crash PC: `sub_82A00DC0 + 0xB44` = guest `0x82A01904`. This is inside the CRT
  block (between `strncpy` @0x82A00BA0 and `strtok` @0x82A01DE8 per MEMORY.md) —
  long enough (+0xB44 = 2884 B deep) to be `sprintf` / `_vsnprintf` or a large
  CRT scan/copy.
- Fault addr: guest `0x0000C000` (below XEX load base → unmapped).
  Derived from **`r3=0x0000BFF8` + 8** (struct field access at +8).
- Register pattern: `r5=0x100` (size), `r6=r7=0xFFFFE535` (mask/fill), `r10=0x08`,
  `r9=0x108` = 0x100+0x08 — strongly suggests a **byte-copy / scan loop where the
  destination/source pointer (`r3`) is a bogus vtable field read from the RAGE RT
  wrapper**. `r3=0x0000BFF8` and `r12=0x0000C0F8` look like low 16-bit garbage
  fields read from an uninitialised wrapper (classic "the game read vtable pointer
  from a zero-initialised slot plus a small adjustment").

### Chain: CreateRAGERT → wrapper uninitialised → next call dereferences garbage

1. `sub_828BEC78` allocates the **backing** `GuestSurface` via our hook — returns
   handle to caller and stores at `sp+84`.
2. Caller wraps the handle in a **RAGE-side `grcTextureXenon`-style wrapper
   struct** (the thing at `0xD906A5C0`). This wrapper is allocated by the GAME
   (RAGE pool), not by us.
3. The wrapper has fields at `+0`, `+4`, `+8` that RAGE's CPU-side code expects
   CreateRAGERT to populate: usually a **`grcTextureXenon` vtable** plus
   width/height/fmt/pitch/tile bits copied into the wrapper. Our hook stores
   ONLY the handle at `sp+84` and returns — **the wrapper fields never get
   populated** because we bypass the real `sub_82A55538` + `sub_82A42F10`
   inner-allocator chain which does that fill-in.
4. Subsequent code (the CRT routine `sub_82A00DC0+0xB44`) is called with
   `r11=wrapper` / `r4=wrapper+8`. It reads `*(wrapper+0)` = 0xBFF8, adds 8 →
   tries to touch guest 0xC000 → unmapped → ACCESS_VIOLATION.

The 5 `[CreateRAGERT]` calls immediately before the crash are the smoking gun:
we created 5 backing surfaces without filling the wrapper the RAGE caller expected.

## 7. vfunc[23] / grcTextureXenon linkage

No `vfunc[23]`, `grcTextureXenon`, `[0x5C]`, or `+92` matches in video.cpp. The hook
does NOT install a vtable on the surface/texture — it only registers descriptors.
The wrapper vtable fill must happen **in the original callee's post-allocation block**
(inside `sub_828BEC78` body or one of its callees, e.g. `sub_82A55538`). Because
`PPC_FUNC_HOOK` **fully replaces** the guest function, that post-alloc vtable fill
is skipped.

## 8. Recommended next steps for repro-fixer (NOT this agent)

1. Decompile the ORIGINAL `sub_828BEC78` body (`glue/gta4-recomp/generated/…`) to
   enumerate every store to `*(outStructPtr+N)` — then mirror those stores in the
   hook so the RAGE wrapper contains a valid vtable + dimensions + format + pitch.
2. Alternative: switch from `PPC_FUNC_HOOK` (full replace) to a **mid-function
   hook** that only intercepts the inner `sub_82A55538` Xbox allocator and lets
   the rest of `sub_828BEC78` run natively so it does its own field fill-in.
3. MSAA sanitation: treat width<=32 || height<=32 as LUTs and force MSAA=0
   regardless of format. Prevents the COUNT_2 misconfig on backends that reject it.

## Summary

- Hook is `PPC_FUNC_HOOK(sub_828BEC78)` at video.cpp:9212.
- 5 logged calls are all `type=0` surface branch → `CreateSurface()` → `GuestSurface`.
- Formats decode to FP16×4 and RGBA8, all shader-read, gamma bit set.
- Sizes (1×1, 8×1, 32×1) = RAGE post-FX 1D LUTs, not real RTs.
- `out=` ptrs are guest VA handles in physical heap.
- Crash r11 (0xD906A5C0) is 0x188 B from out#2, in the **same neighborhood** →
  strongly supports hypothesis that the game's RAGE RT wrapper struct sits adjacent
  to our backing surfaces, and **our full-replace hook never wrote the wrapper
  vtable/fields** the next caller needs — triggering garbage-pointer deref
  `r3=0xBFF8 + 8 → fault 0xC000` inside a CRT copy/format routine reached from
  `lr=0x828D9D0C`.
