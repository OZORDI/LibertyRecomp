# UnleashedRecomp — Texture Pipeline Study (Agent 2 / 5)

Source of truth: `Reference Projects/UnleashedRecomp-main/UnleashedRecomp/gpu/video.cpp`
(7882 lines) and `video.h` (413 lines).

All line numbers below refer to those two files unless otherwise noted.

---

## 1. Texture / Surface struct layout

From `video.h` the entire texture object hierarchy is host-only — the pointer
returned from `CreateTexture` is a native `GuestTexture*` (allocated on the
guest-addressable user heap so the guest can hold its virtual address), not a
Xenos D3D texture header. The guest never reads/writes its fields; every field
is host-side C++.

```
video.h:81-117   GuestResource       { unused, be<refCount>, ResourceType }
video.h:137-151  GuestBaseTexture    { unique_ptr<RenderTexture> textureHolder,
                                       RenderTexture* texture,
                                       unique_ptr<RenderTextureView> textureView,
                                       width/height, RenderFormat format,
                                       uint32_t descriptorIndex,
                                       RenderTextureLayout layout }
video.h:153-163  GuestTexture        { + depth, viewDimension, mappedMemory,
                                       unique_ptr<RenderFramebuffer> framebuffer,
                                       unique_ptr<GuestTexture> patchedTexture,
                                       unique_ptr<GuestTexture> recreatedCubeMapTexture,
                                       GuestSurface* sourceSurface }
video.h:205-211  GuestSurface        { + guestFormat, per-RT framebuffers map,
                                       sampleCount,
                                       set<GuestTexture*> destinationTextures }
video.h:165-169  GuestLockedRect     { be<uint32_t> pitch; be<uint32_t> bits; }
```

Key adoption point: **`mappedMemory` is the ONLY piece of guest-visible memory
per texture.** It is lazily allocated from `g_userHeap.AllocPhysical` on first
`LockTextureRect`, sized `pitch * height`. Everything else (texture handle,
view, descriptor index, layout, format) is host-pointer-reachable only from the
host-side `GuestTexture*`.

## 2. Hook coverage — guest sub_ → host

Collected from the hook block at lines **7798-7882** (texture-relevant rows):

| guest addr | host function | notes |
|-|-|-|
| sub_82BD99B0 | CreateDevice | installs render-state/sampler fn tables |
| sub_82BE6230 | DestructResource | polymorphic across ALL resource types |
| sub_82BE9498 | CreateTexture | Texture / VolumeTexture (`type==17`) |
| sub_82BE95B8 | CreateSurface | RenderTarget / DepthStencil |
| sub_82BE9300 | LockTextureRect | returns host-allocated scratch buffer |
| sub_82BE7780 | UnlockTextureRect | enqueues `RenderCommandType::UnlockTextureRect` |
| sub_82BE96F0 | GetSurfaceDesc | writes width/height into guest desc |
| sub_82BDD330 | GetBackBuffer | `g_backBuffer->AddRef(); return g_backBuffer;` |
| sub_82BE9818 | SetTexture | controller-prompt patched-texture override, then enqueues |
| sub_82BF6400 | StretchRect | records surface→texture copy, deferred to pending queue |
| sub_82BDD9F0 | SetRenderTarget | + SetDefaultViewport |
| sub_82BDDD38 | SetDepthStencilSurface | + SetDefaultViewport |
| sub_82C003B8 | D3DXFillTexture | hardcoded 1×1 R8 (function==0x82BA2150) |
| sub_82C00910 | D3DXFillVolumeTexture | 3D + handcrafted mip0 + mip1 |
| sub_82E43FC8 | MakePictureData | DDS/STB loader for game `GuestPictureData` assets |
| sub_82BFFF88 | GUEST_FUNCTION_STUB | D3DXFilterTexture (GPU mip gen — stubbed!) |
| sub_82BE9C98 / sub_82BEA308 / sub_82BEA018 / sub_82BEA7C0 / sub_82BE9B28 | GUEST_FUNCTION_STUB | remaining Xenos texture helpers |

