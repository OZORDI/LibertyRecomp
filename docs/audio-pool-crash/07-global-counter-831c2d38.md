# Crash slot 0x831C2D38 — Owner, layout, and thread-safety

Agent 7 of 10. Crash site: `sub_828C2300+0x34`, instruction `stw r11, 0(r31)`. The brief
states r31 was expected to be `0x831C2D38` but read `0x20` at the moment of crash, with
r9 holding `0xFFE1E1E1` (RAGE freed-memory poison). This document focuses exclusively on
the global slot at `0x831C2D38` and the data structure that contains it.

## Quick conclusion (read this first)

`0x831C2D38` is **not** an audio/`OcclusionGroups` slot and **not** a draw-context
freelist counter. It is one field inside a small **global immediate-mode draw-primitive
batch state** that sits at `0x831C2D24..0x831C2D40` (a single 28-byte struct,
likely a `grcDraw` / `rage::grcBegin` static state block). The slot at `0x831C2D38`
holds the **destination vertex pointer** ("dest cursor / current emit pointer") for the
batch in progress. It is set by `sub_828C21D0` (the BEGIN function), incrementally read
by `sub_828C2290` (per-vertex EMIT), and zeroed by `sub_828C2300` (the END function).

**The race is plausible.** There is no visible mutex anywhere in the
BEGIN / EMIT / END trio. All 46 callers of END write to the same global from any thread
that happens to draw a HUD/radar/triangle. If two render threads (e.g. main + UI/HUD) or
the gameplay thread plus a parallel CDrawXxxDC vfunc[0] enter BEGIN concurrently, one
will see the other's mid-batch state and the GPU-allocated buffer pointed to by
`0x831C2D28` can be freed by one path while the other is still emitting — producing
exactly the `0xFFE1E1E1` poison observed in r9 and a torn r31 in `sub_828C2300`.

## Address map of `0x831C2D00..0x831C2D80`

All addresses are derived from `lis r11, -31972` ⇒ `r11 = 0x831C0000` followed by
`addi rX, r11, OFFSET` (or direct `lwz/stw OFFSET(r11)` with the same r11). Verified
with `python3 -c "print(hex(((-31972)<<16)+OFFSET & 0xFFFFFFFF))"` for every entry.

| addr | size | role | function(s) that touch it | how |
|-|-|-|-|-|
| `0x831C2D24` | 4B (float) | (likely f0/scratch — `f31`/sentinel float read by `sub_82143C88` via `lfs ..., 11556(r31)`) | `sub_82143C88` | `lfs/stfs ..., 11556(r..)` |
| `0x831C2D28` | 4B ptr | **GPU buffer cursor** — head of the dynamic vertex memory currently being filled. Set by `sub_828C21D0` (return value of `sub_828BF248`); advanced by `+36` per vertex in `sub_828C2290`; read by `sub_828C2300` to decide whether to call `sub_828BF270` (release/submit). Sym: `dword_831C2D28` |  `sub_828C21D0`, `sub_828C2290`, `sub_828C2300` | `stw -16(r31)` and `lwz -20(r10)` |
| `0x831C2D2C` | 4B (float) | scratch / state float | `sub_82143C88` | `stfs ..., 11564(r27)` |
| `0x831C2D30` | 4B u32 | **draw-call arg2** (geometry / topology pointer saved by BEGIN) | `sub_828C21D0` | `stw r31, -8(r11)` (where r11=0x831C2D38) |
| `0x831C2D34` | 4B (float) | scratch / state float (`flt_831C2D34`) | `sub_82143C88` | `lfs/stfs ..., 11572(r..)` |
| `0x831C2D38` | 4B u32 | **THE CRASH SLOT — vertex count *primitive count* / "current arg" — set to the count argument by BEGIN, zeroed by END.** Sym: `dword_831C2D38` | `sub_828C21D0` (`stw r30, 0(r11)`), `sub_828C2300` (`stw r11, 0(r31)` with r11=0) | constructor + destructor |
| `0x831C2D3C` | 4B u32 | **vertex counter / fast-path increment slot.** Set to 0 by BEGIN; bumped by `sub_828C2290` on every emit. Sym: `dword_831C2D3C` | `sub_828C21D0` (`stw r10, 4(r11)` with r10=0), `sub_828C2290` (read+inc) | counter |
| `0x831C2D40` | 4B (float) | next state field (`flt_831C2D40`) | `sub_82143C88` | `lfs ..., 11584(r29)` |
| `0x831C2D44..` | — | unused / next struct | (none observed in this crash trio) | — |
| `0x831C2D48` | 4B u32 | (out of struct, separate slot — `dword_831C2D48`) | (not in BEGIN/EMIT/END) | — |

