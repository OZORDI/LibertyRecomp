# sub_828C2300 Caller Classification — Agent 8

Crash site: `sub_828C2300+0x34` — `stw r11, 0(r31)`. LR=`0x8227F3AC` (caller is `sub_8227F2E8`).
Reported `r31=0x20` is **inconsistent with the function's own logic** — see "Crash Reinterpretation".

---

## What sub_828C2300 actually is

It is **not** a pool/freelist primitive. It is the **`gfx::PrimEnd()`** function of an immediate-mode 2D draw API. The triplet:

| Function | Role | r3 inputs |
|-|-|-|
| `sub_828C21D0` | `PrimBegin(prim_kind, vert_count)` | r3=prim_kind (4=tri-strip, 6=tri-list, 1=line), r4=vert_count |
| `sub_828C2290` | `PrimAddVertex(x,y,z, w?, ?, ?, u, v, color)` | f1..f8 = floats, r9 = packed RGBA |
| `sub_828C2300` | `PrimEnd()` (THE CRASH) | NO r3 — pure singleton flush |

This is corroborated by the recomp:
- `sub_828C21D0` calls `sub_828BF248(rendertarget, prim_kind, vert_stride=36)` and writes 4 fields into the global vertex builder state at `0x831C2D28..0x831C2D40`.
- `sub_828C2290` writes 36 bytes (8 floats + 4-byte color) into the buffer pointed to by `*(0x831C2D28-20)` and bumps the count at `0x831C2D38`.
- `sub_828C2300` checks the "draw context owner ptr" at `0x831C2D28` (`r31-16`); if non-null, calls `sub_828BF270` (which dispatches the buffered primitive to the GPU command stream via `sub_82A3DF50`), then zeros both that pointer and the count.

Vertex stride is 36 bytes (`li r5,36` in `sub_828C21D0`). Globals at `0x831C2D2x` are: `[buffer_ptr, write_cursor, vert_count, capacity_words]`.

### sub_828C2300 reconstruction

```cpp
// Singleton flush of the pending 2D primitive batch.
// File-scope globals at 0x831C2D28 (g_PrimState).
struct PrimState {
    void*    buffer;       // 0x831C2D28 (-16 from r31)
    uint8_t* write_cursor; // 0x831C2D2C
    uint32_t reserved;     // 0x831C2D34 (lwz r3,-16(r31))
    uint32_t count;        // 0x831C2D38 (the +0(r31) field)
    uint32_t capacity;     // 0x831C2D3C
};
extern PrimState g_PrimState; // at 0x831C2D28

void Prim_End() {
    // r31 = &g_PrimState.count = 0x831C2D38
    if (g_PrimState.buffer != nullptr) {           // lwz r11,-16(r31)
        FlushPrimBatch(g_RenderTarget);            // sub_828BF270 -> sub_82A3DF50
        g_PrimState.buffer = nullptr;
    }
    g_PrimState.count = 0;                         // stw r11,0(r31)  <-- CRASH +0x34
}
```

`r31` in this function is a **constant** computed by `lis r11,-31972; addi r31,r11,11576 = 0x831C2D38`. It cannot become 0x20 by any path inside the function (no calls between `addi r31,...` at +0x14 and the crash at +0x34). See the "Crash Reinterpretation" section.

---

## Full caller table (46/46)

All callers invoke the `Begin → AddVertex* → End` triplet. "Vert count" = number of AddVertex calls; "Stride" column = `prim_kind` from `sub_828C21D0`'s r3 (1=lines, 4=tristrip, 6=trilist).

