# Guest Address 0x705D0000 -- Complete Investigation

## Summary

Guest address 0x705D0000 is an **uncommitted page** in the Xbox 360 guest thread
stack region. The 1.5M fault loop on thread t41614048 is caused by the stack guard
page handler returning "handled" after `BaseHeap::Protect` fails, causing the CPU
to retry the same faulting store instruction forever.

---

## 1. What 0x705D0000 Represents

### Memory Map

The Xbox 360 guest address space is managed by multiple heaps. Address 0x705D0000
falls in the **v40000000 heap** (`xmemory.cpp:166-167`):

```
Heap:       heaps_.v40000000
Range:      0x40000000 - 0x7EFFFFFF (1008 MB)
Page size:  64 KB (0x10000)
Type:       kGuestVirtual
```

`LookupHeap()` routes any address in [0x40000000, 0x7F000000) to this heap
(`xmemory.cpp:316-317`).

### Stack Address Range

Thread stacks are allocated within a sub-range of this heap (`xthread.h:150-151`):

```cpp
static constexpr uint32_t kStackAddressRangeBegin = 0x70000000;
static constexpr uint32_t kStackAddressRangeEnd = 0x7F000000;
```

So 0x705D0000 is **5.8 MB into the 240 MB stack region**.

### Why 0x705D0000 is Uncommitted

Thread stacks are allocated bottom-up starting at 0x70000000 using `AllocRange()`.
Each stack allocation is `size + 128KB` (two 64KB guard pages). With typical stack
sizes, about 20-30 thread stacks fill the range up to roughly 0x705C0000. Address
0x705D0000 falls **beyond the last committed allocation** -- it was never allocated
by any `XThread::AllocateStack()` or `MmCreateKernelStack()` call. Its page table
entry has state=0 (no `kMemoryAllocationCommit` flag).

---

## 2. How Guest Thread Stacks Are Allocated

### Path A: XThread::AllocateStack (`xthread.cpp:224-253`)

Used for user threads created via `ExCreateThread`:

```
padding = page_size * 2 = 128 KB
actual_size = round_up(requested_size, 64KB) + padding

Layout (low address to high address):
  alloc_base                 [64 KB guard: PROT_NONE]
  alloc_base + 0x10000       stack_limit_ (lowest usable byte)
  ...                        stack grows DOWNWARD
  stack_base_ - 1            highest usable byte
  stack_base_                [64 KB guard: PROT_NONE]
```

Both guard pages are **committed** via `AllocRange` with
`kMemoryAllocationReserve | kMemoryAllocationCommit`, then set to
`kMemoryProtectNoAccess` via `Protect()`.

### Path B: MmCreateKernelStack (`xboxkrnl_memory.cpp:599-611`)

Used for kernel stacks:

```cpp
heap->AllocRange(0x70000000, 0x7F000000, stack_size_aligned, stack_alignment,
                 kMemoryAllocationReserve | kMemoryAllocationCommit,
                 kMemoryProtectRead | kMemoryProtectWrite, false, &stack_address);
```

**No guard pages are set up.** The entire allocation is RW-committed. Overflow
goes directly into unallocated heap space.

### Default Stack Sizes

| Source | Size |
|--------|------|
| XThread minimum | 16 KB |
| ExCreateThread minimum (after alignment) | 16 KB |
| Kernel stack | 16 KB |
| ELF module fallback | 1 MB |
| Host thread | 16 MB |

---

## 3. How BaseHeap::Protect Works and Why It Fails

**File**: `xmemory.cpp:1212-1291`

`Protect()` changes page permissions within a committed region. It iterates the
page table to verify prerequisites before calling the host `mprotect`:

```cpp
for (page_number = start_page_number; page_number <= end_page_number; ++page_number) {
    if (first_base_address != page_entry.base_address) {
        // FAIL: "request spanning regions"
    }
    if (!(page_entry.state & kMemoryAllocationCommit)) {
        // FAIL: "uncommitted page"       <-- THIS IS THE FAILURE
    }
}
```

