# Deep Analysis: sub_821910D0 (XAudio Render Thread Worker)

## 1. Function Identity

**Address**: 0x821910D0
**File**: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.2.cpp` line 1888
**Size**: ~200 instructions (0x821910D0 - 0x82191224)

This is the **XAudio rendering/mixing thread worker function**. It is NOT a loop — it executes once per invocation, processes one audio frame, and returns. The repeated 0x000F4000 calls are from the caller invoking this function in a loop, not from an internal loop.

## 2. Callers

Only one caller found:

**sub_8219A2B8** (line 23787 in gta4_recomp.2.cpp) — a thin wrapper that:
1. Loads a global object pointer from `[0x831D53EC]` into r3
2. Tail-calls `sub_821910D0(obj)`

sub_8219A2B8 is registered in the function table at 0x8219A2B8. No other callers reference sub_821910D0 directly. sub_8219A2B8 is likely the **thread entry point** passed to `ExCreateThread` / `PsCreateSystemThreadEx`, making sub_821910D0 the actual work function called on each iteration of whatever drives the audio thread.

## 3. Full Control Flow

```
sub_821910D0(r3 = context_obj):
    r28 = 0x82B28338          // critical section base
    r29 = 0                   // constant zero
    r31 = 0                   // "shutdown requested" flag
    r30 = [0x831D53EC]        // global audio device object pointer

    RtlEnterCriticalSection(r28 + 4)    // 0x82B2833C

    if ([r30 + 304] == 0):             // "pending work count" == 0?
        goto PATH_B                     // no pending work -> call sub_8218FFB0
    else:
        goto PATH_A                     // pending work -> wait for events

--- PATH A (pending work exists) ---
    KeSetEvent(0x831D52F0, 1, 0)        // signal "frame ready" event

    // Build wait-object array on stack:
    //   stack[80] = 0x831D52CC          // wait object 1 (semaphore/event)
    //   stack[84] = 0x831D5310          // wait object 2 (shutdown event?)
    //   stack[96] = 0                   // timeout = NULL (infinite)

    sub_829FF840(stack+100, ...)         // likely KeInitializeTimerEx or helper

    KeWaitForMultipleObjects(
        count=2, objects=stack+80,
        wait_type=1 (WaitAny),
        reason=3 (Executive),
        mode=1 (KernelMode),
        alertable=0, timeout=NULL,
        wait_block=stack+96
    )

    if (result == 1):                   // second object signaled = shutdown
        r31 = 1                         // set shutdown flag
    // falls through to DISPATCH

--- PATH B (no pending work) ---
    sub_8218FFB0(r30)                   // process DPC queue / drain audio buffers
    sub_82191228(r30, 1)                // render one audio frame (see below)
    // falls through to DISPATCH

--- DISPATCH (loc_8219119C) ---
    if (r31 & 0xFF != 0):              // shutdown requested?
        goto CLEANUP                    // skip vtable call

    // *** THE VTABLE DISPATCH ***
    obj = [r30 + 64]                    // audio endpoint / voice object
    vtable = [obj + 0]                  // vtable pointer
    method = [vtable + 68]              // virtual method at offset 0x44 (17th slot)
    result = method(obj)                // CALL — this is where 0x000F4000 comes from

    if (result < 0):                    // HRESULT failure?
        goto CLEANUP

    // Atomic increment of counter at 0x831D53F0:
    loc_821911D4:                       // lwarx/stwcx retry loop
        old = atomic_load(0x831D53F0)
        new = old + 1
        if (!CAS(0x831D53F0, old, new)):
            goto loc_821911D4           // retry (this is the ONLY loop)

--- CLEANUP (loc_821911F0) ---
    [r30 + 300] = 0                     // clear thread-id / owner field

    // Copy/swap array at 0x831D53F0 area (7 iterations, stride 4):
    loc_821911F8:                       // loop i = 0..6
        val = [i*4 + (r31-28)]          // read from "new" buffer
        [i*4 + (r31-28)] = 0            // clear "new" buffer
        [i*4 + r31] = val               // write to "current" buffer

    RtlLeaveCriticalSection(r28 + 4)
    return 0
