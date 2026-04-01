# Fix XamContentCreateEx Return Value: IO_PENDING vs SUCCESS

## Problem Statement

`sub_8284A7E8` calls `XamContentCreateEx` (via `sub_82A127A0` -> `sub_82A12690` ->
`__imp__XamContentCreateEx`) and expects return value 997 (`ERROR_IO_PENDING`).
When the return is 0 (`ERROR_SUCCESS`) instead, the game takes the wrong code
path: state=16 instead of state=17. State 16 skips the file-size-population
sequence (CreateFileA + GetFileSize), causing the size comparison in
`sub_8284ADA0` to fail.

---

## Which Implementation Runs at Runtime

**RexGlue's** `XamContentCreateEx_entry` is the active PPC import handler:

- Defined via `XAM_EXPORT(__imp__XamContentCreateEx, ...)` in
  `glue/rexglue-sdk-main/src/kernel/xam/xam_content.cpp:382`
- This expands to `PPC_HOOK(__imp__XamContentCreateEx, XamContentCreateEx_entry)`
  which creates `extern "C" PPC_FUNC(__imp__XamContentCreateEx)` calling the
  RexGlue handler via `HostToGuestFunction`.
- All generated PPC code (`__imp__XamContentCreateEx(ctx, base)`) calls this.

**Liberty's** `XamContentCreateEx` in `LibertyRecomp/kernel/xam.cpp:380` is a
plain C++ function. It is **never called** from any generated PPC code or from
any other C++ code. It is dead code.

---

## RexGlue Already Returns IO_PENDING

The RexGlue implementation in `xeXamContentCreate()` (line 200-206):

```cpp
if (!overlapped_ptr) {
    uint32_t extended_error, length;
    return run(extended_error, length);    // synchronous
} else {
    kernel_state()->CompleteOverlappedDeferredEx(run, overlapped_ptr.guest_address());
    return X_ERROR_IO_PENDING;             // async
}
```

When `overlapped_ptr` is non-null, it:
1. Queues the work to a dispatch thread via `CompleteOverlappedDeferredEx`
2. Returns `X_ERROR_IO_PENDING` (997) immediately
3. The dispatch thread runs the lambda after ~100ms, then calls
   `CompleteOverlappedEx` which writes the result into the OVERLAPPED struct
   and signals `hEvent` if set.

---

## The Game Passes a Non-Null Overlapped

### Call chain (sub_8284A7E8, lines 10895-10932 in gta4_recomp.55.cpp)

```
r9 = r31 + 8          // OVERLAPPED struct embedded at object+8
                       // (zeroed earlier at lines 10903-10912)
call sub_82A127A0      // wrapper
```

### sub_82A127A0 (gta4_recomp.69.cpp:36620)

```
stw r9, sp+84         // save overlapped ptr on stack for callee
r9 = 0                // clear r9 (cache_size param)
call sub_82A12690      // inner wrapper
```

### sub_82A12690 (gta4_recomp.69.cpp:36436)

```
stwu r1, -96(r1)      // allocate frame
lwz r11, sp+180        // 180 = 84 + 96 = overlapped from parent frame
stw r11, sp+84         // pass overlapped on stack
bl __imp__XamContentCreateEx
```

### __imp__XamContentCreateEx (PPC_HOOK -> HostToGuestFunction)

PPC parameter mapping for `XamContentCreateEx_entry`:

| Ordinal | Register    | Parameter         | Value           |
|---------|-------------|-------------------|-----------------|
| 0       | r3          | user_index        | slot index      |
| 1       | r4          | root_name         | name string ptr |
| 2       | r5          | content_data_ptr  | sp+80           |
| 3       | r6          | flags             | 3 (OPEN_EXISTING) or 18 (CREATE_ALWAYS+flags) |
| 4       | r7          | disposition_ptr   | 0               |
| 5       | r8          | license_mask_ptr  | 0               |
| 6       | r9          | cache_size        | 0               |
| 7       | r10         | content_size      | 0               |
| 8       | stack r1+84 | overlapped_ptr    | r31+8 (non-zero)|

