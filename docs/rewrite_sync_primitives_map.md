# Sync Primitives Map — Audio/Streaming Init Chain

Generated 2026-03-28. All arithmetic Python-verified.

---

## 1. NtWaitForSingleObjectEx (sub_82A13040) — 0x8284–0x8286 range

**RexGlue entry:** `NtWaitForSingleObjectEx_entry(handle, wait_mode, alertable, timeout_ptr)`
Delegates to `object->Wait(3, wait_mode, alertable, timeout_ptr)` → `rex::thread::Wait(WaitHandle*, alertable, timeout_ms)` → `PosixConditionBase::Wait()` which calls `std::condition_variable::wait()` / `wait_for()`.
**Yes, it really blocks** — backed by a real pthread `condition_variable` with `std::mutex`. No busy-spin.

Five callers in 0x82840000–0x82860000:

| Wrapper | r4 (wait_mode) | timeout | Notes |
|-|-|-|-|
| sub_82849790 | 0 (KernelMode) | nullptr → INFINITE | Returns bool: 1=success |
| sub_828497D8 | -1 (0xFFFFFFFF) | nullptr → INFINITE | Returns bool: 1=success |
| sub_82849820 | not set | nullptr → INFINITE | Returns bool: 1=success |
| sub_828498C0 | -1 | nullptr → INFINITE | Waits then calls `CloseHandle(r3)` |
| sub_8285D110 | 0 | nullptr → INFINITE | Waits on `[r31+156]` (event in struct at offset 156) |

All four in 0x82849xxx are thin one-shot wrappers: check handle != 0, then call NtWaitForSingleObjectEx. Only sub_828498C0 is a wait-and-close pattern (waits INFINITE, then releases the handle).

---

## 2. NtCreateEvent / NtSetEvent

| Address | Function | Args | Notes |
|-|-|-|-|
| sub_82A10F68 | NtCreateEvent | r4=0 (manual-reset), r5=1 (init signaled), r6=0 | Creates notification event; handle out to stack+124 |
| sub_82A13160 | NtSetEvent (signal) | r3=handle, r4=0 | Thin wrapper; returns 1 on success |

sub_82A10F68 is the event **creator** (used by sub_82849790/sub_82849820 callers indirectly). It allocates an auto-wait-close handle.

---

## 3. NtCreateSemaphore / NtReleaseSemaphore

| Address | Function | Args | Notes |
|-|-|-|-|
| sub_82A12EB8 | NtCreateSemaphore | r5=initial_count, r6=max_count | Creates semaphore; handle out to stack+80 |
| sub_82A12F50 | NtReleaseSemaphore | r3=handle | Thin wrapper; calls NtReleaseSemaphore; returns 1 on success |

---

## 4. KeInitializeSemaphore / KeWaitForSingleObject / KeReleaseSemaphore (Audio subsystem)

These operate on **static global semaphores** in the audio/streaming module at 0x831Dxxxx:

| Address | Value | Created by | Waited by | Signaled by |
|-|-|-|-|-|
| 0x831D52CC | KSEMAPHORE | sub_82190B48 (KeInitializeSemaphore, initial=0, max=6) | sub_821909D0 (KeWaitForSingleObject, r4=3=WaitReason, r5=1=kernel, r6=0=non-alertable, timeout=INFINITE) | sub_821909D0 (KeReleaseSemaphore, count=1) |
| 0x831D52DC | KSEMAPHORE | sub_82190B48 | sub_821909D0 (KeWaitForSingleObject) | sub_821909D0 (KeReleaseSemaphore) |
| 0x831D52F0 | KEVENT | sub_82190B48 (inline KeInitializeEvent) | sub_821909D0, sub_82190A98 (KeWaitForSingleObject, INFINITE) | sub_821904B8 (KeSetEvent, increment=1, wait=0) |
| 0x831D5300 | KSEMAPHORE | sub_82190B48 (KeInitializeSemaphore, initial=1, max=1) | — | — |
| 0x831D5310 | KSEMAPHORE | sub_82190B48 | — | — |

`sub_82190B48` is the **audio/streaming manager init** function (called during game init at gta4_init.cpp:~1399). It initializes all static semaphores/events for the streaming thread pool.

`sub_821909D0` is the **streaming worker thread** loop: it calls KeWaitForSingleObject on 0x831D52F0 (INFINITE), then KeReleaseSemaphore when work is complete.

`sub_821904B8` is the **streaming job dispatcher**: calls KeSetEvent(0x831D52F0, 1, 0) to wake waiting workers.

`sub_82190A98` also waits on 0x831D52F0 (two separate KeWaitForSingleObject calls, both r4=3, r5=1, timeout=INFINITE).

