# rage::grcTextureXenon vtable research

Agent 3 / 15 — research-only deliverable for the post-fix crash in `vfunc[23]` (slot 24).

## Summary

- **Crash slot**: `vfunc[23]` = **slot index 24** in the vtable (0-indexed array, 0-indexed "vfunc[]" naming).
- **Vtable address**: `0x8209612c` (25 slots, slots 0..24, pointer-sized at 4 bytes each).
- **Slot-24 pointer entry** lives at `0x8209612c + 24*4 = 0x8209618C` and points to **`sub_828D9AC8`** (size 764 bytes / 0x2FC).
- **Semantic of `sub_828D9AC8`**: **`CopyFromBitmap` / `Upload` / `UpdateFromBitmap`** — walks the mipmap chain of a source linked-list bitmap, calls `D3DTexture_GetLevelDesc`-style helpers, then `XGCopySurface`-style texture blit paths (`sub_82A57730` for 3D volumes, `sub_82A56560` for 2D arrays/cubes), emitting GPU-tile data into the Xenon texture. On success it also copies 6 floats (UV scale/bias) from source+12..+18 into `this+36..+56`.

## Inheritance chain (RTTI, top-down)

```
rage::datBase  <-  rage::pgBase  <-  rage::grcTexture  <-  rage::grcTextureXenonProxy  <-  rage::grcTextureXenon
```

- `rage::grcTexture` vtable @ `0x8209369c` (25 slots, mostly stub `sub_822BCA90` + `sub_82165060`).
- `rage::grcTextureXenonProxy` vtable @ `0x82098594` (25 slots, identical stub pattern as grcTexture).
- `rage::grcTextureXenon` vtable @ `0x8209612c` (25 slots, Xenon-specific overrides at 0, 9, 15, 16, 17, 24).
- `rage::grcTextureString` is a sibling in the hierarchy; slot 3 of grcTextureXenon reuses `grcTextureString::vfunc[2]` (`return *(dword*)(this+20)`).

All three texture vtables are **25 slots**. The MCP header reports "25 slots" and "vfunc[23]" is the semantic name for the last slot (index 24). So the crash is in the very last vfunc of the class.

## Full slot map (grcTextureXenon @ 0x8209612c)

| slot | addr (entry) | target func | notes / sibling-class alias | semantic |
|-|-|-|-|-|
| 0 | 0x8209612C | sub_828DB6D8 | — | **Destructor / scalar deleting dtor** (writes `&vftable`, calls `sub_828BEB08` on +24, `sub_821B3560` release, `sub_828ED238`) |
| 1 | 0x82096130 | sub_822BCA90 | stub | empty stub (shared by most classes) |
| 2 | 0x82096134 | sub_822BCA90 | stub | empty stub |
| 3 | 0x82096138 | sub_82708728 | `grcTextureString::vfunc[2]` | **GetTexturePtr / GetD3DTexture** — `return *(dword*)(this+20)` |
| 4 | 0x8209613C | sub_822BCA90 | stub | empty stub |
| 5 | 0x82096140 | sub_82165060 | `CRenderPhaseHtml::vfunc[13]` (shared thunk) | no-op return-only stub |
| 6 | 0x82096144 | sub_828D9678 | (inside sub_828D9620 tail) | returns `*(dword*)(this+0x14)` or similar small accessor |
| 7 | 0x82096148 | sub_828D9680 | (inside sub_828D9620 tail) | small accessor at +0x14 region |
| 8 | 0x8209614C | sub_826041E8 | `CViewportHtml::vfunc[2]` (shared thunk) | no-op / return 1 |
| 9 | 0x82096150 | sub_82191F00 | `grcTextureXenon::vfunc[8]` | **GetLevels / GetMipCount** — `return *(dword*)(this+32)` |
| 10 | 0x82096154 | sub_828D9688 | (inside sub_828D9620 tail) | small accessor |
| 11 | 0x82096158 | sub_828D96A8 | (inside sub_828D9620 tail) | small accessor |
| 12 | 0x8209615C | sub_822BCA90 | stub | empty stub |
| 13 | 0x82096160 | sub_822BCA90 | stub | empty stub |
| 14 | 0x82096164 | sub_82165060 | shared thunk | no-op stub |
| 15 | 0x82096168 | sub_828D9F98 | (inside sub_828D9F08 tail) | paired with slot 16 — likely **Unlock** / `UnlockRect` |
| 16 | 0x8209616C | sub_828D9F98 | same as slot 15 | same Unlock |
| 17 | 0x82096170 | sub_828D9DC8 | `grcTextureXenon::vfunc[16]` | **LockRect / Map** — switches on D3D resource type (3/17/18) and fills `a4[0..7]` with row-pitch / slice-pitch / level-desc |
| 18 | 0x82096174 | sub_828D9F08 | `grcTextureXenon::vfunc[17]` | **UnlockRect / Unmap** — switches on D3D resource type and calls `sub_82A42E88` (XGUntileSurface-style completion) |
| 19 | 0x82096178 | sub_822BCA90 | stub | empty stub |
| 20 | 0x8209617C | sub_822BCA90 | stub | empty stub |
| 21 | 0x82096180 | sub_822BCA90 | stub | empty stub |
| 22 | 0x82096184 | sub_822BCA90 | stub | empty stub |
| 23 | 0x82096188 | sub_822BCA90 | stub | empty stub |
| 24 | **0x8209618C** | **sub_828D9AC8** | **`grcTextureXenon::vfunc[23]`** | **CopyFromBitmap / Upload** (see below) |