**There is no UpdateSurface, no GenerateMipmaps, no CreateDepthStencilSurface.**
Depth stencils come through `CreateSurface`; mipmap generation is expected to
be built into the source DDS or produced manually (see `D3DXFillVolumeTexture`).

## 3. Tiling / detiling strategy — **THERE IS NONE**

`grep -in "XGUntile|XGTile|detile|Untile|XGGetTextureLayout|XGSetTextureHeader"`
over the entire UnleashedRecomp tree returns **zero matches**.

Design assumption: the guest Sonic code calls `LockTextureRect`, writes LINEAR
pixel bytes into the buffer returned by the host, then calls `UnlockTextureRect`.
The host treats those bytes as already-linear and blits them straight into an
upload buffer (`ProcUnlockTextureRect`, lines **2190-2206**). Any Xenos 32×32
tile swizzle would simply never occur because the source is CPU-produced linear
data (`D3DLOCKED_RECT::pBits` semantics, not the `D3DTEXTURE::data` GPU ptr).

For *already-tiled* asset bundles Unleashed side-steps the problem entirely by
shipping **DDS** assets and a zstd shader cache; `MakePictureData` (line 5852)
and `LoadTexture` (line 5624) only accept DDS or stb-loadable formats, never
raw Xenos texture memory.

## 4. Format conversion table

`ConvertFormat` — `video.cpp:3065-3093`. Guest D3DFMT → host RenderFormat:

| guest constant | value | host RenderFormat |
|-|-|-|
| D3DFMT_A16B16G16R16F[_2] | 0x1A22AB60 / 0x1A2201BF | R16G16B16A16_FLOAT |
| D3DFMT_A8B8G8R8 / A8R8G8B8 / X8R8G8B8 | 0x1A200186 / 0x18280186 / 0x28280086 | R8G8B8A8_UNORM |
| D3DFMT_D24FS8 / D24S8 | 0x1A220197 / 0x2D200196 | D32_FLOAT |
| D3DFMT_G16R16F[_2] | 0x2D22AB9F / 0x2D20AB8D | R16G16_FLOAT |
| D3DFMT_INDEX16 / INDEX32 | 1 / 6 | R16_UINT / R32_UINT |
| D3DFMT_L8[_2] | 0x28000102 / 0x28000002 | R8_UNORM |
| default | - | assert + R16G16B16A16_FLOAT |

Swizzle fix-ups set in `CreateTexture` (line **3123-3135**):

- `D24FS8`/`D24S8`/`L8`/`L8_2` → componentMapping = (R,R,R,ONE)
- `X8R8G8B8` → componentMapping = (G,B,A,ONE)

DXGIFormat → RenderFormat (DDS ingest) is the massive switch at **5466-5622**
covering every R/G/B/A/BC1-7/D/S variant.

## 5. Lock / Unlock semantics

### LockTextureRect (lines 2163-2178)

```cpp
static uint32_t ComputeTexturePitch(GuestTexture* texture) {
    return (texture->width * RenderFormatSize(texture->format)
            + PITCH_ALIGNMENT - 1) & ~(PITCH_ALIGNMENT - 1);
}

static void LockTextureRect(GuestTexture* texture, uint32_t, GuestLockedRect* lockedRect) {
    uint32_t pitch = ComputeTexturePitch(texture);
    uint32_t slicePitch = pitch * texture->height;
    if (texture->mappedMemory == nullptr)
        texture->mappedMemory = g_userHeap.AllocPhysical(slicePitch, 0x10);
    lockedRect->pitch = pitch;
    lockedRect->bits  = g_memory.MapVirtual(texture->mappedMemory);
}
```