The "uncommitted page" error fires at line 1256-1257 when the page table entry's
`state` field lacks the `kMemoryAllocationCommit` bit. This happens when:

1. The page was **never allocated** (state=0) -- the case for 0x705D0000
2. The page was allocated with `kMemoryAllocationReserve` only (no commit)
3. The page was **decommitted** via `Decommit()` (state loses commit bit)
4. The page was **released** via `Release()` (state reset to 0)

For 0x705D0000, it is case (1): the page was never part of any allocation.

---

## 4. The Stack Guard Page Expansion Mechanism

**File**: `xmemory.cpp:448-461`

This handler was added in commit b595515d to mimic Xbox 360 kernel stack growth.
On the real Xbox 360, the kernel commits additional stack pages on demand when the
guard page is hit. The RexGlue implementation:

```cpp
bool Memory::AccessViolationCallback(..., void* host_address, bool is_write) {
    uint32_t virtual_address = HostToGuestVirtual(host_address);
    BaseHeap* heap = LookupHeap(virtual_address);

    // Handle stack guard page faults
    if (heap->heap_type() == HeapType::kGuestVirtual &&
        virtual_address >= XThread::kStackAddressRangeBegin &&
        virtual_address < XThread::kStackAddressRangeEnd) {
        auto page_size = heap->page_size();
        uint32_t page_addr = virtual_address & ~(page_size - 1);
        heap->Protect(page_addr, page_size, kMemoryProtectRead | kMemoryProtectWrite);
        REXSYS_WARN("Stack guard page hit at guest 0x{:08X}", virtual_address);
        return true;   // <-- ALWAYS returns true
    }
    ...
}
```

### Three Bugs

**Bug 1: Protect() return value is ignored.** If `Protect()` fails (returns false),
the page stays PROT_NONE, but the handler returns `true` claiming success. The CPU
retries the faulting instruction, which re-faults on the same address. Infinite loop.

**Bug 2: No validation that the page is actually a guard page.** The handler treats
ANY fault in [0x70000000, 0x7F000000) as a guard page hit. This includes:
- Free/unallocated pages between stack allocations (state=0)
- Pages beyond the last stack allocation (0x705D0000)
- Pages from freed thread stacks

All of these cause `Protect()` to fail on the uncommitted check.

**Bug 3: No new guard page placed after expansion.** Real Windows/Xbox kernels:
(1) make the faulting guard accessible, (2) place a new guard one page below,
(3) raise `STATUS_STACK_OVERFLOW` if no room for a new guard. The RexGlue handler
only does step 1 (when it succeeds).

---

## 5. The Infinite Loop -- Complete Signal Chain

```
1. Thread t41614048 executes a store to guest 0x705D0000
2. Host page is PROT_NONE (never committed) -> macOS delivers SIGBUS
3. ExceptionHandlerCallback (exception_handler_posix.cpp:56)
   -> On macOS ARM64: hardcodes kWrite for SIGBUS (line 155-160)
   -> Calls InitializeAccessViolation()
   -> Iterates registered handlers[]
4. MMIOHandler::ExceptionCallback (mmio_handler.cpp:364)
   -> Fault address within virtual_membase_ range? Yes
   -> MMIO range match? No (stack addresses aren't MMIO)
   -> Acquires lock, checks host page protection: PROT_NONE
   -> Calls access_violation_callback_() (line 412-413)
5. Memory::AccessViolationCallback (xmemory.cpp:436)
   -> Heap type is kGuestVirtual? Yes
   -> Address in [0x70000000, 0x7F000000)? Yes
   -> Calls heap->Protect(0x705D0000, 0x10000, RW)
6. BaseHeap::Protect (xmemory.cpp:1212)
   -> Page table lookup: page_entry.state = 0
   -> !(0 & kMemoryAllocationCommit) = true
   -> REXSYS_ERROR("BaseHeap::Protect failed due to uncommitted page")
   -> Returns false
7. AccessViolationCallback ignores the false return
   -> Logs "Stack guard page hit at guest 0x705D0000"
   -> Returns true
8. Signal handler restores thread context, returns from signal
9. CPU re-executes the same store -> SIGBUS -> goto step 2
```

