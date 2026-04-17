# sub_8227F5B8 — thin wrapper onto sub_8227F2E8

Address: `0x8227F5B8`, size `0x50` (80 bytes, 21 insns). Leaf w.r.t. state: only touches
its own stack frame + register file; no globals written, no heap ops, no TLS.

## Disassembly summary (no pseudocode, straight from recomp)

```
mflr r12
stw  r12,-8(r1)
stwu r1,-112(r1)                  ; allocate 112-byte frame
lis  r11,-32256                    ; r11 = 0x82000000
lfs  f4,12(r3)
lfs  f3,8(r3)
stw  r4,92(r1)                     ; <-- SPILL caller r4 (handle) to +92
lfs  f2,4(r3)
lfs  f1,0(r3)
lfs  f9,3400(r11)                  ; f9 = [0x82000D48]   constant
lis  r11,-32256
fmr  f8,f9
lfs  f7,2612(r11)                  ; f7 = [0x82000A34]   constant
fmr  f6,f7
fmr  f5,f7
bl   0x8227F2E8                    ; tail-adaptor call
addi r1,r1,112
lwz  r12,-8(r1)
mtlr r12
blr
```

## Calling-convention / semantics

- **r3 in (preserved)**: pointer to a 4-float record = `{f1,f2,f3,f4}` at offsets
  `[0,4,8,12]`. Semantically an **XYZW / XY+XY point tuple** (two 2D pairs, or a
  4-component vector). Passed through unchanged to `sub_8227F2E8` via `f1..f4`.
- **r4 in**: a **32-bit handle / tagged integer**. Not dereferenced here; merely
  spilled to `r1+92` so the callee's epilogue doesn't clobber it. Callee reads it
  with `lwz r30,300(r1)` immediately after its own `stwu r1,-208(r1)`, which
  reaches back into the caller's slot (`-208+300 = +92`, relative to callee r1 =
  caller `r1+92`). Verified in Python.
- **f1..f4 out**: populated from `*(r3+0..12)`.
- **f5, f6 out**: duplicates of `f7` = constant at `0x82000A34`.
- **f7 out**: constant at `0x82000A34`.
- **f8, f9 out**: duplicates of `f9` = constant at `0x82000D48`.
- r5/r6/r7/r8 are NOT set by this wrapper — they pass through whatever the caller
  left in them. `sub_8227F2E8` re-uses them as `r4=0, r3=0` before any sub-call
  that actually consumes them, so the incoming r5..r8 from the wrapper's caller
  are effectively dead at entry to the inner primitive.

The two rodata floats at `0x82000D48` and `0x82000A34` are shared constants used
throughout the vertex-color/line primitive path (e.g. `sub_821B5C90` loads
`2612(r11) = 0x82000A34` into `f26` and `3400(r11) = 0x82000D48` into `f24`,
passing them as alpha/blend scalars). They are **not** in the symbol table. Both
behave as neutral color scalars: the wrapper is supplying default alpha/blend
values so `sub_8227F2E8` can do a **draw-point / draw-line emission with one color
handle and one geometry pair**.

## Stack frame (112 bytes)

| slot (caller r1 +) | size | content |
|-|-|-|
| -8 | 4 | saved LR (via `stw r12,-8(r1)`, still reachable via back-chain after stwu) |
| 0..47 | 48 | Xbox ABI linkage/param area (unused) |
| 48..91 | 44 | slack |
| **92** | **4** | **spilled r4 (the handle)** |
| 96..111 | 16 | slack |

No callee-saved GPRs are spilled — nothing to save, r11/r12 are volatile.
No other guest memory is touched by this function.

## Heap / globals

- **No allocation, no free, no refcount.** The function does not call anything
  except `sub_8227F2E8`. It does not write any global.
- It **does not validate** r4. Whatever value the caller hands in (including the
  freed-handle poison `0xFFE1E1E1`) is passed through untouched.

## Conclusion on the UAF path

`sub_8227F5B8` is a **pure forwarding shim**. The handle comes in via `r4` from
user code, is stashed at `r1+92` purely as an ABI mechanism (so `sub_8227F2E8`'s
own stack adjust doesn't overwrite it), and `sub_8227F2E8` then deref's it via
`lwz r30,300(r1)` followed by `lwz r9,0(r30)` — that load is where the
`0xFFE1E1E1` poison first dereferences. **The bug is not here**; this function
faithfully propagates whatever the caller provided.

## All 8 call sites (5 distinct callers)

| call site (LR = return addr) | caller | caller prepares r3= | caller prepares r4= |
|-|-|-|-|
| `0x8214EB2C` | `sub_8214DBD0` | `r1+320` (3 floats + int built on stack) | `r1+148` (local 4-slot struct) |
| `0x8214EC44` | `sub_8214DBD0` | `r1+384` (3 floats + int built on stack) | `r1+160` (local 4-slot struct) |
| `0x82150D54` | `sub_821506E8` | prior r3 (`r1+296` flowing in from merge label) | `r1+96` (local) |
| `0x821B652C` | `sub_821B5C90` | `r1+224` (3 floats + int built on stack) | `r1+152` (local) |
| `0x821B6EAC` | `sub_821B5C90` | `r1+208` (4 floats built from f27-scaled AABB) | `r1+96` (local) |
| `0x821F2150` | `sub_821F1EE0` | `r1+128` (3 floats + int) | `r1+80` (local) |
| `0x8229ED04` | `sub_8229D8A8` | `r1+256` (big XY packet) | `r1+212` (local, pre-written `r31=-1` i.e. `0xFFFFFFFF`) |
| `0x8229ED14` | `sub_8229D8A8` | `r1+224` | `r1+188` (local, pre-written `r31=-1`) |

Every caller passes `r4 = &some_local_stack_slot` that it has **just populated**
(either with `-1` = no-op sentinel, or with a recently-read resource/texture
handle). None of the eight sites pass `r4 = 0xFFE1E1E1` directly; the poison must
be arriving through the pointed-to handle storage. Two sites in `sub_8229D8A8`
explicitly stamp `-1` immediately before the call, so those two are not suspects
for the UAF. The primary interesting path for the poison is one of the six
other sites where `r4` points at a slot just filled from a lookup/read that
may return a freed handle.

## Next hop

Control flows into `sub_8227F2E8` (`0x8227F2E8`, ~0x170 bytes, calls
`sub_828C19C0`, `sub_828C21D0`, `sub_828C2290`, `sub_828C2300`, `sub_828C60A0`,
`sub_828C6568`, `sub_828C64C8`, `sub_828C6500`, `sub_828C6580`, `sub_8227EE90`).
Inside it, after a `sub_828C21D0(class=4, kind=4)` call (which looks like "begin
primitive, mode=4"), it does `lwz r30,300(r1)` to pull the handle, then
`lwz r9,0(r30)` — that's the dereference the UAF blows up on. The 4 floats from
r3 plus the color constants are emitted as four vertex-stream `sub_828C2290`
draws using that handle. Classic draw-quad / 4-vertex strip emission.