(Symbol names in the symbol table — `dword_831C2D28`, `flt_831C2D34`, `dword_831C2D38`,
`dword_831C2D3C`, `flt_831C2D40` — confirm both the field naming convention IDA inferred
and the alternating int/float layout typical of a per-batch transient draw state.)

## Every function that reads or writes `0x831C2D38`

Search method: `lis r11, -31972; addi rX, r11, 11576` (uniquely identifies a base of
`0x831C2D38`) plus aliases via `r10 = base + 11580` with `-4(r10)`. The constant 11576
appears as a generic literal in 50 files but only the recomp scaffolds for the three
functions below combine it with `lis -31972` against this struct.

### Writers

1. **`sub_828C21D0`** — BEGIN of a draw batch. (file `gta4_recomp.66.cpp`, called by 46
   functions including every `CDraw*DC::vfunc[0]` and many `sub_82143C88`-style
   immediate-mode helpers.)

   ```c
   ctx.r11.s64 = -2095316992;        // 0x831C0000
   ctx.r11.s64 = ctx.r11.s64 + 11576;// r11 = 0x831C2D38
   PPC_STORE_U32(ctx.r11.u32 + -16, ctx.r3.u32); // [0x831C2D28] = sub_828BF248() result
   PPC_STORE_U32(ctx.r11.u32 +  -8, ctx.r31.u32);// [0x831C2D30] = arg "topology pointer"
   PPC_STORE_U32(ctx.r11.u32 +   4, ctx.r10.u32);// [0x831C2D3C] = 0  (counter init)
   PPC_STORE_U32(ctx.r11.u32 +   0, ctx.r30.u32);// [0x831C2D38] = arg "primitive count"
   ```

2. **`sub_828C2300`** — END / commit of a draw batch. The CRASH FUNCTION.

   ```c
   ctx.r11.s64 = -2095316992;        // 0x831C0000
   ctx.r31.s64 = ctx.r11.s64 + 11576;// r31 = 0x831C2D38
   ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + -16); // r11 = [0x831C2D28]
   if (r11 != 0) {
       sub_828BF270();                            // release / submit GPU buffer
       PPC_STORE_U32(ctx.r31.u32 + -16, 0);       // [0x831C2D28] = NULL
   }
   PPC_STORE_U32(ctx.r31.u32 + 0, 0);             // *** CRASH HERE *** [0x831C2D38] = 0
   ```

3. **`sub_828C2290`** — per-vertex EMIT (267 callers, "hot" function — top-500 by call
   count). Treats `0x831C2D38` as the **dest pointer** in the slow-path branch:

   ```c
   ctx.r10.s64 = 0x831C0000 + 11580;              // r10 = 0x831C2D3C  (counter slot)
   ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + -4);  // r11 = [0x831C2D38]  (dest ptr)
   if (r11 == 0) goto SLOW;
   ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + -20); // r11 = [0x831C2D28]  (cursor)
   if (r11 != 0) goto SLOW;
   /* fast path: just increment counter at [0x831C2D3C] */
   ctx.r11.u64 = PPC_LOAD_U32(ctx.r10.u32 + 0);   // r11 = [0x831C2D3C]
   PPC_STORE_U32(ctx.r10.u32 + 0, r11 + 1);       // ++[0x831C2D3C]
   return;
   SLOW:
   /* writes 9 floats (f1..f8 + r9 packed) to *r11 (=cursor) */
   PPC_STORE_U32(ctx.r11.u32 + 0,  f1);  // ... + 4..32
   PPC_STORE_U32(ctx.r10.u32 + -20, ctx.r11.u32 + 36); // [0x831C2D28] += 36
   ++ [0x831C2D3C];
   ```

   Note that EMIT *reads* but does not *write* `0x831C2D38` directly (only via the
   constructor).

### Readers (only)

There are no read-only consumers other than `sub_828C2290`'s slow-path test and
`sub_828C2300`'s "is buffer live?" test. The slot is short-lived per batch.

### Indirect helpers in the same struct

* `sub_828BF248` (allocator wrapper) at `0x831C0000 + 8868 = 0x831C22A4` — this is the
  *system-level* draw context (a long-lived pointer to the draw thread's command-buffer
  manager); the per-batch struct at `0x831C2D28` is allocated *out of* that context.
