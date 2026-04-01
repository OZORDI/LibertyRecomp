# sub_8223F308 - Scene File Loading & Creation

## Overview

`sub_8223F308` is a lightweight dispatcher that loads scene file data from a
binary stream. It does NOT allocate the scene object itself -- that happens in
its wrapper `sub_8223F390`, which calls `sub_8223F308` first, then allocates
the scene object via `sub_821B3510`.

The function is called from **state 4** of `sub_82242910` when `platformMode`
is 3 or 4.

---

## Function Signatures

### sub_8223F308 (the dispatcher)

```
void sub_8223F308(r3: uint8_t flag, r4: SceneStruct* sceneStruct)
```

- **r3** (low byte): If nonzero, calls `sub_8223E980` (load scene WITH save data
  validation). If zero, calls `sub_8223ECA8(sceneStruct, 0)` (load scene WITHOUT
  save data).
- **r4**: Pointer to the SceneStruct at `0x82BF3A78`.
- **Returns**: void (results stored in SceneStruct fields).

### sub_8223F390 (the wrapper/allocator)

```
bool sub_8223F390(r3: uint8_t flag, r4: SceneStruct* sceneStruct)
```

- Same parameters as sub_8223F308.
- After calling sub_8223F308, checks if `sceneStruct[0]` is non-null. If so,
  returns 0 (already created, skip allocation).
- If null, sets up TLS allocator context at `TLS[r13+0]+1676`, calls
  `sub_821B3510` to allocate the scene object, stores result in `sceneStruct[0]`,
  then restores TLS allocator context.
- **Returns**: 1 if scene object was created successfully, 0 if allocation failed
  (cntlzw/rlwinm logic: returns `!(result == 0)`).

---

## Call Chain

```
sub_82242910 state 4
  --> sub_8223F308(r3=1, r4=0x82BF3A78)        [load scene file]
      --> sub_822BCA90(...)                      [no-op trace/log]
      --> sub_8223E980(sceneStruct)              [load WITH save validation]
          --> sub_8223C8B8(buf, 4)               [read 4 bytes from stream, x7+]
          --> sub_822F94F0()                      [case 1: streaming init?]
          --> sub_8221AC70(devPtr)                [case 2: device/renderer setup]
          --> sub_8223E0E8(buf)                   [case 0: init scene descriptor]
          --> sub_8223E300(buf)                   [finalize scene descriptor]
          --> rexcrt_strncmp(name, "common:/", 10) [check arg names]
          --> sub_824FFFF8(printfLike, argPtr)    [log non-common args]
      OR
      --> sub_8223ECA8(sceneStruct, 0)           [load WITHOUT save validation]
          --> sub_8223DB20()                      [readiness check]
          --> sub_826CD808()                      [get object pointer]
          --> sub_829DB688(0x82BF9898)            [init buffer]
          --> sub_829DBAA8()                      [setup]
          --> sub_829DB648(objPtr, buf)           [load operation]
          --> sub_8223C9A0(buf, size)             [read from stream, x many]
          --> sub_82300858(r16)                   [case 1: verify content]
          --> sub_8221AD28(devPtr)                [case 2: renderer setup]
          --> sub_8223E0E8(buf)                   [case 0: scene descriptor]
          --> rexcrt_strncmp(name, "common:/", 10)
      --> sub_822BCA90(...)                      [no-op trace/log]
  --> reads sceneStruct[4] (content ID), stores to 0x82BF99CC
  --> sub_82240B08()                             [check content notification]
```

---

## Memory Addresses (Python-verified)

### SceneStruct at 0x82BF3A78

| Offset | Address      | Type   | Description                       |
|--------|-------------|--------|-----------------------------------|
| +0     | 0x82BF3A78  | u32    | Scene object pointer (from sub_821B3510) |
| +4     | 0x82BF3A7C  | u32    | Content/file ID                   |
| +8     | 0x82BF3A80  | u8     | Flag byte (set to 0 after load)   |
| +12    | 0x82BF3A84  | u32    | Saved content ID (copy of +4)     |
| +16    | 0x82BF3A88  | u32    | Scene data slot 0                 |
| +20    | 0x82BF3A8C  | u32    | Scene data slot 1                 |
| ...    | ...         | u32    | ...                               |
| +140   | 0x82BF3B04  | u32    | Scene data slot 31                |

