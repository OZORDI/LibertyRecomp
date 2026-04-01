# Xbox 360 Stack Layout & RexGlue Stack Implementation

## 1. Xbox 360 Stack Memory Range

The Xbox 360 allocates thread stacks in the **0x70000000 - 0x7F000000** virtual address range.

RexGlue defines this in `include/rex/system/xthread.h`:
```cpp
static constexpr uint32_t kStackAddressRangeBegin = 0x70000000;
static constexpr uint32_t kStackAddressRangeEnd = 0x7F000000;
```

Total available stack space: **0x0F000000 = 240 MB**.

**Is 0x70000000 in the expected stack region?** Yes. 0x70000000 is the very first
byte of the stack address range. Any address from 0x70000000 to 0x7EFFFFFF is a
valid stack region address. Addresses in this range are served by the v40000000
heap (see section 2).

## 2. Heap That Owns the Stack Range

Stack addresses (0x70000000-0x7EFFFFFF) fall within the **v40000000 heap**, initialized at:
```cpp
heaps_.v40000000.Initialize(this, virtual_membase_, HeapType::kGuestVirtual,
                            0x40000000, 0x3F000000, 64 * 1024);
```

Key properties:
- Heap range: **0x40000000 - 0x7F000000** (size 0x3F000000 = 1008 MB)
- Page size: **64 KB (0x10000)**
- Heap type: `kGuestVirtual`

The `LookupHeap()` function in `xmemory.cpp:314-316` confirms addresses 0x40000000 to 0x7EFFFFFF route to this heap.

## 3. Xbox 360 Memory Map (Guest Address Space)

From `xmemory.cpp` map_info[] and heap initialization:

| Guest Address Range | Size | Heap | Page Size | Type |
|---------------------|------|------|-----------|------|
| 0x00000000 - 0x3FFFFFFF | 1 GB | v00000000 | 4 KB | Guest Virtual |
| 0x40000000 - 0x7EFFFFFF | ~1 GB | v40000000 | 64 KB | Guest Virtual |
| 0x7F000000 - 0x7FFFFFFF | 16 MB | (MMIO) | - | GPU registers, audio |
| 0x80000000 - 0x8FFFFFFF | 256 MB | v80000000 | 64 KB | Guest XEX |
| 0x90000000 - 0x9FFFFFFF | 256 MB | v90000000 | 4 KB | Guest XEX |
| 0xA0000000 - 0xBFFFFFFF | 512 MB | vA0000000 | 64 KB | Guest Physical |
| 0xC0000000 - 0xDFFFFFFF | 512 MB | vC0000000 | 16 MB | Guest Physical |
| 0xE0000000 - 0xFFCFFFFF | ~509 MB | vE0000000 | 4 KB | Guest Physical |

Stacks live at 0x70000000-0x7EFFFFFF, the upper portion of the v40000000 heap.
The host maps the entire 4 GB guest space + 512 MB physical into a contiguous
region starting at `virtual_membase_` (found at a power-of-2 address in 64-bit
host space, typically `1<<n` for n in 32..63).

## 4. Two Stack Allocation Paths

### Path A: XThread::AllocateStack (user threads via ExCreateThread)

**Source**: `xthread.cpp:224-253`

Layout for a stack of `size` bytes:
```
padding = page_size * 2 = 128 KB (two 64 KB guard pages)
actual_size = round_up(size, page_size) + padding

alloc_base  [guard: PROT_NONE, 64 KB]   <-- bottom guard page
stack_limit [usable stack, size bytes]   <-- lowest usable address
stack_base  [guard: PROT_NONE, 64 KB]   <-- top guard page
```

- SP (r1 in PPC) starts at `stack_base` (high address) and **grows downward**
- Bottom guard page at `alloc_base` to `alloc_base + 64KB` catches stack overflow
- Top guard page at `stack_base` to `stack_base + 64KB` catches underflow
- Entire region filled with 0xBE before guard protection is applied
- The entire region (stack + guards) is allocated with `kMemoryAllocationReserve | kMemoryAllocationCommit` -- all pages are committed upfront

### Path B: MmCreateKernelStack (kernel stacks)

**Source**: `xboxkrnl_memory.cpp:599-611`

```cpp
auto stack_size_aligned = (stack_size + 0xFFF) & 0xFFFFF000;
uint32_t stack_alignment = (stack_size & 0xF000) ? 0x1000 : 0x10000;
heap->AllocRange(0x70000000, 0x7F000000, stack_size_aligned, stack_alignment,
                 kMemoryAllocationReserve | kMemoryAllocationCommit,
                 kMemoryProtectRead | kMemoryProtectWrite, false, &stack_address);
return stack_address + stack_size;  // returns TOP of stack
```

