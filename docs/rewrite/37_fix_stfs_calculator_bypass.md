# 37. STFS Block Calculator Bypass -- sub_82A12720 Analysis

## Summary

**sub_82A12720** is an STFS container size calculator. Given a raw file size,
it returns the total byte count of an STFS package that would hold that data.
Hooking it to return the input unchanged (identity function) was proposed as
a fix for the save size mismatch. **This analysis shows that approach does NOT
fix the actual bug** and would be counterproductive.

---

## sub_82A12720 Decompilation

**Location**: `gta4_recomp.69.cpp` line 36548
**Address**: `0x82A12720`

### Inputs
- `r3` = raw content size in bytes (uint32)
- `r4` = secondary parameter (always 0 in save-related calls)

### Output
- `r3` = total STFS container size in bytes (uint64, 4KB-aligned)

### Algorithm (Python-verified)

```python
def stfs_calc(content_size, flag):
    # Step 1: Metadata overhead (when flag=0: 2 blocks = 8192 bytes)
    meta = ((flag << 6) & 0xFFFFFFC0 + 4159) << 1) & 0xFFFFE000  # = 8192 when flag=0

    # Step 2: Round content to 4K pages
    content_pages = (content_size + 4095) & 0xFFFFF000

    # Step 3: Total pre-hash blocks
    total_blocks = (meta + content_pages) >> 12

    # Step 4: STFS 3-level hash chain (1 hash block per 170 data blocks)
    L0 = ceil(total_blocks / 170)
    L1 = ceil(L0 / 170) if L0 > 1 else 0
    L2 = ceil(L1 / 170) if L1 > 1 else 0

    # Step 5: Final size
    result_blocks = (L0 + L1 + L2) * 2 + total_blocks + 26
    return result_blocks * 4096
```

### Example Outputs (r4=0, Python-verified)

| Input (bytes) | Data blocks | Hash blocks | Total STFS blocks | Output (bytes) |
|---------------|-------------|-------------|-------------------|----------------|
| 0             | 2           | 1 (L0)     | 30                | 122,880        |
| 262,144       | 66          | 1 (L0)     | 94                | 385,024        |
| 524,288       | 130         | 1 (L0)     | 158               | 647,168        |
| 655,360       | 162         | 1 (L0)     | 190               | 778,240        |
| 782,336       | 193         | 2+1 (L0+L1)| 222               | 909,312        |

The 26-block fixed overhead represents STFS container metadata (header,
content metadata, volume descriptor, etc.). Each hash level doubles because
STFS stores both a primary and backup hash table.

---

## All Call Sites (4 total, all in gta4_recomp.55.cpp)

### Call Site 1: sub_8284A0F8, line 9836
- **r3** = arg5 to sub_8284A0F8 (content size for device selector)
- **r4** = 0
- **Result** -> passed as r6 to `sub_82A116E8` (XamShowDeviceSelectorUI)
- **Purpose**: Tells device selector how much space will be needed
- **Impact of identity hook**: Device selector shows raw size instead of STFS-inflated size. Cosmetic only.

### Call Site 2: sub_8284A7E8, line 10871 -- WRITES slot[136]
- **r3** = r29 = r6 parameter = content size requested by caller
- **r4** = 0
- **Result** -> stored to `slot[136]` (expected STFS container size)
- **Purpose**: Records expected container size at content creation time
- **Immediately after**: `slot[144] = 0` (initialized to zero)

### Call Site 3: sub_8284ADA0, line 11776 -- WRITES slot[144]
- **r3** = GetFileSizeEx result (raw file size on disk)
- **r4** = 0
- **Result** -> stored to `slot[144]` (actual STFS container size)
- **Purpose**: Records actual container size after measuring file
- **Only reached in STATE 17 path** (async XamContentCreateEx)

### Call Site 4: sub_8284B038, line 12186 -- WRITES slot[144]
- **r3** = GetFileSizeEx result (raw file size on disk)
- **r4** = 0
- **Result** -> stored to `slot[144]` (actual STFS container size)
- **Purpose**: Same as call site 3 but in the state 19/20 path (write verification)
- **Only reached in STATE 17/20 path** (async)

---

## Why the Identity Hook Does NOT Fix the Bug

