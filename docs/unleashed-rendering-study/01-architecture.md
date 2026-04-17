# UnleashedRecomp Rendering Architecture (Study 01 of 5)

Source: `Reference Projects/UnleashedRecomp-main/UnleashedRecomp/gpu/video.cpp` (7882 lines) and `video.h` (413 lines). Line numbers below reference those files unchanged.

## 1. Diagram-style overview

```
                         GUEST (PPC recomp) SIDE                             HOST SIDE
 ----------------------------------------------------------    -----------------------------------------------
                                                              
 Guest "Present" thread -- sub_824ECA00 gate --+            +--> g_renderThread (single, std::thread)
    (= the thread that first calls             |    PUSH    |      moodycamel::BlockingConcurrentQueue
     sub_824ECA00 and is latched as            |            |      wait_dequeue_bulk (32 commands at a time)
     g_presentThreadId at line 747)            |            |      
                                               |            |      big switch(cmd.type)  -> Proc<X>(cmd)   (5267-5297)
 Draw*/Lock*/Unlock*/Set* hooks ---------------+            |         |                                     
   -> build RenderCommand on stack                          |         +--> FlushRenderStateForRenderThread()
   -> LocalRenderCommandQueue (up to 20)                    |         +--> barrier tracking (g_barrierMap)
   -> .submit() => g_renderQueue.enqueue_bulk -------------->         +--> g_commandLists[g_frame]->...  (Plume)
                                                                     +--> ExecuteCommandList -> g_queue
                                                                                                  |
                                                             (fence)     g_commandFences[g_frame] | execute
                                                                                                  v
 Video::Present (hook of sub_82BDA8C0)                               +----- RenderCommandQueue (DIRECT) -----
   enqueue ExecutePendingStretchRectCommands                         |  g_queue: DIRECT queue
   enqueue ExecuteCommandList    -- WAIT on g_executedCommandList    |  g_copyQueue: COPY queue (g_copyMutex)
   swap-chain present; g_frame ^= 1; g_commandFences[g_frame] wait   |  g_swapChain : RenderSwapChain
   enqueue BeginCommandList                                          +------------------------------------------
```

## 2. Threading model + fence points

Three threads of consequence:

| thread | role | where |
|-|-|-|
| "Present thread" (guest) | The guest thread that happens to first execute `sub_824ECA00`. From that point, **only this thread is allowed to submit render commands** (asserted). | 739-749, 2182, 2278 |
| Render thread | Single `std::thread` that dequeues `RenderCommand`s and actually talks to Plume. Bumped to `THREAD_PRIORITY_ABOVE_NORMAL` on Win32 and named "Render Thread". | 5249-5300 |
| Pipeline compiler thread(s) | Async PSO builders (out of scope for this study). | 403-437 |

Key global for thread ownership:

```cpp
// video.cpp:739-749
static std::thread::id g_presentThreadId = std::this_thread::get_id();
static std::atomic<bool> g_readyForCommands;

PPC_FUNC_IMPL(__imp__sub_824ECA00);
PPC_FUNC(sub_824ECA00)
{
    // Guard against thread ownership changes when between command lists.
    g_readyForCommands.wait(false);
    g_presentThreadId = std::this_thread::get_id();
    __imp__sub_824ECA00(ctx, base);
}
```

Fence points (synchronization that LibertyRecomp needs to mirror verbatim):

| fence | what it serializes | code |
|-|-|-|
| `g_readyForCommands` (atomic bool, wait/notify) | Prevents guest threads from touching state *between* `BeginCommandList`/`ExecuteCommandList` when command list is not open. Set true at end of `BeginCommandList` (1632-1633), false at top of `Present` (2793). | 740, 746, 1632-1633, 2793 |
| `g_executedCommandList` (atomic bool) | `Video::Present` blocks the guest until the render thread has actually submitted the frame's command list. | 2789, 2811-2812, 2999-3000 |
| `g_commandFences[g_frame]` (RenderCommandFence, NUM_FRAMES=2) | Per-frame fence: after rotating `g_frame`, **wait on the new frame's fence** before reusing its command list / upload allocator. | 2835, 2083, 2092 |
| `g_commandListStates[NUM_FRAMES]` | Bool flag: "this frame's fence is actually signalable" (we issued a command list on it). Avoids waiting on never-used fences. | 2081-2084, 2832-2843, 2997 |
| `g_copyCommandFence` + `g_copyMutex` | Serializes the COPY queue. `ExecuteCopyCommandList` always: lock -> begin -> f() -> end -> execute -> wait. Synchronous. | 1330-1339 |
| `g_acquireSemaphores[NUM_FRAMES]` / `g_renderSemaphores[NUM_FRAMES]` | Swap-chain Vulkan-style semaphores. Acquire is waited before the frame command list runs, render is signalled for present. | 324-325, 1882-1886, 2823-2824, 2983-2989 |
| `g_pendingWaitOnSwapChain` | Debouncer so a loading thread that calls `WaitOnSwapChain()` cannot double-wait with the main-thread Present. | 2765-2786 |