`HostToGuestFunction` reads ordinal 8 from `r1 + 0x54` (= r1 + 84), which is
the overlapped pointer stored by sub_82A12690.

**The overlapped_ptr is non-null (r31+8).** Therefore, `xeXamContentCreate`
should take the async path and return 997.

---

## The Return Value Check in sub_8284A7E8

File: `gta4_recomp.55.cpp` lines 10932-10948:

```
sub_82A127A0(ctx, base);       // XamContentCreateEx
cmplwi cr6,r3,997              // check return == IO_PENDING?
beq cr6, loc_8284A938          // YES: set state=17 (correct)
// NO: falls through...
cmpwi cr6,r27,0                // check r27 param
bne cr6, loc_8284A918          // r27 != 0: different path
li r11,16                      // r27 == 0: state = 16 (WRONG)
stw r26, field[4]              // field[4] = 1
stw r11, field[0]              // write state=16
return 1
```

At `loc_8284A938` (IO_PENDING path):
```
cmpwi cr6,r27,0
bne cr6, loc_8284A948
li r11,17                      // r27 == 0: state = 17 (correct)
stw r11, field[0]              // write state=17
```

### Impact of state=16 vs state=17

In `sub_8284ADA0`:

**State 16** (`loc_8284AF48`): Jumps directly to size comparison. Reads
`field[136]` (expected size) and `field[144]` (actual file size). Since
`field[144]` was set to 0 by sub_8284A7E8 (line 10878) and never populated by
CreateFileA/GetFileSize, the comparison fails.

**State 17** (`loc_8284ADEC`): Checks overlapped completion (via sub_82A11EB8),
then opens the content file via `CreateFileA`, reads its size via
`sub_82A135B0` (GetFileSize wrapper), stores it at `field[144]`, and THEN
proceeds to the size comparison. This is the correct path.

---

## Root Cause Analysis

If the RexGlue implementation is correctly linked and the overlapped pointer is
non-null, the function **should already return 997**. Possible explanations for
why it might return 0:

### Theory 1: Liberty's Dead Code Interferes (UNLIKELY)

Liberty's `XamContentCreateEx` in xam.cpp is a plain C++ function, not a
PPC_FUNC. It cannot intercept `__imp__XamContentCreateEx` calls from generated
code. This would only be an issue if there were a symbol name collision at link
time, but the RexGlue version is `extern "C" void __imp__XamContentCreateEx(PPCContext&, uint8_t*)`
while Liberty's is `uint32_t XamContentCreateEx(...)` -- different signatures,
no collision.

### Theory 2: Build Configuration Issue (POSSIBLE)

If the RexGlue xam_content.cpp is not compiled into the build, the
`PPC_EXTERN_IMPORT(__imp__XamContentCreateEx)` in ppc_func_decls.h would
resolve to a stub that returns 0. Check whether xam_content.cpp is in the
CMakeLists.txt for the RexGlue library.

### Theory 3: Content Doesn't Exist, Error Return (POSSIBLE)

`xeXamContentCreate` with mode=3 (OPEN_EXISTING):
- If content directory does NOT exist, the `run` lambda returns
  `X_ERROR_PATH_NOT_FOUND` (3).
- But the function still returns `X_ERROR_IO_PENDING` to the caller because it
  queues the work via `CompleteOverlappedDeferredEx` regardless of what the
  lambda will return.

So even a failed operation returns 997 to the caller. This theory doesn't
explain the issue.

### Theory 4: Overlapped Pointer is Actually Zero (INVESTIGATE)

If `r31` points to newly allocated/zeroed memory, then `r31+8` is numerically
non-zero but might be in an unmapped region. However, the PPC code zeros the
OVERLAPPED struct AT `r31+8` (lines 10903-10912), proving the memory is
writable. The pointer value `r31+8` is passed through two stack frames and
should arrive intact.

### Theory 5: The OVERLAPPED Address is in PPC Guest Space (INVESTIGATE)

