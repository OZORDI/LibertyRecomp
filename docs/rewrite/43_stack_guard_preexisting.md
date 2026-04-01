# 43: Stack Guard Page Fault -- Pre-existing Issue Analysis

## Question
Is the stack guard page infinite loop at 0x705D0000 a NEW issue caused by scene
creation fixes, or a pre-existing issue previously masked?

## Answer: PRE-EXISTING

The stack guard page fault existed before any scene creation hooks. It was
previously manifesting as an immediate process crash (EXC_BAD_ACCESS) rather
than an infinite loop.

## Evidence

### Timeline of events (all Mar 25, 2026)

| Time  | Event | Log file |
|-------|-------|----------|
| 14:11 | crash6: `EXC_BAD_ACCESS (code=2, address=0x807008fffc)` in sub_821D01B0 | liberty_crash6.log |
| 15:13 | Commit b595515d: Stack guard handler ADDED to xmemory.cpp | git |
| 15:17 | Commit 09b50a9a: SIGBUS/SEH handler fix for macOS ARM64 | git |

The crash6 fault address `0x807008FFFC` translates to guest address `0x7008FFFC`
(base was `0x8000000000` in that session), which is squarely in the stack range
`0x70000000-0x7F000000`. This is the SAME category of fault now caught by the
guard page handler.

### Zero stack guard messages in ALL earlier logs

Every log file from before the handler was added (20 files checked) contains
ZERO instances of "Stack guard", "705D0000", or "BaseHeap::Protect failed":

- liberty_crash{1-6}.log (Mar 25 13:02-14:11) -- 0 hits each
- liberty_run{2-12}.log (Mar 25 19:04-20:36) -- 0 hits each
- liberty_diag{1,2}.log (Mar 25 20:46-21:09) -- 0 hits each

This is because the handler did not exist yet. Guard page faults caused
immediate process death via unhandled SIGBUS.

### Current log (liberty_run.log, Mar 27)

- 1,569,259 occurrences of "Stack guard page hit"
- 1,569,138 of those are at address 0x705D0000 (infinite loop)
- 56 unique addresses successfully expanded before hitting 0x705D0000
- All faults on host thread t41614048

## Root Cause of the Infinite Loop

The handler in `xmemory.cpp` line 448-461 has a bug:

```cpp
heap->Protect(page_addr, page_size,
              memory::kMemoryProtectRead | memory::kMemoryProtectWrite);
REXSYS_WARN("Stack guard page hit at guest 0x{:08X} -- expanded stack", virtual_address);
return true;  // <-- returns true even if Protect() FAILED
```

When `Protect()` fails (page at 0x705D0000 is uncommitted), the handler still
returns `true`, telling the signal handler the exception was handled. Execution
resumes at the faulting instruction, which immediately faults again on the same
address.

## Why 0x705D0000 is Uncommitted

Stack allocations use `AllocRange()` with `kMemoryAllocationReserve | kMemoryAllocationCommit`.
Each thread stack is ~0x30000 bytes (0x10000 stack + 0x20000 guard padding).
The committed range ends somewhere around 0x705C0000 (after ~31 thread stacks).
Address 0x705D0000 falls beyond the last committed allocation -- it was never
allocated by any XThread, so the page table entry has no `kMemoryAllocationCommit` flag.

## What is Actually Accessing 0x70000000-0x705D0000?

The fault pattern shows 56 sequential guard page hits across 5.75 MB of the stack
range, all on a single host thread. This is NOT normal per-thread stack growth
(which would hit one guard page per thread). Instead, something is linearly
scanning or zeroing the entire stack memory region. Likely candidates:

1. A memory scan/initialization routine touching all allocated stack pages
2. The MISSING-FUNC indirect call storm from 0x828C99CC (2.3M occurrences in the
   same log, interleaved with guard faults) -- could be a vtable dispatcher
   walking memory regions
3. World loading code (sub_821D01B0 was the crash site in crash6) accessing
   stack-range memory as if it were data

## Relationship to ALLOC FALLBACK Storm

The MEMORY.md documents an "ALLOC FALLBACK storm" where sub_8218BE28 (game malloc)
falls back to the host page allocator when TLS is uninitialized. This is a
separate issue from the stack guard faults:

- ALLOC FALLBACK: heap TLS not set up, malloc uses fallback path
- Stack guard: linear scan of stack address range hitting uncommitted pages

However, both may share a common root cause: game systems initialized after scene
creation (physics, particles, world streaming) running on threads whose TLS and
stack allocations are not properly sized or configured for the recompiled environment.

## Fix Required

The handler must check the return value of `Protect()` and return `false` on
failure, allowing the fault to propagate to the next handler in the chain (or
crash cleanly with diagnostics):

```cpp
if (!heap->Protect(page_addr, page_size,
                   memory::kMemoryProtectRead | memory::kMemoryProtectWrite)) {
    REXSYS_ERROR("Stack guard expansion FAILED at 0x{:08X} -- page uncommitted", virtual_address);
    return false;  // let fallback crash handler report diagnostics
}
REXSYS_WARN("Stack guard page hit at guest 0x{:08X} -- expanded stack", virtual_address);
return true;
```

Additionally, the root cause of the linear stack-range scan should be investigated
to determine WHY guest code is accessing memory beyond allocated thread stacks.
