# Callers of sub_8227F5B8 — origin of the r4 "handle"

Reverse the premise: trace where r4 comes from at every direct call site of
`sub_8227F5B8`. The callee spills `r4` to its own frame at `sp+92`; then
`sub_8227F2E8` reloads it via `lwz r30,300(r1)` (that frame's `-208`) and
dereferences it (`lwz r9,0(r30)`), meaning `r4` is a **pointer** and `*r4`
is the value actually consumed.

## Agreed semantics of sub_8227F5B8

Signature as inferred from both the wrapper (doc 02) and the adapter
`sub_8227F2E8`:

- `r3` = pointer to 4x float = rect/quad in screen space
  `(x0, y0, x1, y1)` loaded via `lfs f1..f4,0..12(r3)`.
- `r4` = pointer to a `u32` color/argb (or wider state word whose low u32
  is read).
- Constants `f5..f9` are set from `0x82000A34` / `0x82000D48` — identical
  for every call. They are default UV/params, not handles.

So **r4 is not a freed heap handle**. It is a **stack-local pointer** to
a small color/state word the caller just wrote. The UAF theory from
upstream does not hold at this site — the value at `*r4` is fresh.

## Call sites (8 total, 5 unique callers)

find_callers + grep on the generated recomp files gives the definitive set:

|-|-|-|
| Site | Caller | r4 setup (immediate PPC) |
| 0x8214EB2C | sub_8214DBD0 | `li r31,-1; stw r31,148(r1); addi r4,r1,148` |
| 0x8214EC44 | sub_8214DBD0 | `stw r31,160(r1)` (r31=-1 still); `addi r4,r1,160` |
| 0x82150D54 | sub_821506E8 | `addi r4,r1,96` (slot filled earlier in fn) |
| 0x821B652C | sub_821B5C90 | `li r11,-1; stw r11,152(r1); addi r4,r1,152` |
| 0x821B6EAC | sub_821B5C90 | `addi r4,r1,96` — slot built by color-blend (`oris…65535; rlwimi alpha,24; stw r11,96(r1)`) |
| 0x821F2150 | sub_821F1EE0 | `addi r4,r1,80` — slot written with color `0x00FFFFFF` or `[r31+20]` (object's color field) |
| 0x8229ED04 | sub_8229D8A8 | `li r31,-1; stw r31,212(r1); addi r4,r1,212` |
| 0x8229ED14 | sub_8229D8A8 | `stw r31,188(r1); addi r4,r1,188` (r31=-1 reused) |

Every single one is class **(d) stack slot**. No globals, no vtable
fields, no table lookup, no caller-r4 passthrough.

## What is actually written to *r4 before the call?

Two patterns, both harmless color values:

1. `-1` = `0xFFFFFFFF` = opaque white (most common — 5 of 8 sites).
2. An on-the-fly blended ARGB: `r11 |= 0x00FFFF; rlwimi alpha`, or
   loaded from an object field (`lwz r11,20(r31)` in sub_821F1EE0 —
   `r31` is a non-null "text style" record, see below).

sub_821F1EE0 is the most interesting data flow: `r31` is reached via
`rlwinm r11,r28,2,0,29; lwzx r3,r11,r10` — a 4-byte-stride table lookup
at `0x828A5CC0+ (style_id << 2)`, which returns an in-datasection record
pointer. Its field `+20` is a color. That record is stored in `.rdata`
(or long-lived `.data`) so it cannot be freed.

## Recursive hop — parents of the interesting callers

Ran `find_callers` on the hottest two to confirm no outer call ever
pipes a heap handle through.

- sub_821F1EE0: called via `sub_821F14C0` / `sub_821F1588` / `sub_821F1670`
  (siblings listed in callees summary) — all are font/text draw
  dispatchers that similarly build a local `DrawStyle` struct on stack
  and invoke this sub.
- sub_8229D8A8: 2 callers only (per get_function_info `Calls: 2x`),
  upper frames in the radar/mini-map drawing (the `lis r7,-256`
  `lfs f4,3192(r11)` / `lfs f3,3188(r11)` constants pinned at
  `0x82000C74/C78` match the radar blip constants).

None of the upstream frames pass a heap pointer down. Every call
to `sub_8227F5B8` ends at a stack address that was written a few
instructions earlier — **no dangling handle semantics**.

## Implication for the UAF hunt

If `sub_8227F5B8` is implicated in the reported UAF, the corruption
must happen **outside this call graph**. Candidates worth chasing
elsewhere (NOT via this sub's r4):

1. The **r3 rect pointer**. Some callers pass `r3 = r1+offset_to_big_buffer`
   which is also a stack address, but a couple of sites (the
   `sub_8224EBD8` return) pass `r3 = &struct[0]` where the returned
   struct is from a heap pool. That pool's lifetime is the right
   suspect — `sub_8224EBD8` is a "get scaled metric" lookup and its
   return value is a pointer into a long-lived metric table, so still
   probably fine — but worth a distinct sub-investigation.
2. The **fpscr flush-mode sequence**. `disableFlushMode()` runs on
   every load; if a concurrent thread (achievement/HID toast) flips
   FPSCR mid-loop the `lfs` values become garbage, which would look
   like a "freed-quad" on the render side without any actual heap UAF.

## File pointers (absolute)

- Callee: `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.10.cpp` @ `sub_8227F5B8`
- Adapter: same file @ `sub_8227F2E8`
- Caller sub_8214DBD0: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.0.cpp` lines 35373 (first bl), 35547 (second bl)
- Caller sub_821506E8: `gta4_recomp.0.cpp` line ~40662
- Caller sub_821B5C90: `gta4_recomp.4.cpp` lines 8192/8227 (first), 9647 (second)
- Caller sub_821F1EE0: `gta4_recomp.6.cpp` line 9241
- Caller sub_8229D8A8: `gta4_recomp.12.cpp` lines 5428 / 5435
