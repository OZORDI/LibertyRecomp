# D3D Device vs Scene Pointer: Address Analysis

## Question

Is `0x831C2458` (scene list pointer) an offset within the D3D device struct at
`0x106B6080`, or is it a separate global?

## Answer: Separate Global in Guest .bss

`0x831C2458` is a **standalone global pointer** in the RAGE render globals region,
NOT an offset within the device struct.

| Address | Offset from 0x831C0000 | Purpose |
|-|-|-|
| 0x831C22A0 | 8864 | grcDevice secondary ptr |
| 0x831C22A4 | 8868 | grcDevice primary ptr |
| 0x831C22A8 | 8872 | Cleared each frame by render dispatch |
| 0x831C23C8 | 9160 | Scene info struct (read after scene render) |
| 0x831C23E8 | 9192 | Render target array base (r29 in dispatch) |
| 0x831C2458 | 9304 | **Scene list pointer array** |
| 0x831C2460 | 9312 | Frame ready flag |
| 0x831C3DDC | 15836 | Frame timing A |
| 0x831C3DE4 | 15844 | Frame timing B |

The device object lives at guest address `0x106B6080` (host-allocated). The distance
from device to scene ptr is `0x72B0C3D8` (1.9 GB) -- far too large for any struct offset.

## How the Address is Computed (Generated Code)

In `sub_828C15C8` (render dispatch), file `gta4_recomp.58.cpp` line 115325:

```
lis  r11, -31972       ; r11 = 0x831C0000
addi r11, r11, 9304    ; r11 = 0x831C2458
...
lwz  r3, 0(r11)        ; r3 = *(0x831C2458) = scene list ptr
cmplwi cr6, r3, 0      ; null check
beq  skip              ; skip if null (current behavior -- always taken)
lwz  r11, 0(r3)        ; vtable
lwz  r11, 64(r11)      ; vtable[16] = Render()
bctrl                  ; call grcSceneList::Render()
```

## Who Reads 0x831C2458

Only one reader in the entire generated codebase:

- **sub_828C15C8** (`gta4_recomp.58.cpp:115329`) -- render dispatch

The diagnostics in `imports.cpp:2032` and `video.cpp:9402` also read it for logging.

## Who Should Write 0x831C2458

**Nobody in the generated codebase writes to offset 9304.** Zero `stw` instructions
target this address in any generated file. This is the root cause.

### Scene Object Creation (Does Happen)

`sub_82478AF8` (`gta4_recomp.20.cpp:84737`) calls `sub_827ADB48` (grcSceneList
constructor) and stores the result at `0x82FF5368` (g_scene global):

```
bl   sub_827ADB48       ; construct grcSceneList
lis  r11, -32001        ; r11 = 0x82FF0000
addi r31, r11, 21352    ; r31 = 0x82FF5368
stw  r3, 0(r31)         ; g_scene = constructed object
```

### Scene Registration (Does NOT Happen)

The missing step is copying the scene object pointer from `0x82FF5368` into the
device globals slot at `0x831C2458`. This registration would have occurred during
D3D device initialization (sub_82A50890), which is effectively stubbed because
`VdInitializeEngines` and other GPU imports are no-ops.

The device init function (sub_82A50890) uses **indirect addressing** through a
pointer table in .rdata (0x8200078C) to store the device pointer. A similar
mechanism would register scene list pointers. Since the GPU init path is stubbed,
this registration never executes.

## The Fix

Write a hook that bridges the gap:

```cpp
// After scene creation (sub_82478AF8) returns, or periodically:
uint32_t g_scene = PPC_LOAD_U32(0x82FF5368);
if (g_scene != 0 && PPC_LOAD_U32(0x831C2458) == 0) {
    PPC_STORE_U32(0x831C2458, g_scene);
}
```

This alone is necessary but NOT sufficient for rendering -- the scene objects also
need valid render targets, shaders, and vertex/index buffers. But it unblocks the
render dispatch path so `sub_828C15C8` enters the scene render codepath instead of
skipping it.

## Related Globals

| Address | Name | Relationship |
|-|-|-|
| 0x82FF5368 | g_scene | Created scene object (source -- has valid ptr) |
| 0x831C2458 | scene list slot | Destination (always NULL -- never written) |
| 0x82B0B48C | render gate | Must be > 0 to enter render path (already set) |
| 0x8200078C | .rdata ptr table | Device init loads indirection ptr from here |
