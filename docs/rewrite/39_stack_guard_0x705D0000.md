# Stack Guard Page Infinite Loop at Guest 0x705D0000

## Problem

After the scene creation state machine completes, thread `t41614048` enters an
infinite loop producing "Stack guard page hit at guest 0x705D0000" at ~25,000
faults/second (1.5 million in 60 seconds).

## What Is 0x705D0000?

Xbox 360 guest thread stacks are allocated in the range `0x70000000`-`0x7F000000`
(`kStackAddressRangeBegin`/`kStackAddressRangeEnd` in `xthread.h:150-151`).

Address `0x705D0000` is **5.8 MB** into the stack region. Stacks allocate
bottom-up, so this is a thread created mid-game (roughly thread #11-31 depending
on stack sizes). The thread `t41614048` is a macOS native thread ID, not the
guest Xbox 360 thread ID.

### Memory Map Context

The heap managing `0x705D0000` is `heaps_.v40000000`:
- Range: `0x40000000`-`0x7F000000`
- Type: `kGuestVirtual`
- Page size: **64 KB** (`0x10000`)

(Defined at `xmemory.cpp:166`)

### Stack Layout Per Thread

From `XThread::AllocateStack` (`xthread.cpp:224-253`):

```
padding = page_size * 2 = 0x20000 (128 KB)
actual_size = requested_stack_size + padding

Layout (addresses increase upward):
  [stack_alloc_base_]              Bottom guard (1 page = 64 KB, PROT_NONE)
  [stack_alloc_base_ + 0x10000]   stack_limit_ (usable stack bottom)
  ...                              Stack grows DOWNWARD into this region
  [stack_base_ - 1]               Top of usable stack
  [stack_base_]                    Top guard (1 page = 64 KB, PROT_NONE)
```

Both guard pages are **committed** (AllocRange with `kMemoryAllocationCommit`)
then set to `kMemoryProtectNoAccess` via `Protect()`.

## Root Cause: Three Bugs in AccessViolationCallback

**File**: `glue/rexglue-sdk-main/src/system/xmemory.cpp:436-461`

The stack guard expansion handler added to rexglue (not present in upstream
Xenia) has three bugs that combine to cause the infinite loop.

### Bug 1: Protect() Return Value Ignored

```cpp
// xmemory.cpp:457-460
heap->Protect(page_addr, page_size,
              memory::kMemoryProtectRead | memory::kMemoryProtectWrite);
REXSYS_WARN("Stack guard page hit at guest 0x{:08X} -- expanded stack", virtual_address);
return true;  // <-- ALWAYS returns true, even if Protect() failed
```

If `Protect()` fails (returns false), the page remains `PROT_NONE`. The handler
returns `true` telling the signal handler the fault was handled. The CPU retries
the faulting instruction, hits the same `PROT_NONE` page, and the cycle repeats
forever.

`Protect()` can fail for multiple reasons (`xmemory.cpp:1212-1291`):
- **Uncommitted page** (line 1256): page state lacks `kMemoryAllocationCommit`
- **Region spanning** (line 1253): page is in a different allocation region
- **Host mprotect failure** (line 1272): OS-level protection change failed

### Bug 2: No Distinction Between Guard Pages and Unallocated Memory

```cpp
// xmemory.cpp:451-453
if (heap->heap_type() == memory::HeapType::kGuestVirtual &&
    virtual_address >= system::XThread::kStackAddressRangeBegin &&
    virtual_address < system::XThread::kStackAddressRangeEnd) {
```

This treats **any** fault in `[0x70000000, 0x7F000000)` as a guard page hit.
This includes:
- Free/unallocated memory between thread stacks
- Memory below a thread's `stack_alloc_base_` (never allocated)
- Memory from a **freed** thread stack (`Release()` sets state to 0)

All of these will cause `Protect()` to fail on the uncommitted page check, and
with Bug 1, this creates an infinite loop.

### Bug 3: No New Guard Page After Expansion

On real Windows/Xbox 360 kernels, when a stack guard page is committed:
1. The faulting guard page is made accessible
2. A **new** guard page is placed one page below
3. If no room for a new guard, `STATUS_STACK_OVERFLOW` is raised

