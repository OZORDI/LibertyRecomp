# 73: On-Demand Page Commit for Stack Growth

## Question

Instead of pre-allocating a huge stack, could the guard handler commit new pages
on demand, giving the stack virtually unlimited growth (up to the 240MB heap)?

---

## 1. BaseHeap Architecture (xmemory.cpp)

Stacks live in the **v40000000 heap** (guest 0x40000000-0x7EFFFFFF, 64KB pages,
type `kGuestVirtual`). The stack address range is 0x70000000-0x7F000000 (240MB).

The heap is backed by a single shared-memory file mapping created at startup
(`Memory::Initialize`). The entire 4GB guest address space is mapped via
`shm_open`+`mmap` as PROT_NONE pages. "Committing" a page means calling
`mprotect` to make it accessible. "Reserving" just marks the page table entry.

### Page States (PageEntry::state, 2 bits)

| Value | Meaning |
|-------|---------|
| 0     | Free -- never allocated |
| 1     | Reserved only -- address space claimed but no physical backing |
| 3     | Reserved + Committed -- accessible memory |

### Current Stack Allocation (xthread.cpp:224-253)

```
AllocateStack(size):
  actual_size = size + 2 * page_size    // guard padding (2 x 64KB)
  heap->AllocRange(0x70000000, 0x7F000000, actual_size, alignment,
                   kReserve | kCommit, kProtectRead | kProtectWrite)
  heap->Protect(bottom_guard, 64KB, kProtectNoAccess)
  heap->Protect(top_guard, 64KB, kProtectNoAccess)
```

The entire stack is committed upfront. Guard pages are committed pages with
PROT_NONE protection. There are no reserved-but-uncommitted pages to grow into.

---

## 2. AllocFixed and AllocRange -- How Pages Get Committed

**`BaseHeap::AllocFixed`** (line 930-1002):

1. Acquires `global_critical_region_` (recursive mutex)
2. Validates page states: for commit, pages must be reserved; for reserve, pages
   must be free
3. Calls `rex::memory::AllocFixed(host_addr, size, kCommit, access)` -- on macOS
   this is `mprotect(addr, size, PROT_READ|PROT_WRITE)` since the shared-memory
   mapping already exists
4. Updates page_table_ entries: `state = kReserve | kCommit`, sets
   `allocation_protect` and `current_protect`

**`BaseHeap::AllocRange`** (line 1004+):

Same commit logic as AllocFixed, but first scans the page table to find a free
contiguous region.

### What Would It Take to Call AllocFixed from the Guard Handler?

The guard handler would need to:

```cpp
// In Memory::AccessViolationCallback, inside the stack range check:
heap->AllocFixed(page_addr, page_size, page_size,
                 kMemoryAllocationReserve | kMemoryAllocationCommit,
                 kMemoryProtectRead | kMemoryProtectWrite);
```

This would reserve+commit the faulting page, then return true. Mechanically
this is simple. The complications are about locking and signal safety.

---

## 3. Signal Safety Analysis

### The Call Chain

```
SIGBUS signal (macOS) or SIGSEGV (Linux)
  -> ExceptionHandlerCallback (signal handler context)
    -> MMIOHandler::ExceptionCallback
      -> global_critical_region_.Acquire()    // RECURSIVE MUTEX LOCKED
      -> access_violation_callback_(lock, ...)
        -> Memory::AccessViolationCallback
          -> heap->Protect() or heap->AllocFixed()  // WOULD RE-ACQUIRE MUTEX
```

### Can We Call AllocFixed from a Signal Handler?

**Short answer: It already works, with caveats.**

The existing guard handler already calls `heap->Protect()` from inside the
signal handler, and Protect also acquires the global critical region mutex.
This works because:

1. The MMIOHandler passes the `std::unique_lock` by move to the callback, so the
   lock is already held when `AccessViolationCallback` runs
2. `global_critical_region_` is a `std::recursive_mutex`, so re-locking from the
   same thread succeeds
3. On macOS, the actual host-level operation is just `mprotect()`, which IS
   async-signal-safe (POSIX guarantees `mprotect` is signal-safe)