```
Frame N on render thread:
    ProcBeginCommandList(cmd)
        DestructTempResources();                         // free last frame's stuff on THIS frame slot
        BeginCommandList();                              // reset PSO, open commandList, signal g_readyForCommands
    ... draw commands ...
    ProcExecuteCommandList(cmd)
        FlushBarriers() / AddBarrier(backBuffer, PRESENT)
        commandList->end();
        g_queue->executeCommandLists(cmd, g_commandFences[g_frame])   // <- fence posts here
        g_commandListStates[g_frame] = true;
        g_executedCommandList = true; notify_one();      // <- Present() unblocks

Present (on guest present thread):
    g_readyForCommands = false
    enqueue ExecutePendingStretchRectCommands
    DrawImGui()                     // enqueues ImGui commands into g_renderQueue
    enqueue ExecuteCommandList
    g_executedCommandList.wait(false); g_executedCommandList = false
    g_swapChain->present(..., signalSemaphores);
    g_frame = g_nextFrame; g_nextFrame = (g_frame+1) % NUM_FRAMES;
    if (g_commandListStates[g_frame]) g_queue->waitForCommandFence(g_commandFences[g_frame])
    g_dirtyStates = DirtyStates(true);
    g_uploadAllocators[g_frame].reset();
    g_intermediaryUploadAllocator.reset();
    enqueue BeginCommandList
```

Reference: `Video::Present` at 2791-2880, `ProcExecuteCommandList` at 2897-3001.

## 3. Command enqueue / dequeue mechanics

### 3.1 The queue itself

A single `moodycamel::BlockingConcurrentQueue<RenderCommand>` (header-only MPSC/MPMC). Declared at line 1006:

```cpp
// video.cpp:1006
static moodycamel::BlockingConcurrentQueue<RenderCommand> g_renderQueue;
```

### 3.2 The command struct

`RenderCommandType` enum (28 variants; 811-842):

```
SetRenderState, DestructResource, UnlockTextureRect, UnlockBuffer16, UnlockBuffer32,
DrawImGui, ExecuteCommandList, BeginCommandList, StretchRect, SetRenderTarget,
SetDepthStencilSurface, ExecutePendingStretchRectCommands, Clear, SetViewport,
SetTexture, SetScissorRect, SetSamplerState, SetBooleans,
SetVertexShaderConstants, SetPixelShaderConstants, AddPipeline,
DrawPrimitive, DrawIndexedPrimitive, DrawPrimitiveUP,
SetVertexDeclaration, SetVertexShader, SetStreamSource, SetIndices, SetPixelShader
```

`RenderCommand` is a tagged union (844-1004). Entirely POD / trivially copyable. Pointer payloads (textures, buffers, shader memory) are either **heap-owned guest resources** that persist for the frame, or **uploaded into `g_intermediaryUploadAllocator`** (for shader constants/vertex-UP data) so the render thread reads a stable snapshot — never the live guest memory.

### 3.3 Two enqueue paths

**Simple one-shot** (used by idempotent Set calls, Clear, StretchRect, ...):

```cpp
// video.cpp:3324-3332  (SetRenderTarget - typical pattern)
static void SetRenderTarget(GuestDevice* device, uint32_t index, GuestSurface* renderTarget) 
{
    RenderCommand cmd;
    cmd.type = RenderCommandType::SetRenderTarget;
    cmd.setRenderTarget.renderTarget = renderTarget;
    g_renderQueue.enqueue(cmd);

    SetDefaultViewport(device, renderTarget);
}
```

**Batched via `LocalRenderCommandQueue`** (used by every Draw* path so shader-constant uploads are atomic with the draw). Declared at 4280-4295:

```cpp
// video.cpp:4280
struct LocalRenderCommandQueue
{
    RenderCommand commands[20];
    uint32_t count = 0;

    RenderCommand& enqueue() { assert(count < std::size(commands)); return commands[count++]; }
    void submit() { g_renderQueue.enqueue_bulk(commands, count); }
};
```

Used by `DrawPrimitive`, `DrawIndexedPrimitive`, `DrawPrimitiveUP` (4611-4697). The pattern is always:

```
LocalRenderCommandQueue queue;
FlushRenderStateForMainThread(device, queue);   // dirtyFlags scan -> SetBooleans / SetSamplerState / SetVertex|PixelShaderConstants
queue.enqueue() /* the draw */
queue.submit();                                 // single enqueue_bulk to g_renderQueue
```

Why batch: `FlushRenderStateForMainThread` (4297-4360) scans guest `device->dirtyFlags[0..4]`, serializes vertex/pixel float constants and sampler states into `g_intermediaryUploadAllocator`, and emits several `Set*Constants` commands **immediately before** the draw. Batching guarantees nothing else slips between them on the render thread.

### 3.4 The dequeue loop (render thread)

