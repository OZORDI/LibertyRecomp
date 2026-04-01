# BaseHeap::Protect "Uncommitted Page" Failure Analysis

## Symptom

Thread `t41614048` produces 1.5 million paired log messages at guest address
`0x705D0000`:

```
[ERROR] BaseHeap::Protect failed due to uncommitted page
[WARN]  Stack guard page hit at guest 0x705D0000 — expanded stack
```

The messages are contradictory: Protect reports failure, yet the handler claims
the stack was "expanded." The handler ignores the Protect return value (Bug 1
from doc 39).

---

## 1. The Code That Produces "Uncommitted Page"

**File**: `glue/rexglue-sdk-main/src/system/xmemory.cpp`, lines 1246-1260

```cpp
// Ensure all pages are in the same reserved region and all are committed.
uint32_t first_base_address = UINT_MAX;
for (uint32_t page_number = start_page_number; page_number <= end_page_number; ++page_number) {
  auto page_entry = page_table_[page_number];
  if (first_base_address == UINT_MAX) {
    first_base_address = page_entry.base_address;
  } else if (first_base_address != page_entry.base_address) {
    REXSYS_ERROR("BaseHeap::Protect failed due to request spanning regions");
    return false;
  }
  if (!(page_entry.state & memory::kMemoryAllocationCommit)) {   // <-- LINE 1256
    REXSYS_ERROR("BaseHeap::Protect failed due to uncommitted page");
    return false;
  }
}
```

The check is a bitwise AND of the page's `state` field against the
`kMemoryAllocationCommit` flag (bit 1, value 2). If the bit is not set, Protect
refuses to proceed. This is correct behavior: it mirrors the Windows
`VirtualProtect` API contract which states "The access protection value can be
set only on committed pages."

---

## 2. What "Committed" vs "Reserved" vs "Free" Means

The page table entry `state` field is a 2-bit bitmask (`xmemory.h:97`):

| State bits | Meaning | How it gets here |
|------------|---------|-----------------|
| `0` (0b00) | **Free** -- page was never allocated, or was released | Default, or after `Release()` sets `page_entry.qword = 0` |
| `1` (0b01) | **Reserved only** -- address space reserved, no backing memory | `AllocFixed` or `AllocRange` with `kMemoryAllocationReserve` only |
| `3` (0b11) | **Reserved + Committed** -- address space reserved AND physical memory committed | `AllocFixed`/`AllocRange` with `kMemoryAllocationReserve \| kMemoryAllocationCommit` |
| `2` (0b10) | **Committed only** -- theoretically possible but never produced by current code | N/A |

Constants from `xmemory.h:48-49`:
```cpp
kMemoryAllocationReserve = 1 << 0,  // bit 0 = 1
kMemoryAllocationCommit  = 1 << 1,  // bit 1 = 2
```

Key lifecycle transitions:

- **AllocFixed/AllocRange** (line 984/1122): Sets state to `kMemoryAllocationReserve | allocation_type`. If both Reserve+Commit flags are passed, state becomes `0b11` (3). This is the normal path for stack allocation.

- **Decommit** (line 1152): Clears ONLY the commit bit: `page_entry.state &= ~kMemoryAllocationCommit`. State goes from `0b11` to `0b01` (reserved-only). The page remains reserved but is no longer committed.

- **Release** (line 1206): Zeros the entire 64-bit qword: `page_entry.qword = 0`. State becomes `0b00` (free). All metadata (base_address, region_page_count, protections) is also zeroed.

For `0x705D0000` with `state == 0`, the page is **free** -- it was either never
allocated or was released. The commit bit check at line 1256 correctly rejects
it because you cannot change protection on memory that does not exist.

---

## 3. Protect Method's Full Logic Flow

`BaseHeap::Protect(address, size, protect, old_protect*)` at line 1212:

```
Step 1: Size validation
  - Zero size → fail ("zero size")

Step 2: Page range calculation
  - start_page = (address - heap_base_) / page_size_
  - end_page = (address + size - 1 - heap_base_) / page_size_
  - Either out of bounds → fail ("out-of-bounds")

Step 3: Acquire global lock

Step 4: Page validation loop (lines 1248-1260)
  For each page in [start_page, end_page]:
    a) Check all pages share the same base_address (same allocation region)
       - Mismatch → fail ("request spanning regions")
    b) Check page has kMemoryAllocationCommit bit set
       - Missing → fail ("uncommitted page")         ← THIS IS THE FAILURE

Step 5: Host mprotect call (lines 1264-1282)
  - Only if guest page size matches host page granularity
  - Calls rex::memory::Protect → mprotect on the host mapping
  - Failure → fail ("host VirtualProtect failure")
  - Page sizes not aligned → fail ("not 4k page aligned")

Step 6: Update page table protection bits (lines 1285-1288)
  For each page: page_entry.current_protect = protect

Step 7: Return true
```

