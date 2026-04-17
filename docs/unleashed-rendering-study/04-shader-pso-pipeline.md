# 04 — UnleashedRecomp Shader + PSO Pipeline (adoption study)

Source paths (absolute):
- `Reference Projects/UnleashedRecomp-main/UnleashedRecomp/gpu/video.cpp` (7882 lines)
- `Reference Projects/UnleashedRecomp-main/UnleashedRecomp/gpu/video.h`
- `Reference Projects/UnleashedRecomp-main/UnleashedRecompLib/shader/shader_cache.h`
- `Reference Projects/UnleashedRecomp-main/UnleashedRecomp/gpu/shader/*.hlsl` (runtime copy/resolve/blur/imgui shaders)

## Shader struct types and cache

### `ShaderCacheEntry` — produced by XenosRecomp codegen
File: `UnleashedRecompLib/shader/shader_cache.h:3-12`.

```cpp
struct ShaderCacheEntry
{
    const uint64_t hash;
    const uint32_t dxilOffset;
    const uint32_t dxilSize;
    const uint32_t spirvOffset;
    const uint32_t spirvSize;
    const uint32_t specConstantsMask;
    struct GuestShader* guestShader;
};
extern ShaderCacheEntry g_shaderCacheEntries[];
extern const size_t g_shaderCacheEntryCount;
extern const uint8_t g_compressedDxilCache[];  // ZSTD-compressed DXIL blob for D3D12
extern const uint8_t g_compressedSpirvCache[]; // ZSTD-compressed SPIR-V blob for Vulkan
```

Two monolithic blobs per shader bytecode format. Each entry stores an `(offset,size)` slice into the blob plus an xxh64 guest-bytecode hash used as the lookup key. `specConstantsMask` marks which spec-constant bits are actually referenced — used both for linking (D3D12) and key reduction (both).

### `GuestShader` — runtime wrapper on the Xenon heap
File: `video.h:285-298`.

```cpp
struct GuestShader : GuestResource
{
    Mutex mutex;
    std::unique_ptr<RenderShader> shader;           // Plume shader (SPIR-V or DXIL)
    struct ShaderCacheEntry* shaderCacheEntry = nullptr;
    ankerl::unordered_dense::map<uint32_t, std::unique_ptr<RenderShader>> linkedShaders;
#ifdef UNLEASHED_RECOMP_D3D12
    std::vector<ComPtr<IDxcBlob>> shaderBlobs;
    ComPtr<IDxcBlobEncoding> libraryBlob;
#endif
#ifdef ASYNC_PSO_DEBUG
    const char* name = "<unknown>";
#endif
};
```

`linkedShaders` only fills on D3D12 (DXIL spec-constant link path). Vulkan uses `RenderSpecConstant` on the pipeline itself, so `shader` stays the single decoded SPIR-V and `linkedShaders` stays empty.

### Decompression at startup
File: `video.cpp:778-794`.

```cpp
if (g_vulkan) {
    g_shaderCache = std::make_unique<uint8_t[]>(g_spirvCacheDecompressedSize);
    ZSTD_decompress(g_shaderCache.get(), g_spirvCacheDecompressedSize,
                    g_compressedSpirvCache, g_spirvCacheCompressedSize);
} else {
    g_shaderCache = std::make_unique<uint8_t[]>(g_dxilCacheDecompressedSize);
    ZSTD_decompress(..., g_compressedDxilCache, g_dxilCacheCompressedSize);
}
```

Only one blob is decompressed per run. The SPIR-V payload is additionally `smolv`-compressed (per-entry), decoded lazily in `GetOrLinkShader` (`video.cpp:3868-3874`).

### `PipelineState` — the PSO hash key
File: `video.cpp:121-150`, `#pragma pack(push, 1)`.

```cpp
struct PipelineState
{
    GuestShader* vertexShader = nullptr;
    GuestShader* pixelShader = nullptr;
    GuestVertexDeclaration* vertexDeclaration = nullptr;
    bool instancing, zEnable, zWriteEnable;
    RenderBlend srcBlend, destBlend;
    RenderCullMode cullMode;
    RenderComparisonFunction zFunc;
    bool alphaBlendEnable;
    RenderBlendOperation blendOp;
    float slopeScaledDepthBias;
    int32_t depthBias;
    RenderBlend srcBlendAlpha, destBlendAlpha;
    RenderBlendOperation blendOpAlpha;
    uint32_t colorWriteEnable;
    RenderPrimitiveTopology primitiveTopology;
    uint8_t vertexStrides[16];
    RenderFormat renderTargetFormat, depthStencilFormat;
    RenderSampleCounts sampleCount;
    bool enableAlphaToCoverage;
    uint32_t specConstants;
};
```