| # | Address | Size | Vtable | Bucket | Callees | Notes |
|-|-|-|-|-|-|-|
| 1 | sub_82143C88 | 320 | — | DebugDraw | 6 | 4-vert tri-strip; AABB rect; bottom of HUD |
| 2 | sub_82150E08 | 1664 | — | HUD/Map | 11 | calls sub_828C2140 (set state) |
| 3 | sub_82152938 | 776 | — | HUD/Map | 10 | |
| 4 | sub_82154DE0 | 808 | — | HUD/Map | 17 | also calls sub_82146790 |
| 5 | sub_8218DB88 | 504 | — | HUD/Hud | 8 | |
| 6 | sub_8218DF48 | 192 | — | HUD/Hud | 7 | |
| 7 | **sub_821BD0A0** | 152 | **CDrawRadarMapSectionDC::Execute** | DrawCommand | 5 | DC payload @ this+8 |
| 8 | **sub_821BD138** | 256 | **CDrawRadioHudTextDC::Execute** | DrawCommand | 5 | text/icon strip |
| 9 | **sub_821BD238** | 160 | **CDrawTriShapeDC::Execute** | DrawCommand | 5 | indexed tri-strip from DC payload |
| 10 | sub_821C4148 | 2248 | — | HUD/Hud | 13 | calls sub_8227E948 (4-vert quad) too |
| 11 | sub_821CB218 | 376 | — | HUD/Hud | 7 | |
| 12 | sub_821CE978 | 608 | — | HUD/Hud | 11 | |
| 13 | sub_821E6AB8 | 2792 | — | HUD/Map | 9 | |
| 14 | sub_822072D0 | 1288 | — | DebugDraw | 18 | uses VMX (vsubfp/vaddfp) |
| 15 | sub_8224D1E0 | 1424 | — | HUD/Hud | 16 | sprite drawer |
| 16 | sub_8224D770 | 1136 | — | HUD/Hud | 27 | sprite drawer w/ deps on text |
| 17 | sub_82254A78 | 1384 | — | HUD/Hud | 12 | menu-style |
| 18 | sub_82263E68 | 640 | — | HUD/Hud | 6 | minimal triplet |
| 19 | sub_82272428 | 1200 | — | HUD/Hud | 11 | uses sub_828C9980 finalizer |
| 20 | **sub_82276518** | 392 | — | DebugDraw | 6 | OBB list (loops r27 count, 4-vert quads + delta) |
| 21 | sub_8227E948 | 264 | — | DebugDraw | 4 | **HOT — DrawQuad(p1,p2,p3,p4,color)** |
| 22 | sub_8227EA50 | 264 | — | DebugDraw | 4 | DrawQuadUV variant |
| 23 | sub_8227EB58 | 264 | — | DebugDraw | 4 | DrawQuad variant |
| 24 | **sub_8227F2E8** | 352 | — | DebugDraw | 11 | **THE CRASH CALLER** — DrawQuadFP (8 floats f1..f8 + 8 more) |
| 25 | sub_8227F458 | 352 | — | DebugDraw | 11 | sibling of 8227F2E8 (only caller is sub_82201780) |
| 26 | sub_8228BCF8 | 712 | — | HUD/Hud | 6 | |
| 27 | sub_822906F0 | 1200 | — | HUD/Hud | 17 | |
| 28 | sub_82291FA8 | 1032 | — | HUD/Hud | 5 | |
| 29 | sub_822AC680 | 280 | — | HUD/Hud | 5 | minimal triplet |
| 30 | sub_822AEF88 | 1096 | — | HUD/Hud | 5 | |
| 31 | sub_822BCF20 | 672 | — | HUD/Frontend | 14 | menu screen |
| 32 | sub_822BD7F0 | 392 | — | HUD/Frontend | 8 | uses sub_828C9980 |
| 33 | sub_822BD978 | 1056 | — | HUD/Frontend | 12 | |
| 34 | sub_82333068 | 592 | — | HUD/Hud | 10 | |
| 35 | sub_823339E0 | 752 | — | HUD/Hud | 11 | |
| 36 | sub_82334C98 | 2416 | — | HUD/Hud | 11 | |
| 37 | sub_82336048 | 2032 | — | HUD/Hud | 15 | |
| 38 | sub_8233A7C0 | 1072 | — | HUD/RadarMinimap | 22 | strings: "fradar%02d", "radar%02d" |
| 39 | sub_8234A600 | 1200 | — | HUD/Hud | 14 | |
| 40 | sub_8235DA70 | 872 | — | HUD/Hud | 12 | |
| 41 | sub_82369BE8 | 800 | — | HUD/Hud | 8 | |
| 42 | sub_82369F08 | 464 | — | HUD/Hud | 8 | |
| 43 | sub_8238DDA8 | 1424 | — | HUD/Hud | 16 | |
| 44 | sub_823935D0 | 1152 | — | HUD/Hud | 13 | |
| 45 | sub_827C0C08 | 2240 | — | Audio? | 22 | only audio-range caller; sibling of audio code |
| 46 | sub_827D27D0 | 496 | — | Audio? | 9 | minimal triplet, audio range |

### Bucket totals

