# Critical Section 0x83192960 Contention Map

## Overview

CS 0x83192960 is the single lock protecting the game's primary slab allocator (alloc, free, query).
It is acquired by 3 functions, all called via vtable dispatch (indirect call), meaning
any thread performing memory allocation contends on this one lock.

## Direct Lock Holders

### sub_828493E0 -- Allocate

- **Arguments**: r3 = allocator object, r4 = size, r5 = alignment
- **Lock pattern**: Acquires CS 0x83192960, dispatches to inner allocator, releases
- **Inner dispatch**:
  - If `size <= 64 && alignment <= 16`: calls `sub_82871828` (small block alloc)
    - Walks a linked list of slab nodes: `node[0]=capacity, [4]=next, [8]=free_count, [12]=free_head`
    - Loop at 0x82871850: `while (node && node.free_count == 0) node = node.next`
    - If no free block found: calls `sub_82848750` to allocate new 16KB slab
    - After slab alloc: initializes free list, fills slab with blocks, links to slab chain
    - **Worst case**: O(n) in number of slab nodes, PLUS slab allocation via NtAllocateVirtualMemory
  - If `size > 64 || alignment > 16`: calls `sub_82848750` directly (large block path)
    - Allocates from the page allocator (sub_82847160), which does NOT acquire any lock
    - **Worst case**: syscall latency from NtAllocateVirtualMemory
- **Hold duration estimate**: LONG -- free list walk O(n) + possible syscall

### sub_828494D8 -- Free

- **Arguments**: r3 = allocator object, r4 = ptr
- **Lock pattern**: Acquires CS 0x83192960, dispatches to free routine, releases
- **Inner dispatch**:
  - Checks page bitmap to determine small vs large block
  - Small: `sub_82871758` -- adds block back to slab's free list (O(1))
  - Large: `sub_82848B68` -- returns pages to page allocator (O(1))
- **Hold duration estimate**: SHORT -- bitmap lookup + list prepend

### sub_82849580 -- Query/Stats

- **Arguments**: r3 = allocator object
- **Lock pattern**: Acquires CS 0x83192960, calls sub_82849020, releases
- **Inner work**: Read-only stats query
- **Hold duration estimate**: VERY SHORT -- register-level reads

## Lock Mechanism

The lock is acquired via `sub_8285FF50` (scoped lock enter) and released via `sub_8285FFA0` (scoped lock leave).

**sub_8285FF50** (ScopedLock::Enter):
- r3 = stack-allocated ScopedLock object (8 bytes: [0]=refcount, [4]=cs_ptr)
- r4 = pointer to CRITICAL_SECTION
- Sets refcount = 1, stores cs_ptr
- If CS is initialized (first dword != 0): calls RtlEnterCriticalSection

**sub_8285FFA0** (ScopedLock::Leave):
- Decrements refcount; if reaches 0: calls RtlLeaveCriticalSection on stored cs_ptr

## Neighboring Allocator Locks

The allocator system uses multiple critical sections. None are nested under 0x83192960.

| CS Address | Offset from 0x8319293C | Functions | Role |
|-|-|-|-|
| 0x8319293C | +0x00 | 6 | Page table / bitmap operations |
| 0x83192960 | +0x24 | 3 | **Main slab allocator (THIS LOCK)** |
| 0x83192FD0 | +0x694 | 5 | Virtual memory manager |
| 0x831AB5E8 | -- | 1 | Extended allocator A |
| 0x831AB61C | -- | 1 | Extended allocator B |
| 0x831AB63C | -- | 2 | Extended allocator C |
| 0x831AB970 | -- | 1 | Extended allocator D |

### CS 0x8319293C -- Page Table Lock (6 functions)

- sub_82847C80: calls sub_82870E80 (page table walk)
- sub_82847D48: calls sub_82871110, sub_82870F88 (page coalesce)
- sub_82847EC0: calls sub_82871110 (page bitmap update)
- sub_82847F68: calls sub_82871168 (page range operation, ~134 lines)
- sub_82848280: simple bitmap query (~91 lines)
- sub_82848328: simple bitmap query (~89 lines)

