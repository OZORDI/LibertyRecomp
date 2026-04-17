# sub_8227F608 — Colour/Handle Stack-Relay Wrapper (5-arg variant)

Agent 3 / 15 — UAF investigation, handle-poison `0xFFE1E1E1`.

## Snapshot
- **Address**: `0x8227F608`
- **Size**: ~0x50 bytes (80 B) — tiny adapter.
- **Callees**: `sub_8227F2E8` only.
- **Callers**: 4 — `sub_8218D7A8`, `sub_8218E2C0`, `sub_8218E318`, `sub_821F1670`.
- **Hooked?**: no.
- **Class**: unassigned.
- Source recomp: `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.10.cpp`.

## Argument list (how regs are used)

| reg | role | lifetime |
|-|-|-|
| r3 | pointer to 4-float struct A (quad vertex-A / colour-A: `{x,y,z,w}` at `+0,+4,+8,+12`) | forwarded verbatim |
| r4 | pointer to 4-float struct B (quad vertex-B / colour-B, same layout) | forwarded verbatim |
| r5 | handle / object pointer (u32) | **spilled to stack at `r1+92`** |
| f1..f4 | derived from `*r3` | passed to callee |
| f6..f9 | derived from `*r4` | passed to callee |
| f5 | constant float from `[0x81FF0A34]` (TOC literal, likely `0.0f` or `1.0f`) | passed to callee |

No return value used by any caller (blr with r3 untouched from callee).

## Stack frame

```
stwu r1,-112(r1)       ; 112-byte frame
stw  r5, 92(r1)        ; spill r5 (handle) to fixed slot
bl   sub_8227F2E8      ; tail-call-like
addi r1,r1,112 ; blr
```

Spill offset `+92` equals the same slot `sub_8227F5B8` uses for r4: both wrappers publish the handle at the identical frame address so `sub_8227F2E8` can pick it up via r1-relative addressing.

## Relay into sub_8227F2E8

Direct call — **not through a trampoline**. After `sub_8227F2E8`'s own `stwu r1,-208(r1)`, it reads `lwz r30, 300(r1)`. Python: `300 == 208 + 92`, so `r30 := *(wrapper_sp + 92) = handle`. Then `lwz r9, 0(r30)` dereferences the handle as a **vtable/object pointer** (offset 0 = vptr). The `r9` value is an unused-by-callee temp — `sub_828C2290` is called with args in f1..f8, r9 is not consumed. The dereference happens purely as a liveness-hint / prefetch; the **handle itself is not re-stored** into any exterior buffer by this wrapper chain.

So the poisoned `0xFFE1E1E1` is not *written* into the colour field here — this wrapper is one frame up from the write. The field-poisoning happens in a caller that passes a freed slot pointer.

## Caller LR sites & r5 payload

| caller | LR after bl | r5 origin | comment |
|-|-|-|-|
| sub_8218D7A8 | 0x8218D8A0 | `r5 = r1+84` where `r1+84` holds `-1` (sentinel) | pre-stores `li r11,-1; stw r11,84(r1)` — passes pointer to sentinel |
| sub_8218E2C0 | 0x8218E304 | `r5 = caller.r6` (spilled via `mr r29,r6`) | pass-through of outer arg |
| sub_8218E318 | — | not scanned in this pass | 768 B fn, needs separate analysis |
| sub_821F1670 | 0x821F17B8 | `r5 = r1+180` where `r1+180` stores `caller.r5` | pointer-to-handle; outer arg r5 is u32 handle stored on stack |

Caller `sub_8218D7A8` is the most interesting for UAF: it writes a **-1 sentinel** (0xFFFFFFFF) into slot 84 and passes `r5 = &sentinel`. sub_821F1670 by contrast relays the live caller handle — this is a candidate site where a freed allocator handle (0xFFE1E1E1) could slip in as `caller.r5`.

## Sibling comparison — sub_8227F5B8 vs sub_8227F608

Both are adapters onto `sub_8227F2E8` with identical 112-byte frames and identical +92 handle spill slot.

| feature | sub_8227F5B8 | sub_8227F608 |
|-|-|-|
| struct args (float vec4) | **1** — r3 only | **2** — r3 and r4 |
| handle reg | **r4** | **r5** |
| f5 source | const `[0x81FF0A34]` | const `[0x81FF0A34]` |
| f6..f9 source | f6=f7=const0A34, f8=f9=const0D48 — all constants | loaded from `*r4` — real data |
| callers | 5 | 4 |

**Why two variants**: the 2-struct form draws a quad with independent A→B colour/vertex endpoints; the 1-struct form draws with a single colour and uses two hard-coded constant defaults for the second endpoint (likely `{0,0,0,0}` and `{1,1,1,1}` or similar). Both hand the same low-level primitive batcher (sub_8227F2E8) a pair of vec4 inputs plus a handle, just sourced differently.

Register-ordering difference (r4 vs r5 for the handle) mirrors the shape of the C++ calling convention: when an extra vec4* prepends the arg list, every subsequent scalar shifts one register right. This is consistent with C++ overload resolution — these are almost certainly two overloads of the same method emitting slightly different primitives (e.g. `DrawLine(pA, handle)` vs `DrawLine(pA, pB, handle)`).

## UAF relevance for this agent's target

`sub_8227F608` itself does not corrupt the handle — it is a transparent relay. The poison travels **through** r5 → `[r1+92]` → `r30` in callee. For UAF source tracing, follow:

1. **sub_821F1670** — passes outer caller's r5 verbatim as the handle; if the caller of `sub_821F1670` holds a freed slot id this is the leakage path.
2. **sub_8218E2C0** — passes outer r6. Upstream call-graph probe of `sub_8218E2C0` callers is needed (currently 0 recorded, likely because it's only called indirectly / from non-analysed sites, or through a vtable).
3. **sub_8218D7A8** passes a literal `-1` sentinel — not a UAF candidate.
4. **sub_8218E318** not yet decoded — queue for a sibling agent.

The write of `0xFFE1E1E1` into a vertex-colour field therefore occurs *before* entry to `sub_8227F608`, at the moment a caller populates the quad struct at r3/r4 from a freed allocator slot. Suspect the allocator debug-fill pattern on `rex::Heap::Free`.

## Files touched

- Recomp source: `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.10.cpp` (sub_8227F608, sub_8227F5B8, sub_8227F2E8)
- Recomp source: `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.2.cpp` (sub_8218D7A8, sub_8218E2C0)
- Recomp source: `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.6.cpp` (sub_821F1670)
