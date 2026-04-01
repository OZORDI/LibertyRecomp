# Guard Handler Fix: Exact Changes and Implications

## The Bug

`xmemory.cpp:448-461` — the stack guard page fault handler returns `true` even
when `heap->Protect()` fails:

```cpp
// Current code (buggy)
if (heap->heap_type() == memory::HeapType::kGuestVirtual &&
    virtual_address >= system::XThread::kStackAddressRangeBegin &&
    virtual_address < system::XThread::kStackAddressRangeEnd) {
  auto page_size = heap->page_size();
  uint32_t page_addr = virtual_address & ~(page_size - 1);
  heap->Protect(page_addr, page_size,
                memory::kMemoryProtectRead | memory::kMemoryProtectWrite);
  REXSYS_WARN("Stack guard page hit at guest 0x{:08X} — expanded stack", virtual_address);
  return true;   // BUG: unconditional true
}
```

When Protect fails (uncommitted page, freed stack, unallocated gap), the handler
tells the signal dispatcher "fault resolved." The CPU retries the same store
instruction, hits the same PROT_NONE page, and loops forever at ~25K faults/sec.

---

## Q1: What happens when AccessViolationCallback returns false?

The call chain on macOS:

```
SIGBUS → ExceptionHandlerCallback (exception_handler_posix.cpp:56)
  → handlers_[i].first(&ex, ...) (line 225)
    → MMIOHandler::ExceptionCallback (mmio_handler.cpp:364)
      → access_violation_callback_(...) (mmio_handler.cpp:413)
        → Memory::AccessViolationCallback (xmemory.cpp:436)
```

When `AccessViolationCallback` returns **false**:
1. `mmio_handler.cpp:416` returns false to ExceptionCallbackThunk
2. `exception_handler_posix.cpp:225` — the handler loop continues to the next
   registered handler (if any)
3. If NO handler returns true, the `ExceptionHandlerCallback` function returns
   without modifying the signal context
4. Since `SA_SIGINFO` was used without `SA_RESETHAND`, the signal handler itself
   returns, and the kernel re-delivers the signal
5. On the second delivery with no handler claiming it, the **default signal
   disposition** takes effect: SIGBUS terminates the process with a core dump

The process crashes. On macOS, this produces a crash report in
`~/Library/Logs/DiagnosticReports/` with a full native stack trace (but no
guest PPC context). The crash is useful for diagnosis but loses PPC state.

---

## Q2: On macOS, what signal catches the fault?

**SIGBUS** (not SIGSEGV). On macOS, writes to PROT_NONE pages backed by shared
memory mappings (which is how rexglue maps guest memory via `mmap` with
`MAP_SHARED`) deliver SIGBUS, not SIGSEGV.

The handler at `exception_handler_posix.cpp:147-160` has a dedicated macOS ARM64
SIGBUS path that hardcodes `kWrite` (because ESR is unavailable on macOS and
`IsArm64LoadPrefetchStore` can misclassify certain store variants).

If the callback returns false and the process crashes, macOS generates:
- A crash report with native ARM64 backtrace
- The `si_addr` field in the crash report shows the faulting guest address
  (translated to host virtual address)
- No PPC r1, guest PC, or PPC call stack — these exist only in guest memory
  and must be logged before the crash

---

## Q3: Should the fix just return false, or also log diagnostics?

**Both.** Returning false alone produces a macOS crash report with only native
ARM64 context. The PPC guest state is critical for debugging and is lost in a
bare crash. The fix should log before returning false:

### Minimum viable diagnostic

```cpp
if (!heap->Protect(page_addr, page_size,
                   memory::kMemoryProtectRead | memory::kMemoryProtectWrite)) {
  // Protect failed — page is not committed (freed stack, gap, or wild pointer)
  uint32_t page_state = 0;
  heap->QueryRegionInfo(page_addr, &page_state);  // or read page table directly

  REXSYS_ERROR(
      "Stack guard page fault UNHANDLED at guest 0x{:08X} "
      "(page 0x{:08X}, page_state=0x{:X})",
      virtual_address, page_addr, page_state);
  return false;
}
```

