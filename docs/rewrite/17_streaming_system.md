# Streaming System Analysis

**Source files**:
- `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.39.cpp` (sub_826CBA70, sub_826CD808, sub_826CD8B0)
- `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.55.cpp` (sub_8284AB10..sub_8284B490)
- `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.67.cpp` (sub_829EEA58)
- `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.9.cpp` (sub_822BF360)
- `LibertyRecomp/kernel/save_hooks.cpp` (streaming sync hooks)
- `LibertyRecomp/kernel/imports.cpp` (pgStreamer hooks, semaphore seeding)

---

## Key Memory Addresses

| Address | Type | Description |
|---------|------|-------------|
| `0x8317F778` | struct | strStreaming singleton (global streaming manager) |
| `0x8317F838` | struct | strStreaming+0xC0 — sub-object (file/resource handle) |
| `0x8317F870` | u8 | strStreaming+0xF8 — readiness byte (bit 7 = loaded flag) |
| `0x8317F880` | ptr | strStreaming+0x108 — sub-object pointer returned by sub_826CBA80 |
| `0x8317F62C` | u32 | Localisation pack count |
| `0x83180990` | array | Localisation pack array (stride 196, name at offset +8) |
| `0x8317F654` | u8 | Scene dirty flag (cleared after scene load dispatch) |
| `0x8317F634` | array | Scene pointer array (indexed by scene ID, dword entries) |
| `0x8317F4E4` | u32 | Scene count (max valid scene index) |
| `0x8318AAF8` | struct | strStreaming object (passed to sub_822BF360 busy check) |
| `0x82A9172C` | u32 | Current loaded scene index |
| `0x83192C58` | array | Scene entry table base (used by 0x8284xxxx functions, stride variable) |
| `0x830F5820` | u32 | Streaming-pending flag (set by sub_82192E00, polled by sub_827DE648) |
| `0x830F589C` | u32 | pgStreamer sync mode flag (1 = synchronous, 0 = async workers) |

---

## sub_826CBA70 -- Streaming Resource Readiness Check

**Location**: gta4_recomp.39.cpp:66613

**What it does**: Thin wrapper that loads a pointer to strStreaming+0xC0 (`0x8317F838`) and tail-calls sub_829EEA58.

**sub_829EEA58** (gta4_recomp.67.cpp:32579):
1. Loads byte at `[r3 + 56]` (i.e., `0x8317F838 + 56 = 0x8317F870`)
2. Applies `rlwinm r3,r11,25,7,31` which extracts bit 7: `(byte >> 7) & 1`
3. Returns the result

**Return value**:
- `1` = resource is loaded (bit 7 / 0x80 of byte at `0x8317F870` is set)
- `0` = still loading (bit 7 clear)

**Xbox dependencies**: None directly. The readiness byte is set by the streaming completion path, which depends on file I/O completing. In the recomp, file I/O is synchronous via rexcrt, so this byte should eventually be set naturally.

**Recomp status**: Works as recompiled code. No hook needed. The byte at `0x8317F870` is written by the streaming system's load-complete callback path.

**Usage in state machine**: Called by states 1, 2, 3, 4+, and several states in the outer SM (sub_82242910). The pattern is always:
```
call sub_826CBA70()
mask to u8 (clrlwi r11,r3,24)
if nonzero: streaming active/loaded, proceed or wait depending on state
if zero: not yet loaded, try alternative path
```

States 1 and 2 treat nonzero as "network session active, tear down before proceeding."
State 3 treats nonzero as "wait for streaming to settle."
Later states treat it as a readiness gate.

---

## sub_826CBA80 -- Get Localisation Sub-Object Pointer

**Location**: gta4_recomp.39.cpp:66627

Returns pointer `0x8317F880` (strStreaming+0x108). Simple accessor, no side effects.

---

## sub_826CBA90 -- Get Localisation Pack Count

**Location**: gta4_recomp.39.cpp:66638