```cpp
// video.cpp:5249-5300
static std::thread g_renderThread([]
{
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
    GuestThread::SetThreadName(GetCurrentThreadId(), "Render Thread");

    RenderCommand commands[32];

    while (true)
    {
        size_t count = g_renderQueue.wait_dequeue_bulk(commands, std::size(commands));

        for (size_t i = 0; i < count; i++)
        {
            auto& cmd = commands[i];
            switch (cmd.type)
            {
            case RenderCommandType::SetRenderState:    ProcSetRenderState(cmd); break;
            case RenderCommandType::DestructResource:  ProcDestructResource(cmd); break;
            /* ... all 28 variants ... */
            case RenderCommandType::SetPixelShader:    ProcSetPixelShader(cmd); break;
            default: assert(false && "Unrecognized render command type."); break;
            }
        }
    }
});
```

Note: the render thread is **never joined**. It's a singleton daemon that runs for the process lifetime. No shutdown path.

### 3.5 Proc* handlers

Each `RenderCommandType::X` has a matching `static void ProcX(const RenderCommand& cmd)`. Typical categories:

- **Dirty-flag setters** (ProcSetRenderState, ProcSetTexture, ProcSetRenderTarget, ProcSetDepthStencilSurface, ProcSetViewport, ProcSetScissorRect, ProcSetSamplerState, ProcSetBooleans, ProcSetStreamSource, ProcSetIndices, ProcSetVertexDeclaration, ProcSetVertexShader, ProcSetPixelShader, ProcSetVertexShaderConstants, ProcSetPixelShaderConstants): mutate `g_pipelineState`, `g_textures`, `g_vertexBufferViews`, `g_sharedConstants` and flag the corresponding `g_dirtyStates.*` bit via `SetDirtyValue`. **No GPU work.**
- **Barrier + upload** (ProcUnlockTextureRect 2190-2206, ProcUnlockBuffer16/32 2292-2299, ProcStretchRect 3265-3299): add a `RenderTextureBarrier` into `g_barrierMap` and copy bytes into `g_uploadAllocators[g_frame]` then `copyTextureRegion`/`copyBufferRegion` on `g_commandLists[g_frame]`.
- **Draws** (ProcDrawPrimitive 4625, ProcDrawIndexedPrimitive 4668, ProcDrawPrimitiveUP 4699): call `FlushRenderStateForRenderThread()` (4457-4551) -> `g_commandLists[g_frame]->drawInstanced / drawIndexedInstanced`.
- **Lifecycle** (ProcBeginCommandList 3003-3007, ProcExecuteCommandList 2897-3001).
- **AddPipeline** 4433-4452: async PSO compiler -> render thread assignment.

## 4. Upload allocators + per-frame resource management

### 4.1 `UploadAllocator` (per-frame, GPU-visible)

```cpp
// video.cpp:456-529
struct UploadBuffer
{
    static constexpr size_t SIZE = 16 * 1024 * 1024;  // 16 MiB
    std::unique_ptr<RenderBuffer> buffer;
    uint8_t* memory = nullptr;
    uint64_t deviceAddress = 0;
};

struct UploadAllocator
{
    std::vector<UploadBuffer> buffers;
    uint32_t index = 0;
    uint32_t offset = 0;

    UploadAllocation allocate(uint32_t size, uint32_t alignment) { /* bump, roll into next buffer on overflow */ }
    template<bool TByteSwap, typename T>
    UploadAllocation allocate(const T* memory, uint32_t size, uint32_t alignment) { /* copy + optional ByteSwap */ }

    void reset() { index = 0; offset = 0; }
};

static UploadAllocator g_uploadAllocators[NUM_FRAMES];   // 531
```

On every `allocate`, if we overflow the current 16 MiB slab we just bump `index` and allocate another 16 MiB GPU upload buffer on demand — so the allocator grows indefinitely per frame but never shrinks. `reset()` is called from `Present` (2846) and simply rewinds `index=0, offset=0`, leaving the slabs allocated for reuse next rotation.

Hot-path users: shader constants (4522, 4528, 4534), DrawPrimitiveUP vertex stream (4710), UnlockTextureRect (2200), ProcUnlockBuffer (the inline path), gamma-correction push-constants (2962). Same allocator underlies *all* transient vertex/index/constant uploads.

**Byte-swap-on-copy** (`template<bool TByteSwap>`): swaps PPC big-endian constants on the way into the buffer in-place, avoiding a second staging copy.

### 4.2 `IntermediaryUploadAllocator` (CPU-only, cross-thread)

```cpp
// video.cpp:533-578
struct IntermediaryUploadAllocator
{
    static constexpr size_t SIZE = 16 * 1024 * 1024;
    std::vector<std::unique_ptr<uint8_t[]>> buffers;
    uint32_t index = 0;
    uint32_t offset = 0;

    uint8_t* allocate(uint32_t size);
    uint8_t* allocate(const void* memory, uint32_t size);
    void reset();
};
static IntermediaryUploadAllocator g_intermediaryUploadAllocator;
```

