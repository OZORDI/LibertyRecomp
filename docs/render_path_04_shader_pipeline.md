# Render Path 04 — Shader Pipeline

## Overview

LibertyRecomp uses an **offline shader recompilation** model. Xbox 360 Xenos GPU shaders are **not** translated at runtime. Instead, they are pre-compiled by the `XenosRecomp` tool from extracted Xenos bytecode into host shader formats (SPIR-V, DXIL, AIR/Metal), then embedded in a compressed shader cache that ships with the binary.

---

## 1. Shader Directory: `LibertyRecomp/gpu/shader/`

Two subdirectories contain **host-side post-process and utility shaders** (not game shaders):

### `shader/hlsl/` — HLSL source + pre-compiled SPIR-V headers
- Source `.hlsl` files for post-process effects (TAA, SMAA, FSR1, bloom, SSAO, DoF, etc.)
- Pre-compiled `.hlsl.spirv.h` headers (embedded SPIR-V bytecode as C arrays)
- `.hlsl.spv` intermediate SPIR-V binaries for some shaders

### `shader/msl/` — Metal Shading Language
- `.metal` source files (Metal equivalents of the HLSL shaders)
- `.metal.ir` → `.metal.metallib` → `.metal.metallib.h` (pre-compiled Metal shader libraries embedded as C arrays)

### Included in `video.cpp` conditionally:
```cpp
#ifdef LIBERTY_RECOMP_D3D12
#include "shader/hlsl/copy_vs.hlsl.dxil.h"    // DXIL for D3D12
#endif
#ifdef LIBERTY_RECOMP_METAL
#include "shader/msl/copy_vs.metal.metallib.h" // Metal
#endif
#include "shader/hlsl/copy_vs.hlsl.spirv.h"    // SPIR-V always included (Vulkan)
```

These are **utility/post-process shaders** — copy, resolve MSAA, gamma correction, HDR tonemap, ImGui rendering, CSD filter, gaussian blur, enhanced burnout blur.

---

## 2. Xbox 360 Shader Translation Pipeline

### Offline Recompilation: `tools/XenosRecomp/`

The `XenosRecomp` tool translates extracted Xenos (Xbox 360 GPU) shader bytecode:

1. **Input**: Raw `.bin` shader files extracted from GTA IV `.rpf`/`.fxc` archives (organized as `shaders/{fxc_name}/{fxc_name}_{vs|ps}{N}.bin`)
2. **`ShaderRecompiler::recompile()`** ([shader_recompiler.cpp](../tools/XenosRecomp/XenosRecomp/shader_recompiler.cpp)): Disassembles Xenos ALU/fetch/texture instructions → emits HLSL source code
3. **`DxcCompiler::compile()`**: Compiles HLSL → SPIR-V (via DXC with `-spirv` flag), optionally DXIL
4. **`AirCompiler::compile()`**: Compiles HLSL → Apple AIR (Metal intermediate)
5. **Output**: All compiled shaders are concatenated into compressed caches (SPIR-V, DXIL, AIR)

### Shader Common Header: `shader_common.h`

Shared between XenosRecomp output and the runtime. Defines:
- **Push constants layout**: `PushConstants { VertexShaderConstants, PixelShaderConstants, SharedConstants }` — GPU buffer addresses for `vk::RawBufferLoad`
- **Shared constants**: `g_Booleans`, `g_SwappedTexcoords`, `g_HalfPixelOffset`, `g_ClipPlane`, `g_AlphaThreshold`
- **Spec constants**: `SPEC_CONSTANT_R11G11B10_NORMAL`, `SPEC_CONSTANT_ALPHA_TEST`, `SPEC_CONSTANT_CONDITIONAL_SURVEY/RENDERING`
- Metal (`__air__`) and Vulkan (`__spirv__`) variants via preprocessor

---

## 3. Shader Cache: `LibertyRecompLib/shader/`

### `shader_cache.h` — Cache entry structure
```cpp
struct ShaderCacheEntry {
    const uint64_t hash;          // XXH3_64bits of original Xenos bytecode
    const uint32_t dxilOffset, dxilSize;
    const uint32_t spirvOffset, spirvSize;
    const uint32_t airOffset, airSize;
    const uint32_t specConstantsMask;
    char filename[256];           // e.g. "shaders/gta_default/gta_default_ps7.bin"
    struct GuestShader* guestShader; // Runtime pointer (populated on first use)
};
```

