# sub_828493E0 -- Scoped-Lock Allocator (Full Trace)

## Prototype

```
void* sub_828493E0(Allocator* this, uint32_t size, uint32_t alignment);
```

- **r3** = `this` (allocator object, ~2300+ bytes)
- **r4** = requested allocation size
- **r5** = requested alignment
- **Returns** r3 = pointer to allocated memory, or NULL

## High-Level Summary

This is a **thread-safe heap allocator** with two allocation strategies:
1. **Small alloc** (size <= 64, alignment <= 16): slab allocator with fixed-size pools
2. **Large alloc** (all other sizes): segregated free-list allocator with best-fit search

A **scoped critical section lock** protects the entire operation. The lock target is a global `CRITICAL_SECTION` at address `0x83192960`.

## Complete Call Chain

```
sub_828493E0(this, size, alignment)
 |
 +-- sub_8285FF50(scoped_lock@sp+80, &cs_0x83192960)  [lock constructor]
 |     +-- RtlEnterCriticalSection(&cs_0x83192960)
 |
 +-- [SMALL PATH: if eligible]
 |     +-- sub_82871828(bucket_ptr, this)
 |     |     +-- [walk pages for free slot]
 |     |     +-- [if none: sub_82848750(this, 16384, 16384) to get new page]
 |     |     +-- [carve page into elements, build free list]
 |     |     +-- sub_829FF840(ptr, 0xCD, elem_size)  [memset debug fill]
 |     |
 |     +-- [if NULL and extended_flags set]
 |           +-- sub_822BCA90(assert_msg, size, free_space)  [debug assert]
 |
 +-- [LARGE PATH: else]
 |     +-- sub_82848750(this, size, alignment)
 |     |     +-- [align to 16 bytes, search free lists]
 |     |     +-- [unlink block, handle alignment padding, split remainder]
 |     |     +-- sub_82847160(ptr, block_size)  [debug fill via memset/pattern]
 |     |     +-- return block + 16
 |
 +-- sub_8285FFA0(scoped_lock@sp+80)  [lock destructor]
 |     +-- RtlLeaveCriticalSection(&cs_0x83192960)
 |
 +-- return allocation result
```

## Kernel Calls Reached

| Call | Address | When |
|-|-|-|
| RtlEnterCriticalSection | 0x82A74D84 (import) | Always, on entry via scoped lock |
| RtlLeaveCriticalSection | 0x82A74DA4 (import) | Always, on exit via scoped lock |

No other kernel calls. All allocation is from pre-reserved memory pools.

## ScopedLock Object (8 bytes, stack at sp+80)

| Offset | Type | Field |
|-|-|-|
| +0x00 | u32 | recursion_count (set to 1 on construct) |
| +0x04 | u32 | critical_section_ptr |

### sub_8285FF50 (Constructor)

```
ScopedLock_ctor(ScopedLock* this, CRITICAL_SECTION* cs):
    this->recursion_count = 1
    this->cs_ptr = cs
    if cs->field_0 != 0:        // cs is initialized
        RtlEnterCriticalSection(cs)
    return this
```

### sub_8285FFA0 (Destructor)

```
ScopedLock_dtor(ScopedLock* this):
    if this->recursion_count == 0: return
    this->recursion_count -= 1
    if this->recursion_count != 0: return   // still held (recursive)
    cs = this->cs_ptr
    if cs->field_0 == 0: return             // cs not initialized
    RtlLeaveCriticalSection(cs)
```

## Small Alloc Eligibility (sub_828493E0)

All four conditions must be true:
1. `this->small_alloc_enabled` (byte at this+0xC3) != 0
2. Global `[0x82B07070]` == -1 (0xFFFFFFFF) -- system state check
3. `size <= 64`
4. `alignment <= 16`

## Small Alloc Bucket Selection

