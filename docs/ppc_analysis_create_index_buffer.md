# PPC Analysis: CreateIndexBuffer at the RAGE Wrapper Layer

## Summary

CreateIndexBuffer at the **RAGE wrapper layer** is `sub_8253B640` (0x8253xxxx range), NOT in the 0x8254xxxx range.
The hook already exists in `video.cpp` inside the disabled Sonic 06 block at line ~9491.

## Layer Map

| Layer | Address | Function | Status |
|-|-|-|-|
| D3D device layer | `sub_82A44970` | CreateVertexBuffer | ACTIVE hook (GTAIV_CreateVertexBuffer) |
| D3D device layer | no separate fn | CreateIndexBuffer | NOT a separate D3D function — see below |
| RAGE wrapper layer | `sub_8253B508` | CreateVertexBuffer | hook in disabled Sonic 06 block |
| RAGE wrapper layer | `sub_8253B640` | CreateIndexBuffer | hook in disabled Sonic 06 block |

## Init Table: 0x82543AC8–0x82544000 Range

The gta4_init.cpp entries between SetIndices (0x82543AC8) and 0x82544050:

```
{ 0x82543C20, sub_82543C20 },   // render state update (copies buffer descriptors via memcpy)
{ 0x82543CB8, sub_82543CB8 },   // draw state commit (copies vertex stream to cmd buffer, VB stride/offset packing)
{ 0x82544050, sub_82544050 },
{ 0x825440E8, sub_825440E8 },
{ 0x825443E8, sub_825443E8 },
{ 0x825444F0, sub_825444F0 },   // SetRenderTarget (hooked)
```

**No CreateIndexBuffer function exists in the 0x82543xxx–0x82545xxx range.**

## Why the 0x82543AC8–0x82543C20 Gap Contains No CreateIndexBuffer

`sub_82543AC8` (SetIndices) and the surrounding range 0x82542F80–0x82543C1F are all contained
within the single large function `sub_82542F80` in gta4_recomp.26.cpp. This is a RAGE
rendering dispatch function. SetStreamSource (0x82543918) and SetIndices (0x82543AC8) are
entry points inside it that are individually hooked, but no CreateIndexBuffer lives here.

## D3D Layer: No Separate CreateIndexBuffer Function

At the D3D layer (0x82A4xxxx), CreateVertexBuffer is `sub_82A44970` (allocates 48 bytes via
`sub_821B3608`, then calls `sub_82A445B0` to set up the buffer descriptor). There is NO
corresponding `sub_82A44xyzCreateIndexBuffer` — the init table shows:

```
0x82A44850  CreateTexture
0x82A44970  CreateVertexBuffer
0x82A44A98  GetSurfaceDesc
0x82A44B30  (12-byte fn — sets type=16 marker on an existing buffer, NOT an allocator)
0x82A44B78  (larger fn — vertex format/stream binding)
0x82A44CF8  DrawPrimitive (calls sub_82A499B8 ring-buffer flush)
```

`sub_82A44B30` writes `stw r11,4(r31)` with value 16 — this is a post-creation type stamp, not
an allocation. CreateIndexBuffer allocation at the D3D level is handled through the same
`sub_82A445B0` path called from the RAGE-wrapper `sub_8253B640`.

## RAGE Wrapper Layer: sub_8253B640 IS the CreateIndexBuffer

`sub_8253B640` lives in the 0x8253xxxx range between:
- `sub_8253B5D0` = LockVertexBuffer (hooked)
- `sub_8253B630` = UnlockVertexBuffer (hooked)
- `sub_8253B640` = **CreateIndexBuffer** (hooked)
- `sub_8253B6F0` = LockIndexBuffer (hooked)
- `sub_8253B750` = UnlockIndexBuffer (hooked)

Init table gap from 0x8253AB20 to 0x8253BC70 covers all these — none are recompiled, all hooked.

## Shared Parameter Check: CreateVertexBuffer vs CreateIndexBuffer

CreateVertexBuffer signature: `CreateVertexBuffer(uint32_t length)`
CreateIndexBuffer signature: `CreateIndexBuffer(uint32_t length, uint32_t /*unused*/, uint32_t format)`

They are **not** a shared function with a type flag. They are separate functions with different
parameter counts. The index buffer version takes a `format` parameter (passed to `ConvertFormat()`).

## Current Hook Status in video.cpp

The `sub_8253B640` hook is **DISABLED** — it is inside the `#if 0 // Disabled Sonic 06 hooks` block
at line ~9491 of `video.cpp`. The Sonic 06 addresses are the same as GTA IV's RAGE wrapper
layer addresses (both use the RAGE engine), so this hook is valid for GTA IV.

The `CreateIndexBuffer` handler function itself (the C++ implementation) is already written and
correct (lines 4015–4028 of video.cpp).

## Action Required

To enable CreateIndexBuffer at the RAGE wrapper layer, move this line:
```cpp
GUEST_FUNCTION_HOOK(sub_8253B640, CreateIndexBuffer);
```
out of the `#if 0` block and into the active GTA IV hooks section (alongside
`GUEST_FUNCTION_HOOK(sub_8253B508, CreateVertexBuffer)`).

Also consider: the companion buffer operations in the same block may be valid GTA IV addresses:
- `sub_8253B6F0` = LockIndexBuffer
- `sub_8253B750` = UnlockIndexBuffer

## Files Referenced

- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/gpu/video.cpp` — hooks and C++ implementations
- `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/gta4_init.cpp` — function init table
- `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.26.cpp` — RAGE wrapper layer PPC code (0x8253xxxx–0x82545xxx)
- `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.71.cpp` — D3D device layer PPC code (0x82A4xxxx)
