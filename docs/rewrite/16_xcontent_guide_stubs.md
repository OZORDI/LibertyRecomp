# XContent Container Operations & Xbox Guide UI Stubs

## Overview

The save state machine calls five Xbox 360-specific functions that must be
stubbed or replaced for the PC recompilation. Three operate on XContent
containers (the Xbox 360 save-game file format), one shows the Xbox Guide
device-selector UI, and two poll async I/O completion.

All five functions operate on a per-slot async-operation struct whose fields
are documented below. The slot array base address is `0x83192C58` (computed
from `lis r11,-31975; addi r11,r11,11352`). Each slot is 160 bytes
(stride 0xA0), indexed by user/container index.

---

## Async-Operation Struct Layout

Every function takes `r3` = pointer to an async-op struct:

| Offset | Size | Field | Notes |
|--------|------|-------|-------|
| +0     | 4    | state | 0=idle/ready, 1=error, 2=IO pending (create), 3=done-ok, 4=IO pending (write), 5=done-ok-write |
| +4     | 4    | bytes_transferred | Filled on completion |
| +8     | 28   | OVERLAPPED | Xbox XOVERLAPPED struct (status at +0, info at +4, hEvent at +12) |
| +32    | 4    | extended_error | Error code from completed op |
| +36-55 | 20   | reserved / padding | |
| +56    | 4    | content_handle | Handle returned by XamContentCreate (or -1 if none) |
| +60    | 4    | file_handle | Handle returned by NtCreateFile within content (or -1) |

---

## Function 1: sub_8223F9F0 (Xbox Guide UI / Device Selector)

**File:** `gta4_recomp.6.cpp` line 77883
**Called by:** Save state machine states 5, 7, 13 (and many others -- 20+ call sites)

### Parameters
- `r3` (uint32_t): Dialog/action ID (0--53, used as switch index)
- `r4` (ptr): String pointer (content path/name, varies by case)
- `r5` (ptr): Output flag byte pointer (set to 1 on success)

### What It Does
Giant switch statement (54 cases). For save-related paths:
- **Case 0**: Calls `sub_82254FE0` (XamShowDeviceSelectorUI wrapper) with
  content type = 72, device = -1 (any), then calls `sub_8224FFC8` to show
  the Guide blade UI. Waits for user to select storage device.
- **Case 2**: Checks online flag at `0x82BF981F`, shows different messages
  based on Xbox LIVE silver/gold status.
- **Other cases (3-53)**: Various confirmation dialogs, error messages,
  content browser screens.

### Xbox 360 APIs Called (Indirectly)
1. `sub_82254FE0` -> sets up UI state globals, calls `sub_8224CD48`, `sub_8224CD88`
2. `sub_8224FFC8` -> large function that invokes the Guide blade overlay via internal XAM APIs
3. Both ultimately trigger `XamShowDeviceSelectorUI` (ordinal 0x2CB)

### Return
- `r3` = 1 on success (dialog completed, user made selection)
- `r3` = 1 also when jumping to `loc_8223FC04` (early-out with flag set)
- The output byte at `[r5]` is set to 1 when the flow completes

### RexGlue Status
`XamShowDeviceSelectorUI` is **already implemented** in
`src/kernel/xam/xam_ui.cpp` line 489. It runs headless: immediately writes
device_id = 0x00000001 (dummy HDD) and returns `X_ERROR_SUCCESS`. No actual
UI is shown.

### Minimal Stub Strategy
Hook `sub_8223F9F0` to:
1. Ignore the dialog/action ID entirely
2. Write 1 to the output flag byte (`[r5] = 1`)
3. Return `r3 = 1`

This bypasses all Guide UI and pretends the user always accepted/selected
the default storage device.

---

## Function 2: sub_8284A0F8 (XContent Container Create)

**File:** `gta4_recomp.55.cpp` line 9775
**Called by:** State 6 (via wrapper `sub_8284AAE0` at line 11210)

### Parameters
- `r3` (ptr): Async-op struct pointer
- `r4` (ptr): Content path string (e.g., "save:\\")
- `r5` (uint32_t): Content type / size argument (passed to `sub_82A12720`)
- `r6` (bool): If nonzero, adds flag 0x200 (CONTENT_FLAG_OPEN_ALWAYS)
- `r7` (bool): If nonzero, adds flag 0x100 (CONTENT_FLAG_CREATE_NEW)

