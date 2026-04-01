# 49: Why 42 Calls to 0x000F4000 Appear to Cause Stack Overflow (But Don't)

## Executive Summary

The 42 MISSING-FUNC calls to `0x000F4000` from `0x821911C4` do **NOT** cause the
stack overflow. The stack overflow occurs on the **main thread** (t41614048) due to
the main thread's own deep call graph during world initialization. The 42 audio
thread MISSING-FUNC calls are on a **different thread** and happen to be flushed
to stderr at the same moment the main thread hits its stack guard. The temporal
adjacency in the log is an artifact of stderr buffering across threads.

## 1. The MISSING-FUNC Handler Is Identical for NULL and Non-NULL

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

- `PPC_CODE_BASE` = `0x82140000`, `PPC_CODE_SIZE` = `0x0094635C`
- Valid range: `0x82140000` to `0x829D635C`
- **Both** `0x00000000` (NULL) and `0x000F4000` fail the range check identically
  (`_icf_in_range = false`, `_icf_fn = nullptr`)
- Both take the `else` branch: `fprintf` + `fflush` + fall through
- **Zero difference in behavior.** No extra stack frame, no extra computation,
  no memory access to the function table for either address.
- The handler does NOT modify any PPCContext fields (r3, r1, lr, ctr, cr -- all untouched)

## 2. The Two Call Sites Are on Different Threads

### 0x828C99CC (2.3M NULL calls) -- Main Thread Rendering

**Function**: `sub_828C9980` at `gta4_recomp.59.cpp` line 7357

This function is part of the RAGE audibility/physics system. It:
1. Takes an object array pointer, an index, and a target object
2. Loads `[obj + 0]` (vtable), then `[vtable + 52]` (slot 13) -- a virtual method
3. Calls via `bctrl` at 0x828C99CC
4. On MISSING-FUNC miss (NULL vtable entry), execution continues -- checks return value,
   may call `sub_82147AA8` or `sub_828E0F88`, then calls `sub_828C97E0` (NOT recursive)
5. Returns normally (epilogue at line 7479: `addi r1,r1,128` + `__restgprlr_27`)

**Stack frame**: 128 bytes (PPC) + `__savegprlr_27`/`__restgprlr_27` overhead

**Callers**: Two call sites (lines 16914 and 21586 in gta4_recomp.59.cpp), both in
tight loops that iterate over arrays. Each iteration calls `sub_828C9980`, which
pushes/pops its 128-byte PPC frame and returns. **No recursion, no stack growth
across iterations.**

### 0x821911C4 (42 calls to 0x000F4000) -- Audio Thread

**Function**: `sub_821910D0` at `gta4_recomp.2.cpp` line 1888

This is the XAudio render thread per-frame worker. Called via callback from the audio
thread (sub_821909D0 primary worker loop). Each invocation:
1. Pushes 192-byte PPC stack frame
2. Enters critical section
3. Either waits for events (PATH A) or drains audio buffers (PATH B)
4. Dispatches vtable[17] at `[device+64]` endpoint object -- this is where 0x000F4000 comes from
5. r3 (stale, holds the endpoint object pointer -- a large positive guest address) passes
   the `cmpwi cr6,r3,0` / `blt` check (not negative)
6. Does atomic increment of frame counter
7. Swaps double-buffered audio state
8. Leaves critical section
9. Returns 0

**Stack frame**: 192 bytes (PPC) + `__savegprlr_28`/`__restgprlr_28`

**Thread loop**: The audio thread (sub_821909D0, documented in doc 52) calls sub_821910D0
once per iteration, then loops back to `KeWaitForSingleObject` (infinite timeout). Each
iteration reuses the same stack space. **No stack growth across iterations.**

## 3. Thread Identity Proof

The MISSING-FUNC lines use raw `fprintf(stderr, ...)` which does NOT include a thread ID.
The stack guard lines use the RexGlue logging system which DOES include `[t41614048]`.

