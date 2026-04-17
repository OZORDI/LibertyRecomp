# 06 — Bump-arena flip handler hypothesis

**Hypothesis tested**: There exists a "flip handler" or frame-end callback in
GTA IV's RAGE engine that SHOULD invalidate the latches when the arena flips,
but the recompiler is either not running it, or the latches are in a structure
that is not properly traced/registered.

**Verdict**: **PARTIALLY WRONG. The flip handler exists and IS called. But the
contract is "producer drains before flip"; the latches are external copies
held by client code and are outside the handler's scope.** Real root cause is
at a level *above* the arena: per-frame DC-queue draining relative to
chunk-flip ordering.

## The arena allocator (`sub_821BB3D8`, 0x821BB3D8, 212 B, leaf)

From Hex-Rays pseudocode, derived layout of the 32-byte control struct at
`0x82B38B58`:

| field | addr | role |
|-|-|-|
| `a1[0]` / `a1[1]` | `0x82B38B58/5C` | `chunks[0]`, `chunks[1]` base ptrs |
| `a1[2]` / `dirty[]` | `0x82B38B60..67` | per-chunk dirty-flag byte array |
| `a1[3]` | `0x82B38B64` | auxiliary cursor buffer (10 000 B) |
| `a1[4]` | `0x82B38B68` | **write** chunk idx (next frame) |
| `a1[5]` | `0x82B38B6C` | **read** chunk idx (current frame) — alloc target |
| `a1[6]` | `0x82B38B70` | bytes reserved (used by DrawLists queue) |
| `a1[7]` | `0x82B38B74` | current bump offset inside active chunk |
| dirty flag | `byte_82A9263F` | "need-swap" — cleared on flip |

Arena cap: `0xFA000 = 1 024 000` bytes (Python-verified: `0xFA000 == 1024000`).
Chunk cap from init path `sub_821C0088`: `1 034 000` bytes (includes 10 000-B
epilogue). Two chunks = double buffer.

### The allocator does NOT track handles
The function returns `chunks[cur_idx] + bump_off` after a bump; no allocation
metadata is written back into the returned pointer, and there is no per-alloc
free path.

## The flip handler EXISTS: `sub_821BF990` @ 0x821BF990

Called from exactly **two** sites (`find_callers` result):
1. `sub_821B5890` — the per-frame end-of-frame routine
2. `sub_821B5A68` — shutdown/drain (calls the flipper 4× to clear both chunks)

The handler:
```
a1[6] = 0;              // reserved-bytes = 0
a1[7] = 0;              // bump_off = 0
a1[4] = 1 - a1[4];      // swap write idx
a1[5] = old_a1[4];      // advance read idx
dirty[old_read] = 0;    // unmark the chunk we're now reusing
sub_828DCA28(a1 + 12);  // drain sub-buffer #1
sub_828DCA28(a1 + 15);  // drain sub-buffer #2
memset(&a1[256*new_read + 18], 0, 1024);  // scrub the other table
dirty[new_read byte at +2120] = 0;
```
So the handler **does invalidate the arena-internal state**, but it knows
nothing about external latches. Nothing registers with it.

## The frame-end ritual (`sub_821B5890`, 0x821B5890, 372 B)

1. `sub_821B7EB8(dword_82B31960)` — GPU render-list close
2. `sub_821B7D28/88E8(dword_82B31960)` — submit + fence
3. `dword_82B38AB0 = 1 - dword_82B38AB0` — DrawList table double-buffer swap
4. **`sub_821BF990(&dword_82B38B58)` ← ARENA FLIP**
5. `dword_82B38B18[dword_82B38AB0] = -1` — reset matching table slot
6. `dword_82CABDF4 = 1 - dword_82CABDF4` — another double-buffer swap
7. `*(a1+4032) = 1 - *(a1+4032)` — thread-side swap
8. Backbuffer present (`sub_82849860`)

Chain: `sub_82140088` (main render tick, selected via `byte_831D5325`) →
`sub_821B5890`. `sub_82140088` is the frame mainloop: calls
`sub_821C0B38(5)`, game-logic dispatch, DrawList builder, then `sub_821B5890`
to close and flip.

