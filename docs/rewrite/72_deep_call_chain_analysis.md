# 72: Deep Call Chain Analysis -- The "Stack Overflow" Is Actually a Corrupted memcpy

## Executive Summary

The crash diagnosed as "stack overflow consuming 21.4MB" is **not caused by deep recursion
or stack growth**. It is caused by an **unhooked memcpy variant (sub_82A00DC0)** receiving a
corrupted count of 0xFFFFFFDB (~4GB unsigned / -37 signed). The memcpy writes sequentially
through the guest stack address range (0x70000000-0x7F000000), triggering 56 guard page
expansions (3.5MB of pages wrongly committed) before hitting an uncommitted page and crashing.

The guard page handler in `xmemory.cpp` is **blind to the cause** of faults in the stack range
-- it treats every access violation in 0x70000000-0x7F000000 as legitimate stack growth. This
masks the real bug: a caller passing a negative size to memcpy.

---

## 1. Log Evidence (/tmp/liberty_run2.log)

### Thread identity
- **Overflow thread**: t42156022 = Main XThread (handle 0xF8000010)
- **Main loop thread**: t42155587 (YIELD counter, Setup)
- The overflow is on the main game thread during world initialization

### Guard page pattern (lines 73181-73236)
```
0x70000000 -> 0x70080000  (5 hits, 512KB -- different thread's stack)
                           16MB gap -- initial committed stack
0x71090000 -> 0x71580000  (51 hits, steady ~128KB increments upward)
0x71590000                 CRASH -- uncommitted page
```

The addresses go **upward** (0x71090000 to 0x71580000). On PPC where stacks grow **down**,
stack growth would hit guard pages at **lower** addresses. The upward progression means these
are NOT stack growth faults -- they are sequential memory writes moving through higher
addresses.

### Context immediately before crash (lines 73100-73180)
Hundreds of `[MISSING-FUNC] indirect call to 00000000` from addresses 828C99CC, 821446F8,
and 821911C4. These null indirect calls return immediately (no-op), so they don't directly
cause the crash but indicate the rendering subsystem is running with missing vtable entries.

---

## 2. Crash Registers

```
PPCContext at crash:
  r1  = 0x7108F520    <-- PPC stack pointer (WELL within committed stack)
  r3  = 0x7158FFF8    <-- destination buffer (ABOVE stack, in uncommitted region)
  r4  = 0x17A7EF40    <-- source buffer (valid game heap address)
  r5  = 0xFFFFFFDB    <-- count (-37 signed / ~4GB unsigned)
  lr  = 0x8285AE6C    <-- return address (in sub_8285AE20, GPU/render code)

Host ARM64:
  sp  = 0x170B75E10   <-- host stack pointer (fine, no host overflow)
  pc  = 0x105440E80   <-- sub_82A00DC0 + 0xB3C (inside copy loop)
```

**Key observation**: r1 (guest SP) is at 0x7108F520, deep inside the committed stack region.
The fault is at r3 = 0x7158FFF8, which is 5.2MB above r1. This is NOT a stack pointer
overflow -- r3 is a **data pointer** being written by a copy function.

---

## 3. sub_82A00DC0 Is Xbox 360 memcpy

The recompiled code at `gta4_recomp.68.cpp:27638` reveals sub_82A00DC0 is an optimized
block copy routine:

```cpp
// Aligns dst to 8-byte boundary
// Then copies in 8-byte or 128-byte blocks
// Parameters: r3=dst, r4=src, r5=count
```

It performs byte-by-byte alignment, then switches to word/doubleword copies. The crash at
offset +0xB3C is deep in the main copy loop, confirming it was in the middle of a large
copy operation.

**This is NOT the same as the hooked memcpy at 0x82A11940.** It is a separate, unhooked
copy function in the Xbox 360 CRT. It has **582 call sites** across 55 generated files.

---

## 4. The Corrupted Count

r5 = 0xFFFFFFDB at crash time:
- As **unsigned int32**: 4,294,967,259 (~4GB)
- As **signed int32**: -37

