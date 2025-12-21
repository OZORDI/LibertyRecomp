# Runtime Behavior Reference

## Purpose
Combines state machines, worker threads, and async I/O (XXOVERLAPPED) analysis into a single reference so developers understand the runtime control flow, blocking conditions, and synchronization expectations.

### Contents
1. State machine flow charts
2. Worker thread lifecycle and semaphore usage
3. XXOVERLAPPED structure semantics and common issues
4. Suggested fixes and testing patterns

---

## 1. State Machines
- **Async Completion:** Polls `XXOVERLAPPED.Result`; waits on event handles via `NtWaitForSingleObjectEx`. Blocks when events never signal.
- **XAM Tasks:** `XamTaskSchedule` is a no-op and `XamTaskShouldExit` returns 1, so tasks exit immediately and initialization never finishes.
- **Init Completion:** `sub_82120000` → `sub_8218C600` → `sub_82673718`/`sub_829748D0` forms a deadlock cycle where workers and main thread wait on each other.
- **Worker Ready:** Render workers block on semaphores `0xA82487F0`/`0xA82487B0`; main thread never signals them because it is stuck in init.
- **Content/Device Workflow:** Depends on stubbed XAM APIs; wrong return values break transitions (e.g., `XamEnumerate` stays in `ENUM_PENDING`).
- **GPU Init:** `sub_829D87E8` → `sub_829DDC90` loops on `KeWaitForSingleObject`; host must force the fence to avoid infinite loops.
- **Currency:** Shader loading stub returns success without creating shaders, so frame presentation never reaches `DRAW_CALLS` and the render path stalls.

### Fix Priority
| Machine | Priority | Blocking Effect |
|---------|----------|-----------------|
| XAM Task | P0 | Init deadlock (main thread waits forever) |
| Worker Ready | P0 | No render work, semaphores remain zero |
| Init Completion | P0 | Circular dependency stalls boot |
| Shader Loading | P0 | Draw calls not issued |
| Async Completion | P1 | File I/O hangs |
| GPU Init | P1 | Fence loop needs bypass |
| Content/Device | P2 | Save/load UI stuck |
| Frame Presentation | P2 | Depends on shaders/boot |

---

## 2. Worker Thread System
### Thread Summary
- 9 workers created via `sub_8218C600` and `ExCreateThread`: 2 render, 3 streaming, 3 resource, 2 GPU (plus special thread #18 with 256KB stack).
- Render workers (`sub_827DE858`) wait on semaphores `0xA82487F0`/`0xA82487B0`; they never begin because the main thread never signals.
- Streaming/resource workers are generally running and handle the request queue (signals `0xA82402B0`, `0xEB2D...`).
- GPU threads (`sub_829DDC90`) loop on `KeWaitForSingleObject`; host forces success after repeated timeouts.
- Special thread (`sub_82169400`) waits on an event and polls a global flag, but the event never signals and the flag is never set.

### Semaphore Lifecycle
| Semaphore | Waiters | Purpose | Status |
|-----------|---------|---------|--------|
| 0xA82487F0 | Render Worker #1 | Work available | Never signaled (blocked) |
| 0xA82487B0 | Render Worker #2 | Work available | Never signaled (blocked) |
| 0xA82402B0 | Streaming workers | Queue thread | Active |
| 0xEB2D00B0 | Resource workers | Resource queue | Active |

### Critical Functions
- `sub_827DACD8`: waits on semaphore via `KeWaitForSingleObject`; logging identifies which semaphores are blocked.
- `sub_827DAD60`: signals semaphores (used by streaming/resource workers, not render).
- `sub_827DAC78`: attempted fix increments semaphore count but insufficient without main signal.

### Deadlock Cycle
```
Main thread (sub_8218C600) ───┬──► Creates workers
                             │
                             └──► Waits for XAM tasks/subsystems (blocks)
Render workers ───────┬────► Wait on semaphores (0xA82487F0/B0)
                      │
                      └────► Never run because semaphores stay at 0 (main thread blocked)
```

### Suggested Interventions
1. Implement real `XamTaskSchedule`/`XamTaskShouldExit` so init can finish.
2. Signal render worker semaphores once initialization completes (`KeReleaseSemaphore(0xA82487F0,1,FALSE)` etc.).
3. Optionally, allow main thread to execute render work if workers remain blocked temporarily.

---

## 3. XXOVERLAPPED Async I/O
### Structure Layout
```cpp
struct XXOVERLAPPED {
    uint32_t Result;         // +0x00 (0xFFFFFFFF while pending)
    uint32_t Length;         // +0x04
    uint32_t Context;        // +0x08
    uint32_t EventHandle;    // +0x0C
    uint32_t CompletionRoutine; // +0x10
    uint32_t ExtendedError;  // +0x14
};
```
- Big-endian layout; host must byte-swap when reading/writing.
- Event handle must be signaled on completion for `NtWaitForSingleObjectEx` to wake.

### Common Issues
1. **Event not signaled:** `NtWaitForSingleObjectEx` hangs forever; host must call `KeSetEvent(eventHandle, 0, FALSE)` when operation completes.
2. **Result left at 0xFFFFFFFF:** polling loops spin forever; update `Result` field (even on errors).
3. **Big-endian mismatch:** read/write must swap bytes via helper functions.
4. **APC never delivered:** use alertable waits (`alertable=TRUE`) or queue APCs when `CompletionRoutine` is non-null.

### Usage Patterns
- `sub_829AAD20` (file read) waits on the event handle after `STATUS_PENDING`.
- `XamEnumerate`/`XamContentCreate` use overlapped polling loops with `sleep(10)` between retries.

### Implementation Checklist
- [ ] Byte-swap guest fields before reading/writing.
- [ ] Update `Result`, `Length`, and `ExtendedError` during completion.
- [ ] Read `EventHandle`; signal via `KeSetEvent` when non-zero.
- [ ] Call APCs if `CompletionRoutine` is provided.
- [ ] Ensure alertable waits are used when queuing APCs.

### Testing Patterns
- **Sync read:** overlapped event = 0 → expect `NtReadFile` to complete synchronously with `Result == 0`.
- **Async read:** set `EventHandle`, expect `NtReadFile` → `STATUS_PENDING`, wait on event, result becomes 0.
- **Poll loop:** start overlapped with event = 0, loop until `Result` becomes non-0xFFFFFFFF.

---

## 4. Summary
- **Runtime control flow** lives across state machines (boot, tasks, worker readiness) that depend on XAM/kernel semantics.
- **Worker threads** coordinate via semaphores; render workers remain idle because the main thread never finishes init.
- **XXOVERLAPPED** is the core async primitive—without event signaling and byte-swapping, I/O loops hang.
- **Fix checklist:** implement host versions of XAM task APIs, signal worker semaphores, ensure overlapped completions write results/signals, and document these behaviors in one place.

---

## Document History
- 2025-12-20: Consolidated `state_machines_analysis.md`, `worker_threads_analysis.md`, and `xxoverlapped_analysis.md` into `RUNTIME_BEHAVIOR.md`.