The method reaches step 4b and exits immediately on the first page with
`state == 0`. It never reaches the host mprotect call. The page's protection
is unchanged (remains PROT_NONE on the host side if that was the last host-level
setting, or possibly never had any host-level mapping at all).

---

## 4. Does Protect Commit Memory as Part of Stack Expansion?

**No.** `BaseHeap::Protect` ONLY changes protection bits on already-committed
pages. It has no ability to commit memory. The relevant code paths that commit
memory are:

| Method | Can Commit? | Mechanism |
|--------|------------|-----------|
| `BaseHeap::Protect` | **No** | Only changes protection flags |
| `BaseHeap::AllocFixed` | **Yes** | Calls `rex::memory::AllocFixed` with `kCommit` |
| `BaseHeap::AllocRange` | **Yes** | Same as AllocFixed after finding free space |
| `BaseHeap::Decommit` | **Decommits** | Clears commit bit (host decommit is TODO/no-op) |
| `BaseHeap::Release` | **Frees** | Zeros page table entry entirely |

This is the fundamental design gap: the stack guard page handler at line 457
calls `Protect()` expecting it to "expand the stack," but Protect cannot do
that. On a real Xbox 360 kernel (and Windows), the stack guard page fault
handler does TWO things:

1. **Commits** the faulting page (transitions reserved → committed)
2. **Changes protection** to read-write

The rexglue handler only attempts step 2 and has no code for step 1. Since the
faulting page at `0x705D0000` was never allocated at all (state=0, not even
reserved), even adding a commit step would require an allocate-then-commit
sequence.

### What Would Be Needed

To properly handle a guard page fault on an uncommitted-but-reserved page:
```cpp
// Pseudocode for proper stack expansion
if (!(page_entry.state & kMemoryAllocationCommit)) {
    // Page is reserved but not committed -- commit it first
    heap->AllocFixed(page_addr, page_size, alignment,
                     kMemoryAllocationCommit,
                     kMemoryProtectRead | kMemoryProtectWrite);
}
// Then the Protect call would succeed since the page is now committed
```

But for `0x705D0000` where `state == 0` (not even reserved), there is nothing
to commit. The address is simply not part of any thread's stack allocation.

---

## 5. Max Commit Size or Limits

There is **no explicit max commit size** in `BaseHeap::Protect`. The only
limits are:

1. **Page table bounds** (lines 1231-1241): The address + size must fall within
   the heap's page table. For `heaps_.v40000000`, this means within
   `[0x40000000, 0x7F000000)`.

2. **Region spanning** (lines 1248-1255): All pages must belong to the same
   allocation (same `base_address`). You cannot Protect across two separate
   AllocRange results.

3. **Host page alignment** (lines 1265-1282): The guest page range must align
   to host page boundaries. Since `page_size_` is 64KB and macOS page size is
   16KB, this is always satisfied (64KB is a multiple of 16KB).

4. **Committed state** (lines 1256-1259): Every page must have the commit bit.
   This is the failing check.

The heap itself has no per-allocation or per-commit size cap. The total virtual
address space for `v40000000` is 0x3F000000 (1,008 MB), and any amount of that
can be committed.

---

## 6. Relationship Between Protect and "Expanded Stack"

The "expanded stack" log message is produced by the **caller** of Protect, not
by Protect itself. The call chain:

```
AccessViolationCallback (xmemory.cpp:436)
  └→ if address in stack range (line 451-453)
       └→ heap->Protect(page_addr, page_size, RW)    ← line 457-458
       └→ REXSYS_WARN("... expanded stack")          ← line 459 (UNCONDITIONAL)
       └→ return true                                 ← line 460 (UNCONDITIONAL)
```

The "expanded stack" message is logged on EVERY call regardless of whether
Protect succeeded. The handler does not check the boolean return from
`heap->Protect()`. This means:

- **Protect succeeds** (page was committed + guard-protected): Message is
  accurate. The guard page was made writable, stack effectively grew.

- **Protect fails** (page uncommitted, wrong region, etc.): Message is a lie.
  Nothing was expanded. The page is still inaccessible. The fault will
  immediately recur.

The two messages always appear as a pair because:
1. Protect fires first and logs the ERROR
2. The handler logs the WARN regardless
3. The handler returns true
4. The signal handler resumes execution
5. The CPU re-faults → repeat from step 1

---

## 7. Why 0x705D0000 Specifically

### Heap Geometry

Heap `v40000000`:
- `heap_base_` = `0x40000000`
- `heap_size_` = `0x3F000000` (1,008 MB)
- `page_size_` = `0x10000` (64 KB)
- Page table has 0x3F00 entries (16,128 pages)

Address `0x705D0000`:
- Page number: `(0x705D0000 - 0x40000000) / 0x10000 = 0x305D` (12,381)
- Falls within stack range `[0x70000000, 0x7F000000)`
- Stack pages start at page `0x3000` (page for `0x70000000`)

### Stack Allocation Pattern

