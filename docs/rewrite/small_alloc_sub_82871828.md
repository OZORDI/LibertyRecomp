# Small Object Allocator: sub_82871828

## Overview

`sub_82871828` is the small-object allocation core. It implements a slab allocator with
embedded freelists, serving allocations of up to 64 bytes from pre-carved 16 KB pools.

## Call Chain

```
sub_828493E0 (dispatcher)
  sub_8285FF50 (ScopedLock::Enter → RtlEnterCriticalSection on CS @ 0x83192960)
  sub_82871828 (small alloc core)          ← THIS FUNCTION
    sub_82848750 (large alloc / slab refill, 16 KB)
    sub_829FF840 (memset, fills block with 0xCD)
  sub_8285FFA0 (ScopedLock::Leave → RtlLeaveCriticalSection)
```

## Dispatcher: sub_828493E0

**Signature**: `void* sub_828493E0(Allocator* alloc, uint32_t size, uint32_t alignment)`

### Small-path eligibility (all must be true)

1. `alloc->small_alloc_enabled` (byte at +195) != 0
2. `*(int32_t*)0x82B07070` == -1 (global debug/mode flag)
3. `size` <= 64
4. `alignment` <= 16

### Size class routing

| Condition | Bucket offset | Max alloc |
|-|-|-|
| size <= 8 | alloc+204 (0xCC) | 8 |
| size <= 16 | alloc+212 (0xD4) | 16 |
| size <= 32 | alloc+220 (0xDC) | 32 |
| size <= 64 | alloc+228 (0xE4) | 64 |

If ineligible: falls through to `sub_82848750(alloc, size, alignment)` (large path).

## sub_82871828 — Small Alloc Core

**Signature**: `void* sub_82871828(BucketDesc* bucket, Allocator* alloc)`

### BucketDesc struct (8 bytes, at allocator+offset)

| Offset | Type | Field |
|-|-|-|
| +0 | u32 | pool_head — head of doubly-linked pool list |
| +4 | u16 | block_size — size of each allocation block |
| +6 | u16 | blocks_per_pool — blocks carved from each 16 KB slab |

### PoolNode struct (32 bytes = 0x20, at start of each slab)

| Offset | Type | Field |
|-|-|-|
| +0 (0x00) | u32 | prev — back-link to newer pool (NULL if head) |
| +4 (0x04) | u32 | next — forward link to older pool |
| +8 (0x08) | u32 | free_count — number of free blocks remaining |
| +12 (0x0C) | u32 | free_head — head of embedded free-block list |
| +16 (0x10) | u32 | owner — back-pointer to BucketDesc |
| +20..31 | — | padding/unused |

### Allocation Algorithm (pseudocode)

```
function small_alloc(bucket, alloc):
    pool = bucket->pool_head

    // 1. Walk pool chain looking for one with free blocks
    while pool != NULL:
        if pool->free_count != 0:
            goto fast_path
        pool = pool->next

    // 2. Pool exhaustion — allocate a new 16 KB slab
    slab = sub_82848750(alloc, 16384, 16384)

    // 2a. Update page bitmap (marks slab as small-alloc-owned)
    page_idx = (slab - alloc->base_addr) >> 14   // [alloc+236]
    bit = page_idx & 0x1F
    word = page_idx >> 5
    alloc->bitmap[word] |= (1 << bit)            // [alloc+240 + word*4]

    if slab == NULL:
        return NULL

    // 2b. Initialize PoolNode at slab start
    old_head = bucket->pool_head
    if old_head: old_head->prev = slab
    slab->prev = NULL
    slab->next = old_head
    slab->owner = bucket
    slab->free_count = bucket->blocks_per_pool - 1

    // 2c. Build embedded freelist
    first_free = slab + 32 + bucket->block_size
    slab->free_head = first_free
    cursor = first_free
    for i in range(bucket->blocks_per_pool - 2):
        next = cursor + bucket->block_size
        *(u32*)cursor = next
        cursor = next
    *(u32*)cursor = NULL

    // 2d. Return first block (not in freelist)
    block = slab + 32
    bucket->pool_head = slab
    goto fill_and_return

fast_path:
    // 3. Pop block from pool's freelist
    block = pool->free_head
    pool->free_count -= 1
    pool->free_head = *(u32*)block   // next-free pointer embedded in block

fill_and_return:
    // 4. Debug fill and return
    memset(block, 0xCD, bucket->block_size)  // sub_829FF840
    return block
```

### Slab Capacity

Slab = 16384 bytes. Pool header = 32 bytes. Usable = 16352 bytes.

| Block size | Blocks/slab | Waste |
|-|-|-|
| 8 | 2044 | 0 |
| 16 | 1022 | 0 |
| 32 | 511 | 0 |
| 64 | 255 | 32 |

## Pool Exhaustion Behavior

1. `sub_82871828` calls `sub_82848750(alloc, 16384, 16384)` — the same large-block allocator
   used for normal allocations, but requesting a 16 KB aligned slab.
2. If `sub_82848750` returns NULL, `sub_82871828` returns NULL.
3. The dispatcher `sub_828493E0` checks for NULL return:
   - If NULL and `alloc->flags[+2292] & 0x7F` != 0: calls `sub_822BCA90` (assertion/error log)
   - Either way, unlocks and returns NULL to caller.

## Allocator Struct Offsets

| Offset | Hex | Type | Field |
|-|-|-|-|
| +4 | 0x04 | u32 | arena_start (used in large path) |
| +76 | 0x4C | u32 | size field (large path boundary check) |
| +152 | 0x98 | u32 | max_block_size (large path eligibility) |
| +195 | 0xC3 | u8 | small_alloc_enabled |
| +200 | 0xC8 | u32 | generation/tag for block metadata |
| +204 | 0xCC | BucketDesc | 8-byte bucket |
| +212 | 0xD4 | BucketDesc | 16-byte bucket |
| +220 | 0xDC | BucketDesc | 32-byte bucket |
| +228 | 0xE4 | BucketDesc | 64-byte bucket |
| +236 | 0xEC | u32 | base_address (arena base for bitmap math) |
| +240 | 0xF0 | u32[] | page_bitmap (1 bit per 16 KB page) |
| +2292 | 0x8F4 | u8 | flags (bit 0-6: enable error assertion) |

## Blocking Points

- `sub_82871828` itself acquires NO locks.
- The dispatcher `sub_828493E0` wraps the entire operation in a scoped critical section:
  - `sub_8285FF50` → `RtlEnterCriticalSection(0x83192960)` (enter)
  - `sub_8285FFA0` → `RtlLeaveCriticalSection(0x83192960)` (leave)
- The critical section is a global at `0x83192960`, shared across all small allocations.
- `sub_82848750` (slab refill) runs while the lock is held.

## Size Threshold: Small vs Large

| Path | Condition | Function |
|-|-|-|
| Small | size <= 64 AND align <= 16 AND enabled AND global flag == -1 | sub_82871828 |
| Large | size > 64 OR align > 16 OR disabled OR global flag != -1 | sub_82848750 |

The small allocator is a fast path that avoids the overhead of the general-purpose
best-fit free-list allocator (`sub_82848750`). The 64-byte threshold with 4 size classes
(8, 16, 32, 64) eliminates fragmentation for the most common small allocations.
