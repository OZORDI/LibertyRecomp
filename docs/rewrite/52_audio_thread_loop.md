# Audio Thread Loop: sub_8219A2B8 and the XAudio Thread Architecture

## 1. sub_8219A2B8 — NOT a Thread Entry Point

**Address**: 0x8219A2B8
**File**: `gta4_recomp.2.cpp` line 23787
**Size**: 3 instructions (tiny wrapper)

```
sub_8219A2B8():
    r11 = 0x831D0000              // lis r11, -31971
    r3  = [r11 + 21484]          // lwz r3, [0x831D53EC]  (global audio device)
    tail-call sub_821910D0(r3)    // b sub_821910D0
```

sub_8219A2B8 is a **callback function**, NOT a thread entry point. It loads the global audio device from `[0x831D53EC]` and tail-calls the per-frame audio worker sub_821910D0. It is **never called from another recompiled function** -- it is only registered as a function pointer.

### How It Gets Called

In sub_82190B48 (the audio system initializer), at address 0x82190F7C:

```
r11 = [r29 + 64]               // audio endpoint object
r10 = 0x8219A2B8               // lis + addi = sub_8219A2B8 address
r4  = r10                      // callback = sub_8219A2B8
r3  = [[r11 + 68] + 0]         // endpoint's vtable
method = [[r3] + 28]           // vtable slot 7 (offset 0x1C) = "SetCallback"
method(r3, sub_8219A2B8)       // register callback
```

This is the XAudio render driver's **SetCallback** or **RegisterNotification** vtable method. sub_8219A2B8 is invoked by the XAudio system (or by the worker threads) as a per-frame callback, NOT as a thread entry point.

## 2. The Actual Thread Entry Points

sub_82190B48 creates **up to 6 worker threads** in a loop (r31 iterates 0..5). The thread start routine depends on the thread type byte in the configuration array:

| Config Byte | Thread Start | Address | Name |
|-------------|-------------|---------|------|
| 1 | sub_821909D0 | 0x821909D0 | "Primary" audio worker |
| 2+ | sub_82190A98 | 0x82190A98 | "Secondary" audio worker |

### ExCreateThread Parameters (from sub_82190B48, line 1488-1496)

| Register | Value | Meaning |
|----------|-------|---------|
| r3 | &stack_local[84] | Thread handle output |
| r4 | **0** | Stack size = **default** (Xbox 360 default = 64KB) |
| r5 | 0 | Thread ID output (NULL) |
| r6 | 0 | Reserved |
| r7 | 0x821909D0 or 0x82190A98 | Start routine |
| r8 | 0 | Start context (NULL) |
| r9 | processor affinity flags | CPU core mask |

**Stack size = 0 means the kernel uses the default**, which on Xbox 360 is **64 KB** (0x10000).

After creation, each thread gets:
- `KeSetBasePriorityThread(thread, 15)` -- high priority for real-time audio
- `KeResumeThread(thread)` -- start execution (created suspended)
- Device's `[r29+304]` (thread count) is incremented

## 3. Thread Loop Structures

### sub_821909D0 — Primary Audio Worker (Thread Type 1)

```
sub_821909D0():
    // Store current thread ID in audio device's per-core slot
    slot_index = KPCR.ProcessorNumber + 83
    [audio_device + slot_index*4] = KPCR.ThreadId

    event_audReady  = 0x831D52CC
    semaphore       = 0x831D52DC
    event_complete  = 0x831D52F0
    event_shutdown  = 0x831D5310

LOOP:                                           // loc_82190A10
    KeWaitForSingleObject(
        event_complete,     // 0x831D52F0
        WaitReason=3,       // Executive
        WaitMode=1,         // KernelMode
        Alertable=FALSE,
        Timeout=NULL        // INFINITE wait
    )

    device = [0x831D53EC]
    threadCount = [device + 300]    // r30+300 field

    if (threadCount == 0):
        // shutdown bit = 1 via cntlzw+rlwinm:
        //   cntlzw(0) = 32, rlwinm(32, 27, 31, 31) = 1
        shutdown = 1
    else:
        shutdown = 0

    if (shutdown == 0):                         // loc_82190A64
        sub_8218FFB0(device)                    // DPC-level audio buffer drain
        sub_82191228(device, 1)                 // render one audio frame
    else:
        // [device + 304] > 0: pending threads exist
        if ([device + 304] - 1 > 0):
            KeReleaseSemaphore(semaphore, 1, pending-1, 0)

    KeSetEvent(event_audReady, 1, 0)            // signal "ready"

    if (shutdown == 0):
        goto LOOP                               // continue looping
    else:
        return 0                                // thread exits
```