`sub_828D9620` is the **ctor** (zero-init: `*(word*)(this+28)=0`, `*(word*)(this+30)=0`, `*(dword*)(this+32)=0`, `*(dword*)(this+24)=0`, `*(dword*)(this+16)=0`, `*(dword*)this = &vftable`). The "locs" 0x828D9678/80/88/A8 sit in the epilogue of the ctor — they are short trailing helper accessors (each is within the ctor's function bounds per `resolve_address`), effectively single-instruction getters shared across slots 6, 7, 10, 11.

## Vfunc[23] = sub_828D9AC8 — full semantic

Signature (from pseudo): `int __fastcall sub_828D9AC8(grcTextureXenon* this /*r3*/, rage::bitmap* src /*r4*/)`

Preconditions (early-out with `return 0` if any fails):

| check | meaning |
|-|-|
| `this->lvl_count_byte(9) == mip_chain_length(src) - 1` | mip count matches |
| `this->width(28) == src->width(0)` | width matches |
| `this->height(30) == src->height(2)` | height matches |
| `this->depth(32) == mip_chain_depth(src) + 1` | depth/array-len matches |

Body:

1. Walks the source as a linked list via `src->next_level = *(dword*)(src+28)` (that's `sub_828D1C98` / `sub_828D1898` — chain-walkers).
2. Calls `sub_828470E0` / `sub_82847120` — **VMX save/restore block** (increments/decrements a counter at `TLS[r13]+1668/1676/1680`). These are the standard `ProfileBegin/End` or VMX-context save/restore wrappers around the vector blit.
3. Dispatch on `*(byte*)(src+8)` (== source bitmap type 1/3/else):
   - **1** -> `sub_82A44838` (thunk to `sub_82A44168`) — 2D rect locker
   - **3** -> `sub_82A44848` (thunk to `sub_82A441F8`) — cube locker
   - **else** -> `sub_82A44820` (thunk to `sub_82A44168(... ,0, ...)`) — default 2D
4. If the lock flags set `0x100` in `locked_desc[56]`, emits GPU-tiled data:
   - depth==1 (not cube): `sub_82A56560` (2D tile blit with `byte_8200CF50` pixel-size table — clearly **XGCopySurface / XGTileTextureLevel**)
   - else: `sub_82A57730` (3D/volume tile blit, same pixel-size table + 12-byte `v94[6]` box)
5. Calls `sub_82A42E88` -> `sub_82A4A600` — this is an `lwarx/stwcx.` atomic release on the D3D resource (the standard D3D9 `Release/Unlock` fence).
6. On success: copies 6 floats from `src + 12 .. src + 18 (words*4)` into `this + 36..56` — this is **UV min/max scale+offset** (`Texture->UVTransform.ScaleX/Y/Z + OffsetX/Y/Z`).
7. Returns `1` on success.

### Classification

Given:
- Guarded against size/mip mismatch (classic asymmetric `Update` contract, not `Lock`).
- Full mipchain + box iteration.
- Writes UV transform state last.
- Uses `XGCopySurface`-style tilers (`sub_82A57730` / `sub_82A56560`).
- No sampler state or shader binding involved.

This is **`grcTextureXenon::CopyFromBitmap(const rage::bitmap&)`** (RAGE name) or equivalently **`Upload / UpdateFromBitmap`**. It is the "copy pixels from a host bitmap into the GPU-tiled texture" operation — the texture-population entry point used when the CPU wants to refresh the GPU side (typically from a `rage::grcImage` or a runtime-generated source).

Neighboring slots confirm this placement: slot 17 is **Lock**, slot 18 is **Unlock** — slot 24 is the higher-level Upload that internally calls Lock/blit/Unlock equivalents (see how `sub_828D9AC8` calls the same `sub_82A44820/38/48` lockers that `sub_828D9DC8` (Lock) calls, then emits tiled data, then calls `sub_82A42E88` just like `sub_828D9F08` (Unlock) does).

## Parent-vs-child override map (vs `rage::grcTexture` @ 0x8209369c)

Only 6 slots genuinely override (are not stubs or inherited thunks):

| slot | grcTexture (parent) | grcTextureXenon (child) | override? |
|-|-|-|-|
| 0 | sub_828C28A8 (generic dtor) | sub_828DB6D8 (Xenon dtor) | yes |
| 3 | sub_82165060 (stub thunk) | sub_82708728 (GetTexPtr) | yes (also used by grcTextureString) |
| 6 | sub_82165060 | sub_828D9678 | yes |
| 7 | sub_82165060 | sub_828D9680 | yes |
| 9 | sub_826041E8 | sub_82191F00 (GetLevels) | yes |
| 10 | sub_828C2398 | sub_828D9688 | yes |
| 11 | sub_828C2398 | sub_828D96A8 | yes |
| 15 | sub_82165060 | sub_828D9F98 | yes (Unlock alias) |
| 16 | sub_82165060 | sub_828D9F98 | yes |
| 17 | sub_82165060 | sub_828D9DC8 (Lock) | yes |
| 18 | sub_82165060 | sub_828D9F08 (Unlock) | yes |
| 24 | sub_82165060 | **sub_828D9AC8 (CopyFromBitmap)** | **yes — this is the crashing slot** |

All other slots (1, 2, 4, 5, 8, 12, 13, 14, 19..23) are inherited verbatim from `grcTexture` and point to `sub_822BCA90` (no-op) or `sub_82165060` (no-op thunk) or `sub_826041E8` (no-op return-1).

## Layout of `this` (inferred from vfunc[23] and the ctor)

| offset | size | meaning (inferred) |
|-|-|-|
| +0x00 | 4 | vftable pointer |
| +0x08 | 4 | ? (read-only in class) |
| +0x0C | 4 | ? |
| +0x10 | 4 | set to 0 in ctor |
| +0x14 | 4 | read/written — flags / ref / state |
| +0x18 | 4 | **D3D resource pointer** (this+24 — dereffed by every Lock/Unlock path) |
| +0x20 | 4 | mip count / levels (GetLevels returns this) |
| +0x1C | 2 | width (ctor zeroes 28 = +0x1C; pseudo at 28 reads width) |
| +0x1E | 2 | height |
| +0x09 | 1 | "mip_count - 1" sanity byte (byte at +9) |
| +0x24..+0x38 | 6*4 | UV transform floats (ScaleX/Y/Z + OffsetX/Y/Z) written at end of vfunc[23] |
| +0x48 | 4 | read in 1 place |

## Crash interpretation (research-only)

- The crash is in slot 24 = `sub_828D9AC8` = `CopyFromBitmap`.
- Most likely a `this+0x18` (D3D resource pointer) null-deref or a `src->next_level` chain walk off a dangling pointer.
- Alternate: the XGTile blit helpers (`sub_82A57730` / `sub_82A56560`) can be reached with a `locked_desc[56] & 0x100`-set path that on PC (no real D3D locker) returns garbage via the locker thunks (`sub_82A44168`), leading to bogus pitch/offset math.
- Pre-check gate fails silently (returns 0) — so a crash inside the body means the gate passed (mips+w+h+depth all matched) and we are in the blit or the UV-copy tail.
- The UV-copy tail at the very end reads `*((float*)a2 + 12..18)` i.e. `src + 48..72` — if `src` is short, this runs off the end.

No crash-fix work performed by this agent per instructions.

## Files / tooling used

- MCP: `get_class_context`, `get_function_pseudocode`, `get_function_recomp`, `resolve_address`, `find_callers`, `get_string_refs`.
- Recomp scaffold: `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.67.cpp` (contains `sub_828D9AC8`).
- Pseudocode dir: `/Users/Ozordi/Downloads/LibertyRecomp/gta_iv/xex_excavation_retail/pseudocode/`.
- No files edited other than this deliverable.
