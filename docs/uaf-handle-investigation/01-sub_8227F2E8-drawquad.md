# UAF Investigation 01 — `sub_8227F2E8` (DrawQuad helper)

Target function at guest address **0x8227F2E8**, ~368 bytes.
Recomp file: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.10.cpp`
(function body: `PPC_FUNC_IMPL(__imp__sub_8227F2E8)`)

This is the central render helper that takes four corner positions (two
x/y pairs + two u/v pairs) plus a colour/handle argument and emits four
immediate-mode vertices into the current RAGE draw buffer by calling
`sub_828C2290` (`grcDraw::AddVertex`, a leaf, 267 xrefs) four times.

The freed-poison value `0xFFE1E1E1` that lands on vertex colour is fetched
in this function — it comes from **a pointer on the caller's stack that is
already dangling by the time `sub_8227F2E8` dereferences it**.

---

## 1. Recomp annotation (full body)

All comments prefixed `//` are PPC mnemonics from the generator; the plain
text lines are the reconstruction.

### 1.1 Prologue / frame set-up  (0x8227F2E8 – 0x8227F30C)

```
mflr    r12
stw     r12,-8(r1)              ; save LR  at caller_r1-8
std     r30,-24(r1)             ; save r30 at caller_r1-24
std     r31,-16(r1)             ; save r31 at caller_r1-16
addi    r12,r1,-24              ; r12 = &save area
bl      __savefpr_21            ; spills f21..f31 (11 non-volatile FPRs) below r12
stwu    r1,-208(r1)             ; allocate 208-byte frame, link back-chain
```

Non-volatile FPR save area (per PPC `__savefpr_21` convention) uses
8 bytes × 11 FPRs = 88 bytes immediately below the GPR save slot.

### 1.2 Argument preservation  (0x8227F310 – 0x8227F330)

Arguments arrive in `r3, r4, f1..f9` (9 floats + 2 ints).  The prologue
immediately shuffles the 8 incoming floats into callee-saved FPRs so
they survive the upcoming helper calls:

| In FPR | Saved to | Meaning (derived from later AddVertex patterns) |
|-|-|-|
| f1 | f28 | x0 — left x |
| f2 | f27 | y0 — top y |
| f3 | f26 | x1 — right x |
| f4 | f25 | y1 — bottom y |
| f5 | f30 | z (depth) |
| f6 | f29 | *(overwritten below with -1.0 constant — NOT used)* |
| f7 | f24 | u0 — left texture U |
| f8 | f23 | v0 — top texture V |
| f9 | f21 | v1 — bottom texture V |

(`f22`, which is used as u1 in the third and fourth AddVertex, is NOT
restored from an input FPR here — it is set by the wrapper before call;
see §5.)

Then the fixed-zero integer args:

```
li      r4,0
li      r3,0
```

### 1.3 Set-up calls  (0x8227F330 – 0x8227F370)

```
sub_828C19C0(0, 0);                    ; grcBegin-style: set render-state flag
                                        ; (huge 37-case dispatcher, select = r3=0)
sub_8227EE90(0);                        ; prepare GPU resources / push shader
r31 = 0x82C6C1BC;                       ; addi r31,r11,-15940 after lis r11,-32057
                                        ; → pointer into a static RAGE device table
r6 = *(u32*)(r31 + 0)   = *0x82C6C1BC;  ; arg6 = device/context handle word
r3 = *(u32*)(r31 - 4)   = *0x82C6C1B8;  ; arg0 = driver-object pointer
sub_828C6568(r3, 2, 0, *0x82C6C1BC);   ; Bind/SetStreamSource-like; arg2=2 (mode)
sub_828C64C8(*0x82C6C1B8, 0);          ; Set FVF/decl (second arg = 0)
sub_828C21D0(4, 4);                     ; begin primitive: prim=4, count=4
                                        ; this is RAGE's immediate-mode Begin()
```

### 1.4 **First vertex load — the fault site**  (0x8227F370 – 0x8227F3AC)

```
lwz     r30, 300(r1)            ; <-- RELOAD OF SPILLED CALLER ARG
                                 ;     (see §3 for why +300)
lfs     f29, -4876(r11)         ; f29 = *0x820BECF4 = -1.0f   (BF800000)
lfs     f31, 2612(r11)          ; f31 = *0x82000A34 =  0.0f   (00000000)
lwz     r9,  0(r30)             ; <-- COLOUR / HANDLE FETCH   ***
                                 ;     single 32-bit word at r30+0
fmr     f1,f28  f2,f27          ; (x0, y0)
fmr     f3,f30                  ; z
fmr     f4,f31  f5,f31          ; (0, 0) — rhw / pad / pad
fmr     f6,f29                  ; -1.0   (unused slot)
fmr     f7,f24  f8,f23          ; (u0, v0)
sub_828C2290(f1..f8, r9);       ; AddVertex #1 — top-left
```

