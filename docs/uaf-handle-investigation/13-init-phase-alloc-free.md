# 13 — Init-phase allocation that could be freed before first-frame draw

UAF context: crash hits on the very first draw frame immediately after SetVS #1,
SetPS #1. The UAF target is a handle/struct that was allocated during boot
(loading-screen / frontend / HUD init) and whose backing memory has already been
reclaimed by the time `sub_8227F2E8` does `lwz r9,0(r30)`.

This doc nails down the allocator that makes this possible and lists the
init-phase call paths that allocate through it.

## The "allocator" is a per-frame bump arena — not a heap

Function `sub_821BB3D8` (0x821BB3D8, ~240 B, 436 callers, HOT, leaf) is the
allocator driving every draw-primitive construction in the 0x821Fxxxx /
0x8214xxxx / 0x821Bxxxx region. It is NOT `RtlAllocateHeap`. It is a
**double-buffered scratch/frame arena**.

Python-verified layout (all derived from the disassembly constants):

| field | address | purpose |
|-|-|-|
| arena base struct | `0x82B38B58` | ~32-byte descriptor |
| `+0x00..+0x04` (r11+0, r11+4) | `0x82B38B58/5C` | base pointers into chunks 0..1 |
| `+0x08..+0x18` | `0x82B38B60..70` | per-chunk in-use flags (bytes) |
| `+0x14` (r11+20) | `0x82B38B6C` | current chunk index (u32) |
| `+0x1C` (r11+28) | `0x82B38B74` | current bump offset (u32) |
| capacity | `0xFA000` = 1,024,000 B (1 MB) | hard-coded (`lis r11,15; ori r8,r11,40960`) |
| "need-swap" flag | `0x82A946BF` | set-then-cleared on chunk flip |

Allocator behaviour (r3=size, r4=alignment-mask):
1. Pad size up via `r6 = (-r3) & 0xF` (max 16-byte align).
2. If `off + pad + r3 < 0xFA000` → return `chunks[idx]+off`, bump off.
3. Otherwise → flip to the "other" chunk (the loc_821BB428 path writes flags[8+idx]=1, zeroes off), then satisfy from the new chunk.

Crucially there is **no free path per allocation**. Memory comes back when
chunk flags get cleared elsewhere (a whole-chunk reset, almost certainly at the
start of a rendering frame / once the prior chunk's DMA has drained). So:

**Any pointer that a caller hangs onto across a chunk flip is a dangling
pointer.** The bump arena does not care whether a live handle still references
the memory.

## Init-phase consumers (all allocate through `sub_821BB3D8`, all reachable
during boot BEFORE the first scene-frame flips the chunk)

Six callers of the "handle-constructor" `sub_821BD028` (which calls
`sub_821BB3D8(12,0)` or `(8,0)` inside `sub_821F6550`, then writes two
vtables in succession into the fresh block — `0x82000974` then `0x820012A0`
at offset +0 — bumps a global seq counter at `0x82A925F0`, and chains to
`sub_821BAE88` / `sub_821465B0`):

| caller | role | hint |
|-|-|-|
| `sub_821B5C90` | loading-screen / frontend draw | strings `LOADING...`, `FADE_LOAD`, `CARGA...`, `CHARGEMENT...`, `BELADUNG...`, `CARICAMENTO...` |
| `sub_8214DBD0` | language + pad UI init | strings `LOADLANG`, `NO_PAD`, `"%d$"` format |
| `sub_821506E8` | same frontend suite (called by `sub_8214DBD0`) | sibling of the above |
| `sub_821F1EE0` | widget/frontend draw helper | parent `sub_821F5788` → `sub_821F6550` sync |
| `sub_8215ED38` | radar-blip / target-icon draw (5.5 KB) | strings `"icon"`, `"target"`, `"%d ~c~%d"` |
| `sub_8229D8A8` | large frontend emitter (5.2 KB) | writes `r31=-1` sentinels before calls |