**Critical difference**: MmCreateKernelStack does **NOT set up guard pages**. The entire allocation is RW-committed. If a kernel stack overflows, there is no guard -- it silently corrupts adjacent memory or faults on unallocated pages.

## 5. Default Stack Sizes

| Source | Size | Notes |
|--------|------|-------|
| XEX header `XEX_HEADER_DEFAULT_STACK_SIZE` (0x00020200) | Game-defined | Read from XEX at load time; GTA IV's value unknown without parsing the binary |
| Fallback (ELF modules) | 1 MB (0x100000) | `user_module.cpp:174` |
| Minimum (XThread constructor) | 16 KB (0x4000) | `xthread.cpp:72-73` |
| ExCreateThread minimum | 16 KB (0x4000) | `xboxkrnl_threading.cpp:128` -- also aligns to 4KB page boundary |
| Kernel stack (`pib->kernel_stack_size`) | 16 KB | `kernel_state.cpp:320` |
| Host thread stack | 16 MB | `xthread.cpp:371` |

The main thread gets `module->stack_size()` from `kernel_state.cpp:271`.

Threads created with `stack_size=0` in `ExCreateThread` inherit the executable module's default (`xboxkrnl_threading.cpp:123-124`).

**Stack size limit enforcement**: There is no explicit maximum stack size. The only limit is the available space in the stack address range (240 MB total). Each `AllocRange` call finds the next free region. The 64 KB page granularity of the v40000000 heap means all stack allocations are rounded up to 64 KB boundaries.

## 6. Guest Stack Pointer (r1) and Guard Page Relationship

On Xbox 360, the PPC stack pointer is register **r1**. It is initialized in `thread_state.cpp:39`:
```cpp
context_->r1.u64 = stack_base;    // Stack pointer
```

And can be updated at runtime via `KeSetCurrentStackPointers` (`xboxkrnl_threading.cpp:226-244`):
```cpp
thread->stack_base = stack_base.value();
thread->stack_limit = stack_limit.value();
pcr->stack_base_ptr = stack_base.guest_address();
pcr->stack_end_ptr = stack_limit.guest_address();
context->r1.u64 = stack_ptr.guest_address();
```

The kernel also tracks stack boundaries in the PCR (Processor Control Region) at `xthread.h:76-77`:
```cpp
rex::be<uint32_t> stack_base_ptr;  // 0x70 Stack base address (high addr)
rex::be<uint32_t> stack_end_ptr;   // 0x74 Stack end (low addr)
```

And in X_KTHREAD at offsets 0x5C/0x60/0xD0:
```cpp
rex::be<uint32_t> stack_base;        // 0x5C
rex::be<uint32_t> stack_limit;       // 0x60
rex::be<uint32_t> stack_alloc_base;  // 0xD0
```

The guard page sits just below `stack_limit` (at `alloc_base`). When r1 decreases
past `stack_limit` and the code writes to the guard page region, a fault occurs.

## 7. Xbox 360 vs Emulated Guard Page Behavior

### Real Xbox 360 Kernel

On real hardware, the Xbox 360 kernel uses Windows NT-derived stack management:
- Stacks are allocated with a committed region and a reserved-but-uncommitted region
- A single guard page (PAGE_GUARD flag) sits at the bottom of the committed region
- When a thread writes to the guard page, the kernel trap handler:
  1. Commits the guard page (makes it writable)
  2. **Moves the guard page down** by one page into the uncommitted region
  3. Updates `stack_limit` in the thread's KTHREAD structure
  4. If the guard page cannot be moved (out of reserved space), raises STATUS_STACK_OVERFLOW
- This provides **incremental stack growth** -- stacks start small and grow on demand

### RexGlue Emulated Version

RexGlue's implementation is **fundamentally different**:
- `XThread::AllocateStack` commits the ENTIRE stack region upfront (Reserve+Commit)
- Guard pages are set to `PROT_NONE` (no access at all, not PAGE_GUARD)
- There is no reserved-but-uncommitted region for growth
- The guard page handler in `AccessViolationCallback` simply unprotects the faulting page
- **The guard page does NOT move after expansion** -- once unprotected, it stays RW forever
- **stack_limit is never updated** after a guard page fault

This means:
1. For normal XThread stacks: the bottom guard page is a fixed 64 KB buffer. If the stack
   overflows past it, the next access hits unallocated heap space (not another guard page).
