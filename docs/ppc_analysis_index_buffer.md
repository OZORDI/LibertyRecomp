# PPC Analysis: Index Buffer Functions (CreateIndexBuffer / Lock / Unlock)

**Date:** 2026-03-28
**Scope:** GTA IV Xbox 360 XEX v8 — CreateIndexBuffer, LockIndexBuffer, UnlockIndexBuffer address verification

---

## Final Addresses

| Function | Address | Layer | Current status |
|-|-|-|-|
| CreateIndexBuffer | `0x8253B640` | RAGE wrapper | In `#if 0` Sonic 06 block — needs move to GTA IV section |
| LockIndexBuffer | `0x8253B6F0` | RAGE wrapper | In `#if 0` Sonic 06 block — needs move to GTA IV section |
| UnlockIndexBuffer | `0x8253B750` | RAGE wrapper | In `#if 0` Sonic 06 block — needs move to GTA IV section |

These addresses are NOT Sonic 06-specific. They are valid GTA IV addresses in the RAGE
wrapper layer (0x8253xxxx–0x8256xxxx). The "Sonic 06 hooks" comment in video.cpp is misleading.

---

## Architecture: Why No D3D-Level CreateIndexBuffer Exists

The D3D layer (0x82A44xxx) has `sub_82A44970` = CreateVertexBuffer (48-byte alloc, type
bit `oris r11,r11,16`). There is NO parallel `CreateIndexBuffer` in the 0x82A44xxx range.

The full 0x82A44xxx function list (from gta4_init.cpp) is:

```
0x82A440A0  (pitch/tiling helper)
0x82A44168  LockIndexBuffer — D3D kernel (pitch calculation, 6 args)
0x82A441F8  LockVertexBuffer — D3D kernel (pitch calculation)
0x82A442A0  (texture setup helper)
0x82A445B0  (GPU resource descriptor setup — called by CreateVertexBuffer)
0x82A44808  (surface/resource op)
0x82A44818  (mesh/resource op)
0x82A44820  GetTextureDesc
0x82A44838  LockIndexBuffer trampoline → sub_82A44168  [type 18 = IndexBuffer dispatch]
0x82A44840  UnlockIndexBuffer trampoline → sub_82A43AE0 [type 18 = IndexBuffer dispatch]
0x82A44848  LockVertexBuffer trampoline → sub_82A441F8  [type 17 = VertexBuffer dispatch]
0x82A44850  CreateTexture (52-byte alloc)
0x82A44970  CreateVertexBuffer (48-byte alloc, calls sub_82A445B0)
0x82A44A98  GetSurfaceDesc
0x82A44B30  GetIndexBufferDesc (reads offsets 28,24; writes type=16 to output)
0x82A44B78  SetTexture (confirmed by video.cpp comment)
0x82A44CF8  DrawPrimitive / SetViewport
0x82A44FE8  Geometry helper
```

Resource type dispatch (from sub_828D9438 in gta4_recomp.59.cpp):
- `cmpwi cr6,r3,17` → VertexBuffer → sub_82A44848 → sub_82A441F8
- `cmpwi cr6,r3,18` → IndexBuffer → sub_82A44838 → sub_82A44168

CreateIndexBuffer allocation is NOT performed at the D3D kernel layer for GTA IV v8.
It is performed at the RAGE wrapper layer (0x8253B640) which calls the D3D allocator
`sub_821B3608` directly with index-buffer-specific parameters.

---

## RAGE Wrapper Layer: 0x8253xxxx Hooked Region

The cluster around `sub_8253B640`:

| Address | Function |
|-|-|
| `0x8253B5D0` | LockVertexBuffer (Sonic 06 `#if 0`) |
| `0x8253B630` | UnlockVertexBuffer (Sonic 06 `#if 0`) |
| `0x8253B640` | **CreateIndexBuffer** |
| `0x8253B6F0` | **LockIndexBuffer** |
| `0x8253B750` | **UnlockIndexBuffer** |
| `0x8253B760` | IsSet |

None of these appear in the generated `.cpp` files (they are excluded from codegen because all
callers in the 0x8253xxxx range are also hooks). None appear in gta4_init.cpp. They are
reachable only at runtime through the hook dispatch system.

---

## Lock/Unlock at the D3D Level (for reference)

| Function | Address | Role |
|-|-|-|
| `sub_82A44168` | `0x82A44168` | LockIndexBuffer kernel stub (pitch/size calc, 6 args) |
| `sub_82A44838` | `0x82A44838` | Lock dispatch for type 18 (trampoline → 0x82A44168) |
| `sub_82A44840` | `0x82A44840` | Unlock dispatch for type 18 (trampoline → 0x82A43AE0) |

The RAGE-level LockIndexBuffer (0x8253B6F0) calls sub_82A44838 which dispatches to
sub_82A44168. sub_82A44168 takes `(device, r4, r5, r6, r7, r8)` and performs GPU tiling
arithmetic to return pitch/offset values for CPU mapping.

---

## Comparison: RAGE-Level Lock/Unlock for VertexBuffer vs IndexBuffer

| | VertexBuffer | IndexBuffer |
|-|-|-|
| Create | `0x82A44970` (D3D layer) / `0x8253B508` (RAGE) | `0x8253B640` (RAGE only) |
| Lock | `0x82A47C80` (large GPU ring-buf fn) | `0x8253B6F0` (RAGE, calls `0x82A44838`) |
| Unlock | `0x82A47E28` | `0x8253B750` |

Note: LockVertexBuffer at 0x82A47C80 is a 400+ line GPU command ring function. LockIndexBuffer
at 0x8253B6F0 is at the RAGE wrapper level and shorter. Both ultimately access the D3D tiling
functions (0x82A44168 / 0x82A441F8) for pitch calculation.

---

## Action Required in video.cpp

Move the following three lines OUT of the `#if 0 // Disabled Sonic 06 hooks` block (around
line 9474) and INTO the active GTA IV hooks section:

```cpp
GUEST_FUNCTION_HOOK(sub_8253B640, CreateIndexBuffer);
GUEST_FUNCTION_HOOK(sub_8253B6F0, LockIndexBuffer);
GUEST_FUNCTION_HOOK(sub_8253B750, UnlockIndexBuffer);
```

The C++ implementations at video.cpp lines 4015–4028 (CreateIndexBuffer) and 2782–2793
(LockIndexBuffer, UnlockIndexBuffer) are already correct and complete.

---

## Files Referenced

- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/gpu/video.cpp` — hooks and C++ implementations
- `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/gta4_init.cpp` — function address table
- `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.71.cpp` — D3D layer (0x82A4xxxx) implementations
- `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.59.cpp` — RAGE mesh/buffer code with type dispatch
- `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.26.cpp` — RAGE wrapper layer (0x8253xxxx–0x8256xxxx)
