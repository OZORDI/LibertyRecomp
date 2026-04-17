# Pool Helpers in 0x828C1xxx Region (Agent 4)

Targets: `sub_828C19C0`, `sub_828C21D0` — siblings of the crash site `sub_828C2300` (all called by `sub_8227F2E8`).

---

## Header / Quick Facts

| fn | addr | size | callers | callees |
|-|-|-|-|-|
| sub_828C19C0 | 0x828C19C0 | ~0x5E8 (~1512 B, 854 recomp lines) | 99 (top-500 hot) | 1: sub_828C8588 |
| sub_828C21D0 | 0x828C21D0 | ~0xC0 (~192 B) | 46 (top-500 hot) | 4: sub_828BF1F8, sub_828BF248, sub_828C00B0, sub_828C20B0 |
| sub_828C2300 (crash site, reference) | 0x828C2300 | ~0x50 (~80 B) | 46 | 1: sub_828BF270 |

Sibling call order inside `sub_8227F2E8` (single caller responsible for the crashing frame):

1. `sub_828C19C0(r3=2, r4=<ptr>)`          – lookup-table write
2. `sub_8227EE90(0)`
3. `sub_828C6568(...)` + `sub_828C64C8(...)` – (other helpers)
4. `sub_828C21D0(r3=4, r4=4)`              – **POOL INIT (alloc + register pool)**
5. `sub_828C2290(...)` x4                   – **4 PUSH operations into the pool**
6. `sub_828C2300()`                         – **POOL TEARDOWN (free + reset)**  ← crashes at +0x34
7. `sub_828C6500(...)` + `sub_828C60A0(...)`

The pool that `sub_828C21D0` builds is the exact same pool that `sub_828C2300` tears down — all three functions touch the control block whose base is **`0x831C2D28`**, with the critical word at **`0x831C2D38`** (the expected value of `r31` at the crash).

---

## Memory Layout — The Shared Pool Control Block

All three sibling functions dereference the same control block relative to `r11 = 0x831C0000` (from `lis r11, -31972`) with offsets 11560 / 11568 / 11576 / 11580 (note: `sub_828C21D0` uses `r11+11576` as its anchor, `sub_828C2290` uses `r11+11580`, they are the same pool).

| abs addr | offset from pool anchor (0x831C2D38) | field | written by | read by |
|-|-|-|-|-|
| 0x831C2D28 | -16 | **handle / write-cursor** (36-B stride) | `sub_828C21D0` (alloc), `sub_828C2290` (advance by +36), `sub_828C2300` (zero) | `sub_828C2290` (check != 0 for insert), `sub_828C2300` (check != 0 for free) |
| 0x831C2D30 | -8 | **type-key** (param1 from init) | `sub_828C21D0` | (none in these fns — informational) |
| **0x831C2D38** | **0** | **limit / "valid"-flag** (param2 from init) | `sub_828C21D0` (sets), `sub_828C2300` (zeros) | `sub_828C2290` (test != 0 gate) |
| 0x831C2D3C | +4 | **counter** | `sub_828C21D0` (=0), `sub_828C2290` (++) | `sub_828C2290` |

This is a **bump-alloc queue** (not a mutex-protected freelist). Each `sub_828C2290` call:

- if limit `@+0` == 0 → raw push mode (write 36 B at `handle`, advance `handle += 36`, counter++)
- otherwise → counter++ only

It behaves like a per-frame particle / "deferred-draw" command queue (9 floats + 1 word = 36 B/slot).

The crash-site behavior of `sub_828C2300` is literally:

```c
if (pool[-16] != 0) { free(pool[-16]); pool[-16] = 0; }   // free cursor
pool[0] = 0;                                              // clear limit flag  ← stw r11, 0(r31) CRASHES
```

The faulting store is the **second** store — zeroing the limit flag — using `r31 = 0x831C2D38`. Crash has `r31 = 0x20`.

---

## sub_828C19C0 — `RegisterTableEntry(kind, ptr)` [Dispatch + Lookup Table Publisher]

**Role:** general-purpose "set table slot + apply kind-specific side-effects" helper. **This is NOT the pool the crash references.** It writes into two DIFFERENT global arrays.

**Prototype:** `void sub_828C19C0(int kind /*r3, range 0..36*/, uint32_t value /*r4*/)`

**Globals touched:**
- `(uint32_t*)0x831C2740`  — primary table of 37 slots  (`arr[kind] = value`; base = `lis(-31972)+10048`)
- `(uint32_t*)0x831C27E4`  — second table written by kind=2 subswitch (`0x831C0000 + 10212`)
- `(uint32_t*)0x82093248`  — lookup ROM (jump-table read-only base for case 0 path)
- `(uint32_t*)0x82093208`  — lookup ROM (case 15 path)

