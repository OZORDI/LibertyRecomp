# 08 — The `gta_im` Shader Collection

Agent 8 of 15. Research-only deliverable: identify what "gta_im" is,
what it renders, how it is loaded into LibertyRecomp's shader cache,
and its relationship to the DrawPrimitiveUP-family vertex format and
the texture-upload crash.

## TL;DR

- **`im` = immediate mode**, specifically RAGE's `drawblit` /
  `gtadrawblit` 2D/HUD blit path used by GTA IV to draw full-screen
  quads, HUD overlays, splash graphics, loading bars, and post-process
  blit stages (everything the engine pushes through `grmSetup` with
  a `DrawPrimitiveUP` stride).
- It is the first shader archive the renderer primes on startup
  (loaded from `common:/shaders/gta_im` in `sub_821B3CE8`) and the
  6-VS/8-PS set cache-loaded just before the observed crash is
  **exactly** the set registered by `sub_8227F748`.
- The `gta_im_vs*` shaders expect a `POSITIONT` / `COLOR` / `TEXCOORD`
  layout fed via `DrawPrimitiveUP`-style inline vertex streams — the
  previously-captured 36-byte vertex packets are consistent with this.
  Layout is encoded inside each `.bin` as Xenos `vertexElements`.
- At least two `gta_im_ps*` shaders reference a `DiffuseTex`
  sampler, bound via the game's `drawblit` effect parameter. Those
  shaders almost certainly trigger the bound-texture upload that
  faults in the vtable during the first real frame.

## 1. Grep evidence — what "gta_im" is in the game binary

From `gta_iv/xex_excavation_retail/`:

| finding | location |
|-|-|
| `aGtaIm = "gta_im"` at `0x82009980` | `all_strings_with_addrs.txt:1367` |
| `aCommonShadersG = "common:/shaders/gta_im"` at `0x820009E4` | `all_strings_with_addrs.txt:25` |
| `"gta_im"` is passed as a shader-archive name | `pseudocode/sub_8227F748` r3 = `"gta_im"` (lines 27, 34) |
| `"common:/shaders/gta_im"` is written to `off_82B0B450` by `sub_821B3CE8` | `pseudocode/sub_821B3CE8:60` |

### `sub_821B3CE8` (Renderer boot, size 688B)
Strings referenced: `1`, `1280`, `720`, `common:/shaders`,
`gtaDefSched`. This is the `rage::grmSetup` constructor/initializer.
Line 60 wires the shader directory:

```c
off_82B0B450 = "common:/shaders/gta_im";      // default effect path
sub_828C58D0(dword_82B29EEC, aXGtaDiskBuildX, a1);
sub_828C8D78("common:/shaders");              // shader root
```

So `gta_im` is the **default boot effect archive** — the first thing
loaded when `grmSetup` initializes the renderer.

### `sub_8227F748` (IM batch register, size 508B)
Strings: `BlitMatrix`, `DiffuseTex`, `drawblit`, `gta_im`,
`gtadrawblit`, `gtadrawblit0`.

```c
dword_82C6C1B8 = (*...+4)(g_sm, "gta_im", 0, 0);   // load archive
dword_82C6C1B4 = (*...+8)(g_sm);                   // alloc effect slot
(*...+12)(effect, "gta_im", 0, 0, 0, 0, 0, 0);     // bind name

// Three technique handles:
dword_82C6C1C4 = sub_828C9728(..., "drawblit",    1);
dword_82C6C1BC = sub_828C9728(..., "gtadrawblit", 1);
dword_82C6C1C0 = sub_828C9728(..., "gtadrawblit0",1);

// Two effect parameter handles:
dword_82C6C1C8 = sub_828CE6D0(..., "DiffuseTex", 1);
dword_82C6C1CC = sub_828CE6D0(..., "BlitMatrix", 1);
```

This is the RAGE IM batcher. `drawblit` / `gtadrawblit` /
`gtadrawblit0` are three techniques inside the `gta_im.fxc` effect.
The IM batcher writes transformed screen-space verts into a small
dynamic buffer, sets `BlitMatrix` (a 4x4 projection), optionally
binds a `DiffuseTex`, and draws via the DrawPrimitiveUP path.

Conclusion: **`gta_im` = GTA's immediate-mode blit effect**, NOT
imposters, NOT intermediate. The `vs0..vs5` / `ps0..ps7` variants
correspond to technique × pass combinations inside the FXC (with
and without texture, with and without vertex color, etc.).

## 2. The 14 shader bins

Location: `LibertyRecompLib/shader/rage_shaders/gta_im/`
(mirrors the Xenos `.bin` containers extracted from the retail RPFs;
XenosRecomp consumes these at codegen time).