Purpose: the **main thread** may not touch `g_uploadAllocators[g_frame]` (that belongs to the render thread and is tied to a live GPU buffer). But `FlushRenderStateForMainThread` has to snapshot `device->vertexShaderFloatConstants` / `pixelShaderFloatConstants` / DrawPrimitiveUP vertex data *before* enqueueing the draw. This plain-`uint8_t[]` allocator is that snapshot store; the pointer in the `RenderCommand` payload points into it, and the render thread copies/byte-swaps into the real upload allocator at Proc* time.

Reset from `Present` (2847) in lock-step with the GPU upload allocator.

### 4.3 Per-frame GC

```cpp
// video.cpp:580-581, 670-737
static std::vector<GuestResource*> g_tempResources[NUM_FRAMES];
static std::vector<std::unique_ptr<RenderBuffer>> g_tempBuffers[NUM_FRAMES];

static void DestructTempResources();  // called from ProcBeginCommandList, line 3005
```

`DestructResource` from the guest enqueues onto `g_renderQueue`; `ProcDestructResource` pushes to `g_tempResources[g_frame]`; the **next** rotation's `BeginCommandList` walks the list and actually frees. This guarantees a resource being referenced by an in-flight GPU frame is not torn down until that frame's fence signals.

### 4.4 Descriptor heap management (bindless)

```cpp
// video.cpp:321-322, 384, 1321-1322
static constexpr size_t TEXTURE_DESCRIPTOR_SIZE = 65536;
static constexpr size_t SAMPLER_DESCRIPTOR_SIZE = 1024;

struct TextureDescriptorAllocator {
    Mutex mutex;
    uint32_t capacity = TEXTURE_DESCRIPTOR_NULL_COUNT;   // starts at 3 (NULL_TEXTURE_2D/3D/CUBE)
    std::vector<uint32_t> freed;                         // free-list
    uint32_t allocate();                                 // pop freed or ++capacity
    void free(uint32_t value);
};
static TextureDescriptorAllocator g_textureDescriptorAllocator;
```

Single monolithic descriptor set sized for 65536 textures, created once (1891-1896). Every `GuestTexture` / `GuestSurface` gets an `uint32_t descriptorIndex`; samplers likewise. Indices are exposed to shaders via `SharedConstants.texture2DIndices[16]`, `textureCubeIndices[16]`, `samplerIndices[16]`. **No per-draw descriptor table binding** — the 3 texture descriptor sets + 1 sampler descriptor set are bound once in `BeginCommandList` (1627-1630) and never rebound.

Per-pass constants reach the shader either via push constants (Vulkan) or root descriptors (D3D12), uploaded into `g_uploadAllocators[g_frame]` and pointed-to via `SetRootDescriptor` (2887-2895).

## 5. Complete list of guest-hooked D3D9 entry points

From `video.cpp:7798-7882`. All hooks are `GUEST_FUNCTION_HOOK(sub_XXXXXXXX, HostFn)` or `GUEST_FUNCTION_STUB(sub_XXXXXXXX)`.

| Guest sub | Host fn | Purpose |
|-|-|-|
| sub_82BD99B0 | CreateDevice | Device creation, guest-memory back buffer init |
| sub_82BE6230 | DestructResource | Enqueue for deferred destruction |
| sub_82BE9300 | LockTextureRect | Sync, returns guest-mapped pointer |
| sub_82BE7780 | UnlockTextureRect | Enqueues upload to GPU |
| sub_82BE6B98 | LockVertexBuffer | Sync |
| sub_82BE6BE8 | UnlockVertexBuffer | Enqueues UnlockBuffer32 |
| sub_82BE61D0 | GetVertexBufferDesc | Sync, fills desc |
| sub_82BE6CA8 | LockIndexBuffer | Sync |
| sub_82BE6CF0 | UnlockIndexBuffer | Enqueues UnlockBuffer16 or 32 |
| sub_82BE6200 | GetIndexBufferDesc | Sync |
| sub_82BE96F0 | GetSurfaceDesc | Sync |
| sub_82BE04B0 | GetVertexDeclaration | Sync |
| sub_82BE0530 | HashVertexDeclaration | Sync |
| sub_82BDA8C0 | Video::Present | Full frame rotation |
| sub_82BDD330 | GetBackBuffer | Sync, returns `g_backBuffer` (AddRef) |
| sub_82BE9498 | CreateTexture | Sync, allocator only |
| sub_82BE6AD0 | CreateVertexBuffer | Sync |
| sub_82BE6BF8 | CreateIndexBuffer | Sync |
| sub_82BE95B8 | CreateSurface | Sync, allocates RT/DS + descriptor slot |
| sub_82BF6400 | StretchRect | Enqueues |
| sub_82BDD9F0 | SetRenderTarget | Enqueues + SetDefaultViewport |
| sub_82BDDD38 | SetDepthStencilSurface | Enqueues + SetDefaultViewport |
| sub_82BFE4C8 | Clear | Enqueues |
| sub_82BDD8C0 | SetViewport | Enqueues |
| sub_82BE9818 | SetTexture | Enqueues (handles PS-button patched textures) |
| sub_82BDCFB0 | SetScissorRect | Enqueues |
| sub_82BE5900 | DrawPrimitive | Batched via LocalRenderCommandQueue |
| sub_82BE5CF0 | DrawIndexedPrimitive | Batched |
| sub_82BE52F8 | DrawPrimitiveUP | Batched |
| sub_82BE0428 | CreateVertexDeclaration | Sync |
| sub_82BE02E0 | SetVertexDeclaration | Enqueues |
| sub_82BE1A80 | CreateVertexShader | Sync |
| sub_82BE0110 | SetVertexShader | Enqueues |
| sub_82BDD0F8 | SetStreamSource | Enqueues |
| sub_82BDD218 | SetIndices | Enqueues |
| sub_82BE1990 | CreatePixelShader | Sync |
| sub_82BDFE58 | SetPixelShader | Enqueues (with gaussian-blur / motion-blur shader substitution) |
| sub_82C003B8 | D3DXFillTexture | Sync, uses COPY queue |
| sub_82C00910 | D3DXFillVolumeTexture | Sync, uses COPY queue |
| sub_82E43FC8 | MakePictureData | Engine glue |
| sub_82E9EE38 | SetResolution | Engine glue |
| sub_82AE2BF8 | ScreenShaderInit | Engine glue |

