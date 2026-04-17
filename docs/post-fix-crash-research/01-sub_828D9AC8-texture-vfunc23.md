# sub_828D9AC8 — `rage::grcTextureXenon::vfunc[23]` (copy-into-existing-texture)

Agent 1 / 15 — crash research (research-only, no implementation).

- **Address**: `0x828D9AC8`
- **Size**: 0x2FC bytes (end = `0x828D9DC4`, verified by function boundary before `sub_828D9DC8`)
- **Vtable**: `rage::grcTextureXenon` slot 24 @ `0x8209612C` (slot 23 in the cxx numbering convention used in this repo's class-context tool)
- **RTTI inherits**: `grcTextureXenonProxy : grcTexture : pgBase : datBase`
- **Callers**: *no direct textual callers* — invoked strictly via the vtable slot (indirect dispatch from some higher-level `UpdateFromSystemMemoryTexture` / `CopyTo` caller yet to be named).
- **Failing string asset**: the error string at `0x82096390` (`"grcTextureXenon - Texture '%s' - invalid resolution %d by %d"`) is the only decompiled string tied to this class.

--------------------------------------------------------------------

## 1. High-level semantics (what this vfunc does)

This is **`grcTextureXenon::CopyFromTexture(const grcTextureXenon* src)`** (or "clone pixel data from another grcTextureXenon with identical geometry"). It:

1. **Validates geometry**: walks `this` and `src` and bails out (returns 0) unless:
   - mip-tail count matches
   - width matches (`this+0x1C == src+0x00`)
   - height matches (`this+0x1E == src+0x02`)
   - array-size/depth matches (`this+0x20 == src+0x04`)
2. **Per-mip-level loop**: for each mip level, for each face/slice/array-layer, it:
   - **Locks** a subresource on the destination `grcDeviceD3D` (via one of three ABI-compatible `Lock` shim variants `sub_82A44820 / 38 / 48`).
   - Computes the byte count in that subresource's miptail slab.
   - **`memcpy`s** the source mip bytes over the destination mip bytes (via the intrinsic PPC memcpy at `sub_82A00DC0`).
   - **Unlocks** (`sub_82A42E88`).
3. **Copies mipmap metadata**: finally overwrites `this->[0x24..0x3C]` from `src->[0x30..0x48]` (seven float32 lanes — looks like a texture-to-world transform / mip-bias / aniso-LOD tuning block the source carries).
4. Returns `1` on success, `0` on geometry mismatch.

Parameters (from prologue):
- `r3` → `r24 = this` (destination `grcTextureXenon*`)
- `r4` → `r19 = src`  (source `grcTextureXenon*`, validated same layout)
- Uses `r21` to iterate the linked-list chain hanging off `src+0x1C` (or `src+0x28` per the final `lwz r21,28(r21)`), one list per mip-chain. `r20` is the outer loop counter (mip-level index).

--------------------------------------------------------------------

## 2. Annotated pseudo-C++ walk (byte-by-byte)

Field names are inferred; I preserve register tracking so every store can be mapped back to the scaffold.

```cpp
// Stack frame: 416 bytes; r1 points to base of new frame.
// Layout used:
//   [r1+112]  u8    memoryType   (= src+0x08 low byte)   ("r22" cache)
//   [r1+120]  u32   lockPitch1D  (sub_82A44838 out + 0)     -- 2 words from Lock (w,addr)
//   [r1+124]  u32   lockAddr1D
//   [r1+128]  u32   lockPitch3D  (sub_82A44848 out + 0)     -- 3 words from volume Lock
//   [r1+132]  u32   lockH3D
//   [r1+136]  u32   lockAddr3D
//   [r1+144..]      misc scratch for downstream tiler (sub_82A57730 / sub_82A56560)
//   [r1+192]  struct subresourceDesc   (filled by sub_82A57088 from src device handle)

int grcTextureXenon::CopyFromTexture(grcTextureXenon* src)  // sub_828D9AC8
{
    grcTextureXenon* this_ = r24;  // preserved
    grcTextureXenon* src_  = r19;  // preserved

    // ---- GEOMETRY GATE (returns 0 on any mismatch) ----
    int srcMipTailDepth = sub_828D1C98(src);          // walks src->next chain @+0x1C
    if (this->field_9_u8       != srcMipTailDepth - 1) return 0;
    if (this->width_u16_at_28  != src->width_u16_at_0)  return 0;
    if (this->height_u16_at_30 != src->height_u16_at_2) return 0;

    int srcMipCount = sub_828D1898(src);              // walks src->next chain @+0x18
    if (this->field_32_u32 != srcMipCount + 1)        return 0;

    // ---- INIT LOCAL DESCRIPTOR ----
    u8 memoryType = (u8) src->field_8_u32;            // stored at [r1+112] -- r22
    [r1+120..136] = 0;                                // Lock-output cleared

    sub_828470E0();                                   // CRITICAL SECTION ENTER
                                                      //   uses tls[r13+1676]; ref-counts
                                                      //   into tls+1668 (D3D device lock)

    u32 deviceBase = this->device_at_24;              // r11 = *(this+0x18)*? no --
    u32 formatWord = *(u32*)(deviceBase + 48);
    r23 = (formatWord >> (32-21)) & 1;                // bit 10 of deviceFormat -- "is block-compressed"

    // ---- OUTER LOOP: mip level index `mipIdx` ----
    u8 mipIdx = 0;                                     // r20
    grcTextureXenon* curSrc = src_;                    // r21  (mip-chain cursor)
    while (curSrc != nullptr) {

        // Read the destination device subresource metadata:
        //   sub_82A57088(this->device, 0, &outDesc@[r1+192])
        //   (thin branch to sub_82A56C70 — queries D3D texture header bits)
        sub_82A57088(this->device_at_24, 0, &desc /*[r1+192]*/);

        int srcFaceCount = sub_828D1898(curSrc);      // mip-face count
        if (srcFaceCount + 1 > 0) {
            u8 faceIdx = 0;                            // r31

            // ---- INNER LOOP: face/slice `faceIdx` ----
            do {
                // Dispatch to one of three Lock shims based on memoryType:
                if      (memoryType == 1)
                    sub_82A44838(this->device, mipIdx, faceIdx,
                                 &[r1+120]@slicePair, 0, 0);          // 1D/linear
                else if (memoryType == 3)
                    sub_82A44848(this->device, mipIdx, faceIdx,
                                 &[r1+128]@volumeTriple, 0);          // 3D volume
                else
                    sub_82A44820(this->device, mipIdx, faceIdx,
                                 &[r1+120]@slicePair, 0, 0);          // 2D (default)

                // ---- Per-subresource byte-layout derivation ----
                u32 formatBits = desc.format;                         // [r1+208]
                if (formatBits & BIT(8)) {
                    // --- Block-compressed / tiled path ---
                    u32 bytesPerRow   = desc.u32_at_224;              // ref [r1+224]
                    u32 srcMipWords   = desc.u32_at_196;              // [r1+196]
                    ppc_trap_if_zero(bytesPerRow);
                    u16 srcH          = curSrc->height_at_12;
                    u32 rowsPerBlock  = srcMipWords / bytesPerRow;
                    u32 sliceBytes    = rowsPerBlock * srcH;          // r28

                    u32 clrBits = r23 & 0xFF;
                    if (memoryType == 3) {
                        // ... volume branch ... -> sub_82A56560(...)
                    } else {
                        // 2D/1D branch -> sub_82A57730(...)
                        // Builds a full GPU_UNTILED_DESC and calls the untiler.
                    }
                    sub_82A42E88(this->device, faceIdx);              // Unlock-ish (*)
                } else {
                    // --- Linear / non-tiled path — CRASH SITE ---
                    u32 copyDst;
                    if (memoryType == 3)
                        copyDst = [r1+136];            // volume Lock addr
                    else
                        copyDst = [r1+124];            // 2D/1D Lock addr

                    u32 copyBytes =
                        (u32)curSrc->field_2_u16
                      * (u32)curSrc->field_12_u16
                      * (u32)curSrc->field_14_u16;     // depth * w * h (packed-texel units)
                    void* copySrc = (void*)curSrc->bytes_ptr_at_16;

                    //  ------------------------------------------------
                    //  guest PC 0x828D9D04:  bl  sub_82A00DC0   // memcpy
                    //  guest PC 0x828D9D08:  cmplwi cr6,r22,1
                    //  guest PC 0x828D9D0C:  bne cr6, ...      <-- THIS IS LR AT CRASH
                    //  ------------------------------------------------
                    memcpy(copyDst, copySrc, copyBytes);   // <-- FAULTS

                    if      (memoryType == 1) sub_82A42E88(this->device, mipIdx, faceIdx);
                    else if (memoryType == 3) sub_82A42E88(this->device, faceIdx);
                    else                       sub_82A42E88(this->device, faceIdx);
                }

                curSrc = curSrc->next_at_24;    // r25 advance within mip face chain
                ++faceIdx;
            } while (faceIdx < sub_828D1898(curSrc) + 1);
        }

        curSrc = curSrc->next_at_28;            // r21 advance to next mip level
        ++mipIdx;
    }

    sub_82847120();                              // CRITICAL SECTION LEAVE
                                                 //   decrements tls+1668

    // ---- Copy 7 floats of mipmap metadata (src+0x30..+0x48 -> this+0x24..+0x3C) ----
    this->f32_at_36 = src->f32_at_48;
    this->f32_at_40 = src->f32_at_52;
    this->f32_at_44 = src->f32_at_56;
    this->f32_at_48 = src->f32_at_64;            // skip +60 intentionally
    this->f32_at_52 = src->f32_at_68;
    this->f32_at_56 = src->f32_at_72;

    return 1;
}
```

--------------------------------------------------------------------

## 3. Crash-site annotation — `sub_828D9AC8 + 0x244`

- `0x828D9D0C - 0x828D9AC8 = 0x244` (Python verified).
- `0x828D9D0C` is the instruction **immediately after** the `bl 0x82a00dc0` call at `0x828D9D04`, i.e. the compiler stored `LR = 0x828D9D0C` before branching into the intrinsic memcpy. LR at crash pointing at `+0x244` means we faulted **inside `sub_82A00DC0`**, and the parent/guest frame (what the game's stack walker would point to) is `sub_828D9AC8+0x244`.

So the PC at the actual access violation is inside the PPC memcpy (`sub_82A00DC0+0xB44`, host-symbol-reported), and the *guest call stack frame* that invoked it is this function at the linear-path memcpy.

### Which Lock returned the bad `dst = 0xBFF8`?

From the scaffold and the `memoryType` cache at `[r1+112]` (= `r22`), the immediate parent of the faulting memcpy reads `r3 = [r1+124]` when `r22 != 3`. `[r1+124]` is written by **`sub_82A44838`** (linear/1D Lock) **or** **`sub_82A44820`** (2D-mip Lock) — both write their output to the same scratch `r29 = r1+120` struct (`pitch@+0, addr@+4`).

- If `memoryType == 1` → called `sub_82A44838` → bad addr from `sub_82A44838`.
- If `memoryType == 0` (fall-through default) → called `sub_82A44820` → bad addr from `sub_82A44820`.

Both `sub_82A44838` and `sub_82A44820` tail-call `sub_82A44168`, which in turn calls **`sub_82A440A0`**. Inside `sub_82A440A0` the final "address" written back is computed at **`0x82A44154` (`bl sub_82A4A3C8`)** — an address-of-subresource-in-VRAM helper (tile-swizzle / mip-offset solver). Its output gets added to `*(r28)` (the base of the target VRAM slab) and stored. If either the base is wrong or the solver returns a small offset with no base applied, you get a sub-0x10000 "address" like `0xBFF8`.

Given `r4=0xD906A5C8` (the **source** pointer looks valid, well above 0x40000000) and `r3=0xBFF8` (the **destination**, grossly invalid), the lock-shim-returned destination `addr` got baked into `[r1+124]` with a zero or near-zero base. This indicates the destination `grcTextureXenon` has **no backing VRAM base pointer** (field `+0x14` probably null on PC, or the device's linear-buffer base in the Xenon path is not what the solver expects on host).

### Python-verified fault math

- `0xBFF8 + 0x100 = 0xC0F8` (dst end)
- memcpy's `subfic r6,r6,8` alignment prologue issues writes `r3+1 .. r3+8` via `stbu`; the very first write to the low-memory-guard page is at offset 8 → virtual addr `0xC000` → fault. Confirms crash "at guest 0xC000" is the intrinsic's first 8-byte-aligning prologue write.

### Relationship to the reverted fix

The reverted fix hooked `sub_82A42A38 / sub_82A42930 / sub_828BF1F8`. None of those three are called by this vfunc (callee list is closed — see section 5). The crash the revert exposed therefore is a **pre-existing latent bug**: `grcTextureXenon::CopyFromTexture` was always going to fault whenever the copy path is taken on a destination that does not own backing memory on host. The previous hooks must have been masking the call from ever reaching this vfunc.

--------------------------------------------------------------------

## 4. The 14 callees — classified

| Callee | Role | Notes |
|-|-|-|
| `__savegprlr_14` | Prologue helper | `r14..r31` GPR save/restore stub. |
| `sub_828D1C98` | **Metadata walk** | Counts nodes in a `->next@+0x1C` linked list. Used to compare mip-tail depth between src & dst. |
| `sub_828D1898` | **Metadata walk** | Counts nodes in a `->next@+0x18` linked list. Used for mip count and for per-mip face count. |
| `sub_828470E0` | **Synchronisation** | Critical-section enter: bumps ref-count at `tls[+0x684]` (1668) when current-tid already equals owner-tid, else CAS-swaps the owner. |
| `sub_82847120` | **Synchronisation** | Critical-section leave: decrements `tls[+0x684]`; on zero, clears owner. Pair of the above. |
| `sub_82A57088` | **Device descriptor read** | Thin forwarder → `sub_82A56C70` (decodes a GPU_TEXTURE_FETCH_CONSTANT block from `this->device_at_24` into stack struct `[r1+192..]`). |
| `sub_82A44820` | **Lock shim (2D)** | `b sub_82A44168` with `r4=0` (mip=0). 2D-surface subresource Lock variant. |
| `sub_82A44838` | **Lock shim (1D)** | `b sub_82A44168` (passing original r4). 1D/linear subresource Lock variant. |
| `sub_82A44848` | **Lock shim (3D)** | `b sub_82A441F8`. Volume-texture subresource Lock variant. Outputs 3 words (w, h, addr). |
| `sub_828D9328` | **Format/flag enum gate** | Compares r3 to `0x1A2C_AB5D` (`0x1A2C*0x10000+0xAB5D`); if equal sets r3 to `0x1A2C_AB60`. Reads like a fmt-enum remap used to pick a fast-path constant inside the tiler. (Small, leaf.) |
| `sub_82A00DC0` | **memcpy intrinsic** | Crashes here. Hot (577 callers). 8-byte-aligning prologue + 128-byte unroll body. |
| `sub_82A42E88` | **Unlock / state reset** | Masks r10 and r11 by `0xFFFFF000` then tail-calls `sub_82A4A600`. (Paired with the Lock shims.) |
| `sub_82A56560` | **Volume untiler** | Called on the `memoryType==3 & block-compressed` branch to perform 3D tile-swizzle copies. |
| `sub_82A57730` | **2D untiler** | Called on the `memoryType!=3 & block-compressed` branch. Full GPU_UNTILED_DESC consumer. |

Of these, **only three can hand a bogus `dst` into the memcpy**: `sub_82A44820`, `sub_82A44838`, `sub_82A44848` (the Lock trio). The branch taken at `0x828D9B84..0x828D9BDC` is what determines which. The scaffold shows they all share the internal helper `sub_82A440A0`, whose final address computation is `base + sub_82A4A3C8(...)`; inside the untiled branch and when the base (`*(r28) = *(r30->addr_word_0)`) is zero, the resulting address is simply the tile-solver output, which for small mips / small slices is easily in the 0x0000..0xFFFF range. That is exactly the failure mode that produced `r3 = 0xBFF8`.

--------------------------------------------------------------------

## 5. Callers

The `find_callers` query returns **none** — invocation is via the vtable slot only (`0x8209612C[24]`). To identify who actually hit this on the crash, the parent frame of `sub_828D9AC8` in the run log should be decoded from the crash LR chain in `/tmp/liberty_AFTER.log` (agents 2-15 likely cover that). From pattern: RAGE's resource-swap / streaming replacement path (`rage::pgStreamerTexture::Resolve` / `grcTextureFactory::CopyInto`) is the typical consumer of this vfunc.

--------------------------------------------------------------------

## 6. What is actually broken (high-confidence)

The Xenon Lock shims (`sub_82A44820/38/48` → `sub_82A44168/F8` → `sub_82A440A0`) are **Xbox-360-native** subresource-locators. They compute a tiled-VRAM address by `base + tile_offset_solver(...)`. On host, `base` is the guest-memory-backed linear buffer pointer stored on the device/texture; if that pointer is missing (null, or points outside the guest-heap mapping) and the `tile_offset_solver` returns a small offset like `0xBFF8` for a tiny 64x64 mip, the resulting `r3` is a guest address within the low-memory zero page and `memcpy` faults on its first 8-byte-aligning `stbu`.

The right remediation level is not this vfunc — it is ensuring `grcTextureXenon` destinations that travel through `CopyFromTexture` have a valid host-backed `+0x14` / `+0x18` / `+0x20` data pointer (the renderer's Lock path on host is what needs to write a real mapped address into `[r1+124]` or `[r1+136]`, or alternately the Lock shim trio must be the hook target, overriding the tiled address calculation with a flat host pointer).

--------------------------------------------------------------------

## 7. Useful offsets (Python-verified, for future hook authors)

| Offset from func | Meaning |
|-|-|
| `+0x000` | entry |
| `+0x0EC` | `bl sub_82A44838` — 1D Lock shim |
| `+0x108` | `bl sub_82A44848` — 3D Lock shim |
| `+0x114` | `bl sub_82A44820` — 2D Lock shim |
| `+0x240` | `bl sub_82A00DC0` — memcpy |
| `+0x244` | **post-memcpy return — crash LR** |
| `+0x2FC` | end of function (`b __restgprlr_14`) |

All math via Python: `0x828D9D0C - 0x828D9AC8 = 0x244`; `0xBFF8 + 0x100 = 0xC0F8`; fault-first-write `0xBFF8 + 8 = 0xC000`. Confirmed.