Python-verified packed size (sum of member sizes, pack=1): **109 bytes**.
This whole struct is hashed bit-for-bit with `XXH3_64bits(&pipelineState, sizeof(pipelineState))` at `video.cpp:4139`.

## Hook coverage (VS / PS / state-block)

From `video.cpp:7798-7861` (the only hook block in the file):

|guest address|wrapper|description|
|-|-|-|
|`sub_82BE1A80`|`CreateVertexShader`|alloc + xxh64 of DWORDs, bind `ShaderCacheEntry`|
|`sub_82BE0110`|`SetVertexShader`|enqueue `RenderCommandType::SetVertexShader`|
|`sub_82BE1990`|`CreatePixelShader`|same as VS path|
|`sub_82BDFE58`|`SetPixelShader`|enqueue + optional shader swap (DoF / motion blur)|
|`sub_82BE0428`|`CreateVertexDeclaration`|hash + addref into `g_vertexDeclarations`|
|`sub_82BE02E0`|`SetVertexDeclaration`|enqueue + mirror back into `device->vertexDeclaration`|
|`sub_82BE04B0`|`GetVertexDeclaration`|trivial accessor|
|`sub_82BE0530`|`HashVertexDeclaration`|returns the already-cached hash|
|`sub_82BDD0F8`|`SetStreamSource`|enqueue; updates stride slot|
|`sub_82BDD218`|`SetIndices`|enqueue|
|`sub_82BDA8C0`|`Video::Present`|implicit state flush driver|

There is **no hook for SetVertexShaderConstantF / SetPixelShaderConstantF** — Unleashed drives constant uploads from the guest `GuestDevice::dirtyFlags` bitmasks inside `FlushRenderStateForMainThread` (`video.cpp:4325-4359`). That function runs in the guest thread on every draw and fans out `SetVertexShaderConstants`/`SetPixelShaderConstants` commands for just the dirty register ranges.

## PSO hashing ingredients

`CreateGraphicsPipelineInRenderThread` (`video.cpp:4135-4143`):

```cpp
static RenderPipeline* CreateGraphicsPipelineInRenderThread(PipelineState pipelineState)
{
    SanitizePipelineState(pipelineState);
    XXH64_hash_t hash = XXH3_64bits(&pipelineState, sizeof(pipelineState));
    auto& pipeline = g_pipelines[hash];
    if (pipeline == nullptr)
        pipeline = CreateGraphicsPipeline(pipelineState);
    ...
}
```

