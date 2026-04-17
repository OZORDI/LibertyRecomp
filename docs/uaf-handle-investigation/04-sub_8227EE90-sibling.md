# sub_8227EE90 — Sibling of DrawQuadFP

**Address**: 0x8227EE90 | **Size**: ~432 bytes (0x1B0) | **Callers**: 2 (sub_8227F2E8 "DrawQuadFP", sub_8227F458) | **Hooked**: no

## Role — 2D Draw Context / Viewport Setup (not a handle allocator)

`sub_8227EE90` is the **viewport-and-projection setup helper** the 2D quad-draw family calls *before* issuing a quad. It is **NOT** the constructor/allocator of the handle that DrawQuadFP loses; it is a cache-guarded setter that configures a *different* global state block (transform / screen dims / orthographic projection), and — when dimensions change — calls `sub_828CDAE0` which iterates a parent `CRenderTarget`-style struct and invokes `sub_828C8A50` 9×1 to push constants/state to the GPU.

**Signature (derived from caller conventions):**
```c
void sub_8227EE90(bool forceUpdate_r3);
```
`r3` (saved to `r29`) is a single-byte boolean. Both call sites pass **`li r3, 0`** — "only re-upload if dimensions changed."

## Control Flow (decoded from recomp)

Base global struct: **0x82C6C1B4** (`r30` throughout). Fields:

|offset|meaning|evidence|
|-|-|-|
|+0|vtable/this pointer of a draw-context object|`lwz r3,0(r30)` → passed as `this` to `sub_828CDAE0`|
|+24|integer arg passed as `r4` to `sub_828CDAE0`|`lwz r4,24(r30)`|
|+40|byte: "skip this helper" master switch|`lbz r11,40(r30); bne 0x8227f034` (early-out)|

Cached dimensions/viewport (written on change, compared on re-entry):

|addr|size|role|
|-|-|-|
|0x82C6C1E4|u32|cached width (r31 = int(srcW × r11[648] scale))|
|0x82C6C1E8|u32|cached height (r10 = int(srcH × r11[652] scale))|
|0x82C6C1EC|u16|cached viewport/surface id from 0x82C01CB4 (lhz r9,7348(r11c))|

**Entry byte-flag guard** (`[0x82C6C1DC]==0`): skips all work and returns immediately. Likely a "UI not initialized / suppressed" flag.

**Branch A — custom viewport active** (`[0x831C2200]!=0`, a `CRenderTargetViewport*`):
- Loads `r10 = ptr[688]` (raw width int), `r9 = ptr[692]` (raw height int)
- Loads `f0 = ptr[648]`, `f13 = ptr[652]` (float scale factors)
- Converts via std/fcfid/frsp/fmul/fctiwz (signed-int ← float(int × scale)) → `r31`, `r10` hold scaled dims

**Branch B — default device** (0x8227EF28):
- `sub_82146700()` → returns either `[lbl-19372]` or `[lbl-19364]` (device-table width — primary/back buffer branch)
- `sub_82155300()` → same pattern, returns height (hot func: 46 callers, 172 downstream calls)

**Cache check (0x8227EF38):**
```
if (cached_vp == current_vp &&
    cached_w == r31 && cached_h == r10 && forceUpdate_r3 == 0)
    return;      // no-op, dims unchanged
```

**Update path (0x8227EF80):**
1. Writes cached triple (vp, w, h) to globals 0x82C6C1E4/E8/EC.
2. Stacks up a 16×float matrix on `r1+96..r1+156`: diagonal identity + zeros, with `[116]=1.0/float(h)` and `[96]=1.0/float(w)` — **classic orthographic screen-to-NDC matrix** (constants loaded from lbl 0x82000A34=0.0f, 0x82000D48=1.0f, 0x82000DC8 and 0x82000D64 for the scale numerators).
3. Loads `r3 = [r30+0]` (draw-context `this`), `r4 = [r30+24]` (arg), `r5 = &local_matrix`, and calls **`sub_828CDAE0(this, arg, matrix)`** — which iterates `hword[this+12]` (9 entries) through array chains at `[this+56]` and `[this+8]`, calling `sub_828C8A50` with `(subarray+24, subarray+20, 1, 9, 64, matrix)` — i.e. **pushing the projection matrix to 9 shader constant buffers / programmable-pipeline slots** (class=9, count=1, stride=64 bytes = float[4][4]).

