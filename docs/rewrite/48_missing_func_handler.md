# 48: MISSING-FUNC Handler -- What Happens on Out-of-Range Indirect Calls

## Summary

When `PPC_CALL_INDIRECT_FUNC` is called with an address like `0x000F4000` (outside
the code range `0x82140000`-`0x829D635C`), the runtime logs a warning and **falls
through silently**. It does NOT crash, does NOT call a fallback, and does NOT
touch the PPC context (registers, stack pointer r1). This is safe for a single
call but becomes dangerous when the caller is in a loop, because the caller's
own stack frame management is what causes progressive stack growth.

## 1. PPC_CALL_INDIRECT_FUNC Implementation

**File**: `glue/rexglue-sdk-main/include/rex/ppc/context.h`, lines 127-140

```c
#define PPC_CALL_INDIRECT_FUNC(x)                                                        \
  do {                                                                                   \
    uint32_t _icf_addr = uint32_t(x);                                                   \
    bool _icf_in_range = (_icf_addr >= uint32_t(PPC_CODE_BASE) &&                       \
                          _icf_addr <  uint32_t(PPC_CODE_BASE) + uint32_t(PPC_CODE_SIZE)); \
    PPCFunc* _icf_fn = _icf_in_range ? PPC_LOOKUP_FUNC(base, _icf_addr) : nullptr;      \
    if (_icf_fn) {                                                                       \
      _icf_fn(ctx, base);                                                                \
    } else {                                                                             \
      fprintf(stderr, "[MISSING-FUNC] indirect call to %08X (in_range=%d) from %08X\n",  \
              _icf_addr, (int)_icf_in_range, uint32_t(ctx.lr));                          \
      fflush(stderr);                                                                    \
    }                                                                                    \
  } while (0)
```

### Range check

- `PPC_CODE_BASE` = `0x82140000`
- `PPC_CODE_SIZE` = `0x0094635C`
- Valid range: `0x82140000` to `0x829D635C`
- Address `0x000F4000` fails the range check immediately (`_icf_in_range = false`).
- `_icf_fn` is set to `nullptr` without any memory access to the function table.

### On miss (else branch)

- Prints to stderr: `[MISSING-FUNC] indirect call to 000F4000 (in_range=0) from 0x821911C4`
- Flushes stderr.
- Falls through to the next line of the caller.
- **Does NOT set r3 to any value** (no `ctx.r3.u64 = 0`).
- **Does NOT modify r1** (no stack pointer change).
- **Does NOT throw, abort, or longjmp.**

## 2. Where MISSING-FUNC Is Printed

Only one location in the entire codebase:

- `glue/rexglue-sdk-main/include/rex/ppc/context.h`, line 136 (the `else` branch above)

There is no secondary handler, no callback, no hook system.

## 3. After Logging: What Happens

**Answer: (b) The call is skipped entirely, execution continues at the next statement.**

The `do { ... } while(0)` macro completes and control returns to the caller's
recompiled C++ code at the statement immediately following the `PPC_CALL_INDIRECT_FUNC(...)` line.

For `bctrl` (indirect call-and-link), the generated code pattern is:
```c
ctx.lr = 0x821911C4;                    // set link register
PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);    // <-- skipped if miss
// execution continues here regardless
```

For `bctr` (indirect tail-call), the generated code pattern is:
```c
PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);    // <-- skipped if miss
return;                                  // returns from current function
```

### Critical difference between bctrl and bctr on miss:

- **bctrl miss**: Caller continues executing. The callee's side effects (including
  setting a return value in r3) never happen. If the caller checks r3, it sees
  a stale value from a previous operation.
- **bctr miss**: The function returns immediately. This is a tail-call, so the
  return value is whatever r3 currently holds.

## 4. PPC Context Corruption

**No direct corruption by the handler.** The MISSING-FUNC path touches zero
PPCContext fields. The registers (r0-r31, f0-f31, v0-v127), stack pointer (r1),
link register (lr), count register (ctr), and condition registers (cr0-cr7) are
all untouched.

However, **indirect corruption occurs because the callee never ran**:

- If the callee was supposed to modify r3 (return value), r3 retains its stale value.
- If the callee was supposed to write to guest memory (e.g., initialize an object),
  that memory remains uninitialized.
- If the callee was supposed to set a flag that terminates a loop, the loop continues
  forever.

## 5. Stack Frame Push/Pop During Failed Indirect Call

**No stack frame is pushed or popped by PPC_CALL_INDIRECT_FUNC itself.** The macro
is pure inline C code -- no host function call occurs on the miss path (only
`fprintf` + `fflush`, which use the host stack, not the PPC stack).

The PPC stack pointer `ctx.r1` is managed by the recompiled function that
*contains* the `bctrl`/`bctr`, not by the macro. A typical recompiled function
does:

```c
// Function prologue (stwu r1, -96(r1))
ctx.r1.s64 = ctx.r1.s64 + -96;
PPC_STORE_U32(ctx.r1.u32 + 96, ...);  // save back-chain

// ... body with bctrl calls ...

// Function epilogue (addi r1, r1, 96)
ctx.r1.s64 = ctx.r1.s64 + 96;
return;
```

The prologue decrements r1 and the epilogue increments it. If a function is called
normally, its prologue+epilogue is balanced. **The macro skip does not break this
balance within a single function invocation.**

## 6. Stack Pointer Drift Analysis -- The Real Danger

### Can repeated MISSING-FUNC calls cause r1 drift?