## The DC queue is the real "flip handler" in disguise

`sub_821BB4C8` (CNewDrawListDC ctor): `sub_821BA858((int)dword_82B36E90, a2,
dword_82B38B74 - 16)` — pushes the DC's arena-offset into a per-chunk
ring-buffer of 128 × 28-byte slots keyed off `3596 * chunk_idx + a1`.

`sub_821BA928` (producer side, called by `sub_821B5B20` the render thread):
walks each queued DC, sets state=3, calls `sub_821BB2D0` to replay each DC
from the arena, sets state=4 on the slot.

`sub_821BB2D0` (drain): iterates the queue using `a1[v9]` where `v9 = a1[4]` —
i.e. it reads from **the WRITE-side chunk base** (set up before the producer
marks the chunk "full"). It then dispatches `vtbl[1]` on each DC to replay it.

**Ordering invariant the game relies on (Xbox 360 memory model)**:
1. Main thread builds DCs into chunk[`read`] with bump allocator.
2. Main thread queues DC offsets via `sub_821BA858`.
3. Main thread calls `sub_821B5890` → `sub_821BF990` (FLIP).
4. Render thread (`sub_821B5B20`) calls `sub_821BA928` → `sub_821BB2D0` to
   REPLAY the queue from what is now `write`-idx chunk (= old read).

The flip is **not** a free event — it is a producer/consumer handoff. The
queue is keyed on `chunk_idx` so when flip swaps `a1[4]` and `a1[5]`, the
consumer sees the now-frozen chunk.

## Where the latches actually fit

Agent 13 identified these globals (these are **not** DC pointers — they are
sibling state-machine latches around the arena):

| addr | symbol | role |
|-|-|-|
| `0x831E49B0` | `dword_831E49B0` | present in `.data` (symbol table hit) |
| `0x831E49DC` | `dword_831E49DC` | present in `.data` (symbol table hit) |
| `0x824FB9A4` | inside `sub_824FB6A0` | *not a global* — displacement inside code |
| `0x824FB9A8` | inside `sub_824FB6A0` | *not a global* — displacement inside code |
| `0x82520C2A` | inside `sub_82520BE0` | *not a global* — `CRenderPhaseMirrorReflection` ctor body |

`resolve_address` on 0x824FB9A4/A8 and 0x82520C2A reports "inside function
body". These were mis-identified upstream; they're `.text` offsets used as
`lis/lwz` targets. The real per-slot latches are:

- `dword_82A935A4` (handle sentinel, -1 when unresolved) — in `sub_821F6550`
- `dword_82A935A8` (paired handle, passed to `sub_821BD028`)
- `dword_82B98CB8[]` (array of "last-seen" per-slot pointers into `.bss`
  table `dword_82B98CC0`)
- `byte_82B9A04A` / `byte_82B9A140[]` ("init done" bytes, 68-byte stride)
- `dword_82A925F0` (**generation counter** — bumped every `sub_821BB4C8` /
  `sub_821BD028` call; shifted into bit 18+ of CBaseDC `a1[1]`)

## Why the latches are safe here — and where the bug really lives

1. `dword_82B98CB8[]` stores pointers into `.bss` tables, not arena memory.
   Not a UAF source.
2. `dword_82A925F0` generation counter IS the "invalidation token" —
   `sub_821BB2D0` line: `if ( a3 == v13[1] >> 18 ) v8 = 1;` — the drain
   compares the generation. This is exactly the flip-handler semantics the
   hypothesis asked for, but **implemented inline in the playback loop, not
   via a callback list**.
3. The *real* UAF vector is not the latches — it is the **stack-spill
   references** to DCs (sp+92 → sp+300 in `sub_8227F2E8` and callers) that
   persist while the arena flips mid-function. Agent 13's own recommendation
   ("wrap `sub_821BD028` so it zeros `0x82520C2A` and `0x824FB9A4/A8`") targets
   the wrong thing.

## Validation design (watchpoints — NO IMPL)

- **WP1**: write watch on `dword_82B38B68` / `dword_82B38B6C` (chunk-idx
  swap). Log: cycle counter, current stack backtrace, and the DC-queue head
  `*(dword_82B36E90 + 3592 * 2)`.