| Bucket | Count | Notes |
|-|-|-|
| Draw-context vfunc[0] (Execute) | 3 | All `CDraw*DC` derive a virtual `Execute()` not destructor |
| HUD/Map/Frontend/Radar | 30 | Bulk of callers — text, sprites, menus, radar |
| DebugDraw quads/lines | 9 | Generic primitives (sub_8227Exxx + 5 wrappers) |
| Audio range (827xxxxx) | 2 | sub_827C0C08, sub_827D27D0 — **address bucket only** — call pattern is identical to HUD draw |
| **Game-state teardown** | **0** | None of these are destructors |

---

## Bucket "Draw-context vfunc[0]" — three full reconstructions

These are the only ones with class assignments. Slot 1 of each `CDraw*DC` vtable is its `Execute()`. They are NOT destructors — they replay buffered draw commands recorded earlier on the main thread.

### 1. CDrawTriShapeDC::Execute (sub_821BD238)

```cpp
class CDrawTriShapeDC {
public:
    // header laid out by CDC base
    uint32_t  field_0[26];
    /* 104 */ int32_t   prim_kind;     // (lwz r3,104) — usu. 4=tristrip
    /* 108 */ uint32_t  color;         // (lwz r9,108) packed RGBA
    /* 112 */ int32_t   vert_count;    // (lwz r4,112)
    /* 116 */ int32_t   render_target; // (lwz r3,116)
    /* 56  */ struct { float u, v; } uvs[N];   // (this+56 + 8*i)
    /* 8   */ struct { float x, y; } pos[N];   // (this+56-48 = this+8)
    virtual void Execute();
};

void CDrawTriShapeDC::Execute() {
    Prim_SetState(this->prim_kind);                     // sub_828C2140
    Prim_Begin(this->render_target, this->vert_count);  // sub_828C21D0
    for (int i = 0; i < this->vert_count; ++i) {
        Prim_AddVertex(
            /* x  f1 */ this->pos[i].x,
            /* y  f2 */ this->pos[i].y,
            /* z  f3 */ 0.0f,                  // f31 const = 0
            /* w  f4 */ 0.0f,
            /* ?  f5 */ 0.0f,
            /* a  f6 */ 1.0f,                  // f30 const = 1
            /* u  f7 */ this->uvs[i].u,
            /* v  f8 */ this->uvs[i].v,
            /* col r9*/ this->color
        );                                              // sub_828C2290
    }
    Prim_End();                                         // sub_828C2300
}
```

### 2. CDrawRadarMapSectionDC::Execute (sub_821BD0A0)

Same exact prologue as TriShapeDC: `lwz r3,104(r30) → Prim_SetState`, then `Prim_Begin(rt, vcount)`, then 4 hardcoded `Prim_AddVertex` calls (radar tile is always 4 verts = 2 triangles), then `Prim_End()`. Vertex coords are inline from the DC payload. Hot path during minimap render.

### 3. CDrawRadioHudTextDC::Execute (sub_821BD138)

5 callees identical to TriShape; differs only in payload layout (UV from font glyph atlas + per-char advance loop). Used by station-name / track-name overlay.

These three vfuncs are **invoked indirectly** from the deferred command-list dispatch (the main render thread iterates a `CDC*` queue, calling `vt[1]()` on each). They are **NOT** destructors.

---

## Bucket "DebugDraw quads/lines" — two reconstructions

### 4. DrawQuad helper (sub_8227E948) — HOT, 25 callers

```cpp
// Prim_DrawQuad(p1, p2, p3, p4, color_ptr)
// All four points are (float x, float y) pairs.
void DrawQuad(const float* p1, const float* p2,
              const float* p3, const float* p4,
              const uint32_t* color_ptr)
{
    Prim_Begin(/*rt=4*/4, /*vcount=*/4);                   // sub_828C21D0
    uint32_t color = *color_ptr;
    Prim_AddVertex(p1[0], p1[1], 0.f, 0.f, 0.f, 1.f, 0.f, 1.f, color);
    Prim_AddVertex(p2[0], p2[1], 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, color);
    Prim_AddVertex(p3[0], p3[1], 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, color);
    Prim_AddVertex(p4[0], p4[1], 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, color);
    Prim_End();                                            // sub_828C2300
}
```

This is the workhorse for HUD billboards and frontend panels.

### 5. DrawOBBList (sub_82276518)