### 1.5 AddVertex 2 — top-right  (0x8227F3AC – 0x8227F3D4)

```
lwz     r9, 0(r30)              ; same colour refetched from same pointer
fmr     f1,f28  f2,f25          ; (x0, y1)   <-- y1, i.e. bottom
fmr     f3,f30                  ; z
fmr     f4,f31  f5,f31          ; (0, 0)
fmr     f6,f29                  ; -1.0
fmr     f7,f24  f8,f21          ; (u0, v1)
sub_828C2290(...);              ; AddVertex #2
```

Hm — note the vertex order: this second call swaps **y** to y1 while
keeping x = x0 and u=u0, v=v1.  That is the **bottom-left** vertex.

### 1.6 AddVertex 3 — bottom-left  (0x8227F3D4 – 0x8227F3FC)

```
lwz     r9, 0(r30)
fmr     f1,f26  f2,f27          ; (x1, y0)  — u1 implicit in f22
fmr     f3,f30
fmr     f4,f31  f5,f31
fmr     f6,f29
fmr     f7,f22  f8,f23          ; (u1, v0)
sub_828C2290(...);              ; AddVertex #3 — top-right
```

### 1.7 AddVertex 4 — bottom-right  (0x8227F3FC – 0x8227F424)

```
lwz     r9, 0(r30)
fmr     f1,f26  f2,f25          ; (x1, y1)
fmr     f3,f30
fmr     f4,f31  f5,f31
fmr     f6,f29
fmr     f7,f22  f8,f21          ; (u1, v1)
sub_828C2290(...);              ; AddVertex #4
```

### 1.8 Epilogue  (0x8227F424 – 0x8227F44C)

```
sub_828C2300();                 ; End() — flush / submit primitive
r3 = *0x82C6C1B8;
sub_828C6500(r3);               ; unbind vertex decl
r3 = *0x82C6C1B8;
sub_828C60A0(r3);               ; release/return render-list entry
addi    r1,r1,208               ; tear down 208-byte frame
addi    r12,r1,-24
bl      __restfpr_21            ; restore f21..f31
lwz     r12,-8(r1) ; mtlr r12
ld      r30,-24(r1) ; ld r31,-16(r1)
blr
```

---

## 2. Reconstructed pseudo-C  (richly typed)

```c
// RAGE grcDraw::DrawQuad2D(x0,y0, x1,y1, z, u0,v0, v1, handle_or_color_ptr)
// All floats are 32-bit passed via doubled FPRs; handle arrives as a
// single 32-bit pointer spilled by the wrapper one frame up.
//
// Address: 0x8227F2E8   Callers: sub_8227F5B8, sub_8227F608 (both wrappers)

static uint32_t* const kDevice  = (uint32_t*)0x82C6C1B8; // driver obj ptr slot
static uint32_t* const kCtxWord = (uint32_t*)0x82C6C1BC; // context word

void DrawQuad2D(float x0, float y0, float x1, float y1,
                float z,
                float u0, float v0, float v1,
                uint32_t* /* spilled on caller stack at +92 */ handlePtr)
{
    sub_828C19C0(0, 0);                 // begin draw state
    sub_8227EE90(0);                    // prepare shader / buffers

    const uint32_t ctxWord = *kCtxWord;
    void*    driver        = (void*)*kDevice;

    sub_828C6568(driver, /*mode*/2, 0, ctxWord);
    sub_828C64C8(*kDevice, 0);          // set vertex decl / FVF
    sub_828C21D0(/*prim=*/4, /*count=*/4);

    const float minus1 = -1.0f;   // slot f6 in AddVertex (unused by shader)
    const float zero   =  0.0f;   // slots f4, f5 in AddVertex

    // --- handlePtr is re-loaded from the caller's spill slot HERE ---
    // NOTE the generator emits  lwz r30, 300(r1)  which maps to the
    // wrapper-supplied pointer; see §3 for the offset derivation.
    uint32_t* p = handlePtr;   // <-- caller passed a POINTER, not a colour

    uint32_t color;

    // Vertex 1: top-left
    color = p[0];                                       // <<< FAULT: UAF here
    sub_828C2290(x0, y0, z, zero, zero, minus1, u0, v0, color);

    // Vertex 2: bottom-left  (x0, y1, u0, v1)
    color = p[0];
    sub_828C2290(x0, y1, z, zero, zero, minus1, u0, v1, color);

    // Vertex 3: top-right    (x1, y0, u1, v0)
    color = p[0];
    sub_828C2290(x1, y0, z, zero, zero, minus1, u1_from_wrapper, v0, color);

    // Vertex 4: bottom-right (x1, y1, u1, v1)
    color = p[0];
    sub_828C2290(x1, y1, z, zero, zero, minus1, u1_from_wrapper, v1, color);

    sub_828C2300();                     // End()
    sub_828C6500(*kDevice);             // unbind
    sub_828C60A0(*kDevice);             // release
}
```

