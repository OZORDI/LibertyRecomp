# Global Allocator Objects

## Global Address Map

|Address|Object|Size|Vtable|
|-|-|-|-|
|0x82B29500|rage::sysMemMultiAllocator|40 (0x28)|0x8208480C|
|0x82B29528|rage::sysMemBuddyAllocator|192 (0xC0)|0x820848CC|
|0x82B295E8|Lock/CS pointer|4|N/A|
|0x82B295EC|rage::sysMemScopedLockAllocator|2300+|0x82084AE4|
|0x82B29EE4|Init flags bitmask|4|N/A|

TLS slots (set at end of init):
- TLS[1676] = current allocator (used by operator new / sub_8218BE28)
- TLS[1680] = default allocator (same value)

Both point to the multi-allocator at 0x82B29500 after initialization.

## Vtable Hierarchy

```
rage::sysMemAllocator (abstract base)
  vtable = 0x82000FA4  (set by sub_82847060)

  rage::sysMemMultiAllocator
    vtable = 0x8208480C  (set by sub_828475B0)
    Dispatches to child allocators in order

  rage::sysMemBuddyAllocator
    vtable = 0x820848CC  (set by sub_82847C08)
    Block-based allocator with bitmap free tracking

  rage::sysMemScopedLockAllocator
    vtable = 0x82084AE4  (set by sub_82849690)
    Small-object pool + fallback to large alloc
    Has built-in critical section for thread safety
```

## Vtable 0x8208480C (sysMemMultiAllocator) - 14 entries

|Idx|Offset|Function|Role|
|-|-|-|-|
|0|+0x00|sub_828475B0|Constructor (lis+stw vtable, zero count)|
|1|+0x04|sub_828475C8|Destructor (set vtable, tail-call sub_82847060)|
|2|+0x08|sub_828475F8|**Allocate** dispatcher: reads sub[idx] from array, calls sub->vtable[2]|
|3|+0x0C|sub_82847618|**Free** dispatcher: iterates subs, calls sub->vtable[18] to find owner, then sub->vtable[3]|
|4|+0x10|sub_828476F8|**Realloc** dispatcher: iterates subs via vtable[18], then calls sub->vtable[6]|
|5|+0x14|sub_82847790|**Reserve**: iterates subs via vtable[18], calls sub->vtable[24]|
|6|+0x18|sub_82847820|**Release**: iterates subs via vtable[18], calls sub->vtable[25]|
|7|+0x1C|sub_828478B0|**GetAllocInfo**: iterates subs via vtable[18], calls sub->vtable[19]|
|8|+0x20|sub_82847948|**GetSize**: iterates unique subs, calls sub->vtable[7], sums results|
|9|+0x24|sub_828479C8|**GetTotalSize**: iterates unique subs, calls sub->vtable[8], sums results|
|10|+0x28|sub_82847A40|**GetAllocBase**: forwards to sub[0]->vtable[9]|
|11|+0x2C|sub_82847A58|**SetAllocContext**: stores r4 to global (0x82B07070)|
|12|+0x30|sub_82847A68|**UpdateStats**: iterates all subs, calls sub->vtable[17]|
|13|+0x34|sub_82847AC0|**OwnsAddress**: iterates subs via vtable[18], returns matching sub|

### Multi-allocator dispatch pattern (vtable[2] = sub_828475F8)

```
Allocate(this, size, align, flags):
  idx = flags + 1                    // r6+1
  sub = this->sub_allocs[idx]        // lwzx at this + idx*4
  load sub->vtable                   // lwz r11, 0(sub)
  call sub->vtable[2]               // lwz r11, 8(vtable) -> Allocate
```

### Multi-allocator Free pattern (vtable[3] = sub_82847618)

```
Free(this, ptr):
  for i = 0..this->count-1:
    sub = this->sub_allocs[i]
    if sub->vtable[18](sub, ptr):   // OwnsAddress at offset +72
      // Check TLS[1688] and TLS[1704] for special routing
      if special: sub = this->sub_allocs[i+1]
      sub->vtable[3](sub, ptr)      // Free at offset +12
      return
  // fallback: log error
```

## Vtable 0x82084AE4 (sysMemScopedLockAllocator)

Key methods confirmed from generated code:

|Idx|Offset|Function|Role|
|-|-|-|-|
|2|+0x08|sub_828493E0|**Allocate**: acquires lock (sub_8285FF50), tries small pool (sub_82871828), falls back to sub_82848750|
|3|+0x0C|sub_828494D8|**Free**: acquires lock, checks bitmap for pool membership, calls sub_82871758 or sub_82848B68|
|4|+0x10|sub_82849580|**Realloc** (inferred from position)|

Constructor: sub_82849690 (with size arg) or sub_828495C8 (simpler variant).
Destructor: sub_828486B0.

### Scoped-lock Allocate pattern (sub_828493E0)

```
Allocate(this, size, align):
  lock = ScopedLock(this)           // sub_8285FF50 on stack
  if this->small_alloc_flag && global_context == -1:
    if size <= 8:  pool = &this->pool[0]  (+204)
    elif size <= 16: pool = &this->pool[1] (+212)
    elif size <= 32: pool = &this->pool[2] (+220)
    elif size <= 64: pool = &this->pool[3] (+228)
    result = small_alloc(pool, this)     // sub_82871828
    if result: return result
    // if pool full: check overflow flags at +2292
  // large allocation fallback
  return sub_82848750(this, size, align)
```

## Vtable 0x820848CC (sysMemBuddyAllocator)

Constructor: sub_82847C08(this, memory_base, block_size, block_count, lock_ptr).
Destructor: sub_82847C70.

Fields:
- +0x00: vtable
- +0x04: memory_base (ptr to managed region)
- +0x08: lock/parent (ptr)
- +0x0C: total_managed_bytes
- +0x10: block_size
- +0x14: bitmap/free_list (sub-object at +20)

## Object Layout: sysMemMultiAllocator (0x82B29500, 40 bytes)

|Offset|Size|Field|
|-|-|-|
|+0x00|4|vtable ptr (0x8208480C)|
|+0x04|4|sub_allocs[0] (0x82B295EC = scoped-lock alloc)|
|+0x08|4|sub_allocs[1] (0x82B29528 = buddy alloc)|
|+0x0C|4|sub_allocs[2] (0x82B29528 = buddy alloc, same)|
|+0x10|4|sub_allocs[3] (0x82B29528 = buddy alloc, same)|
|+0x14|4|sub_allocs[4] (unused)|
|+0x18|4|sub_allocs[5] (unused)|
|+0x1C|4|sub_allocs[6] (unused)|
|+0x20|4|sub_allocs[7] (unused)|
|+0x24|4|num_sub_allocs (4)|

## Object Layout: sysMemScopedLockAllocator (0x82B295EC, ~2300 bytes)

|Offset|Size|Field|
|-|-|-|
|+0x00|4|vtable ptr (0x82084AE4)|
|+0x04|4|base allocator / memory region ptr|
|+0x08|4|lock object ptr (from sub_82847320)|
|+0x50|4|capacity for managed region|
|+0x98|4|something read by sub_828493E0 at +152|
|+0xC2|1|init_flag (set to 0 on construct)|
|+0xC3|1|small_alloc_enabled_flag (checked in Allocate)|
|+0xC8|4|use_count at +200 (set to 1)|
|+0xCC|4|pool[0].head (0 initially)|
|+0xD0|2|pool[0].min_size = 8|
|+0xD2|2|pool[0].max_count = 2044|
|+0xD4|4|pool[1].head (0 initially)|
|+0xD8|2|pool[1].min_size = 16|
|+0xDA|2|pool[1].max_count = 1022|
|+0xDC|4|pool[2].head (0 initially)|
|+0xE0|2|pool[2].min_size = 32|
|+0xE2|2|pool[2].max_count = 511|
|+0xE4|4|pool[3].head (0 initially)|
|+0xE8|2|pool[3].min_size = 64|
|+0xEA|2|pool[3].max_count = 255|
|+0xEC|4|managed_base (base addr of region, used in Free)|
|+0xF0|2052|small pool bitmap/data (memset to 0)|
|+0x8F4|1|overflow_flags (bit 7 = small alloc ever failed)|

## Initialization Chain

### Call sequence (from C++ static initializer table)