**Decompiled:**

```cpp
// Global: uint32_t g_kindTable[37] @ 0x831C2740;
// Global: uint32_t g_kind2Sub    @ 0x831C27E4;

void sub_828C19C0(uint32_t kind, uint32_t value) {
    g_kindTable[kind] = value;           // unconditional slot write
    if (kind > 36) return;               // (hard cap — early-out via bgt cr6)

    switch (kind) {
    case 0: {                            // lookup into ROM[kind2]
        uint32_t v = *(uint32_t*)(0x82093248 + value*4);
        sub_828C8588(6, v); return;
    }
    case 1:  /* loc_828C1DE4 */ return;
    case 2: {                            // NESTED sub-dispatch: "audio/particle kind routing"
        g_kind2Sub = value;              // 0x831C27E4
        if (value > 13) return;
        switch (value) {
        case 0: sub_828C8588(4, 6); sub_828C8588(5, 7);  sub_828C8588(23, 0); return;
        case 1: sub_828C8588(4, 1); sub_828C8588(5, 1);  sub_828C8588(23, 0); return;
        case 2: sub_828C8588(4, 1); sub_828C8588(5, 1);  sub_828C8588(23, 1); return;
        case 3: sub_828C8588(4, 7); sub_828C8588(5, 6);  sub_828C8588(23, 0); return;
        /* ... cases 4..13 follow same pattern (pair + variant index) ... */
        }
    }
    case 3:  /* loc_828C1D44 */ sub_828C8588(...); return;
    /* cases 4..36: each writes specific (constant, value) pairs via sub_828C8588 */
    case 7:  sub_828C8588(3, value); return;
    case 8:  sub_828C8588(28, value); sub_828C8588(10, ??); return;
    case 15: {                           // mirror of case 0 with different ROM
        uint32_t v = *(uint32_t*)(0x82093208 + value*4);
        sub_828C8588(19, v); return;
    }
    /* ... 28 more cases, each a small bl sub_828C8588(const, subkey) ... */
    }
}
```

**`sub_828C8588(idx, v)`** is a 24-byte leaf: `g_table2[idx] = v; return sub_828E02E8();` — i.e. "publish to a second table and invalidate downstream" (tail-calls a rebuild/update helper). Its base is `lis(-32079)+(-11648) = 0x82D14000 - 11648 = 0x82D11300`.

**Pool lifecycle role:** NONE — this fn writes into global config arrays, not into the `0x831C2D28` pool. It is a *sibling in the caller, not in the data structure*. It does a lot of publishing to parallel config/state tables that downstream systems consume. Any thread that reads `g_kindTable[x]` while `sub_828C19C0` is mid-dispatch will see partial updates (no locking), but this is not the direct cause of the `sub_828C2300` crash.

**Globals touched with 0x831Cxxxx prefix:** `0x831C2740` (main table), `0x831C27E4` (kind=2 subkey). **Does NOT touch 0x831C2D38.**

---

## sub_828C21D0 — `PoolBegin(type, limit)` [Pool Init / Heap-Backed Open]

**Role:** allocates a 36-B record slot via the RAGE small-block allocator and publishes it as the CURRENT pool at `0x831C2D28..0x831C2D3C`.

**Prototype:** `void sub_828C21D0(uint32_t type /*r3*/, uint32_t limit /*r4*/)`

**Globals touched:**
- `(uint32_t*)0x831C3DDC` — "context flag A" (read-only guard) — tests != 0
- `(uint32_t*)0x831C3DE4` — "context flag B" (read) — tests != 0; ALSO written by `sub_828C00B0` helper
- `(uint32_t*)0x831C2298` — heap context / allocator handle passed to sub_828BF1F8
- `0x831C2D28`            — **pool handle** (store-only, from allocator)
- `0x831C2D30`            — **pool type** (store-only, r31 param)
- `0x831C2D38`            — **pool limit** (store-only, r30 param)  ← *the address the crashing fn later zeros*
- `0x831C2D3C`            — **pool counter** (store-only, = 0)

**Decompiled:**

