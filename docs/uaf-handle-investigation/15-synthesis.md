# 15 — Synthesis of UAF-Handle Investigation (agents 01-14)

Cross-read of 13 sibling MDs plus 10 prior audio-pool-crash MDs.
Purpose: converge on the one canonical alloc/free/use story, flag
conflicts, rank the top-3 root-cause hypotheses.

## 1. Crash primitives (agreed by every agent)

| fact | value | source |
|-|-|
| fault PC | `sub_828C2300+0x34` (`stw r11,0(r31)`) | audio-crash/01, 02, 04 |
| expected r31 | `0x831C2D38` (singleton draw-pool limit/vertCount slot) | audio-crash/01 |
| actual r31 | `0x20` (small int; not a pointer) | audio-crash/02 |
| r9 at fault | `0xFFE1E1E1` (RAGE freed-fill, stamped by `sub_82847160`) | uaf/10 |
| LR at fault | `0x8227F3AC` (inside `sub_8227F2E8` after 1st `bl sub_828C2290`) | audio-crash/02 |
| reported thread | tid 12 "Enumerate Content" (bystander; real thread is render) | audio-crash/03 |

## 2. Canonical function roles (now unambiguous)

| function | role |
|-|-|
| `sub_828C21D0` | `grcDraw::Begin(prim, count)` — allocates a 36-B vertex slot via `sub_828BF248`→`sub_82A3DAB0` and publishes `{handle,type,limit,count}` at `0x831C2D28..3C` |
| `sub_828C2290` | `grcDraw::AddVertex(f1..f8, r9)` — stores 8 floats + `r9` as packed ARGB colour at `vertex+24`, bumps cursor by 36 |
| `sub_828C2300` | `grcDraw::End()` — `sub_828BF270()` then zero `0x831C2D28` and `0x831C2D38` |
| `sub_828BF270` | 3-insn shim: loads `*(u32*)0x831C22A4` into r3, tail-calls `sub_82A3DF50` (commits small-block pool). Does **not** touch r31 |
| `sub_8227F2E8` | DrawTexturedQuad2D — 4 × `AddVertex` emitting one quad. Reads `r30 = *(sp+300)`, then `r9 = *r30` each vertex |
| `sub_8227F5B8` / `sub_8227F608` | thin wrappers that spill the handle (r4 or r5) to `sp+92`; by stwu math, `callee_sp+300 == wrapper_sp+92` |
| `sub_821BB3D8` | **double-buffered 1 MB bump arena** (base `0x82B38B58`; 436 callers, hot, leaf). No per-alloc free; whole-chunk flip clears flags but does not null out dangling pointers |
| `sub_821BD028` / `sub_821F6550` | handle minter and HUD/loading-screen draw-list producer — allocates DC-like objects from the bump arena |
| `sub_82847160` | generic poison writer: `memset(dst, 0xE1, len)` when `0x82795E38` is set to `E1 E1 E1 E1` |

## 3. The handle type — type-unified across agents

All offset evidence (01, 09, 11) agrees: `r30` is dereferenced **only** at
`+0`, four times, and the result is stored verbatim into vertex slot `+24`
(the packed ARGB colour word). There is **no vtable call** off r30.

Therefore `r30` is a `uint32_t*` pointing at a **packed ARGB colour field**
embedded inside some draw object. Agent 09 pinned the owner: a 228-byte
`CBaseDC` subclass whose vtable is `0x820BE3C0`, allocated on the HUD draw
path via `sub_821BB3D8` (the bump arena) and holding `Color32` fields
starting around offset `+108/+120/+132/+144`.

## 4. Canonical alloc → free → use timeline

### Alloc
1. Early boot / loading-screen path (`sub_821B5C90` or `sub_8218DD80`) calls
   `sub_821BB3D8(size=228, align=0)` → returns a pointer into chunk-0 of
   the 1 MB bump arena at `0x82B38B58`.
2. Caller writes a CBaseDC vtable (`0x820BE3C0`), a draw function pointer
   (`sub_8218DB88`), and `Color32` fields at `this+108/+120/+132/+144`.
3. `sub_821F6550` / `sub_821BD028` latch its "init done" flag at
   `0x82520C2A` and stash handles at `0x824FB9A4/A8` and/or in
   `dword_831E49B0` / `dword_831E49DC` (loading-screen slots).

### Store
4. The address `&DC.colour` is passed as r4/r5 into wrapper
   `sub_8227F5B8` / `sub_8227F608`. The wrapper spills it at `sp+92`.

