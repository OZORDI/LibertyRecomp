# RexGlue PPC Thread Scheduler — macOS Analysis

**Date:** 2026-03-28
**Purpose:** Understand why init deadlocks by mapping the actual threading model.

---

## Architecture: Real Pthreads, Not Fibers

PPC threads are **real pthreads** — each `XThread` wraps a `rex::thread::Thread`, which is backed
by `pthread_create` in `threading_posix.cpp` (`PosixCondition<Thread>::Initialize`). There are no
fibers, coroutines, or cooperative scheduling anywhere in the stack.

---

## Answers to Key Questions

### a. Are PPC threads real pthreads or cooperative fibers?

**Real pthreads.** `XThread::Create()` calls `rex::thread::Thread::Create(params, lambda)` which
calls `pthread_create`. Each PPC thread runs in a real OS thread.

### b. When ExCreateThread is called, does a new pthread start immediately?

**Yes, with a mandatory 10ms delay.** The thread is created via `pthread_create` and immediately
resumed (unless `X_CREATE_SUSPENDED`). However, `XThread::Execute()` opens with
`rex::thread::Sleep(std::chrono::milliseconds(10))` — an intentional 10ms stall before the guest
function runs. This is by design to handle buggy games that race on shared structures.

### c. When thread A calls KeWaitForSingleObject, does it block the pthread or yield to a fiber?

**Blocks the real pthread.** The call chain is:
```
KeWaitForSingleObject → xeKeWaitForSingleObject → XObject::Wait
  → rex::thread::Wait → PosixConditionBase::Wait
    → cond_.wait(lock, predicate)  // std::condition_variable::wait
```
This is a real `std::condition_variable::wait` / `cond_.wait_for`. The calling pthread is blocked in
the kernel. It does NOT yield to another fiber — it sleeps on a condvar until the object is
signaled.

### d. Can multiple PPC threads run truly concurrently on different CPU cores?

**Yes.** Since every PPC thread is a real pthread with no global interpreter lock, macOS will
schedule them across available cores. Thread priorities and affinities are both **ignored by
default** (`ignore_thread_priorities = true`, `ignore_thread_affinities = true` cvars).

Affinity setting on macOS is explicitly a no-op: `set_affinity_mask` silently ignores the mask
because macOS does not expose per-thread CPU affinity via pthreads. CPU assignment
(`GetFakeCpuNumber`) is a bookkeeping round-robin that only updates KPCR — it does not actually
pin threads to cores.

### e. What happens when the main init thread yields via KeDelayExecutionThread?

**The pthread actually sleeps.** Call chain:
```
KeDelayExecutionThread → XThread::Delay
  → rex::thread::Sleep(milliseconds) or AlertableSleep
    → nanosleep(...)   // real OS sleep
```
`XThread::Delay` converts the 100ns-tick interval to milliseconds (divides by 10000), then calls
`rex::thread::Sleep` (nanosleep) or `AlertableSleep`. The pthread is descheduled by the OS. Other
PPC threads are free to run on other cores.

`NtYieldExecution` calls `thread->Delay(0, 0, 0)`, which computes `timeout_ms = 0` — this is a
zero-duration sleep, not `sched_yield`. The `MaybeYield()` helper (used on timeout paths) calls
`sched_yield() + __sync_synchronize()`.

### f. Any thread affinity or single-threaded restrictions?

- **No global interpreter lock.** Multiple PPC threads run truly concurrently.
- **`global_critical_region_` mutex** is used only for APC delivery (`LockApc`/`UnlockApc`) and
  thread suspension — not for normal execution.
- **`WaitMultiple` uses a separate `global_mutex_`** to check multiple objects atomically, but only
  during multi-object waits.
- **macOS thread affinity is silently ignored** — all threads run on any available core.

---

## Deadlock Implication

Because all waits are real condvar blocks on real pthreads:

1. If thread A is waiting on object X (pthread blocked), and
2. Thread B is supposed to signal X, and
3. Thread B is itself blocked on something thread A must first do,
4. Then classic deadlock — neither pthread can proceed.

The init deadlock is **not** a scheduler starvation problem (cooperative scheduler starving a
thread). It is a genuine circular pthread wait. The 10ms mandatory sleep in `XThread::Execute` means
the spawning thread has 10ms to initialize any shared structures before the new thread's guest code
runs — but if the spawning thread is already blocked on a condvar, that window is irrelevant.

---

## Key Source Locations

| File | Role |
|-|-|
| `src/system/xthread.cpp:373` | `pthread_create` via `Thread::Create` |
| `src/system/xthread.cpp:496` | Mandatory 10ms sleep before guest execution |
| `src/system/xthread.cpp:873` | `XThread::Delay` → nanosleep |
| `src/core/threading_posix.cpp:579` | Actual `pthread_create` call |
| `src/core/threading_posix.cpp:240` | `PosixConditionBase::Wait` → `cond_.wait` |
| `src/core/threading_posix.cpp:960` | `rex::thread::Wait` wrapper |
| `src/system/xobject.cpp:203` | `XObject::Wait` → `rex::thread::Wait` |
| `src/kernel/xboxkrnl/xboxkrnl_threading.cpp:328` | `KeDelayExecutionThread` entry |
| `src/kernel/xboxkrnl/xboxkrnl_threading.cpp:758` | `xeKeWaitForSingleObject` entry |