### Ideal diagnostic (adds PPC context)

Getting the PPC r1 (stack pointer) and guest PC requires access to the
PPCContext of the faulting thread. The AccessViolationCallback does not currently
receive the thread context. Options:

1. **Thread-local PPCContext**: If the faulting thread stores its PPCContext in a
   TLS slot (as recompiled code does), the handler could read it:
   ```cpp
   auto* ppc_ctx = GetCurrentPPCContext();  // TLS lookup
   if (ppc_ctx) {
     REXSYS_ERROR("  PPC r1=0x{:08X}, guest_pc=0x{:08X}",
                  ppc_ctx->r[1], ppc_ctx->current_pc);
   }
   ```

2. **Host backtrace**: `backtrace()` / `backtrace_symbols()` from the signal
   handler gives the native call stack, which can be symbolized to identify
   which recompiled PPC function was executing.

3. **Thread ID mapping**: Log the native thread ID so it can be correlated with
   XThread objects: `pthread_threadid_np(nullptr, &tid)`.

Recommendation: Start with option (1) + native thread ID. This gives the most
actionable information (which guest function, where its stack pointer was) with
minimal code.

---

## Q4: Are there other callers of this guard handler code path?

The guard handler at lines 448-461 is only reachable through one path:

```
ExceptionHandlerCallback
  → MMIOHandler::ExceptionCallback
    → access_violation_callback_ (= Memory::AccessViolationCallbackThunk)
      → Memory::AccessViolationCallback
        → [line 448-461: stack guard check]
```

There are no other callers. `AccessViolationCallback` is called exactly once
per access violation, from `mmio_handler.cpp:412-414`, after MMIO range lookup
fails and the race-condition recheck confirms the page is still protected.

No other code in the codebase calls `AccessViolationCallback` directly — it is
only invoked through the function pointer stored in `MMIOHandler`.

---

## Q5: Would it be better to COMMIT the page instead of just protecting it?

**Yes, in theory. No, for the known failure case.**

### The two scenarios

**Scenario A — Legitimate guard page (committed, PROT_NONE)**:
The page was allocated by `AllocateStack` with Reserve+Commit, then set to
`kMemoryProtectNoAccess`. State bits = `0b11` (reserved+committed). `Protect()`
succeeds because the page IS committed. The current code works for this case.
No commit needed.

**Scenario B — Unallocated page (state=0, or reserved-only)**:
The page at 0x705D0000 has `state=0` (free). `Protect()` fails because
`!(state & kMemoryAllocationCommit)`. The page was never allocated, or was freed
by `FreeStack() → Release()`.

For Scenario B, committing would mean:
```cpp
heap->AllocFixed(page_addr, page_size, page_size,
                 kMemoryAllocationReserve | kMemoryAllocationCommit,
                 kMemoryProtectRead | kMemoryProtectWrite);
```

This would allocate and commit a page that was NEVER part of any thread's stack.
This is **wrong** — it masks a real bug (use-after-free of a stack, wild
pointer, or stack overflow past the guard into unallocated territory). The
correct response is to crash with diagnostics.

### When commit-on-demand would be correct

A proper demand-paged stack (like real Windows/Xbox 360) would:
1. Reserve a large stack region (e.g., 1MB) but only commit a small portion
2. Place a guard page at the commit boundary
3. On guard fault: commit the guard page + one more page, move the guard down
4. If no more reserved space: raise STATUS_STACK_OVERFLOW

But rexglue's `AllocateStack` commits the ENTIRE stack upfront (line 234:
`kMemoryAllocationReserve | kMemoryAllocationCommit`). There are no
reserved-but-uncommitted pages to grow into. The guard pages are committed pages
with NoAccess protection. This is a simpler model that avoids demand-paging
entirely.

