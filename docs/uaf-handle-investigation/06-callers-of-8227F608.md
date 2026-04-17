# 06 — Callers of sub_8227F608 (UAF handle trace)

Agent 6 of 15. Target: **immediate callers of sub_8227F608**, classify r5 sources.

## sub_8227F608 signature recap
- `0x8227F608`, ~80 bytes
- **r3**: plane/matrix (7 floats at 0/4/8/12) — read-only
- **r4**: vec3 (3 floats at 0/4/8/12) — read-only
- **r5**: **handle/key ptr** — spilled to `stack+92` before tail-calling `sub_8227F2E8`
- Only callee: `sub_8227F2E8` (draws via `sub_828C2290` × 4 batching — this is a line/quad rendering primitive)

The 4 floats + 4 floats + handle pattern is classic "draw-textured-plane-with-handle".

## Caller inventory (4 direct callers)

| caller | r5 source | class |
|-|-|-|
| sub_8218D7A8 @ 0x8218D7A8 | `li r11,-1; stw r11,84(r1); addi r5,r1,84` | **stack-local sentinel (-1)** |
| sub_8218E2C0 @ 0x8218E2C0 | `mr r29,r6` (callee param) → `mr r5,r29` | **passthrough (r6)** |
| sub_8218E318 @ 0x8218E318 | `li r10,-1; stw r10,84(r1); addi r5,r1,84` | **stack-local sentinel (-1)** |
| sub_821F1670 @ 0x821F1670 | `stw r5,180(r1); addi r5,r1,180` | **passthrough (r5) boxed onto stack** |

All four pass `r5 = &stack_slot`; sub_8227F608 then dereferences/spills again at `stack+92`. The handle itself is a u32 stored at that stack slot before the call.

## Per-caller detail

### sub_8218D7A8 (r5 = -1 sentinel on stack)
```c
// 0x8218D894
li r11,-1
addi r5,r1,84
stw r11,84(r1)       // stack[84] = 0xFFFFFFFF
// r3=r30 (caller-provided), r4=r1+96 (vec3 built from fdivs)
bl 0x8227f608
```
Hot path: caller passes `r30` object pointer in r3, builds a 3-float vec in stack +96..+108, and a sentinel 0xFFFFFFFF in +84 to indicate "no cached handle yet". **sub_8227F608 likely writes the new handle back.**

### sub_8218E2C0 (pure passthrough)
Takes `(r3, r4, r5, r6)` from caller — swizzles: `r3_new = r31 (=orig r4)`, `r4_new = r30 (=orig r5)`, `r5_new = r29 (=orig r6)`. **The freed handle originates at depth N+1 from this function.** No callers found (`find_callers` empty → likely indirect / vtable).

### sub_8218E318 (stack sentinel, long path)
- Gets r4=param, r25=param; loads color globals at 0x82A90000 - 19360/64/68/72 via `sub_828BF810` gates.
- At 0x8218E5D4 (late-exit path): `li r10,-1; stw r10,84(r1)`; later `addi r5,r1,84; bl 0x8227f608`.
- **Called by sub_82181D00** with `r3=PPC_LOAD_U32(r31+32)` (i.e. `self->field32`) — `r31` is the caller's `self`.
- sub_82181D00 callers: sub_82172840, sub_82181BD8, sub_8218E618.

### sub_821F1670 (passthrough handle boxed)
```c
// entry:
stw r5,180(r1)        // box caller's r5 (handle value) into stack
...
addi r5,r1,180        // pass &stack[180] to sub_8227F608
bl 0x8227f608
```
**r5 comes in as a u32 handle VALUE**, gets written to stack, address passed downward.

Caller **sub_821F1EE0** supplies it:
```c
lwz r5,20(r31)        // r31 = &global @ 0x82A94478, r5 = global->field20
...
bl 0x821f1670
```
Where `r31 = 0x82A94478` (absolute global, `___R0_AVCFontGetNumberLines*` symbol is adjacent — this is a **global UI/font manager struct**). Field +20 (addr `0x82A944CC`) = the handle being passed.

**This is the strongest UAF candidate**: a handle stored in a global struct field gets read, passed down, and potentially used after free.

## Upward trace (one hop more)

- sub_8218D7A8 ← sub_8218D8F0 (passes self object, not a handle)
- sub_8218E318 ← sub_82181D00, sub_8218D640
- sub_821F1670 ← **sub_821F1EE0** (r5 from `global[0x82A94478 + 20]`)
- sub_82181D00 ← sub_82172840, sub_82181BD8, sub_8218E618
- sub_8218D640 ← sub_8218D560 (+ self-recursion)

## Classification summary

- **Global-field** (sub_821F1EE0 path): `global@0x82A944CC` → sub_821F1670 r5 → sub_8227F608 — **PRIME SUSPECT** for stale/freed handle (globals survive lifetime boundaries).
- **Stack-sentinel -1**: sub_8218D7A8, sub_8218E318 — these init a "no-handle-yet" slot; handle may be *written* by sub_8227F608, not freed.
- **Passthrough**: sub_8218E2C0 (r6 → r5), sub_821F1670 (r5 → box → r5&). Root of r6/r5 depth chain not resolved in this agent's scope.

## Notes
- `sub_8227F608 → sub_8227F2E8` reads `lwz r30,300(r1)` (the r5 spilled to caller's stack frame at offset from r5-ptr). sub_8227F2E8 does 4× `sub_828C2290` — a 4-quad textured-primitive draw.
- The handle at `r5` is likely a **texture/material/RT handle** (font glyph cache, UI sprite), consistent with the AVCFont global neighbor.
- Next investigation target: **trace writers to `global@0x82A944CC`** — find where this handle is stored and where it's freed. That's the UAF source.

## Key addresses
- sub_8227F608 = 0x8227F608
- sub_8227F2E8 = 0x8227F2E8 (drawing impl)
- global struct base = 0x82A94478 (neighbor: `___R0_AVCFontGetNumberLines*`)
- suspect handle slot = 0x82A944CC (global +20)
- aux global base  = 0x82A935A0 (ptr loaded early in sub_821F1EE0 @ lwz r11,13728(r11))