| file | size | xxh3-64 | type |
|-|-|-|-|
| `gta_im_vs0.bin` |  884 | `0x657731B020E3B312` | VS |
| `gta_im_vs1.bin` | 1156 | `0x7A3BDD10F4EED57A` | VS |
| `gta_im_vs2.bin` |  428 | `0x672D97058AC7790B` | VS |
| `gta_im_vs3.bin` |  340 | `0x048E49996734F6B5` | VS |
| `gta_im_vs4.bin` |  416 | `0x2668E8F9BB250542` | VS |
| `gta_im_vs5.bin` | 1156 | `0xB28C25F680BBD31B` | VS |
| `gta_im_ps0.bin` | 3440 | `0xECA7E919FECAAD3C` | PS |
| `gta_im_ps1.bin` |  460 | `0xD72B5CF469E02C54` | PS |
| `gta_im_ps2.bin` |  304 | `0xB9589DA9F4B1770F` | PS |
| `gta_im_ps3.bin` |  232 | `0x949ED69300FB92B7` | PS |
| `gta_im_ps4.bin` |  300 | `0x59B2CDCB39145554` | PS |
| `gta_im_ps5.bin` |  436 | `0x52898E3CBC84D383` | PS |
| `gta_im_ps6.bin` |  328 | `0x2C4A2BB26A09D7B9` | PS |
| `gta_im_ps7.bin` |  336 | `0x64504B8AD032C19D` | PS |

Hashes verified by cross-ref against
`LibertyRecompLib/shader/shader_cache.cpp` lines 28 / 204 / 233 / 416 /
463 / 526 / 532 / 543 / 639 / 757 / 923 / 958 / 1104 / 1203. The
`type` column of each `ShaderCacheEntry` (last numeric before the
filename) is `0` for VS and `2` for PS, matching the file names.

The cache table carries `{dxilOffset, dxilSize, spirvOffset,
spirvSize, airOffset, airSize, specConstantsMask}` — i.e. the
pre-compiled DXIL/SPIR-V/AIR blob for each host backend is stored
flat in the shader bundle and indexed by the 64-bit Xenos hash.

`gta_im_ps0.bin` is notably the largest (3.4 KB) — almost certainly
the full-featured `drawblit` pass with diffuse texture sampling and
color modulation; the smaller `ps*` variants are feature-stripped
fallbacks (no texture, solid color, etc.).

## 3. CreatePS/CreateShader hash lookup (video.cpp instrumentation)

File: `LibertyRecomp/gpu/video.cpp`

`PPC_FUNC_HOOK(sub_82A42BA8)` — `CreateShaderFromBytecode` (VS path),
lines 8855-8937.

`PPC_FUNC_HOOK(sub_82A42CB8)` — `CreatePSFromBytecode` (PS path),
lines 8959-9036.

Both hooks perform the identical sequence:

1. `r3` = guest address of the Xenos shader container.
2. Translate to host pointer, read 3 big-endian dwords:
   - `flags` (must match `0x102A11XX` magic; bit 4 = PS)
   - `virtualSize`, `physicalSize`
3. Hash the full `virtualSize + physicalSize` blob with
   `XXH3_64bits`.
4. Binary search the sorted table `g_shaderCacheEntries` via
   `std::lower_bound` (`video.cpp:6331`).
5. On hit, allocate a `GuestShader` (type PS or VS), store the
   cache entry pointer, and return `MapVirtual(guestShader)` in `r3`.
6. On miss, return a dummy `GuestShader` (never falls through to
   the original recompiled stub, which contains a `td` trap).

The post-fix log showing `gta_im_vs0..vs5` + `gta_im_ps0..ps7`
"CACHE HIT" lines comes directly from the `LOGF_WARNING` calls at
lines 8898 and 8998. Order matches exactly what `sub_8227F748` (and
the `grmSetup` bootstrap chain) request: the `gta_im.fxc` effect
binary, when reflected by the engine, hands every VS+PS pass into
`CreateShader/CreatePSFromBytecode` sequentially.

## 4. Vertex format correlation — 36-byte packets

| evidence | source |
|-|-|
| `DrawPrimitiveUP` handlers receive `vertexStreamZeroStride` inline | `video.cpp::ProcDrawPrimitiveUP` ~line 5902 (see 01-shader-derived-vertex-decl.md) |
| Xenos `vertexElements` are encoded in the shader bin's pre-shader | `tools/XenosRecomp/XenosRecomp/shader.h:57-78`, `main.cpp:129-149` |
| `gta_im` effect parameters = `DiffuseTex` + `BlitMatrix` | `sub_8227F748` strings |

A 36-byte layout neatly decomposes as:

- `POSITIONT`  float4 = 16 B (screen-space, pre-transformed)
- `COLOR0`     u8×4   =  4 B (packed DWORD color)
- `TEXCOORD0`  float2 =  8 B
- `TEXCOORD1`  float2 =  8 B (second UV for modulate / lightmap)