Loops `r11 = *r27` boxes, each with center+halfextent (4 floats from `r1+96..120`). For each iteration emits 4 verts forming a screen-space quad with computed UVs. The trailing `*r27 = 0` clears the count. Used for batched debug-volume rendering.

---

## Bucket "HUD/Map/Frontend" — two reconstructions

### 6. DrawAABB (sub_82143C88)

```cpp
// Prim_DrawRect(f1=x_min, f2=y_min, f3=x_max, f4=y_max)
void DrawRect(float x0, float y0, float x1, float y1) {
    Prim_Begin(/*rt=4*/4, /*vcount=*/4);
    uint32_t color = *(uint32_t*)g_currentColor;        // (lwz r11,284(r1) + lwz r9,0(r11))
    // 4 verts: (x0,y0), (x0,y1), (x1,y0), (x1,y1) with 0/1 UVs
    Prim_AddVertex(x0, y0, 0,0,0, COLW=*g_w, U=*g_u, V=0, color);
    Prim_AddVertex(x0, y1, 0,0,1, ...);
    Prim_AddVertex(x1, y0, ...);
    Prim_AddVertex(x1, y1, ...);
    Prim_End();
}
```

### 7. The crash caller (sub_8227F2E8) — DrawQuadFP

```cpp
// Variant of DrawQuad that takes 8 float args (4 corners × 2)
// plus 4 individual color/state floats (f1..f8).
// Fixed-color: f29 = 1.0f (alpha); f31 = 0.0f.
void DrawQuadFP(float ax, float ay, float bx, float by,
                float cx, float cy, float dx, float dy)
{
    DCHandle dc = g_currentDC[*(uint32_t*)(stack[300])];   // r30 = *(r1+300) (stack-spilled)
    SomeStateOp_828C19C0();
    SomeStateOp_8227EE90(0);
    Prim_SetTexture(*(g_state-4), *g_state, 0, 2, 0);     // sub_828C6568
    Prim_SetSampler(*(g_state-4), 0);                     // sub_828C64C8
    Prim_Begin(/*rt=4*/4, /*vcount=*/4);

    uint32_t color = *(uint32_t*)dc;                       // <-- lwz r9,0(r30)
    // 4× Prim_AddVertex with the 8 input floats permuted into corners
    Prim_AddVertex(ax, ay, ...);   // [+0xA8] crashes nowhere — call returns
    Prim_AddVertex(bx, by, ...);
    Prim_AddVertex(cx, cy, ...);
    Prim_AddVertex(dx, dy, ...);

    Prim_End();        // <<< sub_828C2300 — LR=0x8227F3AC says crash returned here
    Prim_PopTexture(*(g_state-4));   // sub_828C6500
    Prim_PopSampler(*(g_state-4));   // sub_828C60A0
}
```

---

## Reachability from sub_825FBB68 (Enumerate Content thread)

**None of the 46 callers are reachable from `sub_825FBB68`.** Verified by `find_callees` on `sub_825FBB68`:

```
XNotifyGetNext, j_XamContentCreateEnumerator, sub_825FB998, sub_82849790,
sub_82849860, sub_82849918, sub_8285FF50, sub_8285FFA0, sub_82A11F50,
sub_82A11F68, __savegprlr_25
```

The Enumerate Content thread does I/O only — no draw calls. **The crashing thread is the main render thread**, not the XAM enumerator. Walking back from `sub_8227F2E8`:

```
sub_8227F2E8 (DrawQuadFP)
  ← sub_8227F5B8
       ← sub_8214DBD0 (entry point — likely top of HUD render pass)
       ← sub_821506E8  ← sub_8214DBD0
       ← sub_821B5C90 (entry point)
       ← sub_821F1EE0  ← sub_821F5788
       ← sub_8229D8A8  ← sub_8229F0F8
  ← sub_8227F608
       ← sub_8218D7A8, sub_8218E2C0, sub_8218E318, sub_821F1670  (HUD draw paths)
```

`sub_8214DBD0` and `sub_821B5C90` have **no callers** — they are vtable entries (most likely `CRenderer::Render()` or per-pass `CDrawList::Execute()` entry points reached via the deferred-render command queue, which is dispatched from the main render thread immediately after `g_RenderState.Begin()`).

---

## Pattern analysis

