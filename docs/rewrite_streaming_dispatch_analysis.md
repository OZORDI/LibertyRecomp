# Streaming Dispatch System Analysis

Generated 2026-03-28. All addresses from generated recomp code in `gta4_recomp.55.cpp` / `gta4_recomp.56.cpp`.

---

## 1. Architecture Overview

The streaming subsystem uses a **producer-consumer ring buffer** pattern with **semaphore-based signaling**:

- **Producer** (main thread): `sub_8285D9D8` enqueues work items into per-channel slot arrays and signals a semaphore.
- **Consumer** (worker thread): `sub_8285D808` dequeues and dispatches items, driven by the same semaphore.
- **Ring buffer**: 256-entry circular queue per channel, at struct offset `+8..+1031` (1024 bytes = 256 x 4-byte pointers). Write index at `+1056`, count at `+1064`, semaphore handle at `+1068`.

---

## 2. Thread Creation

### sub_8285D948 — Streaming Channel Factory

Called from `sub_8285D9D8` (the dispatcher init). Flow:

1. Loads global struct at `lis -31973; addi r31, r11, -17996` → static global `0x8283B974`.
2. Reads `[global+32]` (channel count, initialized to 0).
3. Allocates 1080 bytes via `sub_821B3510` (game malloc).
4. Calls `sub_8285D610` to initialize the channel struct (ring buffer, semaphore, worker threads).
5. Stores channel ptr into `global[channel_count * 4]`; increments `[global+32]`.

### sub_8285D610 — Channel Init

1. Zeroes struct fields: `[r31+0]=0`, `[r31+4]=0`, `[r31+6]=0`.
2. Calls `sub_8285FE48(r31+1032)` → **RtlInitializeCriticalSection** on the ring buffer's CS at struct offset `+1032`.
3. Calls `sub_82849778(0)` → **NtCreateSemaphore**(initial=0, max=32767). Handle stored at `[r31+1068]`.
4. Zeroes ring buffer control: `[r31+1064]=0` (count), `[r31+1060]=0` (read index), `[r31+1068]=handle`, `[r31+1056]=0` (write index).
5. Reads bitmask from `r27` (r8 arg) and iterates bits 0-7 to build a "channel type array" at `[sp+96..sp+124]`.
6. For each set bit: calls `sub_82158E08` (thread name builder) → `sub_8285D500` (creates worker thread for that channel type, passing the channel struct and type index).
7. Stores worker count in `[r31+4]`, total thread object ptr in `[r31+0]`.