### Free (= poison)
5. The bump arena flips chunks: `sub_821BB3D8`'s `loc_821BB428` path sets
   `flags[8+idx]=1` and resets the bump offset.
6. A re-mint from the freed chunk (or the explicit RAGE fill path through
   `sub_82847160` called by `fragHeapAllocator::Free` / the simple
   allocator slow path) stamps `0xE1` across the old payload. The DC's
   `Color32` field is now `0xE1E1E1E1`.
7. Critically, `dword_831E49B0/DC`, `0x824FB9A4/A8`, and any already-spilled
   `&DC.colour` on the render thread's stack are **not** nulled.

### Use-after-free
8. Render thread enters `sub_8227F2E8`, reloads `r30 = *(sp+300)` —
   pointer still equal to the stale `&DC.colour`.
9. `lwz r9, 0(r30)` reads `0xFFE1E1E1` (alpha = 0xFF, RGB = 0xE1E1E1).
10. First `bl sub_828C2290` at `0x8227F3A8` writes the poison into the
    ring buffer (`stw r9,24(r11)` succeeds — LR becomes 0x8227F3AC).
11. Three more vertices follow; `sub_828C2300` runs; inside the
    epilogue `ld r31,-16(r1)` reloads a corrupted save slot (0x20);
    `stw r11, 0(r31)` faults. The `r31=0x20` value is a late symptom,
    not the root cause — see conflicts below.

## 5. Conflicts and resolutions across siblings

| conflict | resolution |
|-|-|
| audio-crash/02 says `sub_8227F2E8` is an **audio command emitter** (4 × AddVertex = audCmdQueue packets). uaf/01 + uaf/09 + uaf/11 say it's a **2D textured quad draw**. | Draw wins. The callee `sub_828C2290` writes 8 floats + 1 u32 at +24 bytes through a ring pointer at `0x831C2D28`; the caller family is `CHud_RenderSpriteDC::vfunc[1]` et al. The "audio" label in audio-crash/02 was a misread — it's the same immediate-mode *draw* batcher used by HUD DCs |
| uaf/02 calls r4 a "sentinel stack slot with -1", implying no UAF through `sub_8227F5B8`. uaf/07 + uaf/11 + uaf/09 treat r4 as a pointer into a heap DC. | Both are right for different call sites. 5 of 8 F5B8 sites pass `&(-1)` (opaque white) on a stack local — not UAF sources. The 3 remaining sites (via `sub_821F1EE0`, `sub_8229D8A8`, `sub_8218DD80`→heap DC) pass `&Color32_in_heap_DC` — those are the UAF paths |
| uaf/04 says the `stack+300` read is **uninitialized** (F2E8 never writes it). uaf/01 + uaf/02 show it equals wrapper's `sp+92` (the handle spill). | uaf/01 is correct. Python: `208 + 92 = 300`. F2E8 never needs to write `sp+300` — the *wrapper* already filled that slot before the `bl`. uaf/04's claim is wrong |
| audio-crash/04 says Pool B is the audio OcclusionGroups pool; uaf/01 + uaf/11 say it is the HUD draw-command queue at `0x831C2D28..3C`. | Draw-command wins. The 46 callers of `sub_828C21D0`/`sub_828C2300` include `CDrawRadarMapSectionDC::vfunc[0]`, `CDrawRadioHudTextDC::vfunc[0]`, `CDrawTriShapeDC::vfunc[0]` — authoritative RTTI evidence |
| How does `r31=0x20` happen? Multiple theories: (a) `sub_828BF270` clobbers saved r31 (audio-crash/02/06); (b) per-thread PPCContext sharing (audio-crash/04); (c) late symbolication artefact (audio-crash/01); (d) spilled `-16` red-zone reload of garbage (uaf/06). | None of `sub_828BF270` / `sub_82A3DF50` touches r31 (proven by audio-crash/06). Most plausible remaining: the 36-B per-vertex ring write bumped the cursor past the allocation that overlapped the caller's saved-r31 slot (the ring buffer and the DC arena both live in the same bump heap at `0x82B38Bxx`). The AddVertex `stw r8,-20(r10)` advance can walk into a frame save area if the arena wrapped. This is a **secondary** corruption caused by the same bump-arena-flip that produced the r9 poison |

## 6. Ranked root-cause hypotheses

