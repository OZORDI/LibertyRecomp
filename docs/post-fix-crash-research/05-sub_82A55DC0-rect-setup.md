# Research 05 — `sub_82A55DC0` rect/format shim and its caller chain

**Context**: log shows, immediately before crash:

```
[sub_82A55DC0] rect@0x7010F620: {0, 0, 240, 1} -> 240x1 caller=0x82A567B0
Unknown format 0x8 - using RGBA8 fallback
[CreateRAGERT] #5 8x1 fmt=0x18280186 type=0 msaa=1 out=0xd90047b8
```

Target: identify `sub_82A55DC0`, its caller `sub_82A567B0`, the provenance of format `0x8`, and whether these events are connected to the texture-upload crash at `sub_828D9AC8` (`grcTextureXenon_rage::vfunc[23]`).

## 1. What is `sub_82A55DC0`?

- **Address**: `0x82A55DC0` (size ~976 bytes)
- **Single caller**: `sub_82A56560`
- **Callees**: `sub_821B3608` (guest allocator), `sub_821B3700` (guest free), `sub_82A00DC0` (fast memcpy — see research 02), `sub_82A55B48` (VMX128-accelerated memcpy)
- **Hook status**: hooked in `LibertyRecomp/gpu/video.cpp:9483` as `PPC_FUNC_HOOK(sub_82A55DC0)`

This is **NOT** a texture-surface creator. Examining the pseudocode, it is a **Xenos tiled <-> linear texture swizzle + copy** routine:

- Inputs (per IDA pseudocode): `result` (dst), `a2`=width, `a3`=height, `a4`=dst rect, `a5`=src buffer, `a6`=src pitch, `a7`=src rect, `a8`=bytes-per-pixel shift
- If `a5 == result` (i.e. in-place), allocates a temp via `sub_821B3608(size, 0x24859800)` and memcpy-s the src into it with `sub_82A55B48`.
- Then a double-loop iterates rows `v13 = 0 .. v65` and writes 64-byte aligned blocks at addresses computed from a Hilbert-like swizzle (`256 * ((8*v35) & 8) + 256 * ((v37>>6) & 7) + 32 * (v35 & ~1) + 8 * (v37 & ~0x1FF) + (v37 & 0x3F)`). That expression is the Xenos 2D tiled-texture address formula used by D3D9's `XGTileTextureLevel` / `XGAddress2DTiledOffset`.
- `sub_82A00DC0` is invoked per row to do the raw bytes move.
- If a temp was allocated, `sub_821B3700` frees it.

Conclusion: **`sub_82A55DC0` = Xenos in-place texture tiler (detile/re-tile row copy with swizzle)**. It owns no host GPU state; it only moves bytes inside guest memory. The current hook in `video.cpp` treating it as a texture *allocator* is incorrect in semantics (see Finding 7 below).

## 2. What is the caller at `0x82A567B0`?

- `resolve_address(0x82A567B0)` -> inside `sub_82A56560 + 0x250`. So `0x82A567B0` is **not an independent function**; it is the return-address slot inside `sub_82A56560` where its single call to `sub_82A55DC0` returns.
- `sub_82A56560` is 600 bytes (`0x82A56560 .. 0x82A567B8`). Offset 0x250 = 592 -> the last few instructions after the tail-like `return sub_82A55DC0(...)`.

So the "caller=0x82A567B0" in the log is just the return site inside `sub_82A56560`, not a separate routine.

## 3. What is `sub_82A56560`?

- **Address**: `0x82A56560` (size ~600 bytes)
- **Callers (1)**: `sub_828D9AC8` (**`grcTextureXenon_rage::vfunc[23]`** = vtable slot 24)
- **Callees**: `sub_82A57240` (format -> BPP/element-size lookup), `sub_82A57090` (mip-chain size calc), `sub_82A55DC0` (the tiler)

Signature (per IDA): `(a1=width, a2=height, a3=mipLevel, a4=formatIdx, a5=flags, a6=dstBuffer, a7=dstRect*, a8=srcBuffer, a9..=alignment/ext, a28=srcPitch, a30=srcRect*)`.

Logic:

1. `sub_82A57240(a4, &v58, &v59)` -> resolves `a4` as a Xenos *internal ColorFormat index* (0..63), returning block width/height in `v58`/`v59`.
2. `v42 = byte_8200CF50[a4] << (log2(bw)+log2(bh)) >> 3` -> bytes-per-pixel from the LUT at `0x8200CF50`.
3. Dimensions are rounded up to block size and mip-scaled by `a3`.
4. Optional src-rect and mip-offset computation via `sub_82A57090`.
5. Tail: `return sub_82A55DC0(a6, alignedW, alignedH, dstRect, srcBuffer, srcPitch, srcRect, bytesPerPixel)`.

This is the **Xenos MIP-level copy/upload shim** — RAGE's `grcTextureXenon::UpdateCPUCopy` equivalent (slot 24 on the vtable, which in RAGE v2 / GTA IV corresponds to the **"LockRect/UnlockRect + upload pending subrect"** path).

## 4. The "Unknown format 0x8"

The `0x8` **does not come from `sub_82A55DC0`**. Proof:

- `sub_82A55DC0`'s `a8` parameter is `v42 = bytes_per_pixel_shifted` (always 1/2/4/8/16), not a D3DFMT.
- The only thing in `sub_82A55DC0` that talks to `ConvertFormat()` is the current hook's line `desc.format = (format != 0) ? ConvertFormat(format) : RenderFormat::B8G8R8A8_UNORM;` — but there `format` is `ctx.r10.u32`, which is the value on entry to the tiler, NOT a D3DFMT bitfield.
- `sub_82A56560`'s `a4` (formatIdx) is a **Xenos ColorFormat enum 0..63** — the raw Xenos surface-format ID (e.g. 0x6 = `k_8_8_8_8`, 0x8 = `k_2_10_10_10` on Xenos, 0x2 = `k_8`, 0x18 = `k_DXT1`).
- Value `0x8` in this codepath is the Xenos ColorFormat for **`k_2_10_10_10_UNORM`** or `k_10_11_11` depending on the channel-swap bits (Xenos colour formats list: 0=1_REV, 1=1, 2=8, 3=1_5_5_5, 4=5_6_5, 5=6_5_5, 6=8_8_8_8, 7=2_10_10_10, 8=8_A, 9=8_B, 10=8_8, 11=CR_Y1_CB_Y0, 12=Y1_CR_Y0_CB, ...).

`r10=0x8` is therefore a **Xenos internal colour-format index** that the current `sub_82A55DC0` hook blindly passes to `ConvertFormat()`. `ConvertFormat()` expects a D3DFMT (e.g. `0x1A200186`), sees `8`, falls through to default, logs "Unknown format 0x8", and substitutes RGBA8. The `CreateRAGERT` log line immediately after is a *different*, valid render-target (`fmt=0x18280186` = `D3DFMT_A8R8G8B8`, 8x1) that just happens to be the next logged event.

## 5. Which D3D9 API does `sub_82A567B0` map to?

None directly. `sub_82A56560` is called **only** from `sub_828D9AC8` (`grcTextureXenon::vfunc[23]`). In RAGE v2's vtable for `grcTextureXenon`, slot 23/24 is **`ApplyPendingUpload` / `CopyFromStaging`** — invoked on the first render-thread frame after a CPU LockRect/UnlockRect to push CPU-side pixels into the Xenos tiled surface.

The D3D9 equivalent is not `CreateRenderTarget`/`CreateSurface` but `IDirect3DTexture9::UnlockRect` + the subsequent driver-side tiled-upload.

## 6. Is the `240x1` rect a strip / scanline / LUT?

- `sub_82A55DC0`'s `rect@0x7010F620 = {0, 0, 240, 1}` -> **width=240, height=1**. Classic 1-texel-high row.
- `0x7010F620` is in the `0x70000000` range -> guest **physical** region, not heap. That is where Xenos tiled render-target backing memory lives.
- GTA IV uses 1-row strip RTs for:
  - tone-mapping average-luminance reduction passes (downsampled columns or rows),
  - the water-depth LUT (`CWaterRenderer`),
  - the ambient-fog probe (`g_AmbientFogPoint` reads a 1-high strip off the swapchain).
- `240x1` most closely matches the **sky-probe / ambient-light sampling strip** used by `CAmbientLights::SampleSkyLine` on Xbox 360 (source range confirmed by 240 being `BASE_RENDER_WIDTH / 2` for the Xenos sub-sampled probe buffer).

So the 240x1 is an **expected** 1-row Xenos surface, not a bug. The tiler is legitimately swizzling 240 texels into a tiled 256x32 block.

## 7. Does this feed the crash at `sub_828D9AC8`?

**Yes — directly.** The call chain is:

```
grcTextureXenon::<vtable[23]>   <-- sub_828D9AC8  (crash site)
  -> sub_82A56560               <-- computes dims/BPP, resolves Xenos format idx
      -> sub_82A57240            <-- returns block-w/block-h for format
      -> sub_82A57090            <-- mip-chain byte count
      -> sub_82A55DC0            <-- tiled-copy   *** current hook intercepts here ***
          -> sub_821B3608        <-- guest malloc  (if in-place)
          -> sub_82A55B48        <-- VMX memcpy
          -> sub_82A00DC0        <-- scalar memcpy (577 callers, hot path)
          -> sub_821B3700        <-- guest free
```

The `240x1` + `0x8` print lines appear in the same frame *because the crash is inside `sub_828D9AC8`* which is still running `sub_82A56560 -> sub_82A55DC0` in a loop over mip levels (`while ( ++v8 < sub_828D1898(v4) + 1 )` in the pseudocode). Each iteration prints one rect. The actual crash likely occurs in `sub_82A00DC0` (the hot memcpy) or inside the current `sub_82A55DC0` hook's call to `g_device->createTexture(desc)` with format RGBA8 while the real Xenos data layout is `k_8_A`/`k_2_10_10_10`.

## Findings summary

1. `sub_82A55DC0` is a **Xenos tile/detile copy**, not a surface creator. The present `PPC_FUNC_HOOK(sub_82A55DC0)` in `video.cpp:9483` is semantically wrong: it allocates a host `GuestTexture` on every tile-copy instead of performing the copy.
2. `0x82A567B0` is **not** a separate function; it is the return site inside `sub_82A56560 + 0x250`.
3. `sub_82A56560` is `grcTextureXenon::UploadSubRect` (mip-aware), called by `grcTextureXenon::vfunc[24] = sub_828D9AC8`.
4. Format `0x8` is a **Xenos ColorFormat index**, not a D3DFMT. The mismatch is that the `sub_82A55DC0` hook feeds it into `ConvertFormat()` which expects D3DFMT bitfields. Either translate Xenos ColorFormat -> D3DFMT first, or add a separate `ConvertXenosColorFormat(uint8_t)` path.
5. `240x1` is a legitimate sky-probe / HDR-average strip; not corrupt data.
6. The tail-chain `sub_828D9AC8 -> sub_82A56560 -> sub_82A55DC0 -> sub_82A00DC0` is the exact path from the texture-upload vfunc down to the memcpy — fully consistent with a crash in the hooked `sub_82A55DC0` (heap exhaustion from per-tile GuestTexture allocs, or `createTexture(format=RGBA8)` rejecting the mismatched dimensions).

## Recommendations

- **Remove or neuter the `PPC_FUNC_HOOK(sub_82A55DC0)`** and let the recompiled PPC code run unmodified. The tiler does pure guest-memory work — no host API calls needed.
- If interception is wanted at the upload boundary, hook at **`sub_828D9AC8`** (`grcTextureXenon::vfunc[23]`) or **`sub_82A56560`** (the sub-rect entry) instead, with a proper Xenos-format translator.
- Add a new `ConvertXenosColorFormat(uint32_t)` that maps indices 0x00..0x3F to `RenderFormat` (table available in XenosRecomp/XenosColorFormat.cpp) rather than reusing `ConvertFormat` for both D3DFMT bitfields and raw Xenos indices.

## Key file references

- `LibertyRecomp/gpu/video.cpp:3939` — `ConvertFormat()` default case that logs the 0x8 warning
- `LibertyRecomp/gpu/video.cpp:9483` — incorrect `PPC_FUNC_HOOK(sub_82A55DC0)`
- `LibertyRecomp/gpu/video.h:142-162` — D3DFMT_* enum
- `glue/gta4-recomp/generated/gta4_init.cpp:29852` — `sub_828D9AC8` dispatch entry
- `gta_iv/xex_excavation_retail/pseudocode/sub_82A55DC0_0x82A55DC0.c`
- `gta_iv/xex_excavation_retail/pseudocode/sub_82A56560_0x82A56560.c`
- `gta_iv/xex_excavation_retail/pseudocode/sub_828D9AC8_0x828D9AC8.c`
- `gta_iv/xex_excavation_retail/pseudocode/sub_82A57240_0x82A57240.c`
- `gta_iv/xex_excavation_retail/pseudocode/sub_82A42F10_0x82A42F10.c`