```
[MISSING-FUNC] indirect call to 00000000 (in_range=0) from 828C99CC     <-- no thread ID
[MISSING-FUNC] indirect call to 00000000 (in_range=0) from 828C1F94     <-- no thread ID
[VBlank-FIX] Allocated stub at guest 0x12BD0000 for device[+10900]
[MISSING-FUNC] indirect call to 000F4000 (in_range=0) from 821911C4     <-- no thread ID
... (42 lines) ...
[warning] [sys] [t41614048] Stack guard page hit at guest 0x70000000     <-- MAIN thread
```

Thread t41614048 is the **main game thread** -- it opens RPFs (`common.rpf`,
`xbox360.rpf`, `audio.rpf`), loads world data, initializes particle systems. The
stack overflow is on THIS thread, NOT on the audio thread that makes the 0x000F4000 calls.

## 4. What Actually Causes the Stack Overflow on the Main Thread

The main thread (t41614048) is deep inside world initialization when the stack guard
pages are hit. At line 81823 (immediately after the 42 MISSING-FUNC lines), the guard
handler starts firing at `0x70000000` and walks upward page by page:

```
0x70000000, 0x70030000, 0x70040000, ... 0x705C0000, 0x705D0000 (FAIL)
```

The stack grows downward. The initial stack allocation for the main thread is at the TOP
of the 0x70000000-0x7F000000 range. The guard pages extend DOWN. When the main thread's
deep call chain (world loading, particle emitter registration, streaming system init)
pushes the PPC r1 stack pointer below the committed region, the guard handler keeps
unprotecting pages until it reaches uncommitted memory at 0x705D0000 where
`BaseHeap::Protect` fails.

The root cause of the main thread's excessive stack usage is the game's init sequence,
NOT the audio thread's MISSING-FUNC calls. The 2.3M NULL calls from 0x828C99CC also
come from the main thread (rendering loop), but they don't cause stack growth because
each call properly returns and the stack frame is reclaimed.

## 5. Why 2.3M Calls Don't Overflow

### sub_828C9980 (the 0x828C99CC caller) call chain

```
caller (loop over array)
  -> sub_828C9980     stwu r1,-128(r1)  ... addi r1,r1,128  return
      -> PPC_CALL_INDIRECT_FUNC(NULL)  -- fprintf + return (no stack change)
      -> sub_828C97E0  stwu r1,-160(r1) ... addi r1,r1,160  return
          -> PPC_CALL_INDIRECT_FUNC(NULL)  -- fprintf + return
          -> sub_828C8A50  (called, returns)
  (back to caller, loop to next element)
```

Each iteration:
1. Pushes 128 bytes (sub_828C9980) + 160 bytes (sub_828C97E0) = 288 bytes max nested depth
2. Both frames are **completely unwound** before the next iteration
3. The host C++ stack mirrors this: each `PPC_FUNC_IMPL` function is a normal C++ function
   that returns after its PPC function body completes
4. Net stack growth per iteration: **zero**

### sub_821910D0 (the 0x821911C4 caller) call chain

```
audio thread entry (sub_821909D0 loop)
  -> sub_821910D0     stwu r1,-192(r1) ... addi r1,r1,192  return
      -> RtlEnterCriticalSection (kernel call, returns)
      -> KeSetEvent (kernel call, returns)
      -> KeWaitForMultipleObjects (blocks, then returns)
      -> PPC_CALL_INDIRECT_FUNC(0x000F4000) -- fprintf + return
      -> RtlLeaveCriticalSection (kernel call, returns)
  (back to audio thread loop, KeWaitForSingleObject blocks)
```

Each iteration:
1. Pushes 192 bytes + several kernel calls (~200 bytes host stack each)
2. All frames are **completely unwound** before the next iteration
3. `KeWaitForSingleObject` in the outer loop actually yields the host thread
4. Net stack growth per iteration: **zero**

## 6. Is 0x000F4000 Mapped to Something?