**Loop termination**: The thread checks `[device + 300]` after each wake. If it is 0, `cntlzw` produces 32, and `rlwinm(32, 27, 31, 31)` extracts bit 0 = **1** (shutdown). The loop exits when `[device + 300]` is set to 0 by the shutdown path.

**Sleep mechanism**: `KeWaitForSingleObject` with `Timeout=NULL` = **infinite wait**. The thread sleeps until `event_complete` (0x831D52F0) is signaled.

### sub_82190A98 — Secondary Audio Worker (Thread Type 2+)

```
sub_82190A98():
    // Store current thread ID in audio device's per-core slot
    slot_index = KPCR.ProcessorNumber + 83
    [audio_device + slot_index*4] = KPCR.ThreadId

    semaphore = 0x831D52DC

    KeWaitForSingleObject(
        semaphore,          // 0x831D52DC
        WaitReason=3,       // Executive
        WaitMode=1,         // KernelMode
        Alertable=FALSE,
        Timeout=NULL        // INFINITE wait
    )

    device = [0x831D53EC]

    if ([device + 300] == 0):       // threadCount == 0 => exit
        return 0

INNER_LOOP:                                     // loc_82190AF8
    sub_82191228(device, 0)                     // render audio frame (flag=0)

    KeWaitForSingleObject(
        semaphore,
        WaitReason=3, WaitMode=1,
        Alertable=FALSE, Timeout=NULL
    )

    device = [0x831D53EC]
    if ([device + 300] != 0):
        goto INNER_LOOP                         // keep processing

    return 0                                    // shutdown
```

**Loop termination**: Same mechanism -- `[device + 300] == 0` causes exit.

**Sleep mechanism**: `KeWaitForSingleObject` on the semaphore with infinite timeout.

## 4. The Callback Path (sub_8219A2B8 / sub_821910D0)

sub_8219A2B8 is NOT called by the worker threads directly. Instead:

1. **Worker thread** (sub_821909D0) calls `sub_82191228(device, 1)` to render audio
2. sub_82191228 processes voice categories, submits packets, etc.
3. After rendering, sub_821910D0 is called (either by the primary worker thread directly at PATH B, or through the callback mechanism)

In sub_821910D0, the flow is:

```
sub_821910D0(device):
    Enter critical section (0x8286833C)

    if ([device + 304] != 0):       // PATH A: pending work
        KeSetEvent(0x831D52F0, ...)
        KeWaitForMultipleObjects(
            {0x831D52CC, 0x831D5310},   // semaphore OR shutdown
            WaitAny, ...)
        if (result == 1):
            shutdown = true
    else:                            // PATH B: no pending work
        sub_8218FFB0(device)         // drain DPC buffers
        sub_82191228(device, 1)      // render frame

    if (!shutdown):
        endpoint = [device + 64]
        vtable = [endpoint]
        result = vtable[17](endpoint)    // <-- THE 0x000F4000 call
        if (result >= 0):
            atomic_increment(0x831D53F0) // frame counter

    [device + 300] = 0
    // swap double-buffer (28 bytes)
    Leave critical section
    return 0
```

## 5. How Many Iterations Before the Stack Overflow?

The "42 MISSING-FUNC calls = 42 iterations" hypothesis from the crash log is **incorrect in its implication**. The 42 calls to 0x000F4000 are NOT causing stack overflow because:

1. sub_821910D0 **returns cleanly** after each vtable call (it has no recursion)
2. The worker thread (sub_821909D0) loops back to `KeWaitForSingleObject` after each iteration
3. Stack frame size: sub_821909D0 uses 128 bytes, sub_821910D0 uses 192 bytes, sub_8218FFB0 uses ~128 bytes, sub_82191228 uses ~144 bytes

**Maximum stack depth per iteration**: ~600 bytes (all nested calls). With 64KB default stack, that allows **~100 iterations** before stack overflow -- but only if calls are recursive. They are not; each iteration returns to the same stack frame.

**The 42 iterations do NOT cause stack overflow.** Each iteration reuses the same stack space. The issue is not stack exhaustion but the corrupted vtable at `[device+64]`.

