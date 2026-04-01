# Yield Loop at 0x825FBCAC — Save/Content Device Monitor Thread

## Function

**sub_825FBB68** (0x825FBB68 – 0x825FBCAC) in `gta4_recomp.35.cpp:26152`

This is a **save/content device monitor thread** — an infinite loop that polls for Xbox 360 storage device change notifications and processes pending content I/O. It is NOT part of the streaming worker pool (sub_8285D610).

## Global State

|Address|Register|Purpose|
|-|-|
|0x83092BC0|r28-13460|XNotifyGetNext listener handle|
|0x83092BBC|r28-13464|Event handle — "new work available" signal|
|0x83096090|r25|Critical section protecting content device state|
|0x830960B0|r27|Callback vtable object (vtable+12 = completion callback)|
|0x8308B838|r26|Content device context (offset 29568 = done-signal event)|
|0x82AACFE4|r29|Active content device file handle (-1 = invalid)|

## Loop Flow (loc_825FBBA0)

1. **XNotifyGetNext(handle, area=11, &id, &param)** — poll for `XN_SYS_STORAGEDEVICESCHANGED` (area 11)
   - If notification received: set `r30 = 1` (device change flag)
2. **Check work event** — sub_82849790(*0x83092BBC): non-blocking WaitForSingleObject(handle, 0)
   - Returns 1 if signaled, 0 if not
   - If NOT signaled AND no notification: skip to step 6
3. **Enter critical section** — sub_8285FF50(&stack_lock, 0x83096090) calls RtlEnterCriticalSection
4. **Process content I/O:**
   - Read handle at *(0x82AACFE4); if -1 (INVALID_HANDLE_VALUE), skip
   - Call sub_82A12710(handle, 0, 2, 0, 96, &out_handle, &out_size) — NtDeviceIoControlFile / storage query
   - On success (0): call sub_82A11F50 (write/transfer result), close handle
   - On error 1317: skip (device not ready)
   - Otherwise: invoke completion callback via vtable at [r27+12]
   - If device change notification: signal event via sub_82849860(*(r26+29568))
5. **Leave critical section** — sub_8285FFA0(&stack_lock) calls RtlLeaveCriticalSection
6. **Sleep(16)** — sub_82849918(16) tails to sub_82A12B60 -> NtDelayExecution(16ms)
7. **goto step 1**

## What It Waits For

The loop exits the "fast path" (skip to sleep) when EITHER:
- **XNotifyGetNext returns a storage-device-changed notification** (area 11), OR
- **The work event at \*0x83092BBC is signaled** (another thread posts content I/O work)

Without either condition, the thread simply sleeps 16ms and polls again.

## Lock Analysis — Does It Block the Main Thread?

**No.** The critical section at 0x83096090 is:
- Acquired ONLY during the work phase (step 3–5)
- Released BEFORE the Sleep(16) call (step 6)
- Scoped via sub_8285FF50 (enter) / sub_8285FFA0 (leave) — a ref-counted scoped lock pattern

The main thread's allocator stuck in sub_821B3510 (operator new, 1024 bytes) uses the heap allocator (sub_8218BE28 -> TLS heap or fallback). This thread does NOT hold any heap lock or allocator lock. The critical section 0x83096090 is specific to the content device subsystem.

**This thread is not the cause of the main thread's allocation stall.**

## Relationship to Streaming Worker Pool

sub_825FBB68 is **separate** from the streaming worker pool (sub_8285D610). Key differences:
- sub_8285D610 creates worker threads with per-CPU affinity and processes streaming requests from a queue
- sub_825FBB68 is a single monitor thread that polls XNotify for storage device changes
- They share infrastructure (sub_8285FF50/FFA0 scoped locks, sub_8285FE48 init) but operate on different data

## Neighboring Functions (Context)

- **sub_825FBB00**: Initializes a 96-entry free-list pool (stride 140 bytes) — content request objects for sub_825FBB68
- **sub_825FBCB0**: Opens/queries a content device file (called after device change detection)
- **sub_825FB998**: Computes a status byte from the content device state