```

## 4. Is There a Loop?

**No internal loop drives the vtable call.** The function has exactly two small loops:

1. **loc_821911D4**: The lwarx/stwcx atomic CAS retry loop — this retries only on CAS failure (another CPU modified the word simultaneously). Typically completes in 1-2 iterations. This is standard PPC atomic increment.

2. **loc_821911F8**: A fixed 7-iteration copy loop that swaps two 28-byte buffers (audio level data).

The vtable dispatch at 0x821911C4 executes exactly **once** per call to sub_821910D0. The 42 repeated calls to 0x000F4000 seen in the crash log come from the **caller** invoking sub_821910D0 repeatedly (the audio thread's main loop), not from any loop within this function.

## 5. The Vtable Dispatch Pattern

```
obj = [r30 + 64]       // r30 = audio device object at [0x831D53EC]
vtable = [obj + 0]      // first field of obj = vtable pointer
method = [vtable + 68]  // 17th virtual method (offset 0x44)
result = method(obj)     // r3 = obj (this pointer), returns HRESULT
```

### What Kind of Object?

This is an **XAudio2 voice/endpoint object**. The evidence:

1. **sub_82191228** (called in PATH B) uses the same `[r30+64]` to call `XAudioGetVoiceCategoryVolumeChangeMask` and `XAudioGetVoiceCategoryVolume` — these are Xbox 360 XAudio kernel exports for managing voice category volumes.

2. **sub_82191228** also calls `KeReleaseSemaphore` on the audio semaphore at 0x831D52DC when `[obj+304] > 1`, which is the "pending buffers" count signaling waiting threads.

3. The vtable offset 68 (0x44) on an XAudio voice object corresponds to a **SubmitPacket / GetStatus / ProcessBuffer** type method — the function that advances audio rendering by one frame.

4. The object at `[r30+64]` is the **IXAudioRenderDriver** or equivalent RAGE audio endpoint interface. RAGE v4 wraps XAudio2 voices in a C++ class with a vtable; offset 64 in the parent audio device object stores the pointer to the active render endpoint.

### Why 0x000F4000?

The call target 0x000F4000 means:
- `obj = [r30 + 64]` loaded a valid-looking pointer
- `vtable = [obj + 0]` loaded from that pointer
- `[vtable + 68]` = 0x000F4000

This is a **corrupted or uninitialized vtable**. The value 0x000F4000 is in low guest memory (below the code segment at 0x82000000). Possible causes:

1. The XAudio render driver object was never fully constructed (missing virtual table initialization)
2. The object was constructed but the vtable pointer was overwritten by a buffer overrun
3. The recompiler's XAudio subsystem stub returns an object whose vtable contains placeholder/zero entries, and 0x000F4000 happens to be whatever sits at `[vtable + 68]` in the stub's layout

## 6. sub_8218FFB0 — DPC-Level Audio Buffer Drain

**Address**: 0x8218FFB0
**File**: `gta4_recomp.1.cpp` line 70175

This function:
1. Calls `KeRaiseIrqlToDpcLevel()` — elevates to DPC level for synchronized access
2. Acquires a spinlock (`KeAcquireSpinLockAtRaisedIrql`) on a global at `0x831E4DB8` (computed: `(-31970 << 16) + 19896`)
3. Stores the current thread (r13) as owner
4. Calls `sub_82190120(r28 + 80)` — processes the main audio packet queue
5. Loops through `[r28+128]` secondary voice slots (stride 44 bytes), calling `sub_82190120` for each
6. Decrements the spinlock recursion count; if it reaches 0, releases the spinlock and lowers IRQL

This is a **DPC-level audio buffer drain** — it processes completed audio packets under spinlock protection. It is the synchronous counterpart to the async KeWaitForMultipleObjects path.

## 7. The Atomic Operations After the Vtable Call

After a successful vtable call (result >= 0), the function atomically increments a counter at `0x831D53F0`:

```asm
loc_821911D4:
    lwarx  r9, 0, r11       // r11 = 0x831D53F0 (atomic counter)
    add    r8, r10, r9       // r8 = 1 + old_value
    stwcx. r8, 0, r11       // try store
    bne    loc_821911D4      // retry on CAS failure
