# Multi-Allocator Dispatcher: sub_828475F8

## Overview

`sub_828475F8` is vtable[2] (Alloc) of the **rage::MultiAllocator** class (vtable `0x8208480C`). It dispatches allocation requests to one of up to 8 child sub-allocators based on the `flags` argument. This is the top-level allocator stored in TLS[1676] and called by `sub_8218BE28` (the game's malloc equivalent).

---

## Allocator Class Hierarchy

```
rage::Allocator (vtable 0x82000FA4)
  Abstract base. Set by sub_82847060. ~26 virtual methods.
  |
  +-- rage::MultiAllocator (vtable 0x8208480C)
  |     40 bytes. Holds array of up to 8 sub-allocator pointers.
  |     Alloc dispatches by flags index.
  |     Free scans all children for ownership.
  |     Constructors: sub_828475B0, sub_828475C8, sub_82847BA8
  |
  +-- rage::PoolAllocator (vtable 0x820848CC)
  |     Fixed-block-size pool allocator with intrusive free list.
  |     Constructors: sub_82847C08, sub_82847C70, sub_82848220
  |
  +-- rage::BuddyAllocator (vtable 0x82084AE4)
        ~2300 bytes. 16-bucket free lists + 16KB-page bitmap.
        Fast path for small allocs (<=64 bytes, <=16 alignment).
        Constructors: sub_828495C8, sub_82849690
```

---

## rage::MultiAllocator Object Layout

Size: **0x28 (40 bytes)**

| Offset|Size|Field|
|-|-|
|+0x00|4|vtable ptr (`0x8208480C`)|
|+0x04|4|sub_allocator[0] ptr (used when flags=0)|
|+0x08|4|sub_allocator[1] ptr (used when flags=1)|
|+0x0C|4|sub_allocator[2] ptr (used when flags=2)|
|+0x10|4|sub_allocator[3] ptr (used when flags=3)|
|+0x14|4|sub_allocator[4] ptr (used when flags=4)|
|+0x18|4|sub_allocator[5] ptr (used when flags=5)|
|+0x1C|4|sub_allocator[6] ptr (used when flags=6)|
|+0x20|4|sub_allocator[7] ptr (used when flags=7)|
|+0x24|4|count (int32, number of active sub-allocators)|

---

## Dispatch Algorithm (sub_828475F8 = Alloc)

PPC calling convention: `r3=this, r4=size, r5=alignment, r6=flags`

```
sub_alloc = *(uint32_t*)(this + (flags + 1) * 4)   // this->sub_allocators[flags]
tail_call sub_alloc->vtable[2](sub_alloc, size, alignment, flags)
```

Step-by-step PPC:
1. `r11 = flags + 1`
2. `r11 = r11 << 2` (rlwinm: rotate left 2, mask 0xFFFFFFFC)
3. `r3 = *(r11 + r3)` — loads sub-allocator pointer from the array
4. `r11 = *(r3 + 0)` — loads sub-allocator's vtable
5. `r11 = *(r11 + 8)` — vtable[2] = the sub-allocator's Alloc function
6. Tail-calls via `bctr`

**No bounds check on flags.** If flags >= count, reads garbage from the object.

Python verification:
```python
for flags in range(8):
    offset = (flags + 1) * 4
    # flags=0 -> this+0x04, flags=1 -> this+0x08, ..., flags=7 -> this+0x20
```

---

## Free Algorithm (sub_82847618)

```
if (ptr == NULL) return;

// 1. Ownership scan
for (i = 0; i < this->count; i++) {
    sub = this->sub_allocators[i];
    if (sub->vtable[18](sub, ptr))  // Owns(ptr)
        goto found;
}
// No owner: error fallback
sub_822BCA90(error_string, ptr);
return;

found:
// 2. TLS check for special override
if (TLS[1688] == 0 || TLS[1704] != 0) {
    // Check persistent allocator indices
    if (i == 1 || i == 3) return;  // NO-OP: never free from these
}

// 3. Actual free
sub = this->sub_allocators[i];
sub->vtable[3](sub, ptr);  // Free(ptr)
```

**Indices 1 and 3 are "persistent" allocators** — Free is suppressed for memory owned by these sub-allocators under normal conditions. This is typical for RAGE's level/streaming memory that gets bulk-freed on level transitions. TLS[1688]/TLS[1704] can override this behavior (e.g., during level teardown).

---

## MultiAllocator Vtable (0x8208480C) Function Map

| Slot|Offset|Function|Description|
|-|-|-|-|
|[0]|+0x00|?|destructor / base reset|
|[1]|+0x04|?|destructor variant|
|[2]|+0x08|sub_828475F8|**Alloc(size, align, flags)** — dispatch by flags index|
|[3]|+0x0C|sub_82847618|**Free(ptr)** — ownership scan → dispatch to sub vtable[3]|
|[4]|+0x10|sub_828476F8|**GetSize(ptr)** — ownership scan → sub vtable[6]@24|
|[5]|+0x14|sub_82847790|ownership scan → sub vtable[24]@96|
|[6]|+0x18|sub_82847820|ownership scan → sub vtable[25]@100|
|[7]|+0x1C|sub_82847948|**GetUsedMemory(flags)** — sum all unique subs via sub vtable[7]@28|
|[8]|+0x20|sub_828479C8|**GetTotalMemory()** — sum all unique subs via sub vtable[8]@32|
|[9]|+0x24|sub_82847A40|delegate sub[0] → sub vtable[9]@36 (GetName?)|
|[10]|+0x28|sub_82847A58|SetDebugLevel — stores to global var|
|[11]|+0x2C|sub_82847B38|delegate sub[0] → sub vtable[11]@44|
|[12]|+0x30|sub_82847B50|delegate sub[0] → sub vtable[12]@48|
|[13]|+0x34|sub_82847B68|delegate sub[0] → sub vtable[13]@52|
|[14]|+0x38|sub_82847B80|delegate sub[0] → sub vtable[14]@56|
|[15]|+0x3C|sub_828478B0|ownership scan → sub vtable[19]@76 (Realloc?)|
|[16]|+0x40|?|unknown|
|[17]|+0x44|sub_82847A68|**WalkAll()** — iterate all subs, call sub vtable[17]@68|
|[18]|+0x48|sub_82847AC0|**Owns(ptr)** — ownership scan via sub vtable[18]@72|

### Dispatch patterns

**Direct dispatch (by flags):** vtable[2] (Alloc) only — uses `this->sub_allocators[flags]` directly.

**Ownership scan then dispatch:** vtable[3,4,5,6,15,18] — iterates all sub-allocators calling `sub->Owns(ptr)` (vtable[18]@72) to find the owner, then dispatches to the relevant method on the owning sub-allocator.

**Aggregate (sum all):** vtable[7,8] — iterates all unique sub-allocators, calls the method on each, sums results. Skips adjacent duplicate sub-allocator pointers.

**Delegate to sub[0]:** vtable[9,11,12,13,14] — always forwards to the first sub-allocator.

**Iterate all (no return):** vtable[17] — calls method on every sub-allocator.

---

## Non-Virtual Helper Functions

| Function|Description|
|-|-|
|sub_828475D8|**AddSubAllocator(sub_alloc_ptr)** — appends to array, increments count|
|sub_82847B98|**GetSubAllocator(index)** — returns `this[(index+1)*4]`|

---

## rage::BuddyAllocator Object Layout (vtable 0x82084AE4)

Size: **~2296 bytes (0x8F8)**

| Offset|Size|Field|
|-|-|-|
|+0x000|4|vtable ptr (`0x82084AE4`)|
|+0x004|4|base memory address (managed region start)|
|+0x008|4|parent/buddy allocator ptr|
|+0x010|4|block_alignment|
|+0x014|4|block_table_ptr|
|+0x048|60|free list bucket array (16 buckets at +0x48..+0x84)|
|+0x04C|4|managed region size|
|+0x054|4|overflow free list|
|+0x098|4|max allocation size|
|+0x0BC|4|allocated block count (this+188)|
|+0x0C2|1|is_initialized flag|
|+0x0C3|1|has_fast_path flag|
|+0x0C8|4|current_usage (this+200)|
|+0x0CC|4|fast_free_list[0].head (size<=8)|
|+0x0D0|2|fast_free_list[0].bucket_size = 8|
|+0x0D2|2|fast_free_list[0].max_count = 2044|
|+0x0D4|4|fast_free_list[1].head (size<=16)|
|+0x0D8|2|fast_free_list[1].bucket_size = 16|
|+0x0DA|2|fast_free_list[1].max_count = 1022|
|+0x0DC|4|fast_free_list[2].head (size<=32)|
|+0x0E0|2|fast_free_list[2].bucket_size = 32|
|+0x0E2|2|fast_free_list[2].max_count = 511|
|+0x0E4|4|fast_free_list[3].head (size<=64)|
|+0x0E8|2|fast_free_list[3].bucket_size = 64|
|+0x0EA|2|fast_free_list[3].max_count = 255|
|+0x0EC|4|bitmap region base address|
|+0x0F0|2052|bitmap/pool area (memset 0 on init)|
|+0x8F4|1|flags byte (bit 7 = initialized)|

### BuddyAllocator Alloc Fast Path (sub_828493E0)

Conditions for fast path:
1. `this[195]` (has_fast_path) is nonzero
2. Global at `0x81F07070` equals -1
3. `size <= 64`
4. `alignment <= 16`

Bucket selection:
- size <= 8: use `this+204` (bucket 0, max 2044 entries)
- size <= 16: use `this+212` (bucket 1, max 1022 entries)
- size <= 32: use `this+220` (bucket 2, max 511 entries)
- size <= 64: use `this+228` (bucket 3, max 255 entries)

Calls `sub_82871828(bucket_head_ptr, this)` to pop from free list.
Falls back to `sub_82848750` (general alloc) if fast path fails or conditions unmet.

### BuddyAllocator General Alloc (sub_82848750)

- Minimum alignment forced to 16 bytes
- Size rounded up to 16-byte granularity: `aligned_size = (size + 15) & ~0xF`
- Checks against max alloc size at `this+152`
- Bucket index = `aligned_size / 16`, capped at 15
- Searches bucket free lists at `this+72..this+132`

### BuddyAllocator Free (sub_828494D8)

1. Reads base address from `this+236`
2. Computes page offset: `offset = (ptr - base) >> 14` (16KB pages)
3. Bit index: `offset & 0x1F`
4. Word index: `(offset >> 5) + 60`
5. Checks bitmap: `this[word_index * 4] & (1 << bit_index)`
6. If bitmap bit set AND ptr is 16KB-aligned → `sub_82871758` (fast free)
7. Otherwise → `sub_82848B68` (general free with coalescing)

### BuddyAllocator Owns (sub_828483E0)

```c
bool Owns(ptr) {
    base = this[4];
    size = this[76];
    return (ptr >= base + 16) && (ptr < base + size);
}
```

### BuddyAllocator Other Functions

| Function|Description|
|-|-|
|sub_828486A8|GetManagedSize() — returns `this[76]`|
|sub_82848410|GetMaxAllocSize() — returns `this[152]`|
|sub_82848700|GetLargestFreeBlock() — scans bucket lists backward for max block|
|sub_828486B0|Destructor (resets vtable, frees parent if initialized)|

---

## rage::PoolAllocator Object Layout (vtable 0x820848CC)

| Offset|Size|Field|
|-|-|-|
|+0x00|4|vtable ptr (`0x820848CC`)|
|+0x04|4|base memory ptr|
|+0x08|4|block header size / alignment|
|+0x0C|4|total usable size (count * block_size)|
|+0x10|4|block_size|
|+0x14|4|free_list_head|

Constructors: `sub_82847C08` (full init), `sub_82847C70` (reset), `sub_82848220` (with registration).
Uses `sub_82871228` to initialize intrusive free list through the memory region.

---

## Runtime State at TLS[1676]

```
TLS[r13+0] → thread_data_block
thread_data_block[1676] = 0x82B29500  (MultiAllocator)

MultiAllocator @ 0x82B29500:
  +0x00: vtable = 0x8208480C
  +0x04: sub_alloc[0] = 0x82B295EC  (BuddyAllocator, vtable 0x82084AE4)
  +0x24: count (unknown, >= 1)

BuddyAllocator @ 0x82B295EC:
  +0x00: vtable = 0x82084AE4
  Offset from MultiAllocator: 0xEC (236 bytes)
  (MultiAllocator is 0x28 bytes, so 0xC4 bytes of intervening data/padding)
```

### Call Chain

```
sub_8218BE28 (game malloc)
  → reads TLS[1676] = 0x82B29500 (MultiAllocator)
  → calls vtable[2] = sub_828475F8 (MultiAllocator::Alloc)
    → selects sub_alloc[flags] = 0x82B295EC (BuddyAllocator)
    → tail-calls vtable[2] = sub_828493E0 (BuddyAllocator::Alloc)
      → fast path (sub_82871828) or general path (sub_82848750)
```

---

## Related TLS Offsets

| TLS Offset|Purpose|
|-|-|
|1676|Current allocator (MultiAllocator ptr)|
|1680|Secondary allocator ptr (used by sub_828470E0)|
|1684|Alloc debug tracking flag|
|1688|Free override flag (enables freeing from persistent allocators)|
|1704|Deferred-free list ptr (if set, skips index 1/3 protection)|

---

## Sub-Allocator Virtual Function Table (shared interface)

All sub-allocator classes implement this interface. Offsets used by MultiAllocator dispatchers:

| Offset|Slot|Called by Multi vtable|Purpose|
|-|-|-|-|
|+0x08|[2]|vtable[2] Alloc|Alloc(size, align, flags)|
|+0x0C|[3]|vtable[3] Free|Free(ptr)|
|+0x18|[6]|vtable[4] GetSize|GetSize(ptr)|
|+0x1C|[7]|vtable[7] GetUsedMemory|GetUsedMemory(flags)|
|+0x20|[8]|vtable[8] GetTotalMemory|GetTotalMemory()|
|+0x24|[9]|vtable[9] GetName|GetName()|
|+0x2C|[11]|vtable[11]|forwarded from multi[11]|
|+0x30|[12]|vtable[12]|forwarded from multi[12]|
|+0x34|[13]|vtable[13]|forwarded from multi[13]|
|+0x38|[14]|vtable[14]|forwarded from multi[14]|
|+0x44|[17]|vtable[17] Walk|Walk/Defragment|
|+0x48|[18]|vtable[18] Owns|Owns(ptr) → bool|
|+0x4C|[19]|vtable[15] Realloc?|called via ownership scan|
|+0x60|[24]|vtable[5]|called via ownership scan|
|+0x64|[25]|vtable[6]|called via ownership scan|
