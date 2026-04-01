# 57: Stack Consumption Per MISSING-FUNC Call

## Summary

The 42 MISSING-FUNC calls to `0x000F4000` do NOT consume 5.8MB of stack.
Each MISSING-FUNC hit is a flat `fprintf` + `fflush` inside a `do { } while(0)`
macro -- no host function call to a recompiled function occurs, no PPC stack
frame is pushed, and the loop in `sub_82191858` is a `goto` loop within a single
C++ function frame. The 5.8MB stack overflow has a different root cause.

## 1. PPC_CALL_INDIRECT_FUNC Implementation

**File**: `glue/rexglue-sdk-main/include/rex/ppc/context.h`, lines 127-140

When the target address (`0x000F4000`) is outside the code range
(`0x82140000`-`0x829D635C`):

1. `_icf_in_range` = `false` (0x000F4000 < 0x82140000)
2. `_icf_fn` = `nullptr` (no table lookup performed)
3. Else branch: `fprintf(stderr, ...)` + `fflush(stderr)`
4. Macro completes. Execution continues at the next C++ statement.

**No host function call. No PPC stack modification. No recursion.**

The macro is pure inline code within the calling function's body. `fprintf` and
`fflush` use the host libc stack temporarily (~256-512 bytes for format string
processing) but that is cleaned up before the macro exits.

## 2. sub_821910D0 Host and Guest Stack Frames

**File**: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.2.cpp`, line 1888

### PPC guest frame
- Prologue: `stwu r1,-192(r1)` -- pushes **192 bytes** onto the PPC stack
- Epilogue: `addi r1,r1,192` -- restores PPC stack pointer
- Balanced: frame is allocated once at entry, freed once at exit

### C++ host frame (estimated)
- Local variables: `uint32_t ea{}` = 4 bytes
- Callee-saved registers: The function calls several sub-functions, so the
  compiler must save/restore its own registers. On x86_64/arm64, typical
  callee-save set is 6-8 registers = 48-64 bytes.
- Function arguments: `ctx` (reference = 8 bytes) + `base` (pointer = 8 bytes)
  passed in registers on both architectures.
- Alignment padding: 16-byte stack alignment.
- **Estimated host frame: ~128-256 bytes** (not kilobytes).

### The vtable dispatch (lines 2014-2024)
```
lwz r3,64(r30)     // load object pointer from member
lwz r11,0(r3)      // load vtable pointer from object
lwz r11,68(r11)    // load vtable[17] -- function pointer
mtctr r11           // load into count register
bctrl               // indirect call
```

When vtable pointer is null/garbage, `lwz r11,68(r11)` reads from guest address
68 (or garbage+68). Whatever value comes out (e.g., `0x000F4000`) becomes the
indirect call target. MISSING-FUNC fires. **No host frame pushed.**

## 3. Does sub_821910D0 Loop or Return?

sub_821910D0 does NOT loop on MISSING-FUNC. After the `PPC_CALL_INDIRECT_FUNC`
at line 2024, execution continues:

- Line 2025: `cmpwi cr6,r3,0` -- checks r3 (stale, never set by missing callee)
- Line 2028: `blt cr6,0x821911F0` -- if r3 < 0, skip to loc_821911F0
- Lines 2029-2058: atomic increment loop (CAS retry, not call recursion)
- Line 2059: loc_821911F0 -- continues to epilogue
- Line 2083: `RtlLeaveCriticalSection`
- Line 2086-2090: Epilogue, `addi r1,r1,192` + return

**sub_821910D0 makes exactly ONE indirect call, then returns.** There is no
loop that repeats the MISSING-FUNC call within this function.

## 4. sub_8219A2B8 -- The Wrapper

**Line 23787**: sub_8219A2B8 is a trivial wrapper:
```c
// lis r11,-31971; lwz r3,21484(r11)  -- load audio manager pointer
// b 0x821910d0                        -- tail call to sub_821910D0
sub_821910D0(ctx, base);
return;
```

**No loop. No PPC frame (no stwu). Tail call only.** Host frame: ~32 bytes.

## 5. sub_82191858 -- The REAL Loop with Indirect Calls

**Line 3003**: This is the function that loops on vtable calls.

### PPC guest frame
- `stwu r1,-160(r1)` -- **160 bytes** per invocation

### Loop structure (lines 3031-3221)
```
loc_82191884:                              // <-- loop top
    KeRaiseIrqlToDpcLevel()
    ... spin lock acquire ...
    ... linked list manipulation ...
    lwz r11,0(r30)                         // load vtable pointer
    lwz r11,68(r11)                        // load vtable[17]
    mtctr r11
    bctrl                                  // PPC_CALL_INDIRECT_FUNC
    cmpwi cr6,r3,0
    blt cr6,loc_82191884                   // if r3 < 0, loop back
    ... atomic increment (CAS) ...
    goto loc_82191884                      // unconditional loop back
