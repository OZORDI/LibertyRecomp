# 35. Fix Hook for sub_8284ADA0 (Content Size Comparison)

## Problem Summary

`sub_8284ADA0` is the content size verification function. When the content slot
is in **state 16** (synchronous I/O completion), the function jumps directly to
the size comparison at `loc_8284AF48` **without ever measuring the file**.

On Xbox 360, async I/O almost always returns STATUS_PENDING (997), so the slot
enters **state 17**. State 17 includes file measurement (CreateFileA + GetFileSize)
that populates `slot+144` (actual_size). In the recompilation, the rexcrt I/O
layer completes synchronously, so the slot enters state 16 instead. State 16 was
the rare/fast path on Xbox and assumed the kernel had already populated the size
metadata.

**Result**: `slot+144 = 0` (never written), `slot+136 > 0` (expected size from
STFS calculator), so the comparison reports a deficit. The caller (state 14 in the
outer state machine) reads a negative delta from `0x82BF99C8`, transitions to
state 13 (storage error), and the save system loops forever.

Additionally, when state 16 is set (line 10941-10952 of `gta4_recomp.55.cpp`),
the function **returns immediately** without building the file path at `slot+64`.
The file path construction happens at `loc_8284A958` (line 10992+), which is only
reached by the state 17/20 code paths. This means even if we patched state 16
to measure the file, there would be no path string to open.

## Code Path Map

### Entry (line 11653)

```
Parameters:
  r3 = 0x83192C50 (unused, table header)
  r4 = slot index
  r5 = output pointer for delta KB (saved as r26)

Computed:
  r28 = 0x83192C58 (slot table base)
  r31 = r28 + (r4 * 160) (slot pointer)
  r29 = 0 (constant)

state = load_u32(slot + 0)
```

### State 16 Path (lines 11684-11686 -> loc_8284AF48)

```
if (state == 16) goto loc_8284AF48;

loc_8284AF48:  (line 11898)
  slot[0] = 18                    // transition to state 18
  actual   = load_u64(slot+144)   // PROBLEM: never populated, always 0
  expected = load_u64(slot+136)   // populated by sub_8284A7E8 during init
  if (expected <= actual) goto SUCCESS;
  delta = expected - actual
  free_space = sub_8284A078(slot) // XamContentGetDeviceData -> 0 in recomp
  if (free_space >= delta) goto SUCCESS;
  // compute shortfall in KB, write negative to output -> triggers state 13
```

**Bug**: `slot+144` is zero because:
1. The file path at `slot+64` was never written (path building is after state 17
   assignment at `loc_8284A958`, state 16 returns before reaching it).
2. The file measurement (CreateFileA + GetFileSize + sub_82A12720 -> slot+144)
   only runs inside the state 17 handler.

### State 17 Path (lines 11688-11690 -> loc_8284ADEC)

```
if (state == 17) goto loc_8284ADEC;

loc_8284ADEC:  (line 11699)
  // 1. Check async completion
  result = sub_82A11EB8(slot+8, &local, 0)   // GetOverlappedResult
  if (result == 996) return 0                  // still pending
  if (result != 0) goto check_state_for_cleanup

  // 2. Check sub_state == 1 (read mode)
  if (slot[4] != 1) goto error_or_write_path

  // 3. MEASURE THE FILE (THIS IS THE WORKING PATH)
  handle = CreateFileA(slot+64, GENERIC_READ, FILE_SHARE_ALL, NULL,
                       OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)
  if (handle == INVALID_HANDLE) goto error
  size_low = GetFileSize(handle, NULL)         // sub_82A135B0
  if (size_low == -1) { GetLastError(); CloseHandle(handle); goto error; }
  actual_size = sub_82A12720(size_low, 0)      // STFS block calculator
  store_u64(slot+144, actual_size)             // POPULATES actual_size
  CloseHandle(handle)

  // 4. Clear async data, close content, compare sizes
  ... (same comparison logic as state 16 path)
```

### State 18

Set by state 16 path. Not checked at entry. Returns 0 (function only handles
16 and 17). Indicates "size check done" to the outer state machine.

### State 21

Set by the state 17 path after comparison. Same as state 18 but for the async
completion case. Also returns 0 if re-entered.

### Default (any other state)

`loc_8284ADE0`: return 0 immediately.

## Slot Structure (160 bytes at 0x83192C58)

