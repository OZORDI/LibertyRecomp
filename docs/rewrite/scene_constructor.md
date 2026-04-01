# Scene Constructor Analysis: Why 0x831C2458 Is NULL

## Key Correction

Previous docs (render_frame_loop.md) incorrectly identified sub_827ADB48 as "grcSceneList constructor" and 0x82FF5368 as "g_scene". Per streaming_manager_0x82B07278.md and scene_store_sub_827ADB48.md:

- **sub_827ADB48** = CFileManagerHashed constructor (streaming manager), vtable 0x82078D48
- **0x82FF5368** = `g_streamingModule` (CFileManagerHashed instance ptr), NOT a scene ptr
- **0x831CC944** = where sub_82478AF8 actually stores its object (r9 = 0x831D0000, offset -0x36BC)
- **0x82FF5368 and 0x831C2458 are unrelated** -- copying one to the other would crash

## Definitive Search Results

### Zero Writers Exist in Generated Code

Exhaustive search of all ~72 generated .cpp files confirms:

| Search | Matches | Result |
|-|-|-|
| `addi rX,rY,9304` (all files) | 3 | ALL are reads, none target 0x831C0000 base |
| `stw rX,9304(rY)` (all files) | 0 | No stores at offset 9304 from any register |
| `PPC_STORE_U32(...+9304,...)` | 0 | No generated store expression |
| `stwx` to computed 0x831C2458 | 0 | No indexed stores either |

The 3 `addi` hits with offset 9304:

1. **gta4_recomp.58.cpp:115329** (sub_828C15C8) -- READS scene ptr, dispatches vtable[16] if non-null
2. **gta4_recomp.31.cpp:9657** -- base is 0x82030000 (false positive), passes to sub_82845600
3. **gta4_recomp.38.cpp:37646** -- base is 0x82690000 (false positive), audio routing table

### sub_82142F90 (Main Frame Update)

Does NOT call scene creation. It calls:

| Call | Function | Purpose |
|-|-|-|
| 1 | sub_8222D660 | World dispatch begin |
| 2 | sub_822BCA90 | Network tick (x2) |
| 3 | sub_8214C8C8 | Main game update (58 callees) |
| 4 | sub_82205850 | World/scene update |
| 5 | sub_8221B238 | Resource streaming |
| 6 | sub_82142B88 | Subsystem dispatcher |
| 7 | sub_826CDEB8 | Save/profile system |
| 8+ | Multiple | Streaming, memory, profiling |

None of these write to 0x831C2458.

### sub_82142230 States 4-6

| State | Function Called | Purpose | Writes to 0x831C2458? |
|-|-|-|-|
| 4 | sub_822440F8 | Save/content SM (7 sub-states); hooked to return 2 (bypass) | No |
| 5 | sub_822422E0 | Episode index setup; resets 0x82BF9834 | No |
| 6 | sub_822438B0 | Inner SM calling sub_82242910 (15-state scene creation) | No |

### sub_82242910 (15-State Scene Creation SM)

State machine at 0x82BF9848, analyzed all 15 states:

| State | Functions Called | Purpose |
|-|-|-|
| 0 | sub_8223DAA0 | XAM readiness check |
| 1-3 | sub_8223DAA0, sub_822414E8, sub_82241428 | Device enumeration, error code 6 setup |
| 4 | sub_8223F308 | Scene file parse (platform mode gated, needs 3 or 4) |
| 5-7 | sub_822422E0, sub_8223DDA8 | Episode/save state setup |
| 8 | sub_8284AB10, sub_8284AB70, sub_8284B490, sub_8284B430 | Scene object creation |
| 9 | sub_82240B08, content readiness check | Content device check |
| 10-11 | sub_8284AB10, sub_8284B490, sub_8284B430 | Scene refinement |
| 12 | sub_822417B0 | Content size comparison (CURRENT BLOCKER) |
| 13 | sub_8223DB20, sub_82240B78, sub_8223F9F0 | Sign-in/storage guards |
| 14 | sub_822417B0 | Content size comparison (second pass) |

**None of these 15 states write to 0x831C2458.** The scene objects are created and stored in the streaming data array at 0x831B2C58 (via sub_8284B490/sub_8284B4D0), but registration into the render device's scene list array at offset 9304 from 0x831C0000 never happens.

## What Reads 0x831C2458

Only **sub_828C15C8** (render dispatch, gta4_recomp.58.cpp:115160):

```
r11 = 0x831C0000 + 9304     // = 0x831C2458
r3  = *(r11 + 0)            // scene list ptr
if (r3 != 0):
    r11 = *(r3)              // vtable
    r11 = *(r11 + 64)        // vtable[16] = Render()
    call r11                 // dispatch
```

The gate check at 0x82B0B48C (lis -32079, offset -19316) must be > 0 to enter this path. It IS > 0, but the scene ptr is null so nothing renders.

## Where the Write SHOULD Come From

On Xbox 360, 0x831C2458 is written during D3D device initialization:

1. `VdInitializeEngines` triggers GPU subsystem init
2. `D3DDevice::Create` allocates and fills the device struct at 0x831C0000
3. During device creation, scene list slots (offset 9304+) are populated
4. Scene objects are created separately and registered into the device

In the recomp, `VdInitializeEngines` is stubbed (just registers MMIO range at 0x7FC80000). The entire D3D device creation path was excluded from recompilation because it directly interfaces with GPU hardware.

## The Scene Object Type at 0x831C2458

This is NOT a CFileManagerHashed or audio object. The render dispatch calls `vtable[16]` (offset 64), which is a `Render()` method. This is a **grcSceneGraph** or **grcRenderTarget** object created during D3D device init.

The vtable at offset 64 is likely from one of these classes (not yet identified):
- grcRenderTargetList
- grcSceneGraph
- A D3D-specific render pipeline object

## Fix Options

### Option A: Manual Registration Hook

After the game reaches a stable state (sub_82242910 completes, or early in sub_82142F90):

```cpp
// Create a minimal shim object with a Render() vtable entry
// that dispatches to host renderer
uint32_t shimObj = allocate_guest_memory(128);
uint32_t shimVtable = allocate_guest_memory(68); // 17 entries
PPC_STORE_U32(shimVtable + 64, host_render_dispatch_addr);
PPC_STORE_U32(shimObj, shimVtable);
PPC_STORE_U32(0x831C2458, shimObj);
```

### Option B: Skip Scene List (Recommended)

Intercept draw calls at sub_82A3CC68 (DrawPrimitive) and translate to host RenderCommands. The scene list only drives guest draw call submission. The host renderer (Metal/Vulkan/D3D12) already has the infrastructure.

### Option C: Trace the Excluded Code

The D3D device init path was excluded from recompilation. Use the IDA pseudocode to find the exact function that writes to 0x831C0000+9304 and implement it as a native hook.

## Current Blocker Chain

```
sub_82242910 state 12 (content size via sub_822417B0)
  -> blocks state 13-14
  -> blocks scene creation completion
  -> BUT even if it completes, scene creation SM does NOT write 0x831C2458
  -> the write comes from D3D device init (stubbed)
  -> 0x831C2458 will remain NULL regardless of state machine progress
```

The state machine blocker at state 12 is a red herring for the scene pointer issue. Even if the SM completes all 15 states, 0x831C2458 stays NULL because the writer is in the excluded D3D device init path.