Returns the u32 at `0x8317F62C` (number of registered localisation packs).

---

## sub_826CBAA0 -- Find Localisation Pack by Name

**Location**: gta4_recomp.39.cpp:66650

**What it does**: Searches the localisation pack array at `0x83180990` (stride 196 bytes) for a pack matching the name string in r3 (arg 1). Each entry's name is at offset +8 (sub_82770AC8 returns `r3 + 8`). Uses inline byte-by-byte strcmp loop.

**Return value**: Pack index (0-based) if found, -1 if not found.

**Recomp status**: Works as recompiled code. Pure memory reads and string comparison.

---

## sub_826CD808 -- Scene Update / Streaming Dispatch

**Location**: gta4_recomp.39.cpp:71134

**What it does**:
1. Loads strStreaming object pointer into r30 = `0x8318AAF8`
2. Loads current scene index from `0x82A9172C` into r31
3. Calls `sub_829DB9B8(r30)` to get the streaming system's active scene ID
4. Compares active scene with stored scene index:
   - If **equal**: skip to step 7 (no update needed)
   - If scene index **< 0**: call `sub_826CD5A8()` (fallback/reset path)
   - Otherwise: validate scene via `sub_829DB7F8(r31)`, then update scene via `sub_829DBBA8`/`sub_829DBC30`/`sub_826CD660`
5. Step 7: Call `sub_822BF360(r30)` — busy check on strStreaming object
6. If busy (returns 0): return `r3 = 0` (not ready)
7. If not busy (returns nonzero): return `r3 = r30` (strStreaming object pointer, truthy)

**Return value**:
- `0` = streaming system busy, scene not ready
- nonzero (strStreaming ptr `0x8318AAF8`) = scene ready

**Xbox dependencies**: sub_829DB9B8, sub_829DB7F8, sub_829DBC30 are streaming system internals that interact with the file system. They should work through rexcrt file I/O.

**Recomp status**: Works as recompiled code. Called extensively by:
- `sub_8223DAA0` (readiness check, doc 03) -- calls it twice: once to check if ready, once to get the active scene ID
- `sub_8223ECD0` (scene transition helper)
- Outer SM `sub_822438B0` -- scene teardown/transition

---

## sub_822BF360 -- Streaming Busy Check

**Location**: gta4_recomp.9.cpp:66733

**What it does**: Loads byte at `[r3 + 92]` and extracts bit 7 via `rlwinm r3,r11,25,7,31`.

For the strStreaming object at `0x8318AAF8`:
- Checks byte at `0x8318AAF8 + 92 = 0x8318AB54`
- Returns `(byte >> 7) & 1`

**Return value**:
- `1` = not busy (bit 7 set = idle/complete)
- `0` = busy (bit 7 clear = streaming in progress)

Same bit-extraction pattern as sub_829EEA58.

**Recomp status**: Works as recompiled code. Pure memory read.

---

## sub_826CD8B0 -- Scene Load by Index

**Location**: gta4_recomp.39.cpp:71237

**What it does**:
1. Takes scene index in r3 (arg 1)
2. Validates index < scene count (from `0x8317F4E4`)
3. Looks up scene pointer from array at `0x8317F634[index*4]`
4. If pointer is null: return 0
5. Calls `sub_826CD808()` to get strStreaming object
6. If streaming not ready: skip to step 8
7. Calls `sub_826CB110(strStreaming, scene_ptr, pos_x, pos_y, pos_z, name_str)` to dispatch scene load
8. Calls `sub_826CD3F8(index)` — cleanup/mark scene as dispatched
9. Clears scene dirty flag at `0x8317F654`
10. Returns 1 if load was dispatched, 0 otherwise

**Xbox dependencies**: None directly. Uses the same streaming infrastructure.

**Recomp status**: Works as recompiled code.

---

## Scene Entry Functions (0x8284xxxx)