| Offset | Size | Field | Notes |
|--------|------|-------|-------|
| +0 | 4 | state | 0=idle, 16=sync-done, 17=async-pending, 18/21=compared |
| +4 | 4 | sub_state | 1=read, 2=write |
| +8 | 28 | OVERLAPPED | Async I/O completion struct |
| +56 | 4 | content_handle | For sub_8284A078 (XamContentGetDeviceData) |
| +64 | 72 | file_path | String, built ONLY for state 17/20 |
| +136 | 8 | expected_size | STFS_alloc(requested_size), set during init |
| +144 | 8 | actual_size | STFS_alloc(file_size), set ONLY by state 17 |

## Callers of sub_8284ADA0

**Single caller**: `sub_822417B0` (two-phase content size check), in the poll
path (r4=0), at `gta4_recomp.6.cpp` line 82304.

The call site is in `loc_82241884` with `r29 == 0` (single-player path).
The multi-player path (`r29 != 0`) calls `sub_8284B038` instead.

No other callers exist in the codebase.

## Hook Strategy Analysis

### Option A: Post-call clamp (override negative delta)

```cpp
extern "C" void __imp__sub_8284ADA0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8284ADA0)
{
    uint32_t output_ptr = ctx.r5.u32;  // save before call (r5 = output pointer)

    __imp__sub_8284ADA0(ctx, base);

    // After return, check if a negative delta was written
    if (ctx.r3.u32 == 1 && output_ptr != 0) {
        int32_t delta = (int32_t)PPC_LOAD_U32(output_ptr);
        if (delta < 0) {
            PPC_STORE_U32(output_ptr, 0);  // clamp to 0 = no deficit
        }
    }
}
```

**Pros**: Simple, safe, always works regardless of state.
**Cons**: Masks the real problem. If there IS a legitimate storage deficit on
a real system, this would hide it. However, in the recomp context, storage
deficits are meaningless (no STFS packages, no real device limits).

### Option B: Pre-call state patch (force state 17 behavior)

```cpp
extern "C" void __imp__sub_8284ADA0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8284ADA0)
{
    // Compute slot pointer: slot = 0x83192C58 + (r4 * 160)
    uint32_t slot_index = ctx.r4.u32;
    uint32_t slot_addr = 0x83192C58 + (slot_index * 160);
    uint32_t state = PPC_LOAD_U32(slot_addr + 0);

    if (state == 16) {
        // Patch to state 17 so the function takes the file measurement path
        PPC_STORE_U32(slot_addr + 0, 17);
    }

    __imp__sub_8284ADA0(ctx, base);
}
```

**Pros**: Uses the game's own file measurement logic.
**Cons**: WILL NOT WORK. When state 16 is set, the file path at `slot+64` was
never built (the path construction at `loc_8284A958` is only reached by the
state 17/20 code path in `sub_8284A7E8`). So CreateFileA would open garbage
or an empty string. Also, the async OVERLAPPED data at `slot+8` was never
initialized, so `sub_82A11EB8` (GetOverlappedResult) would read uninitialized
memory.

### Option C: Pre-call -- populate slot+144 with slot+136

```cpp
extern "C" void __imp__sub_8284ADA0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8284ADA0)
{
    uint32_t slot_index = ctx.r4.u32;
    uint32_t slot_addr = 0x83192C58 + (slot_index * 160);
    uint32_t state = PPC_LOAD_U32(slot_addr + 0);

    if (state == 16) {
        // For synchronous completion in recomp, the actual file size is
        // irrelevant (no STFS packages). Set actual = expected to pass
        // the comparison.
        uint64_t expected = PPC_LOAD_U64(slot_addr + 136);
        PPC_STORE_U64(slot_addr + 144, expected);
    }

    __imp__sub_8284ADA0(ctx, base);
}
```

**Pros**: Makes the function's own comparison pass naturally. The state
transitions (16 -> 18) proceed correctly. No negative delta is produced.
**Cons**: Slightly dishonest -- we claim the actual size matches expected.
But in the recomp there are no STFS packages, so this is semantically correct:
the flat file system has no allocation unit mismatch.

### Option D: Bypass the function entirely for state 16