### CS 0x83192FD0 -- Virtual Memory Lock (5 functions)

- sub_8284C298: simple query (~56 lines)
- sub_8284C828: calls sub_8284C720 (~55 lines)
- sub_8284C888: calls sub_82A01248 (~199 lines, longest)
- sub_8284CB80: calls sub_8284EEC0, sub_82855460 (~199 lines)
- sub_8284CE38: calls sub_82849918, sub_828470E0 (~199 lines)

## Lock Ordering

No nested locking between these three CSes was observed:
- sub_82848750 (called under 0x83192960) does NOT acquire 0x8319293C
- sub_82847160 (page alloc, called from sub_82848750) acquires NO locks
- The three CS groups (0x8319293C, 0x83192960, 0x83192FD0) are independent

## Callers and Thread Mapping

All 3 direct lock holders are called via **vtable dispatch** (function pointer stored in allocator object).
Zero direct call sites exist in generated code -- the allocator interface is fully virtual.

The game's malloc equivalent (`sub_8218BE28`) reads TLS[r13+1676] for allocator context,
then dispatches `allocator->alloc(size, alignment)` which resolves to sub_828493E0.

### Threads that allocate:

| Thread | Allocator Usage | Contention Risk |
|-|-|-|
| Main thread | All game logic, UI, scene management | HIGH (constant) |
| Render thread | GPU resource creation, shader alloc | HIGH (per-frame) |
| Streaming thread | Asset loading, decompression buffers | HIGH (burst) |
| Audio thread | Sound buffer allocation | MEDIUM |
| Physics thread | Collision data, ragdoll buffers | MEDIUM |
| Background workers | Misc async tasks | LOW-MEDIUM |

## Worst Contention Scenario

1. **Streaming thread** calls alloc for a large decompression buffer
   - Enters sub_828493E0, acquires CS 0x83192960
   - Falls through to sub_82848750 (large alloc) -> NtAllocateVirtualMemory
   - Holds lock for ~100us+ during syscall

2. **Main thread** calls alloc for game object
   - Blocked on CS 0x83192960 (streaming thread holds it)

3. **Render thread** calls alloc for GPU upload buffer
   - Also blocked on CS 0x83192960

4. **Audio thread** calls free for completed sound buffer
   - Also blocked -- even O(1) free operations are serialized

**Result**: Convoy effect. All threads serialize behind one slow allocation.
The free list walk in sub_82871828 makes this worse: with many slabs, the O(n)
scan under lock grows linearly with heap fragmentation.

## Other Major Lock Groups (for context)

These are NOT part of the allocator but represent other contention points:

| CS Address | Functions | Subsystem |
|-|-|-|
| 0x82FB56A0 | 13 | Resource manager pool A |
| 0x82FB56C0 | 10 | Resource manager pool B |
| 0x82FB56E0 | 6 | Resource manager pool C |
| 0x82FB5700 | 15 | Resource manager pool D |
| 0x831CC91C | 25 | File system / streaming I/O |
| 0x8307F820 | 2 | Streaming buffer A |
| 0x8307F840 | 5 | Streaming buffer B |
| 0x831D4C8C | 3 | Network subsystem |

Total scoped lock (sub_8285FF50) usage: **419 acquisitions** across **321 functions** in **30 files**.
Total direct RtlEnterCriticalSection usage: **82 calls** across **6 files**.

## Complete Function List: CS 0x83192960

| Function | Role | Hold Duration | Inner Calls |
|-|-|-|-|
| sub_828493E0 | alloc(size, align) | LONG: list walk + slab | sub_82871828, sub_82848750 |
| sub_828494D8 | free(ptr) | SHORT: bitmap + list add | sub_82871758, sub_82848B68 |
| sub_82849580 | query_stats() | VERY SHORT: reads only | sub_82849020 |

## Complete Function List: CS 0x8319293C (Page Table)