All these functions operate on a scene entry table at base `0x83192C58`. Each entry is accessed by computing an index-to-offset transformation:
```
offset = ((index + index*4) << 5)  // i.e., index * 160
entry_ptr = table_base + offset
```
(Python: `offset = (idx * 5) * 32 = idx * 160`)

### sub_8284AB10 -- Get Scene Entry (Init/Setup)

**Location**: gta4_recomp.55.cpp:11245

Computes scene entry address and tail-calls `sub_82849C18` (entry initializer). Takes `r3` = this, `r4` = scene index.

### sub_8284AB30 -- Get Scene Entry Status Value

**Location**: gta4_recomp.55.cpp:11266

Reads dword at entry offset +56 (field after base entry struct). Returns the status value.

### sub_8284AB50 -- Set Scene Entry Status Value

**Location**: gta4_recomp.55.cpp:11288

Writes r5 to entry offset +56. Setter counterpart of sub_8284AB30.

### sub_8284AB70 -- Clear Scene Entry if Status == 3

**Location**: gta4_recomp.55.cpp:11310

Reads entry status (offset +0 from entry). If status == 3 (loaded/complete), resets it to 0. Otherwise does nothing.

**Return value**: void (no explicit return)

### sub_8284ABA0 -- Load Scene Entry

**Location**: gta4_recomp.55.cpp:11338

Shuffles args (r4=index -> compute entry, r5->r4, r6->r5, r7->r6) and tail-calls `sub_8284A1E8` (the actual scene load dispatcher).

### sub_8284ABF8 -- Clear Scene Entry if Status == 5

**Location**: gta4_recomp.55.cpp:11392

Same pattern as sub_8284AB70 but checks for status == 5 (device not ready / pending) instead of 3.

### sub_8284B430 -- Get Scene Load Phase

**Location**: gta4_recomp.55.cpp:12628

Reads dword at entry offset +32 and maps it to a phase code:
- Value == 1223 -> returns **2** (in progress)
- Value == 0x80070012 -> returns **5** (device not ready, HRESULT E_NOT_READY)
- Value == 0x80070570 -> returns **3** (data error, ERROR_FILE_CORRUPT)
- Any other value -> returns **0** (idle/unknown)

**Xbox dependencies**: The status values 0x80070012 and 0x80070570 are Win32 HRESULT codes. In the recomp, file I/O through rexcrt should not produce these error codes during normal operation, so the normal path returns 0 (idle) or 2 (in progress).

### sub_8284B490 -- Get Scene Entry Raw Status

**Location**: gta4_recomp.55.cpp:12684

Reads the dword at entry offset +0 (the raw status field used by sub_8284AB70/ABF8). Returns it directly.

### sub_8284B4B0 -- Reset Scene Entry Status

**Location**: gta4_recomp.55.cpp:12704

Writes 0 to entry offset +0 (resets status to idle).

### sub_8284B4D0 -- Scene Entry Load Setup (Complex)

**Location**: gta4_recomp.55.cpp:12726

Complex function that handles scene entry allocation and resource loading setup. Takes `r3` = this, `r4` = scene index, `r5` = resource ID. Manages half-word counters at entry offsets +4 and +6, calls `sub_82849FF0` for initial setup, `sub_8285A610`/`sub_82849EF8` for resource allocation, and `sub_8285A720` for resource resolution.

---

## Existing Streaming Hooks

### sub_82192E00 -- Streaming Init (save_hooks.cpp:331)

**What it does on Xbox**: Marks a streaming job as "in-flight" by writing nonzero to `0x830F5820`. A hardware streaming completion thread would later clear this flag.

**Hook behavior**: Runs the original function, then immediately clears `0x830F5820` to 0. This prevents the downstream barrier (sub_827DE648) from spinning indefinitely.

**Why needed**: RexGlue VFS is fully synchronous -- no background thread exists to clear the completion flag.

### sub_827DE648 -- Streaming Completion Barrier (save_hooks.cpp:355)