```
sub_82A5AE68()                        // .ctors entry — startup wrapper
  sub_821B3770()                      // main allocator init
    flags = *(0x82B29EE4)

    if !(flags & 1):
      sub_82849690(0x82B295EC, 0x06000000, 1)   // construct scoped-lock, 96MB
      flags |= 1

    if !(flags & 2):
      lock = sub_828472D8(0x33000)              // create CS (208KB)
      *(0x82B295E8) = lock
      flags |= 2
    else:
      lock = *(0x82B295E8)

    if !(flags & 4):
      mem = sub_828472F0(0x11000000)            // alloc 272MB region
      sub_82847C08(0x82B29528, mem, 8192, 34816, lock)  // construct buddy alloc
      flags |= 4

    if !(flags & 8):
      sub_828475B0(0x82B29500)                  // construct multi-alloc
      flags |= 8

    // register sub-allocators into multi-alloc:
    sub_828475D8(lock, 0x82B295EC)              // sub[0] = scoped-lock
    sub_828475D8(lock, 0x82B29528)              // sub[1] = buddy
    sub_828475D8(lock, 0x82B29528)              // sub[2] = buddy (same)
    sub_828475D8(lock, 0x82B29528)              // sub[3] = buddy (same)

    // store multi-alloc to TLS for current thread
    TLS[1676] = 0x82B29500                      // multi-alloc (r30 was overwritten)
    TLS[1680] = 0x82B29500

  // after init: call vtable[11] on TLS[1676] allocator
  allocator = TLS[1676]
  allocator->vtable[11](allocator)              // SetAllocContext
```

### Global destructors

```
sub_82A6F960()  // calls sub_828475C8(0x82B29500)  — destroy multi-alloc
sub_82A6F970()  // calls sub_82847C70(0x82B29528)  — destroy buddy alloc
sub_82A6F980()  // calls sub_828486B0(0x82B295EC)  — destroy scoped-lock alloc
```

## Relationship to Global Heap Objects (0x8312B7D0/0x8312B7D8)

The global heap objects documented in port_allocator_vtable.md:
- Physical heap at 0x8312B7D0
- Virtual heap at 0x8312B7D8

These are separate from the allocator objects at 0x82B29xxx. They are initialized by sub_828AFFB8 (the large merged function containing sub_828B01A0), called from sub_82893060. This function was NOT split by codegen (sub_828B01A0 falls inside sub_828AFFB8 at an internal label, not a separate function entry).

The heap objects use vtable 0x82000970 (set by sub_8218C74C), different from the allocator vtables here. They serve as the backing store for virtual memory operations (NtAllocateVirtualMemory / NtFreeVirtualMemory).

### Allocation path hierarchy

```
operator new / sub_8218BE28
  TLS[1676] -> multi-alloc (0x82B29500, vtable 0x8208480C)
    vtable[2] = sub_828475F8 -> dispatches to:
      sub[0] = scoped-lock (0x82B295EC, vtable 0x82084AE4)
        vtable[2] = sub_828493E0 -> small pool or large alloc
      sub[1..3] = buddy (0x82B29528, vtable 0x820848CC)
        vtable[2] = block-based allocation

sub_8218BF20 (internal alloc with packed flags)
  global heaps at 0x8312B7D0 / 0x8312B7D8
    vtable 0x82000970, vtable[2] = profiled heap allocator
```

## What a Native Rewrite Must Replace

1. **The multi-allocator (0x82B29500)**: Replace with a native dispatcher that routes to sub-allocators by index. Trivial -- 40-byte object with an array of pointers.

2. **The scoped-lock allocator (0x82B295EC)**: Replace with a native allocator that:
   - Acquires a mutex on entry
   - Has 4 small-object pools (8/16/32/64 byte bins with 2044/1022/511/255 capacity)
   - Falls back to a large allocator (sub_82848750) for sizes > 64 or when pools full
   - Tracks allocations in a bitmap for O(1) Free dispatch

3. **The buddy allocator (0x82B29528)**: Replace with a native block allocator managing a 272MB region with 8KB blocks. Uses bitmap free-tracking.

4. **TLS[1676]/[1680] initialization**: Must store the native multi-allocator pointer into TLS offsets 1676 and 1680 before any game code runs.

5. **The lock object at 0x82B295E8**: Replace with a native mutex/critical section.

6. **Init flags at 0x82B29EE4**: Can be eliminated -- native init is unconditional.

7. **sub_828475D8** (add-sub-allocator): Must populate the multi-alloc's sub-array with the same order: [scoped-lock, buddy, buddy, buddy].

8. **Global heap objects** (0x8312B7D0/0x8312B7D8): Separate system, initialized by sub_828AFFB8. These back the NtAllocateVirtualMemory path and are NOT reached from operator new. They can remain as recompiled code unless the native rewrite needs to intercept virtual memory allocation.