No. Address 0x000F4000 is in the `v00000000` heap (guest virtual, 4K pages,
0x00000000-0x3FFFFFFF). This is ordinary user virtual memory. It may or may not
have been committed by `NtAllocateVirtualMemory`. But `PPC_CALL_INDIRECT_FUNC`
never reads from this address -- the range check fails immediately
(`0x000F4000 < 0x82140000`), so `_icf_fn` is set to `nullptr` without any
memory access to the function table or to address 0x000F4000 itself.

The value 0x000F4000 was already read from guest memory BEFORE entering
`PPC_CALL_INDIRECT_FUNC` -- it was loaded at line 2019 (`lwz r11,68(r11)`)
as part of the vtable dispatch chain:
```
obj     = [r30 + 64]      // audio endpoint object
vtable  = [obj + 0]        // vtable pointer
method  = [vtable + 68]    // = 0x000F4000 (garbage)
```

The memory access to read 0x000F4000 from the vtable happened successfully
(no fault), meaning the vtable memory is committed but contains garbage data
from an uninitialized XAudio2 COM endpoint object (see doc 58 for details).

## 7. Stack Consumption Per MISSING-FUNC Call

The `PPC_CALL_INDIRECT_FUNC` macro is inlined into the caller. On the miss path,
the only function calls are `fprintf` and `fflush`:

- `fprintf(stderr, ...)` -- pushes a host stack frame (~100-200 bytes depending
  on platform/compiler), formats the string, writes to stderr, returns
- `fflush(stderr)` -- minimal host stack (~64 bytes)

These are transient -- they push and pop their own host stack frames within the
`do { ... } while(0)` block. The **net host stack cost** after the macro completes
is **zero bytes** (same as if the call had succeeded and the callee had returned).

There is no difference in host stack cost between:
- A NULL target miss
- A 0x000F4000 target miss
- Any other out-of-range target miss

## 8. Conclusion: The Premise Is Wrong

The question's premise -- "42 MISSING-FUNC calls to 0x000F4000 cause a fatal stack
overflow" -- is incorrect. The evidence shows:

1. **The MISSING-FUNC handler is identical for NULL and non-NULL targets.** Both
   print, flush, and return. Zero stack difference.

2. **The 42 calls and the stack overflow are on different threads.** The 0x000F4000
   calls are on the audio thread. The stack overflow is on main thread t41614048.

3. **Neither the 2.3M NULL calls nor the 42 0x000F4000 calls cause stack growth.**
   Both callers properly push/pop PPC and host stack frames per iteration. Net
   growth is zero.

4. **The stack overflow is caused by the main thread's deep call graph** during
   world initialization (particle emitter registration, streaming system init,
   RPF loading). This is a separate issue from the audio vtable corruption.

5. **The temporal adjacency in the log is coincidental.** Multiple threads write
   to stderr concurrently. The audio thread's MISSING-FUNC lines appear just
   before the main thread's stack guard lines because stderr buffering happens
   to flush them in that order.

### What IS the problem

There are TWO independent bugs:

| Bug | Thread | Symptom | Root Cause |
|-----|--------|---------|------------|
| Corrupted XAudio vtable | Audio thread | 42x MISSING-FUNC to 0x000F4000 | Endpoint object at `[device+64]` has uninitialized vtable (XAudio2 COM methods not populated by recomp) |
| PPC stack overflow | Main thread (t41614048) | Stack guard expansion 0x70000000-0x705D0000, then infinite loop at 0x705D0000 | Main thread's world init call graph exceeds allocated PPC stack (deep nesting of recompiled functions during particle/streaming/RPF init) |

The audio vtable bug should be fixed by hooking sub_821910D0 to skip the vtable
dispatch (the recomp's SDL2 audio backend renders natively -- see doc 53).

The main thread stack overflow should be fixed by either:
- Increasing the main thread's PPC stack allocation
- Reducing call depth in the recompiled init sequence
- Adding stack limit checks in the guard page handler to abort gracefully instead of looping
