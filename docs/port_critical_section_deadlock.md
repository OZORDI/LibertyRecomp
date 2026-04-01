# Critical Section Deadlock Analysis: operator new vs Streaming Worker

## Summary

The game's `operator new` hangs during init because `RtlEnterCriticalSection` blocks
indefinitely on a heap critical section. A streaming worker thread simultaneously
yields at 0x825FBCAC. The deadlock is caused by the interaction between the heap
allocator's critical section, the global_critical_region host mutex, and the
XObject lazy-init path.

## Critical Section Implementation

**File**: `glue/rexglue-sdk-main/src/kernel/xboxkrnl/xboxkrnl_rtl.cpp:386-454`

`RtlEnterCriticalSection_entry` follows this sequence:

1. Check recursive ownership (same thread already owns it) -- fast path return
2. Spin loop: `spin_count = header.absolute * 256` iterations of atomic CAS(-1 -> 0)
3. On spin failure: `atomic_inc(&lock_count)` -- if result != 0, a waiter exists
4. Call `xeKeWaitForSingleObject(cs.host_address(), 8, 0, 0, nullptr)` -- **infinite wait**

`RtlLeaveCriticalSection_entry`:

1. Decrement recursion_count; if still > 0, just decrement lock_count and return
2. Clear owning_thread to 0
3. `atomic_dec(&lock_count)` -- if result != -1, wake one waiter via `xeKeSetEvent`

## The Wait Path (Deadlock Vector)

`xeKeWaitForSingleObject` calls `XObject::GetNativeObject` which:

1. **Acquires `global_critical_region`** (a host-side recursive mutex)
2. Reads `X_DISPATCH_HEADER` at the critical section's address
3. If `wait_list_flink == kXObjSignature ('REX\0')`: lookup existing XEvent by handle
4. If first use: **creates a new XEvent** while holding the global lock, then stashes
   handle back into the dispatch header

`xeKeSetEvent` in `RtlLeaveCriticalSection` follows the same pattern: acquires
`global_critical_region`, looks up the XEvent, then signals it.

**Key issue**: Both Enter (wait) and Leave (signal) contend on `global_critical_region`.
If multiple threads simultaneously enter/leave different critical sections, they serialize
through this single host mutex.

## Heap Critical Section

The heap allocator functions at 0x8218ED50, 0x8218EDB8, 0x8218EE30 all lock the same
critical section at guest address **0x82B2835C** (computed as `(-32077 * 65536) + (-31912) + 4`).

Pattern (sub_8218ED50):
```
r30 = lis(-32077) + addi(-31912)   // = 0x82B28358
r3  = r30 + 4                       // = 0x82B2835C (the CS)
RtlEnterCriticalSection(r3)
vtable_call(allocator_obj)           // actual allocation
RtlLeaveCriticalSection(r3)
```

These functions are NOT called by name from generated code -- they are reached through
polymorphic vtable dispatch. The `sub_8218BE28` hook in `imports.cpp` intercepts the
top-level wrapper (which reads TLS[1676] and dispatches to the vtable).

## Streaming Worker (0x825FBCAC)

The streaming thread loop (in `gta4_recomp.35.cpp:26200-26345`) runs:

1. `sub_8285FFA0` -- scoped lock release (decrements refcount, calls
   `RtlLeaveCriticalSection` on the streaming resource lock when refcount hits 0)
2. `sub_82849918` -> `sub_82A12B60` -> `sub_82A1A200` -- **Sleep(r3)** where r3=16ms
3. Loops back to top

The streaming resource lock (per-object at `obj+1024`) uses `sub_8285FE78` to enter
(`RtlEnterCriticalSection` if lock word != 0) and `sub_8285FFA0` to leave.

## Deadlock Scenario

```
Thread A (main/init)               Thread B (streaming worker)
========================           ========================
operator new                       sub_8285FE78: Enter stream CS
  sub_8218BE28 (hooked)              RtlEnterCriticalSection(stream_cs)
    TLS[1676] valid                    acquired OK
    __imp__sub_8218BE28
      sub_8218ED50
        RtlEnterCriticalSection      allocates via vtable ->
          (heap CS 0x82B2835C)          sub_8218ED50
          spin fails                      RtlEnterCriticalSection
          atomic_inc -> contended           (heap CS 0x82B2835C)
          xeKeWaitForSingleObject           OWNS the lock
          GetNativeObject                   ...processing...
            ACQUIRES global_critical_region   xeKeSetEvent (on stream CS)
            ... blocks here if B holds it       GetNativeObject
                                                  ACQUIRES global_critical_region
                                                  ... blocks if A holds it
```

**Classic ABBA deadlock**: Thread A holds `global_critical_region` waiting on heap CS
event. Thread B holds heap CS, tries to signal stream CS event which needs
`global_critical_region`.

However, the actual sequence is more subtle: `GetNativeObject` acquires and releases
the global lock quickly (just for the lookup). The real issue is:

### Actual Root Cause: Uninitialized/Pre-initialized Critical Sections

The code at `xboxkrnl_rtl.cpp:336-337` notes:
> "Unfortunately some games have the critical sections pre-initialized in their
> embedded data and InitializeCriticalSection will never be called."

If the heap CS at 0x82B2835C is **pre-initialized in the .data section** with Xbox 360
byte order but `lock_count` is stored as **native int32** (not big-endian), the initial
value of -1 (`0xFFFFFFFF`) may be misinterpreted. Since `lock_count` is declared as
plain `int32_t` (NOT `rex::be<int32_t>`), and the data section is big-endian, the
initial -1 reads correctly. BUT `header.absolute` (spin count / 256) may be non-zero
in the embedded data, giving a large spin count that appears to work but masks timing
issues.

### More Likely: The Fallback Allocator Bypasses the Lock

The `sub_8218BE28` hook in `imports.cpp:901-928` checks `IsValidAllocator(TLS[1676])`.
When invalid, it calls `SystemHeapAlloc` directly, **bypassing the heap critical section
entirely**. If another thread is inside the real heap allocator holding the CS, and the
fallback allocator's `SystemHeapAlloc` internally acquires a host mutex that the heap
CS waiter also needs, that is the deadlock.

## Recommendations

1. **Trace the actual CS address**: Add logging to `RtlEnterCriticalSection_entry` when
   `lock_count` transitions from -1 to 0 (first acquire) and when contention occurs
   (the `atomic_inc` path). Log `cs.guest_address()` and thread ID.

2. **Check for pre-initialized CS corruption**: Dump bytes at 0x82B28358 (heap CS base)
   before any RtlInitializeCriticalSection call to verify the dispatch header is correct.

3. **global_critical_region contention**: The real bottleneck may be that ALL critical
   section operations (Enter/Leave for every CS in the game) serialize through
   `GetNativeObject`'s global lock. With many threads and many CSes, this becomes
   a hot contention point.

4. **Consider replacing xeKeWaitForSingleObject in RtlEnterCriticalSection**: Instead of
   going through the full XObject/XEvent path (which acquires global_critical_region),
   use a lightweight host-side condition variable per critical section. This eliminates
   the global lock contention entirely.

5. **Check the Sleep(16) path**: The streaming worker calling Sleep(16) at 0x825FBCAC
   goes through `sub_82A1A200` which calls `KeDelayExecutionThread`. If this also
   acquires global_critical_region or interacts with the thread scheduler lock, it could
   contribute to the contention window.