The rexglue handler does step 1 but skips steps 2-3. After a successful
expansion, the stack can grow further without any guard to catch the next
overflow, potentially corrupting adjacent memory.

## The Infinite Loop Mechanism

```
Signal chain: SIGBUS -> ExceptionHandlerCallback -> MMIOHandler::ExceptionCallback
  -> Memory::AccessViolationCallback -> heap->Protect() -> [fail] -> return true
  -> signal handler restores context & retries faulting instruction -> SIGBUS -> ...
```

Detailed flow:

1. Thread writes to `0x705D0000` (PROT_NONE page in stack range)
2. macOS delivers **SIGBUS** (PROT_NONE write on shared memory mapping)
3. `exception_handler_posix.cpp:144-160`: Hardcoded as `kWrite` on macOS ARM64
4. `mmio_handler.cpp:364-416`: No MMIO range matches; calls `access_violation_callback_`
5. `xmemory.cpp:451-460`: Address in stack range; calls `Protect()`
6. `xmemory.cpp:1256`: `Protect()` fails ("uncommitted page") -- page state is 0
7. `xmemory.cpp:460`: Returns `true` regardless
8. `mmio_handler.cpp:413`: Returns `true` to signal handler
9. `exception_handler_posix.cpp:225-260`: Restores thread context, returns from signal
10. CPU re-executes the same store instruction -> goto step 1

Each iteration takes ~40 microseconds (25K/second), consuming 100% of one CPU
core and flooding stderr with warning messages.

## Why This Triggers After Scene Creation

The scene creation state machine creates numerous game threads for world
simulation, streaming, audio, physics, etc. Some of these threads may:

- Have deep call stacks that overflow the allocated stack size
- Be accessing stack memory from a terminated/recycled thread
- Have their stacks freed while still executing teardown code

The specific address `0x705D0000` is likely the bottom guard page (or below the
allocation) of a thread created during scene initialization.

## Fix Requirements

The handler at `xmemory.cpp:448-461` needs:

1. **Check Protect() return value**: If `Protect()` fails, return `false` to
   indicate the fault was NOT handled. This will cause the fallback crash handler
   to fire (or the process to terminate), which is correct for an actual stack
   overflow or invalid memory access.

2. **Validate page state before Protect()**: Query the page table to confirm the
   faulting page is actually committed with `kMemoryProtectNoAccess` before
   attempting to expand it. If the page is unallocated, return `false`.

3. **Place a new guard page**: After expanding, set the next page below to
   `kMemoryProtectNoAccess` (if it exists within the same allocation). If there
   is no room for a new guard page, this is a genuine stack overflow and should
   be treated as a fatal error (return `false`).

4. **Rate limiting** (defense in depth): Track the last N guard page expansions
   per thread. If the same thread hits the guard repeatedly (e.g., >2 times
   without the guard moving), it is likely infinite recursion. Return `false`.

## Key Files

| File | Lines | Role |
|------|-------|------|
| `glue/rexglue-sdk-main/src/system/xmemory.cpp` | 436-461 | The buggy guard page handler |
| `glue/rexglue-sdk-main/src/system/xmemory.cpp` | 1212-1291 | `BaseHeap::Protect` (fails on uncommitted) |
| `glue/rexglue-sdk-main/src/system/xthread.cpp` | 224-253 | `AllocateStack` (guard page setup) |
| `glue/rexglue-sdk-main/src/system/mmio_handler.cpp` | 364-416 | `ExceptionCallback` (dispatches to AV callback) |
| `glue/rexglue-sdk-main/src/core/exception_handler_posix.cpp` | 144-260 | SIGBUS handler (macOS ARM64 path) |
| `glue/rexglue-sdk-main/include/rex/system/xthread.h` | 150-151 | `kStackAddressRange{Begin,End}` constants |
| `LibertyRecomp/main.cpp` | 183-198 | Exception handler installation |
| `tools/xenia-master-1/src/xenia/memory.cc` | 440-466 | Xenia reference (no guard handling) |
