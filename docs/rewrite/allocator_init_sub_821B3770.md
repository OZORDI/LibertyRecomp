# Allocator Init Chain: sub_821B3770

## Entry Point

```
.ctors table entry #60 (of ~68 in 0x82A5xxxx range):
  sub_82A5AE68()                        <- .ctors wrapper
    sub_821B3770()                      <- main allocator constructor
    TLS[1676]->vtable[11]()             <- SetAllocContext
    sub_829FFA48(0x82A6F990)            <- debug print
```

### Preceding .ctors entries (immediate predecessors)
```
sub_82A5ADF8 -> RtlInitializeCriticalSection(0x82B2833C)    <- CS for object at 0x82B28338
sub_82A5AE30 -> RtlInitializeCriticalSection(0x82B2835C)    <- CS for object at 0x82B28358
```

## sub_821B3770 — Full Sequence

Uses a bitmask at **0x82B29EE4** to track which phases have completed (idempotent on re-entry).

### Phase 1: ScopedLockAllocator (bit 0)

```
if !(flags & 1):
  sub_82849690(this=0x82B295EC, pool_size=0x06000000, small_alloc_flag=1)
  sub_829FFA48(0x82A6F980)     <- debug print
  flags |= 1
```

**Constructor sub_82849690**:
1. Sets vtable = **0x82084AE4** at this+0
2. Initializes 4 small-object pool descriptors:

|Pool|Offset|Block Size|Max Count|Total Capacity|
|-|-|-|-|-|
|0|+204|8|2044|16,352 bytes|
|1|+212|16|1022|16,352 bytes|
|2|+220|32|511|16,352 bytes|
|3|+228|64|255|16,320 bytes|

3. `memset(this+240, 0, 2052)` — clears pool bitmap/metadata
4. Sets this+194 = 1 (small_alloc_enabled), this+200 = 1 (use_count)
5. Sets this+2292 bit 7 (overflow tracking byte)
6. Calls **sub_828472F0(0x06000000)** to allocate pool memory:
   - `NtAllocateVirtualMemory(addr=-1, size=96MB, flags=MEM_LARGE_PAGES|PAGE_READWRITE)`
   - Pool is 96 MB, large-page aligned (lower 24 bits = 0)
7. Calls **sub_82849278**(this, pool_ptr, 0x06000000, 1) — inner init:
   - Stores pool_ptr at this+8
   - Computes header: `(pool_ptr + 31) & ~31 + 16` stored at this+4
   - Usable region: pool_size - header_overhead, stored at this+76
   - Alignment down to 16KB stored at this+236
   - 16 free-list bucket heads zeroed at this+88..152 (this+84 = 16)
   - Initializes first block as a sentinel node (self-referential linked list)

**Object size**: >= 2293 bytes (accessed through offset 2292)

### Phase 2: BuddyAllocator metadata (bit 1)

```
if !(flags & 2):
  metadata = sub_828472D8(0x33000)     <- allocate 208,896 bytes
  *(0x82B295E8) = metadata              <- store metadata ptr globally
  flags |= 2
else:
  metadata = *(0x82B295E8)
```

**sub_828472D8**: `NtAllocateVirtualMemory(addr=0, size=0x33000, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE)`

Metadata size = **34816 entries x 6 bytes/entry = 208,896 bytes** (verified: `0x8800 * 6 = 0x33000`)

### Phase 3: BuddyAllocator (bit 2)

```
if !(flags & 4):
  pool = sub_828472F0(0x11000000)      <- allocate 272 MB
  sub_82847C08(this=0x82B29528, pool, block_size=8192, block_count=34816, fallback=metadata)
  sub_829FFA48(0x82A6F970)             <- debug print
  flags |= 4
```

**sub_828472F0(0x11000000)**: `NtAllocateVirtualMemory(addr=-1, size=272MB, MEM_LARGE_PAGES|PAGE_READWRITE)`
- Pool is 272 MB, large-page aligned (lower 24 bits = 0)