**Conclusion**: Committing on demand is architecturally correct for a real OS but
wrong for rexglue's current design where stacks are fully pre-committed. The fix
should return false, not commit.

---

## Q6: How pages get committed — AllocFixed/AllocRange

`BaseHeap::AllocFixed` (xmemory.cpp:916-988):
```
1. Round up size/alignment to page_size_
2. Calculate page range [start_page, end_page]
3. Acquire global lock
4. Validate: if reserving, pages must be free; if committing, pages must be reserved
5. If committing: call rex::memory::AllocFixed(host_addr, size, kCommit, access)
   - On macOS, this is mmap(MAP_FIXED) or mprotect to make pages accessible
6. Update page_table_ entries: state = kReserve | allocation_type
   - For Reserve+Commit: state = 0b11
```

`BaseHeap::AllocRange` (xmemory.cpp:990-1127):
```
1. Scan page table for a free region of sufficient size
2. Once found, same commit logic as AllocFixed
```

Both methods update the page table state bits, which is what `Protect()` checks.
The host-level commit (step 5) and the page-table-level state update (step 6)
happen atomically under the global lock.

---

## Q7: What does Xenia do for stack guard pages?

**Nothing.** Xenia does not have a stack guard page handler.

Xenia's `Memory::AccessViolationCallback` (memory.cc:440-466) handles only
`kGuestPhysical` heap faults (GPU write watches). The function returns false
for any fault in a non-physical heap, including the stack range.

Xenia's `XThread::AllocateStack` (xthread.cc:240-256) sets up guard pages
identically to rexglue:
```cpp
// Setup the guard pages
heap->Protect(stack_alloc_base_, padding / 2, kMemoryProtectNoAccess);
heap->Protect(stack_base_, padding / 2, kMemoryProtectNoAccess);
```