```cpp
void sub_828C21D0(uint32_t type, uint32_t limit) {
    // ---- 1) Guard: if neither context flag is set, probe and install a default context ----
    uint32_t flagA = *(uint32_t*)0x831C3DDC;
    uint32_t flagB = *(uint32_t*)0x831C3DE4;
    bool any_active = (flagA != 0) || (flagB != 0);

    if (!any_active) {
        uint32_t probe = sub_828C20B0(1);                  // read g_kindTable[1]
        // "is-zero" predicate: cntlzw + shift gives 1 iff probe==0
        bool probe_zero = (probe == 0);
        sub_828C00B0(/*r3 =*/!probe_zero, /*r4 =*/0);      // sets flagB (0x831C3DE4) + flagA (0x831C3DDC)
    }

    // ---- 2) Commit the currently-pending slot (flushes prior pool if any) ----
    uint32_t heapCtx = *(uint32_t*)0x831C2298;             // = g_allocCtx
    sub_828BF1F8(heapCtx);                                 // "commit/sync" — see notes below

    // ---- 3) Allocate a fresh 36-byte slot from the same heap, with the given alignment/key ----
    uint32_t handle = sub_828BF248(type, limit, /*size=*/36);

    // ---- 4) Publish as current pool ----
    *(uint32_t*)0x831C2D28 = handle;    // handle / cursor
    *(uint32_t*)0x831C2D30 = type;      // type/key
    *(uint32_t*)0x831C2D38 = limit;     // limit flag (non-zero = "pool active")
    *(uint32_t*)0x831C2D3C = 0;         // counter
}
```

**Callee semantics (verified):**

- **`sub_828BF1F8(ctx)`** — commit/flush helper. Reads `g_curSlot @ 0x831C22D8`, and IF `ctx != 0 && ctx != g_curSlot` it installs `g_curSlot = ctx` and tail-calls `sub_82A42930(heap, *(uint32_t*)ctx)` (returns a value to that slot). This is the **previous pool commit** — i.e. if a pool was already open and un-flushed, this closes it. **Returns 0 always.** It reads `*(uint32_t*)0x831C2298 + 8868 = 0x831C4534` = heap handle, and `+8872 = 0x831C4538` = current-slot sentinel.

- **`sub_828BF248(r3, r4, r5)`** — 40-B leaf. Reads a per-type function-pointer table at `0x82D13320 + r3*4` (= `lis(-32079)+(-19152)+r3*4`) and tail-calls `sub_82A3DAB0(g_heap, fn_ptr, r4, r5)`. `sub_82A3DAB0` is the **RAGE small-block allocator main entry** (~0x4A0 bytes, 10 callees including `__savegprlr_22`). So `sub_828BF248` is effectively `heap_alloc_typed(kind, arg, size)`.

**Pool lifecycle role:** **INIT / OPEN.** This is the "BeginPool" helper. It allocates a single 36-B scratch slot and registers it at `0x831C2D28..3C`. Every subsequent `sub_828C2290(f1..f8, r9)` writes one 36-B record into the slot (and advances the cursor).

---

## Cross-check against `sub_828C2300` (crash site reference)

```cpp
void sub_828C2300(void) {                              // "PoolEnd / Reset"
    uint32_t* r31 = (uint32_t*)0x831C2D38;            // anchor
    if (r31[-4 /*byte -16*/] != 0) {                   // i.e. *(0x831C2D28) != 0 (handle present)
        sub_828BF270();                                // heap_free(*(0x831C2D28))
        r31[-4] = 0;                                   // clear handle
    }
    r31[0] = 0;                                        // <<< clear limit @ 0x831C2D38  -- FAULTING STORE
}
```

**`sub_828BF270()`** — 16-B leaf: `sub_82A3DF50(g_heap)` which is a generic free/flush on the heap (5 callers). It **does not take the handle as an argument** — it uses the heap's internal "current slot" state (written by `sub_828BF1F8` and `sub_828BF248`). That internal state is the same `g_curSlot @ 0x831C4538` mentioned above. **This means freeing is not pointer-keyed; it operates on whatever the last bump-allocated slot was.** If the audio thread meanwhile bump-allocated a DIFFERENT slot into `g_curSlot`, this `sub_828BF270` frees the wrong slot — and the 36-B block still referenced at `0x831C2D28` becomes a dangling pointer.

The 0xFFE1E1E1 in r9 at the crash is RAGE's `FREED_MEMORY_FILL` pattern. `r9` in sub_828C2290 is **the r9 input argument that gets stored at `handle[+24]`** (see the 9-float + 1-word record layout above). A freed-memory pattern in r9 means one of the earlier `sub_828C2290` callers was handed a freed object pointer as input (it passes straight through as r9 into the slot).

---

## Call Maps

### `sub_828C19C0`

Callers (top 15 of 99):