**Key**: worker threads are created as real OS threads (via RexGlue's `XThread` → `std::thread`). Each worker presumably loops on `sub_82849790` / `sub_828497D8` waiting on the semaphore at `[channel+1068]`.

---

## 3. Semaphore Initialization

| Where | What | Args |
|-|-|-|
| `sub_82849778` (called from `sub_8285D610`) | NtCreateSemaphore via `sub_82A12EB8` | r4=handle_out, r5=32767 (max), r6=0 (initial) |

The semaphore starts at count=0 (no work available). Workers block until the producer signals.

---

## 4. Enqueueing: sub_8285D2C8

Called from `sub_8285D9D8` to add a work item to a channel's ring buffer:

1. `sub_8285FE78(ring+1024)` → **RtlEnterCriticalSection** (cond-enter: only if CS initialized).
2. Check count (`[ring+1064]`). If 255 → queue full, leave CS and return 0.
3. Advance write index `[ring+1056]` mod 256. Store work item ptr at `ring[write_idx * 4]`.
4. Increment count `[ring+1064]`.
5. `sub_8285FEE0(ring+1024)` → **RtlLeaveCriticalSection**.
6. **`sub_82849860([ring+1068])`** → **NtReleaseSemaphore**(handle, count=1) — wakes one waiting worker.

---

## 5. Dequeuing: sub_8285D808

This is the **consumer dispatch loop**, called from `sub_8285DAE8` (the drain/shutdown path):

1. `sub_8285FE78(ring+1024)` → EnterCriticalSection.
2. Read count `[ring+1064]`. If 255 → release CS via `sub_8285FEE0`, loop back (flow-control full case).
3. Read item from `ring[read_idx * 4]` where read_idx = `[ring+1060]` (reconstructed from write_idx - count).
4. Advance read tracking, decrement count.
5. `sub_8285FEE0` → LeaveCriticalSection.
6. `sub_82849860([ring+1068])` → signal semaphore (notifies a worker that a slot freed up).
7. For each dequeued item: calls `sub_828498C0` (NtWaitForSingleObjectEx INFINITE then CloseHandle) on the item's event at `[item+16]`.
8. Loops until all items in the channel's `[r28+6]` (hdr_count) are processed.
9. Cleans up: closes semaphore, frees memory.

---

## 6. sub_82852D18 Call Path (The Suspected Hang)

```
sub_82852D18(r3=streaming_ctx, r4=resource_request, r5=callback_vtable, r6=flags)
  │
  ├── sub_82852A50(r3=streaming_ctx, r4=resource_request)
  │     ├── sub_828470E0()                  [TLS-based reentrance guard]
  │     ├── sub_82852300(ctx, request, &slot_out, &type_out)
  │     │     ├── sub_8285AD08()            [read 4 bytes from resource, get type code]
  │     │     └── sub_828D0608() + loop     [iterate registered handlers until one accepts]
  │     │           └── vtable[+12] call    [handler's "CanHandle" method]
  │     ├── sub_82851DF0(ctx, &slot_out)    [hash-table lookup → find resource entry]
  │     ├── vtable[+12] indirect call       [handler's "Open" method → returns resource ptr]
  │     ├── vtable[+4] indirect call        [handler's "Read" method]
  │     ├── sub_8284FC98()                  [allocate 20-byte completion struct]
  │     ├── vtable[+0] indirect call        [handler's "Close/Finalize"]
  │     └── sub_82847120()                  [TLS reentrance guard release]
  │
  ├── sub_82851A10(r3=streaming_ctx, r4=resource_name)
  │     ├── checks for "__" prefix (strips it)
  │     └── sub_82912948()                  [hash-map find by name → returns registered handler]
  │
  ├── vtable[+8] indirect call              [handler's "Process" method]
  │
  ├── sub_828470E0() / sub_82847120()       [TLS reentrance guard]
  ├── sub_8284FA58(r3=completion_struct)    [free all sub-allocations + handle]
  └── sub_821B3560(r3=completion_struct)    [game free]
```

**Critical observation**: `sub_82852A50` does NOT wait on any semaphore or event. It is a **synchronous** path:
- Hash lookup (`sub_82851DF0`) to find a cached entry.
- If found: indirect vtable call to "Open" the resource (vtable[+12]).
- If Open returns non-null: vtable[+4] call ("Read/Bind"), then `sub_8284FC98` to allocate completion token.
- If hash miss: tries second vtable path at struct+32.
- Returns null (r3=0) on complete failure.

**sub_82852D18 does NOT block on a semaphore.** It calls `sub_82852A50` which is purely synchronous hash lookup + vtable dispatch.

---

## 7. sub_82852B78 / sub_82852DD0 Wrappers

Both `sub_82852B78` and `sub_82852DD0` (the two callers in gta4_recomp.55.cpp):

1. Call `sub_8284F468` to find a matching resource in the streaming file store (iterates up to `[store+3076]` entries, calls `sub_8285AA68` to match).
2. Call `sub_82852A50` or `sub_82852D18` to perform the actual resource bind.
3. Call **`sub_8285B088`** — the GPU buffer flush (already hooked/stubbed in imports.cpp).

The flow is: find resource → open/bind → GPU flush. The GPU flush was the original deadlock source (Xenos PM4 ring wait). The hook at `sub_8285A8B0` stubs the GPU path so `sub_8285B088` completes without blocking.

---

## 8. Critical Section Usage in the Dispatch Path

| Function | CS wrapper | What it guards |
|-|-|-|
| `sub_8285D2C8` (enqueue) | `sub_8285FE78` / `sub_8285FEE0` | Ring buffer write/count update |
| `sub_8285D808` (dequeue) | `sub_8285FE78` / `sub_8285FEE0` | Ring buffer read/count update |
| `sub_8285D610` (init) | `sub_8285FE48` | Initial CS creation at ring+1032 |

`sub_82852A50` and `sub_82851A10` do **NOT** use any of the `sub_8285FExx` critical section wrappers. They are lock-free lookups.

---

## 9. Deadlock Analysis: sub_82852D18 Hang

### Hypothesis: Waiting on streaming thread that hasn't completed

**Verdict: UNLIKELY for sub_82852D18 itself.**

`sub_82852D18` does not wait on any semaphore, event, or streaming thread completion. Its call to `sub_82852A50` is synchronous (hash lookup + vtable). The only blocking primitives in the vicinity are:

- `sub_828470E0` / `sub_82847120` — TLS-based reentrance counter, non-blocking (just increments/decrements).
- `sub_8284FA58` — frees memory, non-blocking.

### Where the real blocking could occur

1. **Inside a vtable indirect call** (`PPC_CALL_INDIRECT_FUNC`): If the handler's "Open" or "Read" method dispatches work to a streaming channel (via `sub_8285D2C8`) and then synchronously waits for completion, AND the worker thread is not running or is blocked, this would deadlock. The vtable dispatch at `sub_82852A50` line 30602 and 30618 are the key indirect calls.

2. **`sub_8284FC98`** allocates a 20-byte completion struct via `sub_821B3510` (game malloc) then calls `sub_8284D220` which sets up an event/callback. If the callback infrastructure depends on a streaming worker being alive, and the worker hasn't been created yet (the `sub_8285D9D8` init hasn't run), this could hang.

3. **`sub_82852DD0` (not sub_82852D18 directly)**: This is the more interesting caller. It calls `sub_8284F468` (iterate store entries with `sub_8285AA68` match), then `sub_82852D18`, then `sub_8285B088`. The `sub_8285B088` hook lets it through, but `sub_8284F468` iterates `[store+3076]` entries. If store is uninitialized or the count is corrupted, this could loop indefinitely.

### Most likely root cause

The hang is most likely in **the vtable indirect call chain** inside `sub_82852A50`. The handler's "Open" method (called via `PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32)` at 0x82852ACC) may:
- Dispatch an async I/O request to a streaming channel via `sub_8285D2C8`.
- Wait on the completion event (stored at offset +16 in the request struct).
- The streaming worker thread (created by `sub_8285D610` → `sub_8285D500`) receives the request and calls back with completion.

If the streaming worker thread is:
- **Not yet created** (channel init hasn't happened yet when this code path runs).
- **Blocked on something else** (e.g., waiting to enter a critical section held by the main thread).
- **Dead or crashed** (thread terminated unexpectedly).

Then the semaphore signal from `sub_8285D2C8` goes unheard, the completion event is never set, and the "Open" handler waits forever.

### Verification steps

1. Add a hook on `sub_8285D9D8` to log when streaming channels are initialized (and how many).
2. Add a hook on `sub_8285D500` (worker thread creation inside `sub_8285D610`) to log thread creation.
3. Check if `sub_82852D18` is called BEFORE `sub_8285D9D8` has initialized any channels — that would confirm the ordering bug.
4. Trace the vtable ptr at the indirect call site (ctx.ctr.u32 at 0x82852ACC) to identify which handler is involved.