```

This is a standard **atomic increment** of a "frames rendered" or "buffers submitted" counter. The MSR manipulation around it (mfmsr/mtmsrd) is the Xbox 360 pattern for disabling/enabling interrupts during the critical section — on recomp this maps to a global lock fence.

## 8. The Buffer Swap Loop (loc_821911F8)

After the atomic increment (or if the vtable call returned < 0), the function:
1. Stores 0 to `[r30 + 300]` (clears the "current thread processing" field)
2. Iterates i = 0 to 6 (7 dwords = 28 bytes), doing:
   - Read `val` from `[i*4 + 0x831D53F0]` ("new frame" audio levels)
   - Clear that slot to 0
   - Write `val` to `[i*4 + 0x831D540C]` ("current frame" audio levels)

This is a **double-buffer swap** of audio rendering state — the frame just rendered becomes the "current" state, and the "new" accumulator is zeroed for the next frame.

## 9. Overall Function Purpose

`sub_821910D0` is the **XAudio render thread's per-frame worker**:

1. Enter critical section (prevent concurrent audio operations)
2. Check if there is pending work (`[obj+304]`)
   - **If pending work**: Signal a "frame ready" event, then wait for either the audio semaphore (new work) or a shutdown event
   - **If no pending work**: Synchronously drain DPC audio buffers (`sub_8218FFB0`), then render one audio frame (`sub_82191228` — processes voice categories, submits packets)
3. Unless shutdown was requested, call the **render endpoint's virtual method** (vtable[17]) to commit/submit the rendered audio frame
4. Atomically increment the frame counter
5. Swap the double-buffered audio state
6. Leave critical section, return 0

## 10. Root Cause of the 0x000F4000 Loop

The function itself does NOT loop. The crash pattern (42 calls to 0x000F4000) means:

1. The **audio thread's outer loop** (in the thread entry, likely sub_8219A2B8 or its parent) calls sub_821910D0 repeatedly
2. Each call reaches the vtable dispatch at 0x821911C4
3. `[r30+64]` points to an XAudio endpoint object whose vtable is corrupted/uninitialized
4. `[vtable+68]` = 0x000F4000 — a nonsense address
5. `PPC_CALL_INDIRECT_FUNC(0x000F4000)` triggers a MISSING-FUNC handler
6. If the MISSING-FUNC handler returns (rather than aborting), the function continues: the return value in r3 is likely 0 or positive, so it passes the `blt` check, does the atomic increment, and returns normally
7. The outer loop calls sub_821910D0 again — repeat

**The fix should target the vtable**, not this function. Either:
- The XAudio render driver object at `[r30+64]` needs to be properly initialized with a valid vtable
- Or sub_821910D0 should be hooked to skip the vtable dispatch when the recomp's audio subsystem handles rendering natively

## 11. Key Globals Summary

| Address | Description |
|---------|-------------|
| 0x82B2833C | Critical section for audio render |
| 0x831D53EC | Pointer to global audio device object (loaded into r30) |
| 0x831D52F0 | "Frame ready" KEVENT |
| 0x831D52CC | Wait object 1 (audio semaphore) |
| 0x831D5310 | Wait object 2 (shutdown event) |
| 0x831D52DC | Audio thread semaphore (released by sub_82191228) |
| 0x831D53F0 | Atomic frame counter (also base of "new" buffer) |
| 0x831D540C | Base of "current" audio state buffer |
| `[r30+64]` | XAudio render endpoint object (holds corrupted vtable) |
| `[r30+300]` | Current processing thread ID |
| `[r30+304]` | Pending work count |

## 12. sub_82191228 — Audio Frame Renderer

Called from PATH B of sub_821910D0 with args `(r30, 1)`.

Key operations:
1. Calls `sub_821916D8(r30, 0)` — likely "begin frame" / prepare render state
2. Reads `[r30+64]` (same endpoint obj), gets `[endpoint+68]` (another vtable-like struct), reads `[struct+24]` as a driver handle
3. Calls `XAudioGetVoiceCategoryVolumeChangeMask(driver_handle, mask_ptr)`
4. Loops over 2 voice categories, calling `XAudioGetVoiceCategoryVolume` for changed categories
5. If `[obj+304] > 1`, calls `KeReleaseSemaphore(0x831D52DC, 1, pending-1, 0)` to wake waiting threads
6. Calls `sub_82191858(r30, r30+80)` — process main voice packet queue
7. Calls `sub_82191360(r30, r30+356)` — process secondary voice queue
8. Loops through `[r30+128]` additional voice slots, processing each

This confirms the entire subsystem is **RAGE's XAudio2 rendering pipeline**.
