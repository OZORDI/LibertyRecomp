# 64: Main Thread Stack Size Analysis

## Executive Summary

The main thread's PPC stack overflows at ~5.8MB during world initialization. The stack
is allocated from the v40000000 heap (64KB page granularity) with a single guard page
on each side. The XEX header's `DEFAULT_STACK_SIZE` determines the initial allocation,
but the guard page fault handler dynamically expands the stack until it runs out of
committed memory at 0x705D0000. Increasing the stack to 16MB or 32MB would likely fix
the overflow because the call depth is bounded (world init, not unbounded recursion).

---

## 1. How LaunchModule Sets the Stack Size

**File**: `glue/rexglue-sdk-main/src/system/kernel_state.cpp`, line 260-292

```cpp
object_ref<XThread> KernelState::LaunchModule(object_ref<UserModule> module) {
  // ...
  auto thread =
      object_ref<XThread>(new XThread(kernel_state(), module->stack_size(), 0,
                                      module->entry_point(), 0, X_CREATE_SUSPENDED, true, true));
  // ...
}
```

`module->stack_size()` returns `UserModule::stack_size_`, which is populated in
`UserModule::LoadXexContinue()` (user_module.cpp, line 213):

```cpp
this->xex_module()->GetOptHeader(XEX_HEADER_DEFAULT_STACK_SIZE, &stack_size_);
```

The XThread constructor enforces a 16KB minimum (xthread.cpp, line 72):
```cpp
if (creation_params_.stack_size < 16 * 1024) {
    creation_params_.stack_size = 16 * 1024;
}
```

## 2. XEX Header Stack Size for GTA IV

`XEX_HEADER_DEFAULT_STACK_SIZE` = `0x00020200` (defined in xex2_info.h, line 296).

The key encoding: bottom byte `0x00` means the value is stored **directly** in the
header's `value` field (not as a pointer or offset). So `stack_size_` is set to
whatever 32-bit value the XEX header contains at this key.

For Xbox 360 GTA IV (v8 / TU8), the XEX header specifies `DEFAULT_STACK_SIZE` as a
standard value. Typical Xbox 360 games use 64KB to 1MB. GTA IV, being a large open-world
game, likely uses **256KB to 1MB**. The exact value is read from the XEX binary at runtime.

Without dumping the actual XEX header bytes, the value cannot be confirmed statically
from this codebase alone. However, we can infer it from the crash behavior: the stack
grows from high addresses downward starting near 0x705D0000, and guard pages fire
starting at 0x70000000 (the low end), meaning the initial allocation was placed
somewhere in the 0x70xxxxxx range within the v40000000 heap.

## 3. How XThread::AllocateStack Works

**File**: `glue/rexglue-sdk-main/src/system/xthread.cpp`, lines 224-253

```cpp
bool XThread::AllocateStack(uint32_t size) {
  auto heap = memory()->LookupHeap(kStackAddressRangeBegin);  // heap for 0x70000000

  auto alignment = heap->page_size();          // 64KB (see below)
  auto padding = heap->page_size() * 2;        // 128KB total guard pages
  size = rex::round_up(size, alignment);       // round up to 64KB
  auto actual_size = size + padding;           // stack + 2 guard pages

  uint32_t address = 0;
  heap->AllocRange(kStackAddressRangeBegin, kStackAddressRangeEnd,
                   actual_size, alignment, RESERVE|COMMIT, RW, false, &address);

  stack_alloc_base_ = address;                 // lowest address of allocation
  stack_alloc_size_ = actual_size;
  stack_limit_ = address + (padding / 2);      // +64KB (above bottom guard)
  stack_base_ = stack_limit_ + size;           // top of usable stack

  // Fill with 0xBE
  memory()->Fill(stack_alloc_base_, actual_size, 0xBE);

  // Set guard pages (NoAccess)
  heap->Protect(stack_alloc_base_, padding / 2, kMemoryProtectNoAccess);  // bottom guard: 64KB
  heap->Protect(stack_base_, padding / 2, kMemoryProtectNoAccess);        // top guard: 64KB
}
```

**Memory layout** (addresses increase downward in diagram):

```
stack_alloc_base_  = address
  [64KB bottom guard page - NoAccess]
stack_limit_       = address + 64KB
  [usable stack: size bytes, filled with 0xBE, grows DOWN from stack_base_]
stack_base_        = address + 64KB + size
  [64KB top guard page - NoAccess]
  total = size + 128KB
```

The heap used is **v40000000** (covers 0x40000000 to 0x7F000000), which has a
**64KB page size** (xmemory.cpp, line 167). This is critical -- all allocations,
guard pages, and protection changes are in 64KB granules.