`XThread::AllocateStack` (xthread.cpp:224-253) allocates stacks bottom-up
within the range using `AllocRange(kStackAddressRangeBegin, kStackAddressRangeEnd, ...)`.
Each stack is `actual_size = requested_size + 2 * page_size` bytes. For a
typical 64KB stack: `actual_size = 64KB + 128KB = 192KB = 3 pages`.

The first stack starts at `0x70000000` (page 0x3000).
Address `0x705D0000` is page `0x305D`, which is 93 pages (0x5D) into the stack
region. With 3-page stacks, this could be in the range of stack #31.

But if `page_table_[0x305D].state == 0`, this page is in a gap between stacks,
or belongs to a thread whose stack was freed by `FreeStack()` (which calls
`Release()`, zeroing all page entries).

### Three Possible Scenarios

**A. Freed thread stack**: A game thread completed and called `FreeStack()`,
which calls `heap->Release(stack_alloc_base_)`, setting all pages in that
allocation to `state=0`. But something (recompiled code with a stale stack
pointer, or a fiber/coroutine resume) still references the old stack address.

**B. Gap between allocations**: `AllocRange` with bottom-up allocation may
leave gaps due to alignment constraints. A corrupted stack pointer could land
in such a gap.

**C. Stack never allocated at this address**: The thread's actual stack is
elsewhere, and a wild pointer or buffer overrun caused a write to `0x705D0000`.

---

## 8. Comparison with Xenia Reference

The Xenia source at `tools/xenia-master-1/src/xenia/memory.cc` has an identical
`BaseHeap::Protect` implementation (lines 1160-1236). Xenia does NOT have the
stack guard page handler at all -- there is no equivalent of the
`AccessViolationCallback` code at xmemory.cpp:448-461 in upstream Xenia. That
handler was added specifically for LibertyRecomp/rexglue.

Xenia handles stack guard faults at the kernel level through its `ExCreateThread`
and thread scheduling infrastructure, which is not present in rexglue.

---

## Summary of Findings

| Question | Answer |
|----------|--------|
| What produces the error? | `xmemory.cpp:1256-1258` -- bitwise check of `page_entry.state` against `kMemoryAllocationCommit` (bit 1) |
| What does "uncommitted" mean? | Page state lacks the commit bit. State=0 means free (never allocated or released). State=1 means reserved-only (address space held but no backing memory). |
| When does Protect fail vs succeed? | Fails if: zero size, out of bounds, pages span regions, any page uncommitted, host mprotect fails, page alignment mismatch. Succeeds only when ALL pages are committed and in the same region. |
| Should Protect commit memory? | **No** -- Protect only changes permission bits. Committing is done by `AllocFixed`/`AllocRange`. The guard handler needs to commit BEFORE calling Protect, but `0x705D0000` is free (not even reserved), so there is nothing to commit. |
| Is there a max commit limit? | No explicit limit. Bounded only by heap address space (1,008 MB for v40000000) and host memory. |
| Relationship to "expanded stack"? | The "expanded stack" message is logged unconditionally by the guard handler (line 459) regardless of Protect's return value. It is a false positive when Protect fails. |

## Key Source Files

| File | Lines | Role |
|------|-------|------|
| `glue/rexglue-sdk-main/src/system/xmemory.cpp` | 1212-1291 | `BaseHeap::Protect` -- the full implementation |
| `glue/rexglue-sdk-main/src/system/xmemory.cpp` | 1246-1260 | The validation loop with the "uncommitted page" check |
| `glue/rexglue-sdk-main/src/system/xmemory.cpp` | 436-461 | `AccessViolationCallback` -- the guard page handler |
| `glue/rexglue-sdk-main/src/system/xmemory.cpp` | 916-988 | `BaseHeap::AllocFixed` -- commits pages (sets state bits) |
| `glue/rexglue-sdk-main/src/system/xmemory.cpp` | 1129-1156 | `BaseHeap::Decommit` -- clears commit bit |
| `glue/rexglue-sdk-main/src/system/xmemory.cpp` | 1158-1210 | `BaseHeap::Release` -- zeros entire page entry |
| `glue/rexglue-sdk-main/src/system/xthread.cpp` | 224-253 | `XThread::AllocateStack` -- stack layout and guard setup |
| `glue/rexglue-sdk-main/src/system/xthread.cpp` | 255-265 | `XThread::FreeStack` -- releases stack via `Release()` |
| `glue/rexglue-sdk-main/include/rex/system/xmemory.h` | 48-49 | `kMemoryAllocationReserve` / `kMemoryAllocationCommit` constants |
| `glue/rexglue-sdk-main/include/rex/system/xmemory.h` | 82-100 | `PageEntry` union -- state is 2-bit field |
| `glue/rexglue-sdk-main/include/rex/system/xthread.h` | 150-151 | `kStackAddressRange{Begin,End}` = `0x70000000` / `0x7F000000` |
| `tools/xenia-master-1/src/xenia/memory.cc` | 1160-1236 | Xenia reference -- identical Protect, no guard handler |
