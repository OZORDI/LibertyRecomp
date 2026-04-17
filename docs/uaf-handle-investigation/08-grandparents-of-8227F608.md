# Agent 8 — Grandparents of sub_8227F608

Mirror of agent 7 for the `sub_8227F608` lineage. Walked up 2 levels.

## sub_8227F608 itself

| field | value |
|-|-|
| address | 0x8227F608 |
| size | ~0x50 (80B) |
| callees | sub_8227F2E8 (single) |
| pseudocode | forwards 4 float args from r3/r4 plus an i32 from r5 into sub_8227F2E8 |

Recomp confirms: loads floats at [r3+0..12] and [r4+0..12] (two vec3/vec4), a literal from `[.rodata + 2612]`, spills r5 to stack+92, then tail-calls sub_8227F2E8. Classic thin vararg/setup wrapper around the shared worker.

Note: `sub_8227F2E8` is the SAME shared worker called by `sub_8227F5B8` (agent 7 target). The two wrappers differ only in argument preprocessing.

`sub_8227F2E8` calls: `sub_8227EE90`, `sub_828C19C0/21D0/2290/2300/60A0` — the 0x828Cxxxx cluster is the text/glyph drawing primitives (rage font backend).

## Parents (level 1) — 4 callers

| caller | size | strings | notes |
|-|-|-|-|
| sub_8218D7A8 | 0x148 | none | also calls sub_8227ECF8/ED38/EDA0/F040/F1E8/F658 — sibling of F608 in same text-draw family |
| sub_8218E2C0 | 0x58 | none | no callers found (entry point / indirect dispatch / vtable). calls same F608 family |
| sub_8218E318 | 0x300 | "None" | branches on r4+216 == 21 (element-type switch); calls sub_82146690/790, sub_8216EAD0/EB50, sub_8218EAF0 |
| sub_821F1670 | 0x168 | none | calls sub_828BF810 (likely memset/clear) + sub_8227F608 |

All four parents are in one of two address bands:
- 0x8218xxxx (3 of 4) — tight cluster with sub_8227ECF8/ED38/EDA0 shared
- 0x821F1xxx (1 of 4) — uses string-table lookup at (-32070, -24496)

## Grandparents (level 2)

| grandparent | path | size | notes |
|-|-|-|-|
| sub_8218D8F0 | ← sub_8218D7A8 | 0x298 | calls sub_8216E6C8 + sub_8218D7A8 + sub_8218E9F0 |
| sub_82181D00 | ← sub_8218E318 | 0x1C0 | layout/paragraph walker (see below) |
| sub_8218D640 | ← sub_8218E318 | 0x168 | self-recursive; calls sub_8218D8F0/DD80/E318/E618 (tree walk) |
| sub_821F1EE0 | ← sub_821F1670 | 0x438 | **also grandparent of sub_8227F5B8** — convergence! |
| (sub_8218E2C0 has no callers) | | | |

## Subsystem identification

### 0x82181xxx–0x8218Exxxx cluster = HTML text rendering

Decisive evidence: **`sub_82181BD8` is called by `sub_8216E488`, which is `CHtmlTextFormat::vfunc[0]`** (vtable slot 1 @ 0x820bccd4, class `___7CHtmlTextFormat__6B_ @ 0x820b9a24`).

`sub_82181BD8` is called by `sub_82181C70`, which is called by `sub_82172840` and `sub_8218E618`. `sub_8218E618` is called by `sub_8218D640` — tying the whole 0x8218xxxx tree to CHtmlTextFormat.

`sub_82181D00` (one of our grandparents) walks a `[r3+568]`-count array at stride 4 of pointers, reading offsets 44/48 (rect left/top), 136, 264, 392, and calling `sub_8218E008` or `sub_8218E318` — this is a paragraph / inline-run layout loop typical of HTML formatters. Branch on `r4+216 == 21` is an element-type dispatch (class/tag id).

### 0x821F1xxx range = font/text measurement system

`sub_821F1EE0` loads globals at `(-32087, 13728)` and `(-32087, 17528)` (font-system TLS/config struct at 0x821FC478ish), reads `[struct+20/32/37/44]` and `[struct+8]` (line-height divisor), calls `sub_82155300` (likely `FtoI`), `sub_82146700/790` (rage framework helpers), `sub_821F14C0/1588/1670`, and uses a string table at `(-32070, -24496) = 0x821FA2D0ish`. It looks like a `DrawTextWithFormat(f1=x, f2=y, r5=flags)` routine that measures and places glyphs — and on the hit path calls `sub_8227EDA0` (sibling of our F608 chain).

`sub_821F1EE0` is called by `sub_821F6550` (38 callers, HOT — called by 38 distinct draw sites), which is called by `sub_821F5788` — the HUD/message-draw dispatch level.

## Convergence with agent 7 (sub_8227F5B8)

`sub_8227F5B8` has 5 callers: `sub_8214DBD0, sub_821506E8, sub_821B5C90, sub_821F1EE0, sub_8229D8A8`.

**`sub_821F1EE0` is a direct parent of sub_8227F5B8 AND a grandparent of sub_8227F608.**

Inside sub_821F1EE0 the recomp shows (line ~0x821F1FF8) `sub_8227EDA0` then `sub_82155300` — that's the same glyph-width measurement chain that feeds both wrappers. Further down (after loc_821F2180 — truncated at 300 lines) the function branches to paths that call `sub_8227F5B8` directly and, via `sub_821F1670`, `sub_8227F608`.

### Conclusion

Not two independent subsystems hitting the same crash — **one subsystem (font/text rendering pipeline) with two shape wrappers around the same shared worker `sub_8227F2E8`**:

- `sub_8227F5B8` = thin wrapper A (single vec3/vec4 arg pattern)
- `sub_8227F608` = thin wrapper B (dual vec3/vec4 arg pattern, slightly more setup + r5 int)

Both wrappers tail-call the same `sub_8227F2E8` which in turn uses the 0x828Cxxxx font/glyph draw primitives.

Both wrappers are consumed by the same 0x821F1xxx / 0x8218xxxx text layout family, whose top-level class anchor is **CHtmlTextFormat** (vtable at 0x82103080). The 0x8218xxxx cluster is the HTML formatter's paragraph/inline walker; the 0x821F1xxx cluster is the lower-level glyph placement / measurement layer. They are two layers of the same stack, not two subsystems.

The UAF likely lives in the object passed as `r3`/`r4` (the vec3/vec4 source structs) or the resolved node pointer inside `sub_8227F2E8`'s callees (sub_828C19C0/21D0/2290/2300/60A0 — rage font backend). Given both wrappers share `sub_8227F2E8` → `sub_8227EE90` (which calls `sub_828CDAE0`), the freed handle is most plausibly a rage font / glyph-cache entry rather than an HTML layout node.

## Addresses for follow-up

| addr | role |
|-|-|
| 0x8227F2E8 | shared worker (both F5B8 + F608 tail-call this) |
| 0x8227EE90 | worker helper — calls sub_828CDAE0 (font backend) |
| 0x8227EDA0 | measurement helper — called by sub_821F1EE0 + sub_8218D7A8 |
| 0x828CDAE0 | likely rage font glyph cache |
| 0x82103080 | CHtmlTextFormat RTTI |
| 0x820B9A24 | CHtmlTextFormat vtable |
| 0x820BCCD4 | CHtmlTextFormat vtable slot 1 (= sub_8216E488) |
| 0x821F1EE0 | **convergence point** — parent of F5B8, grandparent of F608 |
| 0x821F6550 | 38-caller HOT HUD draw entry (calls sub_821F5788) |