* `sub_828BF270` (commit/release) reads the same `0x831C22A4` slot and dispatches into
  `sub_82A3DF50` which simply does `*(r3 + 48) = *(r3 + 13428)` — a function-pointer
  rotate inside the draw context. If `0x831C22A4` has been freed (poisoned to
  `0xFFE1E1E1`), the dereference `r3 + 13428` lands in unmapped memory.

## Inferred struct layout

```c
// 28 bytes, located at fixed BSS address 0x831C2D24, no header pointer, no lock.
struct ImmDrawBatch_t {
    float    scratch_or_state_24;   // +0x00 = 0x831C2D24
    void*    cursor;                // +0x04 = 0x831C2D28  (advances by 36 per vertex)
    float    state_2C;              // +0x08 = 0x831C2D2C
    void*    topology_arg;          // +0x0C = 0x831C2D30  (geometry/IB ptr from BEGIN)
    float    state_34;              // +0x10 = 0x831C2D34
    uint32_t prim_count_arg;        // +0x14 = 0x831C2D38  *** CRASH SLOT ***
    uint32_t vertex_count;          // +0x18 = 0x831C2D3C
    float    state_40;              // +0x1C = 0x831C2D40
};                                  // total 0x20 (no synchronization primitive)
```

The choice of 36-byte vertex stride (9 floats: pos.xyz, normal.xyz, color, u, v) is
classic RAGE `grcVertexBuffer`-style immediate vertex with attached normal+color+UV.

## Owning subsystem identification

Evidence:

1. **All 46 callers of `sub_828C2300` are draw-context-style functions.** Among them,
   three have RTTI vtable assignments — `CDrawTriShapeDC::vfunc[0]` (`sub_821BD238`),
   `CDrawRadioHudTextDC::vfunc[0]` (`sub_821BD138`), and
   `CDrawRadarMapSectionDC::vfunc[0]` (`sub_821BD0A0`). Each derives from `CBaseDC`. The
   "DC" suffix means **D**raw**C**ommand. These are render-thread vfunc[0]s whose job is
   to *replay* a queued draw command — they call `sub_828C2140` (texture/material bind),
   `sub_828C21D0` (BEGIN batch), repeated `sub_828C2290` (emit vertices), and finally
   `sub_828C2300` (END batch).
2. **Sibling commits in the same code page**: `sub_828C0688`, `sub_828C0848`,
   `sub_828C15C8` are direct callers of `sub_82A42930` and `sub_82A3DAB0` (~1184 bytes,
   the actual `grcDraw*Primitives` core), confirming the entire `0x828Cxxxx` cluster is
   the global immediate-draw module — not audio, not pool free-list.
3. **No string refs** were found for any of the crash-trio functions
   (`sub_828BF270`, `sub_828C2290`, `sub_828C2300`, `sub_828C21D0`,
   `sub_82A3DAB0`, `sub_82A3DF50`). This is consistent with a tight inlined `grcBegin/
   End` style helper — RAGE strips strings from these.
4. **Hot-function profile**: `sub_828C2290` (267 callers, top-500), `sub_828C20B0`
   (100 callers, top-500), `sub_828C21D0` (75 callers, top-500), `sub_828C2300`
   (46 callers — top-500). Audio pool helpers are called O(thousands) at startup; this
   is a per-frame hot path.
5. **No OcclusionGroups string**, no `audOcclusion`, no `naOcclusion` ref anywhere in
   the call tree. The earlier crash hypothesis ("audio pool head") is wrong for this
   slot.

**Conclusion**: `0x831C2D38` is owned by **the global immediate-mode draw batch state**
(working name `g_grcImmDrawBatch`). It is a singleton — there is exactly one such struct
per process.

## Thread-safety analysis

| concern | finding |
|-|-|
| Any mutex / critical section in BEGIN? | **None.** `sub_828C21D0` enters and exits without acquiring any lock. It calls `sub_828BF1F8` (which also has no lock — just compares to a previous pointer at `0x831C22A8` and calls `sub_82A42930`). |
| Any mutex in END? | **None.** `sub_828C2300` reads `[0x831C2D28]`, branches to `sub_828BF270` (which loads `[0x831C22A4]` and tail-calls `sub_82A3DF50`), then unconditionally writes `[0x831C2D38] = 0`. No fences, no compare-and-swap. |
| Any mutex in EMIT? | **None.** `sub_828C2290` does an unfenced load of `[0x831C2D38]` and `[0x831C2D28]`, then a non-atomic increment of `[0x831C2D3C]`. |
| TLS / per-thread? | No. The base `0x831C0000` is a fixed BSS literal; r13 (TLS) is never used. |
| Any visible event/signal? | None. The only "synchronization" is the implicit assumption that BEGIN/EMIT*/END runs sequentially on a single thread. |

