# Agent 9: Handle Type Inference — UAF in sub_8227F2E8

## TL;DR

The "handle" crashing with `0xE1E1E1E1` poison is a **`uint32_t*`** pointing to an ARGB colour field inside a heap-allocated **`CBaseDC`-derived draw command** (most likely **`CDrawSpriteDC`** or a closely related sprite-quad DC) that was enqueued to the render list but freed before `sub_8227F2E8` dereferences it.

The handle is NOT an object. It is a **raw pointer to `uint32_t colour`** (rage::Color32 encoded as packed 8.8.8.8 ARGB).

## Task 1 — Every r30 offset read in sub_8227F2E8

`sub_8227F2E8` has a stwu of -208 bytes, then `lwz r30, 300(r1)`. That loads r30 from its own stack at +300, which is `caller_sp + 92` (verified with Python: 300 - 208 = 92 = 0x5C = stack arg 9 slot per Xenon ABI).

Offsets read off r30 (exhaustive enumeration of the whole function body):

| Insn | Offset | Used in |
|-|-|-|
| `lwz r9, 0(r30)` | **+0** | passed to `sub_828C2290` as r9 (vertex-emit helper, writes into `vertex+24`) |
| `lwz r9, 0(r30)` | **+0** | 2nd vertex push |
| `lwz r9, 0(r30)` | **+0** | 3rd vertex push |
| `lwz r9, 0(r30)` | **+0** | 4th vertex push |

**Only offset 0 is read. Four identical loads (one per quad corner).** No +4, +8, +12, etc.

`sub_828C2290` is a 112-byte leaf called 267x (hot). It writes a 36-byte vertex into a ring-buffer at offset r11 (= `base[-31972*65536 + 11560]`, a globals pointer). Vertex layout:

```
+0  stfs f1   ; x
+4  stfs f2   ; y
+8  stfs f3   ; z
+12 stfs f4   ; ?
+16 stfs f5   ; ?
+20 stfs f6   ; ?
+24 stw  r9   ; <-- THE COLOUR (packed ARGB u32)
+28 stfs f7   ; u
+32 stfs f8   ; v
```

So `r9 = *r30` is stored verbatim into vertex slot +24. Standard GTA IV `rage::grcVertex2d` tint colour.

## Task 2 — Matching field offsets to a known class

**Only +0 is used**, and it is a **packed u32 D3DCOLOR**. This matches `rage::Color32` (a trivial struct wrapping a single `uint32_t`), i.e. the handle is effectively a `rage::Color32*` or `uint32_t*`.

### Caller proof

5 callers of `sub_8227F5B8` (8-arg wrapper that delegates to F2E8) and 4 callers of `sub_8227F608`. Strings on those callers confirm the domain:

- `sub_821B5C90` — "LOADING...", "CHARGEMENT...", "CARGA...", "BELADUNG...", "CARICAMENTO..." + `"FADE_LOAD"`
- `sub_8214DBD0` — `"LOADLANG"`, `"NO_PAD"`, `"$%d"`, `"%d$"`
- `sub_8218E318` — `"None"` (HUD/glyph fallback)

This is the **2D HUD / fade / font quad renderer**.

### Caller pattern 1: stack-local colour

`sub_8218D7A8`:
```
stw  r11, 84(r1)     ; r11 = -1 (white = 0xFFFFFFFF)
addi r5, r1, 84      ; r5 = &stack[84]
bl   sub_8227F608    ; forwards r5 as the "handle" (= &colour)
```
→ r30 points at a local `uint32_t colour = 0xFFFFFFFF`.

### Caller pattern 2: computed 4-corner colours

`sub_821F1670` and `sub_8218E318` call `sub_828BF810` four times, store each returned u32 into stack at +96/+100/+104/+108, and pass `r1+180` (not the colour array — a separate single u32) as the handle. So even here the handle is still just `uint32_t*`.

### Caller pattern 3 (the heap-allocated one — where the UAF happens)

`sub_8218DD80` is the smoking gun path:

```
li   r3, 228                 ; allocate 228-byte object
bl   sub_821BB3D8            ; object pool allocator (leaf, 436 callers, hot)
mr   r31, r3                 ; r31 = new 228-byte DC
lis  r9, -32244              ; r9 = 0x820BE3C0 (VTABLE)
addi r9, r9, -7232
stw  r9, 0(r31)              ; this->vtable = 0x820BE3C0
stw  r8, 4(r31)              ; link/flags
lis  r11, -32231             ; r11 = 0x8218DB88 (= sub_8218DB88)
addi r11, r11, -9336
stw  r11, 8(r31)             ; this->drawFn = sub_8218DB88
bl   sub_82181EC0            ; init DC header
li   r5, 196
mr   r4, r30                 ; r30 = r4(caller) + 20 (source vertex array)
mr   r3, r29                 ; r29 = r31 + 12 (dest)
bl   sub_82A00DC0            ; memcpy(this+12, src, 196 bytes)
stfs f29, 208(r31)
stfs f28, 212(r31)
stfs f31, 216(r31)
stfs f30, 220(r31)
stw  r28, 224(r31)           ; 4 floats + 1 int = remaining 20 bytes → total 228
```

**So the 228-byte heap object is a CBaseDC subclass** whose vtable is 0x820BE3C0 (not in the public symbol table but adjacent to the `CB_Generic_4Args_Color32` / `CB_Generic_4Args_grcTexture_CRect_Color32` vtables at 0x820BE2FD / 0x820BE319 — both descend from `CBaseDC`, confirmed via `search_symbols CBaseDC`: 108 child classes).