**Constructor sub_82847C08**:
1. Sets vtable = **0x820848CC** at this+0
2. Stores pool_base at this+4
3. Stores metadata_ptr (fallback) at this+8
4. block_size (8192) stored at this+16
5. usable_bytes = block_size * (block_count - 1) = 8192 * 34815 = **285,204,480** stored at this+12
6. Calls **sub_82871228(this+20, 34815, metadata_ptr)** — initializes bitmap free-tracker
7. Calls **sub_82847160(pool_base, pool_size)** — fills/initializes pool memory

**Verification**: 8192 * 34816 = 285,212,672 = 0x11000000 (matches pool allocation)

### Phase 4: MultiAllocator (bit 3)

```
if !(flags & 8):
  sub_828475B0(this=0x82B29500)
  sub_829FFA48(0x82A6F960)             <- debug print
  flags |= 8
```

**Constructor sub_828475B0**:
1. Sets vtable = **0x8208480C** at this+0
2. Sets count = 0 at this+36

### Phase 5: Register sub-allocators

Always executes (no flag guard):

```
sub_828475D8(multi=0x82B29500, child=0x82B295EC)    <- sub[0] = ScopedLockAllocator
sub_828475D8(multi=0x82B29500, child=0x82B29528)    <- sub[1] = BuddyAllocator
sub_828475D8(multi=0x82B29500, child=0x82B29528)    <- sub[2] = BuddyAllocator (same)
sub_828475D8(multi=0x82B29500, child=0x82B29528)    <- sub[3] = BuddyAllocator (same)
```

**sub_828475D8** appends child to the array at this+4 and increments count at this+36.

### Phase 6: TLS initialization

```
TLS[1676] = 0x82B29500    <- current allocator = MultiAllocator
TLS[1680] = 0x82B29500    <- default allocator = MultiAllocator
```

Both TLS slots get the same pointer — the MultiAllocator.

## Post-init (in sub_82A5AE68)

After sub_821B3770 returns:

```
allocator = TLS[1676]                          <- load MultiAllocator
allocator->vtable[11](allocator)               <- sub_82847A58: SetAllocContext
  -> stores r4 to global 0x82B07070
```

The r4 value at this point is 0x82B29528 (BuddyAllocator ptr, left over from the last sub_828475D8 call). So **0x82B07070 = BuddyAllocator**.

Then: `sub_829FFA48(0x82A6F990)` — final debug print.

## Global Address Map