| caller | addr |
|-|-|
| sub_82144188 | 0x82144188 |
| sub_82146E98 | 0x82146E98 |
| sub_8214DBD0 | 0x8214DBD0 |
| sub_82150060 | 0x82150060 |
| sub_82174FA0 | 0x82174FA0 |
| sub_82175038 | 0x82175038 |
| sub_8218D470 | 0x8218D470 |
| sub_8218D7A8 | 0x8218D7A8 |
| sub_821B5C90 | 0x821B5C90 |
| sub_821B7D38 | 0x821B7D38 |
| sub_821C3410 | 0x821C3410 |
| sub_821C3930 | 0x821C3930 |
| sub_821C3C38 | 0x821C3C38 |
| sub_821C4148 | 0x821C4148 |
| sub_821C5780 | 0x821C5780 |

(+84 more; spread across many rendering / audio registration paths.)

Callees: **`sub_828C8588`** only (~47 sites inside this function).

### `sub_828C21D0`

Callers (46 total, hot ones):

| caller | addr | notes |
|-|-|-|
| sub_82143C88 | 0x82143C88 | |
| sub_82150E08 | 0x82150E08 | |
| sub_82152938 | 0x82152938 | |
| sub_82154DE0 | 0x82154DE0 | |
| sub_8218DB88 | 0x8218DB88 | |
| sub_8218DF48 | 0x8218DF48 | |
| sub_821BD0A0 | 0x821BD0A0 | **CDrawRadarMapSectionDC::vfunc[0]** (display-command vtable slot 0 = exec) |
| sub_821BD138 | 0x821BD138 | **CDrawRadioHudTextDC::vfunc[0]** |
| sub_821BD238 | 0x821BD238 | **CDrawTriShapeDC::vfunc[0]** |
| sub_821C4148 | 0x821C4148 | (also calls 828C19C0) |
| sub_82276518 | 0x82276518 | |
| sub_8227E948 | 0x8227E948 | |
| sub_8227EA50 | 0x8227EA50 | |
| sub_8227EB58 | 0x8227EB58 | |
| **sub_8227F2E8** | **0x8227F2E8** | **← OUR CRASHING CALLER** |
| sub_8227F458 | 0x8227F458 | sibling (probably same pattern) |
| sub_8228BCF8 / sub_822906F0 / sub_82291FA8 / sub_822AC680 / sub_822AEF88 | — | |

The `CDraw*DC::vfunc[0]` hits are huge — they confirm **this pool is the Display-Command / Render-Queue helper** (not the audio OcclusionGroups pool). Every "draw this thing" command opens a 36-B slot, pushes up to N records, then closes it. The same pool slot is shared across `sub_8227F2E8` (a draw-text/shape helper) and all DisplayCommand vtable exec slots.

Callees (exactly 4):

| callee | role |
|-|-|
| sub_828BF1F8 | commit-pending / flush prior pool if any |
| sub_828BF248 | heap_alloc_typed(kind, arg, size=36) → via sub_82A3DAB0 |
| sub_828C00B0 | install context flags at 0x831C3DDC / 0x831C3DE4 |
| sub_828C20B0 | read g_kindTable[k] (bump-free read) |

### `sub_828C2300` (for symmetry)

**Callers are the SAME 46 addresses as `sub_828C21D0` with identical ordering** — every `PoolBegin` site pairs 1:1 with a `PoolEnd` site. Confirms they're open/close siblings.

Callees: `sub_828BF270` only (heap free).

---

## Globals Summary (0x831Cxxxx slots touched)

| addr | fn(s) | role |
|-|-|-|
| 0x831C2298 | 21D0 | g_allocCtx (read) |
| 0x831C2740 .. 0x831C27D0 | 19C0 | g_kindTable[0..36] (write) |
| 0x831C27E4 | 19C0 | g_kind2Sub (write in case=2 path) |
| **0x831C2D28** | **21D0 (set), 2300 (read/clear)** | **pool handle (cursor)** |
| 0x831C2D30 | 21D0 (set) | pool type/key |
| **0x831C2D38** | **21D0 (set), 2300 (clear)** | **pool "limit" / valid flag — THIS IS THE CRASH'S EXPECTED r31** |
| 0x831C2D3C | 21D0 (init 0) | pool counter |
| 0x831C3DDC | 21D0 (read-only guard) | context flag A |
| 0x831C3DE4 | 21D0 (read), sub_828C00B0 (write) | context flag B |

Neither `sub_828C19C0` nor any fn it calls touches `0x831C2D38`. `sub_828C21D0` is the sole WRITER of `0x831C2D38`, and `sub_828C2300` is the sole CLEARER. The pool is a **single-slot singleton** — there is only one at a time.