- `PITCH_ALIGNMENT = 0x100` (line 1341).
- `PLACEMENT_ALIGNMENT = 0x200` (line 1342).
- 16-byte `AllocPhysical` alignment (0x10).
- Pitch is rounded up to the nearest 256 bytes (D3D12/Vulkan copyTextureRegion
  requirement; Python-verified: 512-wide RGBA8 → pitch=0x800, slice=0x100000).
- Never frees `mappedMemory` on Unlock — kept alive for subsequent locks.
  Freed only in `DestructTempResources` (line 681-682) when the GuestResource
  is finally reaped.
- `lockedRect->bits` is a **guest virtual address** so the guest code can
  memcpy into it using PPC `lwzx/stwx`.

### UnlockTextureRect (lines 2180-2206)

Enqueues to `g_renderQueue` (thread hopper). `ProcUnlockTextureRect`:

1. `AddBarrier(texture, COPY_DEST); FlushBarriers();`
2. Allocate `g_uploadAllocators[g_frame].allocate(slicePitch, PLACEMENT_ALIGNMENT)`.
3. `memcpy(allocation.memory, texture->mappedMemory, slicePitch);`
4. `commandLists[g_frame]->copyTextureRegion(Subresource(0), PlacedFootprint(...))`.

One flat copy per unlock. No mip handling — slice pitch × height, subresource 0.
Multi-mip pushes go through the DDS loader, not the Lock/Unlock path.

## 6. Update flow — guest memcpy → host GPU

```
  game calls sub_82BE9300(tex, 0, &rect)
  → LockTextureRect: AllocPhysical(pitch*height, 0x10)
    → rect.bits = guest-VA(mappedMemory)
    → rect.pitch = (width*bpp + 0xFF) & ~0xFF

  game writes linear bytes into guest memory @ rect.bits
  (no detiling, no swizzle — already-CPU-linear)

  game calls sub_82BE7780(tex)
  → UnlockTextureRect: enqueue cmd (render thread)

  render thread: ProcUnlockTextureRect
  → barrier → upload alloc → memcpy → copyTextureRegion(tex, 0, PlacedFootprint)

  subsequent SetTexture: AddBarrier(tex, SHADER_READ)
  → setDirty(g_sharedConstants.texture2DIndices[slot] = tex->descriptorIndex)

  pipeline bind: commandList->setGraphicsDescriptorSet(g_textureDescriptorSet, 0..2)
  (bindless — all textures live in a single heap indexed by descriptorIndex)
```

The descriptor is **bindless** (line 337-384): `g_textureDescriptorSet` is one
set populated via `g_textureDescriptorSet->setTexture(descriptorIndex, ...)`,
and shaders index it with `texture2DIndices[slot]` in `g_sharedConstants`.
That removes any per-draw descriptor binding overhead and makes SetTexture
essentially constant-buffer writes plus barriers.

## 7. GPU-VA math — there is no replacement because there's nothing to replace

Xenos's `sub_82A4A3C8`-style rowPitch/slicePitch/baseAddress calculator
encodes the GPU **tiled** layout. Unleashed **never calls that logic**. The
pitch math in Unleashed is the trivial CPU-linear one (PITCH_ALIGNMENT=0x100),
plus the DDS slice walker at **5681-5704**:

```cpp
rowPitch    = ((width + blockWidth - 1) / blockWidth) * bitsPerPixelOrBlock;
srcRowPitch = (rowPitch + 7) / 8;
dstRowPitch = (srcRowPitch + 0xFF) & ~0xFF;
rowCount    = (height + blockHeight - 1) / blockHeight;
```

This is D3D12 placed-footprint math, not Xenos GPU-VA math. The host never
reads guest GPU memory; the guest hands it CPU-linear bytes.

## 8. CreateTexture / CreateSurface lifecycle

### CreateTexture (lines 3095-3153)

1. `AllocPhysical<GuestTexture>(type==17 ? VolumeTexture : Texture)` — host
   ctor runs on guest-addressable memory, ref=1.