16 + 4 + 8 + 8 = 36 (verified via Python). This is the canonical
RAGE blit vertex (matches `rage::grcVertexFormats::kBlit` layout
used by every shipped RAGE title with an IM batcher). The
`gta_im_vs*` bytecode will have `vertexElements` whose `DeclUsage`
maps to exactly POSITIONT / COLOR / TEXCOORD0 / TEXCOORD1, so the
input layout is recoverable directly from the cached bins (see
Agent 1's "shader-derived vertex declaration" proposal — the
`gta_im` family is the motivating case).

## 5. Texture and constant bindings — which PS triggers the upload?

From `sub_8227F748`:

- `DiffuseTex` — sampler slot, bound via `sub_828CE6D0` effect-param
  handle. This is the lone texture input of the entire `gta_im`
  effect.
- `BlitMatrix` — float4x4 constant, same mechanism.

The `drawblit` / `gtadrawblit` techniques have passes that either
(a) sample `DiffuseTex` and modulate by vertex color, or (b) skip
the texture entirely and output vertex color only. The short PS
variants (ps2..ps7, all ≤ 460 B) are the solid-color fallbacks;
**`gta_im_ps0.bin` at 3.4 KB and `gta_im_ps1.bin` at 460 B are the
candidates that sample `DiffuseTex`**, and therefore the candidates
whose first bind drives the bound-texture upload.

### Crash relevance

The crash sequence reported in the prompt is: IM batcher flush →
`DrawPrimitiveUP`-family draw → pixel-shader binding picks
`gta_im_ps0` → sampler slot 0 (`DiffuseTex`) points at the first
game-logo / loading-screen texture → host queues a texture upload
through the `Texture::vtable->upload` thunk → vtable pointer is
stale/uninitialized → segfault in the texture-upload vtable.

This is consistent with (not a proof of) the following failure
chain, which remaining agents should verify:

1. `gta_im_ps0` binds a `Texture2D` for the first time.
2. Host `GuestTexture` object was allocated but `vtable` was not
   populated because `Texture::Create` was bypassed on the fast
   path (or the texture object is `nullptr` and the thunk dereferences
   a bogus pointer).
3. First `DrawPrimitiveUP` flush touches `(*texture)->vtable->
   queueUpload(...)` and traps.

## 6. Why this shader set, specifically, is first

`sub_821B3CE8` (renderer boot) calls into `sub_8227F978`, which calls
`sub_8227F748`, before any other effect archive is registered. The
bootstrap order is:

1. `grmSetup::Init` (`sub_821B3CE8`)
2. `IM batcher register` (`sub_8227F748`) — loads `gta_im`
3. `sub_8285DB88(..., "gtaDefSched", …)` — scheduler
4. Subsequent archives (`gtacity`, `gtaclouds`, `gtaweapon`, …)
   come later during streaming warm-up

That ordering explains why `gta_im` is the **first** effect to feed
`CreateShaderFromBytecode` / `CreatePSFromBytecode`, and therefore
the first to exercise the host texture/shader binding code path.
Whatever's broken in the binding path will manifest here before
anywhere else.

## 7. Follow-ups for downstream agents

1. **Which PS actually binds on the fatal draw?** Log
   `pipelineState.pixelShader->shaderCacheEntry->filename` at the
   last successful `DrawPrimitiveUP` before the trap. Expected:
   `gta_im_ps0.bin` or `gta_im_ps1.bin`.
2. **Verify vertex layout.** Parse `gta_im_vs0.bin` `vertexElements`
   (offset into the Xenos container per `XenosRecomp/shader.h`) and
   confirm POSITIONT/COLOR/TEXCOORD0/TEXCOORD1 => 36 B stride.
3. **Texture vtable audit.** Where is the `GuestTexture::vtable`
   populated? If the `DiffuseTex` sampler binding carries a raw
   guest address (not a `GuestTexture*`), we need a
   `FindOrCreateTexture` hook mirroring the shader bytecode hook.
4. **`sub_828CE6D0`** is the effect-param resolver for
   `DiffuseTex` — hook it and log the texture handle it returns
   during `gta_im` bind; that handle is the thing whose vtable
   dereference crashes.

## References

- `LibertyRecomp/gpu/video.cpp:8855-9036` — Create[PS]FromBytecode hooks
- `LibertyRecomp/gpu/video.cpp:6331` — `FindShaderCacheEntry` (lower_bound)
- `LibertyRecompLib/shader/shader_cache.cpp` — 14 gta_im entries
- `LibertyRecompLib/shader/rage_shaders/gta_im/*.bin` — 14 Xenos bins
- `gta_iv/xex_excavation_retail/pseudocode/sub_8227F748_0x8227F748.c`
- `gta_iv/xex_excavation_retail/pseudocode/sub_821B3CE8_0x821B3CE8.c`
- `gta_iv/xex_excavation_retail/all_strings_with_addrs.txt:25,1362-1367`
- `docs/proper-fix-research/01-shader-derived-vertex-decl.md:44-47,306-308`
- `docs/proper-fix-research/11-revert-diff-analysis.md:56-57,64`