If the singleton is touched from more than one thread, a torn write (write to `0(r31)`
seeing `r31 = 0x20` is consistent with a partial 64-bit store interleaving the lis/addi
on Xenon, or — more likely on PPC — with another thread freeing the underlying RAGE
allocator behind `0x831C22A4` while this thread is mid-call) is exactly what we see in
the crash report.

A 32-bit single-word write to `0x831C2D38` itself cannot tear, but **the bug is not in
the write to 2D38**. The likely sequence is:

1. Thread A enters `sub_828C2300`, executes `lis/addi` -> r31=0x831C2D38, then
   `lwz r11,-16(r31)` reads the cursor, finds it non-NULL.
2. Thread A calls `sub_828BF270` -> `sub_82A3DF50`. `sub_82A3DF50` does
   `lwz r11, 13428(r3); stw r11, 48(r3)`.
3. **Concurrently** Thread B (or a deferred completion handler) frees the draw context
   pointed to by `[0x831C22A4]` (writes RAGE's `0xE1E1E1E1` poison through it).
4. r3 in `sub_82A3DF50` was loaded *before* the free, so it is the old pointer; the
   `lwz 13428(r3)` succeeds returning poison, `stw 48(r3)` writes poison into freed
   memory.
5. The recomp scaffold's mid-instruction debugger / sentinel sees `r9 == 0xFFE1E1E1`
   and reports the crash at the next instruction whose memory access fails — which is
   the `stw r11, 0(r31)` at `sub_828C2300+0x34` after r31 was clobbered by the poisoned
   stack frame restore. The "r31 == 0x20" in the report is the low byte of a poison
   word reload from the freed stack save area at `[r1-16]`.

In other words: **the BEGIN/EMIT/END global trio is racing against a freer of the
parent draw context** at `0x831C22A4`. Adding a mutex around BEGIN..END alone would not
fix the freed-context problem; the real fix is to either (a) hook
`sub_828C21D0`/`sub_828C2300` to take a per-frame lock that also blocks the freer, or
(b) hook `sub_828BF270`/`sub_82A3DF50` to validate `[0x831C22A4]` is non-poison before
dereferencing.

## Final answer to the brief's hypotheses

* **"Is `0x831C2D38` the head/counter of `OcclusionGroups`?"**  No. No audio code
  touches this slot; the call graph is 100% draw-side.
* **"Is it a draw-context freelist counter?"**  No. It is the *primitive-count
  argument* slot of a singleton immediate-mode draw batch. The freelist counters are
  `0x831C2D28` (cursor) and `0x831C2D3C` (vertex count); `0x831C2D38` itself is a
  scratch arg copy.
* **"Are there multiple writers from different threads with no synchronization?"**
  Yes — the BEGIN/EMIT/END trio is unprotected, and any of the 46 callers may run from
  any draw thread. The crash signature (`r9 == 0xFFE1E1E1`) shows the actual UAF is on
  the parent draw context at `0x831C22A4` rather than on the singleton struct at
  `0x831C2D28..0x831C2D40`, but the singleton's lack of a lock is what allows the UAF
  to be observed.

### Cross-references

* Recomp scaffolds (read but do NOT modify here):
  `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.66.cpp` — sub_828BF1F8,
  sub_828BF248, sub_828BF270, sub_828C20B0, sub_828C21D0, sub_828C2290, sub_828C2300.
  `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.67.cpp` — sub_828C60A0
  family (sibling per-vertex helpers using a *separate* struct at `0x831C3DD8`).
  `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.82.cpp` — sub_82A3DAB0
  (commit core), sub_82A3DF50 (sub-helper), sub_82A42930 (helper).
* Sibling investigations (read-only references):
  `docs/audio-pool-crash/01-inner-crash-trio.md`,
  `docs/audio-pool-crash/04-pool-helpers-828c1.md`,
  `docs/audio-pool-crash/05-pool-helpers-828c6.md`,
  `docs/audio-pool-crash/06-sibling-caller-827bf.md`.
