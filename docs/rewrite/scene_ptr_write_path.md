# Scene Pointer Write Path: 0x831C2458

## Summary

The scene pointer at `0x831C2458` is **never written by any function in the game binary**. It is populated exclusively by the Xbox 360 D3D runtime kernel during device creation. In the recompilation, where the D3D kernel is absent, this address must be written manually by a hook.

---

## Evidence

### Binary-Level Exhaustive Search

1. **No `stw` with displacement 0x2458**: Zero `stw rX, 0x2458(rY)` instructions exist in the entire 18MB binary.

2. **Only one `addi` computing 0x831C2458**: Searched all `lis -31972` (0x831C0000) + `addi 9304` (0x2458) patterns. Found exactly **3 `addi` with immediate 9304** across all generated code, but only **one** uses base 0x831C0000:
   - `gta4_recomp.58.cpp:115329` in `sub_828C15C8` (render dispatch) -- this is a **READ** (`lwz r3, 0(r11)`), not a write
   - The other two use bases 0x82790000 and 0x82030000 (different data areas)

3. **No `stw` at offset 0 following the computed address**: After computing `r11 = 0x831C2458`, the code only loads (`lwz`), never stores.

4. **No vtable/function-pointer references to sub_828BF3C8**: The adjacent field `0x831C2460` (frame ready flag, offset 9312) is written by `sub_828BF3C8`, but this function has **zero callers and zero vtable references** in the binary. It too is intended to be called by the D3D kernel.

5. **Address is in the .data segment**: `0x831C2458` falls within the binary file (file offset 0x11C2458), with initial value `0xD3741CA7` (random/compressed XEX data, zeroed at load time by the runtime).

### Generated Code Analysis

| File | Line | Instruction | Address | Type |
|-|-|-|-|-|
| gta4_recomp.58.cpp | 115329 | `addi r11,r11,9304` | 0x831C2458 | Compute |
| gta4_recomp.58.cpp | 115353 | `lwz r3,0(r11)` | 0x831C2458 | READ |
| gta4_recomp.58.cpp | 115355-115357 | null check + skip | -- | Gate |

No `PPC_STORE` to this address exists anywhere in the generated code.

---

## The D3D Kernel Path

On Xbox 360, scene list registration happens inside the D3D runtime kernel (`d3d.xex`):

```
Game Code                           D3D Kernel
---------                           ----------
sub_828C0B48 (grcDevice init)
  -> sub_82A416B8 (D3D setup)
       -> sub_82A50890 (CreateDevice)
            -> sub_82A507A8
            -> sub_82A49D08 (ring buffer)     ---> XDK D3D kernel
            -> sub_82A503C8 (render targets)        populates device
            -> sub_82A42020                         globals including
            -> sub_82A50160                         *(0x831C2458)
```

The device object is 22400 bytes, allocated by `sub_82A412D0`. After CreateDevice completes, the D3D kernel writes the scene list pointer, frame ready flag, and other fields into the static globals at `0x831C0000`. The game binary only ever READS these fields.

---

## Scene Object Lifecycle

| Step | Function | Address | What Happens |
|-|-|-|-|
| 1 | sub_827ADB48 | -- | grcSceneList constructor creates scene object |
| 2 | sub_82478AF8 | 0x82FF5368 | Scene object pointer stored at `g_scene` |
| 3 | (D3D kernel) | 0x831C2458 | Scene pointer registered into device globals |
| 4 | sub_828C15C8 | 0x831C2458 | Render dispatch reads scene pointer |

Step 3 never executes in the recompilation because the D3D kernel is absent.

---

## Required Fix

Write the scene pointer into the device globals after scene creation completes. The simplest approach is a post-init hook:

```cpp
// After sub_82242910 (scene creation SM) returns 0 (success),
// or after sub_82478AF8 (audio/scene init) completes:
uint32_t sceneObj = PPC_LOAD_U32(0x82FF5368);  // g_scene
if (sceneObj != 0) {
    PPC_STORE_U32(0x831C2458, sceneObj);        // register into device globals
}
```

### Placement Options

| Where | Hook target | Condition |
|-|-|-|
| After scene creation SM | sub_82242910 returns 0 | Scene SM must complete states 0-14 |
| After audio init | sub_82478AF8 exit | Scene object created at call 55 |
| Post-init activation | sub_82142230 state >6 | All init systems completed |
| Manual force | Any frame tick | Check g_scene non-null, write once |

### Prerequisite

The scene object at `0x82FF5368` must be valid (non-null) before writing. This requires:
- sub_827ADB48 (grcSceneList constructor) to have run successfully
- The allocated object to have a valid vtable at `0x82078D48`
- vtable slot 16 (offset 64) to point to the Render() virtual function

### Limitations

Writing the scene pointer alone will not produce rendered geometry. The scene object also needs:
1. Valid render targets (RT array at 0x831C23E8)
2. Shader bindings (grcEffect instances with non-null vtables)
3. Vertex/index buffers with host-side counterparts
4. Draw call translation in sub_82A3CC68

The scene pointer write enables the render dispatch path to execute, but actual visible output requires the full GPU rewrite pipeline.

---

## Address Map

| Address | Offset | Name | Written By |
|-|-|-|-|
| 0x831C0000 | 0 | RAGE global data base | (static) |
| 0x831C22A4 | 8868 | grcDevice primary ptr | sub_828C01E0 (game code) |
| 0x831C22A8 | 8872 | grcDevice secondary ptr | sub_828BF898 (game code) |
| 0x831C2458 | 9304 | Scene list pointer | **D3D kernel only** |
| 0x831C2460 | 9312 | Frame ready flag | **D3D kernel only** (sub_828BF3C8 exists but never called) |
| 0x82FF5368 | -- | g_scene (created object) | sub_82478AF8 (game code) |