**IMPORTANT**: `imports.cpp` reads `0x82BF3A88` and labels it "scene object
pointer". This is **incorrect**. `0x82BF3A88` is scene data slot 0 (the first
32-bit value read from the scene file). The actual scene object pointer
allocated by `sub_821B3510` is stored at `0x82BF3A78` (sceneStruct+0).

### Global Variables

| Address      | Size | Name (tentative)        | Description                                    |
|-------------|------|------------------------|------------------------------------------------|
| 0x82BF981C  | u8   | g_sceneErrorFlag       | Set to 1 on scene load failure                 |
| 0x82BF981D  | u8   | g_sceneCreating        | 1 while scene creation in progress, 0 after    |
| 0x82BF981E  | u8   | g_sceneFlag2           | Another scene state flag                       |
| 0x82BF9818  | u32  | g_saveDataLength       | Length of save data blob                        |
| 0x82BF9820  | u32  | g_sceneIndex           | Current scene data slot index                  |
| 0x82BF9824  | u32  | g_sceneStructPtrCopy   | Copy of sceneStruct pointer (for nested calls) |
| 0x82BF9858  | u32  | g_sceneIndexCompare    | Scene index for version comparison             |
| 0x82BF9898  | ---  | g_sceneTempBuffer      | Temp buffer for sub_829DB688                   |
| 0x82BF99CC  | u32  | g_sceneContentId       | Content ID stored by state 4 after load        |

### Content Notification Globals (used by sub_82240B08)

| Address      | Size | Name (tentative)        | Description                                    |
|-------------|------|------------------------|------------------------------------------------|
| 0x82BF3A76  | u8   | g_notifyFlag           | Set on content notification received           |
| 0x82BF3A77  | u8   | g_sceneReady           | Set to 1 when content ready for scene          |
| 0x82BF3CDA  | u8   | g_contentReady         | Set to 1 when content file is available         |
| 0x82BF3D90  | u32  | g_contentType          | Content type index for sub_8284B3D8            |

### Notification Array (sub_8284B3D8)

- Base: `0x83192C58`
- Stride: 160 bytes per entry (`index * 5 * 32`)
- File handle at entry + 56
- Indexed by `g_contentType` at `0x82BF3D90`

### Other Referenced Addresses

| Address      | Description                          |
|-------------|--------------------------------------|
| 0x82005DE4  | Log string (pre-load trace)          |
| 0x82005DB4  | Log string (post-load trace)         |
| 0x82005974  | Log format string                    |
| 0x82005968  | String "common:/" (10-byte compare)  |
| 0x82AA694C  | Heap allocator context (set in TLS)  |
| 0x82A953E0  | Argv string pointer array (32 ptrs)  |
| 0x82A95460  | Scene header comparison string       |
| 0x82A95468  | Error code / status variable         |
| 0x82B39504  | Version number for scene comparison  |
| 0x82D5539C  | Config/vtable pointer                |
| 0x831927B8  | Config value                         |
| 0x831E4DD4  | Renderer/device pointer              |
| 0x83017630  | Printf-like function pointer         |
| 0x83192C50  | Scene object array base              |

---

## sub_8223F308 Control Flow

```
sub_8223F308(r3=flag, r4=sceneStruct):
    sub_822BCA90(0x82005DE4)           // trace: "entering scene load"

    r10 = flag & 0xFF
    g_sceneCreating (0x82BF981D) = 1   // mark scene creation active

    if (r10 == 0):
        sub_8223ECA8(sceneStruct, 0)   // load WITHOUT save validation
    else:
        sub_8223E980(sceneStruct)      // load WITH save validation

    g_sceneCreating (0x82BF981D) = 0   // mark scene creation done
    sceneStruct[8] = 0                 // clear flag byte (0x82BF3A80)

    sub_822BCA90(0x82005DB4)           // trace: "exiting scene load"
    return
```

---

## sub_8223E980 - Scene Load WITH Save Validation

This is the more complex path (called when flag=1, i.e., resume from save).
It reads a structured binary stream containing the scene file header, content
identifiers, and 32 data slots representing scene arguments.

### High-Level Flow