### The Actual Bug (from docs 30 and 32)

The bug is **NOT** that sub_82A12720 produces wrong values. The bug is:

1. `XamContentCreateEx` returns `ERROR_SUCCESS` (0) synchronously
2. This sets slot state to **16** (not 17)
3. The state 16 path in sub_8284ADA0 **never opens the file**
4. Therefore **sub_82A12720 is never called** for slot[144]
5. slot[144] remains **0** from initialization
6. Comparison: slot[136] (positive) > slot[144] (0) => **MISMATCH**

### The Function Is Only Called in State 17

sub_82A12720 is called for slot[144] only in the **state 17** path (call
sites 3 and 4). In state 16, the function is never invoked for slot[144].
The comparison reads slot[144]'s stale zero value directly.

**Hooking sub_82A12720 to return identity has ZERO EFFECT on the state 16
comparison because the function is never called in that path.**

### Even in State 17, Identity Hook Provides No Benefit

If the XamContentCreateEx path were changed to return ERROR_IO_PENDING
(forcing state 17), the state 17 path would run:

```
slot[136] = sub_82A12720(content_size, 0)
slot[144] = sub_82A12720(GetFileSizeEx_result, 0)
```

With STFS calc: both values go through the same function, so they match
if and only if `content_size == GetFileSizeEx_result`.

With identity: `slot[136] = content_size`, `slot[144] = GetFileSizeEx_result`.
They match if and only if `content_size == GetFileSizeEx_result`.

**Same condition either way.** The STFS calculator is actually slightly MORE
forgiving because its 4KB block rounding provides tolerance for small
differences (e.g., content_size=262144 and written=260000 both map to the
same STFS output). The identity hook would be STRICTER.

---

## Comparison of Fix Approaches

| Approach | Fixes state 16? | Fixes state 17? | Risk | Complexity |
|----------|----------------|-----------------|------|------------|
| **A. Hook sub_82A12720 to identity** | NO | No benefit | Makes state 17 stricter | Trivial |
| **B. Hook sub_8284ADA0 state 16 to copy slot[136] to slot[144]** | YES | N/A | Minimal, targeted | Low |
| **C. Make XamContentCreateEx return ERROR_IO_PENDING** | YES (forces 17) | YES | Requires valid OVERLAPPED | Medium |
| **D. Hook sub_8284A7E8 to set slot[144] = slot[136] at creation** | YES | N/A | Minimal, targeted | Low |
| **E. Hook sub_822417B0 to bypass size check entirely** | YES | YES | Bypasses all validation | Low |

### Recommended: Option C (already identified in doc 30)

Making XamContentCreateEx return ERROR_IO_PENDING forces the state 17
path, which naturally performs the file measurement and STFS calculation.
This is the most correct fix because it makes the recomp follow the same
code path as Xbox 360.

### Fallback: Option B or D

If Option C is too complex, hooking sub_8284ADA0 (Option B) or
sub_8284A7E8 (Option D) to ensure slot[144] is populated before
comparison is a clean targeted fix.

### Not Recommended: Option A (this proposal)

The identity hook for sub_82A12720:
- Does not fix the actual bug (state 16 path)
- Makes state 17 slightly stricter (loses 4KB tolerance)
- Affects call site 1 (device selector size display)
- Provides no benefit in any scenario

---

## Source File Locations

| File | Content |
|------|---------|
| `gta4_recomp.69.cpp:36548` | sub_82A12720 implementation |
| `gta4_recomp.55.cpp:9836` | Call site 1 (sub_8284A0F8) |
| `gta4_recomp.55.cpp:10871` | Call site 2 (sub_8284A7E8, writes slot[136]) |
| `gta4_recomp.55.cpp:11776` | Call site 3 (sub_8284ADA0, writes slot[144]) |
| `gta4_recomp.55.cpp:12186` | Call site 4 (sub_8284B038, writes slot[144]) |
| `LibertyRecomp/kernel/xam.cpp:451` | XamContentCreateEx returns ERROR_SUCCESS |
| `docs/rewrite/30_device_data_size_mismatch.md` | Root cause analysis |
| `docs/rewrite/32_size_comparison_logic.md` | Full slot structure and comparison |