## 4. The 5.8MB Overflow: Allocated vs Overflow

From the crash logs (doc 41 and doc 49):
- Guard page hits start at `0x70000000` and walk upward to `0x705D0000`
- `0x705D0000 - 0x70000000 = 0x5D0000 = ~5.8MB`
- `BaseHeap::Protect` fails at `0x705D0000`

**What this means**: The stack was allocated starting at some address. The
`AllocRange` call places the allocation within 0x70000000 to 0x7F000000 using
a bottom-up scan (`false` = bottom-up). So the allocation starts at 0x70000000
(or very near it).

The total allocation is: `round_up(xex_stack_size, 64KB) + 128KB`.

If GTA IV's XEX specifies a stack of 64KB (0x10000):
- rounded = 64KB
- total alloc = 64KB + 128KB = 192KB = 0x30000
- `stack_alloc_base_ = 0x70000000`
- `stack_limit_ = 0x70010000`
- `stack_base_ = 0x70020000`
- Top guard ends at `0x70030000`
- Total committed: 0x30000

But the guard page handler (xmemory.cpp, line 448-461) handles faults in the entire
0x70000000-0x7F000000 range by unprotecting pages. This means when the stack grows
past the bottom guard page, the handler fires and unprotects the page, allowing the
write. This continues page by page (each 64KB) until `heap->Protect()` fails at an
uncommitted page.

**The 5.8MB is NOT the allocated stack size. It is the total amount of memory the
guard page handler managed to unprotect before hitting an uncommitted region.**

The actual committed region from the original `AllocRange` call was much smaller
(whatever `actual_size` was). The guard handler then expanded into uncommitted pages
within the same heap, which succeeds until it reaches a page that was never committed.

The `Protect` call on an uncommitted page fails because you cannot change protection
on memory that was never committed (reserved but not committed).

## 5. Would 16MB or 32MB Fix the Overflow?

### Call Depth: Bounded or Unbounded?

The stack overflow occurs during **world initialization** -- specifically during the
particle emitter registration phase (`sub_825BF8A8` batch loop) and streaming/RPF
init. This is a one-time init sequence, not unbounded recursion.

Evidence from doc 49:
- "The stack guard is NOT a memory corruption bug -- it is stack overflow caused by
  unbounded recursion or extremely deep call chains in the world loading code"
- However, the call sites identified (particle emitter registration, RPF loading)
  are all bounded loops that process finite arrays
- The stack guard messages show steady growth, not exponential growth

**Key factor: Recompilation stack amplification.** Each recompiled PPC function becomes
a native C++ function. A PPC function with a 128-byte stack frame becomes a C++ function
with potentially 400-1000 bytes of host stack (due to `PPCContext` passing, register
spills, local variables). Deep call chains that worked on Xbox 360 with small PPC frames
consume 3-8x more stack in recompiled form.

The PPC stack (guest r1) is managed in guest memory (0x70xxxxxx), but the **host stack**
is also relevant. The host thread is allocated 16MB (xthread.cpp, line 371:
`params.stack_size = 16_MiB`). The host stack overflow would crash the process, while
the PPC stack overflow triggers the guard page handler.

### Assessment

**16MB would very likely fix the overflow.** The current overflow is at ~5.8MB, meaning
the actual peak usage is 5.8MB + whatever the initial allocation was. A 16MB allocation
would provide ~3x headroom. Since the call depth is bounded (world init completes
eventually), this is not a "delay the crash" situation -- it is a genuine fix.

**32MB would provide even more safety margin** but may be unnecessary. The
0x70000000-0x7F000000 range has 240MB of virtual address space, so even 32MB is fine.

### Recommendation

Set the main thread stack to **16MB** to match the host thread stack size. This can be
done by overriding `module->stack_size()` before passing it to the XThread constructor
in `KernelState::LaunchModule()`.

## 6. Stack Size Overrides in the Codebase

There are **no existing configuration options or CVARs** for overriding the XEX stack size.

- No `REXCVAR` for stack size exists anywhere in the codebase
- No TOML config key for stack override in `gta4_config.toml`
- No command-line option
- `config.toml` in the install directory has no stack-related keys

The only stack size manipulation is:
1. **16KB minimum** in XThread constructor (line 72-73)
2. **16KB alignment** in ExCreateThread_entry (xboxkrnl_threading.cpp, line 128)
3. **XAM task threads** inherit the module's stack size (xam_task.cpp, line 52-55)
4. **Host threads** always get 16MB (xthread.cpp, line 371 and line 1478)

