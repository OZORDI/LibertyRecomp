# Semaphore Seeding in sub_8284CFD8 (Streaming Ring-Buffer Worker Pool)

**Hook location**: `LibertyRecomp/kernel/imports.cpp:662`
**Generated code**: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.55.cpp:16725`

---

## What the Original Function Does

sub_8284CFD8 initializes a streaming ring-buffer worker pool. It:

1. Takes r3 = worker count, r4/r5 = thread params
2. Allocates a free-list of `r3 * 28`-byte entries via sub_821B3510
3. Initializes 2 critical section objects via sub_82871980 (loop at `loc_8284D0B0`)
4. Creates 2 worker threads via sub_82849A50, each receiving:
   - A ring-buffer struct base address (from global at `0x83192980`)
   - Thread entry point at `0x82AB5668` (sub_82AB5668, the worker drain loop)
   - 64KB stack size, priority flag = 1

The worker structs live at BSS address **unk_8319F2F8**, with stride **24944** bytes (2 workers total).

---

## Semaphore Seeding (The Hook)

Each worker struct has a semaphore handle slot at **offset +24940** (4 bytes before end of struct). On Xbox 360, the game's init path writes a valid NtCreateSemaphore handle there. In the recomp, BSS is zeroed, so the slot contains 0.

**Problem**: Workers call NtWaitForSingleObjectEx on this semaphore to sleep between ring-buffer drain passes. With handle=0, the wait returns STATUS_INVALID_HANDLE immediately. Workers busy-spin instead of sleeping, and the main thread's completion event is never released.

**Fix**: The hook runs the original `__imp__sub_8284CFD8`, then post-seeds each slot:

| Worker | Struct Address | Semaphore Slot | Handle |
|-|-|-|-|
| 0 | `0x8319F2F8` | `0x831A5464` (`+24940`) | XSemaphore(0, 0x7FFF) |
| 1 | `0x831A5468` | `0x831AB5D4` (`+24940`) | XSemaphore(0, 0x7FFF) |

Handles are kernel objects (0xF8xxxxxx range) created via `rex::system::XSemaphore` C++ API. The hook checks for existing valid handles and skips if already seeded.

---

## Relationship to sub_8285D018 Wait Chain

The hook comment mentions a deadlock path:

```
sub_8285D018 (GPU ring buffer submit + fence wait)
  -> sub_8285CF98 -> sub_8285CEA8 -> sub_8285C648 -> sub_828497D8
    -> sub_82A13040 (NtWaitForSingleObjectEx) — blocks forever
```

This chain is **independently stubbed** (imports.cpp:1236). sub_8285D018 returns 0 (fence already complete), sub_8285C648 returns 1 (signaled). The semaphore seeding prevents a **different** deadlock: the ring-buffer workers spinning without yielding, starving other threads that need CPU time.

---

## Relationship to the Yield Loop at 0x825FBCAC

The yield loop is in **sub_825FBB68** (gta4_recomp.35.cpp:26152). Its loop structure:

1. `XNotifyGetNext(handle, 11, &buf_a, &buf_b)` — poll for system notifications
2. `sub_82849790(event_at_0x83192BBC)` — non-blocking wait on an event handle
3. If not signaled: `sub_82849918(16)` — yield/sleep 16ms, goto step 1

sub_82849790 calls `sub_82A13040` (NtWaitForSingleObjectEx with timeout=0) on the event handle at `[r26 + 29568]` where r26 = `0x8318B838`, so the event address is `0x83192BB8`.

**This event is NOT in the worker struct array.** The worker structs span `0x8319F2F8..0x831AB5D8` (two workers of 24944 bytes). The yield loop's event at `0x83192BB8` is ~50KB below the worker array.

**Conclusion**: The yield loop at 0x825FBCAC is **not directly blocked** by the ring-buffer worker semaphores. It waits on a separate event (`0x83192BB8`) that is likely a content/resource readiness event, unrelated to the ring-buffer pool. The loop naturally yields via sub_82849918 every iteration, so it cannot cause CPU starvation.

---

## Can Streaming Workers Hold a Lock That Blocks the Allocator?

The streaming workers (sub_82849A50 threads) operate on the ring-buffer struct at `0x83192980`. They:

- Acquire a critical section at `0x83192980 + 716` (initialized in sub_82849A50)
- Dequeue work items from the ring buffer
- Call sub_82A13040 to wait on per-item completion events
- Release the critical section

The **allocator** (sub_8218BE28) reads TLS slot `[r13+1676]` for its context. Workers run on their own threads with their own TLS. There is no shared lock between the ring-buffer critical section and the allocator's TLS-based dispatch.

**However**, if workers busy-spin (before the semaphore fix), they consume 100% CPU on their cores, causing thread starvation. On macOS with GCD-backed threading, this can delay the main thread's allocator TLS setup, making `sub_825EDDA0`'s particle emitter allocations all fall through to the host page allocator (the "ALLOC FALLBACK storm" noted in MEMORY.md). The semaphore seeding fix ensures workers sleep properly between drain passes.

---

## Per-Frame Signaling Gap

The seeded semaphores at `0x831A5464` and `0x831AB5D4` are **not signaled** by the per-frame `SignalSemaphoreByGuestAddr` calls in imports.cpp:207-214. The per-frame signals cover:

- `0x83137B80` — Audio worker event
- `0x83130008` — Audio work queue semaphore
- `0x83131E10` — Streaming I/O event
- `0x83131B34` — XamTask completion
- `0x82A9800C/82AA0010/82AA0014/82AA0018` — File I/O events
- `0x82000768` — GPU sync

If the workers need periodic wakeups to drain the ring buffer, their semaphores should be added to the per-frame signal list. Currently, workers would block on the semaphore indefinitely unless the producer (ring-buffer enqueue path) explicitly releases it. In sync mode (`0x830F589C=1`), this is moot because pgStreamer processes work inline, but if async streaming is ever enabled, the missing per-frame signal would be a problem.
