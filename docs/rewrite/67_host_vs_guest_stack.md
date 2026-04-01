# 67: Host Stack vs Guest Stack Overflow Analysis

## Summary

**The fault at guest address 0x705D0000 is a GUEST stack guard page fault, triggered by a
recompiled PPC `stwu r1,-N(r1)` instruction writing to guest memory.** It is NOT a host
stack overflow. However, the reason the guest stack is exhausted may still be caused by
excessive host-side call depth (deep recursion in recompiled code), which drains the guest
stack in parallel with the host stack.

---

## 1. What Actually Triggers the Fault

The recompiled code for PPC `stwu r1, -96(r1)` (the standard PPC stack frame prologue)
generates this C++ code:

```cpp
ea = -96 + ctx.r1.u32;
PPC_STORE_U32(ea, ctx.r1.u32);   // <-- THIS is the faulting instruction
ctx.r1.u32 = ea;
```

Where `PPC_STORE_U32` expands to:

```cpp
*(volatile uint32_t*)(base + (uint32_t)(ea)) = __builtin_bswap32(ctx.r1.u32);
```

The `base` pointer is `virtual_membase_` (a host pointer like `0x100000000`), and `ea` is
the 32-bit guest address (like `0x705D0000`). The actual host address being written is:

```
host_addr = base + 0x705D0000 = e.g. 0x1705D0000
```

This host address falls within the mmap'd region for guest virtual memory. When the guest
stack grows past its guard page (a PROT_NONE region), the write to `base + 0x705D0000`
triggers a SIGBUS (macOS) or SIGSEGV (Linux).

## 2. How the Signal Handler Processes It

The fault path is:

1. **OS delivers SIGBUS/SIGSEGV** with `signal_info->si_addr` = the HOST address
   (e.g., `0x1705D0000`)

2. **`ExceptionHandlerCallback`** (exception_handler_posix.cpp:56) captures
   `si_addr` as `fault_address_` -- this is a 64-bit HOST address

3. **`MMIOHandler::ExceptionCallback`** (mmio_handler.cpp:364) checks if
   `fault_address` is within `[virtual_membase_, memory_end_]` -- it is,
   because `0x1705D0000` is within the mapped guest range

4. **`host_to_guest_virtual_`** converts: `host_addr - virtual_membase_` =
   `0x705D0000` (the guest address)

5. **`Memory::AccessViolationCallback`** (xmemory.cpp:436) calls
   `HostToGuestVirtual(host_address)` to get guest address `0x705D0000`

6. **Stack guard check** (xmemory.cpp:448-461) checks:
   - `heap_type() == kGuestVirtual` -- YES (0x70000000 is in `v40000000` heap)
   - `0x705D0000 >= 0x70000000` -- YES (kStackAddressRangeBegin)
   - `0x705D0000 < 0x7F000000` -- YES (kStackAddressRangeEnd)
   - Result: unprotects the page, returns true, execution continues

**The handler IS looking at the right thing.** The faulting address is a host address
that maps to a guest stack address, and the translation is correct.

## 3. The Two Stacks Are Independent

### Guest Stack (PPC r1)
- **Location**: Allocated in guest virtual address range 0x70000000-0x7F000000
- **Backing**: mmap'd host memory at `virtual_membase_ + 0x70000000`
- **Size**: Determined by `creation_params_.stack_size` (minimum 16KB, typically 64KB)
- **Guard pages**: One page at bottom (PROT_NONE), one page at top (PROT_NONE)
- **Growth mechanism**: `ctx.r1.u32 -= N` (PPC register math), then
  `PPC_STORE_U32(ctx.r1.u32, ...)` writes to `base + ctx.r1.u32`
- **Layout** (from AllocateStack in xthread.cpp:224):
  ```
  stack_alloc_base_ = address                    // bottom guard page (PROT_NONE)
  stack_limit_      = address + page_size        // usable stack bottom
  stack_base_       = stack_limit_ + size         // usable stack top (r1 starts here)
  stack_base_ + page_size                        // top guard page (PROT_NONE)
  ```

### Host Stack (native thread)
- **Location**: OS-managed, in high address space (e.g., `0x16XXXXXXX` on macOS ARM64)
- **Size**: Explicitly set to **16 MiB** (`params.stack_size = 16_MiB` in xthread.cpp:371)
- **Guard pages**: OS-managed (typically 1 page at the bottom)
- **Growth mechanism**: ARM64/x86 `sp` register, decremented by compiled C++ function
  prologues