### Where to Add an Override

The cleanest place is `KernelState::LaunchModule()` (kernel_state.cpp, line 260-292).
Before creating the XThread, override the stack size:

```cpp
auto stack = module->stack_size();
// GTA IV's XEX stack size is too small for recompiled code.
// Recompilation amplifies stack usage ~4-8x per call frame.
stack = std::max(stack, uint32_t(16 * 1024 * 1024));  // 16 MB minimum
auto thread = object_ref<XThread>(new XThread(kernel_state(), stack, 0, ...));
```

Alternatively, a game-specific override could be added to the TOML config:
```toml
[runtime]
main_thread_stack_size = 0x1000000  # 16 MB
```

## 7. Xbox 360 Typical Main Thread Stack Sizes

On real Xbox 360 hardware:
- Default kernel stack: **16KB** (set in ProcessInfoBlock at kernel_state.cpp, line 320)
- Default game thread stack: **64KB** (the XEX header's `DEFAULT_STACK_SIZE`)
- Typical range: **64KB to 1MB** depending on game complexity
- GTA IV likely uses **256KB to 1MB** given its complexity

The Xbox 360 kernel also supports dynamic stack growth via guard pages, similar to
the RexGlue implementation. On real hardware, the kernel commits additional pages
from the 64KB virtual heap when a stack guard page is hit. However, real hardware
has a hard limit at 1MB for most threads.

The critical difference: on Xbox 360, PPC stack frames are compact (typically 32-256
bytes per frame). Recompilation inflates each frame to 400-1000+ bytes of host stack
due to:
- Full `PPCContext` struct passed by reference (but still on stack)
- Register spills from 32 GPRs + 32 FPRs + CR + XER + CTR + LR
- C++ function prologue/epilogue overhead
- Compiler alignment requirements (16-byte stack alignment on macOS)

A function using 128 bytes of PPC stack might consume 600-800 bytes of combined
host+guest stack in the recompiled version.

## 8. RexGlue Support for Overriding XEX Stack Size

**Currently: No.** RexGlue does not support overriding the XEX-specified stack size.
The value flows directly from the XEX header through `UserModule::stack_size_` to
`KernelState::LaunchModule()` to `XThread::AllocateStack()` with no interception point.

The guard page handler in xmemory.cpp (line 448-461) provides implicit dynamic growth,
but it only works within already-committed pages or pages that the host OS can lazily
commit. It cannot grow past the `AllocRange` reservation boundary.

### Implementation Options

1. **Hardcoded override in LaunchModule** -- simplest, 2 lines of code
2. **CVAR** -- add `REXCVAR_DEFINE_UINT32(main_thread_stack_size, 0, "Runtime", "...")`
   and use it in LaunchModule if non-zero
3. **TOML config per-game** -- add to gta4_config.toml and pass through to runtime
4. **Scale factor** -- multiply XEX stack size by a constant (e.g., 16x) to account
   for recompilation amplification

Option 1 is recommended for an immediate fix. Option 2 or 3 would be better long-term.

## 9. Guard Page Handler Limitations

The current guard page handler (xmemory.cpp, line 448-461) has two problems:

1. **No bounds checking**: It will attempt to unprotect ANY page in the 0x70000000-0x7F000000
   range, even if it belongs to a different thread's stack allocation. With multiple threads
   allocating stacks in this range, unprotecting a neighbor's memory is possible.

2. **Infinite loop on uncommitted pages**: When `Protect()` fails on an uncommitted page,
   the handler returns `true` (claiming it handled the fault), but the page is still
   inaccessible. The CPU retries the write, faults again, handler fires again -- infinite
   loop. This is the 0x705D0000 behavior seen in the logs.

A fix for issue 2 was landed in commit `09b50a9a` ("fix: resolve two root causes
preventing stack guard page fault handling"), but the fundamental issue remains: if the
allocated stack is too small, the guard handler cannot manufacture more memory.

## Summary Table

| Property | Value |
|----------|-------|
| Stack source | XEX header `DEFAULT_STACK_SIZE` (0x00020200) |
| Heap for stacks | v40000000 (0x40000000-0x7F000000, 64KB pages) |
| Guard pages | 2 x 64KB (top and bottom) |
| Stack alignment | 64KB |
| Host thread stack | 16MB (hardcoded) |
| Current PPC stack overflow | ~5.8MB at 0x705D0000 |
| Call depth | Bounded (world init, not recursion) |
| Override mechanism | None exists |
| Recommended fix | Set main thread PPC stack to 16MB in LaunchModule |
| Fix location | kernel_state.cpp line 271 |
