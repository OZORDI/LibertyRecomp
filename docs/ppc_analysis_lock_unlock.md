# PPC Analysis: Lock/Unlock Buffer Functions — GTA IV v8

**Date**: 2026-03-28
**Source file**: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.71.cpp`
**Hook file**: `LibertyRecomp/gpu/video.cpp`

---

## Summary of Known Hooks

| Function | Address | Status |
|-|-|-|
| LockTextureRect | 0x82A479B0 | Hooked |
| UnlockTextureRect | 0x82A47AE0 | Hooked |
| LockVertexBuffer | 0x82A47C80 | Hooked |
| UnlockVertexBuffer | 0x82A47E28 | Hooked |
| **LockIndexBuffer** | **UNKNOWN** | TODO |
| **UnlockIndexBuffer** | **UNKNOWN** | TODO |

---

## LockTextureRect — 0x82A479B0

**Signature**: `(texture_obj*, out_rect_with_offset*, out_locked_rect*, flags)`
**Size**: 162 lines

**Structure**:
1. Reads `texture_obj[13216]` — checks if texture has a D3D resource ptr (GPU surface)
2. If D3D resource present: computes texture dimensions from `obj[14888]`, `obj[14892]`, `obj[14904]`; tests bit 26 of `obj[10941]` (format flag, e.g. tiled vs linear); computes mip offset into `out_rect_with_offset`
3. If no D3D resource: falls through to slow path via `sub_82A47A54`, reads `obj[10940]` texture flags
4. Returns: fills `out_locked_rect->pitch` and `out_locked_rect->bits` (virtual address of mapped memory)

**Key offsets**: `obj[13216]` = D3D surface ptr, `obj[10941]` = format/tiling flags, `obj[14888/14892/14904]` = mip dimensions

---

## UnlockTextureRect — 0x82A47AE0

**Signature**: `(texture_obj*, rect_or_level, flags)`
**Size**: 135 lines

**Structure**:
1. Reads `obj[13500]` — inner resource pointer
2. Sets bit 6 of `resource[108]` (`ori r10,r10,64`) — marks region dirty/needs upload
3. If both rect args are zero (full unlock): reads texture flags `obj[10940]`, tests bits 27/28/29 for tiling mode
4. Depending on tiling: checks `obj[12432/12436]` (surface range bounds) against `obj[12720/12724]` (current dirty range)
5. Updates dirty region tracking, then calls deferred GPU flush if necessary

**Key offsets**: `obj[13500]` = inner resource, `resource[108]` = dirty flags, `obj[10940]` = texture format flags

---

## LockVertexBuffer — 0x82A47C80

**Signature**: `(vb_obj*)`
**Size**: 162 lines

**Structure**:
1. Reads `obj[13500]` — inner GPU buffer resource ptr
2. Calls `sub_82A415A8` — acquire buffer lock (ring allocator gate)
3. Checks `resource[152]` — if null, takes slow path via `sub_82A49830`
4. Fast path: reads `obj[48]` (ring buffer end ptr), `obj[14912]` (ring base), computes `(end - base + 4) >> 2` = free vertex count
5. If count exceeds 0x100000 (1M entries): sets bit 5 of `obj[10941]` (overflow flag)
6. Reads ring buffer write cursor `obj[13520]`, end `obj[13524]`; if cursor at end calls `sub_82A47060` (advance ring)
7. Packs command: writes GPU memory address descriptor into ring slot `[r3+0]` and `[r3+4]` (oris with 0x8100_0000 base, tiled address bits)
8. Advances ring cursor: `obj[13520] = slot + 8`
9. Returns: r3 = ring slot pointer (mapped GPU memory to write vertex data into)

**Key offsets**: `obj[13500]` = D3D resource, `obj[13520/13524]` = ring cursor/end, `obj[48]` = ring end ptr, `obj[14912]` = ring base, `obj[10941]` = flags

---

## UnlockVertexBuffer — 0x82A47E28

**Signature**: `(vb_obj*, dirty_mask_a, dirty_mask_b)`
**Size**: 178 lines

**Structure**:
1. Saves r4/r5 as dirty_mask_a / dirty_mask_b
2. Loads 5 bitmask words from `obj[0..40]` and 5 from `dirty_mask_a[0..56]`; ANDs them together → r4, r27, r28, r29, r30 (dirty region descriptor)
3. Reads `obj[13500]` into r18 (inner resource for later flush)
4. For each non-zero dirty component:
   - Calls `sub_82A4B428(obj, mask, PAGE_SIZE_FLAG, dirty_region_ptr)` to flush region
   - Zeros out `obj[0..40]` entries for that component
5. Handles tiled surface component at `obj+1920` (flag 0x1E0000 = bits 11-14 set = tiled resource)
6. Calls `sub_82A4C1C0` for tiling conversion if needed
7. Late path: applies further dirty masks from `obj[40+]` range

**Key insight**: This is a GPU command buffer flush — it does NOT do CPU memcpy. It records dirty tile ranges and issues GPU copy commands via sub_82A4B428.

---

## Functions in Range 0x82A47E28–0x82A48500 (Complete List)

| Address | Lines | Pattern | Assessment |
|-|-|-|-|
| 0x82A47E28 | 178 | UnlockVertexBuffer — see above | Hooked |
| 0x82A48440 | 30 | Snapshot: copies `obj[48,52,14912,14908,14900,14904]` → `obj[13392..13412]` | Save-state helper (snapshot ring ptrs) |
| 0x82A48478 | 34 | Restore: copies `obj[13392..13412]` back → `obj[48,52,14912,14908,14900,14904]`; also sets `obj[56]=obj[52]-160` | Restore-state helper |
| 0x82A484B8 | 94 | Alignment check on `obj[10896]→[4]` vs r5; calls sub_82A41188 + sub_82A41320 in loop | Buffer alignment wait loop |

---

## Analysis: LockIndexBuffer / UnlockIndexBuffer

### Why they are not in 0x82A47xxx range

The vertex buffer functions cluster at 0x82A47C80/0x82A47E28. The index buffer functions in RAGE are typically adjacent in the vtable — but GTA IV v8 may place them in a different section. The init table shows a gap between 0x82A47E28 and 0x82A48440; no candidate function in between.

### Search: Functions with LockVertexBuffer structural signature

LockVertexBuffer structural fingerprint:
- Reads `obj[13500]` (inner resource ptr)
- Calls `sub_82A415A8` (lock acquire)
- Accesses `resource[152]`
- Ring buffer logic with `obj[13520/13524]`

**sub_82A48B90** (132 lines, in 0x82A47E28–0x82A49000 range):
- Reads `obj[10941]` bit 26 (tiled flag) — same as LockVertexBuffer
- Reads `obj[13500]` — inner resource ptr
- Checks `resource[152]` — same check
- Calls `sub_82A48988` if resource is null (slow path, analogous to `sub_82A49830` in LockVB)
- Calls `sub_82A46DF8` if resource is non-null (lock acquire helper)
- Manages `obj[14916/14920]` ring counters (different offsets than VB's 13520/13524 — consistent with different buffer type)
- Returns void (no pointer return) — **NOT a Lock function, likely an internal flush/sync**

### Shared Lock/Unlock for Vertex+Index Buffers

Checking whether GTA IV uses a shared Lock/Unlock: **NO**. The video.cpp hook list explicitly has separate `LockVertexBuffer` and `LockIndexBuffer` entries, and the TODO comment at line 9371 says CreateIndexBuffer address is unknown. This confirms they are separate functions not yet identified.

### Cross-file search in generated code

Running a full search for index-buffer-like signatures (objects using different ring offset but same 13500/152 pattern) across the entire 82A4xxxx range in gta4_init.cpp shows the following candidates **not yet analyzed**:

- **0x82A46028** — in 82A46xxx block (just before LockTextureRect cluster)
- **0x82A46098** — adjacent
- **0x82A46198** — adjacent

These may be the index buffer lock/unlock functions. They are adjacent to the texture rect lock/unlock pair (82A479B0/82A47AE0) in a similar way that vertex buffer lock/unlock is at 82A47C80/82A47E28.

---

## Recommended Next Steps for LockIndexBuffer

1. **Read sub_82A46028, 0x82A46098, 0x82A46198** from gta4_recomp.71.cpp — these are in the same cluster as texture and vertex buffer functions
2. Compare structural signature: should read `obj[13500]`, access `resource[152]`, compute ring buffer address
3. Index buffer lock differs from vertex: stride is 16-bit indices (element size 2 or 4 bytes), check for `obj[10896]→[4]` = format field (D3DFMT_INDEX16 vs INDEX32)
4. If those don't match, check **0x82A46578, 0x82A465F8** (other candidates in 82A46xxx cluster)

### Note on Sonic 06 Reference

The disabled Sonic 06 hooks use `sub_8253B6F0` / `sub_8253B750` for LockIndexBuffer/UnlockIndexBuffer at completely different addresses — **not applicable to GTA IV**.

---

## video.cpp Hook Status

```cpp
// LibertyRecomp/gpu/video.cpp line 9381-9382
GUEST_FUNCTION_HOOK(sub_82A47C80, LockVertexBuffer);
GUEST_FUNCTION_HOOK(sub_82A47E28, UnlockVertexBuffer);
// LockIndexBuffer: TODO — address not yet found
// UnlockIndexBuffer: TODO — address not yet found
```

The `LockIndexBuffer` host implementation (line 2782) and `UnlockIndexBuffer` (line 2787) are already implemented and ready — only the GTA IV PPC address is missing.
