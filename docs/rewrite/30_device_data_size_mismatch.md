# XamContentGetDeviceData Size Mismatch Analysis

## Summary

The save state machine (state 12/14) fails because `sub_8284ADA0` compares
`slot[136]` (expected STFS container size) against `slot[144]` (actual STFS
container size). In the recomp, `slot[144]` is never populated because
`XamContentCreateEx` returns synchronously (state 16) instead of returning
`ERROR_IO_PENDING` (state 17). Only the state-17 path performs the file
open + `GetFileSizeEx` + STFS recalculation that populates `slot[144]`.

---

## XamContentGetDeviceData Return Data

### Xbox 360 XDEVICE_DATA Structure (0x50 bytes)

| Offset | Type         | Field             | Description              |
|--------|--------------|-------------------|--------------------------|
| 0x00   | be<uint32_t> | DeviceID          | Device identifier        |
| 0x04   | be<uint32_t> | DeviceType        | 1=HDD, 2=MU             |
| 0x08   | be<uint64_t> | ulDeviceBytes     | Total device capacity    |
| 0x10   | be<uint64_t> | ulDeviceFreeBytes | Free space on device     |
| 0x18   | be<uint16_t>[28] | wszName        | Device display name      |

Defined in `/Users/Ozordi/Downloads/LibertyRecomp/tools/XenonRecomp/XenonUtils/xbox.h` line 350.

### LibertyRecomp Implementation

File: `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/kernel/xam.cpp` line 513

```c
pDeviceData->ulDeviceBytes     = 0x40000000;  // 1 GB total
pDeviceData->ulDeviceFreeBytes = 0x40000000;  // 1 GB free
```

These are `be<uint64_t>` fields assigned `uint32_t` values. The assignment
works correctly (implicit widening), producing 64-bit big-endian values.

### RexGlue Implementation

File: `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/src/kernel/xam/xam_content_device.cpp` line 39

```c
20ull * ONE_GB,  // total_bytes = 20 GB
3ull * ONE_GB,   // free_bytes  = 3 GB
```

Reports device capacity and free space. NOT content-specific size.
NOT the "expected size" of any particular file.

---

## What is the "Expected Size" and Where Does It Come From?

The "expected size" is NOT from `XamContentGetDeviceData`. It is computed by
`sub_82A12720` -- an **STFS cluster size calculator**.

### sub_82A12720 (STFS Size Calculator)

Address: `0x82A12720`
File: `gta4_recomp.69.cpp` line 36546

**Inputs:**
- `r3` = raw content size in bytes
- `r4` = secondary parameter (0 for save data operations)

**Returns:**
- `r3` = total STFS container size in bytes (aligned to 4KB blocks)

**Algorithm:**
1. Rounds raw size up to 4KB page boundaries
2. Adds 2 base pages (8KB) for STFS header data
3. Computes hash table overhead: every 170 data blocks needs 1 hash block
   (3-level STFS hash chain: L0 / L1 / L2)
4. Adds 26 fixed blocks for STFS container metadata
5. Returns `(total_blocks) * 4096`

**Example outputs (Python-verified, r4=0):**

| Raw file size | Data pages | Hash blocks | Total STFS blocks | STFS bytes |
|---------------|------------|-------------|-------------------|------------|
| 0 bytes       | 2          | 1           | 30                | 0x1E000 (120 KB) |
| 256 KB        | 66         | 1           | 94                | 0x5E000 (376 KB) |
| 512 KB        | 130        | 1           | 158               | 0x9E000 (632 KB) |
| 1 MB          | 258        | 3           | 290               | 0x122000 (1.13 MB) |

---

## slot[136] and slot[144] -- What They Represent

These are fields within a content management slot in the array at base
address `0x83192C58`. Each slot is 160 bytes (computed via stride calculation).

### slot[136] -- Expected STFS Container Size

Set in `sub_8284A7E8` (content creation function), line 10869-10875:

```
// r3 = content_size_from_caller, r4 = 0
sub_82A12720(r3, r4=0)         // compute STFS container size
std r3, 136(r31)               // store to slot[136]
std r30, 144(r31)              // r30=0, initialize slot[144] to 0
```

`slot[136]` = the STFS container size for the REQUESTED content size.
This is set once during content creation and represents "how much space
the STFS container needs for this content."

### slot[144] -- Actual STFS Container Size (on disk)

Set in `sub_8284ADA0` (content verification function), line 11770-11778,
**but only in the state==17 path:**

```
// After overlapped I/O completes:
// Opens content file via CreateFileA at slot+64
// Gets raw file size via sub_82A135B0 (GetFileSizeEx wrapper)
sub_82A12720(actual_file_size, 0)   // compute STFS size from actual
std r3, 144(r31)                     // store to slot[144]
```

In the **state==16 path** (loc_8284AF48), `slot[144]` is read but never
written -- it retains whatever value it had from initialization (0).