```cpp
extern "C" void __imp__sub_8284ADA0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8284ADA0)
{
    uint32_t slot_index = ctx.r4.u32;
    uint32_t slot_addr = 0x83192C58 + (slot_index * 160);
    uint32_t output_ptr = ctx.r5.u32;
    uint32_t state = PPC_LOAD_U32(slot_addr + 0);

    if (state == 16) {
        // Synchronous completion in recomp: skip size comparison entirely.
        // Set state to 18 (done) and report no deficit.
        PPC_STORE_U32(slot_addr + 0, 18);
        if (output_ptr != 0) {
            PPC_STORE_U32(output_ptr, 0);  // no deficit
        }
        ctx.r3.s64 = 1;  // return 1 = complete
        return;
    }

    __imp__sub_8284ADA0(ctx, base);
}
```

**Pros**: Cleanest solution. Avoids running any of the broken state 16 logic.
Correctly transitions to state 18 (which is what state 16 does naturally).
Writes 0 delta. Returns 1 (complete).
**Cons**: Skips `sub_8284A078` (XamContentGetDeviceData) -- but that returns 0
in the recomp anyway. Also skips setting `slot+144`, but nothing reads it after
state 18 is set.

## Recommended Hook: Option D

Option D is the cleanest and most robust. It:

1. Only activates for state 16 (sync completion) -- the broken path.
2. Does exactly what the state 16 path WOULD do if sizes matched:
   sets state = 18, writes 0 to output, returns 1.
3. Does not interfere with state 17 (async) which works correctly.
4. Has no side effects on the single caller.

### Concrete Implementation

```cpp
// In save_hooks.cpp or a new content_hooks.cpp:

extern "C" void __imp__sub_8284ADA0(PPCContext& ctx, uint8_t* base);
PPC_FUNC(sub_8284ADA0)
{
    // Content slot table base = 0x83192C58, stride = 160 bytes
    // r4 = slot index, r5 = output delta pointer
    uint32_t slot_index = ctx.r4.u32;
    uint32_t slot_addr  = 0x83192C58u + (slot_index * 160u);
    uint32_t state      = PPC_LOAD_U32(slot_addr + 0);

    if (state == 16) {
        // State 16 = synchronous content creation completion.
        // On Xbox 360 this is rare (I/O is usually async -> state 17).
        // In the recomp, I/O completes synchronously, so state 16 is the
        // norm. The state 16 path in the original code assumes slot+144
        // (actual_size) was populated by the kernel, but rexcrt doesn't
        // do this. The file path at slot+64 was also never built.
        //
        // Fix: transition to state 18 (done) with zero deficit.
        // This matches the successful outcome of the size comparison.
        PPC_STORE_U32(slot_addr + 0, 18);  // state -> 18 (comparison done)

        uint32_t output_ptr = ctx.r5.u32;
        if (output_ptr != 0) {
            PPC_STORE_U32(output_ptr, 0);   // delta KB = 0 (no deficit)
        }

        ctx.r3.s64 = 1;  // return 1 = operation complete
        return;
    }

    // All other states: run original implementation
    __imp__sub_8284ADA0(ctx, base);
}
```

## Why This Is Safe

1. **Single caller**: Only `sub_822417B0` calls `sub_8284ADA0`, in the poll path.
   The caller checks `r3 != 0` to detect completion, then reads `[0x82BF99C8]`
   for the delta. Writing 0 there means "no storage shortfall."

2. **State 18 is correct**: The original state 16 path always transitions to 18.
   We just skip the comparison that would fail.

3. **No STFS in recomp**: The entire size comparison exists to verify that the
   STFS content package was allocated correctly on the Xbox 360 storage device.
   In the recomp, files are flat on the host filesystem. There is no allocation
   unit mismatch to detect.

4. **State 17 still works**: If any future code path produces state 17 (e.g.,
   if async I/O is re-enabled), the original logic handles it correctly.

## Key Source Locations

| File | Line | Content |
|------|------|---------|
| `gta4_recomp.55.cpp` | 11651-11979 | sub_8284ADA0 (full function) |
| `gta4_recomp.55.cpp` | 10783-11052 | sub_8284A7E8 (slot init, sets state 16/17) |
| `gta4_recomp.55.cpp` | 10941-10952 | State 16 assignment (sync, returns immediately) |
| `gta4_recomp.55.cpp` | 10978-10991 | State 17 assignment (async, falls into path building) |
| `gta4_recomp.55.cpp` | 10992-11044 | File path construction at slot+64 (state 17/20 only) |
| `gta4_recomp.6.cpp` | 82304 | Call site in sub_822417B0 (poll path) |
| `gta4_recomp.55.cpp` | 9695-9771 | sub_8284A078 (get device free space) |

All generated code in:
`/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/`