`u1_from_wrapper` comes in through `f9` in wrapper `sub_8227F608`, which
itself stores it into `f22` inline prior to the call (it is not touched
inside `sub_8227F2E8` — sub_8227F2E8 never renames `f9`, so `f22` must
have been established by the wrapper.  Actually: the wrapper doesn't
spill to f22 either — so in the `sub_8227F5B8` path where only 4 floats
of corner data arrive, `f22` holds a stale value.  See §5.)

---

## 3. Stack frame diagram

Offsets are relative to `r1` **after** the `stwu r1,-208(r1)`.
The caller's `r1` was `r1+208` at entry; the wrappers reserve an extra
112 bytes above that, so the spilled pointer sits at wrapper_r1+92
which equals callee_r1 + 208 + 92 = **callee_r1 + 300**.

```
offset  size  contents
------  ----  --------------------------------------------------------
+300     4   [SPILLED HANDLE POINTER]   <-- lwz r30,300(r1) reads this;
                                            written by wrapper via
                                            stw r4,92(r1) / stw r5,92(r1)
                                            before this call frame existed

 ...  (wrapper's own frame: args, LR save, etc.)

+208    ->  top of this function's own 208-byte frame
+ 200       back-chain to wrapper frame     (implicit via stwu)
  ...       local area (unused by this function)
+   8       back-chain overlap
+   0       saved r1 back-chain (written by stwu)

-  8       saved LR  (stw r12,-8(r1) BEFORE stwu, so relative to
               entry r1, i.e. callee_r1 + 200)
- 16       saved r31 (high 64 bits)
- 24       saved r30 (high 64 bits)
- 32..-120  saved f21..f31 (11 × 8 = 88 bytes) via __savefpr_21
           (starting at r12 = caller_r1-24 and growing down)
```

No outgoing-args region is used (all callees take <=8 int + <=13 FPR).

**The hand-off from wrapper to callee:**

```
Wrapper sub_8227F5B8:     Wrapper sub_8227F608:     Callee sub_8227F2E8:
  stwu r1,-112(r1)         stwu r1,-112(r1)           prologue saves, then
  ...                      ...                         stwu r1,-208(r1)
  stw  r4, 92(r1)          stw  r5, 92(r1)            ...
  bl   sub_8227F2E8        bl   sub_8227F2E8            lwz r30, 300(r1)  <-- reads spill
                                                         lwz r9,   0(r30)  <-- UAF
```

---

## 4. Argument list & calling convention

**PPC Xbox 360 ABI (ELFv1-derived).**  Integer args in `r3..r10`,
float args in `f1..f13`, all floats doubled on the wire.

For `sub_8227F2E8`:

| slot | register | role | type | comes from |
|-|-|-|-|-|
| 0 | r3   | unused (zeroed early) | — | |
| 1 | r4   | unused (zeroed early) | — | |
| 2 | f1   | x0    | float  | wrapper |
| 3 | f2   | y0    | float  | wrapper |
| 4 | f3   | x1    | float  | wrapper |
| 5 | f4   | y1    | float  | wrapper |
| 6 | f5   | z     | float  | wrapper const (0.0 in F5B8) |
| 7 | f6   | u0    | float  | wrapper |
| 8 | f7   | v0    | float  | wrapper |
| 9 | f8   | v1    | float  | wrapper |
| 10| f9   | (u1 in F608 path; stale in F5B8 path) | float | wrapper |

**Handle pointer is NOT in a register** — it is passed via the
stack-spill convention used by these two wrappers:

- `sub_8227F5B8` enters with the handle in `r4`, does `stw r4, 92(r1)`.
- `sub_8227F608` enters with the handle in `r5`, does `stw r5, 92(r1)`.
- `sub_8227F2E8` reads it back as `lwz r30, 300(r1)` after allocating
  its own 208-byte frame.

This is an unusual "pointer-via-stack-scratchpad" idiom — probably a
leftover from the original RAGE inlined helper that took the colour by
reference to avoid a FP-int round trip.

---

## 5. Offsets read off `r30`  (the critical question)

```
r30 = *(u32*)(callee_r1 + 300)   ; pointer loaded from caller spill

lwz r9, 0(r30)                   ; 32-bit load at OFFSET +0  (x4)
```

**Only offset +0 is ever read.**  No `+4`, `+8`, `+C` load, no `lfs` at
non-zero offsets, no indirect call off `(r30)`, no `lwzx` at variable
offsets.  r30 is used only as a pointer-to-uint32 to fetch a single
word which is then passed as the colour argument in `r9` to
`AddVertex` (`sub_828C2290`'s store: `stw r9, 24(r11)` — colour slot
in a 36-byte RAGE vertex record at offset +24).