2. For kernel stacks (MmCreateKernelStack): there are NO guard pages at all.
3. There is no incremental growth -- the stack is fully committed from the start.

## 8. Stack Guard Fault Handling Chain

When a write hits a PROT_NONE page in the stack range:

1. **OS signal**: macOS delivers **SIGBUS** (not SIGSEGV) for writes to PROT_NONE shared memory / mmap'd regions

2. **ExceptionHandlerCallback** (`exception_handler_posix.cpp:56`):
   - On macOS ARM64: hardcodes `kWrite` operation (SIGBUS is exclusively for writes to PROT_NONE pages)
   - Calls `InitializeAccessViolation()` and iterates registered handlers

3. **MMIOHandler::ExceptionCallback** (`mmio_handler.cpp:364`):
   - Checks `operation != kRead && operation != kWrite` -- rejects `kUnknown` (the old bug)
   - Checks fault address is within virtual_membase_ to memory_end_
   - No MMIO range match for stack addresses
   - Falls through to `access_violation_callback_()` at line 412-413

4. **Memory::AccessViolationCallback** (`xmemory.cpp:436`):
   - Looks up heap for the faulting guest address
   - Checks: `heap_type == kGuestVirtual && address in [0x70000000, 0x7F000000)`
   - **Unprotects the faulting page**: `heap->Protect(page_addr, page_size, RW)`
   - Logs warning: "Stack guard page hit at guest 0x{:08X}"
   - Returns `true` -- execution resumes at the faulting instruction (which now succeeds)

## 9. What Happens When Expansion Fails (Uncommitted Page)

**This is the confirmed infinite-loop bug** (documented in detail in `43_stack_guard_preexisting.md`).

The handler code at `xmemory.cpp:448-461`:
```cpp
auto page_size = heap->page_size();
uint32_t page_addr = virtual_address & ~(page_size - 1);
heap->Protect(page_addr, page_size,
              memory::kMemoryProtectRead | memory::kMemoryProtectWrite);
REXSYS_WARN("Stack guard page hit at guest 0x{:08X} -- expanded stack", virtual_address);
return true;  // <-- BUG: returns true even if Protect() FAILED
```

The `Protect()` call can fail for two reasons:
1. The page was never allocated (not reserved+committed) -- happens when the fault address
   is beyond any thread's allocated stack region
2. The heap's page table entry has no `kMemoryAllocationCommit` flag for that page

When `Protect()` fails:
- The page remains PROT_NONE (inaccessible)
- The handler returns `true`, telling the signal dispatcher the fault was handled
- Execution resumes at the faulting instruction
- The same instruction faults again immediately on the same address
- This creates an **infinite loop** of: fault -> handler -> "handled" -> resume -> fault

In the observed GTA IV logs, this produced **1,569,138 repeated faults** at address
0x705D0000 on a single host thread, with no forward progress.

## 10. The Guard Page Does NOT Move After Expansion

Unlike real Xbox 360 kernel behavior (see section 7), RexGlue's handler:
- Does NOT create a new guard page below the expanded page
- Does NOT update `stack_limit` in X_KTHREAD or the PCR
- Does NOT check whether the faulting address is within any thread's allocated stack

