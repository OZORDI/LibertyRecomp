# NtAllocateVirtualMemory Hang Analysis

## Summary

operator new (sub_821B3510) hangs on a 1024-byte allocation because the entire
virtual memory subsystem is serialized behind a **single global recursive mutex**
that is shared by every heap, every kernel module load, every thread
suspend/resume, and the kernel dispatch thread.

## The Lock

`rex::thread::global_critical_region` wraps a process-wide
`static std::recursive_mutex` (singleton in `src/core/mutex.cpp:17`).
Every `BaseHeap` and the top-level `Memory` class each hold an instance, but
they all resolve to the **same mutex**.

## Call Chain (when rexcrt_heap_enable = false)

The rexcrt o1heap is disabled by default (`rexcrt_heap_enable` defaults false).
When disabled, the function table maps 0x82A17180 to `rexcrt_RtlAllocateHeap`,
but the **recompiled original Xbox CRT code** at that address calls
`__imp__NtAllocateVirtualMemory` to grow the heap.

```
sub_821B3510 (operator new, 1024 bytes)
  -> sub_8218BE28 (game malloc, TLS allocator or fallback)
    -> recompiled RtlAllocateHeap @ 0x82A17180
      -> __imp__NtAllocateVirtualMemory (xboxkrnl_memory.cpp:63)
        -> kernel_memory()->LookupHeap()->Alloc() / AllocFixed()
          -> BaseHeap::AllocRange() (line 1023)
            -> global_critical_region_.Acquire()   *** BLOCKS HERE ***
```

## Who Else Holds the Lock

| Holder | File | Line | Duration |
|-|-|-|-|
| BaseHeap::AllocFixed | xmemory.cpp | 943 | page table walk + host mmap |
| BaseHeap::AllocRange | xmemory.cpp | 1023 | free page scan (O(n) pages) |
| PhysicalHeap::Alloc/Decommit/many | xmemory.cpp | 1150-1598 | various |
| Memory::AccessViolationCallback | xmemory.cpp | 437 | triggers invalidation callbacks |
| PhysicalHeap::TriggerCallbacks | xmemory.cpp | 1647-1861 | scans page flags + fires callbacks |
| KernelState dispatch thread | kernel_state.cpp | 369 | holds lock while waiting on condvar |
| KernelState::LoadUserModule | kernel_state.cpp | 411 | module load + init |
| Processor::ExecuteInterrupt | processor.cpp | 171 | entire interrupt handler execution |
| XThread::Suspend | xthread.cpp | 857 | thread suspend |

## Root Cause

During init, multiple threads contend on this single mutex:

1. **Main thread** loads modules (KernelState::LoadUserModule holds the lock).
2. **Dispatch thread** acquires the lock in a loop, and crucially **waits on a
   condvar while holding it** (kernel_state.cpp:371) -- `dispatch_cond_.wait(global_lock)`.
   Since it is a recursive_mutex, the condvar releases only one level of
   recursion. If another path on the same thread re-acquired it recursively
   before the condvar wait, the condvar will NOT release the mutex.
3. **Game threads** (particle init, audio, streaming) call operator new ->
   RtlAllocateHeap -> NtAllocateVirtualMemory -> BaseHeap::AllocRange, all
   competing for the same mutex.
4. **PhysicalHeap::TriggerCallbacks** (GPU texture streaming invalidation) can
   hold the lock for extended periods scanning page flags.

When `ExecuteInterrupt` fires (e.g. DPC timer), it holds the global lock for
the **entire duration** of the interrupt handler. If that handler allocates
memory, it re-enters the recursive mutex. Meanwhile any non-interrupt thread
calling NtAllocateVirtualMemory is blocked until the interrupt completes.

## When rexcrt_heap_enable = true

The o1heap path (`kernel/crt/heap.cpp`) uses its own **private `std::mutex`**
(`ReXHeap::mutex_`), completely bypassing the global critical region. This
eliminates the contention for heap allocations but does NOT help if the game's
recompiled RtlAllocateHeap still internally calls NtAllocateVirtualMemory for
large allocations or heap growth (the rexcrt hooks fully replace the Rtl
functions, so this path is avoided when enabled).

## Recommendations

1. **Enable rexcrt_heap_enable = true** in config.toml. This routes all
   RtlAllocateHeap/Free/Size/ReAlloc through o1heap with a private mutex,
   completely avoiding the global lock for heap operations.

2. **If rexcrt heap cannot be enabled**: The recompiled RtlAllocateHeap code
   calls NtAllocateVirtualMemory which acquires the global lock. The O(n)
   free-page scan in BaseHeap::AllocRange combined with lock contention from
   interrupt dispatch and physical heap callbacks creates the hang conditions.

3. **Long-term**: The global_critical_region singleton is an architectural
   bottleneck inherited from Xenia. Consider splitting it into per-subsystem
   locks (heap lock, thread lock, module lock) to reduce contention.

4. **Investigate ExecuteInterrupt**: Any DPC/interrupt that runs during init
   blocks ALL memory allocation system-wide. Ensure interrupt handlers during
   boot are minimal and non-allocating.
