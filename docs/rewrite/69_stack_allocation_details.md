# 69 - Stack Allocation Details and Guard Handler Expansion Analysis

## 1. How AllocateStack Works

**File**: `glue/rexglue-sdk-main/src/system/xthread.cpp` lines 224-253

```
AllocateStack(size):
  heap = LookupHeap(0x70000000)  // -> v40000000 heap (64KB pages)
  alignment = 64KB
  padding = 2 * 64KB = 128KB    // two guard pages
  size = round_up(size, 64KB)
  actual_size = size + 128KB

  AllocRange(0x70000000, 0x7F000000, actual_size, 64KB,
             RESERVE|COMMIT, READ|WRITE, top_down=false, &address)

  stack_alloc_base_ = address
  stack_alloc_size_ = actual_size
  stack_limit_      = address + 64KB       // bottom of usable stack
  stack_base_       = address + 64KB + size // top of usable stack (SP starts here)

  Fill(stack_alloc_base_, actual_size, 0xBE)

  // Guard pages:
  Protect(stack_alloc_base_, 64KB, NO_ACCESS)  // bottom guard
  Protect(stack_base_,       64KB, NO_ACCESS)  // top guard
```

**Uses AllocRange, NOT AllocFixed.** Bottom-up scan (`top_down=false`), starting from 0x70000000.

## 2. Main Thread Stack Size

**File**: `glue/rexglue-sdk-main/src/system/kernel_state.cpp` line 276

```cpp
constexpr uint32_t kMinStackSize = 32 * 1024 * 1024;  // 32 MB
uint32_t stack_size = std::max(module->stack_size(), kMinStackSize);
```

The main thread uses **32MB minimum**, not 16MB. The XEX header's `DEFAULT_STACK_SIZE` is overridden by `kMinStackSize`.

### Main Thread Layout (32MB stack)

| Range | Size | Content |
|-------|------|---------|
| 0x70000000 - 0x7000FFFF | 64KB | Bottom guard (NO_ACCESS) |
| 0x70010000 - 0x7200FFFF | 32MB | Usable stack (RW, filled with 0xBE) |
| 0x72010000 - 0x7201FFFF | 64KB | Top guard (NO_ACCESS) |

- `stack_alloc_base_` = 0x70000000
- `stack_limit_` = 0x70010000
- `stack_base_` = 0x72010000 (SP starts here, grows DOWN)
- Total committed: 32MB + 128KB = 0x02020000 bytes

## 3. The Guard Handler and Why It Expands Into Guards

**File**: `glue/rexglue-sdk-main/src/system/xmemory.cpp` lines 449-475

The guard handler intercepts ANY access violation in 0x70000000-0x7F000000 and calls `heap->Protect(page_addr, 64KB, READ|WRITE)`.

**Critical detail**: The bottom guard pages (0x70000000-0x7000FFFF) ARE committed in the page table -- `AllocRange` sets ALL pages in the allocation (including guard region) to `state = RESERVE|COMMIT`. The `Protect(NO_ACCESS)` call only changes `current_protect`, not `state`. So when the stack overflows into the bottom guard:

1. Fault at some address in 0x70000000-0x7000FFFF
2. Guard handler calls `heap->Protect()`
3. `Protect()` checks `page_entry.state & kMemoryAllocationCommit` -- **passes** (page IS committed)
4. `mprotect()` succeeds -- guard is consumed
5. Stack continues writing into what was the guard page
6. Log says "Stack guard page hit at guest 0x700xxxxx -- expanded stack"

**The guard page is consumed without stopping the stack.** This is by design for Xbox 360 stack growth emulation, but it means the bottom guard provides zero protection.

## 4. What Happens Below 0x70000000?

Once the stack consumes the bottom guard and reaches 0x6FFFFFFF:
- Address 0x6FFFFFFF is in the v40000000 heap (covers 0x40000000-0x7EFFFFFF)
- But it is BELOW `kStackAddressRangeBegin` (0x70000000)
- The guard handler's range check `virtual_address >= kStackAddressRangeBegin` **fails**
- Falls through to `heap->heap_type() != kGuestPhysical` check -- returns **false**
- POSIX signal handler gets no handler returning true
- On macOS: SIGBUS is re-delivered, infinite loop (or crash depending on handler logic)

But wait -- 0x6FFFFFFF might be part of the v40000000 shared memory mapping. On macOS, the entire 0x40000000-0x7EFFFFFF range is mmap'd as MAP_SHARED from the shm fd. This means the OS pages exist and are accessible at the mmap level. However, the page_table_ entries below the stack allocation have `state == 0`. So `Protect()` would fail on them even if the handler tried.

**But the handler doesn't try** -- the range check excludes addresses below 0x70000000.

## 5. Why 21.4MB of Stack Was Consumed

With a 32MB committed stack (0x70010000 to 0x7200FFFF), the SP starts at 0x72010000 and grows down. If the overflow fault is at 0x71590000:

- Stack consumed: 0x72010000 - 0x71590000 = **0xA80000 = 10.5MB**

If the "walked from 0x70000000" means the stack consumed all the way down to the bottom guard:

- Stack consumed: 0x72010000 - 0x70000000 = **32MB + 64KB** (entire allocation including guard)

The 21.4MB figure (0x71590000 - 0x70000000 = 0x1590000) represents: the address 0x71590000 is the fault point, and the stack has consumed from 0x72010000 down to 0x71590000 = 10.5MB of actual usage. The remaining 21.5MB from 0x70000000 to 0x71590000 is still 0xBE-filled committed memory that hasn't been touched yet.

**The fault at 0x71590000 is NOT a guard page hit -- it's inside committed RW memory.** Something else is causing the fault.

## 6. Could There Be Multiple Stacks Overlapping?

### Thread Stack Allocation Pattern

`AllocRange` scans bottom-up from 0x70000000. Each thread gets its own allocation:

- **Thread 1** (main): 0x70000000 - 0x7201FFFF (32MB + 128KB)
- **Thread 2** (first worker): 0x72020000 - 0x7203FFFF+ (depends on requested size)
- **Thread 3**, etc.: continues upward

Worker threads created via `ExCreateThread` use the XEX header's `DEFAULT_STACK_SIZE` (or game-requested size), rounded up to 16KB minimum, then aligned to 64KB pages. They do NOT get the 32MB minimum -- that's only for the main thread in `KernelState::LaunchModule`.

**Stacks cannot overlap** -- `AllocRange` checks every page in the requested range is free (state == 0) before allocating. Each stack is a distinct committed region with gaps only if the alignment requires it.

## 7. The v40000000 Heap Does NOT Commit All 240MB Upfront

The v40000000 heap is initialized as:
```cpp
heaps_.v40000000.Initialize(this, virtual_membase_, HeapType::kGuestVirtual,
                            0x40000000, 0x3F000000, 64 * 1024);
// base=0x40000000, size=0x3F000000 (1008MB), page_size=64KB
```

The heap is backed by the shared memory file mapping (shm_open + mmap MAP_SHARED). The entire 0x40000000-0x7EFFFFFF address range IS mapped at the OS level (pages exist in the shm fd). However:

- **page_table_ starts all-zero** (state=0 for every page)
- `AllocRange` with `RESERVE|COMMIT` calls `rex::memory::AllocFixed()` which on macOS does `mprotect(RW)` on the shm-mapped pages
- Only allocated regions have committed page_table_ entries
- The remaining ~975MB of the heap is mapped but has state=0 in the page table

**On macOS specifically**: the shared memory mapping means ALL pages in 0x40000000-0x7EFFFFFF are backed by the shm fd and technically accessible at the mmap level. The "commit" via `AllocFixed` just does `mprotect()` to change access permissions. Pages not explicitly allocated remain at whatever protection the shm mapping provides.

This is the dangerous part: **the shm mapping may give all pages in the range RW access by default** (MapFileView uses `PageAccess::kReadWrite`). If so, a stack overflow would silently write into "unallocated" heap space without faulting -- the OS sees the pages as valid shm-backed mappings.

## 8. Root Cause Hypothesis

The fault at 0x71590000 is inside a 32MB committed stack allocation. With 32MB available, the stack has only used ~10.5MB (from 0x72010000 down to 0x71590000). The fault is NOT a stack overflow.

Possible causes:
1. **The fault is a write to an invalid POINTER stored on the stack** (not the stack pointer itself). The stack frame at depth N contains a pointer to 0x71590000, and dereferencing that pointer faults.
2. **A different thread's stack is at 0x71590000** -- if a worker thread was allocated there and its guard page was hit.
3. **The 16MB figure in the user's premise may be stale** -- the current code uses 32MB (kMinStackSize was likely bumped from 16MB to 32MB, but the user may be testing an older build).

## 9. Summary of Key Facts

| Property | Value |
|----------|-------|
| Heap covering stacks | v40000000 (0x40000000-0x7EFFFFFF, 64KB pages) |
| Stack range | 0x70000000 - 0x7F000000 (240MB reserved range) |
| Allocation method | `AllocRange` (bottom-up scan) |
| Main thread stack | 32MB (kMinStackSize at kernel_state.cpp:276) |
| Worker thread stack | XEX header default or game-requested (typically 64KB-1MB) |
| Guard pages | 64KB bottom + 64KB top (committed but NO_ACCESS) |
| Guard pages committed? | YES -- Protect(NO_ACCESS) doesn't clear commit bit |
| Guard handler checks page_table_? | YES -- via heap->Protect() which checks commit bit |
| Guard pages expandable by handler? | YES -- they're committed, so Protect(RW) succeeds |
| Pages below allocation expandable? | NO -- state=0, Protect() fails |
| macOS shm mapping | Entire 1008MB range mapped, pages exist at OS level |
| AllocFixed on macOS (commit) | Just mprotect() on existing shm-mapped pages |