Stubbed (no-op): `sub_82BAAD38` (buggy framebuffer recreation), `sub_822C15D8`, `sub_822C1810`, `sub_82BD97A8`, `sub_82BD97E8`, `sub_82BDD370` (SetGammaRamp), `sub_82BE05B8`, `sub_82BE9C98`, `sub_82BEA308`, `sub_82CD5D68`, `sub_82BE9B28`, `sub_82BEA018`, `sub_82BEA7C0`, `sub_82BFFF88` (D3DXFilterTexture), `sub_82BD96D0`.

Also note the **SetRenderState thunking** at 2123-2135: Unleashed appends synthetic host functions past `PPC_CODE_BASE + PPC_CODE_SIZE` and writes their addresses into `GuestDevice::setRenderStateFunctions[0x65]` so the guest's per-state dispatch table invokes `SetRenderState<D3DRS_*>` wrappers directly:

```cpp
// video.cpp:2124-2135
uint32_t functionOffset = PPC_CODE_BASE + PPC_CODE_SIZE;
g_memory.InsertFunction(functionOffset, HostToGuestFunction<SetRenderStateUnimplemented>);

for (size_t i = 0; i < std::size(device->setRenderStateFunctions); i++)
    device->setRenderStateFunctions[i] = functionOffset;

for (auto& [state, function] : g_setRenderStateFunctions)
{
    functionOffset += 4;
    g_memory.InsertFunction(functionOffset, function);
    device->setRenderStateFunctions[state / 4] = functionOffset;
}
```

This is how `D3DRS_ZENABLE`, `D3DRS_CULLMODE` etc. flow without each needing a `GUEST_FUNCTION_HOOK`.

## 6. Key globals and types

### 6.1 Pipeline / draw state (mutated only on render thread)

```cpp
// video.cpp:122-149
struct PipelineState
{
    GuestShader* vertexShader = nullptr;
    GuestShader* pixelShader = nullptr;
    GuestVertexDeclaration* vertexDeclaration = nullptr;
    bool instancing = false;
    bool zEnable = true;
    bool zWriteEnable = true;
    RenderBlend srcBlend = RenderBlend::ONE, destBlend = RenderBlend::ZERO;
    RenderCullMode cullMode = RenderCullMode::NONE;
    RenderComparisonFunction zFunc = RenderComparisonFunction::LESS;
    bool alphaBlendEnable = false;
    RenderBlendOperation blendOp = RenderBlendOperation::ADD;
    float slopeScaledDepthBias = 0.0f;
    int32_t depthBias = 0;
    RenderBlend srcBlendAlpha, destBlendAlpha;
    RenderBlendOperation blendOpAlpha = RenderBlendOperation::ADD;
    uint32_t colorWriteEnable = uint32_t(RenderColorWriteEnable::ALL);
    RenderPrimitiveTopology primitiveTopology = RenderPrimitiveTopology::TRIANGLE_LIST;
    uint8_t vertexStrides[16]{};
    RenderFormat renderTargetFormat{}, depthStencilFormat{};
    RenderSampleCounts sampleCount = RenderSampleCount::COUNT_1;
    bool enableAlphaToCoverage = false;
    uint32_t specConstants = 0;
};

// video.cpp:177-193
static GuestSurface* g_renderTarget;
static GuestSurface* g_depthStencil;
static RenderFramebuffer* g_framebuffer;
static RenderViewport g_viewport(0.0f, 0.0f, 1280.0f, 720.0f);
static PipelineState g_pipelineState;
static int32_t g_depthBias;
static float g_slopeScaledDepthBias;
static uint32_t g_vertexShaderConstants[0x400];   // 1024 uint32 (256 float4 slots)
static uint32_t g_pixelShaderConstants[0x380];    // 896 uint32 (224 float4 slots)
static SharedConstants g_sharedConstants;
static GuestTexture* g_textures[16];
static RenderSamplerDesc g_samplerDescs[16];
static bool g_scissorTestEnable = false;
static RenderRect g_scissorRect;
static RenderVertexBufferView g_vertexBufferViews[16];
static RenderInputSlot g_inputSlots[16];
static RenderIndexBufferView g_indexBufferView({}, 0, RenderFormat::R16_UINT);
```

