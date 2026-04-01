# sub_8218BE28 Heap Lock Analysis

## IDA Pseudocode (Original Xbox 360)

```c
int __fastcall sub_8218BE28(int a1)  // a1 = size
{
    int v1; // r13 (TLS base register)
    return (*(int (**)(DWORD, int, int, DWORD))
        (**(_DWORD **)(*(_DWORD *)v1 + 1676) + 8))(
            *(_DWORD *)(*(_DWORD *)v1 + 1676),
            a1,     // size
            16,     // alignment (hardcoded)
            0);     // flags
}
```

The function is a thin vtable dispatch:

1. Read TLS[r13+1676] to get the RAGE allocator object pointer
2. Dereference the allocator's vtable (offset 0)
3. Call vtable[+8] (the `Allocate` virtual method) with `(allocator, size, 16, 0)`

sub_8218BE28 itself contains NO locking, NO retry loop, NO spin, NO yield, NO call to NtAllocateVirtualMemory, and NO call to sub_82849918. It is 9 PPC instructions.

The aligned variant sub_8218BE50 is identical except it passes `a2` (caller-supplied alignment) instead of hardcoded 16.

## Does It Call RtlEnterCriticalSection?

No. sub_8218BE28 performs zero lock operations. It is a pure vtable dispatch.

## Does It Call NtAllocateVirtualMemory?

Not directly. The vtable dispatch target (the RAGE allocator implementation) may internally call NtAllocateVirtualMemory to grow the heap. In the recomp, NtAllocateVirtualMemory is a RexGlue kernel import that calls `BaseHeap::AllocRange`, which acquires `global_critical_region_` (a process-wide `std::recursive_mutex`).

## Does It Call sub_82849918 (Yield)?

No. sub_82849918 does not appear in the pseudocode or in the codebase.

## Does It Have a Retry/Spin Loop?

No. It is a single vtable call with a direct return.

## Fallback Path (TLS Allocator Null)

In `imports.cpp` line 901, `PPC_FUNC_HOOK(sub_8218BE28)` intercepts all calls:

```cpp
PPC_FUNC_HOOK(sub_8218BE28) {
    uint32_t memMgr = ReadTLS(r13, 1676);
    if (!IsValidAllocator(memMgr)) {
        // Fallback: RexGlue SystemHeapAlloc
        uint32_t guest = mem->SystemHeapAlloc(size);
        ctx.r3.u32 = guest;
        return;
    }
    __imp__sub_8218BE28(ctx, base);  // Normal path (dead code -- see below)
}
```

The fallback calls `Memory::SystemHeapAlloc` -> `BaseHeap::Alloc` -> `BaseHeap::AllocRange`, which acquires `global_critical_region_.Acquire()` -- a `std::recursive_mutex` singleton shared by ALL heap operations (alloc, free, protect, release).

**Critical finding**: `__imp__sub_8218BE28` is declared `extern "C"` but has NO definition anywhere. The codegen does not produce this function (0x8218BE28 is not a discovered function boundary; it falls within sub_8218BC28's body in the generated code). The normal path at line 928 is dead code. ALL allocations go through the fallback.

## Lock Chain for a 1024-Byte Allocation

```
sub_8218BE28 (hook)
  -> IsValidAllocator: reads TLS[1676], checks vtable range
  -> FALLBACK (always, because __imp__ is undefined):
     -> Memory::SystemHeapAlloc(1024)
        -> BaseHeap::Alloc(1024, 0x20, RESERVE|COMMIT, RW)
           -> BaseHeap::AllocRange(...)
              -> global_critical_region_.Acquire()  // std::recursive_mutex
              -> linear page table scan for free pages
              -> host mmap/mprotect
              -> global_critical_region_ released on scope exit
```

The `global_critical_region_` is a **process-wide singleton recursive mutex** (`rex::thread::global_critical_region::mutex()`). Every guest memory operation (NtAllocateVirtualMemory, NtFreeVirtualMemory, SystemHeapAlloc, SystemHeapFree, Protect, Release) contends on this same lock.

## Hang Scenario

A 1024-byte allocation hangs if another thread holds `global_critical_region_` indefinitely. Possible causes:

1. **Page table scan**: `AllocRange` does a linear scan of the page table (no free-list optimization). With heavy fragmentation, the scan under the lock can take a long time, blocking all other allocators.

2. **Deadlock with guest thread suspension**: The mutex.h comments warn that `global_critical_region_` must be the FIRST lock acquired. If a guest thread holds another lock and then tries to allocate (acquiring global_critical_region_), while a kernel thread holds global_critical_region_ and tries to suspend that guest thread, a classic AB-BA deadlock occurs.

3. **ReXHeap contention**: If `rexcrt_heap_enable` is true, `RtlAllocateHeap` also acquires its own `mutex_` inside `ReXHeap::Alloc`. The RAGE allocator's vtable dispatch to RtlAllocateHeap (via the CRT hook) would attempt: global_critical_region_ -> ReXHeap::mutex_, creating a two-lock path.

4. **Fallback storm**: With TLS always invalid, every allocation from every thread goes through SystemHeapAlloc, serializing all game allocations through a single mutex. During init, the particle emitter registration loop (4608 allocations) and other subsystems compete for this lock on the main thread, while streaming, audio, and render threads also allocate.

## Key Files

| File | Path |
|-|-|
| Hook implementation | `LibertyRecomp/kernel/imports.cpp:901` |
| IDA pseudocode | `/Users/Ozordi/Downloads/default (1).xex.c:1227150` |
| Global critical region | `glue/rexglue-sdk-main/include/rex/thread/mutex.h` |
| BaseHeap::AllocRange | `glue/rexglue-sdk-main/src/system/xmemory.cpp:1004` |
| SystemHeapAlloc | `glue/rexglue-sdk-main/src/system/xmemory.cpp:544` |
| ReXHeap (o1heap) | `glue/rexglue-sdk-main/src/kernel/crt/heap.cpp` |
| Push/Pop allocator | IDA pseudocode at `default (1).xex.c:2552613` |
