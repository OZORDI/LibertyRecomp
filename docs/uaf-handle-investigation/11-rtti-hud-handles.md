# 11 — RTTI / HUD-handle class enumeration (UAF candidates)

Scope: identify the object type whose pointer is the freed handle that `sub_8227F2E8` (DrawQuadFP) dereferences at offset 0. Caller chain converges on `sub_8227F2E8`, inside the `0x8227xxxx` HUD / quad-emit cluster.

## 1. DrawQuadFP reality check — `sub_8227F2E8`

Key recomp observations (addresses inside the function):

- Spill: `lwz r30, 300(r1)` — r30 is a pointer to an object passed via caller's stack slot
- Per-vertex emit loop: `lwz r9, 0(r30)` (x4), then `sub_828C2290(r9 as color)`
- Callee `sub_828C2290` is the **immediate-mode vertex emitter** in RAGE's `grcDrawMode`-style ring:
  - writes 8× stfs (x, y, z, nx?, ny?, u, ?, v → 8 floats) + `stw r9, 24(r11)` (the packed colour `uint32_t`)
  - advances ring pointer by `+36` and increments vertex count in TLS-ish globals
- Quad = 4 iterations → 4 × 36 = **144-byte draw into vertex ring**
- So r30 is a **small object** whose `[0]` is a packed colour DWORD (or the first entry in a struct whose first 4 bytes is the colour/vtable)

Conclusion: r30 is not a primitive; it is an object the quad emitter reads per-vertex.

## 2. The DrawCommand ("DC") class hierarchy

Scan of XEX RTTI reveals a massive `CBaseDC` hierarchy (108 child classes). These are **per-frame allocated render commands** — textbook UAF territory.

RTTI root:

- `CBaseDC` @ vtable `0x82000974` (6 slots)
  - Field cluster (r3 reads): `+0x0` (vtable/payload), `+0x8` (next-link / cookie)
  - Slot[0] = `sub_821BB5F0` (CBaseDC ctor) — writes vtable at `this+0`, optionally calls `sub_821B3560` (TLS allocator dispatch for DC body/free)

Sub-classes that directly match the caller chain to `sub_8227F2E8`:

|class|vtable|slot-0 ctor|slot-1 "render"|
|-|-|-|-|
|CHud_RenderBarDC|0x820013A8|sub_821BB5F0|sub_821BCF80|
|CHud_RenderSpriteDC|0x820013C4|sub_821BB5F0|sub_821BCFA0|
|CHud_RenderWindowDC|0x820013E0|sub_821BB5F0|sub_821BD018|
|CDrawSpriteDC|0x8200146C|sub_821BB5F0|sub_821BD2D8|
|CDrawSpriteUVDC|0x820014A4|sub_821BB5F0|sub_821BD4C8|
|CDrawSpritePerspDC|0x82001514|sub_821BB5F0|(small)|
|CDrawRectDC|0x820014C0|sub_821BB5F0|sub_821BD528|
|CDrawCurvedWindowDC|0x820014DC|sub_821BB5F0|sub_821BD538|
|CDrawTriShapeDC|0x82001450|sub_821BB5F0|sub_821BD238|
|CDrawRadarMapSectionDC|0x820013FC|sub_821BB5F0|sub_821BD0A0|
|CDrawRadarCircleDC|0x82001434|sub_821BB5F0|sub_821BD218|
|CDrawRadioHudTextDC|0x82001418|sub_821BB5F0|sub_821BD138|

**All of these render functions live in the `0x821BCxxx–0x821BDxxx` range**, which is precisely the HUD-draw cluster that calls into `sub_8227F2E8`/`sub_8227F5B8`/`sub_8227F608` (the DrawQuadFP family).

## 3. The allocator behind DC objects

- `CDrawCommandAllocator` (vtable `0x82000F2C`, 19 slots) inherits **`rage::sysMemAllocator`**.
- Base alloc class `rage::sysMemAllocator` has 10 subclasses; `CDrawCommandAllocator` is the **frame-scoped drawcommand heap**.
- DC ctor helper `sub_821B3560` resolves `TLS[r13+0] → +1676 → allocator ctx → vtable[12]` — this is the guest's **per-thread allocator lookup**, exactly the TLS-1676 slot my project memory notes mark as the allocator context (`sub_8218BE28` is the malloc equivalent, same TLS slot).
- When not set up: that TLS slot is 0 → host page-alloc fallback (matching the known "ALLOC FALLBACK storm" during particle init).
- DCs are allocated **into the CDrawCommandAllocator arena** which is **reset at end-of-frame** (CEndDrawListDC → sub_822BCA90, slot 18 in CDrawCommandAllocator, a likely "wipe/free-all" op).

## 4. Size / layout profile

Every CBaseDC-derived class in the HUD / sprite cluster shows exactly the same r3 field access cluster: `{+0x0, +0x8}`. The writes happen in their ctors (not visible via the `r3` field cluster, which only shows reads from incoming `this`), so the observed read set understates the size. The vtable holds a "size" slot (slot 5 in CBaseDC layout per `sub_821BEF28`, `sub_821BF3F0`, etc.) — these are the **object-size / move-to-next-DC** helpers that drive the arena walker.

- Minimum DC object: 16 bytes (vtable + 4-byte payload + pad)
- Typical DC object (e.g. CDrawRectDC): 0x3C = **60 bytes** (field cluster shows offsets up to `+0x38`)
- CDrawSpriteDC-family: small payload (`this+0`, `this+8`) plus per-class texture/color/pos data (not visible via caller reads — filled at ctor time, consumed by the `render` vfunc which reads via locals not via r3)