1. Initialize: clear error flag, save data length, scene index, store sceneStruct ptr
2. Set `sceneStruct[4] = 0` (clear content ID)
3. Read scene file header using `sub_8223C8B8` (multiple 4-byte reads)
4. Verify "SAVE" magic bytes (0x53, 0x41, 0x56, 0x45)
5. Loop over 32 argument slots (r28 = 0..31):
   a. Read 5-byte argument header
   b. Read 4-byte data value into sceneStruct[offset = (r28+4)*4]
   c. Per-argument processing via switch on r28:
      - Case 0: Init scene descriptor (`sub_8223E0E8`) + validate with `sub_8223C8B8`
      - Case 1: Streaming init (`sub_822F94F0`)
      - Case 2: Device/renderer setup (`sub_8221AC70`)
      - Cases 3-19: Compare arg name vs "common:/" (strncmp, 10 bytes). If mismatch,
        log via `sub_824FFFF8`.
      - Case 20+: Same as 3-19
   d. Check error flag after each slot; abort if set
   e. If sceneStruct flag byte (+8) is set, write scene index to data slot
   f. If not set, verify data slot value matches expected (abort on mismatch)
6. After all 32 slots: compute checksum over save data if `g_sceneCreating` is 0
7. Write checksum + final validation block via `sub_8223C8B8`
8. If sceneStruct flag (+8) is set: copy content ID (+4) to saved ID (+12)
9. If not set: verify saved ID (+12) matches newly-read value
10. Return 1 on success, 0 on any failure

### Failure Cases

- Any `sub_8223C8B8` read returns false: return 0
- Error flag (`0x82BF981C`) gets set during argument processing: return 0
- Data slot mismatch (expected vs actual): log and return 0
- Checksum mismatch: log and return 0
- Saved content ID mismatch: log and return 0

---

## sub_8223ECA8 - Scene Load WITHOUT Save Validation

Called when flag=0 (new game, no existing save). Similar structure but uses
`sub_8223C9A0` (flexible stream reader) instead of `sub_8223C8B8`.

### Additional Steps (vs sub_8223E980)

1. If `g_sceneCreating` is 0: calls `sub_8223DB20()` for readiness check
2. If readiness check fails: logs error and returns 0
3. If r4 (second param, always 0 from sub_8223F308) is 0:
   a. Calls `sub_826CD808()` to get an object pointer
   b. If null: init buffer at `0x82BF9898` via `sub_829DB688`, log error, return 0
   c. If non-null: calls `sub_829DBAA8()` then `sub_829DB648()` for setup
   d. On setup failure: log and return 0
4. Then follows similar argument-parsing loop as sub_8223E980 (32 slots)
5. Per-slot switch identical to sub_8223E980

---

## sub_8223F390 - Wrapper / Scene Object Allocator

Called by state 4 indirectly. Wraps sub_8223F308 with object allocation logic.

```
bool sub_8223F390(r3=flag, r4=sceneStruct):
    if (flag == 0):
        // Check version mismatch
        oldVersion = PPC_LOAD_U32(0x82B39504)
        curVersion = PPC_LOAD_U32(0x82BF9858)
        if (oldVersion != curVersion):
            // Set restart flag
            PPC_STORE_U8(0x82BF3A80, 1)  // sceneStruct[8] = 1

    sub_8223F308(flag, sceneStruct)

    if (sceneStruct[0] != 0):
        return 0   // scene object already exists, no alloc needed

    // Set up TLS heap allocator
    tls = PPC_LOAD_U32(r13 + 0)
    PPC_STORE_U32(tls + 1676, 0x82AA694C)  // set allocator context

    sub_822BCA90(0x82005E14, sceneStruct[4])  // trace log

    sceneObj = sub_821B3510(sceneStruct[4])    // allocate scene object
    sceneStruct[0] = sceneObj                  // store at 0x82BF3A78

    // Restore original TLS allocator
    PPC_STORE_U32(tls + 1676, PPC_LOAD_U32(tls + 1680))

    return (sceneObj != 0) ? 1 : 0
```

### sub_821B3510 - Scene Object Allocator

```
ptr sub_821B3510(r3=contentId):
    tls = PPC_LOAD_U32(r13 + 0)
    allocator = PPC_LOAD_U32(tls + 1676)
    vtable = PPC_LOAD_U32(allocator + 0)
    allocFn = PPC_LOAD_U32(vtable + 8)
    // tail-call: allocFn(allocator, contentId, size=16, flags=0)
```