Each iteration takes ~40 microseconds (25K faults/second).

---

## 6. What 0x705D0000 is NOT

- **Not a valid stack guard page**: Guard pages are committed with
  `kMemoryAllocationCommit` then set to `kMemoryProtectNoAccess`. Page 0x705D0000
  was never committed at all (state=0).

- **Not within any thread's stack allocation**: The 56 successful guard page
  expansions from 0x70000000 to 0x705CFFFF correspond to actual guard pages. At
  0x705D0000, allocations end.

- **Not game code**: The 0x70xxxxxx range is pure data (thread stacks). Game code
  lives at 0x82000000-0x83300000.

- **Not the end of stack space**: Only 5.8 MB of the 240 MB stack region is used.
  The issue is a linear memory scan walking past the end of allocated stacks.

---

## 7. What is Accessing 0x705D0000?

The fault pattern shows 56 sequential guard page hits walking upward from
0x70000000 in ~0x10000-0x30000 increments, all on one host thread. This is NOT
normal stack growth (which would hit one guard page per thread). Something is
**linearly scanning or zeroing the stack memory region** until it hits the first
uncommitted page.

Likely candidates:
1. A memory initialization routine zeroing all allocated stack pages
2. A vtable dispatcher or scan routine walking memory regions
3. World loading code (`sub_821D01B0` was the crash site in pre-handler logs)
   interpreting stack-range memory as data

---

## 8. Fix Requirements

The handler at `xmemory.cpp:448-461` needs:

1. **Check Protect() return value**: Return `false` on failure so the fault
   propagates to the crash handler instead of looping.

2. **Validate page state**: Before calling `Protect()`, check that the page is
   actually committed with `kMemoryProtectNoAccess` (a real guard page). If the
   page is unallocated (state=0), return `false` immediately.

3. **Place a new guard page**: After successful expansion, set the next page below
   to `kMemoryProtectNoAccess`. If no room, raise stack overflow.

4. **Investigate the linear scan**: Determine what guest code is walking the entire
   stack range. This is the root cause -- the guard page handler is just the
   mechanism that converts a crash into an infinite loop.

---

## Key Source Files

| File | Lines | Role |
|------|-------|------|
| `glue/rexglue-sdk-main/include/rex/system/xthread.h` | 150-151 | `kStackAddressRangeBegin/End` constants |
| `glue/rexglue-sdk-main/src/system/xthread.cpp` | 224-253 | `AllocateStack()` -- guard page setup |
| `glue/rexglue-sdk-main/src/system/xmemory.cpp` | 166-167 | v40000000 heap init (64KB pages, kGuestVirtual) |
| `glue/rexglue-sdk-main/src/system/xmemory.cpp` | 313-333 | `LookupHeap()` -- routes 0x70xxxxxx to v40000000 |
| `glue/rexglue-sdk-main/src/system/xmemory.cpp` | 448-461 | Stack guard handler (the buggy code) |
| `glue/rexglue-sdk-main/src/system/xmemory.cpp` | 1212-1291 | `BaseHeap::Protect()` -- fails on uncommitted pages |
| `glue/rexglue-sdk-main/src/system/mmio_handler.cpp` | 364-416 | `ExceptionCallback` -- dispatches to AV callback |
| `glue/rexglue-sdk-main/src/core/exception_handler_posix.cpp` | 144-260 | SIGBUS signal handler (macOS ARM64 kWrite hardcode) |
| `glue/rexglue-sdk-main/src/kernel/xboxkrnl/xboxkrnl_memory.cpp` | 599-611 | `MmCreateKernelStack` (no guard pages) |
| `LibertyRecomp/main.cpp` | 187-196 | Exception handler installation + comment |