**So r30 into sub_8227F2E8 is compatible with ANY DC object in the 16–64 byte range.** The specific identity depends on which DrawQuadFP caller you're in. Given caller chain:

- `sub_8227F608` (4 callers inside 0x8218xxxx–0x821Fxxxx) — reached from **CHud_RenderSpriteDC::vfunc[1] (`sub_821BCFA0`)** family
- `sub_8227F5B8` (5 callers) — reached from **radar / HUD / text DC renders**

## 5. The freed-handle hypothesis

The "freed handle" is **NOT the DC itself being dereferenced past frame-end** — because the DC arena reset happens between frames and the DC pointer never leaves the current frame (it's consumed by `CEndDrawListDC`).

The UAF is one of:

### 5a. `rage::grcTexture*` cached inside a DC, pointing at a released TXD

- DCs like `CHud_RenderSpriteDC` / `CDrawSpriteDC` cache a texture handle at a payload offset (see `sub_8227EE90` which the DrawQuadFP calls first — likely "bind texture / apply state").
- If a HUD texture dictionary (TXD) is streamed out between the DC being *built* (update thread) and *executed* (render thread), the cached `grcTexture*` dangles.
- grcTexture size = vtable(1) + pgBase/datBase fields; vtable has 25 slots (`0x8209369C`), so the handle is ≥0x10 bytes and `*(uint32_t*)handle = vtable@0x82093694` (still valid address) — which would NOT crash on read but WOULD read the wrong byte pattern → corrupted colour value → visible artifacts OR an out-of-range texture.
- This is **the most likely UAF class**: texture pointers cached cross-thread in DCs.

### 5b. CDrawCommandAllocator arena wrap-around

- If the drawlist allocator is a ring, a late DC from frame N is read *after* the allocator has wrapped and overwritten its bytes in frame N+1. The "handle" now points into the *new* DC data, which still has a vtable-like dword at `+0` → no read crash, but wrong geometry/colour.
- Less likely, because CEndDrawListDC typically flushes the draw-thread queue before reset.

### 5c. Model/TXD reference cached in `CAddTxdReferenceDC` / `CAddModelInfoReferenceDC`

- These DC classes explicitly exist to extend refcounts across the build→render split (the fact that they're named `Add*ReferenceDC` is the giveaway). Bugs in pairing those with `CEndDrawListDC` can leak or prematurely drop the refcount, leaving dangling handles in other DCs.

## 6. Direct UAF-suspects to watch in LibertyRecomp

Based on the 0x8227xxxx HUD convergence + DC hierarchy:

|suspect handle|size|why it matches|
|-|-|-|
|`rage::grcTexture*` (cached in CHud_RenderSpriteDC / CDrawSpriteDC)|~0x30+ (vtable @ 0x82093694)|caller reads `*(u32*)handle` = vtable → passes to `sub_828C2290` (colour arg). If the packed-color happens to live at `texture+0` in a subclass we'd see it in the ring.|
|`CHud_RenderBarDC*` itself (frame-allocated, used post-EndDrawList)|16–32 B|matches sub_821BCF80 render path; if draw-thread sees it after arena reset → UAF but vtable at +0 still valid→ no crash, wrong colour|
|`CHud_RenderWindowDC*` (Hud-panel quad)|16–32 B|matches sub_821BD018 render path, same profile as above|
|`CDrawSpriteUVDC*` or `CDrawRadioHudTextDC*`|16–32 B|same DC ring, radio-text HUD|

## 7. Recommended next probes (for agents 12+)

1. Map each `sub_8227F608` caller back to its wrapping **C\*DC::render** vfunc; find which DC class provides the r30 handle.
2. Inspect the DC ctor pattern: search for `PPC_STORE_U32(this+8, handle)` stores in the 0x821Bxxxx range — the `handle` value passed as ctor arg is the UAF candidate.
3. Grep for `sub_821B3560` callers during frame-end → confirm arena reset semantics (buddy-free vs full-wipe).
4. In LibertyRecomp host code, check if `CDrawCommandAllocator` is currently emulated or if DC allocations fall through to `sub_8218BE28`'s host fallback — that fallback does NOT reset per frame, so DCs would leak into the system heap and **the UAF would occur on the DESTRUCTION path** when the game's own CEndDrawListDC walks stale arena pointers.

## 8. Key addresses (for hooks / watch-points)

- CBaseDC vtable: `0x82000974`
- CBaseDC ctor: `0x821BB5F0` (writes vtable at this+0, cond-calls allocator hook)
- DC allocator helper: `0x821B3560` (TLS[+1676] dispatch, vtable[12] = "free DC")
- CDrawCommandAllocator vtable: `0x82000F2C` (19 slots, inherits sysMemAllocator)
- CHud_RenderSpriteDC render path: `sub_821BCFA0` → `sub_8227F608` → `sub_8227F2E8` (DrawQuadFP)
- CHud_RenderBarDC render path: `sub_821BCF80` → `sub_8227F5B8` → `sub_8227F2E8`
- CHud_RenderWindowDC render path: `sub_821BD018` → `sub_8227F5B8/F608` → `sub_8227F2E8`
- Vertex ring emitter: `sub_828C2290` (reads ring ctx from `r11 = -2095316992 + 11580`, stride 36 B)

## 9. One-line verdict

The UAF "handle" dereferenced at `sub_8227F2E8 + 0` is almost certainly a **`grcTexture*` cached inside a `C*DC` draw-command** (CHud_RenderSpriteDC / CDrawSpriteDC / CDrawSpriteUVDC / CHud_RenderBarDC), and the UAF triggers when the HUD DC arena is NOT being reset per-frame on the host path — causing DCs (and their cached texture pointers) from a stale frame to outlive the streaming manager's release of their TXD.