**What it does on Xbox**: Spins in a loop while `0x830F5820 != 0`, waiting for the streaming completion thread to signal done.

**Hook behavior**: Returns immediately without checking the flag. Belt-and-suspenders fallback since sub_82192E00 already clears the flag.

**Why needed**: Without this hook, the main thread would spin forever waiting for a thread that does not exist.

### sub_827DF248 -- pgStreamer::Init (imports.cpp:714)

**What it does on Xbox**: Creates pgStreamer table and 2 worker threads for asynchronous streaming I/O.

**Hook behavior**: Sets `0x830F589C = 1` (sync mode flag) before calling the original. In sync mode, worker thread creation is skipped; all streaming work is processed inline on the calling thread via sub_827DE1C0.

**Why needed**: Worker threads die immediately due to a race condition -- the worker starts before its queue is initialized, finds the shutdown sentinel (v4[387]==0) in zeroed BSS, and exits. With dead workers, atomic refcount on pgStreamer entries is never decremented, causing pgStreamer::Close to busy-wait forever.

### sub_8284CFD8 -- Streaming Ring-Buffer Worker Pool Init (imports.cpp:656)

**What it does on Xbox**: Initializes 2 streaming workers with ring-buffer structs at `0x8319F2F8 + i*24944`. Each has a semaphore handle at offset +24940.

**Hook behavior**: Runs the original init, then post-seeds valid XSemaphore handles into each worker's semaphore slot. Without this, the semaphore slots contain BSS zero, causing NtWaitForSingleObjectEx to return INVALID_HANDLE and workers to spin.

**Why needed**: Even in sync mode, the ring-buffer infrastructure is initialized and needs valid semaphore handles to avoid INVALID_HANDLE errors.

### sub_829A2540 -- NtSetEvent Wrapper (imports.cpp:738)

**What it does on Xbox**: Sets an event handle to signal streaming I/O completion.

**Hook behavior**: Guards against invalid handles (0 or 0xCDCDCDCD). The completion event handle at work item offset 156 is never initialized by the producer (which only writes 156 of 160 bytes), retaining debug fill.

**Why needed**: Without the guard, NtSetEvent floods the log with STATUS_INVALID_HANDLE errors on every streaming work item completion.

### Per-Frame Event Signaling (imports.cpp:212)

The frame loop signals streaming events every frame:
```
SignalEventByGuestAddr(0x83131E10);   // Streaming I/O
```
This simulates the Xbox hardware interrupt that would normally wake streaming workers.

---

## Streaming System Architecture Summary

The GTA IV streaming system has three layers:

1. **pgStreamer** (low-level, `0x827Dxxxx`): Manages async file I/O with worker threads. In recomp, forced to synchronous mode via `0x830F589C = 1`.

2. **strStreaming** (mid-level, `0x826Cxxxx`): Manages scene and localisation resource loading. Uses pgStreamer for file reads. The readiness flag at `0x8317F870` (bit 7) and busy flag at `0x8318AB54` (bit 7) track completion.

3. **Scene Manager** (high-level, `0x8284xxxx`): Scene entry table at `0x83192C58` with 160-byte entries. Tracks load phases via status codes (0=idle, 2=in progress, 3=loaded/error, 5=pending).

The state machine in sub_82242910 polls sub_826CBA70 repeatedly to check if strStreaming resources are loaded before proceeding with scene creation. The flow is:

```
State 1/2: Call sub_826CBA70() -- if nonzero (loaded), tear down network and advance
State 3:   Call sub_826CBA70() -- if nonzero, wait (streaming still active)
State 4+:  Call sub_826CBA70() -- gate for scene load operations
```

**Recomp viability**: All streaming functions work as recompiled code. The critical hooks are in pgStreamer (forcing sync mode) and the streaming init/barrier pair (clearing completion flags). The higher-level strStreaming and scene manager code operates on guest memory and does not require Xbox-specific hardware.