### Drawing virtual method

`sub_8218DB88` is invoked to render the DC. It reads 4 colour fields from `this+88`, `this+100`, `this+112`, `this+124` (4 u32 ARGB values), each paired with a format enum at `+92/+116/+128` compared to `20`. Format `20` = `D3DDECLTYPE_D3DCOLOR` (Xbox packed ARGB declaration type). After the `+20` header offset in the DC this maps to struct base +108, +120, +132, +144 — a `grcVertex2d[4]` array with colours.

For each of the 4 vertices, `sub_8218DB88` does:

```
stw  r9, -18920(r31)      ; spill colour to TLS/global (because sub_828C2290 clobbers r9)
bl   sub_828C2290         ; vertex 0
lwz  r9, -18920(r31)      ; reload
bl   sub_828C2290         ; vertex 1 with same colour (two tris share a vert)
```

**Confirming the same +24 vertex-slot write path `sub_8227F2E8` uses.**

### Which CBaseDC subclass is it?

Searching `search_symbols CDraw` lists 108 candidates including:
- `CDrawSpriteDC` (vtable 0x8200146C) — sprite quad, virtual draw calls `sub_821BD2D8` → forwards `this+8, this+16, this+24, this+32, this+44` to `sub_8227E948`
- `CDrawRectDC` (vtable 0x820014C0) — rect, virtual draw calls `sub_821BD528(r3=this+8, r4=this+24)` → forwards to `sub_8227F658` → eventually into the same vertex emitter chain
- `CDrawSpriteUVDC`, `CDrawSpritePerspDC`, `CDrawSpriteInPerspectiveDC`, `CDrawTriShapeDC`, `CDrawCurvedWindowDC` — all plausible

Note `CDrawRectDC` passes `this+24` as the colour pointer (r4 = &u32 at `this+24`) — this is **exactly** the handle pattern we see at `sub_8227F2E8`. The virtual dispatcher routes r4 through the DC's specific draw method into `sub_8227F608` / `sub_8227F658` / `sub_8227F2E8`.

The 228-byte DC at 0x820BE3C0 (not CDrawRectDC's 0x820014C0) is a different sprite-quad variant — probably an anonymous lambda-style `__T_CB_Generic_4Args_P6AXPAVgrcTexture_rage__AAVCRect__1AAVColor32_2_` (adjacent vtable) bound with a pooled DC.

## Task 3 — Type of the +0 field

`uint32_t packed ARGB / rage::Color32`. Not a vtable, not a material pointer. The bug isn't a virtual call crash — it's a **value read** (`lwz r9, 0(r30)`) into a vertex stream. Because the holding DC was freed, the 4 bytes read back as `0xE1E1E1E1`, which the renderer faithfully pushes into all 4 vertex colours of the quad. The rendered quad then appears as the tell-tale RAGE free-poison tint (medium-pink / rgba (225,225,225,225)).

## Task 4 — Vtable trace

Not applicable: +0 is a raw colour word, not a vtable slot. The vtable on the **holding DC** lives at `this+0` (= `0x820BE3C0`), but `sub_8227F2E8` never touches the DC's vtable. It only touches `&DC.colour` which was handed off via `r4`/`r5` and survived into `r30`.

The UAF is:

1. Caller (e.g. the loading screen / HUD fade at `sub_821B5C90`) allocates a 228-byte DC via `sub_821BB3D8`, writes vertex data + 4 colour fields, enqueues it, and gets a pointer to `&DC.colour`.
2. Caller then calls `sub_8227F5B8(... , r4 = &DC.colour, ...)` which spills r4 to its own stack at +92.
3. Before `sub_8227F2E8` runs four `lwz r9, 0(r30)` loads, **the enclosing DC has been freed** (render list flushed / pool reclaimed) and RAGE has stamped `0xE1E1E1E1` over the payload.
4. All four quad vertices get colour `0xE1E1E1E1`.

## Summary table

| Property | Value |
|-|-|
| `r30` register role | stack arg 9 (`caller_sp + 92`) |
| `r30` static type | `const uint32_t*` (aka `rage::Color32*`) |
| Field read at `*r30` | packed ARGB u32 |
| Use of value | `vertex[i].colour = *r30` (written into vertex ring-buffer slot `+24`) |
| Number of reads | 4 (one per quad corner) |
| Owner object | 228-byte `CBaseDC` subclass (vtable 0x820BE3C0, draw-fn `sub_8218DB88`) |
| Owner allocator | `sub_821BB3D8` (hot pool allocator, 436 callers) |
| Owner layout | vtable @+0, link @+4, drawFn @+8, vertex array @+12 (196 bytes), 5 trailing floats/ints @+208..+228 |
| Free-poison origin | `0xE1E1E1E1` written across payload when DC is reclaimed |
| Candidate class name | unnamed `CBaseDC` subclass adjacent to `CB_Generic_4Args_grcTexture_CRect_Color32` — behaviourally a **textured sprite-quad DC** in the HUD/fade/loading screen render list |

## Fix direction (for later agents)

Do NOT try to keep the `rage::Color32*` alive. The correct fix is making sure the **DC that owns the colour** is not freed before the render list flushes. That likely means either:
- extending the DC's lifetime to match the render-list dispatch boundary, or
- copying the u32 colour eagerly into the vertex at enqueue time (before the DC can be freed), making the pointer dereference unnecessary.

The recomp of `sub_8227F2E8` itself is correct — the UAF is upstream in the DC lifetime contract.