**AllocFixed would work identically** because:

- It acquires the same recursive mutex (already held -- re-entrant is fine)
- On macOS with existing mapping, the host-level "commit" is `mprotect()` via
  `IsMacOSRangeAlreadyMapped` + `mprotect` path (memory_posix.cpp:250-258)
- `IsMacOSRangeAlreadyMapped` calls `vm_region_64` (Mach trap, signal-safe)
- Page table updates are just writes to `std::vector<PageEntry>` (no allocation,
  the vector was pre-sized at heap init)

**Risk**: `REXSYS_WARN` / `REXSYS_ERROR` logging may use `fmt::format` which
allocates heap memory. This is NOT async-signal-safe. However, the existing
handler already logs (`REXSYS_WARN("Stack guard page hit...")`), so this is a
pre-existing risk, not a new one.

### Verdict: AllocFixed IS safe to call from the guard handler

The only truly unsafe operations would be:
- `malloc`/`free` (heap allocation) -- but AllocFixed doesn't allocate
- File I/O -- but the commit path only does `mprotect`
- Non-recursive mutexes -- but `global_critical_region_` IS recursive

---

## 4. Design: Reserve-then-Commit-on-Demand

### Approach A: Reserve Large, Commit on Fault

Modify `AllocateStack` to reserve a large region but only commit a small
initial portion:

```cpp
bool XThread::AllocateStack(uint32_t size) {
  auto heap = memory()->LookupHeap(kStackAddressRangeBegin);
  auto alignment = heap->page_size();       // 64KB
  auto guard_size = heap->page_size();      // 64KB per guard

  // Reserve a large region (e.g., 64MB) but only commit 'size' bytes
  uint32_t reserve_size = 64 * 1024 * 1024; // 64MB max growth
  if (reserve_size < size) reserve_size = size;
  auto actual_reserve = reserve_size + guard_size * 2;

  uint32_t address = 0;
  // Step 1: RESERVE the full range (no physical backing)
  if (!heap->AllocRange(kStackAddressRangeBegin, kStackAddressRangeEnd,
                        actual_reserve, alignment,
                        kMemoryAllocationReserve,    // reserve only
                        kMemoryProtectNoAccess, false, &address))
    return false;

  // Step 2: COMMIT only the initial stack + guards
  uint32_t commit_base = address + actual_reserve - guard_size - size;
  heap->AllocFixed(commit_base, size + guard_size,  // stack + top guard
                   alignment,
                   kMemoryAllocationCommit,
                   kMemoryProtectRead | kMemoryProtectWrite);

  // Step 3: Protect guard pages
  heap->Protect(address, guard_size, kMemoryProtectNoAccess);           // bottom sentinel
  heap->Protect(address + actual_reserve - guard_size, guard_size,
                kMemoryProtectNoAccess);                                 // top guard

  stack_alloc_base_ = address;
  stack_alloc_size_ = actual_reserve;
  stack_limit_ = commit_base;             // current commit frontier
  stack_base_ = address + actual_reserve - guard_size;
  return true;
}
```

Then modify the guard handler to commit pages on demand:

```cpp
// In Memory::AccessViolationCallback, stack range check:
if (heap->heap_type() == HeapType::kGuestVirtual &&
    virtual_address >= XThread::kStackAddressRangeBegin &&
    virtual_address < XThread::kStackAddressRangeEnd) {

  auto page_size = heap->page_size();
  uint32_t page_addr = virtual_address & ~(page_size - 1);

  // First try: page is committed but PROT_NONE (existing guard page)
  if (heap->Protect(page_addr, page_size,
                    kMemoryProtectRead | kMemoryProtectWrite)) {
    REXSYS_WARN("Stack guard hit at 0x{:08X} -- expanded", virtual_address);
    return true;
  }

  // Second try: page is reserved but not committed -- commit it
  if (heap->AllocFixed(page_addr, page_size, page_size,
                       kMemoryAllocationCommit,
                       kMemoryProtectRead | kMemoryProtectWrite)) {
    REXSYS_WARN("Stack demand-commit at 0x{:08X} -- committed new page",
                virtual_address);
    return true;
  }

  // Page is not even reserved -- true stack overflow
  REXSYS_ERROR("FATAL: Stack overflow at 0x{:08X} -- page not reserved",
               virtual_address);
  std::abort();
}
```