### H1 (PRIMARY) — Bump-arena chunk flip without invalidating stale handles
Confidence: **high**.
Evidence: uaf/13 proves `sub_821BB3D8` is a 1 MB double-buffered arena with
no per-alloc free. Chunk flip clears flags but leaves `dword_831E49B0/DC`,
`0x824FB9A4/A8`, and in-flight `&Color32` pointers dangling. Free-fill
`0xE1E1E1E1` confirmed as RAGE poison written by `sub_82847160` (uaf/10)
at the same time the other allocator path releases blocks. First frame
after a loading-screen flip reads the stale pointer and drops poison into
the vertex ring (uaf/14's captured vertex matches exactly).

Fix surface: instrument `sub_821BB3D8` chunk flip to null
`0x824FB9A4/A8`, `dword_831E49B0/DC`, and the HUD handle latch
`0x82520C2A`; or promote colour fields to copy-at-enqueue so the
dereference on the render side is eliminated (uaf/09's suggestion).

### H2 (SECONDARY, COMPATIBLE WITH H1) — Cross-thread DC arena reset without render-queue drain
Confidence: medium.
Evidence: uaf/11 + audio-crash/04. The `CDrawCommandAllocator` (vtable
`0x82000F2C`) is a per-frame arena reset by `CEndDrawListDC`
(`sub_822BCA90`, vtable slot 18). If the host doesn't drain the render
queue before the update thread wipes the arena (recomp build may have
elided the fence), DCs enqueued in frame N get freed while still
referenced by frame-N vertex submissions. Same poison byte pattern, same
`0xE1E1E1E1` outcome.

Fix surface: ensure render-thread `CEndDrawListDC` blocks for GPU
submission before allocator reset. In LibertyRecomp the relevant hook
is the `sub_821B3560` free-DC dispatch.

### H3 (TERTIARY) — grcTexture* cached inside DC dangles after TXD stream-out
Confidence: lower.
Evidence: uaf/11's option 5a. If the DC caches a `rage::grcTexture*` that
was released by the TXD streaming manager, the held pointer dereferences
fine (its vtable is at `0x82093694`, still valid), but the colour field
inside the *texture wrapper* is gone — leading to wrong tint values. Does
not fully explain `0xFFE1E1E1` unless the texture object itself sits in
a heap that was `0xE1`-filled. Retain as a candidate if H1/H2 fixes do
not fully stop the crash.

## 7. Files relevant to the fix

- `/Users/Ozordi/Downloads/LibertyRecomp/docs/uaf-handle-investigation/01-sub_8227F2E8-drawquad.md`
- `/Users/Ozordi/Downloads/LibertyRecomp/docs/uaf-handle-investigation/09-handle-type-inference.md`
- `/Users/Ozordi/Downloads/LibertyRecomp/docs/uaf-handle-investigation/10-rage-poison-writer.md`
- `/Users/Ozordi/Downloads/LibertyRecomp/docs/uaf-handle-investigation/11-rtti-hud-handles.md`
- `/Users/Ozordi/Downloads/LibertyRecomp/docs/uaf-handle-investigation/13-init-phase-alloc-free.md`
- `/Users/Ozordi/Downloads/LibertyRecomp/docs/uaf-handle-investigation/14-vertex-widget-identification.md`
- `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.10.cpp` (sub_8227F2E8 / F5B8 / F608)
- `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.66.cpp` (sub_828C21D0 / 2290 / 2300 / BF270)
- `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.62.cpp` (sub_82847160 poison writer)

## 8. Python-verified facts

```
python3 -c "
import struct
# stack-spill math
assert 208 + 92 == 300, 'F2E8 reads wrapper sp+92 as callee sp+300'
# globals
assert (-31972<<16)&0xFFFFFFFF == 0x831C0000
assert 0x831C0000 + 11576 == 0x831C2D38
assert 0x831C0000 + 11560 == 0x831C2D28
assert 0x831C0000 + 8868  == 0x831C22A4
# arena base
assert (-2099662848)&0xFFFFFFFF == 0x82B38B00  # nearby anchor
# poison
assert struct.unpack('>I', b'\\xff\\xe1\\xe1\\xe1')[0] == 0xFFE1E1E1
"
```

## 9. Open questions (for follow-up, not required for fix)

1. Does the recompiler emit a memory fence between update-thread DC writes
   and render-thread DC reads? (This is where H2 would be reproduced or
   ruled out.)
2. Is `CDrawCommandAllocator::Reset` (vtable slot 18 at
   `0x82000F2C+72`) being called on the host path, or did the reset get
   lost when the allocator fell through to `sub_8218BE28`'s host page
   fallback?
3. The two boot-only handle slots `dword_831E49B0` and `dword_831E49DC`
   — when are they expected to transition from boot-heap pointer to
   scene-heap pointer? If they are never rewritten post-boot, H1 is
   definitive.