### What It Does
1. Checks state field at `[r3+0]` -- proceeds only if state == 0 or 1
2. Zeroes out the OVERLAPPED at `[r3+8]` (28 bytes) and content_handle at `[r3+56]`
3. Calls `sub_82A12720(r5)` -- size calculator that computes buffer allocation
   size for the content container (STFS hash table size estimation)
4. Computes user index from struct address (divides offset from base `0x83192C58` by 160)
5. Builds content flags: 0x100 if `r7` nonzero (create), 0x200 if `r6` nonzero (open)
6. Calls `sub_82A116E8` = **`XamShowDeviceSelectorUI`**
   - `r3` = user index (clamped to 0--3, or 255 if >= 4)
   - `r4` = content path
   - `r5` = flags
   - `r6` = size from step 3
   - `r7` = ptr to content_handle at `[r3+56]`
   - `r8` = ptr to OVERLAPPED at `[r3+8]`

   Wait -- actually, tracing more carefully: it calls `sub_82A116E8` which is
   a direct jump to `__imp__XamShowDeviceSelectorUI`. But examining the
   register setup, `r3` is the user index and the rest follows the
   XamShowDeviceSelectorUI signature.

   **Correction after re-reading**: The function name `sub_82A116E8` is the
   game's thunk for `XamShowDeviceSelectorUI`, but in the context of state 6,
   the wrapper `sub_8284AAE0` computes the slot pointer and jumps to
   `sub_8284A0F8`. The actual XamContentCreate call is NOT here -- this
   function only does the device selection step. Let me re-examine...

   Actually, re-reading lines 9893-9895: `sub_82A116E8(ctx, base)` is called.
   `sub_82A116E8` = direct jump to `XamShowDeviceSelectorUI`. So this function
   shows the device selector, gets the device_id, then checks return == 997
   (`ERROR_IO_PENDING`). If pending, sets state = 2 and returns 1.

### Return
- `r3` = 0 if state was not 0/1 (no-op)
- `r3` = 0 if XamShowDeviceSelectorUI returned != ERROR_IO_PENDING (failed)
- `r3` = 1, state = 2 if call returned ERROR_IO_PENDING (async started)

### RexGlue Status
`XamShowDeviceSelectorUI` is implemented headless (returns immediately with
device_id = 0x00000001 and `X_ERROR_SUCCESS`). This means it does NOT return
`ERROR_IO_PENDING`. The function will see return != 997 and return 0 (failure).

### Minimal Stub Strategy
Hook `sub_8284A0F8`:
1. Set state at `[r3+0]` = 2 (pretend async create is pending)
2. Zero the OVERLAPPED, set content_handle to a valid dummy
3. Return `r3 = 1`

Or better: make `XamShowDeviceSelectorUI` return `ERROR_IO_PENDING` when
called with an overlapped, then have `sub_82849C18` see it complete.

---

## Function 3: sub_82849C18 (Async IO Completion Poll -- Create)

**File:** `gta4_recomp.55.cpp` line 9029
**Called by:** State 8 (via wrapper `sub_8284AB10` at line 11247)

### Parameters
- `r3` (ptr): Async-op struct pointer

### What It Does
1. Checks state at `[r3+0]` -- proceeds only if state == 2
2. If state != 2, returns `r3 = 0` (not ready)
3. Calls `sub_82A11EB8(OVERLAPPED_ptr, bytes_out_ptr, wait=0)` which is
   **XOverlappedGetResult** (non-blocking poll):
   - Checks OVERLAPPED.status at `[r3+8]` for value 997 (ERROR_IO_PENDING)
   - If still pending and `wait` arg != 0: calls `sub_82A13040` = `NtWaitForSingleObjectEx` on the OVERLAPPED.hEvent
   - If wait=0 and still pending: returns WAIT_TIMEOUT (258) which maps to **996** (ERROR_IO_INCOMPLETE)
   - If complete (status != 997): copies bytes_transferred to output, returns actual status
4. If poll returns 996 (IO_INCOMPLETE): returns `r3 = 0` (still waiting)
5. If poll returns 0 (SUCCESS): sets state = 3, returns `r3 = 1`
6. If poll returns nonzero error: sets state = 1 (error), returns `r3 = 1`

### Return
- `r3` = 0: Still waiting (IO incomplete) or wrong state
- `r3` = 1: Completed (state set to 3 for success, 1 for error)

### RexGlue Status
`XOverlappedGetResult` is not a separate RexGlue export -- it is inline
recompiled code at `sub_82A11EB8`. The OVERLAPPED completion mechanism
depends on `NtWaitForSingleObjectEx` and the kernel event system.