---

## Conclusion: Pool Lifecycle & Race Window

**Lifecycle roles:**

- `sub_828C19C0` — **not part of the pool**. A config-publisher. Writes into `g_kindTable` at `0x831C2740` and invokes `sub_828C8588` for downstream mirror writes. Irrelevant to the direct crash other than being called earlier in the same outer function.
- `sub_828C21D0` — **POOL INIT / BEGIN**. Allocates 36 B via `sub_828BF248`/`sub_82A3DAB0` and publishes the handle+type+limit at `0x831C2D28..3C`. Sets the limit flag (`0x831C2D38 = r4`) that downstream `sub_828C2290` checks.
- `sub_828C2300` — **POOL TEARDOWN / END**. Frees the current slot via `sub_828BF270`/`sub_82A3DF50` and zeroes the limit flag at `0x831C2D38`. Also zeros the handle at `0x831C2D28`.

**The pool is a SINGLETON bump-queue at `0x831C2D28`, not a mutex-protected free-list.** There are no atomic ops, no lock acquire/release anywhere in these three functions. The entire design assumes single-threaded use.

**Race window (three distinct possibilities, all consistent with the crash):**

1. **Double-tear-down.** If any other thread entered `sub_828C2300` between our `sub_828C21D0(4,4)` and our `sub_828C2300()`, the first call already freed the handle and cleared the slot. The limit flag `0x831C2D38` is now `0`. Our `sub_828C2290` calls in between read `limit==0` and fall into the *insert* path, bump-allocating into whatever `handle` was left at `0x831C2D28` (possibly 0, possibly pointing at freed memory — matches `r9 = 0xFFE1E1E1`). When our `sub_828C2300` runs, `pool[-16]==0` so the free branch is skipped — but `stw r11, 0(r31)` should still succeed since `r31` is a local `addi`. **UNLESS the recompiled `r31` register was itself spilled across a context switch.**

2. **Race with `sub_828C21D0` reopen.** Thread B calls `sub_828C21D0` between our pushes and our `sub_828C2300`. `sub_828BF1F8` COMMITS the open pool (moves it into `g_curSlot @ 0x831C4538`), then `sub_828BF248` overwrites `0x831C2D28` with a fresh handle. Our subsequent `sub_828C2300` frees the *new* handle — not the one our pushes filled. The filled handle stays in `g_curSlot` as orphaned. Later frees use `g_curSlot` and hit freed memory.

3. **`r31` spill corruption.** The crashing store is `stw r11, 0(r31)` at `sub_828C2300+0x34`. Just before it, the function did `addi r31, r11, 11576`. The observed `r31=0x20` can only happen if the recompiled context's `r31` field was overwritten by a different guest thread after the `addi` and before the `stw`. In PPC_FUNC the host thread doesn't yield registers mid-function — unless the PPCContext is shared. `0x20` = 32 — same numeric constant passed as `size` in certain allocator paths (`li r5, 36` above; or adjacent constants). **Most likely root cause: the audio thread and the draw thread share a PPCContext that the harness is not setting up per-thread, so both threads trample each other's r-registers.**

**Connection to the OcclusionGroups hypothesis:** this pool is used by `CDrawRadarMapSectionDC::vfunc[0]`, `CDrawRadioHudTextDC::vfunc[0]`, `CDrawTriShapeDC::vfunc[0]` — i.e. it is the **Display-Command execution pool**, not the audio OcclusionGroups pool. But `sub_8227F2E8` (our crashing caller) is invoked from both rendering and audio callbacks (see callers at `sub_82276518`, `sub_8228BCF8` — audio-tree evaluator sites). If the audio mixer thread calls `sub_8227F2E8` while the render thread is mid-pool-push on the same 0x831C2D28 slot, possibility (2) above is the concrete race. The `initial_count=1` mutex in the audio pool, mentioned in the hypothesis, is AROUND this draw-pool usage — not inside it. The draw pool itself has NO mutex.

**Recommended fix direction for the repair task:**

- Audit per-thread PPCContext setup in the recomp harness first (explains r31=0x20).
- If contexts ARE per-thread, wrap `sub_828C21D0` + pushes + `sub_828C2300` in a single critical section at the `sub_8227F2E8` site (the pool is single-slot global and cannot be safely interleaved).
- Verify `sub_828BF270` is using `g_curSlot` correctly; if two threads open pools concurrently, one `sub_828BF1F8` commit overwrites the other's pending slot.