The `ppc_pvoid_t overlapped_ptr` in the RexGlue function receives the value
from `GetIntegerArgumentValue(ctx, base, 8)` which reads from
`r1 + 0x54`. This gives a **guest address** (0x82xxxxxx range). The
`ppc_pvoid_t` type has a `.guest_address()` method that returns this raw value.
`xeXamContentCreate` passes `overlapped_ptr.guest_address()` to
`CompleteOverlappedDeferredEx`. The null check `if (!overlapped_ptr)` checks
the **host pointer** translation. If the guest address is valid and non-zero,
this check should pass.

---

## Proposed Fix (if needed)

### Option A: Verify RexGlue Is Active (Diagnostic First)

Add a printf to RexGlue's `XamContentCreateEx_entry`:

```cpp
ppc_u32_result_t XamContentCreateEx_entry(..., ppc_pvoid_t overlapped_ptr) {
    printf("[RexGlue:XamContentCreateEx] overlapped=0x%08X\n",
           overlapped_ptr.guest_address());
    return xeXamContentCreate(...);
}
```

This confirms whether the RexGlue version is actually called and what
overlapped value it sees.

### Option B: Hook sub_8284A7E8 to Force State=17

If the RexGlue implementation is confirmed to return 0 for some reason,
the simplest fix is a PPC hook:

```cpp
extern "C" void __imp__sub_8284A7E8(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8284A7E8)
{
    __imp__sub_8284A7E8(ctx, base);

    // After the function runs, check if it set state=16 when it should be 17
    uint32_t obj = ctx.r31.u32;  // object pointer (saved by callee)
    uint32_t state = PPC_LOAD_U32(obj);
    if (state == 16) {
        // Force state=17 so sub_8284ADA0 takes the overlapped completion path
        PPC_STORE_U32(obj, 17);
        printf("[sub_8284A7E8 hook] Forced state 16 -> 17\n");
    }
}
```

**Caveat**: r31 is a callee-saved register, so its value after the function
returns is not necessarily the same object pointer. A more reliable approach
would be to inspect the actual state field via the object pointer passed in
r3 (first argument).

### Option C: Fix the Return Value in __imp__XamContentCreateEx (Targeted)

Override `__imp__XamContentCreateEx` to always return IO_PENDING when overlapped
is non-null, even if the underlying implementation returns synchronously:

```cpp
PPC_FUNC(__imp__XamContentCreateEx)
{
    // Read overlapped_ptr from stack (ordinal 8 = r1 + 0x54)
    uint32_t overlapped_guest = __builtin_bswap32(
        *reinterpret_cast<uint32_t*>(base + ctx.r1.u32 + 0x54));

    // Call original RexGlue handler
    rex::HostToGuestFunction<rex::kernel::xam::XamContentCreateEx_entry>(ctx, base);

    // If overlapped was provided but we got a synchronous result,
    // force IO_PENDING and manually complete the overlapped
    if (overlapped_guest != 0 && ctx.r3.u32 != 997) {
        uint8_t* ovl = base + overlapped_guest;
        // Write result to OVERLAPPED.Error
        *reinterpret_cast<uint32_t*>(ovl + 0) = __builtin_bswap32(ctx.r3.u32);
        // Write length (disposition) to OVERLAPPED.Length
        // ... (need to capture disposition value)
        ctx.r3.u32 = 997;  // force IO_PENDING return
    }
}
```

**This approach is fragile** because it needs to properly populate the
OVERLAPPED struct fields that the game will read later. The RexGlue
`CompleteOverlappedDeferredEx` already handles this correctly when it runs
asynchronously.

### Option D: Ensure overlapped_ptr Passes Correctly (Root Cause Fix)

The most robust fix is to confirm the overlapped pointer arrives correctly at
the `HostToGuestFunction` level. Add logging to `GetIntegerArgumentValue` for
ordinal 8 reads, or add a printf inside `xeXamContentCreate`:

```cpp
printf("[xeXamContentCreate] overlapped_ptr.guest_address()=0x%08X host=%p\n",
       overlapped_ptr.guest_address(), (void*)overlapped_ptr);
```

---

## Callers of XamContentCreateEx in Generated Code