2. Fill `RenderTextureDesc` (w/h/d/mipLevels=levels, arraySize=1, format).
3. Flags: `D32_FLOAT → DEPTH_TARGET; usage!=0 → RENDER_TARGET; else NONE`.
4. `g_device->createTexture(desc)` + `createTextureView(viewDesc)`.
5. `texture->descriptorIndex = g_textureDescriptorAllocator.allocate();`
6. `g_textureDescriptorSet->setTexture(descriptorIndex, texture, SHADER_READ, view)`.
7. Return raw `GuestTexture*`. Guest stores this pointer wherever it kept its
   D3DTEXTURE9 handle before.

### CreateSurface (lines 3184-3221)

Same pattern. MSAA sample count is driven by `Config::AntiAliasing` (only if
`multiSample != 0`). Depth stencils also get a SHADER_READ view and descriptor
index so the game can sample them as textures.

### Destruction (lines 670-737)

`DestructResource` enqueues `destructResource.resource`. On the render thread,
`DestructTempResources`:

- Texture: `g_userHeap.Free(mappedMemory); descriptorAllocator.free(descriptorIndex);`
  also frees `patchedTexture` and `recreatedCubeMapTexture` descriptors.
- Surface: `descriptorAllocator.free(descriptorIndex);`
- Then `~Type()` then `g_userHeap.Free(resource)`.

## 9. Asset ingest path (MakePictureData)

`MakePictureData` (line 5852) is the game-asset entry:

1. Construct transient `GuestTexture texture(ResourceType::Texture)`.
2. Call internal `LoadTexture(texture, data, dataSize, {})`:
   - If DDS (`ddspp::decode_header`): walk arraySlice × mipSlice, produce
     `Slice[]` with src/dst offsets, allocate one upload buffer, memcpy
     row-by-row (handles pitch alignment mismatch), then one
     `copyTextureRegion` per (subresource, face). Force-cubemap path for
     the Cool Edge whale asset (hash 0x160E9E250FDE88A9) duplicates face 0
     across faces 1-5.
   - Else `stbi_load_from_memory` → RGBA8 → single copyTextureRegion.
3. `DiffPatchTexture`: XXH3 hash lookup into `g_buttonBcDiff`; if match,
   produces `patchedTexture` via recursive `LoadTexture`.
4. Move into `g_userHeap.AllocPhysical<GuestTexture>(std::move(texture))`,
   write guest pointer back to `pictureData->texture` via MapVirtual.

## 10. How this differs from LibertyRecomp's half-finished implementation

Liberty has `CreateTexture`, `CreateSurface`, `LockTextureRect`,
`UnlockTextureRect`, `ConvertFormat`, the 0x100/0x200 constants, and the
`g_textureDescriptorAllocator/g_textureDescriptorSet` bindless machinery —
all copied faithfully from Unleashed (see `LibertyRecomp/gpu/video.cpp:2697-2734,
3981,4101,3939`). The Lock/Unlock host path is literally identical to
Unleashed's and is NOT the bug source. Evidence:

- `LibertyRecomp/gpu/video.cpp:2697-2735` matches `video.cpp:2163-2206`
  line-for-line modulo whitespace.

What Liberty did differently, and what the crash tells us:

### A. Dual-stacked CreateTexture hook (LIBERTY BUG)

`LibertyRecomp/gpu/video.cpp:9190` and `9308` both bind
`GUEST_FUNCTION_HOOK(sub_82A44850, ...)` — first to `GTAIV_CreateTexture`,
later to `CreateTexture`. Only the last registration wins, so one of these is
dead code. Unleashed only ever binds one hook per guest sub (see 7798-7857).
Liberty must decide: either RAGE-native `GTAIV_CreateTexture` (has 7 args, no
`depth` or `pool`) or Sonic-style 8-arg `CreateTexture`. They're different
call shapes.

### B. CopyFromBitmap / vfunc[23] crash at dst=0xBFF8

