# PPC Analysis: sub_8285D610 — Streaming Channel Initializer

**Generated**: 2026-03-28
**Source file**: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.56.cpp` lines 7988–8267

---

## Function Signature (Reconstructed)

```cpp
// r3 = StreamingChannel* channel  (192-byte struct, array element)
// r4 = uint32_t flags             (bitmask, bits 0–6 select task types)
// r5 = uint32_t thread_name_idx   (index into name table at 0x82085E4C)
// r6 = uint32_t unk6
// r7 = uint32_t task_type_flags   (bitmask controlling which task IDs to register)
// r8 = uint32_t unk8
void sub_8285D610(StreamingChannel* channel, uint32_t flags, uint32_t thread_name_idx,
                  uint32_t unk6, uint32_t task_type_flags, uint32_t unk7, uint32_t unk8)
```

---

## Step-by-Step Breakdown

### Block 1 — Channel struct zero-init (lines 7990–8047)

Saves r3–r8 to callee-save registers (r22–r31). Stack frame is 224 bytes.

- `r31 = r3` (channel ptr)
- `r29 = r3 + 8` (start of channel data section)
- Clears `channel[0]` (u32), `channel[4]` (u16), `channel[6]` (u16) to 0.
- Calls `sub_8285FE48(r29 + 1024)` — initializes a **RtlCriticalSection** embedded at `channel+8+1024 = channel+1032`.
- Calls `sub_82849778(r3=0)` — calls `NtCreateSemaphore(r3, r4=0, r5=32767, r6=0)`. This creates a **semaphore** (max count 32767) for the channel's I/O completion queue, storing the handle at `channel+1068`.
- Stores the semaphore result at `channel+8+1068`.
- Clears `channel+8+1060`, `channel+8+1064`, `channel+8+1056` to 0.

### Block 2 — Task type bitmask expansion (lines 8048–8172)

Tests bits 0–6 of `r7` (task_type_flags). For each set bit N (0=bit31, 1=bit30, ..., 6=bit24), appends the constant `N+1` to a task-ID array on the stack at `r1+96`. `r30` tracks the count of entries added.

After the loop, `channel[6]` (u16 at +6) is updated with the total count `r30`, and if the previous value was 0, `sub_8285D488` is called to allocate the channel's task array.

### Block 3 — Thread spawning loop (lines 8201–8258)

Executes only if `r30 > 0` (at least one task type registered).

**r26 = 0x82085E4C** (thread name string table)

Iterates `r30` times. Each iteration:

1. Calls `sub_82158E08(r3, r4=16, r5=name_ptr, r6=thread_name_idx, r7=entry_ptr)` — **printf-style thread name formatter** (writes into a stack buffer).
2. Calls `sub_8285D500(r3=thread_slot, r4=channel, r5=name_buf, r6=task_type_id, r7=entry, r8=1, r9=unk8)`.

`sub_8285D500` is the **per-thread initializer**:
- Copies the formatted name string (up to 16 chars) into the thread slot's name field.
- Calls `sub_821B3510(size=1080)` — **allocates 1080 bytes** (the thread's context/state struct).
- Stores result at thread_slot+28.
- Stores `0` at thread_slot+20 (running flag).
- Stores `unk8` at thread_slot+188.
- Calls `sub_82849A50(r3=name_ptr, r4=thread_slot, r5=stack_size, r6=unk8, r7=thread_slot, r8=1, r9=task_type_alloc_ptr)`.

`sub_82849A50` is the **thread creation wrapper**:
- Clamps stack size to minimum 16384 bytes.
- Acquires a free thread slot from the pool at `0x83192C44`.
- Reads TLS allocator context from `r13+1676` and stores it at `thread_slot+8`.
- Calls `sub_82A13110` → `sub_82A1A4B8` → **`__imp__ExCreateThread`** with:
  - `handle_out = r1+80`
  - `stack_size = 16384`
  - `xapi_startup = name` (from `lis -32094, addi 5632 = ~0x82801600`)
  - `start_routine = thread_slot` (the thread proc is a generic dispatcher)
  - `context = thread_slot`
  - `flags = r7` (thread creation flags; bit 0 = CREATE_SUSPENDED)
- On success, calls `sub_82A11478` (set thread name), `sub_82A114F8` (set affinity, hardcoded =1), `sub_82A11580` (set priority).
- If the `resume` flag is set, calls `sub_82A13120` → `NtResumeThread`.

After `sub_8285D500` returns:
- Stores thread handle at `channel[0] + r28*192 + 16`.
- `r28` increments by 192, `r29` increments by 4 (next task-ID in stack array), `r30` decrements.

---

## Caller: sub_8285D948 (lines 8456–8541)

**Purpose**: Allocates a 1080-byte channel struct via `sub_821B3510`, then calls `sub_8285D610` with parameters.

Key: loads a global CPU affinity mask from `0x82B08210` and ANDs it with `r7` before passing to `sub_8285D610` as `task_type_flags`. This is how streaming channels are pinned to specific CPU cores on Xbox 360 (6 hardware threads).

Stores the returned channel pointer into the streaming manager's channel array at `0x831AB9B4 + 32 + index*4`.

---

## Synchronization Primitives Used

| Primitive | Location | Xbox 360 kernel call |
|-|-|-|
| CRITICAL_SECTION | `channel+1032` | `RtlInitializeCriticalSection` |
| SEMAPHORE | `channel+8+1068` | `NtCreateSemaphore(max=32767)` |
| THREAD | per thread slot | `ExCreateThread` |
| THREAD AFFINITY | per thread | `NtSetInformationThread` (via sub_82A11580) |

---

## Memory Regions Written

| Address | Description |
|-|-|
| `channel + 0` (u32) | 0 initially, then task array ptr |
| `channel + 4` (u16) | task type count |
| `channel + 6` (u16) | registered task count |
| `channel + 8` | 192-byte thread descriptor array (stride 192) |
| `channel + 1032` | Embedded CRITICAL_SECTION (1024 bytes into data section) |
| `channel + 1068` | Semaphore handle (NtCreateSemaphore result) |
| `0x831AB9B4+32+N*4` | Channel pointer stored by caller sub_8285D948 |
| `0x83192C40` | Thread pool count (incremented per thread) |
| `0x83192C44` | Thread pool head pointer (linked list) |

---

## Why It Hangs on PC/macOS

Sub_8285D610 itself does **not** hang — it completes successfully. The hang occurs in the caller chain or in what the spawned threads do after startup.

The spawned streaming threads call `KeWaitForSingleObject` on the semaphore at `channel+1068` waiting for I/O requests. On Xbox 360, the main thread drives the semaphore from the streaming manager's request queue. If the streaming manager never enqueues work (because it is itself blocked waiting for the threads to signal readiness), a deadlock forms.

**The actual hang path:**

1. `sub_8285D948` spawns N streaming threads.
2. Each thread enters its dispatch loop and calls `KeWaitForSingleObject(semaphore, INFINITE)`.
3. The caller of `sub_8285D948` (likely `sub_8285D9D8`) checks a ready flag written only by the first thread after it starts processing.
4. The ready flag is only written once the thread receives its first semaphore signal.
5. Nobody signals the semaphore because the main init thread is blocked waiting on the ready flag.

This is a classic **two-party startup handshake** that works on Xbox 360 because:
- On Xbox 360, CPU affinity causes the thread to run on a different hardware core immediately.
- On PC, cooperative scheduling or single-core contention can block the spawned thread from running at all until the spawning thread yields or blocks.

The condition that makes this deadlock-resistant on Xbox: `sub_82849A50` passes `flags` bit that causes `ExCreateThread` to start the thread suspended (`CREATE_SUSPENDED` is bit 0 of `r9`), then resumes it only after setup. On Xbox 360 with 3 physical cores, the new thread can run immediately. On macOS with the recomp's cooperative scheduling model, the spawned PPC thread context may not get scheduled until the spawning context yields.

---

## What Happens If Stubbed vs Fixed

### Stubbed (return immediately from sub_8285D610)

- No streaming threads are spawned.
- No semaphores are created.
- The streaming manager has zero channels.
- Game will crash or hang when the first streaming request is queued — `channel[0]` is null, all dispatch calls dereference null.
- **Do not stub.**

### Stubbed (return immediately from sub_8285D948)

- Same outcome: no channel is registered in the streaming manager array.
- `sub_8285D9D8` (the outer setup function) would loop indefinitely waiting on the ready flag from the channel.
- **Do not stub.**

### Proper Fix

The streaming thread handshake hang is likely triggered by `sub_8285D9D8` — the function that calls `sub_8285D948` and then waits. The wait loop in `sub_8285D9D8` checks `*(0x831AB9B4+32) != 0` (the channel count at the streaming manager). If that is never written because the spawned thread never processes its first request, the hang lives there, not in sub_8285D610 itself.

**Recommended fix**: Hook `sub_8285D9D8` or its wait loop. Alternatively, ensure that after `sub_8285D948` returns, a semaphore signal is posted to each new channel to bootstrap the streaming pipeline.

If the hang is confirmed to be in the spawned thread's wait for `ExCreateThread` to actually start the thread (i.e., `sub_82849A50` blocks), then the fix is in `xboxkrnl_threading.cpp`'s `ExCreateThread` implementation — the thread must be started and allowed to run before the creating function returns the handle.

---

## File Locations

| File | Lines | Content |
|-|-|-|
| `gta4_recomp.56.cpp` | 7988–8267 | `sub_8285D610` (channel initializer + thread spawner) |
| `gta4_recomp.56.cpp` | 7833–7986 | `sub_8285D500` (per-thread slot initializer) |
| `gta4_recomp.56.cpp` | 8456–8541 | `sub_8285D948` (caller: alloc + configure one channel) |
| `gta4_recomp.56.cpp` | 14007–14039 | `sub_8285FE48` (RtlInitializeCriticalSection wrapper) |
| `gta4_recomp.56.cpp` | 14041–14054 | `sub_8285FE78` (RtlEnterCriticalSection wrapper) |
| `gta4_recomp.56.cpp` | 14113–14126 | `sub_8285FEE0` (RtlLeaveCriticalSection wrapper) |
| `gta4_recomp.55.cpp` | 8274–8289 | `sub_82849778` (NtCreateSemaphore wrapper) |
| `gta4_recomp.55.cpp` | 8749–8916 | `sub_82849A50` (thread pool alloc + ExCreateThread) |
| `gta4_recomp.69.cpp` | 50677–50745 | `sub_82A1A4B8` (ExCreateThread thin wrapper) |
| `gta4_recomp.69.cpp` | 37298–37360 | `sub_82A12EB8` (NtCreateSemaphore implementation) |
| `gta4_recomp.0.cpp` | 59745–59787 | `sub_82158E08` (thread name formatter) |
| `src/kernel/xboxkrnl/xboxkrnl_threading.cpp` | — | `ExCreateThread`, `NtResumeThread`, `NtSetEvent` implementations |

All `gta4_recomp.*` paths relative to `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/`.

---

## Key Global Addresses

| Address | Description |
|-|-|
| `0x831AB9B4` | Streaming manager singleton |
| `0x831AB9B4 + 32` | Start of channel pointer array (4 bytes each) |
| `0x83192980` | Thread pool allocator base |
| `0x83192C40` | Thread pool count |
| `0x83192C44` | Thread pool head pointer (linked list) |
| `0x83192C4C` | Thread pool list base |
| `0x82085E4C` | Thread name string table |
| `0x82B08210` | CPU affinity mask global |