`SharedConstants` (160-171): texture2D/3D/Cube descriptor indices, sampler indices, booleans, swapped-texcoord bits, half-pixel offset, alpha threshold — uploaded as cbuffer #2.

### 6.2 Dirty-state bitmask

```cpp
// video.cpp:195-225
struct DirtyStates
{
    bool renderTargetAndDepthStencil;
    bool viewport;
    bool pipelineState;
    bool depthBias;
    bool sharedConstants;
    bool scissorRect;
    bool vertexShaderConstants;
    uint8_t vertexStreamFirst;
    uint8_t vertexStreamLast;
    bool indices;
    bool pixelShaderConstants;
    DirtyStates(bool value);       // all-on or all-off constructor
};
static DirtyStates g_dirtyStates(true);

template<typename T>
static void SetDirtyValue(bool& dirtyState, T& dest, const T& src)
{
    if (dest != src) { dest = src; dirtyState = true; }
}
```

On every `Present` (2845), `g_dirtyStates = DirtyStates(true)` — forcing a full re-emit at the top of the new frame. All Proc\* write through `SetDirtyValue` to cheaply skip no-ops.

### 6.3 Command list / fence / queue

```cpp
// video.cpp:302-327
static constexpr size_t NUM_FRAMES = 2;
static constexpr size_t NUM_QUERIES = 2;

static uint32_t g_frame = 0;
static uint32_t g_nextFrame = 1;

static std::unique_ptr<RenderCommandQueue> g_queue;                         // DIRECT
static std::unique_ptr<RenderCommandList> g_commandLists[NUM_FRAMES];
static std::unique_ptr<RenderCommandFence> g_commandFences[NUM_FRAMES];
static std::unique_ptr<RenderQueryPool>   g_queryPools[NUM_FRAMES];
static bool g_commandListStates[NUM_FRAMES];

static Mutex g_copyMutex;
static std::unique_ptr<RenderCommandQueue> g_copyQueue;                     // COPY
static std::unique_ptr<RenderCommandList>  g_copyCommandList;
static std::unique_ptr<RenderCommandFence> g_copyCommandFence;

static std::unique_ptr<RenderSwapChain> g_swapChain;
static bool g_swapChainValid;
static constexpr RenderFormat BACKBUFFER_FORMAT = RenderFormat::B8G8R8A8_UNORM;

static std::unique_ptr<RenderCommandSemaphore> g_acquireSemaphores[NUM_FRAMES];
static std::unique_ptr<RenderCommandSemaphore> g_renderSemaphores[NUM_FRAMES];
```

`g_commandLists[g_frame]` is the **only** command list any Proc* touches. Never written to from the guest thread. Each frame the render thread calls `commandList->begin()` (1623), accumulates all commands (barriers, draws, copies, imgui), then `commandList->end()` (2978) and `g_queue->executeCommandLists(...)` (2986/2994) — exactly once per guest `Present()`.

### 6.4 Barrier tracking

```cpp
// video.cpp:751-776
static ankerl::unordered_dense::map<RenderTexture*, RenderTextureLayout> g_barrierMap;
static std::vector<RenderTextureBarrier> g_barriers;

static void AddBarrier(GuestBaseTexture* texture, RenderTextureLayout layout);
static void FlushBarriers();
```

Coalesced per-texture. `FlushBarriers()` packs the map into a single `g_commandLists[g_frame]->barriers(GRAPHICS|COPY, ...)` call. Called from every `Proc*` that changes a texture's conceptual layout — driven entirely from the render thread where `texture->layout` is authoritative.

## 7. Backends (Plume abstraction)

```cpp
// video.cpp:101-119
namespace plume {
#ifdef UNLEASHED_RECOMP_D3D12
    extern std::unique_ptr<RenderInterface> CreateD3D12Interface();
#endif
#ifdef SDL_VULKAN_ENABLED
    extern std::unique_ptr<RenderInterface> CreateVulkanInterface(RenderWindow sdlWindow);
#else
    extern std::unique_ptr<RenderInterface> CreateVulkanInterface();
#endif
}
```

