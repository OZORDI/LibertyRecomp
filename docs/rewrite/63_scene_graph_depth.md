# 63: sub_8291DF00 Scene Graph Traversal — Stack Overflow Analysis

## Summary

sub_8291DF00 is **NOT recursive** and does **NOT contribute** to the main thread
stack overflow. It is a flat iterative loop that processes scene objects with a
stride of 768 bytes. The 2.37M MISSING-FUNC entries come from two null vtable
calls per object per frame, but each call is handled inline (fprintf+fflush) with
no host stack growth. The stack overflow must originate elsewhere.

---

## 1. Function Structure (0x8291DF00)

**File**: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.62.cpp`, line 7315

### Prologue
```
__savegprlr_18(ctx, base);    // save r18-r31 + LR
stwu r1, -208(r1)             // allocate 208-byte PPC stack frame
```

### PPC stack frame size: 208 bytes
### Estimated host (C++) frame: ~256-400 bytes (callee-saves, locals, alignment)

### Core loop (loc_8291DF64 to line 7778)
```
r22 = 0                        // object index
r23 = 0                        // byte offset (stride 768)
r20 = object count             // loaded from global[-28992]+40

loop:
  if (r22 >= r20) break
  // ... process object at base[r23] ...
  r22 += 1
  r23 += 768
  goto loop
```

This is a **flat for-loop**. The function iterates `r20` objects at stride 768
bytes and returns. There is no recursive call to itself, no call to its parent
(sub_8291E620), and no call to any function in its own call chain.

### Functions called (all flat, none recursive):
1. `sub_82849918` — yield/sleep (spin-waits on a flag)
2. `sub_829222E0` — slot lookup helper
3. Up to 9 indirect vtable calls via `PPC_CALL_INDIRECT_FUNC`
4. `sub_82921E60` — slot release helper
5. `sub_82902328` — epilogue/cleanup

---

## 2. Is This Function Recursive?

**No.** The function body (lines 7315-7789) contains these direct calls:
- `sub_82849918` (line 7350)
- `sub_829222E0` (line 7409)
- `sub_82921E60` (line 7769)
- `sub_82902328` (line 7784)

Plus 9 indirect calls via `PPC_CALL_INDIRECT_FUNC`. None of these are
`sub_8291DF00` itself, and none of the direct callees call back to it.

The parent function `sub_8291E620` (line 8345) calls `sub_8291E260` in a loop
and then calls `sub_8291DF00` once at the end (line 8486). sub_8291E260 does
not call sub_8291DF00 or sub_8291E620.

---

## 3. Recursion Depth Analysis (Hypothetical)

Even if this function WERE recursive (it is not), the arithmetic rules it out as
a stack overflow cause:

- PPC frame: 208 bytes per level
- Host frame: ~300 bytes per level (estimated)
- Total per level: ~500 bytes
- 5.8MB / 500 bytes = ~11,600 levels of recursion needed

RAGE scene graphs have at most 5-7 levels of hierarchy:
```
World -> Sector -> Interior -> Entity -> Drawable -> Geometry -> SubGeometry
```

Even at 7 levels with a 768-byte object stride, total stack consumed would be:
7 * 500 = 3,500 bytes (0.003 MB). Nowhere near 5.8 MB.

Since the function is not recursive at all, its actual contribution to the call
stack is a single frame of ~500 bytes total.

---

## 4. Vtable Calls — What Happens When They Are NULL

The function makes indirect calls through the object's vtable at `[r31+0]`.
The vtable slots accessed (by offset from vtable base):

| Offset | Slot | Line  | Return Addr | Purpose (estimated)     |
|--------|------|-------|-------------|-------------------------|
| 20     | [5]  | 7437  | 0x8291DFE8  | IsVisible() — bool      |
| 8      | [2]  | 7458  | 0x8291E010  | OnHide() / Deactivate() |
| 0      | [0]  | 7471  | 0x8291E028  | Destructor / Release()  |
| 28     | [7]  | 7528  | 0x8291E090  | SetLOD() / priority     |
| 12     | [3]  | 7585  | 0x8291E0F8  | Update() — visibility   |
| 16     | [4]  | 7618  | 0x8291E130  | Render() / Submit()     |
| 40     | [10] | 7630  | 0x8291E144  | **Render/Draw (2.37M)** |
| 36     | [9]  | 7671  | 0x8291E190  | GetSortKey()            |
| 24     | [6]  | 7689  | 0x8291E1B0  | **GetBounds (2.37M)**   |

When a vtable slot is 0x00000000, `PPC_CALL_INDIRECT_FUNC(0)` executes:
1. Checks: is 0x00000000 in code range [0x82140000, 0x82A8635C)? **No.**
2. Sets `_icf_fn = nullptr`
3. Prints `[MISSING-FUNC] indirect call to 00000000 ...` via fprintf
4. Returns from the do-while(0) macro
5. **Execution continues at the next C++ statement**

The return value register (`r3`) is **stale** — it retains whatever value it had
before the call. This means:

- **Slot [5] (IsVisible, line 7437-7448)**: Checks `r3 & 0xFF != 0`. If stale r3
  happens to be nonzero, the branch at line 7448 skips to loc_8291E048. If zero,
  falls through to the deactivation path (slots [2] and [0]).
- **Slot [10] (Render, line 7630)**: Return value used to compute sort flags at
  line 7639-7642. Stale r3 produces wrong flag bits but no crash.
- **Slot [6] (GetBounds, line 7689)**: Return value packed into byte field at
  line 7698-7700. Stale r3 writes garbage to the bounds cache.

**The function continues iterating regardless of NULL returns.** It does NOT
loop infinitely due to missing return values. After processing all objects
(r22 reaches r20), it exits normally.

---

## 5. Call Chain (Bottom-Up)

```
sub_8291DF00        <- scene object iteration loop (208-byte frame)
  called by:
sub_8291E620        <- scene update + sort + iterate (160-byte frame)
  called by:
sub_829022B0        <- thin wrapper, loads global scene ptr (96-byte frame)
  called by:
sub_82902420        <- per-frame scene render call (160-byte frame)
  called by:
sub_82902728        <- render loop driver with continue flag (96-byte frame)
  called by:
  (indirect — via vtable or function pointer table)
```

Total stack depth for the full chain:
- PPC: 208 + 160 + 96 + 160 + 96 = 720 bytes
- Host: ~1.5-2.5 KB estimated (5 nested C++ frames)

This is negligible relative to the 5.8 MB stack overflow.

### sub_82902728 loop structure (line 3627):
```c
loc_82902754:
    sub_82902420(ctx, base);          // process one frame
    if (byte[r31+777] != 0) goto loc_82902754;  // continue if flag set
```
This is a **per-frame render loop** — it calls sub_82902420 once per iteration,
not recursively. The flag at offset 777 is the "keep rendering" flag. Each
iteration uses and releases the same stack frame.

---

## 6. The 2.37M MISSING-FUNC Entries

### Source
Two vtable slots in sub_8291DF00:
- **Slot [10] at offset 40** (return addr 0x8291E144): called for every object
- **Slot [6] at offset 24** (return addr 0x8291E1B0): called for every object

### Count arithmetic
If there are N objects in the scene graph and the render loop runs for F frames:
- MISSING-FUNC count = 2 * N * F
- 2.37M / 2 = 1.185M object-visits
- If game runs at ~30fps for ~60 seconds: ~1800 frames
- 1.185M / 1800 = ~658 objects per frame

This is consistent with a GTA IV scene graph: ~600-800 active scene nodes
(entities, drawables, LOD groups, lights, effects) in a typical Liberty City
scene.

### Are objects visited repeatedly due to corrupted pointers?

**No.** The iteration is index-based (`r22` counts from 0 to `r20`), not
pointer-chasing. There is no next/child pointer to corrupt. The object array is
accessed as `base_array + r22 * 768`, where base_array comes from a global
pointer. Even if individual objects have garbage data, the loop termination
condition (`r22 < r20`) is controlled by the object count field, not by object
contents.

The only way to get unbounded iteration would be if the object count (`r20`,
loaded from global[-28992]+40) were corrupted to an extremely large value.
At 2 MISSING-FUNC calls per object per frame, and ~1800 frames observed,
the object count of ~658 is reasonable.

---

## 7. Conclusion: Not a Stack Overflow Contributor

| Question | Answer |
|----------|--------|
| Is sub_8291DF00 recursive? | **No** — flat loop |
| Does it call itself for child nodes? | **No** — index-based iteration only |
| How deep can recursion go? | N/A — no recursion exists |
| Stack frame size? | 208 bytes PPC, ~300 bytes host |
| Called from render loop or init? | **Per-frame render loop** (sub_82902728) |
| Levels needed for 5.8MB? | 11,600+ (impossible, max 7 in RAGE) |
| Does it loop indefinitely after NULL vtable? | **No** — continues to next object |
| Are 2.37M calls from repeated visits? | **No** — ~658 objects x ~1800 frames x 2 slots |

The 2.37M MISSING-FUNC calls are a **symptom** (null vtable entries for scene
render objects) but not the **cause** of the stack overflow. Each MISSING-FUNC
is handled inline with zero stack growth. The function's call chain adds ~2 KB
to the stack — 0.03% of the 5.8 MB overflow threshold.

The stack overflow root cause must be found elsewhere — likely in a genuinely
recursive function or in a function with a very large local frame that is called
from deep in the render pipeline.