**Not directly from the macro**, but YES from the caller's control flow.

Consider the scenario from the logs: `0x821911C4` calls `0x000F4000` 42 times.
Address `0x821911C4` is a `bctrl` instruction. The generated code is:

```c
ctx.lr = 0x821911C4;
PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);  // miss -- skipped
// caller continues...
```

The callee at `0x000F4000` was supposed to execute and return. Since it is
skipped, the caller continues as if the call succeeded. If the caller is
in a loop that:

1. Pushes a stack frame (stwu r1, -N(r1)) at its own prologue -- this only
   happens once per function invocation, not per loop iteration.
2. Calls the virtual function via bctrl in the loop body.
3. Checks a return value or flag to decide whether to continue looping.

Then the MISSING-FUNC skip means:
- Step 2 is a no-op (the callee's side effects are lost).
- Step 3 sees stale data and may loop forever.
- **But r1 does not drift per iteration** because the stwu is in the prologue,
  not inside the loop body.

### When CAN r1 drift?

If the loop body contains a `bl` (direct call) to another function that itself
does `stwu; ...; bctrl 0x000F4000; ...; addi r1` -- and the bctrl target was
supposed to early-exit (longjmp, exception) but instead falls through -- the
called function returns normally, and its stack frame is correctly unwound.

**The real stack growth happens on the HOST side.** Each recompiled function is a
native C++ function. If the call graph is deeply recursive (A calls B calls C
calls ...), the HOST stack grows with each native call frame. The MISSING-FUNC
skip prevents the callee from executing, but the HOST caller still has its own
native stack frame active. If the loop calls many other (successful) functions
that are deeply nested, the host stack grows.

### The actual stack overflow mechanism

Based on the log evidence (42x MISSING-FUNC to 0x000F4000 from 0x821911C4,
immediately followed by stack guard hits at 0x70000000):

1. The **PPC stack (r1)** is not necessarily drifting. The 42 calls to 0x000F4000
   happen within the same function invocation of the function containing 0x821911C4.

2. The stack guard pages at 0x70000000+ are in the **guest address space** (PPC
   stack region). The guard page handler in xmemory.cpp commits new pages as r1
   descends. Once it hits 0x705D0000 (end of committed region), it cannot allocate
   more and loops infinitely.

3. The likely chain: the missing virtual function call causes the caller to not
   terminate its current operation, which continues into deeper call chains
   (world loading, particle systems, etc.), which legitimately push the PPC
   stack deeper than the allocated region.

## 7. Function Table Internals

### Structure

The function table is a flat array in guest memory at `IMAGE_BASE + IMAGE_SIZE`
(`0x82000000 + 0x01300000 = 0x83300000`).

### Indexing

`PPC_LOOKUP_FUNC(base, addr)` computes:
```
slot_ptr = base + 0x83300000 + ((addr - 0x82140000) * 2)
func_ptr = *(PPCFunc**)slot_ptr
```

Each 4-byte-aligned guest address maps to an 8-byte slot (host pointer size on
64-bit). The `* 2` factor accounts for the 4-byte guest instruction alignment
to 8-byte host pointer mapping.

### Initialization

1. `Memory::InitializeFunctionTable()` allocates `code_size * 2` bytes at
   `0x83300000` and zero-fills it (all slots = nullptr).
2. `Processor::SetFunction()` writes each recompiled function pointer into both:
   - A C++ `std::unordered_map<uint32_t, PPCFunc*>` (for `Processor::Execute`)
   - The guest memory table (for `PPC_LOOKUP_FUNC` in recompiled code)
3. For addresses outside the code range, `Memory::SetFunction()` silently skips
   the write (logged at DEBUG level).

### In-range but unmapped addresses

If an address is in range (`0x82140000`-`0x829D635C`) but has no registered
function, `PPC_LOOKUP_FUNC` reads a nullptr from the zero-initialized table.
The macro then takes the same `else` (MISSING-FUNC) path.

## 8. Comparison with XenonRecomp (Upstream)

The original XenonRecomp `PPC_CALL_INDIRECT_FUNC` at
`tools/XenonRecomp/XenonUtils/ppc_context.h:115`:

```c
#define PPC_CALL_INDIRECT_FUNC(x) (PPC_LOOKUP_FUNC(base, x))(ctx, base)
```

This has **no range check and no null check**. If `x` is out of range, it reads
garbage from memory and calls it -- immediate segfault. If the slot is nullptr,
it calls nullptr -- also immediate segfault.

RexGlue's version is strictly safer, but the silent fall-through creates a
different class of bug: functions that should have been called are silently
skipped, leading to corrupted game state and eventual stack overflow from
unbounded loops or missing termination conditions.

## 9. Recommendations

1. **Add r1 logging to MISSING-FUNC**: Print `ctx.r1.u32` in the MISSING-FUNC
   message to detect stack pointer drift in real time.

2. **Add a call counter with abort**: If the same (target, caller) pair fires
   more than N times (e.g., 10), abort or break to prevent infinite loops.

3. **Stub the target address**: For known-bad addresses like `0x000F4000`, hook
   the caller site to return a sane value (e.g., set `ctx.r3.u64 = 0` and skip).

4. **Investigate what 0x000F4000 is**: This address is in the low 1MB of guest
   memory, well below any code section. It is likely a corrupted vtable pointer
   read from an uninitialized object. The root cause is likely a missing
   initialization step earlier in the boot sequence.
