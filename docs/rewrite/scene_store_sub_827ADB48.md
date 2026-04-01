# sub_827ADB48 and the 0x82FF5368 / 0x831C2458 Relationship

## Correction: sub_827ADB48 Is NOT a Scene Constructor

Previous docs (render_frame_loop.md) misidentified sub_827ADB48 as "grcSceneList constructor"
and 0x82FF5368 as "g_scene". This is wrong.

Per engine_init_sub_82478AF8.md (verified against generated code):
- **sub_827ADB48** is an audio/voice block constructor called from sub_82478AF8 (audio engine init, Phase 12)
- **0x82FF5368** = `g_pManager2` (voice block pointer), NOT a scene pointer
- The `0x82FF53xx` region is audio engine globals (0x82FF5360=engine obj, 0x82FF5364=XAudio graph, 0x82FF5368=voice block, 0x82FF536C=viewport array)

## What sub_827ADB48 Actually Does

**File**: `gta4_recomp.50.cpp` line 8045

1. Takes `this` pointer (r3, pre-allocated 1024-byte buffer) + 10 params (resource names/paths)
2. Copies 6 display config blocks via sub_82A00DC0 (memcpy, 16 bytes each)
3. Calls sub_827AD9C8 (base constructor) which calls sub_827AD200 (deep init)
4. Sets vtable to `0x82078D48`
5. Returns constructed object in r3

Caller sub_82478AF8 stores the result: `PPC_STORE_U32(0x82FF5368, r3)`.

## Who Reads 0x82FF5368

All readers are in the audio subsystem:

| Location | Function | What It Does |
|-|-|-|
| gta4_recomp.20.cpp:78862 | sub_82476918 | Reads ptr, calls vtable[0](ptr,1) (destructor), then NULLs 0x82FF5368 |
| gta4_recomp.20.cpp:79453 | sub_82476AF8 | Reads ptr, passes to sub_827ACCB8 (voice mixer dispatch at this+16) |
| gta4_recomp.15.cpp:953 | sub_823B2278 | Reads ptr, checks obj.field_96 bit 0, calls vtable[41] on parent obj |
| gta4_recomp.36.cpp:93315 | sub_8263F078 | Same pattern as sub_823B2278 (field_96 bit 0 guard + vtable[41] call) |
| gta4_recomp.10.cpp:106254 | sub_82300990 | Same pattern as sub_823B2278 (field_96 bit 0 guard + vtable[41] call) |

## 0x831C2458: No Writer Exists in Generated Code

Searched the entire generated codebase for:
- `stw rX, 9304(rY)` where rY = 0x831C0000 base -- **zero matches**
- `PPC_STORE_U32(... + 9304, ...)` -- **zero matches**
- `addi rX, rY, 9304` -- 3 matches, all are READS (sub_828C15C8 render dispatch, sub_825B7C40, sub_82693F80)

The three `addi` sites:
1. **gta4_recomp.58.cpp:115329** (sub_828C15C8): reads `*(0x831C2458)`, if non-null dispatches vtable[16] = scene Render()
2. **gta4_recomp.31.cpp:9657**: base is 0x82030000 (NOT 0x831C0000), false positive -- passes to sub_82845600
3. **gta4_recomp.38.cpp:37646**: base is 0x82690000 (NOT 0x831C0000), false positive -- audio routing table

## 0x82FF5368 and 0x831C2458 Are Unrelated

- **0x82FF5368** (`g_pManager2`): audio voice block pointer, written by sub_82478AF8 after calling sub_827ADB48
- **0x831C2458** (scene list array, offset 9304 from RAGE globals 0x831C0000): render device scene list, read by sub_828C15C8

There is **no copy function** between them. The render_frame_loop.md suggestion to write `*(0x82FF5368)` into `*(0x831C2458)` would copy an audio object pointer into a scene list slot -- this would crash immediately because the vtable layouts are incompatible.

## Where 0x831C2458 Gets Written (On Real Xbox 360)

The address is part of the D3D device global area at 0x831C0000. On the original hardware:

1. `VdInitializeEngines` / `D3DDevice::Create` initializes the full device struct
2. During device creation, the scene list array (at offset 9304) is populated with scene object pointers
3. Scene objects are created separately and registered into the device during init

In the recomp, `VdInitializeEngines` is stubbed (just registers MMIO range). The device creation path that writes to 0x831C0000+9304 was never recompiled because it calls into GPU hardware init routines.

## Root Cause Summary

The scene pointer at 0x831C2458 is NULL because:

1. It is written during D3D device initialization on Xbox 360
2. D3D device init is fully stubbed in the recomp (VdInitializeEngines stub)
3. No generated code in the entire codebase writes to this address
4. The code that writes it is part of the GPU hardware init path, which was excluded from recompilation

## Correct Fix

Do NOT copy 0x82FF5368 into 0x831C2458. Instead:

**Option 1**: Find the actual scene object constructor and registration path in the original binary (not in generated code -- it was excluded). Create a hook that allocates a scene object with vtable 0x82078D48 and writes its pointer into 0x831C2458.

**Option 2**: Write a host-side scene list shim. In the sub_82A467D8 hook (video.cpp), create a minimal scene object that dispatches to the host renderer instead of trying to run the guest scene graph.

**Option 3**: Skip scene list entirely. Intercept draw calls at sub_82A3CC68 (DrawPrimitive) level and translate them to host RenderCommands. The scene list is only needed to drive the guest draw call submission pipeline.