|Address|Object|Size (bytes)|
|-|-|-|
|0x82B29500|rage::sysMemMultiAllocator|40|
|0x82B29528|rage::sysMemBuddyAllocator|~192|
|0x82B295E8|Metadata allocation pointer|4|
|0x82B295EC|rage::sysMemScopedLockAllocator|~2300|
|0x82B29EE4|Init flags bitmask|4|
|0x82B07070|Alloc context (set by vtable[11])|4|
|0x82B2833C|Critical section (pre-init #1)|28|
|0x82B2835C|Critical section (pre-init #2)|28|

## Vtable Map

|Vtable|Type|Set By|
|-|-|-|
|0x8208480C|sysMemMultiAllocator|sub_828475B0|
|0x820848CC|sysMemBuddyAllocator|sub_82847C08|
|0x82084AE4|sysMemScopedLockAllocator|sub_82849690|

## Memory Requirements

|Allocation|Size|Method|Pages|
|-|-|-|-|
|ScopedLockAllocator pool|96 MB (0x06000000)|sub_828472F0|Large|
|BuddyAllocator metadata|204 KB (0x00033000)|sub_828472D8|Regular|
|BuddyAllocator pool|272 MB (0x11000000)|sub_828472F0|Large|
|**TOTAL**|**~368 MB**|||

Plus ~2.3 KB for the ScopedLockAllocator object itself and 40 bytes for the MultiAllocator (both in .bss).

### BuddyAllocator Details

- **Block size**: 8,192 bytes (8 KB)
- **Block count**: 34,816
- **Usable blocks**: 34,815 (count - 1; first block reserved)
- **Usable capacity**: 285,204,480 bytes (~272 MB)
- **Metadata**: 6 bytes per block (34,816 * 6 = 208,896 bytes)
- **Metadata structure**: Initialized by sub_82871228 — likely a buddy-system bitmap + free-list index

### ScopedLockAllocator Details

- **Pool**: 96 MB contiguous region
- **Small pools**: 4 bins (8/16/32/64 byte blocks) with capacities 2044/1022/511/255
- **Small pool total**: ~63.8 KB of 96 MB used for small allocations
- **Large allocations**: sub_82848750 for sizes > 64 bytes or when pools exhaust
- **Thread safety**: Built-in critical section (sub_8285FF50 / sub_8285FFA0)
- **16 free-list buckets** at this+88..152 for size-class dispatch

### MultiAllocator Dispatch Order

```
sub[0] = ScopedLockAllocator  <- small/general allocations (flags==-1 or default)
sub[1] = BuddyAllocator       <- large block allocations (flags==0)
sub[2] = BuddyAllocator       <- same object, for flags==1
sub[3] = BuddyAllocator       <- same object, for flags==2
```

Dispatch via vtable[2] (sub_828475F8): `child = sub_allocs[flags + 1]`, then calls child->vtable[2].

## Complete Init Sequence (Ordered)

```
1. sub_82A5ADF8: RtlInitializeCriticalSection(0x82B2833C)
2. sub_82A5AE30: RtlInitializeCriticalSection(0x82B2835C)
3. sub_82A5AE68: Allocator init wrapper
   3a. sub_821B3770:
       - Phase 1: Construct ScopedLockAllocator (96 MB pool)
       - Phase 2: Allocate BuddyAllocator metadata (204 KB)
       - Phase 3: Construct BuddyAllocator (272 MB pool)
       - Phase 4: Construct MultiAllocator
       - Phase 5: Register [scoped, buddy, buddy, buddy] into multi
       - Phase 6: TLS[1676] = TLS[1680] = MultiAllocator
   3b. Call vtable[11] (SetAllocContext) -> 0x82B07070 = BuddyAllocator
   3c. Debug print
4. sub_82A5AEB0: Construct object at 0x82B29EF0 (vtable 0x82000A08)
5. sub_82A5AED0: Construct 3 objects at 0x82B29F18 (188-byte stride, sub_822094C8)
6. sub_82A5AF20: Construct single object at 0x82B2A208 (sub_822094C8)
7. sub_82A5AF30: Construct 3 objects at 0x82B2A2F0 (4220-byte stride, sub_822B5958)
```

## What a Native Rewrite Must Replicate

1. **Allocate ~368 MB total** at startup:
   - 96 MB for the scoped-lock allocator's pool (can use mmap/VirtualAlloc with large pages if available)
   - 272 MB for the buddy allocator's pool
   - 204 KB for buddy allocator metadata

2. **Initialize three allocator objects**:
   - ScopedLockAllocator: 4 small-object pools (8/16/32/64 byte bins), mutex-protected, backed by 96 MB pool
   - BuddyAllocator: 8 KB block size, 34815 usable blocks, bitmap free-tracking, backed by 272 MB pool
   - MultiAllocator: dispatcher array [scoped, buddy, buddy, buddy], routes by flags+1 index

3. **Set TLS for main thread**: Both TLS[1676] and TLS[1680] = MultiAllocator pointer

4. **Set global**: 0x82B07070 = BuddyAllocator pointer (alloc context)

5. **Init flags at 0x82B29EE4**: Set to 0xF (all 4 phases done) — or eliminate entirely since native init is unconditional

6. **Critical sections** at 0x82B2833C and 0x82B2835C must be initialized before allocator init

7. **For worker threads**: sub_82849940 copies the allocator pointer from a thread descriptor into TLS[1676]/[1680] — native rewrite uses `thread_local` instead

8. **The allocator vtables must remain valid** in guest memory because 86+ inline call sites in generated code do vtable dispatch through TLS[1676]. A native replacement must either:
   - Hook all vtable entry points to redirect to native allocators, OR
   - Write native vtable entries into guest memory at the expected addresses