### Approach A Limitations

- AllocFixed with `kMemoryAllocationCommit` requires `state & kReserve` (line
  958-964). If the page is in state 0 (free), AllocFixed will warn and
  auto-promote to reserve+commit. This actually works but produces a log warning.
- Each fault commits one 64KB page. On macOS the `mprotect` call is very fast
  (~1us). But a deep call stack that grows by 1MB would trigger 16 faults.
- The v40000000 heap is shared with other 64KB-page allocations (not just
  stacks). Reserving 64MB per thread could exhaust the address space if many
  threads are created.

---

## 5. Alternative: Pre-Commit a Larger Region

### Approach B: Just Allocate Bigger Stacks

The simplest fix: increase the stack allocation at creation time.

```cpp
// In AllocateStack, or in LaunchModule when setting stack_size:
if (creation_params_.stack_size < 32 * 1024 * 1024) {
  creation_params_.stack_size = 32 * 1024 * 1024;  // 32MB minimum
}
```

This commits 32MB + 128KB (guards) = ~32.1MB per thread. GTA IV creates ~6-8
guest threads, so total stack memory would be ~200-256MB, which fits within the
240MB stack range (0x70000000-0x7F000000).

**Pros**: Zero code changes to the guard handler. Zero signal-safety concerns.
Trivially correct. The memory is committed but not physically resident until
touched (macOS/Linux use demand paging at the host level).

**Cons**: Wastes virtual address space in the guest page table. Each thread
claims 32MB of the 240MB stack range. With 8 threads, that is 256MB -- nearly
the entire range. If any thread needs even more, it cannot grow.

### Host-Level Demand Paging

On both macOS and Linux, `mprotect(PROT_READ|PROT_WRITE)` on a shared-memory
mapping does NOT immediately allocate physical RAM. The host OS demand-pages the
memory as it is touched. So committing 32MB per thread in the guest allocator
only consumes address space, not physical memory. The host physical memory
footprint is determined by actual stack usage.

This means Approach B has no memory overhead penalty -- only address space
overhead.

---

## 6. Page State Requirements for Protect

`BaseHeap::Protect` (line 1226-1305) requires:

```cpp
if (!(page_entry.state & kMemoryAllocationCommit)) {
  REXSYS_ERROR("BaseHeap::Protect failed due to uncommitted page");
  return false;
}
```

So `state & kMemoryAllocationCommit` (bit 1) MUST be set. This bit is only set by:

1. `AllocFixed` with `allocation_type` including `kMemoryAllocationCommit`
2. `AllocRange` (same path)

You cannot set this flag without going through AllocFixed/AllocRange because the
page table is protected by the global critical region and there is no public API
to directly manipulate page state bits.

However, there is nothing stopping you from adding a lightweight method:

```cpp
bool BaseHeap::CommitPage(uint32_t address) {
  auto global_lock = global_critical_region_.Acquire();
  uint32_t page_number = (address - heap_base_) / page_size_;
  auto& entry = page_table_[page_number];
  if (entry.state & kMemoryAllocationCommit) return true;  // already committed
  if (!(entry.state & kMemoryAllocationReserve)) return false;  // not reserved

  void* host = TranslateRelative(page_number * page_size_);
  if (mprotect(host, page_size_, PROT_READ | PROT_WRITE) != 0) return false;

  entry.state |= kMemoryAllocationCommit;
  entry.current_protect = kMemoryProtectRead | kMemoryProtectWrite;
  return true;
}
```

This is essentially AllocFixed stripped to its minimum for the single-page commit
case, avoiding the size/alignment rounding and multi-page iteration.

---

## 7. Bypassing BaseHeap Tracking with Raw mmap/mprotect

### Could we call mprotect directly on the host address?

Yes. The guard handler already has the host address (from the signal info). We
could do:

```cpp
// In AccessViolationCallback:
void* host_page = (void*)((uintptr_t)host_address & ~0xFFFull);  // 4KB host page
mprotect(host_page, 4096, PROT_READ | PROT_WRITE);
return true;
```

This would make the page accessible at the host level without updating the guest
page table. The page would appear as state=0 (free) or state=1 (reserved-only)
in the guest page table but be physically accessible.

### Consequences of Bypassing the Page Table

| Operation | Impact |
|-----------|--------|
| `heap->Protect()` on same page later | Succeeds (mprotect works regardless of page table) |
| `heap->QueryRegionInfo()` | Reports wrong state (uncommitted) |
| `heap->Release()` covering this page | Calls `munmap` which works, but page table says it was never committed |
| Guest `NtQueryVirtualMemory` | Reports wrong state -- game might see stack as "free" |
| `DumpMap()` | Shows gap in committed memory |
| Another thread calling `AllocRange` | Could allocate over this "free" page -- data corruption |

**Verdict**: Bypassing the page table is dangerous. The page table is the source
of truth for the guest allocator. If another thread scans for free pages (e.g.,
to allocate a new stack), it could allocate over a silently committed page,
causing data corruption.

The only safe bypass would be if we can guarantee no other allocation will ever
overlap the stack range, but `AllocRange` scans the same page table for all
allocations in the v40000000 heap.

---

## 8. Recommendation

### For Immediate Fix: Approach B (Pre-Commit Larger Stacks)

Increase the default stack size to 16MB or 32MB. This is:
- Zero risk (no signal handler changes)
- Zero overhead (host demand-paging means physical RAM is only used as touched)
- Proven correct (the existing fully-committed model works; just more of it)

The only constraint is the 240MB stack address range (0x70000000-0x7F000000).
With 32MB per thread and 7 threads, that uses 224MB -- tight but workable.

### For Future Enhancement: Approach A (Reserve-then-Commit-on-Demand)

If stack space becomes scarce (more threads, or a thread needing >32MB):

1. Modify `AllocateStack` to reserve 64MB but commit only the initial size
2. Add `BaseHeap::CommitPage()` as a lightweight single-page commit method
3. Modify the guard handler to call `CommitPage()` on reserved-but-uncommitted
   pages

This is safe because:
- `mprotect` is async-signal-safe
- The recursive mutex is already held by the MMIOHandler caller
- Page table writes are plain memory stores (no allocation)
- The existing guard handler already performs equivalent operations (Protect)

### Do NOT bypass the page table (Approach 7)

Raw `mprotect` without page table updates creates inconsistency that can cause
data corruption through double-allocation. Always go through BaseHeap methods.

---

## Key Source Files

| File | Lines | Role |
|------|-------|------|
| `glue/rexglue-sdk-main/src/system/xmemory.cpp` | 930-1002 | `BaseHeap::AllocFixed` -- page commit logic |
| `glue/rexglue-sdk-main/src/system/xmemory.cpp` | 1004-1127 | `BaseHeap::AllocRange` -- free page scan + commit |
| `glue/rexglue-sdk-main/src/system/xmemory.cpp` | 1226-1305 | `BaseHeap::Protect` -- requires committed pages |
| `glue/rexglue-sdk-main/src/system/xmemory.cpp` | 437-489 | `AccessViolationCallback` -- guard handler |
| `glue/rexglue-sdk-main/src/system/xthread.cpp` | 224-253 | `AllocateStack` -- current full-commit model |
| `glue/rexglue-sdk-main/src/core/memory_posix.cpp` | 238-316 | `AllocFixed` (host) -- macOS mprotect path |
| `glue/rexglue-sdk-main/src/core/exception_handler_posix.cpp` | 56-296 | Signal handler (SIGBUS/SIGSEGV) |
| `glue/rexglue-sdk-main/src/system/mmio_handler.cpp` | 390-416 | MMIOHandler -- acquires lock, calls AV callback |
| `glue/rexglue-sdk-main/include/rex/system/xmemory.h` | 83-100 | `PageEntry` struct -- state bits |
| `glue/rexglue-sdk-main/include/rex/thread/mutex.h` | 56-83 | `global_critical_region` -- recursive mutex |
