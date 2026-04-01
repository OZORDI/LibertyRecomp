# PPC Analysis: Index Buffer Cross-References

**Date:** 2026-03-28
**Scope:** GTA IV recomp — SetIndices, CreateIndexBuffer, DrawIndexedPrimitive

---

## 1. SetIndices Hook (sub_82543AC8) — Parameter Analysis

**Location:** `/LibertyRecomp/gpu/video.cpp` L9935–9952
**Hook type:** `PPC_FUNC_HOOK`

```
r3 = deviceAddr  (guest pointer to GuestDevice, translated as base + deviceAddr)
r4 = bufAddr     (guest physical address of index buffer, OR 0 to unbind)
```

The hook calls `GTAIV::LookupBuffer(bufAddr)` to translate the guest address to a host `GuestBuffer*`. If the buffer is not registered (creation hook didn't fire), the call is silently skipped with a warning log. This means **SetIndices will silently no-op if CreateIndexBuffer hasn't been called yet**.

---

## 2. Callers of sub_82543AC8 in Generated Code

**Result: ZERO direct callers found in any generated `.cpp` file.**

Exhaustive search across all 79 generated files (`gta4_recomp.0.cpp` through `gta4_recomp.76.cpp`) found no occurrences of the string `sub_82543AC8`. The function is also absent from `gta4_init.h` as a `PPC_EXTERN_IMPORT` or `PPC_EXTERN_FUNC` declaration.

**Why:** Sub_82543AC8 lives in the address range 0x82530468–0x82569C18 (file 26). Within this file, there is a documented gap from **0x82541AF8 to 0x82543C20** — over 5KB — where all functions are hooked and excluded from code generation. This gap contains:

| Address | Function | Hook |
|-|-|-|
| 0x82543918 | SetStreamSource | `PPC_FUNC_HOOK` |
| 0x82543AC8 | **SetIndices** | `PPC_FUNC_HOOK` |
| 0x82543628 | SetScissorRect | `GUEST_FUNCTION_HOOK` |
| 0x825436F0 | SetViewport | `GUEST_FUNCTION_HOOK` |
| 0x82543EE0 | SetRenderTarget | `GUEST_FUNCTION_HOOK` |
| 0x82541A78 | SetRenderState[ZENABLE] | `GUEST_FUNCTION_HOOK` |
| (many more render state setters) | | |

**Implication:** These functions form a **shared graphics abstraction layer** (comment at video.cpp L9881). They are called via `bl` (direct PPC branch-link) from RAGE geometry/draw functions that are *themselves* hooked or excluded from generated code. The entire D3D submission path is either hooked or bypassed at a higher level.

---

## 3. Callers of DrawIndexedPrimitive (sub_826FF030)

**Result: ZERO direct callers in generated code.**

Sub_826FF030 sits in the gap 0x826FEFB0–0x826FF070 in file 41, alongside:

| Address | Function | Hook |
|-|-|-|
| 0x826FEC28 | DrawPrimitive | `GUEST_FUNCTION_HOOK` (Sonic 06 `#if 0` block — disabled) |
| 0x826FF030 | **DrawIndexedPrimitive** | `GUEST_FUNCTION_HOOK` (Sonic 06 `#if 0` block — disabled) |
| 0x826FE5C0 | DrawPrimitiveUP | `GUEST_FUNCTION_HOOK` (Sonic 06 `#if 0` block — disabled) |

**Critical finding:** These three draw hooks in `video.cpp` are inside a `#if 0 // Disabled Sonic 06 hooks` block (L9463–9511). They are **NOT active**. The active GTA IV draw hooks use different addresses (the 0x82A4xxxx range hooked at L9369–9374), but no active hook for `DrawIndexedPrimitive` is present in the GTA IV section.

The active GTA IV hooks are:
- `GUEST_FUNCTION_HOOK(sub_82A44850, CreateTexture)` — L9369
- `GUEST_FUNCTION_HOOK(sub_82A44970, CreateVertexBuffer)` — L9370
- `// TODO: Find CreateIndexBuffer address` — L9371 (not yet done)

---

## 4. CreateIndexBuffer (sub_8253B640)

**Result: ZERO direct callers in generated code.**

Sub_8253B640 sits in the hooked gap 0x8253AB20–0x8253BC70 in file 26, alongside:

| Address | Function |
|-|-|
| 0x8253B508 | CreateVertexBuffer |
| 0x8253B640 | **CreateIndexBuffer** |
| 0x8253B6F0 | LockIndexBuffer |
| 0x8253B750 | UnlockIndexBuffer |
| 0x8253B760 | IsSet |

The active hook is `GUEST_FUNCTION_HOOK(sub_8253B640, CreateIndexBuffer)` at video.cpp L9491 — inside the `#if 0` Sonic 06 block and therefore **not active**.

**For the GTA IV path**, the TODO at L9371 identifies `sub_82A44970` (VertexBuffer) as the pattern. The CreateIndexBuffer equivalent in GTA IV's allocator layer is likely at a similar offset in the 0x82A44xxx–0x82A45xxx range. The `CreateIndexBuffer` C++ implementation at video.cpp L4015–4028 is correct; it just needs to be hooked to the right GTA IV address.

---

## 5. D3DFMT_INDEX16/INDEX32 Constants (0x65/0x66)

No occurrences of `D3DFMT_INDEX16` (0x65=101) or `D3DFMT_INDEX32` (0x66=102) as tagged format arguments appear in the generated PPC code, because `CreateIndexBuffer` is never called from generated code — all calls originate from within the hooked/excluded layer.

In `video.cpp`, these constants appear at:
- L2789: `if (buffer->guestFormat == D3DFMT_INDEX32)` in `UnlockIndexBuffer`
- L3895–3898: `ConvertFormat()` switch — INDEX16 → R16_UINT, INDEX32 → R32_UINT
- L4020: `buffer->format = ConvertFormat(format)` in `CreateIndexBuffer`

---

## 6. "IndexBuffer" / "index" String Search in Generated Code

No string literals containing "IndexBuffer" or "index" appear in generated code as context strings. The generated PPC code is pure arithmetic/load-store; all resource type metadata is managed in the C++ host layer.

---

## 7. RAGE grmGeometry/grmModel Pattern

The RAGE geometry draw path follows this sequence, all in the hooked abstraction layer:

```
grmGeometry::Draw  (some 0x8254xxxx or 0x826xxxxx function)
  → sub_82543918   SetStreamSource(device, slot, vertexBuffer, offset, stride)
  → sub_82543AC8   SetIndices(device, indexBuffer)
  → sub_826FF030   DrawIndexedPrimitive(device, type, baseVtx, minVtx, numVtx, startIdx, primCount)
```

The geometry system likely stores the index buffer pointer in the `grmGeometry` struct (a field loaded at draw time). The struct is allocated via:
- `grmGeometry::CreateBuffers` (unidentified address) → calls `CreateIndexBuffer` → returns a registered `GuestBuffer*` whose guest address is stored in the struct field that SetIndices receives as r4.

---

## 8. Key Gaps / Action Items

| Issue | Impact |
|-|-|
| DrawIndexedPrimitive hook in `#if 0` block | **No draw calls reach GPU** |
| CreateIndexBuffer not hooked (GTA IV address unknown) | Index buffers created as raw Xbox D3D objects, invisible to LookupBuffer |
| SetIndices skips unregistered buffers | Even when called, SetIndices is a no-op until creation is hooked |
| GTA IV-specific draw function address unknown | Need to find the GTA IV equivalent of sub_826FF030 |

The GTA IV active Present path is hooked at `sub_82A467D8` (L9392). Tracing backward from that function in the generated code is the recommended path to find the actual GTA IV DrawIndexedPrimitive address.

---

## File References

- Hook implementations: `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/gpu/video.cpp`
- Generated code file covering 0x8253xxxx–0x8256xxxx: `.../generated/gta4_recomp.26.cpp`
- Generated code file covering 0x826Exxxx–0x8270xxxx: `.../generated/gta4_recomp.41.cpp`
- Function declarations: `.../generated/gta4_init.h`