---

## The Comparison Logic in sub_8284ADA0

### State 17 path (async, Xbox 360 normal):

```
// Line 11856-11863
r11 = slot[136]   // expected STFS size
r10 = slot[144]   // actual STFS size (populated by GetFileSizeEx)
if r11 != r10:
    goto size_mismatch_handler   // loc_8284AFA0
else:
    output_delta = 0             // success
    return 1
```

### State 16 path (sync, LibertyRecomp path):

```
// Line 11898-11910 (loc_8284AF48)
slot[0] = 18          // advance state
r10 = slot[144]       // actual size (STILL 0 from init!)
r11 = slot[136]       // expected size (positive value)
if r11 <= r10:
    output_delta = 0  // success
else:
    // compute shortfall, call sub_8284A078 for free space check
```

---

## Root Cause: Synchronous vs Asynchronous XamContentCreateEx

### On Xbox 360:
1. `XamContentCreateEx` returns `ERROR_IO_PENDING` (997)
2. Slot state is set to **17**
3. `sub_8284ADA0` enters the state-17 branch
4. Polls `OVERLAPPED` until I/O completes
5. Opens the content file on disk, measures it with `GetFileSizeEx`
6. Computes `slot[144] = sub_82A12720(actual_file_size, 0)`
7. Compares `slot[136]` vs `slot[144]` -- they match (the content system
   wrote the exact container size requested)

### In LibertyRecomp:
1. `XamContentCreateEx` returns `ERROR_SUCCESS` (0) synchronously
2. Slot state is set to **16**
3. `sub_8284ADA0` enters the state-16 branch (loc_8284AF48)
4. **Skips the file open + GetFileSizeEx + sub_82A12720 calculation**
5. Reads `slot[144]` which is still **0** from initialization
6. `slot[136]` (positive, e.g., 0x1E000) > `slot[144]` (0) => **MISMATCH**
7. State machine enters state 13 (error recovery / "not enough storage")

---

## Values in the Recomp vs What Should Be Returned

### XamContentGetDeviceData

| Field             | LibertyRecomp      | RexGlue           | Note |
|-------------------|--------------------|--------------------|----|
| ulDeviceBytes     | 0x40000000 (1 GB)  | 20 GB              | LR value is fine |
| ulDeviceFreeBytes | 0x40000000 (1 GB)  | 3 GB               | LR value is fine |

**XamContentGetDeviceData is not the problem.** It reports total/free device
capacity (used by `sub_8284B3D8` as a simple device-connected check). The
actual size comparison uses `sub_82A12720` output, not device data directly.

### The Real Problem Values

| Field     | Expected (state 17) | Actual (state 16) | Why |
|-----------|---------------------|--------------------|-----|
| slot[136] | sub_82A12720(N, 0)  | sub_82A12720(N, 0) | Same in both paths |
| slot[144] | sub_82A12720(file_size, 0) | **0** (never set) | State 16 skips file measurement |

---

## Call Chain Summary

```
sub_82242910 (outer state machine, state 12)
  -> sub_822417B0 (r4=1, initiate)
    -> sub_8284AD78
      -> sub_8284A7E8 (content creation)
        -> sub_82A12720(content_size, 0) => slot[136]
        -> slot[144] = 0
        -> sub_82A127A0 (XamContentCreateEx thunk)
           returns 0 (sync) => slot[0] = 16
           returns 997 (async) => slot[0] = 17
  -> sub_822417B0 (r4=0, poll/verify)
    -> sub_8284ADA0 (content verification)
        state==17: GetFileSizeEx -> sub_82A12720 -> slot[144], then compare
        state==16: compare slot[136] vs slot[144] directly (slot[144] still 0)
```

---

## Fix Options

1. **Make XamContentCreateEx return ERROR_IO_PENDING (997)** and set up a
   valid OVERLAPPED structure so the state-17 path is taken. The OVERLAPPED
   must eventually complete with success so the file measurement happens.

2. **Hook sub_8284ADA0** to force `slot[144] = slot[136]` when state==16,
   making the size comparison always pass.

3. **Hook sub_8284A7E8** to set `slot[144]` to the same value as `slot[136]`
   at creation time, so the state-16 path comparison succeeds.

4. **Hook sub_82242910 state 12/14** to bypass the sub_822417B0 poll call
   entirely and set the success outputs directly.

---

## sub_8284B3D8 -- Device Data Check

Address: `0x8284B3D8`
File: `gta4_recomp.55.cpp` line 12572

Simple function that checks if a save slot has a valid device. Loads
`slot[56]` (device_id), and if nonzero calls `XamContentGetDeviceData`
(via thunk `sub_82A12718`). Returns 1 if the call succeeds, 0 otherwise.

This function is a **gate check** -- it answers "is there a storage device
at all?" It does NOT use the returned size values for comparison purposes.
The device data struct is allocated on the stack and discarded.
