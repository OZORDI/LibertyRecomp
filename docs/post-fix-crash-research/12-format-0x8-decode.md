# Format 0x8 Decode - Agent 12 Research

## Summary

**Xenos raw format 0x8 = `TextureFormat::k_8_A`** — a single-channel 8-bit
format (the "A" variant stores the value in the alpha component; sampled
identically to `k_8` with the single component replicated). Source of truth:
`a2xx_sq_surfaceformat` enum in Xenia's `xenos.h`.

**The RGBA8 fallback is WRONG for this format.** It produces a 4x byte-count
mismatch and almost certainly drives the downstream pointer arithmetic into
undefined territory.

## Where the log is emitted

`/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/gpu/video.cpp:3974-3978`

```
static RenderFormat ConvertFormat(uint32_t format) { switch (format) {
   ... D3DFMT_* packed 32-bit codes ...
   default:
      LOGF_WARNING("Unknown format {:#x} - using RGBA8 fallback\n", format);
      return RenderFormat::R8G8B8A8_UNORM;
} }
```

`ConvertFormat` callers (all in video.cpp):
- `CreateTexture`       (3939 / used at 4005)
- `CreateIndexBuffer`   (used at 4087 - formats here are tiny codes like
                         `D3DFMT_INDEX16 = 1`, `D3DFMT_INDEX32 = 6`)
- `CreateSurface`       (4134) - **render target path**
- Two additional constructor sites (9162, 9245, 9553)

## The format-code system is bimodal

`video.h:140-163` (`enum GuestFormat`) contains TWO kinds of values:

| Kind | Example | Bit-width |
|-|-|-|
| D3DFMT-packed (X360 GPUCONSTANTS) | `D3DFMT_A8R8G8B8 = 0x18280186` | 32-bit packed |
| Raw enum | `D3DFMT_INDEX16 = 1`, `D3DFMT_INDEX32 = 6` | 1-byte |

So the switch already accepts raw enum-index codes for the index-buffer path.
A raw `0x8` fits the second category exactly and maps cleanly to
`TextureFormat::k_8_A = 8` in the Xenos enum.

## Xenos `a2xx_sq_surfaceformat` / `TextureFormat` enum

From `tools/XeniOS-xenios/src/xenia/gpu/xenos.h:489`:

| Value | Name | bpp |
|-|-|-|
| 0 | k_1_REVERSE | 1 |
| 1 | k_1 | 1 |
| 2 | k_8 | 8 |
| 3 | k_1_5_5_5 | 16 |
| 4 | k_5_6_5 | 16 |
| 5 | k_6_5_5 | 16 |
| 6 | k_8_8_8_8 | 32 |
| 7 | k_2_10_10_10 | 32 |
| **8** | **k_8_A** | **8** |
| 9 | k_8_B | 8 |
| 10 | k_8_8 | 16 |
| ... | ... | ... |

Confirmed bpp from Xenia's `tools/XeniOS-xenios/src/xenia/gpu/texture_info_formats.inl:9`:

```
FORMAT_INFO(k_8_A, kResolvable, 1, 1, 8),
```

Triangl3l's comment (`xenos.h:498-509`): `k_8_A` is "possibly similar to k_8,
but may be storing alpha instead of red when resolving/memexporting [...]
From the point of view of sampling, it should be treated the same as k_8".

This is a **single 8-bit alpha-only channel** — exactly the kind of surface
used for small 1D lookup textures, LUTs, glyphs, or monochrome masks. A
**240x1 dimension is highly consistent with a 1D 8-bit LUT / gradient table**.

## Stride mismatch math (Python-verified)

Python:
```
w, h = 240, 1
bytes_k8a = w * h * 8 // 8            # 240 bytes (0xf0)
bytes_rgba8 = w * h * 32 // 8         # 960 bytes (0x3c0)
overrun = bytes_rgba8 - bytes_k8a     # 720 bytes (0x2d0)
ratio = bytes_rgba8 / bytes_k8a       # 4.0x
```

If the guest/caller allocates `240 * 1 * 1 == 240` bytes for what it believes
is a single-channel 8bpp 240x1 surface, but the host backing texture is created
as RGBA8 (4 bpp), then:

- Host creates a texture expecting 960 bytes of source data per upload.
- Any upload path that reads `host_stride * height == 960` bytes from a
  `240`-byte guest allocation will walk 720 bytes past the end of the guest
  buffer (OOB read of arbitrary neighboring heap data).
- Conversely, any `copyFromGuest`-style path writing to a guest-sized
  240-byte region with a host-derived 960-byte source will OOB **write**
  720 bytes.

**The fallback is exactly the "stride miscalculation leading to a bogus dst
pointer" scenario described in the crash hypothesis.**

## The correct handling

Format 0x8 (k_8_A) must map to a **single-channel 8-bit host format**:

- `RenderFormat::R8_UNORM` — matches what the existing `case D3DFMT_A8:`
  arm returns. Use it. The component-mapping swizzle for an A-channel
  semantic is `(0,0,0,R)` or `(R,R,R,R)` depending on how the shader samples
  it; k_8_A is routinely sampled as `.wwww` (swizzle 111W per the inline
  comment), so mapping the R texel to the sampled alpha (`SWIZZLE_A`) via
  `RenderComponentMapping(ZERO, ZERO, ZERO, R)` is the safest default,
  matching `.wwww` swizzling paths in the game's fetch constants.

## Recommended fix sketch (for separate agent)

In `video.cpp:ConvertFormat`, add raw-Xenos cases **before** the default
fallback:

```
// Raw Xenos TextureFormat enum codes (a2xx_sq_surfaceformat)
case 2:   // k_8
case 8:   // k_8_A
case 9:   // k_8_B
    return RenderFormat::R8_UNORM;
case 10:  // k_8_8
    return RenderFormat::R8G8_UNORM;
case 6:   // k_8_8_8_8  (raw enum, distinct from packed D3DFMT_A8R8G8B8)
    return RenderFormat::R8G8B8A8_UNORM;
```

**Companion change needed**: if the surface is being addressed as an 8bpp
source/dst elsewhere (stride calc in `copyFromGuest`/upload), the fix must
propagate through. Masking via RGBA8 fallback is exactly what is papering
over the crash cause now.

## Mapping of raw Xenos vs D3DFMT-packed (for future agents)

| Raw Xenos value | TextureFormat name | D3DFMT-packed equivalent (if any) |
|-|-|-|
| 2 | k_8 | (part of D3DFMT_L8 = 0x28000102) |
| 6 | k_8_8_8_8 | D3DFMT_A8R8G8B8 = 0x18280186 |
| 8 | k_8_A | D3DFMT_A8 = 0x4900102 |
| 18 | k_DXT1 | D3DFMT_DXT1 = 0x1A200152 |
| 20 | k_DXT4_5 | D3DFMT_DXT4 = 0x1A200154 |

Notice raw 6 and packed 0x18280186 both exist. The game is handing in both
kinds at different call sites — consistent with the bimodal enum in video.h.

## Hypothesis for the crash

Caller path was almost certainly **a render-target or surface-descriptor
creation site** that passed a raw `ColorFormat` value (not a packed D3DFMT).
`ColorFormat` does include `k_8_A = 8` as a resolvable RT format
(`xenos.h:589`). With `RGBA8_UNORM` as the host format but the guest EDRAM /
VRAM region sized for 8bpp, the resolve/copy-back path computes a destination
stride 4x what the guest expects and tramples whatever follows.

## File references (absolute)

- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/gpu/video.cpp` — lines
  3939-3978 (`ConvertFormat`), 3976 (the log), 4005/4087/4134/9162/9245/9553
  (call sites)
- `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/gpu/video.h` — lines
  140-163 (`GuestFormat` bimodal enum)
- `/Users/Ozordi/Downloads/LibertyRecomp/tools/XeniOS-xenios/src/xenia/gpu/xenos.h`
  lines 489-609 (`TextureFormat`, `ColorFormat`)
- `/Users/Ozordi/Downloads/LibertyRecomp/tools/XeniOS-xenios/src/xenia/gpu/texture_info_formats.inl`
  line 9 (k_8_A bpp=8 authoritative value)