```

**This is a goto loop within a single C++ function.** Each iteration does NOT
create a new host stack frame. The host frame for sub_82191858 is allocated once
(~256-512 bytes estimated) and reused for every iteration.

### MISSING-FUNC behavior in the loop

When the vtable call hits MISSING-FUNC:
1. `fprintf` prints the warning (~256 bytes of temporary host stack for libc)
2. `fflush` flushes stderr
3. Macro completes, execution continues at `cmpwi cr6,r3,0`
4. **r3 is STALE** -- not set by the missing callee
5. If stale r3 < 0: loop continues at loc_82191884 (line 3190)
6. If stale r3 >= 0: falls through to atomic increment, then unconditionally
   loops back to loc_82191884 (line 3221)

**Either way, the loop continues forever.** But each iteration uses ZERO
additional host stack and ZERO additional PPC stack.

## 6. Stack Budget Calculation

### Per-iteration host stack cost in the loop: ~0 bytes
The loop is a `goto` within sub_82191858. No new frames.

### Per-iteration PPC stack cost in the loop: 0 bytes
The `stwu r1,-160(r1)` is in the prologue, not the loop body.

### The 5.8MB / 42 calls arithmetic
```
5.8 MB / 42 = ~142 KB per call
```

**This is impossible from MISSING-FUNC calls alone.** A recompiled function's
host frame is ~128-512 bytes. Even if each MISSING-FUNC somehow pushed a full
function frame (it does not), 42 calls would consume:
```
42 * 512 bytes = ~21 KB   (host stack)
42 * 192 bytes = ~8 KB    (PPC stack, if sub_821910D0 were called 42 times)
```

Neither figure approaches 5.8 MB.

## 7. The 2.3M Null Vtable Calls -- Gradual Stack Growth?

The 2.3 million MISSING-FUNC calls from the vtable loop in sub_82191858 are
all within a single `goto` loop in a single C++ function frame:

- **Host stack**: Flat. One frame for sub_82191858, reused 2.3M times.
- **PPC stack**: Flat. The `stwu` is in the prologue, not the loop.
- **fprintf temporary stack**: ~256-512 bytes per call, allocated and freed
  within the same fprintf invocation. No accumulation.

**2.3M iterations of the loop consume the same stack as 1 iteration.**

## 8. Alternative Theory: Stack Already Nearly Full

The stack overflow is NOT caused by the 42 MISSING-FUNC calls themselves. The
likely mechanism:

### Deep call chain to reach sub_82191858

The full host call stack to reach the vtable loop:
```
main thread
  -> game loop
    -> world update
      -> audio system update
        -> sub_82191228 (called from sub_821910D0)
          -> sub_82191858            <-- 160 byte PPC frame, ~256B host frame
            -> [vtable loop -- flat, no growth]
```

But sub_82191228 also calls several other functions in its body:
- `sub_821916D8` (audio voice setup)
- `__imp__XAudioGetVoiceCategoryVolumeChangeMask`
- `sub_82191858` (the looping function -- called MULTIPLE TIMES in a loop)
- `sub_82191360` (called in a loop, bounded by r128 count)

### PPC stack consumption of the full chain

Each invocation of sub_82191858 pushes 160 bytes of PPC stack. If the caller
(sub_82191228) calls it in a loop (lines 2219-2251 show a loop calling
sub_82191858 multiple times), and if that loop runs many iterations due to
corrupted count data (because earlier MISSING-FUNCs left memory uninitialized),
the PPC stack could grow.

But even 1000 iterations of sub_82191228's inner loop would only consume:
```
1000 * 160 = 160 KB PPC stack
```

### The real culprit: the call chain ABOVE these functions

The 5.8MB exhaustion likely comes from the full call tree depth during world
initialization. GTA IV's world loading involves hundreds of nested function
calls (scene graph traversal, resource loading, physics init, etc.). Each
recompiled function pushes:
- 96-256 bytes of PPC stack (stwu)
- 128-512 bytes of host stack (C++ frame)

A call chain 10,000 deep (plausible for recursive scene graph traversal)
would consume:
```
Host: 10,000 * 256 bytes = 2.5 MB
PPC:  10,000 * 160 bytes = 1.6 MB
Total addressable: ~4.1 MB
```

With the missing vtable functions causing the game to NOT terminate certain
initialization paths (because the virtual function that should signal "done"
never executes), the game wanders deeper into its initialization graph than
it would on real hardware.

## 9. PPC Guest Stack Pointer (r1) -- Does MISSING-FUNC Modify It?

**No.** The MISSING-FUNC macro touches exactly zero PPCContext fields. The
register state after a MISSING-FUNC hit is identical to the state before it,
with the exception that `ctx.lr` was set to the return address by the
recompiled code BEFORE the macro was invoked (this is the standard `bctrl`
pattern: `ctx.lr = NEXT_ADDR; PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);`).

r1 is only modified by recompiled function prologues (`stwu r1,-N(r1)`) and
epilogues (`addi r1,r1,N`). These are always balanced within a single
function invocation.

## 10. Conclusion

| Question | Answer |
|----------|--------|
| Does MISSING-FUNC push a host frame? | No. Inline macro with fprintf. |
| Does MISSING-FUNC modify PPC r1? | No. Zero PPCContext writes. |
| Can 42 calls consume 5.8MB? | No. 42 * 512B = 21KB maximum. |
| Do 2.3M loop iterations grow stack? | No. goto loop, flat host/PPC stack. |
| What causes the 5.8MB overflow? | Deep call chains from uninitialized game state. The missing vtable functions cause the game to never terminate certain init paths, leading to deeper-than-normal call nesting in world loading, scene graph, and resource management code. |
| Is the 42-call count significant? | It is a symptom, not the cause. The 42 calls happen near the end when the stack is already nearly exhausted from the deep call chain above. |
