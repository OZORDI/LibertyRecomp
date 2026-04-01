# Content Creation Flow & Scene Pointer NULL Analysis

## Why 0x831C2458 (scene pointer) is NULL

The scene pointer is written during state 6 of the outer state machine (`sub_82142230`), specifically in the 15-state scene creation sub-machine (`sub_82242910`). The pointer stays NULL because the state machine stalls before scene creation completes.

---

## State Machine Architecture (3 nested levels)

### Level 1: Outer SM (`sub_82142230`) -- 7 states

| State | Function | Status in recomp |
|-|-|-|
| 0 | sub_822414E8 -- sign-in check | Passes (USER_STATE hooks) |
| 1 | sub_8223DDA8 -- storage device selection | Passes |
| 2 | sub_8223DEE8 -- save/load check | Passes |
| 3 | (complex gate) -- content transition | Passes (player slot fields populated by sub_821406C8 hook) |
| 4 | sub_822440F8 -- save device inner SM | **BYPASSED** (returns 2 directly) |
| 5 | sub_822422E0 -- level selection/dispatch | Passes (resets 0x82BF9834 to prevent error 34) |
| 6 | sub_822438B0 -- scene creation inner SM | **REACHES but stalls internally** |

### Level 2: State 6 Inner SM (`sub_822438B0`) -- 8 states

State variable at `0x82BF9838`. Calls `sub_82242910` in its state 2.

### Level 3: Scene Creation SM (`sub_82242910`) -- 15 states

State variable at `0x82BF9848`. This is where the stall occurs.

| State | Key call | Status |
|-|-|-|
| 0 | sub_8223DAA0 (readiness) | Passes -- intercepted to force 0->1 (not 0->4 fast path) |
| 1-3 | Network + content validation | Passes |
| 4 | sub_8223DB20, sub_8223F308 | Passes (platformMode forced to 3) |
| 5-10 | Scene loading + resource activation | Passes |
| 11 | platformMode check | Routes to state 12 (platformMode=3) |
| 12 | **sub_822417B0 (content size check)** | **CURRENT BLOCKER** |
| 13 | Error recovery | Loops back (if state 12 fails) |
| 14 | sub_822417B0 second phase | Blocked (delta < 0 triggers state 13) |

---

## The State 12 Blocker: Content Size Mismatch

### Flow

1. State 12 calls `sub_822417B0(r4=1)` -- initiates content creation
2. This calls `sub_8284A7E8` which calls `XamContentCreateEx(mode=3, CREATE_ALWAYS)`
3. RexGlue's `XamContentCreateEx` returns `X_ERROR_IO_PENDING` (997) when overlapped is non-null
4. `sub_8284A7E8` checks the return: 997 sets slot state=17 (async path, correct)
5. State transitions 12->14
6. State 14 calls `sub_822417B0(r4=0)` -- polls completion via `sub_8284ADA0`
7. `sub_8284ADA0` opens the content file via `CreateFileA`, reads size into `slot[144]`
8. **Compares `slot[136]` (expected size from XamContentGetDeviceData) vs `slot[144]` (actual disk size)**
9. If expected > actual: writes **negative delta** to `0x82BF99C8` -> state 13 (error restart)

### Root Cause

The `sub_822417B0` hook clamps `0x82BF99C8` to >= 0, which prevents the 12->13 error loop. However, the comparison at `sub_8284ADA0` still determines the outcome:

- `slot[136]` = expected content size (from `XamContentGetDeviceData` / device data query)
- `slot[144]` = actual file size on disk (from `GetFileSizeEx`)
- Content slot table: `0x83192C58`, stride 160 bytes

If the content file does not exist or is empty (likely in recomp -- no real Xbox STFS content container), `slot[144]` = 0 while `slot[136]` may be non-zero, producing a negative delta.

The delta clamping fix at line 2241-2242 of imports.cpp masks the symptom. The state machine should advance past 14 after clamping. If it still loops, the return value from `sub_822417B0` may be 1 (still pending) rather than 0 (complete), keeping the machine in a poll loop.