### They do NOT overlap
Guest stack memory is in the mmap'd region (`virtual_membase_ + 0x70xxxxxx`).
Host stack memory is in OS-allocated thread stack space. These are in completely
different address ranges.

## 4. Why the Guest Stack Overflows

Each recompiled PPC function:
1. Is a native C++ function (`void sub_XXXXXXXX(PPCContext& ctx, uint8_t* base)`)
2. Calls other recompiled functions as **native C++ function calls**
3. Each such call consumes BOTH:
   - **Host stack**: native call frame (~32-128 bytes per frame for locals + ABI)
   - **Guest stack**: `ctx.r1.u32 -= N` then `PPC_STORE_U32(...)` to guest memory

A deep call chain of, say, 1000 nested PPC function calls would consume:
- ~100KB of host stack (well within 16 MiB)
- ~96KB of guest stack (96 bytes per frame x 1000 = could exceed a 64KB guest stack)

The guest stack is typically much smaller than the host stack. A function like the
particle emitter registration loop (`sub_825BF8A8`) that deeply recurses could
exhaust the guest stack long before the host stack.

## 5. Could the Host Stack Overflow Instead?

**Unlikely given current configuration.** The host stack is 16 MiB, which is very
generous. A host stack overflow would fault at an address in OS thread stack space
(e.g., `0x16FXXXXXX` on macOS ARM64), NOT in the guest memory range. The MMIO handler
would reject it immediately at mmio_handler.cpp:375-378 ("Quick kill anything outside
our mapping") and the signal would be fatal.

The guard handler at xmemory.cpp:448 would never see a host stack overflow -- it only
fires for faults within `[virtual_membase_, physical_membase_)`.

## 6. Key Finding: The Guard Handler Works Correctly

The guard page handler at xmemory.cpp:448-461 is correctly:
1. Receiving the HOST address from `si_addr`
2. Translating it to a guest address via `HostToGuestVirtual`
3. Checking if it falls in the stack range (0x70000000-0x7F000000)
4. Unprotecting the page to allow stack growth

**The 5.8 MB figure** (if `0x705D0000` is the low-water mark and `stack_base_` is near
`0x70BB0000` or similar) represents how far the guest stack has grown downward, which
means the guest thread was allocated a stack larger than the 16KB minimum, or the guard
handler has been expanding the stack repeatedly by unprotecting successive pages.

## 7. The Real Question

The guard handler unprotects pages to let the stack grow, but there is no limit check --
it will keep expanding the stack all the way down to `0x70000000` (the bottom of the
stack address range). This means:

- **It is not a hard crash** -- the handler catches it and expands
- **But it indicates pathological recursion** in the recompiled guest code
- **The fix strategy should focus on WHY the guest call stack is so deep**, not on
  changing the guard handler

## 8. Diagnostic Recommendations

1. **Log ctx.r1 value** when the guard handler fires -- this shows how far the guest
   stack has grown
2. **Log the host RSP/SP** in the signal handler -- compare to thread stack bounds to
   verify host stack is not also near exhaustion
3. **Add a hard limit** in the guard handler: if `virtual_address < stack_alloc_base_ +
   some_minimum`, refuse to expand and abort with a diagnostic dump of the call chain
4. **Check guest stack allocation size** -- GTA IV may need larger guest stacks than the
   default (Xbox 360 default is 64KB, but some titles request more via XEX headers)

## Source Files Referenced

- `glue/rexglue-sdk-main/src/system/xmemory.cpp` -- guard page handler (line 448-461)
- `glue/rexglue-sdk-main/src/core/exception_handler_posix.cpp` -- signal handler
- `glue/rexglue-sdk-main/src/system/mmio_handler.cpp` -- MMIO fault dispatch
- `glue/rexglue-sdk-main/src/system/xthread.cpp` -- AllocateStack (line 224), host stack
  size 16 MiB (line 371)
- `glue/rexglue-sdk-main/include/rex/ppc/memory.h` -- PPC_STORE_U32 macro (line 95)
- `glue/rexglue-sdk-main/include/rex/ppc/context.h` -- PPCFunc signature (line 46)
- `glue/rexglue-sdk-main/include/rex/system/xthread.h` -- kStackAddressRangeBegin/End
  (line 150-151)
- `glue/rexglue-sdk-main/src/codegen/builders/helpers.h` -- emitStoreWithUpdate (line 221)
- `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.0.cpp` -- example recompiled
  output showing stwu codegen