Ingredients that land in the hash (because they're in the struct):

1. VS/PS `GuestShader*` raw pointers — identity-based, so two shaders with the same bytecode but different heap allocations collide intentionally only if they share one `GuestShader`. `CreateShader` dedupes by xxh64 of guest bytecode (`video.cpp:5059-5083`), so the pointer is stable per guest-shader.
2. `GuestVertexDeclaration*` — also pointer identity. `CreateVertexDeclarationWithoutAddRef` dedupes by xxh64 of vertex-element array (`video.cpp:4841-4847`), so identical declarations share a pointer. The struct's own `hash` field is stored but **does not participate in the PSO hash directly** — only the pointer does. That's enough because dedup is canonical.
3. Blend, depth, stencil format, cull, MSAA count, primitive topology.
4. `vertexStrides[16]` (per-slot stride, pruned against `vertexDeclaration->vertexStreams[i]` in sanitize).
5. `specConstants` (already masked to the shaders' union mask by sanitize).

Python-confirmed: hash covers 109 bytes, no uninitialised padding (pack=1).

## Constant upload mechanics (vs, ps, shared)

### Storage
File: `video.cpp:184-186`.

```cpp
static uint32_t g_vertexShaderConstants[0x400];   // 1024 u32 = 256 vec4  (4 KiB)
static uint32_t g_pixelShaderConstants[0x380];    //  896 u32 = 224 vec4  (3.5 KiB)
static SharedConstants g_sharedConstants;         // texture/sampler indices + bools + half-pixel offsets + alpha threshold
```

`SharedConstants` (`video.cpp:160-171`) packs bindless texture/sampler indices, half-pixel offsets, and an alpha threshold — everything XenosRecomp wants that isn't a float register.

### Guest-driven fan-out
`FlushRenderStateForMainThread` reads `device->dirtyFlags[0]` (VS) and `[1]` (PS) as 64-bit masks, each bit meaning "a 16-register block is dirty." Using `countl_zero`/`countr_zero` it emits one coalesced `SetVertexShaderConstants` / `SetPixelShaderConstants` command per dirty range (`video.cpp:4325-4359`). The bytes are stashed in a single-frame `g_intermediaryUploadAllocator`.

### Render-thread apply
`ProcSetVertexShaderConstants` / `ProcSetPixelShaderConstants` (`video.cpp:4415-4431`) just memcpy into the two static arrays and mark `g_dirtyStates.{vertex,pixel}ShaderConstants = true`. The actual GPU upload happens at the end of `FlushRenderStateForRenderThread` (`video.cpp:4520-4536`):

```cpp
if (g_dirtyStates.vertexShaderConstants) {
    auto vertexShaderConstants = g_uploadAllocators[g_frame]
        .allocate<true>(g_vertexShaderConstants, sizeof(g_vertexShaderConstants), 0x100);
    SetRootDescriptor(vertexShaderConstants, 0);
}
if (g_dirtyStates.pixelShaderConstants) {
    auto pixelShaderConstants = g_uploadAllocators[g_frame]
        .allocate<true>(g_pixelShaderConstants, sizeof(g_pixelShaderConstants), 0x100);
    SetRootDescriptor(pixelShaderConstants, 1);
}
if (g_dirtyStates.sharedConstants) {
    auto sharedConstants = g_uploadAllocators[g_frame]
        .allocate<false>(&g_sharedConstants, sizeof(g_sharedConstants), 0x100);
    SetRootDescriptor(sharedConstants, 2);
}
```

Slot indices `0=VS, 1=PS, 2=Shared` are fixed. 256-byte alignment = D3D12 CBV requirement carried over to Vulkan.

### `SetRootDescriptor` — the backend split
`video.cpp:2887-2895`:

```cpp
static void SetRootDescriptor(const UploadAllocation& allocation, size_t index)
{
    auto& commandList = g_commandLists[g_frame];
    if (g_vulkan)
        commandList->setGraphicsPushConstants(0, &allocation.deviceAddress, 8 * index, 8);
    else
        commandList->setGraphicsRootDescriptor(allocation.buffer->at(allocation.offset), index);
}
```

Vulkan packs 3 × `uint64_t` device addresses into a 24-byte push-constant block; shaders deref them via `VK_KHR_buffer_device_address`. D3D12 sets real root CBV descriptors (standard root-sig slot).

## Backend-specific paths (D3D12 vs Vulkan)

### Pipeline layout — set up once at startup
`video.cpp:1888-1976`.

```cpp
pipelineLayoutBuilder.begin(false, true);
// set 0/1/2 = bindless texture heap shared across VS/PS/… 3 copies
pipelineLayoutBuilder.addDescriptorSet(descriptorSetBuilder);
pipelineLayoutBuilder.addDescriptorSet(descriptorSetBuilder);
pipelineLayoutBuilder.addDescriptorSet(descriptorSetBuilder);
// set 3 = samplers (bindless)
pipelineLayoutBuilder.addDescriptorSet(descriptorSetBuilder);

if (g_vulkan) {
    pipelineLayoutBuilder.addPushConstant(0, 4, 24, VERTEX | PIXEL);
} else {
    pipelineLayoutBuilder.addRootDescriptor(0, 4, CONSTANT_BUFFER); // VS
    pipelineLayoutBuilder.addRootDescriptor(1, 4, CONSTANT_BUFFER); // PS
    pipelineLayoutBuilder.addRootDescriptor(2, 4, CONSTANT_BUFFER); // Shared
    pipelineLayoutBuilder.addPushConstant(3, 4, 4, PIXEL);          // copy/resolve shaders
}
```

So the backend split lives in the layout definition and exactly one function (`SetRootDescriptor`). Everything else is unified.

### Spec constants: Vulkan vs D3D12
`GetOrLinkShader` (`video.cpp:3854-4008`):

- **Vulkan**: decode the SPIR-V blob exactly once per shader (behind `guestShader->mutex`). Spec constants are attached to the pipeline via `RenderSpecConstant` in `CreateGraphicsPipeline` (`video.cpp:4092-4098`). Driver specialises the SPIR-V at PSO create time — no linking here.
- **D3D12**: if `specConstantsMask != 0`, mask `specConstants &= mask`, lazily compile a tiny `lib_6_3` HLSL library that exports `g_SpecConstants()`, register it with `IDxcLinker` alongside the shader lib blob, and link per-variant DXIL. Each variant is cached in `guestShader->linkedShaders[specConstants]`. Global caches: `g_compiledSpecConstantLibraryBlobs` for the per-value library, per-GuestShader cache for the linked DXIL.

### Runtime built-in shaders
`shader/*.hlsl` are not guest shaders — they're engine shaders compiled by the host build system (`CREATE_SHADER(blend_color_alpha_ps)` etc, see `video.cpp:5071-5075`) and used for StretchRect, MSAA resolve, CSD filtering, gamma correction, enhanced motion blur, and ImGui rendering. A handful of them overwrite a matching `CreateShader` result when the xxh64 hits a known magic number (`0x85ED723035ECF535`, `0xB1086A4947A797DE`, `0xB4CAFC034A37C8A8` — lines 5070-5075).

## XenosRecomp → runtime binding

Flow for guest `CreateVertexShader(dword* function)`:

1. `CreateShader` (`video.cpp:5057-5096`) hashes `function[1] + function[2]` DWORDs — that's bytecode length stored in the Xenon preamble.
2. `FindShaderCacheEntry` (`video.cpp:5046-5055`) binary-searches `g_shaderCacheEntries` (sorted by hash at codegen time).
3. If found and `entry->guestShader == nullptr`, allocate a fresh `GuestShader`, assign `shader->shaderCacheEntry = entry`, and cache `entry->guestShader = shader`. Subsequent calls with the same bytecode re-use the same `GuestShader`.
4. If not found (novel shader), allocate a `GuestShader` with no cache entry — `shader` will stay null, which is later handled as a no-op draw (`pixelShader != nullptr` guards in `CreateGraphicsPipeline`).

Identity of the `GuestShader*` is stable per hash, so the pointer-as-key in `PipelineState` is safe.

## Vertex declaration → pipeline hash

`CreateVertexDeclarationWithoutAddRef` (`video.cpp:4825-5008`):

- Hashes the vertex-element array with `XXH3_64bits(vertexElements, vertexElementCount * sizeof(GuestVertexElement))` (line 4841).
- Stores that hash in `vertexDeclaration->hash` (line 4847) and deduplicates in `g_vertexDeclarations[hash]`.
- Populates `inputElements[]`, `vertexStreams[16]`, `swappedTexcoords` mask, and `hasR11G11B10Normal` flag.

`ProcSetVertexDeclaration` (`video.cpp:5027-5043`) then pushes:
- `hasR11G11B10Normal` → `SPEC_CONSTANT_R11G11B10_NORMAL` bit of `pipelineState.specConstants`
- `swappedTexcoords` → `g_sharedConstants.swappedTexcoords`
- the pointer itself → `pipelineState.vertexDeclaration`

**So `vertexDeclaration->hash` is not directly folded into the PSO hash; the dedupe'd pointer takes its place.** `hash` exists so the game can ask `HashVertexDeclaration` for its own purposes and so the dedupe map has a stable key. Because every distinct hash maps to one `GuestVertexDeclaration*`, hashing the pointer is equivalent to hashing the declaration.

## `SanitizePipelineState` — the contract

`video.cpp:4011-4055`:

```cpp
static void SanitizePipelineState(PipelineState& pipelineState)
{
    if (!pipelineState.zEnable) {
        pipelineState.zWriteEnable = false;
        pipelineState.zFunc = RenderComparisonFunction::LESS;
        pipelineState.slopeScaledDepthBias = 0.0f;
        pipelineState.depthBias = 0;
        pipelineState.depthStencilFormat = RenderFormat::UNKNOWN;
    }
    if (pipelineState.slopeScaledDepthBias == 0.0f)
        pipelineState.slopeScaledDepthBias = 0.0f; // remove -0.0 sign
    if (!pipelineState.colorWriteEnable) {
        pipelineState.alphaBlendEnable = false;
        pipelineState.renderTargetFormat = RenderFormat::UNKNOWN;
    }
    if (!pipelineState.alphaBlendEnable) {
        pipelineState.srcBlend = RenderBlend::ONE;
        pipelineState.destBlend = RenderBlend::ZERO;
        pipelineState.blendOp = RenderBlendOperation::ADD;
        pipelineState.srcBlendAlpha = RenderBlend::ONE;
        pipelineState.destBlendAlpha = RenderBlend::ZERO;
        pipelineState.blendOpAlpha = RenderBlendOperation::ADD;
    }
    for (size_t i = 0; i < 16; i++) {
        if (!pipelineState.vertexDeclaration->vertexStreams[i])   // <-- null deref if vd == null
            pipelineState.vertexStrides[i] = 0;
    }
    uint32_t specConstantsMask = 0;
    if (pipelineState.vertexShader->shaderCacheEntry != nullptr)  // <-- null deref if vs == null
        specConstantsMask |= pipelineState.vertexShader->shaderCacheEntry->specConstantsMask;
    if (pipelineState.pixelShader != nullptr && pipelineState.pixelShader->shaderCacheEntry != nullptr)
        specConstantsMask |= pipelineState.pixelShader->shaderCacheEntry->specConstantsMask;
    pipelineState.specConstants &= specConstantsMask;
}
```

Why sanitize exists: it canonicalises the struct so that **"logically equivalent" pipelines produce the same hash**. Disabling depth→zeroing depth-related state, disabling color write→zeroing color state, etc. Without this, each draw with stale state bits would miss the cache.

### Null-guarding as in practice (the contract)

Unleashed **does not null-guard** `pipelineState.vertexDeclaration` (line 4043) nor `pipelineState.vertexShader` (line 4048). `pixelShader` *is* guarded (line 4051) — because depth-only passes really exist.

Why it's safe in Unleashed:

1. Sonic Unleashed Project (Hedgehog Engine) always calls `SetVertexDeclaration` + `SetVertexShader` before any draw — these are tied to the material system. Depth-only shadow passes still bind a vertex shader.
2. Unleashed never calls `FlushRenderStateForRenderThread` before the first draw (the initial state is `g_dirtyStates.pipelineState = true`, but the function is only invoked from `ProcDrawPrimitive[UP]` / `ProcDrawIndexedPrimitive`).
3. Precompilation paths that manufacture `PipelineState` objects (e.g. `video.cpp:6467`, `6472`, `6525-6526`, `6541`, `6610-6611`, `7230-7233`) *explicitly* assign `vertexShader` from `FindShaderCacheEntry(hash)->guestShader` before calling `SanitizePipelineState` — so the contract is "caller owns the invariant."

### Why LibertyRecomp crashes on the same code

GTA IV's PC build supports draws where the D3D9-era pipeline would actually tolerate a null vertex decl (the game zeros it, falls through some code paths, and either abandons the draw or expects the runtime to silently skip). UnleashedRecomp inherited the "it never happens" invariant from Unleashed. Either:

- **LibertyRecomp must add `if (vd) for(i) …; if (vs) specMask |= …` null guards** to `SanitizePipelineState`, **and** skip the actual pipeline creation / draw when either is null (we can't hash a pipeline with no vertex input); or
- **Null `SetVertexShader` / `SetVertexDeclaration` must be treated as "cancel next draw"** in the command processors — store a `g_drawInhibited` flag and early-return from the draw procs.

The first approach is cheaper; the second matches D3D9 semantics more exactly. Either way, the `SanitizePipelineState` body as-copied is a ticking null-deref.

## Adoption notes for LibertyRecomp

- Unleashed's PSO key is 109 bytes (pack=1) hashed with xxh3-64; identical shape works for us once vertex shader/decl are guaranteed non-null.
- Root-descriptor slot layout (VS=0, PS=1, Shared=2, samplers/textures in separate descriptor sets) is a good default — matches XenosRecomp's emitted shader expectations.
- The Vulkan buffer-device-address push-constant trick (24 bytes, 3 × u64) avoids needing dynamic UBO offsets and is what XenosRecomp-generated SPIR-V already expects.
- The SPIR-V smolv layer on top of ZSTD is worth inheriting (big shader-cache size wins).
- The asynchronous pipeline compiler pool sized at `max(2, hw_concurrency * 2 / 3)` with precompile-during-logo semantics is a proven shipping strategy (`video.cpp:6310-6319`).
- `CreateShader` bytecode hashing (`XXH3_64bits(function, function[1] + function[2])`) is the canonical entry — we already use this in LibertyRecomp's `CreateVertexShader` wrapper.