| Function | Role | Hold Duration | Inner Calls |
|-|-|-|-|
| sub_82847C80 | page walk | MEDIUM: ~114 lines | sub_82870E80 |
| sub_82847D48 | page coalesce | MEDIUM: ~92 lines | sub_82871110, sub_82870F88 |
| sub_82847EC0 | bitmap update | MEDIUM: ~95 lines | sub_82871110 |
| sub_82847F68 | page range op | LONG: ~134 lines | sub_82871168 |
| sub_82848280 | bitmap query | SHORT: ~91 lines | -- |
| sub_82848328 | bitmap query | SHORT: ~89 lines | -- |

## Complete Function List: CS 0x83192FD0 (Virtual Memory)

| Function | Role | Hold Duration | Inner Calls |
|-|-|-|-|
| sub_8284C298 | VM query | SHORT: ~56 lines | -- |
| sub_8284C828 | VM lookup | SHORT: ~55 lines | sub_8284C720 |
| sub_8284C888 | VM operation | LONG: ~199 lines | sub_82A01248 |
| sub_8284CB80 | VM operation | LONG: ~199 lines | sub_8284EEC0, sub_82855460 |
| sub_8284CE38 | VM operation | LONG: ~199 lines | sub_82849918, sub_828470E0 |

## Allocator Initialization

- **Constructor**: sub_8284DA58 (allocator struct init)
  - Calls sub_8285FE48 (RtlInitializeCriticalSection wrapper) on CS at `(allocator_base + 4 + 512)`
  - Calls sub_82849778 to create the page pool
  - Calls sub_82849A50 to set up virtual address space
  - 36 total calls to sub_82849778 across codebase (multiple allocator instances)
  - 14 total calls to sub_82849A50

- **CS init wrapper**: sub_8285FE48
  - Simple wrapper: saves r3, calls RtlInitializeCriticalSection, restores r3
  - 63 total calls across codebase (each creates a new critical section)

## Native Rewrite Recommendations

### Problem
Single global lock serializes ALL allocation/free operations across ALL threads.
The O(n) free list walk in sub_82871828 under lock creates unbounded hold times.
On PC with 8+ cores, this is much worse than on Xbox 360's 6 hardware threads.

### Solution Architecture

1. **Per-size-class pools with per-pool locks**
   - Replace single CS with one lock per size class (8, 16, 32, 64 bytes)
   - Each size class has its own slab chain and free list
   - Dramatically reduces contention: threads allocating different sizes never contend

2. **Lock-free free lists for small blocks**
   - Use CAS-based Treiber stacks for the free list within each slab
   - Alloc: `do { head = slab.free; } while (!CAS(&slab.free, head, head->next))`
   - Free: `do { head = slab.free; block->next = head; } while (!CAS(&slab.free, head, block))`
   - Eliminates lock entirely for the common case (block available in current slab)

3. **Thread-local allocation caches (TLS magazines)**
   - Each thread maintains a small cache of pre-allocated blocks per size class
   - Alloc from local cache (no lock). Refill from global pool (lock, but rare).
   - Free to local cache (no lock). Flush to global pool when full.
   - This is how modern allocators (jemalloc, mimalloc, tcmalloc) work.

4. **Separate large-allocation lock**
   - Large allocs (>64 bytes) already bypass the free list
   - Give them their own lock or use the OS allocator directly
   - These are infrequent enough that a simple lock suffices

5. **Read-write lock for stats query**
   - sub_82849580 only reads -- use a shared/exclusive lock
   - Or make stats atomic (relaxed loads) so no lock needed

### Minimal Viable Fix (least invasive)
Hook sub_828493E0, sub_828494D8, sub_82849580 with native implementations
that use `std::mutex` per size class (4 mutexes instead of 1 CS).
Keep the same slab-based allocation logic but split by size class.
Estimated contention reduction: 4x for typical workloads.

### Optimal Fix
Replace the entire allocator with a modern thread-scalable allocator
(mimalloc or jemalloc) via PPC_FUNC_HOOK on the 3 vtable entry points.
This eliminates the convoy effect entirely and improves cache locality.