`plume_render_interface.h` supplies `RenderInterface`, `RenderDevice`, `RenderCommandQueue`, `RenderCommandList`, `RenderCommandFence`, `RenderCommandSemaphore`, `RenderSwapChain`, `RenderTexture`, `RenderTextureView`, `RenderBuffer`, `RenderSampler`, `RenderPipeline`, `RenderPipelineLayout`, `RenderDescriptorSet`, `RenderQueryPool`, `RenderFormat`, `RenderBlend`, etc. Only **one** include in `video.h`:

```cpp
// video.h:7
#include <plume_render_interface.h>
```

Backend selection in `Video::CreateHostDevice` (1663-1805):

1. Build vector of `RenderInterfaceFunction*` candidates.
2. On Windows with `UNLEASHED_RECOMP_D3D12`, try D3D12 first then Vulkan (or vice versa, configurable).
3. Under Wine, auto-select Vulkan (1675).
4. Wrap `interfaceFunction()` and `createDevice` in `__try/__except` (1707) — on driver crash, restart the process with `--graphics-api-retry` (1787).
5. Apply per-vendor workarounds: AMD old-driver MSAA-resolve bug, Intel D3D12 bugs, AMD triangle-strip restart-index workaround (`g_triangleStripWorkaround` 1771).
6. `g_capabilities = g_device->getCapabilities()` feeds dynamic-depth-bias / GPU-upload-heap / presentWait / triangleFan / UMA checks.

So the **game's guest D3D9 surface never touches the actual backend** — only Plume's `RenderCommand*` / `Render*` abstractions. Swapping D3D12 for Vulkan is a matter of which `CreateXInterface()` returns a non-null `RenderInterface`.

The Plume clone lives in `Reference Projects/UnleashedRecomp-main/thirdparty/plume/` (confirmed present). Spec constants, push-constant vs root-descriptor path, and triangle-fan / triangle-strip / dynamic depth-bias all key off `g_capabilities` / `g_vulkan` at render-thread time.

## 8. Per-frame lifecycle

```
Process start
    g_renderThread std::thread launched (5249)  -- waits on g_renderQueue
    Video::CreateHostDevice():
        create interface -> device
        create g_queue (DIRECT)
        create g_commandLists[2], g_commandFences[2], g_queryPools[2]
        create g_copyQueue, g_copyCommandList, g_copyCommandFence
        create g_swapChain (2 or 3 buffers)
        create g_acquireSemaphores[2], g_renderSemaphores[2]
        build g_pipelineLayout + g_textureDescriptorSet (65536) + g_samplerDescriptorSet (1024)
        init null textures (TEXTURE_DESCRIPTOR_NULL_TEXTURE_2D/3D/CUBE)
        CheckSwapChain(); BeginCommandList();      <-- frame 0 already open before CreateDevice hook fires

Every frame (main/present guest thread):
    guest calls Video::Present (hook of sub_82BDA8C0):
        g_readyForCommands = false
        enqueue ExecutePendingStretchRectCommands
        DrawImGui()                                -- enqueues ImGui drawing
        enqueue ExecuteCommandList
        g_executedCommandList.wait(false); reset   -- BLOCKS UNTIL RENDER THREAD SUBMITTED

        g_swapChain->wait() if pending             -- frame pacing
        g_swapChain->present(backBufferIndex, renderSemaphores)
        g_frame = g_nextFrame; g_nextFrame = (g_frame+1) % 2

        if (g_commandListStates[g_frame])
            g_queue->waitForCommandFence(g_commandFences[g_frame])   -- WAIT for NEW frame's prior fence
            queryPool->queryResults() -> GPU-time profiler

        g_dirtyStates = DirtyStates(true)          -- re-emit everything
        g_uploadAllocators[g_frame].reset()
        g_intermediaryUploadAllocator.reset()
        g_triangleFanIndexData.reset(); g_quadIndexData.reset()

        CheckSwapChain()                           -- acquireSemaphore + texture
        enqueue BeginCommandList                   -- tells render thread to open next CL

Every frame (render thread):
    ... many dirty-state / draw / barrier commands ...
    RenderCommandType::ExecuteCommandList:
        ProcExecuteCommandList:
            if swap chain valid: blit to swap chain (gamma correction)
            barrier backBuffer -> PRESENT
            commandList->writeTimestamp(1); commandList->end()
            g_queue->executeCommandLists(cmd, acquireSem, renderSem, g_commandFences[g_frame])
            g_commandListStates[g_frame] = true
            g_executedCommandList = true; notify_one()

    RenderCommandType::BeginCommandList:
        ProcBeginCommandList:
            DestructTempResources()                -- free things the GPU is now done with
            BeginCommandList():
                g_renderTarget = g_backBuffer; g_depthStencil = null
                pick intermediary-or-direct backbuffer texture
                commandList->begin()
                resetQueryPool; writeTimestamp(0)
                setGraphicsPipelineLayout / setGraphicsDescriptorSet (×4, including 3 texture + 1 sampler)
                g_readyForCommands = true; notify_one()
```