This is a virtual method call through the heap allocator's vtable (method at
index 2, offset +8). The allocation request is for 16 bytes with the content
ID as a key.

---

## sub_82240B08 - Content Notification Check

Called from state 4 after scene creation. Checks if a content file is ready.

```
bool sub_82240B08():
    contentType = PPC_LOAD_U32(0x82BF3D90)

    ready = sub_8284B3D8(0x83192C50, contentType)
    // sub_8284B3D8: checks notification array at 0x83192C58
    //   entry = base + contentType*160
    //   handle = entry[56]
    //   if handle != 0: GetOverlappedResult(handle, &result) != 0
    //   returns 1 if I/O completed, 0 otherwise

    if (ready):
        g_sceneReady (0x82BF3A77) = 1
        g_contentReady (0x82BF3CDA) = 1
        return 1
    else:
        sub_8223DB90(1)   // reset/cleanup
        return 0
```

### sub_8284B3D8 - Async I/O Completion Check

Checks if an asynchronous content file load has completed:
- Indexes into notification array at `0x83192C58` using `contentType * 160`
- Reads file handle from entry offset +56
- If handle is null: returns 0 (not ready)
- If handle exists: calls `sub_82A12718` (GetOverlappedResult wrapper)
- Returns 1 if I/O completed, 0 if still pending

### sub_8223DB90 - Content Reset

Called when content is NOT ready:
- Calls `sub_8284B3B0(0x83192C50, contentType)` to reset the notification entry
- Checks `g_contentReady` (0x82BF3CDA): if set, stores flag at `0x82BF3A76`
- Clears `g_sceneReady` (0x82BF3A77) = 0
- Sets error indicator at `0x82A954E7` = 1

---

## State 4 Integration (sub_82242910)

From the state 4 code at line 85161:

```
state_4:
    // r10 is set to 1 (or determined by prior logic)
    if (r10 & 0xFF != 0):
        sceneStruct = 0x82BF3A78       // addi r30,r11,14968
        sub_8223F308(1, sceneStruct)    // load scene with save validation
        contentId = sceneStruct[4]      // lwz r11,4(r30)
        PPC_STORE_U32(0x82BF99CC, contentId)  // save content ID

    if (g_sceneFlag2 at 0x82BF981E != 0):
        goto next_state                 // skip content check

    result = sub_82240B08()             // check content notification
    if (result):
        goto state_5_setup

    // Content not ready yet - enter sub-state machine (0-4)
    subState = PPC_LOAD_U32(0x82BF9844 or similar)
    switch(subState):
        case 3: check another flag...
        case 4: ...
        ...
```

---

## Can sub_8223F308 Fail?

**sub_8223F308 itself cannot fail** -- it has no return value and always runs to
completion. However, it can leave the SceneStruct in a failed state:

- `sceneStruct[4]` (content ID) remains 0 if the inner load fails
- `sceneStruct[0]` (object pointer) stays NULL if not previously allocated
- `g_sceneErrorFlag` (0x82BF981C) is set to 1 on failure
- `g_sceneCreating` (0x82BF981D) is always cleared to 0 regardless

The **wrapper** `sub_8223F390` CAN fail and returns 0 when:
1. `sub_8223F308` loaded scene data but `sceneStruct[0]` was already non-null
   (returns 0 meaning "already created, not an error")
2. `sub_821B3510` allocation returns NULL (returns 0 meaning real failure)

**sub_82240B08** can fail (returns 0) when:
1. Content file I/O is still pending (not an error, just needs retry)
2. On I/O failure, calls `sub_8223DB90` to reset and returns 0

---

## Summary

The scene creation pipeline works as follows:

1. **sub_8223F308** reads the scene file binary data from a stream, parsing a
   header, "SAVE" magic, and 32 argument slots into the SceneStruct at
   `0x82BF3A78`.

2. **sub_8223F390** (if used as wrapper) then allocates the actual scene object
   via `sub_821B3510` using a TLS-based heap allocator, storing the result at
   `sceneStruct[0]` = `0x82BF3A78`.

3. **sub_82240B08** verifies that the content file associated with the scene has
   finished loading asynchronously.

4. The scene object pointer lives at **0x82BF3A78**, NOT at 0x82BF3A88 as
   currently referenced in `imports.cpp`. The value at 0x82BF3A88 is scene data
   slot 0 (the first 32-bit value parsed from the scene file).