### 5.1 Is r30 a pointer to the freed object?

**Yes — r30 is a raw pointer into guest heap memory that has already
been freed by the time this draw executes.**  The evidence:

1. `0xFFE1E1E1` is RAGE's canonical freed-heap fill byte pattern
   (0xE1 repeated) — it is never a valid colour (ARGB alpha=0xFF,
   RGB=0xE1E1E1 is plausible, but the specific byte `0xE1` is the fill).
2. The load width is a single 32-bit word at offset 0.
3. There is **no vtable dereference** (no `lwz rN, 0(r30)` followed by
   `lwz rM, offN(rN)` and `mtctr`).  So `r30` is NOT a polymorphic
   RAGE class pointer — it does **not** point to an object whose first
   word is a vtable.
4. The fact that the only dereferenced offset is +0 means r30 points
   at **a single 32-bit value in memory** — most likely the colour
   field of a `Color32` or of the first element of a graphic-resource
   struct (texture / grcTexture wrapper).

### 5.2 What does offset 0 mean semantically?

Given the surrounding context — this helper is a textured 2D quad
drawer, not a solid-coloured one (it binds a vertex decl and uses
u/v coordinates) — offset 0 on the freed object is almost certainly
either:

- **Option A**: the vertex colour / tint of a `grcTexture` wrapper
  or `CSprite` record (most common layout: first 4 bytes = ARGB
  modulate colour).
- **Option B**: the first 4 bytes of a freed `grcTexture*` —
  in which case this is a use-after-free of a texture object whose
  first slot got fill-poisoned, being interpreted as a colour word.

Option A is much more likely, because:

- A `grcTexture*` would normally be dereferenced for its bind method,
  which we do not see here; the binding happens in `sub_828C64C8` via
  the static device pointer, unrelated to r30.
- Loading `0(r30)` four times in a row, each feeding the colour slot,
  matches a "tint/colour/alpha" scalar being sampled per vertex (even
  though it's constant across the four vertices — a minor RAGE
  inefficiency).

### 5.3 Inferred handle type

**`uint32_t*` into a freed RAGE heap block, pointing at the first word
of either a `CRGBA`/`Color32`/ARGB modulator struct, or the colour
field of a larger draw-descriptor whose object was released before the
draw executed.**

It is not a polymorphic object (no vtable read) and it is not an
indexed handle table entry (no `& mask ; base + idx` pattern).

---

## 6. Findings

1. **`sub_8227F2E8` is RAGE's `DrawTexturedQuad2D` helper.**  It emits
   four vertices via `sub_828C2290` (`grcDraw::AddVertex`) by calling
   Begin/End around a vertex-decl bind/unbind pair (`sub_828C21D0`,
   `sub_828C2300`, `sub_828C64C8`, `sub_828C6500`).

2. **The poisoned value reaches AddVertex through `r9`, which is
   loaded as `*(uint32_t*)r30` where r30 itself is reloaded from
   `r1+300` in this frame — a stack spill written by either
   `sub_8227F5B8` (stw r4,92) or `sub_8227F608` (stw r5,92) in the
   wrapper's 112-byte frame.**  The hand-off offset math is exact:
   wrapper_r1 + 92 = callee_r1 + 208 + 92 = callee_r1 + 300.

3. **Only offset 0 of the pointed-to object is read, four times.**
   There is no vtable load, no method dispatch, no secondary-field
   reads — so `r30` points to a plain 32-bit colour/tint word in a
   freed heap block, not to a polymorphic RAGE class.  The caller is
   handing in `&modulate_colour` where `modulate_colour` has been
   freed.

4. **Constant pools** used by this function are legitimate (`-1.0f` at
   `0x820BECF4`, `0.0f` at `0x82000A34`, `1.0f` at `0x82000D48`,
   verified by reading `default.bin`).  The TOC pointer `r31 =
   0x82C6C1BC` points to a static RAGE device/context record which
   remains live — so the crash is **not** a dangling device pointer.

5. **Upstream investigation must focus on the wrapper callers** (the
   single 32-bit argument in `r4` of `sub_8227F5B8`, `r5` of
   `sub_8227F608`).  Those callers pass a pointer to an object they
   believe still owns a colour word — that object is what has already
   been freed by the time the first draw frame runs.  `sub_8227F2E8`
   is blameless: it just trusts the pointer it was given.

6. **Side note: the `f22` register is read in AddVertex #3 and #4 as
   `u1`, but is not materialised inside `sub_8227F2E8`.**  In the
   `sub_8227F608` (9-float) path this is fine (the wrapper leaves f9
   in place and relies on ABI quirks).  In the `sub_8227F5B8`
   (4-corner + single w-constant) path, `f22` holds whatever the
   caller's caller left there — a separate latent issue independent
   of the UAF.