The crash sits in `grcTextureXenon::CopyFromBitmap` (guest vtable slot 23).
Unleashed sidesteps this class entirely: Sonic never invokes it because
the game goes Lock → write linear → Unlock. GTA IV's RAGE texture code instead
builds a `grcTextureXenon` that wraps a Xenos texture header at a real GPU
virtual address, then `CopyFromBitmap` does a raw `memcpy(xenosGpuVA, src,
linearSize)`. When LibertyRecomp hands the game a bare **host** `GuestTexture*`
as the grcTextureXenon handle, the vtable call indexes host memory +0xBFF8 as
if it were a Xenos header field → garbage pointer crash.

The Unleashed adoption for this: **do not let `grcTextureXenon` run**.
Either

1. Hook `grcTextureXenon::CreateFromData` / the class factory so the game never
   constructs one (match by guest address, redirect to our `CreateTexture`
   path), OR
2. Install a `PPC_FUNC_HOOK` on the vtable entry at offset 0x5C of
   `grcTextureXenon` (slot 23 × 4 = 0x5C) and have it call our
   `ProcUnlockTextureRect` equivalent with the host `GuestTexture*` it
   retrieves from a `GuestTexture*` ↔ grcTextureXenon-handle registry.

Unleashed's equivalent of (1) is the pair
`CreateTexture` + `MakePictureData` + `LoadTexture` — the only three entry
points a texture can come from. Anything else is stubbed
(`GUEST_FUNCTION_STUB(sub_82BFFF88)` for `D3DXFilterTexture`, etc.). Liberty
should **stub every Xenos-specific texture helper** the game calls below the
D3D-level API and force flow through the three canonical entry points.

### C. Surface and depth-stencil parity is already good

LibertyRecomp `CreateSurface` (line 4101) + `sub_828BEC78` PPC_FUNC_HOOK
(line 9212) correctly mirror Unleashed's `CreateSurface`. The 1×1 fmt=0
safe-fmt fix and compressed-format MSAA bail are Liberty-specific defensive
additions not present in Unleashed, but they're fine.

### D. Missing — must add to match Unleashed

- **No UpdateSurface hook** in either project; that's expected. If Liberty's
  GTA IV code calls a CPU→GPU `UpdateSurface` equivalent (D3D9-style
  blit-from-sysmem-to-vidmem), add a Liberty hook that uses the same
  upload-allocator + copyTextureRegion pattern as `ProcUnlockTextureRect`.
  Unleashed doesn't need one because Sonic never issues one.
- **No GenerateMipmaps** — Liberty must accept that GTA IV assets come with
  embedded mips (RPF DDS), exactly like Unleashed assumes DDS assets bring
  their own mips. If GTA IV expects runtime mip generation, that's a gap.
- **StretchRect** — Liberty needs the Unleashed pattern at 3253-3299:
  record `texture->sourceSurface = surface`, defer via `g_pendingSurfaceCopies`
  until next SetTexture or render-target flush, execute with either a
  hardware resolve (COUNT_1 → shaderResolve path) or MSAA resolve shader.

## 11. Single most load-bearing adoption pattern for Liberty

Everything else in the Unleashed texture pipeline rests on this one
invariant:

**A `GuestTexture*` is a HOST pointer wrapped in guest-addressable memory,
not a Xenos D3D texture header.** The guest holds the pointer, MapVirtuals it
as 32-bit, and passes it back to our hooks — but the guest NEVER reads any
field off it. Every field after `refCount` is C++-layout, not Xenos-layout.
If any guest code path reads a field of this object, the hook for that path
is missing. The Liberty crash at memcpy(dst=0xBFF8) is exactly this: some
guest path (grcTextureXenon::CopyFromBitmap) is reading field `+0xBFF8` off
a host-layout GuestTexture, getting garbage, and memcpy'ing into it.

Fix: stub or hook every code path that reads a texture's Xenos header fields,
so no guest code ever touches `GuestTexture` memory directly beyond
`Lock*`-returned `mappedMemory`.