| Condition | Bucket Offset | Bucket |
|-|-|-|
| size <= 8 | this+204 (0xCC) | 8-byte pool |
| size <= 16 | this+212 (0xD4) | 16-byte pool |
| size <= 32 | this+220 (0xDC) | 32-byte pool |
| size <= 64 | this+228 (0xE4) | 64-byte pool |

### Small Bucket Descriptor (8 bytes)

| Offset | Type | Field |
|-|-|-|
| +0x00 | u32 | first_page_ptr |
| +0x04 | u16 | element_size |
| +0x06 | u16 | elements_per_page |

## sub_82871828 -- Small Alloc Path

**Args**: r3 = bucket_descriptor, r4 = allocator_this

### Algorithm

1. **Walk page list** (bucket->first_page, linked via page+4):
   - If page->free_count (+8) > 0: pop from free list (fast path)
   - Else: try next page

2. **If no free items in any page**: allocate new 16KB page via `sub_82848750(allocator, 16384, 16384)`
   - Set bitmap bit: `allocator->bitmap[(page_index >> 5)] |= (1 << (page_index & 31))`
     where `page_index = (page_addr - allocator->bitmap_base) >> 14`
   - Initialize page header (32 bytes)
   - Build singly-linked free list through page, elements spaced by `element_size`
   - First element returned directly; free list starts at second element
   - Prepend page to bucket's page list

3. **Debug fill**: `memset(result, 0xCD, element_size)`

4. **Return** pointer to allocated element

### Page Header (32 bytes at start of 16KB page)

| Offset | Type | Field |
|-|-|-|
| +0x00 | u32 | reserved (set to 0) |
| +0x04 | u32 | next_page (linked list) |
| +0x08 | u32 | free_count |
| +0x0C | u32 | first_free (free list head) |
| +0x10 | u32 | owner_bucket_ptr (back-pointer) |
| +0x14 | u32 | next_in_chain (traversal / cycle detect) |

### Free Element (within page)

| Offset | Type | Field |
|-|-|-|
| +0x00 | u32 | next_free (NULL = end of list) |
| +0x04.. | -- | 0xCD debug fill |

## sub_82848750 -- Large Alloc Path

**Args**: r3 = allocator, r4 = size, r5 = alignment

### Algorithm