But Xenia runs guest code through its JIT (x64 backend), which handles memory
access differently. Stack overflows in Xenia result in process crashes (or are
caught by Xenia's structured exception handling on Windows via SEH).

Xenia's `MmCreateKernelStack` (xboxkrnl_memory.cc:624-638) allocates kernel
stacks with full commit, no guard pages at all:
```cpp
kernel_memory()->LookupHeap(0x70000000)
    ->AllocRange(0x70000000, 0x7F000000, stack_size_aligned, stack_alignment,
                 kMemoryAllocationReserve | kMemoryAllocationCommit,
                 kMemoryProtectRead | kMemoryProtectWrite, false, &stack_address);
```

**Key insight**: The stack guard handler at xmemory.cpp:448-461 is a
LibertyRecomp/rexglue addition, not from Xenia. Xenia never needed it because
its stacks are fully committed and stack overflows simply crash.

---

## Exact Fix

### Minimal correct fix (recommended first step)

Replace xmemory.cpp lines 448-461 with:

```cpp
  // Handle stack guard page faults: when a guest thread's stack grows past
  // its guard page, unprotect the page so the stack can expand — mimicking
  // Xbox 360 kernel stack growth behavior.
  if (heap->heap_type() == memory::HeapType::kGuestVirtual &&
      virtual_address >= system::XThread::kStackAddressRangeBegin &&
      virtual_address < system::XThread::kStackAddressRangeEnd) {
    auto page_size = heap->page_size();
    uint32_t page_addr = virtual_address & ~(page_size - 1);
    if (heap->Protect(page_addr, page_size,
                      memory::kMemoryProtectRead | memory::kMemoryProtectWrite)) {
      REXSYS_WARN("Stack guard page hit at guest 0x{:08X} — expanded stack",
                   virtual_address);
      return true;
    }
    // Protect failed: page is not committed (freed/unallocated stack memory).
    // Return false so the signal handler does not retry — this is a genuine
    // access violation (stack overflow, use-after-free, or wild pointer).
    REXSYS_ERROR(
        "Stack guard fault UNHANDLED at guest 0x{:08X} (page 0x{:08X}) — "
        "Protect() failed (page not committed). Crashing.",
        virtual_address, page_addr);
    return false;
  }
```

### Changes from current code

| Aspect | Before | After |
|--------|--------|-------|
| Protect return value | Ignored | Checked — controls true/false return |
| Return on Protect failure | `true` (lie) | `false` (honest — fault unhandled) |
| Log on failure | "expanded stack" (false) | "UNHANDLED ... Crashing" (accurate) |
| Log on success | Same WARN | Same WARN |
| Process behavior on failure | Infinite loop (25K faults/sec) | Clean crash with diagnostic |

### Implications of this fix

1. **The 0x705D0000 infinite loop stops.** The first fault returns false, the
   signal handler falls through, and macOS terminates the process with SIGBUS.

2. **Legitimate guard page hits still work.** When a thread hits its own
   committed guard page (state=0b11, protect=NoAccess), `Protect()` succeeds
   and the handler returns true. No behavioral change for the happy path.

3. **The process will crash where it previously looped.** This is correct
   behavior — a fault on unallocated memory IS a fatal error. The crash report
   will show where the bad access originated.

4. **No new guard page placement.** The fix does not add Bug 3 handling (placing
   a new guard page below the expanded one). This is acceptable for now because:
   - Stacks are fully pre-committed with fixed-size guards
   - A second guard hit on the same stack means true stack overflow
   - Adding guard page movement is a larger change best done separately

### Future enhancement (not needed for the immediate fix)

To add full PPC diagnostic context before crashing:

```cpp
    REXSYS_ERROR(
        "Stack guard fault UNHANDLED at guest 0x{:08X} (page 0x{:08X})",
        virtual_address, page_addr);
    // Log PPC context if available via TLS
    auto* ppc_ctx = /* TLS PPCContext lookup */;
    if (ppc_ctx) {
      REXSYS_ERROR("  PPC r1=0x{:08X} lr=0x{:08X} pc=0x{:08X}",
                   uint32_t(ppc_ctx->r[1]),
                   uint32_t(ppc_ctx->lr),
                   uint32_t(ppc_ctx->current_pc));
    }
    uint64_t tid = 0;
    pthread_threadid_np(nullptr, &tid);
    REXSYS_ERROR("  native thread ID: {}", tid);
    return false;
```

---

## Key Source Files

| File | Lines | Role |
|------|-------|------|
| `glue/rexglue-sdk-main/src/system/xmemory.cpp` | 448-461 | The buggy guard handler (fix target) |
| `glue/rexglue-sdk-main/src/system/xmemory.cpp` | 1212-1291 | `BaseHeap::Protect` — fails on uncommitted pages |
| `glue/rexglue-sdk-main/src/system/xmemory.cpp` | 916-988 | `BaseHeap::AllocFixed` — how pages get committed |
| `glue/rexglue-sdk-main/src/system/xthread.cpp` | 224-253 | `AllocateStack` — stacks fully pre-committed + guard setup |
| `glue/rexglue-sdk-main/src/system/mmio_handler.cpp` | 360-416 | `ExceptionCallback` — dispatches to AV callback |
| `glue/rexglue-sdk-main/src/core/exception_handler_posix.cpp` | 56-296 | SIGBUS/SIGSEGV signal handler (macOS) |
| `tools/xenia-master-1/src/xenia/memory.cc` | 440-466 | Xenia reference — no guard handler |
| `tools/xenia-master-1/src/xenia/kernel/xboxkrnl/xboxkrnl_memory.cc` | 624-638 | Xenia `MmCreateKernelStack` — no guards |
| `tools/xenia-master-1/src/xenia/kernel/xthread.cc` | 240-256 | Xenia guard page setup (identical layout) |
