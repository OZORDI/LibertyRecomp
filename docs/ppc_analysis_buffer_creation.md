# PPC Analysis: GPU Buffer Creation — CreateVertexBuffer, CreateTexture, CreateIndexBuffer

## TL;DR

| Name | Address | Layer | Status |
|-|-|-|-|
| CreateTexture | `sub_82A44850` | D3D device | ACTIVE hook (`GTAIV_CreateTexture`) |
| CreateVertexBuffer | `sub_82A44970` | D3D device | ACTIVE hook (`GTAIV_CreateVertexBuffer`) |
| CreateIndexBuffer | `sub_8253B640` | RAGE wrapper | ACTIVE hook (`CreateIndexBuffer`) |
| GpuMemAllocStub | `sub_82A50F28` | D3D device | ACTIVE stub (fake GPU memory offsets) |

---

## Layer Map

GTA IV's D3D resource creation has two layers:

1. **D3D device layer** (`0x82A4xxxx`): Xbox 360 IDirect3DDevice9-like vtable entries. These allocate the struct and call the GPU memory allocator.
2. **RAGE wrapper layer** (`0x8253xxxx`): RAGE engine wrappers that match standard D3D9 signatures. These are the functions that actually get called from game code and dispatch down into the D3D device layer.

The active hooks in `video.cpp` operate at BOTH layers:
- `GTAIV_CreateVertexBuffer` / `GTAIV_CreateTexture` hook the D3D device layer
- `CreateIndexBuffer` hooks the RAGE wrapper layer

---

## D3D Device Layer Analysis

### CreateVertexBuffer — `sub_82A44970`

**Source**: `gta4_recomp.71.cpp` (recompiled PPC), hooked in `video.cpp` line 9250.

**Signature** (inferred from PPC): `CreateVertexBuffer(device, length, usage, pool, unknown, ppVB_out, stride_out)`

**Allocation pattern**:
- Allocates **48 bytes** via `sub_821B3608` (RAGE pool allocator), `r3=48`
- Calls `sub_82A445B0` to initialize the buffer descriptor struct
- Calls `sub_82A50F28` (GpuMemAlloc) to allocate GPU-side memory
- Falls back to `sub_82A50ED8` (alternate GPU allocator) if GpuMemAlloc fills its pool (>2048 entries)

**Struct fields populated** (inferred from `sub_82A445B0` output and post-call writes):
- `+0`: flags word — sets bit `0x00100000` (oris r11,r11,16) and `0x80000000` (pool default = D3DPOOL_DEFAULT)
- `+28`: GPU physical address returned by `sub_82A50F28` (packed with rlwimi from existing field)
- `+32`: buffer size — lower 17 bits (`clrlwi r11,r11,15`)
- `+40`: format/type — lower 6 bits (`clrlwi r11,r11,26`); compared to 22 (D3DFMT_INDEX16) and 23 (D3DFMT_INDEX32)

**Xbox 360 kernel calls**: None direct. `sub_82A50F28` internally uses `RtlInitializeCriticalSection`, `RtlEnterCriticalSection` and `sub_82A50CC0` (a GPU ring-buffer allocator using `lwarx/stwcx.` for lock-free CAS).

**D3D usage constants seen**:
- Pool: `D3DPOOL_DEFAULT = 0` — bit `0x80000000` set in flags when pool==0
- Type flags at +40: 22 = D3DFMT_INDEX16, 23 = D3DFMT_INDEX32

---

### CreateTexture — `sub_82A44850`

**Source**: `gta4_recomp.71.cpp`, hooked in `video.cpp` line 9251.

**Signature**: `CreateTexture(device, width, height, levels, usage, format, pool, ppTexture_out, pitch_out)`

**Allocation pattern**:
- Allocates **52 bytes** via `sub_821B3608`, `r3=52`
- Calls `sub_82A442A0` to set up texture descriptor (which internally calls `sub_82A445B0`)
- Allocates a second smaller struct for the surface via another `sub_821B3608` call with computed size

**Struct fields populated**:
- `+32`: texture descriptor word (rlwimi from GPU alloc result)
- `+48`: format/size packed field (rlwimi)

**Key difference from CreateVertexBuffer**: Texture has width/height/mipLevels parameters; vertex buffer has only length. Index buffer is a third variant (see below).

---

### GpuMemAllocStub — `sub_82A50F28`

**Source**: `gta4_recomp.71.cpp` (804-byte function). Hooked in `video.cpp` line 9149 with `GpuMemAllocStub`.

**Signature**: `(size: r3, out_offset_ptr: r4) -> 1 on success`

**Pattern**: Manages a pool of 6144-entry free-list (`stw r11,28(r31)` with 6144). Uses `lwarx/stwcx.` CAS to atomically link a new pool block if none exists. Returns a **GPU memory offset** (small integer, not a host pointer) written to `*r4`. The pool struct is 804 bytes, initialized with `RtlInitializeCriticalSection`.

**Callers**:
- `sub_82A44970` (CreateVertexBuffer) — 1x call
- `sub_82A50ED8` — alternate path, itself called from CreateVertexBuffer on overflow