1. **Enforce minimums**: alignment = max(alignment, 16); size = max(size, alignment)
2. **Align size**: `aligned_size = (size + 15) & ~0xF`
3. **Debug assertion**: if TLS[+1684] != 0, assert via sub_822BCA90
4. **Capacity check**: if `aligned_size > allocator->free_space` (+152), return NULL
5. **Size class**: `class = min(aligned_size >> 4, 15)` -- 16 buckets, 0..15
6. **Best-fit search** across buckets `class..15`:
   - Walk free list (doubly-linked via +16/+20, with Floyd's tortoise for cycle detection)
   - For each free block: check if `block_end - aligned_addr >= aligned_size`
   - Track best-fit (smallest sufficient block)
   - Perfect fit breaks immediately
7. **If no block found**: return NULL
8. **Unlink block** from free list (doubly-linked removal)
9. **Mark in-use**: `block->flags |= 0x10; block->flags = (generation << 6) | (flags & 0x3F) | 0x10`
10. **Handle alignment padding**:
    - If user data address not aligned: split front portion as separate free block or merge with predecessor
    - Adjust block to start at aligned boundary
11. **Split remainder**:
    - If `block_size > aligned_size + 16`: create new free block for the excess
    - Insert remainder into appropriate bucket
12. **Update stats**: `used_overhead += block_size; free_space -= block_size`
13. **Size-class counter**: `allocator[((flags & 0xF) + 22) * 4] += block_size`
14. **Debug fill**: `sub_82847160(ptr, block_size)` -- fills with pattern from `[0x82B07038]` or calls memset
15. **Return**: `block_addr + 16` (user data after 16-byte header)

### Large Block Header (16 bytes)

| Offset | Type | Field |
|-|-|-|
| +0x00 | u32 | self_ptr (== block_addr when free; integrity check) |
| +0x04 | u32 | block_size (usable bytes, not including header) |
| +0x08 | u32 | prev_block_ptr (physical predecessor in memory) |
| +0x0C | u32 | flags: bits[0:3] = size_class, bit[4] = IN_USE, bits[6:31] = generation |
| +0x10 | u32 | free_list_prev (NULL when in-use) |
| +0x14 | u32 | free_list_next (NULL when in-use) |

## Allocator Object Layout

| Offset | Size | Field |
|-|-|-|
| +0x00 | 4 | field_0 (possibly vtable or type tag) |
| +0x04 | 4 | base_address (pool memory base) |
| +0x0C | 64 | free_list_heads[16] (large alloc buckets, 4 bytes each) |
| +0x4C | 4 | pool_limit_offset (+76) |
| +0x54 | 4 | used_overhead (+84, total allocated header bytes) |
| +0x58..+0x94 | -- | size-class counters array |
| +0x98 | 4 | free_space (+152, remaining allocatable bytes) |
| +0xC3 | 1 | small_alloc_enabled (+195) |
| +0xC8 | 4 | generation_stamp (+200) |
| +0xCC | 8 | small_bucket_8 (+204) |
| +0xD4 | 8 | small_bucket_16 (+212) |
| +0xDC | 8 | small_bucket_32 (+220) |
| +0xE4 | 8 | small_bucket_64 (+228) |
| +0xEC | 4 | bitmap_base (+236, address used for page index calculation) |
| +0xF0 | var | page_bitmap (+240, bit per 16KB page) |
| +0x8F4 | 1 | extended_flags (+2292, top 25 bits checked for OOM assert) |

## Global Addresses

| Address | Purpose |
|-|-|
| 0x83192960 | Global CRITICAL_SECTION for allocator lock |
| 0x82B07070 | Global state word (-1 enables small alloc path) |
| 0x831927D1 | Debug fill enable flag (byte) |
| 0x82B07038 | Debug fill pattern (4 bytes) |

## sub_82847160 -- Debug Memory Fill

Called at end of large alloc to fill allocated memory with a pattern.

```
sub_82847160(void* ptr, uint32_t size):
    if [0x831927D1] != 0:
        // byte-by-byte fill (debug: each byte = low byte of ptr)
        // This is likely a corrupted decompilation; real behavior fills with debug pattern
    else:
        pattern = load_u32(0x82B07038)
        if all 4 bytes of pattern are equal:
            memset(ptr, pattern_byte, size)  // fast path via rexcrt_memset
        else:
            word-fill with pattern
```

## sub_822BCA90 -- Debug Assert

Called on various error conditions (OOM, corruption). Takes a string pointer and optional args. Not a kernel call -- internal debug/logging only.

## Key Design Observations

1. **Two-tier allocator**: Small objects (<=64 bytes) use slab allocation from 16KB pages; large objects use segregated free lists with best-fit.
2. **Single global lock**: The entire allocation (both paths) is protected by one critical section at 0x83192960. This is a potential contention point.
3. **16 size classes** for large alloc: each bucket holds free blocks whose `size >> 4` equals the class index (capped at 15 for anything >= 256 bytes).
4. **Best-fit with alignment**: Large alloc searches across all eligible size classes, accounting for alignment waste.
5. **Block splitting**: After allocation, leftover space is split into a new free block if remainder > 16 bytes.
6. **Debug fills**: Both paths fill allocated memory with debug patterns (0xCD for small, configurable pattern for large). This can be disabled.
7. **Bitmap tracking**: Small alloc pages are tracked in a bitmap at allocator+240, indexed by `(page - bitmap_base) >> 14`.
8. **No kernel memory calls**: All allocation is from pre-reserved pools. Only kernel calls are RtlEnterCriticalSection / RtlLeaveCriticalSection.