The caller (sub_8285AE20, a GPU/rendering function at lr=0x8285AE6C) likely computed:
```
count = end_ptr - start_ptr
```
where `start_ptr > end_ptr`, yielding a negative result. When passed to memcpy as an
unsigned count, this becomes ~4GB.

The memcpy dutifully copies bytes forward from src to dst. Starting at dst=0x7158FFF8,
it writes through page after page. Each page in 0x70000000-0x7F000000 triggers the guard
page handler, which commits the page. After 56 pages (3.5MB), it reaches 0x71590000 --
a page that was never allocated (past the end of any thread's stack) -- and crashes.

---

## 5. Guard Page Handler Blindness

From `glue/rexglue-sdk-main/src/system/xmemory.cpp:449`:

```cpp
if (heap->heap_type() == memory::HeapType::kGuestVirtual &&
    virtual_address >= system::XThread::kStackAddressRangeBegin &&  // 0x70000000
    virtual_address < system::XThread::kStackAddressRangeEnd) {     // 0x7F000000
  // Unprotect the faulting page so the write can proceed.
  bool ok = heap->Protect(page_addr, page_size, RW);
```

The handler checks ONLY whether the fault address is in the global stack range
(0x70000000-0x7F000000). It does **not** check:
1. Whether the fault address is within the **current thread's** stack allocation
2. Whether the access is from the stack pointer (r1) or a data pointer (r3, r4, etc.)
3. Whether the access direction matches stack growth (downward)

This means any write to the 0x70-0x7F range -- including a runaway memcpy, heap corruption,
or stale pointer dereference -- gets silently accommodated by committing new pages.

---

## 6. The Real Stack Budget

### Guest stack allocation (AllocateStack in xthread.cpp:224)
- **Stack size**: 16MB (kMinStackSize at the time of this run, now 64MB)
- **Page size**: 64KB (v40000000 heap)
- **Layout**: [64KB guard] [16MB usable] [64KB guard]
- **Total allocation**: 16MB + 128KB
- **Entire usable region is committed** (Reserve|Commit flags)

### Multiple stacks in 0x70000000-0x7F000000
- Audio Worker (thid 1): small stack
- Kernel Dispatch (thid 2): small stack
- Main XThread: 16MB stack
- Game-created stacks via MmCreateKernelStack: variable

### Host thread stack
- 16MB for XThread native thread (xthread.cpp:371)
- Host SP at crash: 0x170B75E10 (nowhere near exhausted)

---

## 7. Answers to Original Research Questions

### Q1: Pattern between guard expansions and failure?
The guard pages expand **upward** (0x71090000 to 0x71580000) because memcpy writes forward
through memory. This is NOT stack growth (which goes downward). The "expansion" is the guard
page handler wrongly committing pages for a runaway data copy.

### Q2: During initial world loading or per-frame updates?
During initial world loading. The YIELD counter is at #76000. VFS text file accesses
and MISSING-FUNC calls from rendering code (828C99CC) are active, indicating the game is
in its early initialization/rendering setup phase.

### Q3: Multiple deep recursive functions each consuming several MB?
**No.** The consumption is from a single memcpy call with a corrupted count, not recursion.
Doc 66's analysis of sub_82343EF0 (3,840B recursive frames) remains valid as a potential
future risk, but it is not the cause of THIS crash.

### Q4: Recompiled function calling convention overhead?
Each recompiled PPC function is a C++ function: `void sub_XXX(PPCContext& ctx, uint8_t* base)`.
The PPCContext is passed by **reference** (8-byte pointer), not by value (~1.3KB struct).
Per-call host overhead:
- ARM64 frame: x29/x30 save = 16 bytes
- Callee-saved registers (x19-x28 used): 0-80 bytes
- Local variables (PPCRegister temp, uint32_t ea): 8-16 bytes
- **Total: ~48-256 bytes per host frame** (much less than the PPC frame)

For sub_82343EF0 specifically: 3,840B PPC frame + ~200B host frame = ~4,040B total per
recursion level. At 16MB, this allows ~4,000 recursion levels before guest stack overflow.

### Q5: sub_82343EF0 active at overflow time?
**No.** The crash function is sub_82A00DC0 (memcpy), called from sub_8285AE20 (GPU/render
code). sub_82343EF0 (collision) is not on the call chain.

### Q6: Could reducing recursive frame sizes help?
For THIS specific crash: no, it's not recursion. For future robustness: yes, hooking
sub_82343EF0 to use an iterative stack on the heap would prevent collision-recursion
overflow. But the immediate fix is the memcpy corruption.

### Q7: Theoretical maximum call depth?
Not relevant to this crash. The call depth is shallow -- sub_8285AE20 calls sub_82A00DC0
directly. The "21MB" is not call depth but memcpy write distance.

---

## 8. Proposed Fixes

### Fix 1: Hook the unhooked memcpy (sub_82A00DC0)
Add a CRT hook that validates the count parameter:
```cpp
PPC_FUNC_HOOK(sub_82A00DC0) {
    uint32_t dst   = ctx.r3.u32;
    uint32_t src   = ctx.r4.u32;
    uint32_t count = ctx.r5.u32;
    if (count > 0x10000000) { // > 256MB is surely corrupt
        fprintf(stderr, "[WARN] memcpy2 count=0x%08X from lr=0x%08X -- clamped\n",
                count, (uint32_t)ctx.lr);
        return; // skip the copy
    }
    __imp__sub_82A00DC0(ctx, base); // call original
}
```

### Fix 2: Fix the caller (sub_8285AE20)
Identify why sub_8285AE20 at offset +0x4C passes r5=0xFFFFFFDB. This is likely a
subtraction that went negative due to a missing vtable dispatch (the MISSING-FUNC calls
from 828C99CC are in the same address range). A null function pointer returning 0 instead
of a valid size could cause `end - start` to be negative.

### Fix 3: Improve the guard page handler
Add validation to `AccessViolationCallback`:
```cpp
// Only expand if the fault address is within THIS thread's stack bounds
auto* xthread = XThread::GetCurrentThread();
if (xthread && (virtual_address < xthread->stack_alloc_base() ||
                virtual_address >= xthread->stack_alloc_base() + xthread->stack_alloc_size())) {
    // Fault is NOT in current thread's stack -- don't expand
    return false;
}
```

### Fix 4: Add the second memcpy to rexcrt hooks
sub_82A00DC0 (582 call sites) should be added to the rexcrt configuration alongside the
existing memcpy at 0x82A11940. This would redirect all calls to the native host memcpy,
which is faster and would segfault cleanly on bad counts rather than silently corrupting
pages.

---

## 9. Why 64MB Stack Does Not Fix This

The kMinStackSize was raised to 64MB in `kernel_state.cpp:276`. This prevents the crash
only because the 64MB committed region absorbs the memcpy's writes before reaching
uncommitted pages. The corrupted memcpy still writes ~4GB of garbage; it just happens to
stay within committed memory for the first 64MB. This is a **band-aid**, not a fix:

1. The memcpy still corrupts 5+MB of memory (other threads' stacks, guard pages)
2. A future larger corruption count could still overflow 64MB
3. The guard page handler still masks data corruption as "stack growth"

---

## 10. Related Observations

### Null indirect calls (MISSING-FUNC pattern)
The lines before the crash show hundreds of `indirect call to 00000000` from:
- **828C99CC**: Render dispatch (sub_828C9980 area, see doc 51)
- **821446F8**: Unknown (possibly resource manager)
- **8214434C**: Unknown
- **821911C4**: Unknown

These null calls are vtable entries that were not populated. They return immediately (the
MISSING-FUNC handler is a no-op for null targets). However, the caller may depend on the
return value -- getting 0 instead of a valid pointer/size could cause the corrupted count
passed to memcpy.

### The -37 value
0xFFFFFFDB = -37. This specific value suggests a size computation like:
```
length = string_end - string_start  // where string_start is 37 bytes past string_end
```
or an error code (-37) being used as a buffer size. The caller (sub_8285AE20) is in the
GPU/rendering subsystem (0x8285xxxx range), likely copying shader parameters, vertex data,
or texture metadata.
