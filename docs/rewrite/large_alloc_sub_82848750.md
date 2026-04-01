# sub_82848750 -- Large Allocation Path (Free-List Allocator)

## Function Signature

```
void* sub_82848750(r3=allocator_obj, r4=size, r5=alignment)
```

- **r29** = allocator_obj (`this`)
- **r25** = alignment (clamped to min 16)
- **r20** = aligned_size = `(max(size, alignment) + 15) & ~0xF`
- **Returns**: user data pointer (block + 16), or NULL on failure

## Allocator Object Struct Layout (this+X)

| Offset | Size | Field |
|-|-|-------|
| 0x00 | 4 | (unused in this function; TLS base read via r13+0) |
| 0x04 | 4 | heap_base |
| 0x0C-0x48 | 64 | **bucket[0..15]** -- free-list head pointers (4 bytes each) |
| 0x4C | 4 | heap_size |
| 0x54 | 4 | used_bytes |
| 0x58-0x94 | 60 | stat_buckets[0..14] -- per-flag-nibble allocation stats (4 bytes each) |
| 0x98 | 4 | largest_free_block |
| 0xC3 | 1 | small_alloc_flag (byte; checked by parent sub_828493E0) |
| 0xC8 | 4 | alloc_stamp / generation counter |
| 0xCC | 4 | small_pool_8 (parent) |
| 0xD4 | 4 | small_pool_16 (parent) |
| 0xDC | 4 | small_pool_32 (parent) |
| 0xE4 | 4 | small_pool_64 (parent) |
| 0xEC | 4 | page_base (parent; used for bitmap indexing) |
| 0x8F4 | 1 | debug_flags (parent checks bit 7) |

### Bucket Array Detail

```
bucket[i] @ this + (i + 3) * 4
```

| Bucket | Offset | Size class (data bytes) |
|-|-|-|
| 0 | this+12 | 0-15 |
| 1 | this+16 | 16-31 |
| 2 | this+20 | 32-47 |
| ... | ... | ... |
| 14 | this+68 | 224-239 |
| 15 | this+72 | 240+ (catch-all) |

## Free Block Node Layout (24 bytes)

| Offset | Size | Field |
|-|-|-|
| 0x00 | 4 | self_ptr (sentinel; equals own address when valid) |
| 0x04 | 4 | data_size (user data capacity, excluding 16-byte header) |
| 0x08 | 4 | prev_physical (physical predecessor block, NOT list link) |
| 0x0C | 4 | flags_stamp (bits 0-3: type flags; bit 4: ALLOCATED; bits 6-31: stamp from this+0xC8 << 6) |
| 0x10 | 4 | left_child (prev in doubly-linked bucket list; NULL if list head) |
| 0x14 | 4 | next_in_bucket (next free block in same bucket) |

Total block in memory: 16-byte header + data_size bytes of user data.

User data returned to caller = `block_addr + 16`.

## Free-List Walk Algorithm

```
function allocate(this, size, alignment):
    alignment = max(alignment, 16)
    size = max(size, alignment)
    aligned_size = (size + 15) & ~0xF

    if aligned_size > this->largest_free:
        return NULL

    start_bucket = aligned_size >> 4
    if start_bucket >= 16:
        return NULL           # no bucket can hold this size

    best_block = NULL
    best_waste = UINT32_MAX

    for bucket = start_bucket .. 15:
        block = this->bucket[bucket]
        if block == NULL: continue

        tortoise = block->next_in_bucket   # Floyd cycle detection

        while block != NULL:
            ASSERT(block->self_ptr == block)     # corruption check (no-op)

            block_end     = block + block->data_size + 16
            aligned_start = (block + alignment) & ~(alignment - 1)

            if block_end > aligned_start:
                usable = block_end - aligned_start
                if usable == aligned_size:
                    best_block = block       # exact match -- stop
                    BREAK
                if usable > aligned_size AND usable < best_waste:
                    best_block = block
                    best_waste = usable

            block = block->next_in_bucket
            # Advance tortoise x2 (cycle detection)
            if tortoise: tortoise = tortoise->next
            if tortoise: tortoise = tortoise->next

        if best_block: break

    if best_block == NULL: return NULL
```

**Search strategy**: Best-fit within ascending bucket scan. Prefers exact match; otherwise smallest sufficient block. Floyd's tortoise detects linked-list corruption.

## Block Allocation Phase

### 1. Mark allocated

```
block->flags_stamp |= 0x10                        # set ALLOCATED bit
block->flags_stamp = (block->flags_stamp & 0x3F)
                   | (this->alloc_stamp << 6)      # stamp generation
                   | 0x10
```

### 2. Unlink from free list

Doubly-linked via `+0x10` (left_child / prev) and `+0x14` (next_in_bucket):

```
if block->left_child:
    block->left_child->next = block->next
else:
    bucket_head[size_class] = block->next       # was list head

if block->next:
    block->next->left_child = block->left_child
```

### 3. Alignment split (front remnant)

Triggered when `(block + 16) & (alignment - 1) != 0` (user data not aligned).

