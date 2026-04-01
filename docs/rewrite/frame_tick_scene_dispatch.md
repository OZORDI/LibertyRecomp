# Frame Tick Scene Dispatch Analysis

## Summary

The scene pointer at 0x831C2458 is NULL because **two** independent gates prevent scene rendering:

1. **Render gate at 0x82B0B48C** is initialized to -1 (signed) and never incremented
2. **Scene registration** into the device global array at 0x831C2458 never occurs

The RENDER-GATE log line reads the gate from the **wrong address** (0x8290B48C instead of 0x82B0B48C), producing misleading output.

---

## sub_821428C8 (per-frame tick)

**File**: `gta4_recomp.0.cpp` line 6281

Executes 17 subsystems sequentially. Does **not** directly read 0x831C2458 or dispatch scene rendering. The scene dispatch happens deeper in the call chain via:

```
sub_821428C8
  -> sub_8214C8C8 (main game update, #2)
       -> ... -> sub_828C5BA0 (frame timing wrapper)
            -> sub_828C15C8 (RAGE render dispatch)
```

sub_821428C8 itself only orchestrates subsystem calls. It passes a world manager pointer (loaded from globals) to sub_82205850 for scene updates.

## sub_82142F90 (main frame update variant)

**File**: `gta4_recomp.0.cpp` line 7313

Alternative frame update path with ~30 subsystem calls. Includes:
- sub_8222D660 / sub_8222D680 (streaming lock/unlock)
- sub_8214C8C8 (game update)
- sub_82205850 (world/scene update)
- sub_8221B238 (world manager)
- sub_826CDEB8 (save/profile)
- sub_822021E8, sub_822446E0, sub_8233CA00 (init subsystems)
- sub_822BCA90 (network tick, called 4 times)

Does not directly read scene pointer either; the scene dispatch is always via sub_828C15C8.

## sub_828C15C8 (render dispatch) — scene pointer usage

**File**: `gta4_recomp.58.cpp` line 115160

The scene dispatch sequence (lines 115325-115368):

```
r11 = lis -31972               // r11 = 0x831C0000
r11 = r11 + 9304               // r11 = 0x831C2458 (scene list ptr addr)

r10 = *(0x82B0B48C)            // load render gate (signed)
cmpwi cr6, r10, 0              // signed compare
ble cr6, skip_scene            // if gate <= 0, skip scene dispatch

r3 = *(r11)                    // r3 = *(0x831C2458) = scene list ptr
cmplwi cr6, r3, 0
beq cr6, skip_vtable           // if NULL, skip vtable call

r11 = *(r3)                    // vtable
r11 = *(r11 + 64)              // vtable[16] = Render()
call r11(r3)                   // grcSceneList::Render()
```

After the vtable call, it also reads 0x831C23C8 (RT index) and 0x831C23E8 (RT array base) to determine the correct render target.

## Render gate (0x82B0B48C)

**Initialized** to -1 (0xFFFFFFFF) by sub_828C1228 (line 115144-115146 in gta4_recomp.58.cpp). This is the **only** write to this address in the entire generated codebase.

Since `cmpwi` is a **signed** compare, -1 < 0, so `ble` is taken every frame. The scene dispatch path is **permanently gated off**.

On the original Xbox 360, a D3D device creation function (part of the stubbed GPU init) would set this gate to a positive value (likely the number of scene lists registered). In the recomp, VdInitializeEngines only registers the GPU MMIO range and does not touch this global.

### The 0x7FE80578 value is from the wrong address

The RENDER-GATE diagnostic in `video.cpp` line 9401:
```cpp
uint32_t gateVal = PPC_LOAD_U32(0x8290B48C);  // BUG: should be 0x82B0B48C
```

The address `0x8290B48C` is 2 MB below the correct render gate. The value 0x7FE80578 is unrelated data from that wrong address. Reading from `0x82B0B48C` would show -1 (0xFFFFFFFF).

## Scene pointer registration gap

The scene object is created by sub_827ADB48 (grcSceneList constructor, gta4_recomp.20.cpp line 84737 area) and stored at global 0x82FF5368. But it is never copied into the device scene list array at 0x831C2458.

The registration would normally happen during D3D device initialization:
1. Device creates render targets (populates 0x831C23E8)
2. Device creates scene lists and stores pointers at 0x831C2458+
3. Device sets render gate (0x82B0B48C) to scene count (> 0)

All three steps are part of the stubbed GPU subsystem.

## vtable dispatch details

When the scene dispatch does fire (gate > 0 AND scene ptr != NULL):

| Step | Code | Effect |
|-|-|-|
| Load scene ptr | `r3 = *(0x831C2458)` | Get grcSceneList* |
| Load vtable | `r11 = *(r3)` | Get vtable base |
| Load vfunc | `r11 = *(r11 + 64)` | vtable[16] = Render |
| Call | `call r11(r3)` | grcSceneList::Render(this) |

The grcSceneList vtable at 0x82078D48 has Render() at entry 16.

## What must happen to get scene rendering

Two fixes are needed:

1. **Set render gate positive**: `PPC_STORE_U32(0x82B0B48C, 1)` after GPU init
2. **Register scene pointer**: Copy `*(0x82FF5368)` to `*(0x831C2458)` after scene creation

These are necessary but not sufficient — the scene objects also need valid render targets, shaders, and vertex buffers to produce actual geometry. But without these two fixes, the render dispatch in sub_828C15C8 unconditionally skips the scene path.

## Diagnostic fix

In `video.cpp` line 9401, change `0x8290B48C` to `0x82B0B48C`:
```cpp
uint32_t gateVal = PPC_LOAD_U32(0x82B0B48C);  // was 0x8290B48C (wrong)
```
