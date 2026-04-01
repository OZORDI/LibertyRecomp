# 32. Size Comparison Logic in sub_8284ADA0

## Function Signature

```
sub_8284ADA0(r3=table_ptr_unused, r4=slot_index, r5=output_delta_ptr)
```

- **r3**: Ignored in the function body (slot_ptr is recomputed from r4)
- **r4**: Slot index into the content table at `0x83192C58`
- **r5**: Pointer to a `uint32_t` where the size delta (in KB) is written; saved as `r26`
- **Returns**: `r3` = 1 on completion, 0 if still pending

The slot pointer is computed as:
```
slot = 0x83192C58 + slot_index * 160
```

Slot size = 160 bytes (0xA0). Computation: `index * 5 * 32`.

## Slot Structure (160 bytes at table 0x83192C58)

| Offset | Size | Field | Description |
|--------|------|-------|-------------|
| +0 | 4 | state | State machine variable (0,1,2,16,17,18,19,20,21) |
| +4 | 4 | mode | Sub-state: 1=read, 2=write |
| +8 | 28 | OVERLAPPED | Async I/O completion struct (zeroed between uses) |
| +36 | 20 | (reserved) | Unknown / XCONTENT_DATA fields |
| +56 | 4 | content_handle | XamContent handle from XamContentCreateEx |
| +60 | 4 | (padding) | |
| +64 | 72 | file_path | Null-terminated path string (max 72 chars) |
| +136 | 8 | expected_size | STFS allocation for requested size (uint64) |
| +144 | 8 | actual_size | STFS allocation for actual file size (uint64) |
| +152 | 8 | (padding) | To 160-byte boundary |

## State Machine Overview

sub_8284ADA0 handles **states 16, 17**, and transitions to **18, 21**:

- **State 16**: Direct size comparison (content was already open)
- **State 17**: Open file, get actual size, close content, then compare
- **State 18**: Set after comparison in state 16 path
- **State 21**: Set after comparison in non-17 path

Any other state returns 0 immediately.

## How slot[136] (Expected Size) Is Populated

**Writer**: `sub_8284A7E8` (content creation initiator)

At line 10869-10875 of `gta4_recomp.55.cpp`:
```
// r29 = r6 parameter = requested content size
r3 = r29 & 0xFFFFFFFF          // clrldi r3,r29,32
r4 = 0                          // li r4,0
r3 = sub_82A12720(r3, r4)       // STFS block calculator
std r3, 136(r31)                 // slot[136] = STFS_alloc(requested_size)
std r30, 144(r31)                // slot[144] = 0 (initialized to zero)
```

**sub_82A12720** is the **STFS block allocation calculator**. Given a raw file size, it computes the total STFS package size including:
- Data blocks (4096 bytes each, rounded up)
- Level 0 hash table blocks (1 per 170 data blocks)
- Level 1 hash table blocks (1 per 170 L0 blocks)
- Level 2 hash table blocks (1 per 170 L1 blocks)
- 26-block metadata overhead
- Returns total * 4096 (byte count)

After computing slot[136], sub_8284A7E8 calls `sub_82A127A0` which wraps `sub_82A12690` which calls **XamContentCreateEx** (at `0x82A74C34`). This creates the STFS content package on the storage device.

**sub_82A116E8** (called in sub_8284A0F8) is **XamShowDeviceSelectorUI** -- the device selector, not the content creator.

## How slot[144] (Actual Size) Is Populated

**Writer**: `sub_8284ADA0` itself, in the **state 17** path.

At lines 11727-11778 of `gta4_recomp.55.cpp`:

```c
// Step 1: Check overlapped result for XamContentCreateEx completion
r3 = sub_82A11EB8(OVERLAPPED, &stack_result, 0)  // GetOverlappedResult
if (r3 == 996) return 0;  // still pending
if (r3 != 0) goto error;

// Step 2: Check mode (must be 1 = read/open)
if (slot[4] != 1) goto error_or_write_path;

// Step 3: Open the file to get its size
handle = CreateFileA(
    slot+64,          // file path
    0x80000000,       // GENERIC_READ
    7,                // FILE_SHARE_READ|WRITE|DELETE
    NULL,             // security
    3,                // OPEN_EXISTING
    128,              // FILE_FLAG_SEQUENTIAL_SCAN (0x80)
    NULL              // template
);
if (handle == -1) goto error;

// Step 4: Get actual file size
actual_size_low = sub_82A135B0(handle, NULL);
// sub_82A135B0 wraps GetFileSizeEx (rexcrt_GetFileSizeEx at 0x82A14918)
// Returns low 32-bit file size, or -1 on error

if (actual_size_low == -1) { GetLastError(); CloseHandle(handle); goto error; }

// Step 5: Convert to STFS allocation via block calculator
r3 = sub_82A12720(actual_size_low, 0);  // STFS block calc
std r3, 144(r31);                        // slot[144] = STFS_alloc(actual_file_size)

CloseHandle(handle);
```

