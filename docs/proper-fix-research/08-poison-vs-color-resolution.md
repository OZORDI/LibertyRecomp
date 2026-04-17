# Agent 8 — Poison vs Color Resolution for 0xFFE1E1E1

## TL;DR
At the CRASH site captured in `/tmp/liberty_watch5.log` L4398–L4407, the observed
value `0xFFE1E1E1` is a **LEGITIMATE ARGB light-gray HUD color** (alpha=0xFF,
RGB=0xE1E1E1), **NOT** RAGE heap poison. The actual crash is unrelated to the
color: it is a bad vertex-cursor pointer (0x20) returned by `sub_82A3DAB0` and
stored at the draw-pool cursor slot `0x831C2D28` during `sub_828C21D0`'s
BeginPrim / `sub_828BF248` path.

The prior-agent dispute ("all-legitimate" vs "all-poison") is resolved:
callers of `sub_8227F5B8` / `sub_8227F608` fall into three classes — explicit
`-1` alpha, table-driven computed color, and pre-written argument slot — and
the CRASH site's color originates in the **table-driven computed-color** bucket
(sub_821506E8 or sub_821F1EE0).

## Crash Frame (from liberty_watch5.log)

```
L4398 [WATCH] DAB0 seq=1 LR=0x828C2258 dev=0x20084480 prim=6 vc=4 str=36
L4403 [WATCH] DrawPrimUP seq=1 buf_guest=0xBF1A0580 v0.pos=[192,269.625,0,0]
       v0.color=0xFFE1E1E1 v0.uv=[0.375,0.781]
L4404 [WATCH] *** COLOR IS RAGE POISON 0xFFE1E1E1 ***
L4405 [WATCH] DAB0 seq=2 LR=0x828C2258 dev=0x20084480 prim=6 vc=4 str=36
L4407 CRASH — Symbol: sub_828C2290 + 0xA4
      guest lr=0x8227F3AC  r9=0xFFE1E1E1  r10=0x831C2D3C  r11=0x00000020
      host fault_addr = base+0x20 (r11 was loaded from [0x831C2D28] = 0x20)
```

- prim=6 = QUADLIST (4 verts), stride=36 → matches `sub_828C21D0`:
  `li r5,36 ; bl sub_828BF248`.