Additional consumers that go through `sub_821F6550` (HOT, 38 callers, top-500):
`sub_82144800`, `sub_8214EEC8`, `sub_8218E008`, `sub_821C62C0`, `sub_821F6680`,
`sub_82202540`, `sub_82223CF8`, `sub_8222C360`, `sub_8224CF58`, `sub_8224D1E0`,
`sub_8224DFC8`, `sub_822518B0`, plus 13 more.

## State-machine globals around the arena (also init-sensitive)

`sub_821F6550` uses a "lazy init, latch result" pattern pinned at these data
slots (Python-verified from the lis+lwz pairs):

| slot | address | meaning |
|-|-|-|
| signed-int handle | `0x824FB9A4` | -1 until first caller resolves it |
| paired handle | `0x824FB9A8` | passed to `sub_821BD028` on the "one-shot init" branch |
| state-table base | `0x8251F898` | ~5010+B table, entries stride 4 off r30 |
| initialised flag | `0x82520C2A` (table+5010) | set to 1 after first successful init |
| alt state table | `0x8252131C` (+ stride 68) | parallel per-index table |
| call-seq counter | `0x82A925F0` | incremented by `sub_821BD028` on every handle mint |

The `(r31+5010)` byte latches "init done". If a handle minted during the
*first* pass through this code (loading screen, before first frame flip) is
still referenced after the bump arena rolls, the latched state says "already
initialised", so the next draw just `lwz r9,0(r30)` on a stale pointer — this
matches exactly the reported crash pattern.

## Why this fits the first-draw UAF

The prior wrapper analysis (`02-sub_8227F5B8-wrapper.md`) showed the wrapper
faithfully forwards whatever handle it got; the poison `0xFFE1E1E1` appears
inside `sub_8227F2E8` at `lwz r9,0(r30)` — `r30 = *(caller_r4_slot)`. That
slot is populated from a lookup that on PC/recomp timing returns a pointer
into chunk 0 of the bump arena, but on the very first scene frame the chunk
flip writes chunk 0's `flags[8+idx]=1` without tearing down any of the
pointers `sub_821F6550`/`sub_821BD028` stashed at `0x824FB9A4..A8` and in the
`0x8251F898`/`0x8252131C` tables. Result: poison deref on first SetVS/SetPS.

## Boot-phase entry points feeding this path

`sub_821B5C90` (loading/frontend) is indirectly invoked (zero direct callers in
the symbol table → vtable/callback dispatch; fits a frontend draw-state
vtable). It calls BOTH `sub_821F6550` and `sub_8227F5B8` (the draw-quad
wrapper), AND it calls `sub_829FFB80` = `rand` (boot-time RNG seeding) and
`sub_828C1958`/`sub_828BD648` (GPU cmd submission). That is the archetypal
"loading screen draws a fade rectangle using scratch-arena geometry" entry
point, reachable while only 0..N shaders have ever been bound (SetVS #1 / SetPS
#1).

The two sites in `sub_8229D8A8` that pre-write `r31 = -1` (from
`02-sub_8227F5B8-wrapper.md`) are pass-through sentinels, not the UAF source.
**The UAF arrives through one of the six other bump-arena sites** — with the
loading-screen path (`sub_821B5C90`) and the HUD/radar path
(`sub_8215ED38` / `sub_821F5788`) as the primary suspects because they run
during boot before the first scene frame.

## Recommendation for the fix surface

- Instrument `sub_821BB3D8`: log chunk-flip events and dump the current bump
  offset; correlate against `sub_821BD028` handle-mint seq counter
  (`0x82A925F0`). Any handle whose backing address is outside the active
  chunk's window at use-time is a UAF.
- Watch-point the "init done" byte `0x82520C2A` and the paired handle at
  `0x824FB9A8`; re-minting after the first frame without a reset is fine, but
  re-use of the pre-boot value after flip is the bug.
- Candidate hook target: wrap `sub_821BD028` so it zeros `0x82520C2A` and
  `0x824FB9A4/A8` on the *first chunk flip after boot*, forcing a fresh
  lazy-init pass instead of reading stale arena memory.