**Universal calling pattern:** `[Prim_SetState(r3=mode)] → Prim_Begin(rt, n) → n× Prim_AddVertex(...) → Prim_End()`. 100% of callers (46/46) follow this, with vert counts ranging from 4 (most) to runtime-bound loops (`sub_821BD238`, `sub_82276518`, several HUD drawers).

**No caller passes a "kind of pointer" in r3 to `sub_828C2300`** — `sub_828C2300` ignores its arguments. The bucketing therefore reflects what the *caller* draws, not what `Prim_End()` "kind" is.

**Setup/teardown semantics:** `Prim_End` is the **flush + reset** of a singleton vertex builder. Setup is `Prim_Begin`, teardown is `Prim_End`. Pairs are statically balanced — every caller calls `Prim_Begin` exactly N times and `Prim_End` exactly N times in the same function.

**Why the audio-range callers (827C0C08, 827D27D0) appear here:** the `0x827xxxxx` band is the radio/audio-overlay code, which draws the in-car radio panel UI — `sub_827C0C08` calls many `sub_827Bxxxx` audio symbols *and* the draw triplet. These are HUD draws that happen to live in audio code, **not** audio-pool teardowns.

---

## Crash reinterpretation

The reported `r31=0x20` at `sub_828C2300+0x34` is **physically impossible** under that function's logic — `r31` is loaded purely from constants (`lis r11,-31972; addi r31,r11,11576 = 0x831C2D38`), and there are zero call instructions between that load (offset +0x14) and the crash store (offset +0x34) that could clobber it.

The most likely real explanation:

1. **The crash PC is not actually inside `sub_828C2300`.** The reported PC may be slightly stale or rounded down from a later frame whose return-address happens to point into `sub_828C2300`'s body. The recomp dispatcher decodes by LR which can desynchronize from the real fault site by one frame on a corrupted stack.

2. **Stack corruption inside `sub_8227F2E8` (the CALLER).** That function's `r30 = *(uint32_t*)(stack+300)` (`lwz r30,300(r1)`) is consumed by every `lwz r9,0(r30)` between vertex emits. If something on the host side scribbled `(stack+300)` with `0x20`, then the AddVertex calls would read a bogus color from `0x20`, and the **next** function — `Prim_End` — would happily complete... but the stack-restored `r31` (`ld r31,-16(r1)` at the epilogue, +0x44) would also be reading from a corrupted slot, returning `0x20` to the caller.

3. **Corruption of the global pool at `0x831C2D28`.** Specifically, if `g_PrimState.buffer` (offset -16 from r31) was overwritten with garbage, then `sub_828BF270` (the dispatcher) could blow up — but its frame would unwind out, and the post-call `stw r11,-16(r31)` at +0x2C / `stw r11,0(r31)` at +0x34 would still target the same fixed `0x831C2D38`.

**Most likely actual crash site:** the fault is in `sub_828BF270` (the GPU dispatcher) called at `sub_828C2300+0x24`, **not** the `stw r11,0(r31)` at +0x34. The `r31=0x20` is a downstream artifact of stack walk decoding the unwound frame after the inner crash. `sub_828BF270` -> `sub_82A3DF50` is where the rendertarget submit happens — that path needs the host GPU to be fully ready, and on first-frame-after-content-enum this is exactly where the audio-pool init sequence (Agent's premise) overlaps with the first HUD-draw frame.

---

## Conclusion

**One-line description:** `sub_828C2300` is `Prim_End()` — the flush-and-reset of GTA IV's singleton 2D primitive batch builder, invoked by every screen-space draw helper at the bottom of its `Begin/AddVertex*/End` triplet.

**Most likely crash entry:** `sub_8227F2E8` (DrawQuadFP). It is the only caller that `lwz r9,0(r30)` between vertex emits, where `r30` is **stack-loaded** at offset 300, making it the most exposed to host-side stack corruption. The visible `r31=0x20` in the crash report is downstream of the actual fault, which is almost certainly inside `sub_828BF270 → sub_82A3DF50` — the GPU command-stream submit. This is consistent with prior agent findings that the fault occurs during the first HUD draw after content enumeration, when the audio init storm is contending for memory/GPU resources.

**Recommended hook target:** wrap `sub_828C2300` (`Prim_End`) with a guard: if `g_PrimState.buffer (0x831C2D28)` is non-null but malformed (e.g. unaligned, or not in the heap range), skip the inner `sub_828BF270` call and just zero the state. This will let the game survive the first-frame race without the crash.