Worth highlighting: `BeginCommandList` is called **before** `CreateDevice` is hooked (2060-2061), because `g_backBuffer` / `g_backBufferHolder` exists as a host-memory placeholder and is later migrated to guest memory inside `CreateDevice` (2110-2118). Stale pointer is patched at 2114-2115.

## 9. Key differences vs LibertyRecomp's partial implementation

(Detectable from a shallow scan of `LibertyRecomp/gpu/video.cpp`, 10245 lines; deep comparison is later studies in this series.)

1. **Duplicate / conflicting hooks.** Unleashed has exactly **one** `GUEST_FUNCTION_HOOK(sub_XXX, Fn)` per entry point. LibertyRecomp's `video.cpp` has multiple lines declaring the same guest sub to different host functions — e.g. `GUEST_FUNCTION_HOOK(sub_82A44850, GTAIV_CreateTexture)` (9190) and `GUEST_FUNCTION_HOOK(sub_82A44850, CreateTexture)` (9308). The recomp codegen resolves this non-deterministically and will call whichever was linked last, producing the unpredictable texture crashes. Port Unleashed's "one hook per guest sub" discipline exactly.
2. **Same command queue library already present.** Both codebases use `moodycamel::BlockingConcurrentQueue<RenderCommand>` with a single daemon `g_renderThread` running `wait_dequeue_bulk` on 32-element bursts. Unleashed's `LocalRenderCommandQueue` batch pattern (max 20 commands) for draws is already copied in LibertyRecomp — so the skeleton is intact, the bugs are in the per-hook handlers.
3. **Thread-ownership gate.** Unleashed latches `g_presentThreadId` to whichever guest thread first enters `sub_824ECA00` and asserts on every Unlock\* that the guest caller matches. LibertyRecomp must find the equivalent guest-side "render-thread init" sub and apply the identical pattern; missing this assert lets worker threads race `UnlockTextureRect` / `UnlockBuffer` and corrupt the upload allocator.
4. **`g_readyForCommands` vs `g_executedCommandList` handshake.** These two atomics — latched false on guest `Present`, latched true by `ProcExecuteCommandList` / `BeginCommandList` — are the *only* correctness-critical sync between guest and render thread beyond the frame fence. LibertyRecomp's half-hook layer crashes because state mutation between frames races the render thread while a command list is closed; porting these two atomics verbatim is prerequisite to any other fix.
5. **Descriptor-heap model.** Unleashed uses a single 65536-slot bindless texture descriptor set, bound ONCE per command list in `BeginCommandList`. Per-draw binding is only constants (`SetRootDescriptor` -> push-constant or root-descriptor). LibertyRecomp should verify it is not rebinding descriptor sets per draw (classic porting pitfall producing stutters and driver crashes on AMD / Intel).
6. **Per-frame GC via `g_tempResources[NUM_FRAMES]` + `DestructTempResources()`.** Unleashed never frees a guest-visible GPU resource synchronously — `DestructResource` is always enqueued, then processed one frame later at `ProcBeginCommandList` time, guaranteeing the fence for the in-flight frame has been waited on. If LibertyRecomp's `DestructResource` frees directly from the guest thread (common for partial ports), it will crash on any resource destroyed mid-draw.
7. **`PPC_FUNC_IMPL`/`PPC_FUNC` trampoline on `sub_824ECA00`** (Unleashed 742-749) is the only non-hook override in the entire renderer — it is not a `GUEST_FUNCTION_HOOK`, it *re-enters* `__imp__sub_824ECA00` after latching `g_presentThreadId`. Replicating this exact pattern (not a hook, a wrapped call) is subtle and easy to get wrong.
8. **`SetRenderState` table thunking** (Unleashed 2123-2135). Instead of hooking 65+ guest D3DRS_\* subs individually, Unleashed writes synthetic host-function addresses into `GuestDevice::setRenderStateFunctions[0x65]` past `PPC_CODE_BASE + PPC_CODE_SIZE`. If LibertyRecomp is hooking each D3DRS_\* sub individually it is burning hook slots and guaranteed to miss states; port the thunking model.

## 10. Numeric constants (Python-verified)

```
NUM_FRAMES                          = 2
UploadBuffer::SIZE                  = 16 MiB (16777216 bytes)
IntermediaryUploadAllocator::SIZE   = 16 MiB (16777216 bytes)
LocalRenderCommandQueue.commands[]  = 20 entries
render-thread wait_dequeue_bulk     = 32 commands
TEXTURE_DESCRIPTOR_SIZE             = 65536 bindless slots
SAMPLER_DESCRIPTOR_SIZE             = 1024
g_vertexShaderConstants             = 0x400  uint32_t = 1024 (256 float4)
g_pixelShaderConstants              = 0x380  uint32_t =  896 (224 float4)
g_textures / g_vertexBufferViews    = 16 each
PITCH_ALIGNMENT                     = 0x100
PLACEMENT_ALIGNMENT                 = 0x200
```