**Other GPU alloc function**: `sub_82A50ED8` — called when GpuMemAlloc pool offset exceeds 2048; resets/reallocates. Also called from `sub_82A4A1E0` (vertex declaration?) and `sub_82A50DA0`.

---

## RAGE Wrapper Layer Analysis

### CreateIndexBuffer — `sub_8253B640`

**Source**: Entry point within `sub_8253AB20` (large dispatch function, `gta4_recomp.26.cpp`). The address `0x8253B640` is a label inside `sub_8253AB20` — codegen excluded this range (0x8253AB20 to 0x8253BC70) entirely; all functions in this gap are hook entry points.

**Disassembly at 0x8253B640** (first 3 instructions):
```
0x8253b630:  addi r26, r11, -0x20f0    ; (still in outer function body)
0x8253b634:  lis  r11, -0x7d58
0x8253b638:  addi r17, r11, -0x2a0
0x8253b63c:  lis  r11, -0x7d58
0x8253b640:  addi r25, r11, -0x230     ; <-- CreateIndexBuffer entry
0x8253b644:  addi r6, r1, 0x70
```

Note: This is mid-function code, not a prologue. The outer prologue (mflr r12 + __savegprlr chain) is at `0x8253B440`.

**Signature** (from `video.cpp` C++ hook): `CreateIndexBuffer(uint32_t length, uint32_t /*unused*/, uint32_t format)`

**C++ implementation** (lines 4015–4028 of `video.cpp`):
```cpp
static GuestBuffer* CreateIndexBuffer(uint32_t length, uint32_t, uint32_t format)
{
    auto buffer = g_userHeap.AllocPhysical<GuestBuffer>(ResourceType::IndexBuffer);
    buffer->buffer = g_device->createBuffer(RenderBufferDesc::IndexBuffer(length, GetBufferHeapType()));
    buffer->dataSize = length;
    buffer->format = ConvertFormat(format);
    buffer->guestFormat = format;
    GTAIV::RegisterBuffer(g_memory.MapVirtual(buffer), buffer);
    return buffer;
}
```

**Hook status**: ACTIVE at `video.cpp` line 9491:
```cpp
GUEST_FUNCTION_HOOK(sub_8253B640, CreateIndexBuffer);
```

---

## Init Table Gap (RAGE Wrapper Range)

The `gta4_init.cpp` init table jumps from `0x8253AB20` directly to `0x8253BC70` — a gap of `0x1150` bytes containing all the buffer operation entry points:

| Address | Function | Hook Status |
|-|-|-|
| `0x8253A8D8` | CreateTexture (RAGE wrapper) | ACTIVE |
| `0x8253AB20` | Large dispatch function | recompiled |
| `0x8253B440` | Outer function prologue (contains B508–B750) | inside 8253AB20 |
| `0x8253B508` | CreateVertexBuffer (RAGE wrapper) | ACTIVE |
| `0x8253B5D0` | LockVertexBuffer | ACTIVE (in disabled block per ppc_analysis_lock_unlock.md) |
| `0x8253B630` | UnlockVertexBuffer | ACTIVE |
| `0x8253B640` | **CreateIndexBuffer** | ACTIVE |
| `0x8253B6F0` | LockIndexBuffer | ACTIVE |
| `0x8253B750` | UnlockIndexBuffer | ACTIVE |
| `0x8253BC70` | Next recompiled function | recompiled |

---

## D3D Layer Surrounding Functions

Adjacent functions to CreateTexture/CreateVertexBuffer in `gta4_recomp.71.cpp`:

| Address | Inferred Purpose |
|-|-|
| `0x82A44808` | GetLevelCount (reads field +44, extracts 4-bit count) |
| `0x82A44818` | Thunk -> sub_82A439E0 (Release?) |
| `0x82A44820` | Thunk -> sub_82A44168 with r4=0 (CreateIndexBuffer D3D level thunk) |
| `0x82A44838` | Thunk -> sub_82A44168 with r4=? |
| `0x82A44840` | Thunk chain |
| `0x82A44848` | Thunk -> sub_82A44850 (CreateTexture) |
| `0x82A44850` | **CreateTexture** |
| `0x82A44970` | **CreateVertexBuffer** |
| `0x82A44A98` | GetSurfaceDesc (hooked) |
| `0x82A44B30` | Sets type=16 on buffer struct (post-creation stamp) |
| `0x82A44B78` | Vertex format/stream binding |
| `0x82A44CF8` | DrawPrimitive (calls ring-buffer flush sub_82A499B8) |

**Key finding**: There is **no separate CreateIndexBuffer** at the D3D device layer (0x82A4xxxx). The `sub_82A44820` thunk dispatches to `sub_82A44168`, which is a unified buffer creation path taking a type flag; index buffers are created via the RAGE wrapper layer directly through the hook at `sub_8253B640`.

---

## Files Referenced

- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/gpu/video.cpp` — hook registrations and C++ implementations
- `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.71.cpp` — D3D device layer PPC (0x82A4xxxx)
- `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.26.cpp` — RAGE wrapper layer PPC (0x8253xxxx)
- `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/gta4_init.cpp` — function init table
- `/Users/Ozordi/Downloads/LibertyRecomp/gta_iv/default.bin` — decompressed Xbox 360 PE image