## Relationship to DrawQuadFP (sub_8227F2E8)

DrawQuadFP's call order:
1. `sub_828C19C0()` — unknown setup.
2. `sub_8227EE90(0)` — **this function**, syncs projection/viewport constants.
3. `sub_828C6568`, `sub_828C64C8` — draw-state binds (using `[0x82C6C1B8]` and `[0x82C6C1BC]` as r3 args — a *different* global pair at r31_F2E8=0x82C6C1BC).
4. `sub_828C21D0(4,4)` — begin-primitive.
5. `lwz r30, 300(r1)` ← **the bug**: reads `r30` handle pointer from the *stack frame*, not from the globals 8227EE90 writes.
6. 4× `sub_828C2290(...,f1..f8,r9=[r30+0])` — 4 vertices, each dereferencing `[r30+0]`.

**Critical finding**: `stack+300` was established by the prologue as `std r30,-24(r1); stwu r1,-208(r1)` (saved caller's r30 at callerSP-24 == currentSP+184, **not 300**). The `lwz r30,300(r1)` at 0x8227F390 is reading an **uninitialized** stack slot — that's the UAF vector. Nothing in DrawQuadFP writes to `r1+300` before this read. `sub_828C19C0` / `sub_8227EE90` / `sub_828C6568` / `sub_828C64C8` / `sub_828C21D0` are candidates for stashing the quad-batch handle there as an out-param, but `sub_8227EE90` writes **only** to its own r1+80..r1+160 locals (off-stack from F2E8's r1 because of F2E8's own 208-byte stwu) — **sub_8227EE90 does NOT populate stack+300**.

## Does sub_8227EE90 allocate/free the poisoned handle?

**No.** Evidence:
- Writes only to 3 globals (0x82C6C1E4/E8/EC — cached dims) and a transient local matrix.
- Never touches any heap allocator (`sub_8218BE28`, `RtlAllocateHeap`, `RtlFreeHeap`).
- Its only "heavy" call is `sub_828CDAE0`, which iterates an existing constant-buffer array; it does not allocate.
- The global pair DrawQuadFP uses at `[r31_F2E8-4]=0x82C6C1B8` and `[r31_F2E8+0]=0x82C6C1BC` is **separate** from EE90's struct base at 0x82C6C1B4/+24 — EE90 writes `[r30+0..+40]` region (0x82C6C1B4..0x82C6C1DC) but never `+4` or `+8`.

**Conclusion**: Sibling = **projection-matrix uploader**, shares the same base struct (0x82C6C1B4 — likely a `CSprite2d`/`CFillPoly` renderer singleton) but operates on a different slice. The UAF source is elsewhere. The real candidates are **sub_828C19C0** (runs first, could set `stack+300` via caller save of r30) or a missing prologue-write we haven't seen — the lwz at stack+300 is reading a caller frame slot that's only valid if DrawQuadFP was tail-called with a specific r30 in the caller's frame (the caller being sub_8227F658 or sub_8227F458 sibling chain).

## Action items for the wider UAF hunt

- **Clear**: 8227EE90 is *not* the bug. Eliminate from suspect list.
- **Next**: decompile `sub_828C19C0` (F2E8's very first callee) — it may produce the quad-batch handle and write it to `r1+300` via the F2E8 caller's frame (r30 in caller's frame aliases r1+some_offset). Alternatively `sub_828C64C8` / `sub_828C21D0` may return the batch handle in r3 and F2E8 is missing a `stw r3, 300(r1)` — compiler omitted store, or the recomp is losing an instruction near 0x8227F364–0x8227F390.
- Cross-check `sub_8227F458` — second caller of 8227EE90 — to see if it also reads `stack+300` or stores to it before the shared helper is called.
- The poison 0xFFE1E1E1 value implies `free()` fill-on-free — look for `sub_8218BE28(free)` on a struct that had `[+0]=render_vtable_ptr`; that struct is whatever F2E8 is trying to use at `[r30+0]`.