- **WP2**: read watch on any address in range
  `[chunks[old_read] .. chunks[old_read]+0xFA000)` *after* the swap at WP1
  fires — any hit between WP1-fire and the next call to `sub_821BB2D0` with
  the matching chunk_idx is a UAF on a stale arena pointer.
- **WP3**: read watch on the generation counter compare site inside
  `sub_821BB2D0` at offset `+a1[6]` reads — verify `(v13[1] >> 18) ==
  expected_gen`; any mismatch means the queue has stale entries.
- **WP4**: instrument `sub_821BA858` enqueue — log (chunk_idx, bump_off, gen)
  for every push. On the first crash, dump the queue vs. the current chunk
  window.

## Design of the fix (root-cause targeting, NO IMPL)

The bug is **not** missing latch invalidation — the flip handler in
`sub_821BF990` is already running and the generation-compare in
`sub_821BB2D0` already guards replay. Two root causes remain that require
research, not code:

1. **Queue-vs-flip ordering on recompiled Mach-O**: On Xbox 360 the render
   thread stalls on the GPU present fence (`sub_828497D8`) before the main
   thread reaches the flipper. On recomp host, PPC reordering or the lack of
   X360 weak-memory fences may mean `sub_821BF990` runs while
   `sub_821BA928` is mid-drain — the producer flips chunk-idx before the
   consumer has reached the `v8 = 1` sentinel. Needs an empirical trace of
   `sub_821B5890` → `sub_821BF990` vs `sub_821B5B20` → `sub_821BA928` timing
   on host.

2. **Stack-spill pointer lifetime across a yielded guest thread**: functions
   that call `sub_821BB3D8` then yield (e.g. `sub_821BB4C8` via `sub_821BA858`
   which calls `sub_824F3278`, a sync primitive) spill the returned pointer
   into stack slots that survive across the yield. If the cooperative thread
   scheduler re-enters the main thread and it runs `sub_821B5890` before
   resuming the producer, the spilled pointer is into a now-swapped chunk.

### Correct fix surface (to research, not implement):
- **Option A**: wrap `sub_821BF990` with a hook that delays the swap until
  the render-thread DC queue (`dword_82B36E90 + 3596 * old_write + 3592`
  i.e. the "pending count") reaches 0. This preserves the producer/consumer
  contract without touching latches.
- **Option B**: wrap `sub_821BB2D0` to refuse replay of any DC whose `(v13[1]
  >> 18)` generation is less than the current arena generation — a strict
  interpretation of the existing inline gen-check, turning it into a hard
  invariant. The existing check only terminates the loop on match; it does
  not *skip* stale entries before match.
- **Option C** (if scheduling turns out to be the cause): hook
  `sub_824F3278` / `sub_824F35E8` (the guest yield/wait points) to drain the
  DC queue before chunk flip can occur.

The hypothesis's "register latches with a flip handler" approach is
incorrect for this arena — the game uses a generation-stamp design, not a
callback-list design, and the stamp is already present at bit 18+ of every
DC's `a1[1]` word.

## Key addresses (Python-verified)

```
ARENA_CAP    = 0xFA000  (1 024 000 B)
CHUNK_CAP    = 1 034 000 B  (from sub_821C0088 alloc)
ARENA_BASE   = 0x82B38B58
FLIPPER      = 0x821BF990  (sub_821BF990)
FRAME_END    = 0x821B5890  (sub_821B5890, 2 callers: sub_82140088, sub_821428C8)
SHUTDOWN     = 0x821B5A68  (calls flipper 4x)
DC_QUEUE     = dword_82B36E90 (CDrawListManager? 7200+ B)
DC_ENQUEUE   = 0x821BA858  (sub_821BA858)
DC_DRAIN     = 0x821BA928  (sub_821BA928, 1 caller: sub_821B5B20)
DC_REPLAY    = 0x821BB2D0  (sub_821BB2D0 — the generation-compare site)
GEN_COUNTER  = 0x82A925F0  (dword_82A925F0, shifted into DC[1] bit 18+)
```