### `shader_cache.cpp` — ~1148 entries
Contains all pre-compiled GTA IV shaders as offset/size pairs into three compressed blobs:
- `g_compressedSpirvCache` (Vulkan)
- `g_compressedDxilCache` (D3D12)
- `g_compressedAirCache` (Metal)

Decompressed at startup via Zstd into `g_shaderCache` ([video.cpp:1023](../LibertyRecomp/gpu/video.cpp#L1023)).

### Cache loading priority (video.cpp `LoadEmbeddedResources()`):
1. **Embedded** (build-time compiled into binary) — `TryLoadEmbeddedShaderCache()`
2. **Disk** (install-time generated) — `TryLoadDiskShaderCache()` from `PlatformPaths::GetShaderCacheDirectory()`

---

## 4. Runtime Shader Creation Hooks in `video.cpp`

### `CreateShader()` (line ~6180)
Core shader creation. Called when RAGE's FXC parser encounters a VS/PS fragment:
1. Hashes Xenos bytecode: `XXH3_64bits(function, function[1] + function[2])`
2. Binary-searches `g_shaderCacheEntries[]` via `FindShaderCacheEntry(hash)`
3. On **HIT**: Creates `GuestShader` with `shaderCacheEntry` pointer → registered via `GTAIV::RegisterShader()`
4. On **MISS**: Calls `_Exit(1)` — incomplete cache is fatal

### `PPC_FUNC_HOOK(sub_82A42BA8)` (line ~9019)
Alternative entry point for shader creation from raw bytecode containers (magic `0x102A11XX`):
- Same hash→lookup→`GuestShader` flow
- On miss: creates dummy `GuestShader` (no `_Exit`, allows graceful degradation)

### `SetVertexShader()` / `SetPixelShader()` (lines ~6277, ~6367)
Enqueues `RenderCommand` to render thread. If shader is NULL, substitutes `g_defaultVertexShader`/`g_defaultPixelShader`.

### `ProcSetVertexShader()` / `ProcSetPixelShader()` (lines ~6298, ~6388)
Render-thread processing. Performs **shader replacement** for enhanced effects:
- Radial blur VS/PS hashes → `g_enhancedBurnoutBlurVSShader` / `g_enhancedBurnoutBlurPSShader` (when `Config::RadialBlur == ERadialBlur::Enhanced`)

### Stubbed shader reload hooks:
| Hook | Address | Purpose |
|-|-|-|
| `sub_828574A0` | Shader reload | No-op: "Liberty shaders are immutable" |
| `sub_8285BDC8` | FXC file open/parse | Stubbed: returns 1 (success) |
| `sub_8285DF10` | Shader fixup | No-op |
| `sub_82858758` | Per-FXC preload | Stubbed: returns 0 |

---

## 5. SPS Preset Table: `sps_preset_table.h`

Maps RAGE **Shader Preset System** names (`.sps` files) to FXC shader names with material parameters.

### Structure
```cpp
struct SpsPresetEntry {
    const char* spsName;    // e.g. "gta_vehicle_paint1.sps"
    const char* fxcName;    // e.g. "gta_vehicle_paint1" (which FXC shader set to use)
    uint8_t drawBucket;     // __rage_drawbucket (0=opaque, 1=alpha, 2=decal, 3=cutout, 4=emissive, 5=emissive+alpha)
    uint8_t paramCount;
    const SpsParamEntry* params; // material parameter overrides (SpecularColor, Specular, etc.)
};
```

### Size: 131 entries
Covers all GTA IV material types: default, normal, spec, reflect, vehicle paints/chrome/rims/glass, ped/skin, terrain, trees, decals, emissive, parallax, wire, radar, billboard, etc.

### Runtime population: `PPC_FUNC_HOOK(sub_82869F30)` (line ~8813)
Replaces RAGE's disk-based SPS scanner (`common:/shaders/db`):
1. Allocates guest-visible memory for the SPS database
2. Copies all 131 entries from `g_spsPresetTable[]` into 28-byte guest structures
3. Writes global pointers at `0x83127DA4` (count), `0x83127DAC` (entries), `0x83127DB0` (path)
4. Game's material setup code (`sub_82869FC0` lookup → `sub_82869620` get) then resolves SPS → FXC shader mapping

### Lazy FXC creation: `PPC_FUNC_HOOK(sub_82869620)` (line ~8906)
When a SPS entry's `fxcShaderPtr` (offset +12) is NULL:
- Calls `shaderMgr->vtable[1](fxcName, 0, 0)` to create the FXC effect
- Stores result back at entry+12 for future lookups

---

## 6. GuestShader Runtime Structure

```cpp
struct GuestShader : GuestResource {
    Mutex mutex;
    std::unique_ptr<RenderShader> shader;           // Host GPU shader object
    struct ShaderCacheEntry* shaderCacheEntry;       // Points into g_shaderCacheEntries[]
    unordered_dense::map<uint32_t, unique_ptr<RenderShader>> linkedShaders; // Spec-constant variants
};
```

When a draw call references a `GuestShader`, the pipeline state builder reads:
- `shaderCacheEntry->hash` for pipeline cache key
- Decompresses SPIR-V/DXIL/AIR from `g_shaderCache` at the entry's offset
- Creates `RenderShader` (via plume abstraction) with appropriate spec constants
- Caches the `RenderPipeline` in `g_pipelines[]` keyed by full `PipelineState` hash

---

## 7. Post-Process Pipeline

### `postprocess_renderer.cpp` — **Active, initialized**
Full GPU dispatch for post-processing:
- **TAA** (Temporal Anti-Aliasing) with Halton jitter
- **SMAA** (3-pass: edge detect → blend weight → neighborhood blend)
- **FSR 1.0** (AMD FidelityFX — EASU + RCAS passes)
- **SSAO** (GTAO variant — SPIR-V shaders included, DXIL/Metal commented out)
- **DoF** (4-pass: prefilter → bokeh → postfilter → combine)
- **SSR** (Screen-Space Reflections — raytrace + composite)
- **Film Grain**, **Chromatic Aberration**, **Motion Blur**
- **Bloom** (extract → downsample → upsample → composite)
- **Sun Shafts** (prepass → radial blur → composite)

### `postprocess_aa.cpp` — **Active, controls AA mode selection**
- Validates shader availability via `g_postprocessShaderEntries[]` hash lookup
- Checks: `FullscreenVS`, `TAAPS`, `SMAAEdgeDetectPS`, `SMAABlendPS`, `FSR1EASUPS`, `FSR1RCASPS`
- If all present → `g_postprocessShadersAvailable = true`

### `postprocess_renderer_stub.cpp` — Fallback no-op stubs (when renderer not linked)

---

## 8. Key Findings for macOS Rendering

### What works:
- Shader cache loads (SPIR-V for Vulkan, AIR for Metal)
- SPS database populated (131 presets)
- `CreateShader` hook matches ~1148 Xenos shaders to cache entries
- Post-process pipeline initializes (TAA/SMAA/FSR1/bloom/etc.)

### Potential issues for blank frames:
1. **Cache misses in `sub_82A42BA8`**: Returns dummy shaders (no `RenderShader`, no `shaderCacheEntry`) which produce null pipeline states → draw calls silently dropped
2. **`g_defaultVertexShader`/`g_defaultPixelShader`**: Set to the first shader created by `CreateShadersForFxc()` — if that function isn't called or finds nothing, defaults remain null
3. **Stubbed `sub_8285BDC8`**: Prevents native FXC loading → if `CreateShader` via `sub_82A42BA8` doesn't cover all shaders, some materials get dummy shaders
4. **Pipeline creation failures**: Spec constant mismatches or missing vertex declarations may cause `g_pipelines[]` lookup to return null
5. **Render target setup**: Post-process pipeline may not receive the game's framebuffer if the main render path produces no visible pixels

### Shader format summary:
| Backend | Game Shaders | Post-Process Shaders |
|-|-|-|
| Vulkan | SPIR-V (from cache) | SPIR-V (embedded `.spirv.h`) |
| D3D12 | DXIL (from cache) | DXIL (embedded `.dxil.h`) |
| Metal | AIR (from cache) | Metal Library (embedded `.metallib.h`) |