This means after a legitimate guard page hit:
- The guard page becomes permanently RW
- There is no new guard page to catch further overflow
- The thread's `stack_limit` field still points to the old limit
- If the stack continues growing, the next fault will be on whatever page lies below
  (which may be another thread's top guard page, an unallocated page, or data)

## 11. Historical Bugs (Fixed in Commit 09b50a9a)

### Bug 1: SEH Overwrites Signal Handlers
`seh_initialize()` runs before `MMIOHandler::Install()` and overwrites SIGBUS/SIGSEGV handlers with SEH versions. When SIGBUS fires in non-SEH code, the SEH handler calls `raise(SIGBUS)` causing process death.

**Fix**: Always re-install signal handlers in `ExceptionHandler::Install()`.

### Bug 2: kUnknown Operation in Release Builds
On macOS ARM64, ESR is unavailable. `IsArm64LoadPrefetchStore` may fail for certain compiler-generated store variants. With `NDEBUG` (release builds), `assert_always` is a no-op, silently setting `kUnknown`. MMIOHandler rejects `kUnknown` at line 370, dropping the exception -- causing a crash.

**Fix**: Hardcode `SIGBUS -> kWrite` on macOS ARM64. SIGBUS is exclusively delivered for writes to PROT_NONE pages on macOS.

## 12. Example: 0x705D0000 Stack Guard Page Fault

Assuming a 1 MB (0x100000) stack allocated at 0x705D0000:

| Address Range | Size | Purpose |
|---------------|------|---------|
| 0x705D0000 - 0x705DFFFF | 64 KB | Bottom guard page (PROT_NONE) |
| 0x705E0000 - 0x706DFFFF | 1 MB | Usable stack (RW) |
| 0x706E0000 - 0x706EFFFF | 64 KB | Top guard page (PROT_NONE) |

- SP starts at **0x706E0000** and grows downward
- When SP decreases past **0x705E0000**, the next write touches the bottom guard page at **0x705D0000**
- This triggers SIGBUS (macOS) or SIGSEGV (Linux) because the page is PROT_NONE

If this is a MmCreateKernelStack allocation instead, 0x705D0000 would be the **base** of a committed region with **no guard page**. A fault at 0x705D0000 in that case means the kernel stack overflowed past its allocation boundary into unallocated heap space.

## 13. Potential Issue: MmCreateKernelStack Has No Guard Pages

`MmCreateKernelStack` allocates stacks without guard pages. If a kernel thread's stack overflows:
- The write goes to unallocated heap pages (not PROT_NONE guard pages)
- On macOS, this may produce SIGBUS on the unmapped page
- The fault address would be in the stack range (0x70000000-0x7F000000)
- The stack guard handler in `AccessViolationCallback` would **still catch it** and attempt to unprotect the page
- But the page was never committed, so `Protect()` fails silently, triggering the infinite loop (section 9)

## 14. Summary of Issues and Fixes Needed

| Issue | Status | Fix |
|-------|--------|-----|
| SEH overwrites signal handlers | FIXED (09b50a9a) | Re-install handlers in Install() |
| kUnknown on macOS ARM64 | FIXED (09b50a9a) | Hardcode SIGBUS -> kWrite |
| Protect() failure -> infinite loop | OPEN | Check Protect() return; return false on failure |
| Guard page does not move after expansion | OPEN | Optionally re-protect next page down as new guard |
| stack_limit not updated after expansion | OPEN | Update X_KTHREAD and PCR stack_end_ptr |
| MmCreateKernelStack has no guard pages | OPEN | Add guard pages matching XThread::AllocateStack |
| No stack size limit enforcement | BY DESIGN | 240 MB range provides implicit cap |

## Key Source Files

| File | Purpose |
|------|---------|
| `glue/rexglue-sdk-main/include/rex/system/xthread.h` | kStackAddressRangeBegin/End constants, XThread class, X_KPCR and X_KTHREAD structs |
| `glue/rexglue-sdk-main/src/system/xthread.cpp` | AllocateStack(), FreeStack(), guard page setup |
| `glue/rexglue-sdk-main/src/system/xmemory.cpp` | Heap init, LookupHeap(), AccessViolationCallback (stack guard handler) |
| `glue/rexglue-sdk-main/src/system/mmio_handler.cpp` | ExceptionCallback dispatch to access_violation_callback_ |
| `glue/rexglue-sdk-main/src/core/exception_handler_posix.cpp` | SIGBUS/SIGSEGV signal handler, kWrite hardcode fix |
| `glue/rexglue-sdk-main/src/core/memory_posix.cpp` | ToPosixProtectFlags (kNoAccess -> PROT_NONE), mmap/mprotect wrappers |
| `glue/rexglue-sdk-main/src/system/thread_state.cpp` | ThreadState constructor: sets r1 = stack_base, r13 = PCR address |
| `glue/rexglue-sdk-main/src/kernel/xboxkrnl/xboxkrnl_threading.cpp` | ExCreateThread, KeSetCurrentStackPointers |
| `glue/rexglue-sdk-main/src/kernel/xboxkrnl/xboxkrnl_memory.cpp` | MmCreateKernelStack (no guard pages), MmDeleteKernelStack |
| `glue/rexglue-sdk-main/src/system/user_module.cpp` | XEX_HEADER_DEFAULT_STACK_SIZE reading |
| `glue/rexglue-sdk-main/src/system/kernel_state.cpp` | Main thread creation with module->stack_size() |
| `glue/rexglue-sdk-main/include/rex/system/xmemory.h` | BaseHeap, VirtualHeap, PhysicalHeap, HeapType enum, PageEntry struct |
| `glue/rexglue-sdk-main/include/rex/ppc/memory.h` | Xbox 360 memory map comments, MMIO range definition |