## The Comparison Logic

There are **two comparison sites**, both comparing slot[136] vs slot[144]:

### Comparison 1: loc_8284AEF0 (state 21 path)

Reached when the async content close completes and state was not 17:

```
slot[0] = 21
sub_8284A078(slot)                    // calls XamContentGetDeviceData → gets free space
r11 = load_u64(slot + 136)           // expected_size
r10 = load_u64(slot + 144)           // actual_size
if (r11 == r10) → SUCCESS            // sizes match, write 0 to output
if (r11 != r10) → loc_8284AFA0       // MISMATCH
```

At **loc_8284AFA0**:
```
r11 = expected_size - actual_size     // subf: unsigned subtraction
cmpld r11, 0                         // unsigned compare with 0
if (r11 > 0):                        // expected > actual
    r11 = (r11 + 1023) >> 10         // convert to KB (round up)
    *output_ptr = r11                 // POSITIVE delta (KB needed)
if (r11 <= 0):                       // unsigned: only possible if r11==0 (dead code here)
    r11 = (r11 - 1023) >> 10         // (unreachable from this path)
    *output_ptr = r11
```

### Comparison 2: loc_8284AF48 (state 16/18 path) -- THE CRITICAL ONE

Reached from state 16 entry or after state 17 transitions to 18:

```
slot[0] = 18
r10 = load_u64(slot + 144)           // actual_size
r11 = load_u64(slot + 136)           // expected_size
cmpld r11, r10                       // unsigned 64-bit compare

if (r11 <= r10) → SUCCESS            // expected <= actual: write 0 to output

if (r11 > r10):                      // expected > actual: FILE IS TOO SMALL
    delta = r11 - r10                // bytes needed beyond actual
    free_space = sub_8284A078(slot)   // XamContentGetDeviceData → free bytes
    if (free_space >= delta) → SUCCESS // device has enough room to extend

    // INSUFFICIENT SPACE PATH:
    free_KB  = (free_space + 1023) >> 10    // round up to KB
    needed_KB = (delta + 1023) >> 10        // round up to KB
    shortfall = free_KB - needed_KB         // NEGATIVE VALUE
    *output_ptr = shortfall                 // Write negative KB delta
```

**This negative delta is the value written to the output pointer (0x82BF99C8 in the caller context).** It represents how many KB the storage device is short of the needed space. The caller interprets a nonzero value as a storage error, triggering state 13 (error recovery) and the infinite restart loop.

## sub_8284A078: Get Device Free Space

```c
sub_8284A078(slot_ptr):
    handle = slot[56]                         // content handle
    if (handle == 0) return 0

    result = XamContentGetDeviceData(handle, &device_data)  // sub_82A12718
    if (result != 0) return 0

    // Retry once (unclear why)
    XamContentGetDeviceData(handle, &device_data)
    return device_data.DeviceFreeBytes         // 64-bit free space at offset +16 of XDEVICE_DATA
```

## Root Cause of the Infinite Loop

### On Xbox 360 (working)
1. `sub_8284A7E8` requests content creation with size S
2. `sub_82A12720(S, 0)` → slot[136] = STFS_alloc(S)
3. XamContentCreateEx creates STFS package of exactly STFS_alloc(S) bytes
4. `sub_8284ADA0` opens file, GetFileSizeEx returns STFS_alloc(S)
5. `sub_82A12720(STFS_alloc(S), 0)` → slot[144] = STFS_alloc(STFS_alloc(S))
6. **Wait** -- both go through the STFS calculator, so: slot[136] = STFS_calc(S), slot[144] = STFS_calc(GetFileSizeEx). On Xbox, the actual file IS the STFS package, so GetFileSizeEx returns the STFS-allocated size, and STFS_calc(STFS_size) >= STFS_calc(S). The `ble` (less-or-equal) comparison passes.