```
aligned_addr = (block + alignment) & ~(alignment - 1)
front_size   = aligned_addr - block - 16

if prev_physical_block exists AND front_size == 16:
    # Coalesce: grow prev block by 16
    prev->data_size += 16
else:
    # Shrink original block into front free remnant
    block->data_size = front_size - 16
    block->flags &= 0x2F                       # clear ALLOCATED + stamp
    insert_into_bucket(block)                   # re-insert into free list

# New block header at aligned_addr - 16
new_block = aligned_addr - 16
new_block->prev_physical = block (or prev)
new_block->data_size = block_end - aligned_addr
new_block->flags = stamp | 0x10

# Update next physical neighbor's back-pointer
if next_block < heap_end AND next_block != NULL:
    next_block->prev_physical = new_block

this->used_bytes     += 16     # header overhead
this->largest_free   -= 16
```

### 4. Remainder split (tail remnant)

Triggered when `block->data_size > aligned_size + 16`:

```
tail = block + aligned_size + 16
tail->self_ptr       = tail                     # sentinel
tail->prev_physical  = block
tail->data_size      = block->data_size - aligned_size - 16
tail->flags          = 0
insert_into_bucket(tail)

# Update next physical neighbor
if tail_end < heap_end:
    tail_next->prev_physical = tail

block->data_size = aligned_size

this->used_bytes   += 16
this->largest_free -= 16
```

### 5. Finalize

```
this->used_bytes   += block->data_size
this->largest_free -= block->data_size

# Memory category tagging (from TLS)
tls_category = PPC_LOAD_U32(TLS_base + 1696)        # r13+0 -> TLS, then +1696
block->flags = (block->flags & 0xFFFFFFF0) | (tls_category & 0xF)
# block header low nibble = current thread's memory category

# Per-category stat update
cat = tls_category & 0xF
stat_idx = cat + 22
this->stat_array[stat_idx] += block->data_size       # @ this + stat_idx * 4

# Pattern-fill (memset)
sub_82847160(block + 16, block->data_size)

return block + 16
```

### insert_into_bucket helper (inline)

```
bucket_idx = min(block->data_size >> 4, 15)
slot_offset = (bucket_idx + 3) * 4

block->next_in_bucket = this->bucket[bucket_idx]     # point to old head
if old_head:
    old_head->left_child = block
block->left_child = NULL
this->bucket[bucket_idx] = block                      # new head
```

## Sub-calls

| Address | Name | Purpose |
|-|-|-|
| 0x822BCA90 | sub_822BCA90 | Debug assert -- stripped to `blr` (no-op) |
| 0x82847160 | sub_82847160 | Pattern memset -- fills user region (delegates to `rexcrt_memset`) |

## Blocking Analysis

**sub_82848750 itself**: Non-blocking. Pure pointer arithmetic and guest memory reads/writes. No kernel calls, no mutexes, no syscalls.

**Parent (sub_828493E0)** provides external synchronization:
- `sub_8285FF50` = `RtlEnterCriticalSection` (lock) before calling sub_82848750
- `sub_8285FFA0` = `RtlLeaveCriticalSection` (unlock) after

## Large Path vs Small Path

Parent `sub_828493E0` dispatches:

| Condition | Path |
|-|-|
| `small_alloc_flag` (this+0xC3) set AND size <= 64 AND alignment <= 16 | **Small**: `sub_82871828` using fixed pools at this+204/212/220/228 |
| Any of: size > 64, alignment > 16, flag clear | **Large**: `sub_82848750` using 16-bucket free-list best-fit |

## TLS Fields (Thread-Local via r13)

| TLS Offset | Purpose |
|-|-|
| r13+0 | Pointer to KTHREAD/thread data block |
| thread_data+1684 | Debug validation flag (checked; handler is no-op) |
| thread_data+1696 | Memory category tag (low 4 bits used for block tagging + stat routing) |

## Key Constants

| Constant | Value |
|-|-|
| MIN_ALIGNMENT | 16 |
| BLOCK_HEADER_SIZE | 16 bytes |
| NUM_BUCKETS | 16 |
| ALLOCATED_FLAG | 0x10 (bit 4) |
| FLAGS_MASK | 0x2F (preserve bits 0-3,5 when clearing) |
| STAMP_SHIFT | 6 bits left |
| SIZE_GRANULARITY | 16 bytes |
| MAX_BUCKETED_SIZE | bucket 15 catches all >= 240 bytes |

## Exhaustion Behavior

When no block fits across all 16 buckets: returns NULL. **No kernel fallback, no VirtualAlloc, no heap growth**. The caller (`sub_828493E0` or `sub_82871868`) must handle NULL.

## Data Structure Summary

This is a **segregated free-list allocator** with:
- 16 size-class buckets (each a singly-linked list with back-pointer for O(1) removal)
- Best-fit search within each bucket, ascending bucket scan
- Physical neighbor tracking for coalescing during alignment splits
- Self-pointer sentinel for corruption detection
- Floyd's cycle detection during list traversal
- External critical section for thread safety
- Generation stamp in block headers for debugging