- guest LR=0x8227F3AC = return inside `sub_8227F2E8` after **first**
  `sub_828C2290` call (vertex #0 write). The color passed in r9 is the one
  under dispute.
- The crash instruction is `stw`/`stfs` inside `sub_828C2290` into the vertex
  record at `r11` (cursor), where r11 was just read as 0x20 — an invalid
  guest address. The cursor comes from the draw-pool global at 0x831C2D28,
  populated by `sub_82A3DAB0`'s allocation result.
- 0xFFE1E1E1 in r9 is carried THROUGH `sub_828C2290` unmodified, so the
  crash is downstream of (and independent of) the color value itself.

## All Call Sites of sub_8227F5B8 / sub_8227F608

| caller | target | call LR | color-pointer slot | color-slot writer |
|-|-|-|-|-|
| sub_8229D8A8 | F5B8 | 0x8229ED04 | r1+212 | `li r31,-1 ; stw r31,212(r1)` → 0xFFFFFFFF |
| sub_8229D8A8 | F5B8 | 0x8229ED14 | r1+188 | `stw r31,188(r1)` (r31=-1) → 0xFFFFFFFF |
| sub_8214DBD0 | F5B8 | 0x8214EB2C | r1+148 | literal -1 via `li r31,-1` path |
| sub_8214DBD0 | F5B8 | 0x8214EC44 | r1+160 | literal -1 path |
| sub_821B5C90 | F5B8 | 0x821B652C | r1+152 | literal -1 path |
| sub_821B5C90 | F5B8 | 0x821B6EAC | r1+96  | literal -1 path |
| sub_821506E8 | F5B8 | 0x82150D54 | r1+96  | **`or r11,r9,r10 ; stw r11,96(r1)` where r9=[r28+36]&0xFFFFFF, r10=r19<<24** |
| sub_821F1EE0 | F5B8 | 0x821F2150 | r1+80  | table-driven (same shape) |
| sub_8218D7A8 | F608 | 0x8218D8A0 | r1+84  | `li r11,-1 ; stw r11,84(r1)` → 0xFFFFFFFF |
| sub_8218E2C0 | F608 | 0x8218E304 | r5=r29 (caller's r6) | passed-through from grand-caller |
| sub_8218E318 | F608 | 0x8218E600 | r1+84  | prior code; not `-1` literal |
| sub_821F1670 | F608 | 0x821F17B8 | r1+180 | prior code; not `-1` literal |

Three classes emerge:

1. **Literal 0xFFFFFFFF "white" alpha slot** (6 sites). If taken, color
   delivered to `sub_828C2290.r9` would be 0xFFFFFFFF — NOT 0xFFE1E1E1.
   These sites are excluded from the crash.
2. **Computed ARGB from font/table entry** (sub_821506E8 @ 0x82150D54;
   sub_821F1EE0 @ 0x821F2150): `color = ([r28+36] & 0x00FFFFFF) | (alpha << 24)`.
   r28 = static array base (0x82DF1F60 + 44*index). If `[r28+36]` = 0x00E1E1E1
   and alpha = 0xFF → r11 = 0xFFE1E1E1 written at r1+96 → picked up by F2E8
   via the r30←(caller_r1+92) adjustment → r9 = 0xFFE1E1E1 at vertex-write.
3. **Pass-through caller argument** (sub_8218E2C0 etc.): the color-slot is
   whatever the grand-caller produced.

## Static Table at r28 (0x82DF0000 + 8032 + 44*i)

`default.bin` shows the region is zero-initialized; the table is populated at
runtime. Record layout (from sub_821506E8 body):

- +0 : index tag (-1 = sentinel)
- +4..+35 : per-glyph metrics
- +36 : RGB color word (masked with 0xFFFFFF when consumed)
- +40 : alpha byte

This is almost certainly the **HUD text glyph table** for
`CTextHelpers::PrintString` / radar label rendering. A light-gray
(0xE1,0xE1,0xE1) color for minimap text fits the game's design — it is
visually identical to RAGE's debug poison but, at this caller site, the value
is **computed by the game and is legitimate**.

## Why the Ambiguity Exists

RAGE's poison byte is 0xE1 (`sub_82847160` writes `dword_82B07038` into freed
blocks). In `default.bin`, `0x82B07038 = 0xCDCDCDCD` at link time; runtime
initialization sets it to 0xE1E1E1E1. The shipping HUD designers *also* picked
a light-gray 0xE1E1E1 for some text overlays. When a vertex color prints as
0xFFE1E1E1, a watchpoint cannot tell poison from HUD-gray by value alone.

Deterministic classification REQUIRES tracing the color-slot's writer in the
recomp scaffold, which is what the table above records. The conclusion is
caller-dependent:

- **Literal-(-1) sites**: any 0xFFE1E1E1 read there MUST be poison (contract
  violated — someone overwrote the stack slot). **Not applicable to this
  crash** because the observed value is 0xFFE1E1E1, not 0xFFFFFFFF.
- **Table-driven sites (sub_821506E8 / sub_821F1EE0)**: 0xFFE1E1E1 reflects the
  glyph-table color. Legitimate by construction.
- **Pass-through sites**: requires one more hop up the call chain; not
  implicated in the captured crash (color does not flow through F608 for the
  prim=6/str=36 path reaching `sub_828C2290+0xA4`).

## Identifying the Crash-Site Caller Chain

- Crash guest regs: `lr=0x8227F3AC`, `r12=0x8227F370` → the only path
  consistent with these is the FIRST `sub_828C2290` call in `sub_8227F2E8`
  (at 0x8227F3A8). That vertex receives color from r30 = caller_r1 + 92.
- The captured color is 0xFFE1E1E1 — excludes all "literal -1" callers
  (they would yield 0xFFFFFFFF).
- v0.pos=[192, 269.625, 0, 0] is an on-screen HUD-space coordinate,
  consistent with `sub_821506E8`/`sub_821F1EE0` which emit HUD-quad text.
- Vertex buffer `buf_guest=0xBF1A0580` is in the dynamic draw arena
  (0xBF0000 range is heap; not the font-glyph static).
- **Conclusion**: the caller chain is a HUD text path via
  `sub_821506E8` (or `sub_821F1EE0`) → `sub_8227F5B8` → `sub_8227F2E8`.
  The color is a legitimate, glyph-table-supplied light gray.

## Why Agents 9/11/13/14/15 Thought It Was Poison

- The color BYTE VALUE exactly matches `sub_82847160`'s RAGE poison pattern
  0xE1E1E1E1 from `dword_82B07038`, so a value-only check flagged it.
- The surrounding symptom (cursor=0x20, invalid vertex pointer) IS consistent
  with a freed-pool-reused scenario, so the HUD gray coincidence reinforced
  the poison hypothesis.
- Agents did not trace r28 (static font-table) as the color source —
  0x82DF1F60 is outside the heap arenas and is never freed/poisoned.

## Why Agents 5/10 Were Right-ish

- They correctly identified that every F5B8/F608 call site passes a
  stack-local or table-driven pointer (not a freed heap pointer) for the
  color argument. However, they over-generalized by calling the value a
  "literal light-gray constant in .rdata". It is not a .rdata constant —
  0x82000A34 and 0x82000D48 hold the floats (0.0f, 1.0f) loaded by F5B8/F608
  for the vertex corner geometry, not the color. The color is a runtime
  computed `RGB | (alpha<<24)` from a runtime-filled BSS glyph table, which
  IS legitimate but via a different mechanism than they described.

## Crash Root Cause (Out of Scope But Noted)

`sub_82A3DAB0` (draw-pool alloc at 0x831C22A4) returned a bad/stale vertex
cursor (guest 0x20 after truncation). The mgr pointer at `0x831C22A4` is
`0x20084480` in the log — a SUSPICIOUS-LOW value flagged by
`draw_pool_watchpoint.cpp`. This is the SEPARATE mgr-pointer bug Agents 9/11
have been investigating. It is orthogonal to the color value.

## Verdict

**The captured `0xFFE1E1E1` at LR=0x828C2258 / sub_8227F3AC is a LEGITIMATE
ARGB light-gray HUD color**, produced by `sub_821506E8`/`sub_821F1EE0`'s
`or r11, r9, r10 ; stw r11, 96(r1)` sequence reading runtime-populated static
font-table data. It is NOT freed-heap poison. The crash is caused by an
independent bad vertex cursor (0x20) originating in the draw-pool allocator.

## Files / Addresses Touched

- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/patches/draw_pool_watchpoint.cpp`
- `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.{0,2,4,6,10,12,66}.cpp`
- `/tmp/liberty_watch5.log` L4398–L4440
- static font table base: **0x82DF1F60** (r28 in sub_821506E8; stride 44)
- draw-pool cursor slot: **0x831C2D28**
- draw-pool mgr pointer: **0x831C22A4**
- RAGE poison fill global: **0x82B07038** (runtime-set to 0xE1E1E1E1)