### Minimal Stub Strategy
Hook `sub_82849C18`:
1. If state == 2: set state = 3 (success), return `r3 = 1`
2. Otherwise: return `r3 = 0`

This instantly completes the "create container" async operation.

---

## Function 4: sub_8284A1E8 (XContent Container Write / Enumerate)

**File:** `gta4_recomp.55.cpp` line 9915
**Called by:** State 9 (via wrapper `sub_8284ABA0` at line 11340)

### Parameters
- `r3` (ptr): Async-op struct pointer
- `r4` (ptr): Buffer to write (or enumerate results buffer)
- `r5` (uint32_t): Buffer size / count
- `r6` (uint32_t): Additional size parameter

### What It Does
1. Checks state at `[r3+0]` -- proceeds if state == 0 or 1 or if content_handle at `[r3+56]` is nonzero
2. If `r6` (size) == 0: returns 0 (nothing to do)
3. Computes user index from struct address offset (same formula as sub_8284A0F8)
4. Calls `sub_82A12710` = **`XamContentCreateEnumerator`**:
   - `r3` = user index
   - `r4` = content_handle from `[r3+56]`
   - `r5` = buffer/data ptr
   - `r6` = flags (0x1000 XOR'd based on user index)
   - `r7` = OVERLAPPED ptr at `[r3+8]`
   - `r8` = stack local for size output
5. If XamContentCreateEnumerator returns nonzero: error, sets state = 1, CloseHandle on file_handle, returns 0
6. If returns 0 (SUCCESS): calls `sub_82A11F50` = **`XamEnumerate`** with:
   - `r3` = handle from `[r3+60]`
   - `r4` = buffer ptr
   - `r5` = buffer size (from stack output)
   - `r6` = 0 (flags)
   - `r7` = OVERLAPPED ptr
7. If XamEnumerate returns 997 (ERROR_IO_PENDING): sets state = 4, returns `r3 = 1`
8. If XamEnumerate returns other: error path

### Return
- `r3` = 0: Error or nothing to do
- `r3` = 1, state = 4: Write/enumerate async started successfully

### RexGlue Status
Both APIs are **implemented**:
- `XamContentCreateEnumerator` at `src/kernel/xam/xam_content.cpp` line 61
- `XamEnumerate` at `src/kernel/xam/xam_enum.cpp` line 74

Both handle overlapped completion properly via `CompleteOverlappedDeferredEx`.

### Minimal Stub Strategy
Hook `sub_8284A1E8`:
1. Set state = 4 (write/enum pending), return `r3 = 1`

Or: ensure RexGlue's XamEnumerate returns ERROR_IO_PENDING with the
overlapped, and let sub_82849C98 detect completion naturally.

---

## Function 5: sub_82849C98 (Async IO Completion Poll -- Write/Enumerate)

**File:** `gta4_recomp.55.cpp` line 9104
**Called by:** State 10 (via wrapper `sub_8284ABD0` at line 11367)

### Parameters
- `r3` (ptr): Async-op struct pointer
- `r4` (ptr): Output result pointer (error code written here on failure)

### What It Does
1. Checks state at `[r3+0]` -- proceeds only if state == 4
2. If state != 4: returns `r3 = 0`
3. Calls `sub_82A11EB8(OVERLAPPED_ptr, bytes_out_ptr, wait=0)` (same
   XOverlappedGetResult as sub_82849C18)
4. If returns 996 (IO_INCOMPLETE): returns `r3 = 0` (still waiting)
5. If returns 0 (SUCCESS): sets state = 5, writes bytes_transferred to
   `[r4]`, calls `CloseHandle([r3+60])`, sets `[r3+60]` = -1, returns `r3 = 1`
6. If returns nonzero error:
   - Checks if error == 1223 (ERROR_CANCELLED) -> go to error path
   - Checks if error == 0x80070012 (HRESULT_FROM_WIN32(ERROR_NOT_READY)) -> go to success path
   - Otherwise -> error path: state = 1, writes -1 to `[r4]`, CloseHandle, returns `r3 = 1`

### Return
- `r3` = 0: Still waiting or wrong state
- `r3` = 1: Completed (state = 5 for success, state = 1 for error, result in `[r4]`)

### RexGlue Status
Same as sub_82849C18 -- relies on the inline `XOverlappedGetResult` polling
which depends on kernel event signaling. RexGlue's overlapped completion
should signal the event, making this poll succeed.

### Minimal Stub Strategy
Hook `sub_82849C98`:
1. If state == 4: set state = 5 (success), write 0 to `[r4]`, return `r3 = 1`
2. Otherwise: return `r3 = 0`

---

## Summary: Xbox APIs in the Call Chain

| Game Function | Xbox 360 API | Ordinal | RexGlue Impl? | Notes |
|---------------|-------------|---------|---------------|-------|
| sub_82A116E8 | XamShowDeviceSelectorUI | 0x2CB | YES (headless) | Returns device_id=1 immediately |
| sub_82A12710 | XamContentCreateEnumerator | 0x25C | YES | Lists save content |
| sub_82A11F50 | XamEnumerate | 0x250 | YES | Enumerates content items |
| sub_82A12720 | (size calculator) | N/A | N/A | Computes STFS buffer size, pure math |
| sub_82A11EB8 | XOverlappedGetResult (inline) | N/A | Recompiled | Polls OVERLAPPED.status, waits on hEvent |
| sub_82A13040 | NtWaitForSingleObjectEx wrapper | N/A | YES (kernel) | Wait with timeout=0 |
| sub_82A12950 | RtlGetLastError equivalent | N/A | Recompiled | Reads TLS error code |

## Known Issue: Headless XamShowDeviceSelectorUI

The current RexGlue `XamShowDeviceSelectorUI` returns `X_ERROR_SUCCESS`
synchronously, NOT `X_ERROR_IO_PENDING`. This means `sub_8284A0F8` will see
the return value != 997 and return 0 (failure), blocking the state machine.

**Fix options:**
1. Hook `sub_8284A0F8` directly to bypass the device selector entirely
2. Modify `XamShowDeviceSelectorUI` to return `X_ERROR_IO_PENDING` when
   overlapped is provided, and immediately complete the overlapped
3. Hook both `sub_8284A0F8` and `sub_8284A1E8` plus their completion
   counterparts to short-circuit the entire async flow

Option 3 (four hooks) is the most robust since it eliminates all Xbox async
I/O dependency.

## Recommended Stub Set (4 hooks)

```cpp
// sub_8284A0F8: XContent create -> instant success
PPC_FUNC_HOOK(sub_8284A0F8) {
    uint32_t state = PPC_LOAD_U32(ctx.r3.u32 + 0);
    if (state == 0 || state == 1) {
        PPC_STORE_U32(ctx.r3.u32 + 0, 2);  // state = IO_PENDING_CREATE
        ctx.r3.s64 = 1;  // success
    } else {
        ctx.r3.s64 = 0;  // no-op
    }
}

// sub_82849C18: create completion poll -> instant done
PPC_FUNC_HOOK(sub_82849C18) {
    uint32_t state = PPC_LOAD_U32(ctx.r3.u32 + 0);
    if (state == 2) {
        PPC_STORE_U32(ctx.r3.u32 + 0, 3);  // state = DONE_OK
        ctx.r3.s64 = 1;
    } else {
        ctx.r3.s64 = 0;
    }
}

// sub_8284A1E8: XContent write/enum -> instant pending
PPC_FUNC_HOOK(sub_8284A1E8) {
    uint32_t state = PPC_LOAD_U32(ctx.r3.u32 + 0);
    if (state == 0 || state == 1 || PPC_LOAD_U32(ctx.r3.u32 + 56) != 0) {
        if (ctx.r6.u32 != 0) {
            PPC_STORE_U32(ctx.r3.u32 + 0, 4);  // state = IO_PENDING_WRITE
            ctx.r3.s64 = 1;
        } else {
            ctx.r3.s64 = 0;
        }
    } else {
        ctx.r3.s64 = 0;
    }
}

// sub_82849C98: write completion poll -> instant done
PPC_FUNC_HOOK(sub_82849C98) {
    uint32_t state = PPC_LOAD_U32(ctx.r3.u32 + 0);
    if (state == 4) {
        PPC_STORE_U32(ctx.r3.u32 + 0, 5);  // state = DONE_OK_WRITE
        PPC_STORE_U32(ctx.r4.u32 + 0, 0);  // result = success
        ctx.r3.s64 = 1;
    } else {
        ctx.r3.s64 = 0;
    }
}
```

For `sub_8223F9F0`, the existing passthrough hook in `imports.cpp` (line 2035)
lets the recompiled code run. If it blocks, a replacement hook should:
```cpp
PPC_FUNC_HOOK(sub_8223F9F0) {
    PPC_STORE_U8(ctx.r5.u32 + 0, 1);  // output flag = done
    ctx.r3.s64 = 1;                    // success
}
```