## 6. sub_82190B48 — Worker Thread Spawner

**Address**: 0x82190B48
**File**: `gta4_recomp.2.cpp` line 1108

This is the **audio system initializer**. It:

1. Reads a configuration structure (r24 parameter) describing the audio thread topology
2. Allocates per-voice XAudio2 voice pool objects via vtable call `[allocator + 20]` (r4=size)
3. Initializes kernel synchronization objects:
   - `KeInitializeSemaphore(0x831D52DC)`
   - `KeInitializeEvent` for 0x831D52CC, 0x831D52F0, 0x831D5310
4. Registers `ExRegisterTitleTerminateNotification` for cleanup
5. Calls `sub_82190FB8(processor_mask, config_buffer)` to populate the thread type array
6. **Creates up to 6 threads** in a loop (r31 = 0..5):
   - Reads `config_buffer[r31]` for the thread type (0=skip, 1=primary, 2=secondary)
   - If type != 0: calls `ExCreateThread(NULL, 0, NULL, 0, start_routine, NULL, flags)`
   - After creation: `KeSetBasePriorityThread(thread, 15)`, `KeResumeThread(thread)`
   - Increments `[device + 304]` (active thread count)
7. After all threads are created, creates XAudio render endpoint via `sub_82199EC8`
8. Registers sub_8219A2B8 as the render callback on the endpoint

### Does sub_82190B48 Create Audio Threads?

**Yes.** sub_82190B48 is the sole creator of audio worker threads. It creates:
- One **primary** worker (sub_821909D0) on the first available core
- Up to 5 **secondary** workers (sub_82190A98) on remaining cores
- The exact count depends on the processor affinity mask in the config

## 7. Shutdown Mechanism

The audio system shuts down through the `[device + 300]` field:

1. Someone (likely the terminate notification handler) sets `[device + 300] = 0`
2. **Primary worker** (sub_821909D0): wakes from `KeWaitForSingleObject`, reads `[device + 300]`, `cntlzw(0) = 32` gives shutdown=1, releases semaphore for secondary threads, sets event, exits
3. **Secondary workers** (sub_82190A98): wake from semaphore wait, read `[device + 300] == 0`, exit

Additionally, sub_821910D0 PATH A waits on `{0x831D52CC, 0x831D5310}`. Object index 1 is `0x831D5310` (the "shutdown event"). If that fires (result == 1), r31 is set to 1 (shutdown flag), the vtable dispatch is skipped, and the function returns cleanly.

## 8. Synchronization Object Map

| Address | Object | Purpose |
|---------|--------|---------|
| 0x831D52CC | KEVENT (or semaphore) | "Audio ready" -- waited on by sub_821910D0 PATH A |
| 0x831D52DC | KSEMAPHORE | Thread work semaphore -- released by primary, waited by secondary workers |
| 0x831D52F0 | KEVENT | "Frame complete" -- waited by primary worker, set by sub_821910D0 PATH A |
| 0x831D5310 | KEVENT | "Shutdown" -- signaled to terminate, waited by sub_821910D0 PATH A |
| 0x8286833C | RTL_CRITICAL_SECTION | Guards sub_821910D0's per-frame rendering |
| 0x831D53EC | Global pointer | Audio device object |
| 0x831D53F0 | Atomic u32 | Frame counter / "new" audio state buffer base |
| 0x831D540C | u32[7] | "Current" audio state buffer |
| `[device+300]` | u32 | Current processing thread ID (0 = shutdown) |
| `[device+304]` | u32 | Active thread count |

## 9. Summary

- sub_8219A2B8 is a **thin callback wrapper**, not a thread entry or loop
- The actual audio thread loop lives in **sub_821909D0** (primary) and **sub_82190A98** (secondary)
- Both threads sleep via **KeWaitForSingleObject** with **infinite timeout** between iterations
- Both terminate when `[device + 300]` is set to 0
- Stack size is **0 (default = 64KB on Xbox 360)** -- more than sufficient for the ~600 bytes used per iteration
- The 42 repeated MISSING-FUNC calls are caused by the **corrupted vtable** at `[device+64]`, not by stack overflow or an uncontrolled loop
- sub_82190B48 creates all audio threads (up to 6) with `KeSetBasePriorityThread(15)` (high priority)