### On PC Recomp (broken)
1. `sub_8284A7E8` requests content creation with size S
2. `sub_82A12720(S, 0)` → slot[136] = STFS_alloc(S) (e.g., 0x1E000 for empty save)
3. XamContentCreateEx is emulated -- creates a directory/flat file, NOT an STFS package
4. `sub_8284ADA0` opens file, GetFileSizeEx returns actual flat file size (e.g., 0 for new empty file)
5. `sub_82A12720(0, 0)` → slot[144] = STFS_alloc(0) = 0x1E000 for zero-length input
6. Comparison: slot[136] vs slot[144]

**Actually**, since both sizes go through `sub_82A12720`, the comparison might match for some sizes. The mismatch occurs when the requested content size and the actual file size produce different STFS allocations. For a new save file:
- Requested size might be nonzero (e.g., the game requests a specific allocation)
- Actual file on disk is 0 bytes or very small
- STFS_calc(requested) >> STFS_calc(0)

The negative delta triggers state 13 error recovery in the caller's state machine, which restarts the save operation, creating the infinite loop.

## Example Calculation

For a GTA IV save file (requested size = 262144 bytes = 256 KB):

| Step | Value |
|------|-------|
| sub_82A12720(262144, 0) | 385024 (0x5E000) -- 66 data blocks + 1 hash + 26 metadata |
| slot[136] | 0x000000000005E000 |
| Actual file on PC | 0 bytes (new save) |
| sub_82A12720(0, 0) | 122880 (0x1E000) -- 2 data blocks + 1 hash + 26 metadata |
| slot[144] | 0x000000000001E000 |
| expected > actual? | YES: 0x5E000 > 0x1E000 |
| delta | 0x40000 (262144 bytes) |
| sub_8284A078 free_space | 0 (emulated, no real STFS device) |
| shortfall | 0 - 256 = -256 KB |
| *output_ptr | 0xFFFFFF00 (negative 256 as uint32) |

## Summary of Call Chain

```
sub_8284A7E8  (content create)
  ├── sub_82A12720(requested_size, 0)  → slot[136]
  ├── slot[144] = 0
  └── sub_82A12690 → XamContentCreateEx

sub_8284ADA0  (size verification, called repeatedly)
  ├── State 17: Open file, get actual size
  │   ├── sub_82A11EB8  (GetOverlappedResult)
  │   ├── CreateFileA(slot+64)
  │   ├── sub_82A135B0(handle)  → GetFileSizeEx
  │   ├── sub_82A12720(actual_size, 0)  → slot[144]
  │   └── sub_82A126F8  (XamContentClose)
  ├── State 16/18: Compare sizes
  │   ├── load slot[136] and slot[144]
  │   ├── If expected <= actual: SUCCESS (write 0)
  │   └── If expected > actual:
  │       ├── sub_8284A078  → XamContentGetDeviceData → free_space
  │       ├── If free >= delta: SUCCESS
  │       └── If free < delta: write (free_KB - needed_KB) → NEGATIVE → triggers state 13
  └── State 21: Equality check
      ├── If equal: SUCCESS
      └── If unequal: write positive KB delta
```

## Key Addresses

| Address | Identity |
|---------|----------|
| 0x83192C58 | Content slot table (base, computed: `lis -31975 + addi 11352`) |
| 0x82B0710C | Device path table (computed: `lis -32080 + addi 28940`) |
| sub_82A12720 | STFS block allocation calculator |
| sub_82A135B0 | GetFileSize wrapper (calls GetFileSizeEx) |
| sub_82A12718 | XamContentGetDeviceData thunk |
| sub_82A126F8 | XamContentClose thunk |
| sub_82A12690 | XamContentCreateEx wrapper (validates params, calls XamContentCreateEx) |
| sub_82A127A0 | XamContentCreate wrapper (calls sub_82A12690) |
| sub_82A11EB8 | GetOverlappedResult wrapper |
| sub_8284A078 | Get device free space (calls XamContentGetDeviceData, returns DeviceFreeBytes) |

## Source File Location

All generated code in:
`/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.55.cpp`

- sub_8284ADA0: line 11651
- sub_8284A7E8: line 10783
- sub_8284A0F8: line 9773
- sub_8284A078: line 9695

Helper thunks in `gta4_recomp.69.cpp`:
- sub_82A12720 (STFS calc): line 36546
- sub_82A135B0 (GetFileSize): line 38140
- sub_82A12690 (XamContentCreateEx): line 36434