---

## 5. KeResetEvent / KeSetEvent — Audio Decoder (0x82A4Fxxx)

| Address | Function | Event | Pattern |
|-|-|-|-|
| sub_82A4F0E0 | KeWaitForSingleObject then KeResetEvent | struct+32 | Wait for decode complete, then reset event. Timeout=INFINITE. |
| sub_82A4F1D0 | KeSetEvent | struct+32 | Signal decode complete (increment=1, wait=0) |

These are per-object events embedded at offset +32 inside audio decoder state structs. Creator is not in the 0x8284–0x8286 range (initialized elsewhere in the audio decoder allocation path).

---

## 6. RtlInitializeCriticalSection / RtlEnterCriticalSection / RtlLeaveCriticalSection

**Implementation** (`xboxkrnl_rtl.cpp`):
- `RtlInitializeCriticalSection`: sets `lock_count=-1`, `recursion_count=0`, `owning_thread=0`
- `RtlEnterCriticalSection`: spin-wait loop (spin_count = `header.absolute * 256`), then `atomic_inc(lock_count)`. If contended, calls `xeKeWaitForSingleObject(cs_ptr, 8, 0, 0, nullptr)` — INFINITE wait, non-alertable
- `RtlLeaveCriticalSection`: `owning_thread=0`, `atomic_dec(lock_count)`. If waiters, calls `xeKeSetEvent(cs_ptr, 1, 0)` to wake one

**In 0x8284–0x8286 range** (file 56), these wrappers guard a **reference-counted lock** pattern:

| Wrapper | Role | Notes |
|-|-|-|
| sub_8285FE48 | InitCS | Stores CS ptr in r31, calls RtlInitializeCriticalSection(r31) |
| sub_8285FE78 | CondEnter | `if ([r3+0] != 0)` → RtlEnterCriticalSection([r3]) |
| sub_8285FEE0 | CondLeave | `if ([r3+0] != 0)` → RtlLeaveCriticalSection([r3]) |
| sub_8285FEF8 | RefcountEnter | `[r3+0]++; if count==1` → RtlEnterCriticalSection([r3+4]) |
| sub_8285FF28 | RefcountLeave | `[r3+0]--; if count==0` → RtlLeaveCriticalSection([r3+4]) |
| sub_8285FF50 | InitReflock | Sets refcount=1, stores CS ptr, conditionally enters CS |
| sub_8285FFA0 | ReleaseReflock | Decrements refcount, if 0 → RtlLeaveCriticalSection |

These are heavily used by the streaming system (gta4_recomp.14.cpp for file streaming, gta4_recomp.62–63.cpp for audio streaming) to guard buffer/queue access.

---

## 7. Thread Scheduler: pthreads, not coroutines

`XThread` creates real OS threads via `rex::thread::Thread::Create()` which uses `std::thread` (backed by pthreads on macOS/Linux/PS4/Switch). Each guest PPC thread gets a real OS thread. See `src/core/threading_posix.cpp`.

`rex::thread::Wait()` → `PosixConditionBase::Wait()` → `std::condition_variable::wait()` — this is a **true blocking wait** (pthread_cond_wait internally). The calling pthread blocks; the OS scheduler can run other pthreads.

There are **no coroutines** or fibers. Thread switching is fully cooperative via the OS scheduler.

---

## 8. Potential Issues

1. **sub_82849820**: does not set r4 before calling NtWaitForSingleObjectEx — wait_mode is whatever was in r4 from caller. Benign if callers always pass a valid handle in r3 and the wait_mode default is 0.

2. **sub_82190A98** calls KeWaitForSingleObject **twice** in a row (lines 1053 and 1080) on the same event (0x831D52F0). If the event is only signaled once, the second wait will hang INFINITE. This is a potential deadlock if the dispatcher (sub_821904B8) only calls KeSetEvent once per job.

3. **RtlEnterCriticalSection spin-count**: GTA IV sets `header.absolute = 0` → spin_count = 0. Every contended lock goes immediately to `xeKeWaitForSingleObject`. Combined with high contention in the streaming file-read path (gta4_recomp.14.cpp calls sub_8285FF50/FF28 ~20 times per 500 lines), this may cause pthread oversubscription.

4. **NtWaitForSingleObjectEx timeout=-1 as wait_mode**: sub_828497D8 and sub_828498C0 pass r4=-1 (0xFFFFFFFF). In `NtWaitForSingleObjectEx_entry`, this becomes `wait_mode=0xFFFFFFFF`. This is outside the normal range (0=KernelMode, 1=UserMode) but the implementation ignores wait_mode and passes it through — no crash, but logically wrong.