---

## XamContent Hooks Inventory

| Hook | Address | Purpose | Implementation |
|-|-|-|-|
| sub_8284A7E8 | imports.cpp:2204 | Content creation initiator | Pass-through + logging (monitors slot state 16 vs 17) |
| sub_822417B0 | imports.cpp:2234 | Two-phase content size checker | Pass-through + clamps negative delta to 0 |
| sub_82240B08 | imports.cpp:2175 | Content device readiness check | Stub: forces g_sceneReady=1, g_contentReady=1, returns 1 |
| sub_82240B78 | imports.cpp:2164 | Storage device notification guard | Stub: returns 0 (no device change) |
| sub_8223DB20 | imports.cpp:2155 | Sign-in notification guard | Stub: returns 0 (no sign-in change) |
| sub_8224FFC8 | imports.cpp:2188 | XAM dialog result processor | Returns 1 for accept (type=8), 0 for cancel |
| sub_822440F8 | imports.cpp:2065 | State 4 inner SM (save device) | **Full bypass: returns 2** (no-save success) |
| sub_822422E0 | imports.cpp:2122 | State 5 (level selection) | Pass-through + resets 0x82BF9834 from 2->0 |
| sub_82242910 | imports.cpp:2346 | 15-state scene creation SM | Pass-through + fast-path intercept (0->4 forced to 0->1) + platformMode enforcement |

### Save System Hooks (save_hooks.cpp)

| Hook | Purpose |
|-|-|
| sub_829A1C38 | Content creation wrapper (calls XamContentCreateEx) -- logging only |
| sub_829A1CA0 | Content close wrapper -- logging only |
| sub_829A1CB8 | Content enumeration wrapper -- logging only |

---

## Key Diagnostic Addresses

| Address | Type | Meaning |
|-|-|-|
| 0x831C2458 | u32 ptr | **Scene pointer** (NULL = scene not created) |
| 0x82BF9848 | u32 | sub_82242910 state counter (0-14) |
| 0x82BF9838 | u32 | sub_822438B0 inner state |
| 0x82BF9844 | u32 | platformMode (must be 3) |
| 0x82BF99C8 | s32 | Content size delta (negative = deficit = error) |
| 0x82BF3A88 | u32 ptr | Scene object pointer (logged by state 6 inner hook) |
| 0x82A9546C | u32 | Error code (6/10/33/34) |
| 0x82BF9B70 | u32 | XAM readiness (-1 = no dialog pending) |
| 0x83192C58 | table | Content slot table (stride 160, slot[136]=expected size, slot[144]=actual size) |

---

## Recommendations

### 1. Verify sub_822417B0 return value after clamping

The delta clamping prevents the 12->13 error loop, but `sub_822417B0` may still return 1 (pending) if the overlapped I/O completion never signals. Add logging to confirm the return value after the clamp fires.

### 2. Stub sub_8284ADA0 (content completion/verification)

This is the function that performs the size comparison. Stubbing it to always report "sizes match" (write 0 to `0x82BF99C8` and return success) would bypass the content size check entirely. The recomp has no real save storage -- there is no meaningful content to verify.

### 3. Alternative: Bypass state 12 entirely

Since save data is not needed for scene creation, hook `sub_82242910` to skip states 11-14 when `stateBefore == 11` by forcing the state counter directly to the next scene creation state (likely state 6 or the completion state). States 11-14 are the "save overwrite check" sequence which is meaningless on PC.

### 4. Check OVERLAPPED completion signaling

RexGlue's `CompleteOverlappedDeferredEx` spawns a background thread to run the content creation lambda, then signals the OVERLAPPED event. If the event handle in the OVERLAPPED struct is invalid or uninitialized (hEvent fix from commit 09b50a9a), the recompiled code's `WaitForSingleObject` on that handle may hang or return immediately with an error, leaving the slot in state 17 forever. Verify that the hEvent fix in the OVERLAPPED struct is being applied to content creation operations.