All calls go through `sub_82A12690` (the validation wrapper):

| Caller           | Via            | flags (r6) | Expects 997? | State on 997 | State on !997 |
|------------------|----------------|------------|-------------|--------------|---------------|
| sub_8284A320     | sub_82A127A0   | 18 (CREATE)| Yes         | 6            | 1 (fail)      |
| sub_8284A500     | sub_82A127A0   | 3 (OPEN)   | Yes         | 7            | 1 (fail)      |
| sub_8284A7E8     | sub_82A127A0   | 3 (OPEN)   | Yes         | 17           | 16 (skip size)|
| sub_8284B5B8     | sub_82A127A0   | varies     | Yes         | varies       | varies        |
| sub_8284B958     | sub_82A127A0   | varies     | Yes         | varies       | varies        |

**Every single caller expects 997.** Returning 0 (synchronous success) causes
all callers to take the failure/skip path. This confirms that the game was
designed for the asynchronous XamContentCreateEx pattern used on Xbox 360.

**Returning IO_PENDING will NOT break any callers** -- it is the expected return
value for all of them.

---

## OVERLAPPED Completion Flow

When `CompleteOverlappedDeferredEx` runs (after ~100ms delay):

1. Sets `OVERLAPPED.Error = X_ERROR_IO_PENDING` initially
2. Sets `OVERLAPPED.InternalContext = current thread handle`
3. Dispatch thread runs the lambda
4. `CompleteOverlappedEx` writes:
   - `OVERLAPPED.Error = result` (0 for success)
   - `OVERLAPPED.ExtendedError = HRESULT`
   - `OVERLAPPED.Length = disposition` (1=Create, 2=Open)
5. If `OVERLAPPED.hEvent != 0`: signals the event via `XEvent::Set()`
6. If `OVERLAPPED.pCompletionRoutine != 0`: queues APC to requesting thread

The game's OVERLAPPED at `r31+8` is zeroed before the call (hEvent=0,
pCompletionRoutine=0). So only the Error/Length fields need to be written.

In `sub_8284ADA0` state 17 (`loc_8284ADEC`), the game calls `sub_82A11EB8`
to check the OVERLAPPED result. If it returns 996 (`ERROR_IO_INCOMPLETE`),
the function returns 0 (still pending). If it returns 0 (complete), the game
proceeds to open the file and read its size.

---

## Recommended Investigation Order

1. **Add printf in RexGlue's XamContentCreateEx_entry** to confirm it is called
   and see the overlapped value.
2. **Check the build** to ensure `xam_content.cpp` is compiled and linked.
3. If RexGlue IS called with non-null overlapped and returns 997, the bug is
   elsewhere (perhaps in the overlapped completion timing or sub_82A11EB8).
4. If RexGlue is NOT called, check linker output for symbol resolution of
   `__imp__XamContentCreateEx`.

---

## File Inventory

| File | Role |
|------|------|
| `glue/rexglue-sdk-main/src/kernel/xam/xam_content.cpp` | RexGlue XamContentCreateEx (active, returns IO_PENDING when overlapped != null) |
| `glue/rexglue-sdk-main/include/rex/ppc/function.h` | PPC_HOOK macro, HostToGuestFunction, ArgTranslator (stack param at r1+0x54) |
| `glue/rexglue-sdk-main/src/system/kernel_state.cpp` | CompleteOverlappedDeferredEx (async dispatch, hEvent signaling) |
| `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.55.cpp:10785` | sub_8284A7E8 - the caller that sets state=16 vs 17 |
| `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.55.cpp:11651` | sub_8284ADA0 - state machine that reads state 16/17 |
| `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.69.cpp:36434` | sub_82A12690 - XamContentCreateEx validation wrapper |
| `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.69.cpp:36618` | sub_82A127A0 - XamContentCreateEx call wrapper (passes overlapped) |
| `LibertyRecomp/kernel/xam.cpp:380` | Liberty's XamContentCreateEx (dead code, never called by PPC) |
| `LibertyRecomp/kernel/save_hooks.cpp` | Existing PPC function hooks |
