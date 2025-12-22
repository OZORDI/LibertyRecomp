# Unleashed Recompiled: Technical Documentation

## A Post-Mortem and Technical Guide for Static Recompilation Projects

---

## Table of Contents

1. [Project Scope and Constraints](#1-project-scope-and-constraints)
2. [Initial Binary Analysis](#2-initial-binary-analysis)
3. [Platform Boundary Identification](#3-platform-boundary-identification)
4. [Bridging Platform-Specific Code](#4-bridging-platform-specific-code)
5. [Static Recompilation Strategy](#5-static-recompilation-strategy)
6. [Hooking and Native Code Interposition](#6-hooking-and-native-code-interposition)
7. [Adding Modern Features](#7-adding-modern-features)
8. [Debugging and Validation](#8-debugging-and-validation)
9. [Incremental Bring-Up Strategy](#9-incremental-bring-up-strategy)
10. [Lessons Learned](#10-lessons-learned)

---

## 1. Project Scope and Constraints

### 1.1 Target Platform and Architecture

**Source Platform:** Xbox 360 (Xenon CPU - PowerPC 64-bit, Xenos GPU - ATI R500-derivative)

**Target Platforms:** Windows (x86-64), Linux (x86-64)

**Original Binary Format:** XEX (Xbox Executable) with basic compression

The Xbox 360 uses a custom PowerPC architecture with:
- Big-endian byte order
- 32-bit virtual address space (mapped into 64-bit physical)
- VMX128 SIMD extensions (PowerPC AltiVec variant)
- Custom calling conventions with r1 (stack), r13 (TLS base)

### 1.2 Why Static Recompilation

Static recompilation was chosen over dynamic emulation for several reasons:

1. **Performance**: Eliminates interpreter/JIT overhead entirely
2. **Integration**: Allows direct modification of game logic
3. **Shader Handling**: Xbox 360 shaders can be pre-translated to HLSL/SPIR-V
4. **Determinism**: Compiled code behavior is consistent across runs
5. **Modern API Access**: Direct use of Vulkan/D3D12 without GPU emulation

The tradeoff is significant upfront development cost and game-specific adaptation.

### 1.3 Architectural Constraints

```
+------------------+     +------------------+     +------------------+
|   XEX Binary     | --> | XenonRecomp      | --> | C++ Source       |
|   (PowerPC)      |     | (Translator)     |     | (x86-64)         |
+------------------+     +------------------+     +------------------+
                                                          |
+------------------+     +------------------+              v
|   Xenos Shaders  | --> | XenosRecomp      | --> +------------------+
|   (Xbox GPU)     |     | (Translator)     |     | Native Binary    |
+------------------+     +------------------+     | + Runtime        |
                                                  +------------------+
```

**Key Constraints:**

| Constraint | Original (Xbox 360) | Solution |
|------------|---------------------|----------|
| Byte Order | Big-endian | `ByteSwap()` on all guest memory access |
| Address Space | 32-bit guest in 64-bit host | Base pointer offset translation |
| Calling Convention | PowerPC ABI (r3-r10, f1-f13) | `PPCContext` structure marshaling |
| Threading Model | Xbox kernel threads | Host `std::thread` + guest TLS emulation |
| Memory Model | Unified 512MB | 4GB mapped region at fixed address |
| GPU Commands | Xenos command buffer | Modern API translation layer |

### 1.4 Memory Layout

```
Host Address Space:
+------------------+ 0x100000000 (Preferred base)
|                  |
| Guest Memory     | <- 4GB mapped region
| (PPC_MEMORY_SIZE)|
|                  |
+------------------+ 0x200000000
|                  |
| Host Heap        |
|                  |
+------------------+

Guest Address Translation:
  host_ptr = g_memory.base + guest_addr
  guest_addr = (uint32_t)(host_ptr - g_memory.base)
```

The memory subsystem allocates a contiguous 4GB region:

```cpp
// memory.cpp
Memory::Memory()
{
#ifdef _WIN32
    base = (uint8_t*)VirtualAlloc((void*)0x100000000ull, PPC_MEMORY_SIZE, 
                                   MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
    base = (uint8_t*)mmap((void*)0x100000000ull, PPC_MEMORY_SIZE, 
                          PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
#endif
    // First page protected to catch null pointer dereferences
    mprotect(base, 4096, PROT_NONE);
}
```

---

## 2. Initial Binary Analysis

### 2.1 Tooling

The project uses two custom recompilers:

1. **XenonRecomp**: PowerPC to C++ translator
   - Repository: https://github.com/hedge-dev/XenonRecomp
   - Analyzes `.pdata` exception tables for function boundaries
   - Handles switch tables via configuration

2. **XenosRecomp**: Xenos shader to HLSL/SPIR-V translator
   - Repository: https://github.com/hedge-dev/XenosRecomp

Additional tools:
- **XenonAnalyse**: Static analysis for function discovery
- IDA Pro / Ghidra for manual reverse engineering

### 2.2 Entry Points and Boot Flow

```
main() [host]
    |
    v
KiSystemStartup()
    |-- Initialize g_userHeap
    |-- Mount content paths (game, update, DLC)
    |-- XAudioInitializeSystem()
    |
    v
LdrLoadModule(modulePath)
    |-- Load XEX binary
    |-- Decompress sections
    |-- Extract entry point
    |
    v
GuestThread::Start({ entry, 0, 0 })
    |-- Create PPCContext
    |-- Call recompiled entry function
    |
    v
[Game main loop runs as recompiled C++ code]
```

### 2.3 Executable Layout

XEX loading handles multiple compression types:

```cpp
// main.cpp - LdrLoadModule
if (fileFormatInfo->compressionType == XEX_COMPRESSION_NONE) {
    memcpy(destData, srcData, security->imageSize);
}
else if (fileFormatInfo->compressionType == XEX_COMPRESSION_BASIC) {
    // Block-based decompression with zero padding
    for (size_t i = 0; i < numBlocks; i++) {
        memcpy(destData, srcData, blocks[i].dataSize);
        destData += blocks[i].dataSize;
        memset(destData, 0, blocks[i].zeroSize);
        destData += blocks[i].zeroSize;
    }
}
```

### 2.4 Function Classification

Functions are classified through:

1. **Automatic Discovery**: Exception unwind data (`.pdata`)
2. **Manual Definition**: TOML configuration for edge cases
3. **Import Resolution**: Known Xbox kernel/XAM functions

```toml
# SWA.toml - Manual function definitions
functions = [
    { address = 0x824E7EF0, size = 0x98 },
    { address = 0x82C980E8, size = 0x110 },
    # ... functions with jump tables not analyzable automatically
]

invalid_instructions = [
    { data = 0x00000000, size = 4 },  # Padding
    { data = 0x831B1C90, size = 8 },  # C++ Frame Handler
]
```

---

## 3. Platform Boundary Identification

### 3.1 Subsystem Taxonomy

Platform-specific code was grouped into these subsystems:

```
+------------------------------------------------------------------+
|                        GAME LOGIC                                 |
|  (Recompiled C++ - Portable)                                     |
+------------------------------------------------------------------+
           |              |              |              |
           v              v              v              v
+----------+   +----------+   +----------+   +----------+
| Kernel   |   | Graphics |   | Audio    |   | Input    |
| Services |   | (GPU)    |   | (APU)    |   | (HID)    |
+----------+   +----------+   +----------+   +----------+
           |              |              |              |
           v              v              v              v
+------------------------------------------------------------------+
|                     HOST PLATFORM LAYER                          |
|  (Windows/Linux - Native implementations)                        |
+------------------------------------------------------------------+
```

### 3.2 Identified Platform APIs

**Kernel Services:**
- Thread creation/synchronization (`ExCreateThread`, `KeWaitForSingleObject`)
- Memory allocation (`MmAllocatePhysicalMemoryEx`)
- Critical sections/spinlocks (`RtlEnterCriticalSection`)
- Events/semaphores (`NtCreateEvent`, `KeSetEvent`)
- TLS (`KeTlsAlloc`, `KeTlsSetValue`)

**File System:**
- File operations (`XCreateFileA`, `XReadFile`, `XSetFilePointer`)
- Directory enumeration (`XFindFirstFileA`, `XFindNextFileA`)
- Content management (`XamContentCreateEx`)

**Graphics:**
- Device initialization (`VdInitializeEngines`)
- Command buffer operations (`VdSwap`)
- Resource creation (textures, buffers, shaders)

**Audio:**
- Render driver registration (`XAudioRegisterRenderDriverClient`)
- Frame submission (`XAudioSubmitRenderDriverFrame`)

**Input:**
- Gamepad state (`XInputGetState`)
- Capabilities query (`XInputGetCapabilities`)

### 3.3 Distinguishing Logic from Glue

Call pattern analysis identified platform glue:

```cpp
// Pattern: Direct kernel call with fixed signature
uint32_t XamContentCreateEx(uint32_t dwUserIndex, const char* szRootName, 
                            const XCONTENT_DATA* pContentData, ...);

// Pattern: Function pointer in virtual table
// → These are game logic, preserved through recompilation

// Pattern: Hardware register access (MMIO)
const size_t XMAIOBegin = 0x7FEA0000;
const size_t XMAIOEnd = XMAIOBegin + 0x0000FFFF;
// → Requires complete replacement with host implementation
```

---

## 4. Bridging Platform-Specific Code

### 4.1 File System Bridge

**Original Behavior:** Xbox content packages with virtual paths (`game:\`, `update:\`)

**Host Implementation:** VFS layer mapping to host filesystem

```cpp
// file_system.cpp
std::filesystem::path FileSystem::ResolvePath(const std::string_view& path, bool checkForMods)
{
    // Check mod loader first
    if (checkForMods) {
        std::filesystem::path resolvedPath = ModLoader::ResolvePath(path);
        if (!resolvedPath.empty())
            return resolvedPath;
    }

    // Parse Xbox-style path
    size_t index = path.find(":\\");
    if (index != std::string::npos) {
        std::string_view root = path.substr(0, index);
        
        // Redirect game:\work\ to update (title update has newer files)
        if (path.starts_with("game:\\work\\"))
            root = "update";
            
        const auto newRoot = XamGetRootPath(root);
        builtPath += newRoot;
        builtPath += '/';
        builtPath += path.substr(index + 2);
    }
    
    std::replace(builtPath.begin(), builtPath.end(), '\\', '/');
    return builtPath;
}
```

File handle abstraction:

```cpp
struct FileHandle : KernelObject
{
    std::fstream stream;
    std::filesystem::path path;
};

FileHandle* XCreateFileA(const char* lpFileName, uint32_t dwDesiredAccess, ...)
{
    std::filesystem::path filePath = FileSystem::ResolvePath(lpFileName, true);
    std::fstream fileStream;
    
    std::ios::openmode fileOpenMode = std::ios::binary;
    if (dwDesiredAccess & GENERIC_READ)
        fileOpenMode |= std::ios::in;
    if (dwDesiredAccess & GENERIC_WRITE)
        fileOpenMode |= std::ios::out;
        
    fileStream.open(filePath, fileOpenMode);
    
    FileHandle* fileHandle = CreateKernelObject<FileHandle>();
    fileHandle->stream = std::move(fileStream);
    fileHandle->path = std::move(filePath);
    return fileHandle;
}
```

### 4.2 Threading Bridge

**Original:** Xbox kernel threads with custom TEB/PCR structures

**Host Implementation:** Native threads with emulated guest thread context

```cpp
// guest_thread.cpp
constexpr size_t PCR_SIZE = 0xAB0;   // Processor Control Region
constexpr size_t TLS_SIZE = 0x100;   // Thread Local Storage
constexpr size_t TEB_SIZE = 0x2E0;   // Thread Environment Block
constexpr size_t STACK_SIZE = 0x40000;

GuestThreadContext::GuestThreadContext(uint32_t cpuNumber)
{
    thread = (uint8_t*)g_userHeap.Alloc(TOTAL_SIZE);
    memset(thread, 0, TOTAL_SIZE);
    
    // Setup PCR pointers
    *(uint32_t*)thread = ByteSwap(g_memory.MapVirtual(thread + PCR_SIZE));
    *(uint32_t*)(thread + 0x100) = ByteSwap(g_memory.MapVirtual(thread + TEB_OFFSET));
    *(thread + 0x10C) = cpuNumber;
    
    // Initialize PPC context
    ppcContext.r1.u64 = g_memory.MapVirtual(thread + TEB_OFFSET + TEB_SIZE + STACK_SIZE);
    ppcContext.r13.u64 = g_memory.MapVirtual(thread);  // TLS base
    ppcContext.fpscr.loadFromHost();
    
    SetPPCContext(ppcContext);
}
```

Synchronization primitives reimplemented with `std::atomic`:

```cpp
struct Event final : KernelObject
{
    bool manualReset;
    std::atomic<bool> signaled;

    uint32_t Wait(uint32_t timeout) override
    {
        if (timeout == INFINITE) {
            if (manualReset) {
                signaled.wait(false);
            } else {
                while (true) {
                    bool expected = true;
                    if (signaled.compare_exchange_weak(expected, false))
                        break;
                    signaled.wait(expected);
                }
            }
        }
        return STATUS_SUCCESS;
    }
    
    bool Set() {
        signaled = true;
        if (manualReset)
            signaled.notify_all();
        else
            signaled.notify_one();
        return TRUE;
    }
};
```


### 4.2.1 Memory Management Deep-Dive

**Original:** Xbox 360 unified memory model with 512MB shared between CPU and GPU

**Host Implementation:** Dual-heap architecture with o1heap allocator

#### Memory Layout and Reserved Regions

```
Guest Address Space (4GB mapped):
+------------------+ 0x00000000
| Protected Page   | <- Null pointer trap (PROT_NONE)
+------------------+ 0x00001000
|                  |
| Virtual Heap     | <- General allocations (0x20000 - 0x7FEA0000)
| (o1heap)         |
|                  |
+------------------+ 0x7FEA0000
| Reserved         | <- XMA I/O region (hardware audio)
| (XMAIOBegin)     |
+------------------+ 0x7FEAFFFF
|                  |
| Reserved         | <- System reserved
|                  |
+------------------+ 0xA0000000
|                  |
| Physical Heap    | <- Aligned allocations (0xA0000000 - 0x100000000)
| (o1heap)         |
|                  |
+------------------+ 0x100000000
```

#### Heap Implementation

The project uses **o1heap**, a constant-time deterministic allocator:

```cpp
// heap.cpp
constexpr size_t RESERVED_BEGIN = 0x7FEA0000;  // XMA I/O start
constexpr size_t RESERVED_END = 0xA0000000;    // Physical heap start

void Heap::Init()
{
    // Virtual heap: general game allocations
    heap = o1heapInit(g_memory.Translate(0x20000), RESERVED_BEGIN - 0x20000);
    
    // Physical heap: GPU resources, aligned allocations
    physicalHeap = o1heapInit(g_memory.Translate(RESERVED_END), 0x100000000 - RESERVED_END);
}
```

#### Physical Memory Allocation

Physical allocations require alignment (typically 4KB for GPU resources):

```cpp
void* Heap::AllocPhysical(size_t size, size_t alignment)
{
    size = std::max<size_t>(1, size);
    alignment = alignment == 0 ? 0x1000 : std::max<size_t>(16, alignment);

    std::lock_guard lock(physicalMutex);

    // Over-allocate to allow alignment
    void* ptr = o1heapAllocate(physicalHeap, size + alignment);
    size_t aligned = ((size_t)ptr + alignment) & ~(alignment - 1);

    // Store original pointer for freeing
    *((void**)aligned - 1) = ptr;
    *((size_t*)aligned - 2) = size + O1HEAP_ALIGNMENT;

    return (void*)aligned;
}
```

#### Xbox Kernel Memory API Bridges

```cpp
// MmAllocatePhysicalMemoryEx - Used for GPU buffers, textures
uint32_t MmAllocatePhysicalMemoryEx(uint32_t flags, uint32_t size, 
                                     uint32_t protect, uint32_t minAddress, 
                                     uint32_t maxAddress, uint32_t alignment)
{
    return g_memory.MapVirtual(g_userHeap.AllocPhysical(size, alignment));
}

// RtlAllocateHeap - Standard heap allocation
uint32_t RtlAllocateHeap(uint32_t heapHandle, uint32_t flags, uint32_t size)
{
    void* ptr = g_userHeap.Alloc(size);
    if ((flags & 0x8) != 0)  // HEAP_ZERO_MEMORY
        memset(ptr, 0, size);
    return g_memory.MapVirtual(ptr);
}

// XAllocMem - Xbox-specific allocation with flags
uint32_t XAllocMem(uint32_t size, uint32_t flags)
{
    void* ptr = (flags & 0x80000000) != 0 ?
        g_userHeap.AllocPhysical(size, (1ull << ((flags >> 24) & 0xF))) :
        g_userHeap.Alloc(size);

    if ((flags & 0x40000000) != 0)  // Zero memory
        memset(ptr, 0, size);

    return g_memory.MapVirtual(ptr);
}
```

### 4.3 Graphics Bridge

**Original:** Xenos GPU with D3D9-like API, command buffers, EDRAM tiling

**Host Implementation:** Modern graphics via plume abstraction layer (Vulkan/D3D12)

```
Xbox 360 Graphics Pipeline:
+-------------+     +-------------+     +-------------+
| Game D3D9   | --> | Xenos GPU   | --> | EDRAM       |
| Calls       |     | Commands    |     | (10MB tile) |
+-------------+     +-------------+     +-------------+

Recompiled Pipeline:
+-------------+     +-------------+     +-------------+
| Intercepted | --> | plume       | --> | Vulkan/D3D12|
| D3D9 Calls  |     | Abstraction |     | GPU         |
+-------------+     +-------------+     +-------------+
```

Key translation patterns:

```cpp
// video.cpp - Pipeline state management
struct PipelineState
{
    GuestShader* vertexShader = nullptr;
    GuestShader* pixelShader = nullptr;
    GuestVertexDeclaration* vertexDeclaration = nullptr;
    bool instancing = false;
    bool zEnable = true;
    RenderBlend srcBlend = RenderBlend::ONE;
    RenderBlend destBlend = RenderBlend::ZERO;
    RenderCullMode cullMode = RenderCullMode::NONE;
    // ... maps Xbox 360 render state to modern API state
};

// Shader format translation (handled by XenosRecomp at build time)
// Xbox 360 microcode → HLSL → DXIL (D3D12) or SPIR-V (Vulkan)
```

Texture format conversion:

```cpp
enum GuestFormat
{
    D3DFMT_A16B16G16R16F = 0x1A22AB60,
    D3DFMT_A8B8G8R8 = 0x1A200186,
    D3DFMT_D24S8 = 0x2D200196,
    // Maps to RenderFormat enum in plume
};
```


### 4.3.1 Rendering Pipeline Deep-Dive

#### Multi-Threaded Command Queue Architecture

The rendering system uses a producer-consumer model with a dedicated render thread:

```
+------------------+                      +------------------+
| Game Thread      |                      | Render Thread    |
| (Producer)       |                      | (Consumer)       |
+------------------+                      +------------------+
         |                                         ^
         v                                         |
+----------------------------------------------------------+
|              BlockingConcurrentQueue<RenderCommand>      |
|                     (g_renderQueue)                       |
+----------------------------------------------------------+
```

#### Render Command Structure

Commands are batched as a tagged union for efficient dispatch:

```cpp
enum class RenderCommandType
{
    SetRenderState, DestructResource, UnlockTextureRect,
    DrawImGui, ExecuteCommandList, BeginCommandList,
    StretchRect, SetRenderTarget, SetDepthStencilSurface,
    Clear, SetViewport, SetTexture, SetScissorRect,
    SetSamplerState, SetBooleans, SetVertexShaderConstants,
    SetPixelShaderConstants, AddPipeline, DrawPrimitive,
    DrawIndexedPrimitive, DrawPrimitiveUP, SetVertexDeclaration,
    SetVertexShader, SetStreamSource, SetIndices, SetPixelShader,
};

struct RenderCommand
{
    RenderCommandType type;
    union
    {
        struct { GuestRenderState type; uint32_t value; } setRenderState;
        struct { GuestResource* resource; } destructResource;
        struct { float x, y, width, height, minDepth, maxDepth; } setViewport;
        struct { uint32_t primitiveType; uint32_t startVertex; uint32_t primitiveCount; } drawPrimitive;
        // ... other command-specific data
    };
};
```

#### Render Thread Main Loop

```cpp
static std::thread g_renderThread([]
{
    RenderCommand commands[32];

    while (true)
    {
        size_t count = g_renderQueue.wait_dequeue_bulk(commands, std::size(commands));

        for (size_t i = 0; i < count; i++)
        {
            auto& cmd = commands[i];
            switch (cmd.type)
            {
            case RenderCommandType::SetRenderState:       ProcSetRenderState(cmd); break;
            case RenderCommandType::DrawPrimitive:        ProcDrawPrimitive(cmd); break;
            case RenderCommandType::DrawIndexedPrimitive: ProcDrawIndexedPrimitive(cmd); break;
            // ... dispatch all command types
            }
        }
    }
});
```

#### Dirty State Tracking

Minimizes redundant API calls:

```cpp
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
};

template<typename T>
static void SetDirtyValue(bool& dirtyState, T& dest, const T& src)
{
    if (dest != src) { dest = src; dirtyState = true; }
}
```

#### Render State Translation (D3D9 → Modern API)

```cpp
static void ProcSetRenderState(const RenderCommand& cmd)
{
    uint32_t value = cmd.setRenderState.value;

    switch (cmd.setRenderState.type)
    {
    case D3DRS_ZENABLE:
        SetDirtyValue(g_dirtyStates.pipelineState, g_pipelineState.zEnable, value != 0);
        break;
    case D3DRS_SRCBLEND:
        SetDirtyValue(g_dirtyStates.pipelineState, g_pipelineState.srcBlend, ConvertBlendMode(value));
        break;
    case D3DRS_CULLMODE:
    {
        RenderCullMode cullMode;
        switch (value) {
        case D3DCULL_NONE: cullMode = RenderCullMode::NONE; break;
        case D3DCULL_CW:   cullMode = RenderCullMode::FRONT; break;
        case D3DCULL_CCW:  cullMode = RenderCullMode::BACK; break;
        }
        SetDirtyValue(g_dirtyStates.pipelineState, g_pipelineState.cullMode, cullMode);
        break;
    }
    }
}
```

#### Texture Format Conversion Table

| Xbox 360 Format | Host Format | Notes |
|-----------------|-------------|-------|
| D3DFMT_A16B16G16R16F | R16G16B16A16_FLOAT | HDR render targets |
| D3DFMT_A8B8G8R8 | R8G8B8A8_UNORM | Standard textures |
| D3DFMT_D24S8 | D32_FLOAT | Depth (reverse-Z) |
| D3DFMT_G16R16F | R16G16_FLOAT | Velocity buffers |
| D3DFMT_L8 | R8_UNORM | Luminance (swizzled) |

#### Upload Allocator Pattern

Efficient GPU memory uploads using ring buffers:

```cpp
struct UploadAllocator
{
    std::vector<UploadBuffer> buffers;
    uint32_t index = 0;
    uint32_t offset = 0;

    UploadAllocation allocate(uint32_t size, uint32_t alignment)
    {
        offset = (offset + alignment - 1) & ~(alignment - 1);
        if (offset + size > UploadBuffer::SIZE) { ++index; offset = 0; }
        
        // Create buffer if needed, return allocation with mapped memory
        auto& buffer = buffers[index];
        return { buffer.buffer->at(offset), buffer.memory + offset };
    }
};
```

### 4.3.2 Shader Translation and Caching

#### Shader Cache Architecture

```
Build Time:                              Runtime:
+------------------+                     +------------------+
| Xbox 360 Shaders | --> XenosRecomp --> | Shader Cache     |
| (Xenos microcode)|                     | (DXIL + SPIR-V)  |
+------------------+                     +------------------+
                                                  |
                                                  v
                                         +------------------+
                                         | ZSTD Compressed  |
                                         +------------------+
```

#### Shader Cache Entry Structure

```cpp
// shader_cache.h
struct ShaderCacheEntry
{
    const uint64_t hash;              // XXH3 hash of original shader bytecode
    const uint32_t dxilOffset;        // Offset in DXIL cache blob
    const uint32_t dxilSize;          // Size of DXIL shader
    const uint32_t spirvOffset;       // Offset in SPIR-V cache blob
    const uint32_t spirvSize;         // Size of SPIR-V shader
    const uint32_t specConstantsMask; // Specialization constants used
    struct GuestShader* guestShader;  // Runtime pointer (lazy-loaded)
};
```

#### Runtime Shader Loading

```cpp
static void LoadEmbeddedResources()
{
    if (g_vulkan)
    {
        g_shaderCache = std::make_unique<uint8_t[]>(g_spirvCacheDecompressedSize);
        ZSTD_decompress(g_shaderCache.get(), g_spirvCacheDecompressedSize, 
                        g_compressedSpirvCache, g_spirvCacheCompressedSize);
    }
    else  // D3D12
    {
        g_shaderCache = std::make_unique<uint8_t[]>(g_dxilCacheDecompressedSize);
        ZSTD_decompress(g_shaderCache.get(), g_dxilCacheDecompressedSize, 
                        g_compressedDxilCache, g_dxilCacheCompressedSize);
    }
}
```

#### Asynchronous Pipeline Compilation

Pipelines are compiled on background threads during asset loading:

```cpp
// Thread pool uses 2/3 of available cores
static std::vector<std::unique_ptr<std::thread>> g_pipelineCompilerThreads = []()
{
    size_t threadCount = std::max(2u, (std::thread::hardware_concurrency() * 2) / 3);
    std::vector<std::unique_ptr<std::thread>> threads(threadCount);
    for (auto& thread : threads)
        thread = std::make_unique<std::thread>(PipelineCompilerThread);
    return threads;
}();

static void PipelineCompilerThread()
{
    while (true)
    {
        PipelineStateQueueItem queueItem;
        g_pipelineStateQueue.wait_dequeue(queueItem);

        // Boost priority during loading screens
        bool loading = *SWA::SGlobals::ms_IsLoading;
        SetThreadPriority(GetCurrentThread(), 
            loading ? THREAD_PRIORITY_HIGHEST : THREAD_PRIORITY_LOWEST);

        CompilePipeline(queueItem.pipelineHash, queueItem.pipelineState);
    }
}
```

#### Dynamic Shader Replacement

Custom shaders replace originals for enhanced features:

```cpp
static void ProcSetPixelShader(const RenderCommand& cmd)
{
    GuestShader* shader = cmd.setPixelShader.shader;
    
    if (shader != nullptr && shader->shaderCacheEntry != nullptr)
    {
        // Replace DoF blur based on resolution
        if (shader->shaderCacheEntry->hash == 0x4294510C775F4EE8)
        {
            size_t height = round(Video::s_viewportHeight * Config::ResolutionScale);
            size_t shaderIndex = (height > 1440) ? GAUSSIAN_BLUR_9X9 :
                                 (height > 1080) ? GAUSSIAN_BLUR_7X7 :
                                 (height > 720)  ? GAUSSIAN_BLUR_5X5 : GAUSSIAN_BLUR_3X3;
            shader = g_gaussianBlurShaders[shaderIndex].get();
        }
        // Enhanced motion blur
        else if (shader->shaderCacheEntry->hash == 0x6B9732B4CD7E7740 && 
                 Config::MotionBlur == EMotionBlur::Enhanced)
        {
            shader = g_enhancedMotionBlurShader.get();
        }
    }
}
```

### 4.4 Audio Bridge

**Original:** XMA hardware decoder with callback-driven rendering

**Host Implementation:** SDL_mixer with software XMA decoding

```cpp
// audio.h
#define XAUDIO_SAMPLES_HZ 48000
#define XAUDIO_NUM_CHANNELS 6
#define XAUDIO_SAMPLE_BITS 32
#define XAUDIO_NUM_SAMPLES 256

void XAudioInitializeSystem();
void XAudioRegisterClient(PPCFunc* callback, uint32_t param);
void XAudioSubmitFrame(void* samples);

// audio.cpp
uint32_t XAudioSubmitRenderDriverFrame(uint32_t driver, void* samples)
{
    XAudioSubmitFrame(samples);  // Byte-swap and forward to SDL
    return 0;
}
```


### 4.4.1 Audio Subsystem Deep-Dive

#### Audio Architecture Overview

```
+------------------+     +------------------+     +------------------+
| Game Audio       |     | SDL2 Audio       |     | Host Audio       |
| Callback         | --> | Driver           | --> | Device           |
| (Guest Code)     |     | (Bridging)       |     | (WASAPI/ALSA)    |
+------------------+     +------------------+     +------------------+
         ^
         |
+------------------+
| Audio Thread     |
| (Timing Control) |
+------------------+
```

#### Audio Format Specifications

```cpp
#define XAUDIO_SAMPLES_HZ   48000   // 48kHz sample rate
#define XAUDIO_NUM_CHANNELS 6       // 5.1 surround sound
#define XAUDIO_SAMPLE_BITS  32      // 32-bit float samples
#define XAUDIO_NUM_SAMPLES  256     // Samples per callback frame
```

#### SDL2 Audio Driver Implementation

```cpp
static void CreateAudioDevice()
{
    SDL_AudioSpec desired{}, obtained{};
    desired.freq = XAUDIO_SAMPLES_HZ;      // 48000 Hz
    desired.format = AUDIO_F32SYS;          // 32-bit float, system endian
    desired.channels = Config::ChannelConfiguration == EChannelConfiguration::Surround 
                       ? XAUDIO_NUM_CHANNELS : 2;
    desired.samples = XAUDIO_NUM_SAMPLES;   // 256 samples per callback
    
    g_audioDevice = SDL_OpenAudioDevice(nullptr, 0, &desired, &obtained, 
        Config::ChannelConfiguration == EChannelConfiguration::Surround 
            ? SDL_AUDIO_ALLOW_CHANNELS_CHANGE : 0);

    // Fall back to stereo if surround not available
    g_downMixToStereo = (obtained.channels == 2);
}
```

#### Audio Thread with Timing Control

```cpp
static void AudioThread()
{
    GuestThreadContext ctx(0);  // Create guest context for callback

    while (!g_audioThreadShouldExit)
    {
        uint32_t queuedAudioSize = SDL_GetQueuedAudioSize(g_audioDevice);
        constexpr size_t MAX_LATENCY = 10;  // Max frames of latency

        // Only request more audio if buffer isn't too full
        if ((queuedAudioSize / callbackAudioSize) <= MAX_LATENCY)
        {
            ctx.ppcContext.r3.u32 = g_clientCallbackParam;
            g_clientCallback(ctx.ppcContext, g_memory.base);  // Call guest audio code
        }

        // Precise timing: sync to audio frame boundaries
        auto now = std::chrono::steady_clock::now();
        constexpr auto INTERVAL = 1000000000ns * XAUDIO_NUM_SAMPLES / XAUDIO_SAMPLES_HZ;
        auto next = now + (INTERVAL - now.time_since_epoch() % INTERVAL);

        std::this_thread::sleep_for(std::chrono::floor<std::chrono::milliseconds>(next - now));
    }
}
```

#### 5.1 Surround to Stereo Downmixing

When surround sound isn't available, the 6-channel audio is downmixed:

```cpp
void XAudioSubmitFrame(void* samples)
{
    auto floatSamples = reinterpret_cast<be<float>*>(samples);

    if (g_downMixToStereo)
    {
        // Channel layout:
        // 0: Front Left    (L: 1.0, R: 0.0)
        // 1: Front Right   (L: 0.0, R: 1.0)
        // 2: Center        (L: 0.75, R: 0.75)
        // 3: LFE           (discarded)
        // 4: Rear Left     (L: 1.0, R: 0.0)
        // 5: Rear Right    (L: 0.0, R: 1.0)

        for (size_t i = 0; i < XAUDIO_NUM_SAMPLES; i++)
        {
            float ch0 = floatSamples[0 * XAUDIO_NUM_SAMPLES + i];  // Front Left
            float ch1 = floatSamples[1 * XAUDIO_NUM_SAMPLES + i];  // Front Right
            float ch2 = floatSamples[2 * XAUDIO_NUM_SAMPLES + i];  // Center
            float ch4 = floatSamples[4 * XAUDIO_NUM_SAMPLES + i];  // Rear Left
            float ch5 = floatSamples[5 * XAUDIO_NUM_SAMPLES + i];  // Rear Right

            audioFrames[i * 2 + 0] = (ch0 + ch2 * 0.75f + ch4) * Config::MasterVolume;
            audioFrames[i * 2 + 1] = (ch1 + ch2 * 0.75f + ch5) * Config::MasterVolume;
        }

        SDL_QueueAudio(g_audioDevice, &audioFrames, sizeof(audioFrames));
    }
}
```

#### Embedded Audio Player (UI Sounds)

For sounds that need to play outside the guest audio system:

```cpp
static void PlayEmbeddedSound(EmbeddedSound s)
{
    EmbeddedSoundData& data = g_embeddedSoundData[size_t(s)];
    
    if (data.chunk == nullptr)
    {
        // Lazy-load from embedded OGG resources
        data.chunk = Mix_LoadWAV_RW(SDL_RWFromConstMem(soundData, soundDataSize), 1);
    }

    Mix_VolumeChunk(data.chunk, Config::MasterVolume * Config::EffectsVolume * MIX_MAX_VOLUME);
    Mix_PlayChannel(g_channelIndex % MIX_CHANNELS, data.chunk, 0);
}

void EmbeddedPlayer::Init()
{
    Mix_OpenAudio(XAUDIO_SAMPLES_HZ, AUDIO_F32SYS, 2, 4096);
    g_installerMusic = Mix_LoadMUS_RW(
        SDL_RWFromConstMem(g_installer_music, sizeof(g_installer_music)), 1);
}
```

### 4.5 Input Bridge

**Original:** XInput with Xbox 360 controller support

**Host Implementation:** SDL2 with multi-controller support

```cpp
// hid.h
namespace hid
{
    enum class EInputDevice { Unknown, Keyboard, Mouse, Xbox, PlayStation };
    enum class EInputDeviceExplicit { 
        Unknown, Xbox360, XboxOne, DualShock3, DualShock4, 
        DualSense, SwitchPro, /* ... */
    };
    
    uint32_t GetState(uint32_t dwUserIndex, XAMINPUT_STATE* pState);
    uint32_t SetState(uint32_t dwUserIndex, XAMINPUT_VIBRATION* pVibration);
}
```

---

## 5. Static Recompilation Strategy

### 5.1 Instruction Translation

XenonRecomp translates PowerPC instructions to C++ function calls operating on a `PPCContext`:

```cpp
// Generated code pattern
void sub_82000000(PPCContext& ctx, uint8_t* base)
{
    ctx.r3.u32 = ctx.r4.u32 + ctx.r5.u32;  // add r3, r4, r5
    ctx.cr.setFromCompare(ctx.r3.s32, 0);   // cmpwi r3, 0
    if (ctx.cr.lt()) {
        sub_82000100(ctx, base);             // blt target
    }
}
```

### 5.2 Function Lookup Table

Functions are registered at startup:

```cpp
// memory.cpp
Memory::Memory()
{
    // ... memory allocation ...
    
    for (size_t i = 0; PPCFuncMappings[i].guest != 0; i++)
    {
        if (PPCFuncMappings[i].host != nullptr)
            InsertFunction(PPCFuncMappings[i].guest, PPCFuncMappings[i].host);
    }
}

// Lookup at runtime
PPCFunc* FindFunction(uint32_t guest) const noexcept
{
    return PPC_LOOKUP_FUNC(base, guest);
}
```

### 5.3 Calling Convention Translation

```cpp
// function.h - Argument marshaling
struct ArgTranslator
{
    // Integer arguments: r3-r10, then stack
    constexpr static uint64_t GetIntegerArgumentValue(const PPCContext& ctx, 
                                                       uint8_t* base, size_t arg)
    {
        switch (arg) {
            case 0: return ctx.r3.u32;
            case 1: return ctx.r4.u32;
            // ... r5-r10 ...
            case 7: return ctx.r10.u32;
            default:
                // Stack arguments at r1 + 0x54 + (arg-8)*8
                return *reinterpret_cast<be<uint32_t>*>(base + ctx.r1.u32 + 0x54 + ((arg - 8) * 8));
        }
    }
    
    // Float arguments: f1-f13
    static double GetPrecisionArgumentValue(const PPCContext& ctx, uint8_t* base, size_t arg)
    {
        switch (arg) {
            case 0: return ctx.f1.f64;
            case 1: return ctx.f2.f64;
            // ... f3-f13 ...
        }
    }
};
```

### 5.4 Global State and TLS

Thread-local PPC context:

```cpp
// ppc_context.h
inline thread_local PPCContext* g_ppcContext;

inline PPCContext* GetPPCContext()
{
    return g_ppcContext;
}

inline void SetPPCContext(PPCContext& ctx)
{
    g_ppcContext = &ctx;
}
```

Guest TLS emulation:

```cpp
// imports.cpp
static std::vector<size_t> g_tlsFreeIndices;
static size_t g_tlsNextIndex = 0;

static uint32_t& KeTlsGetValueRef(size_t index)
{
    thread_local std::vector<uint32_t> s_tlsValues;
    if (s_tlsValues.size() <= index)
        s_tlsValues.resize(index + 1, 0);
    return s_tlsValues[index];
}

uint32_t KeTlsAlloc()
{
    std::lock_guard<Mutex> lock(g_tlsAllocationMutex);
    if (!g_tlsFreeIndices.empty()) {
        size_t index = g_tlsFreeIndices.back();
        g_tlsFreeIndices.pop_back();
        return index;
    }
    return g_tlsNextIndex++;
}
```

### 5.5 Handling Undefined Behavior

Byte swapping is pervasive:

```cpp
template<typename T>
inline T ByteSwap(T value);

// Big-endian wrapper template
template<typename T>
struct be
{
    T value;
    
    T get() const { return ByteSwap(value); }
    operator T() const { return get(); }
    be& operator=(T v) { value = ByteSwap(v); return *this; }
};
```

---

## 6. Hooking and Native Code Interposition

### 6.1 Function Hook Mechanism

The `GUEST_FUNCTION_HOOK` macro replaces guest functions with host implementations:

```cpp
// function.h
#define GUEST_FUNCTION_HOOK(subroutine, function) \
    PPC_FUNC(subroutine) { HostToGuestFunction<function>(ctx, base); }

#define GUEST_FUNCTION_STUB(subroutine) \
    PPC_FUNC(subroutine) { }

// Usage example
uint32_t XCreateFileA(const char* lpFileName, uint32_t dwDesiredAccess, ...);
GUEST_FUNCTION_HOOK(sub_82BD4668, XCreateFileA);
```

The `HostToGuestFunction` template handles argument marshaling:

```cpp
template<auto Func>
PPC_FUNC(HostToGuestFunction)
{
    using ret_t = decltype(std::apply(Func, function_args(Func)));
    
    auto args = function_args(Func);
    _translate_args_to_host<Func>(ctx, base, args);
    
    if constexpr (std::is_same_v<ret_t, void>) {
        std::apply(Func, args);
    } else {
        auto v = std::apply(Func, args);
        
        if constexpr (std::is_pointer<ret_t>()) {
            ctx.r3.u64 = g_memory.MapVirtual(v);
        } else if constexpr (is_precise_v<ret_t>) {
            ctx.f1.f64 = v;
        } else {
            ctx.r3.u64 = (uint64_t)v;
        }
    }
}
```

### 6.2 Mid-Function Hooks (Midasm Hooks)

For patches that need to modify behavior mid-function:

```toml
# SWA.toml
[[midasm_hook]]
name = "CameraAspectRatioMidAsmHook"
address = 0x82468E78
registers = ["r30", "r31"]

[[midasm_hook]]
name = "HighFrameRateDeltaTimeFixMidAsmHook"
address = 0x82345468
registers = ["f1"]
```

Implementation in C++:

```cpp
// fps_patches.cpp
void HighFrameRateDeltaTimeFixMidAsmHook(PPCRegister& f1)
{
    constexpr double threshold = 1.0 / 60.0;
    if (f1.f64 < threshold)
        f1.f64 = threshold;
}
```

### 6.3 Original Function Preservation

The `PPC_FUNC_IMPL` pattern allows calling original code:

```cpp
// app.cpp
PPC_FUNC_IMPL(__imp__sub_822C1130);  // Declare original
PPC_FUNC(sub_822C1130)               // Hook
{
    Video::WaitOnSwapChain();
    
    // Modify delta time precision
    if (Config::FPS >= FPS_MIN && Config::FPS < FPS_MAX) {
        double targetDeltaTime = 1.0 / Config::FPS;
        if (abs(ctx.f1.f64 - targetDeltaTime) < 0.00001)
            ctx.f1.f64 = targetDeltaTime;
    }
    
    App::s_deltaTime = ctx.f1.f64;
    
    // Call original implementation
    __imp__sub_822C1130(ctx, base);
}
```

### 6.4 Guest-to-Host Function Calls

Calling recompiled guest code from host:

```cpp
template<typename T, typename TFunction, typename... TArgs>
T GuestToHostFunction(const TFunction& func, TArgs&&... argv)
{
    auto args = std::make_tuple(std::forward<TArgs>(argv)...);
    auto& currentCtx = *GetPPCContext();
    
    PPCContext newCtx;
    newCtx.r1 = currentCtx.r1;
    newCtx.r13 = currentCtx.r13;
    newCtx.fpscr = currentCtx.fpscr;
    
    _translate_args_to_guest(newCtx, g_memory.base, args);
    SetPPCContext(newCtx);
    
    if constexpr (std::is_function_v<TFunction>)
        func(newCtx, g_memory.base);
    else
        g_memory.FindFunction(func)(newCtx, g_memory.base);
    
    SetPPCContext(currentCtx);
    
    return static_cast<T>(newCtx.r3.u64);
}
```

---

## 7. Adding Modern Features

### 7.1 Ultrawide and Aspect Ratio Support

```cpp
// aspect_ratio_patches.cpp
void AspectRatioPatches::ComputeOffsets()
{
    float width = Video::s_viewportWidth;
    float height = Video::s_viewportHeight;
    
    g_aspectRatio = width / height;
    
    if (g_aspectRatio >= NARROW_ASPECT_RATIO) {
        // Widescreen: letterbox on sides
        g_aspectRatioOffsetX = (width - height * WIDE_ASPECT_RATIO) / 2.0f;
        g_aspectRatioOffsetY = 0.0f;
        g_aspectRatioScale = height / 720.0f;
    } else {
        // Narrow: pillarbox top/bottom
        g_aspectRatioOffsetX = (width - width * NARROW_ASPECT_RATIO) / 2.0f;
        g_aspectRatioOffsetY = (height - width / NARROW_ASPECT_RATIO) / 2.0f;
        g_aspectRatioScale = width / 960.0f;
    }
}

// Hook for screen position calculation
PPC_FUNC(sub_8250FC70)
{
    __imp__sub_8250FC70(ctx, base);
    
    auto position = reinterpret_cast<be<float>*>(base + ctx.r3.u32);
    position[0] = (position[0] / 1280.0f * Video::s_viewportWidth - g_aspectRatioOffsetX) 
                  / g_aspectRatioScale;
    position[1] = (position[1] / 720.0f * Video::s_viewportHeight - g_aspectRatioOffsetY) 
                  / g_aspectRatioScale;
}
```

### 7.2 High Frame Rate Support

Delta time fixes for physics and camera:

```cpp
// fps_patches.cpp
static double ComputeLerpFactor(double t, double deltaTime)
{
    double fps = 1.0 / deltaTime;
    double bias = t * 60.0;
    return 1.0 - pow(1.0 - t, (30.0 + bias) / (fps + bias));
}

void CameraLerpFixMidAsmHook(PPCRegister& t)
{
    t.f64 = ComputeLerpFactor(t.f64, App::s_deltaTime);
}

// Boss fight timer fix (runs at fixed 30 FPS internally)
PPC_FUNC(sub_82B00D00)  // CExStageBoss::CStateBattle::Update
{
    constexpr auto referenceDeltaTime = 1.0f / 30.0f;
    auto pElapsedTime = (float*)(base + ctx.r3.u32 + EX_STAGE_BOSS_STATE_BATTLE_SIZE);
    
    *pElapsedTime += std::min(App::s_deltaTime, 1.0 / 15.0);
    
    if (*pElapsedTime >= referenceDeltaTime) {
        __imp__sub_82B00D00(ctx, base);  // Run original at 30 FPS rate
        *pElapsedTime -= referenceDeltaTime;
    }
}
```

### 7.3 Modern Input Support

D-Pad support and touchpad integration:

```cpp
// input_patches.cpp
class SDLEventListenerForInputPatches : public SDLEventListener
{
    bool OnSDLEvent(SDL_Event* event) override
    {
        switch (event->type) {
            case SDL_CONTROLLERTOUCHPADMOTION:
                g_isCursorActive = true;
                ms_cursorDeltaX = ms_cursorX - ms_cursorPrevX;
                ms_cursorDeltaY = ms_cursorY - ms_cursorPrevY;
                break;
                
            case SDL_CONTROLLERTOUCHPADDOWN:
                // Auto-detect controller type for sensitivity
                g_worldMapCursorParams = hid::g_inputDeviceExplicit == hid::EInputDeviceExplicit::DualSense
                    ? g_worldMapCursorParamsProspero
                    : g_worldMapCursorParamsOrbis;
                break;
        }
        return false;
    }
};
```

### 7.4 Asynchronous Shader Compilation

Pipeline precompilation during asset loading:

```cpp
// video.cpp
static void EnqueuePipelineTask(PipelineTaskType type, 
                                 const boost::shared_ptr<Hedgehog::Database::CDatabaseData>& databaseData)
{
    if (type != PipelineTaskType::PrecompilePipelines)
        ++g_compilingPipelineTaskCount;
    
    {
        std::lock_guard lock(g_pipelineTaskMutex);
        g_pipelineTaskQueue.emplace_back(type, databaseData);
    }
    
    if ((++g_pendingPipelineTaskCount) == 1)
        g_pendingPipelineTaskCount.notify_one();
}

// Precompiled pipeline cache loaded at startup
static const PipelineState g_pipelineStateCache[] = {
    #include "cache/pipeline_state_cache.h"
};
```

---

## 8. Debugging and Validation

### 8.1 Validation Without Source

Techniques used:

1. **Original Hardware Comparison**: Side-by-side testing with Xbox 360
2. **Memory State Inspection**: Dump guest memory at key points
3. **Trace Logging**: Log function calls and parameters

```cpp
// Logging macros
#define LOG_UTILITY(msg) /* ... */
#define LOGF_UTILITY(fmt, ...) /* ... */

// Example usage in imports.cpp
uint32_t MmAllocatePhysicalMemoryEx(uint32_t flags, uint32_t size, ...)
{
    LOGF_UTILITY("0x{:x}, 0x{:x}, ...", flags, size);
    return g_memory.MapVirtual(g_userHeap.AllocPhysical(size, alignment));
}
```

### 8.2 Crash Tracing

Debug trap insertion:

```cpp
void KeBugCheckEx()
{
    __builtin_debugtrap();
}

void KeBugCheck()
{
    __builtin_debugtrap();
}
```

### 8.3 Behavioral Mismatch Detection

The profiler system for performance analysis:

```cpp
// video.cpp
static Profiler g_gpuFrameProfiler;
static Profiler g_presentProfiler;
static Profiler g_updateDirectorProfiler;

struct Profiler
{
    std::atomic<double> value;
    double values[PROFILER_VALUE_COUNT];
    std::chrono::steady_clock::time_point start;
    
    void Begin() { start = std::chrono::steady_clock::now(); }
    void End() { 
        value = std::chrono::duration<double, std::milli>(
            std::chrono::steady_clock::now() - start).count(); 
    }
};
```

### 8.4 Known Failure Modes

| Issue | Cause | Fix |
|-------|-------|-----|
| Sparkle locus corruption at HFR | Epsilon check fails with stable framerate | Force particle update path |
| Egg Dragoon drill missile rotation | Quaternion denormalization | Explicit normalization hook |
| Boss timer desync | Frame-dependent timer increments | Fixed timestep wrapper |

---

## 9. Incremental Bring-Up Strategy

### 9.1 Phase 1: Minimal Boot

Priority order:
1. Memory allocation (`g_memory`, `g_userHeap`)
2. XEX loading and decompression
3. Entry point resolution
4. Basic kernel stubs (enough to not crash)

### 9.2 Phase 2: Video Output

1. Video backend initialization (`Video::CreateHostDevice`)
2. Swap chain management
3. Basic rendering (clear, present)
4. Shader translation and caching

### 9.3 Phase 3: Game Logic

1. File system implementation
2. Content mounting
3. Audio initialization
4. Input handling

### 9.4 Stub Strategy

Functions stubbed early:

```cpp
// Stubs for non-essential Xbox features
GUEST_FUNCTION_STUB(__imp__vsprintf);
GUEST_FUNCTION_STUB(__imp___vsnprintf);
GUEST_FUNCTION_STUB(__imp__sprintf);

// Network stubs (game doesn't require online)
void NetDll_WSAStartup() { LOG_UTILITY("!!! STUB !!!"); }
void NetDll_socket() { LOG_UTILITY("!!! STUB !!!"); }
```

Stubs that work because the runtime adapts:
- `VdInitializeEngines`: Video engine init (not needed with native backend)
- `XamShowMessageBoxUI`: UI prompts (game continues without)
- `XGetVideoMode`: Returns hardcoded 720p mode

---

## 10. Lessons Learned

### 10.1 What Worked

1. **Separate Recompiler Tools**: XenonRecomp and XenosRecomp as standalone tools enabled iteration
2. **Configuration-Driven Hooks**: TOML-based midasm hook definitions allowed rapid patching
3. **Type-Safe Guest Memory Access**: The `be<T>` template caught many endianness bugs at compile time
4. **Modern Graphics Abstraction**: The plume layer enabled Vulkan and D3D12 from single codebase
5. **Integrated Asset Loading**: Pipeline compilation during streaming eliminated shader stutters

### 10.2 What Failed

1. **Initial Over-Stubbing**: Stubbing "init" functions that had important side effects caused cascading issues
2. **Assuming Fixed Framerate**: Original game's variable timestep required extensive HFR patching
3. **GPU Emulation Attempt**: Early attempts at lower-level GPU emulation were abandoned for direct API translation

### 10.3 Recommendations for Future Projects

**Do:**
- Identify phase boundaries (kernel init, graphics init, game loop) and only bypass at boundaries
- Create replacement implementations that produce equivalent observable state
- Use configuration files for patch definitions to avoid recompilation
- Implement robust logging from the start
- Compare against original hardware frequently

**Don't:**
- Bypass "init", "register", or "create" functions without understanding their side effects
- Assume original code is frame-rate agnostic
- Implement stubs without logging (silent failures are worst)
- Skip input latency considerations in the rendering pipeline

### 10.4 Patterns to Follow

```cpp
// Pattern: Safe function replacement
// Instead of: bypass A, bypass B, bypass C
// Do: Replace {A, B, C} with HostSubsystem_Init() that produces all expected state

// Pattern: Hook with preservation
PPC_FUNC_IMPL(__imp__original);
PPC_FUNC(original)
{
    // Pre-processing
    ModifyInputs(ctx);
    
    // Call original
    __imp__original(ctx, base);
    
    // Post-processing
    ModifyOutputs(ctx);
}

// Pattern: Configuration-driven patching
[[midasm_hook]]
name = "FixFunction"
address = 0x82000000
registers = ["r3", "f1"]
jump_address_on_true = 0x82000100
```

---

## Appendix A: Project Structure

```
UnleashedRecomp/
├── cpu/                    # Guest thread and context management
│   ├── guest_thread.cpp    # Thread creation and lifecycle
│   ├── guest_thread.h
│   └── ppc_context.h       # Thread-local PPC state
├── gpu/                    # Graphics subsystem
│   ├── video.cpp           # Main rendering implementation (~8000 lines)
│   ├── video.h             # Guest resource structures
│   ├── shader/             # Pre-translated shaders
│   └── imgui/              # Debug UI
├── apu/                    # Audio subsystem
│   ├── audio.cpp           # XAudio bridge
│   └── embedded_player.cpp # Background music
├── kernel/                 # OS services
│   ├── imports.cpp         # Kernel function implementations
│   ├── memory.cpp          # Memory allocation
│   ├── xam.cpp             # Xbox Application Manager
│   ├── xdm.h               # Kernel object definitions
│   └── io/                 # File system
├── hid/                    # Input handling
│   └── hid.cpp             # SDL input bridge
├── patches/                # Game-specific modifications
│   ├── aspect_ratio_patches.cpp
│   ├── fps_patches.cpp
│   └── input_patches.cpp
├── ui/                     # Custom menus
│   ├── options_menu.cpp
│   └── achievement_menu.cpp
└── mod/                    # Mod loader
    └── mod_loader.cpp

UnleashedRecompLib/
├── config/
│   ├── SWA.toml            # Recompiler configuration
│   └── SWA_switch_tables.toml
├── ppc/                    # Generated PPC code (gitignored)
└── shader/                 # Generated shaders (gitignored)
```

---

## Appendix B: Key Data Structures

### PPCContext

```cpp
struct PPCContext
{
    PPCRegister r0, r1, r2, r3, /* ... */ r31;  // General purpose
    PPCFPRegister f0, f1, f2, /* ... */ f31;    // Floating point
    PPCVRegister v0, v1, v2, /* ... */ v127;    // Vector
    PPCCRField cr;                               // Condition register
    PPCFPSCRField fpscr;                        // FP status
    // XER, LR, CTR omitted in recompiled code (optimized)
};
```

### Memory

```cpp
struct Memory
{
    uint8_t* base{};
    
    void* Translate(size_t offset) const noexcept {
        return base + offset;
    }
    
    uint32_t MapVirtual(const void* host) const noexcept {
        return static_cast<uint32_t>(static_cast<const uint8_t*>(host) - base);
    }
    
    PPCFunc* FindFunction(uint32_t guest) const noexcept {
        return PPC_LOOKUP_FUNC(base, guest);
    }
};
```

---

*Document generated from analysis of UnleashedRecomp codebase*
*Target audience: Emulator developers, recompiler authors, reverse engineers*

## Appendix C: Conceptual Summary - How Each Part Was Done

This section provides a high-level conceptual overview of how each major subsystem was transformed from Xbox 360 to modern PC.

---

### CPU: PowerPC to x86-64

**The Problem:** The Xbox 360 uses a PowerPC CPU with big-endian byte order, different registers, and a different instruction set than x86-64.

**The Solution:** XenonRecomp statically translates each PowerPC function into a C++ function that operates on a `PPCContext` structure. Instead of executing PPC instructions directly, the recompiled code manipulates a struct containing all 32 general-purpose registers (r0-r31), 32 floating-point registers (f0-f31), and 128 vector registers (v0-v127). Each guest function becomes a host function with signature `void func(PPCContext& ctx, uint8_t* base)`.

**Key Insight:** The recompiler doesn't emulate the CPU—it transforms the code at build time so it runs natively, with the `PPCContext` serving as the "virtual CPU state."

---

### Memory: Guest Address Space

**The Problem:** The game expects a contiguous 32-bit address space with specific memory regions for code, heap, and hardware I/O.

**The Solution:** A 4GB contiguous memory block is allocated at startup (preferably at address 0x100000000). All guest addresses are translated by adding them to a base pointer. Two heaps are carved out: a virtual heap (0x20000–0x7FEA0000) for general allocations and a physical heap (0xA0000000+) for GPU resources requiring alignment. The reserved region (0x7FEA0000–0xA0000000) covers Xbox hardware I/O that doesn't exist on PC.

**Key Insight:** Guest pointers become `base + offset`, and host pointers become `ptr - base`. The `be<T>` template handles byte-swapping transparently.

---

### Threading: Xbox Threads to Host Threads

**The Problem:** The game creates Xbox kernel threads with custom Thread Environment Blocks (TEB), Processor Control Regions (PCR), and thread-local storage (TLS).

**The Solution:** Each guest thread becomes a native `std::thread` with a `GuestThreadContext` that allocates fake PCR, TEB, TLS, and stack regions in guest memory. The `PPCContext` is stored in a `thread_local` variable so each host thread maintains its own guest CPU state. Synchronization primitives (events, semaphores, critical sections) are reimplemented using `std::atomic` with wait/notify semantics.

**Key Insight:** The guest threading model is preserved by allocating the expected memory structures, even though the underlying implementation uses native threads.

---

### Graphics: D3D9 to Vulkan/D3D12

**The Problem:** The game issues D3D9-style rendering commands to the Xenos GPU, which has unique features like EDRAM tiling and hardware-specific shaders.

**The Solution:** A multi-threaded command queue decouples the game thread from the render thread. The game thread enqueues `RenderCommand` structs (tagged unions) that the render thread processes. Each D3D9 render state is translated to equivalent Vulkan/D3D12 state. Pipeline states are cached by hash to avoid recompilation. The `plume` abstraction layer provides a unified API for both Vulkan and D3D12 backends.

**Key Insight:** The rendering is not emulated—D3D9 calls are intercepted and translated to modern API equivalents at a semantic level.

---

### Shaders: Xenos Microcode to HLSL/SPIR-V

**The Problem:** Xbox 360 shaders are compiled to Xenos GPU microcode, which is incompatible with modern GPUs.

**The Solution:** XenosRecomp translates Xenos shader microcode to HLSL at build time. The HLSL is then compiled to both DXIL (for D3D12) and SPIR-V (for Vulkan). These are stored in ZSTD-compressed blobs embedded in the executable. At runtime, shaders are identified by XXH3 hash of the original bytecode and looked up in the cache. Some shaders are replaced with enhanced versions (e.g., higher-quality blur, depth-aware motion blur).

**Key Insight:** Shader translation happens once at build time, not at runtime. The hash lookup enables lazy loading and shader replacement.

---

### Audio: XAudio Callbacks to SDL

**The Problem:** The game uses Xbox's XAudio system with callback-driven rendering and 5.1 surround sound in a specific interleaved format.

**The Solution:** An audio thread periodically calls the guest's audio callback function (using a `GuestThreadContext`), receives 6-channel, 48kHz, 32-bit float samples, byte-swaps them, optionally downmixes to stereo, and queues them to SDL's audio device. Timing is carefully controlled to maintain low latency without buffer underruns. UI sounds use a separate SDL_mixer-based embedded player.

**Key Insight:** The guest audio code runs unmodified—only the output path is redirected from Xbox hardware to SDL.

---

### Input: XInput to SDL

**The Problem:** The game uses XInput for Xbox 360 controllers with specific button layouts and vibration support.

**The Solution:** SDL2's game controller API provides a unified interface for all modern controllers. The `hid` subsystem translates SDL controller state to the `XINPUT_STATE` structure the game expects. Controller type detection enables appropriate button prompts. Additional features like keyboard/mouse support and touchpad cursor control are injected via patches.

**Key Insight:** The game still "sees" XInput—the translation layer converts SDL events to the expected format.

---

### File System: Xbox Paths to Host Paths

**The Problem:** The game uses Xbox-style paths like `game:\work\` and `update:\` that don't exist on PC.

**The Solution:** A virtual file system (VFS) layer intercepts all file operations. Xbox paths are parsed and translated to host filesystem paths. The `game:\` root maps to the extracted game files, `update:\` to title update content, and `save:\` to the user's save directory. The mod loader hooks into this layer to redirect file access to modded content when available.

**Key Insight:** Path translation happens at the API boundary—the game's file access logic runs unmodified.

---

### Kernel Objects: Xbox Primitives to std::atomic

**The Problem:** The game uses Xbox kernel objects (events, semaphores, mutexes) with specific semantics for synchronization.

**The Solution:** Each kernel object type is reimplemented as a C++ class wrapping `std::atomic` with appropriate wait/notify semantics. Manual-reset vs auto-reset events, semaphore counting, and timeout handling are all preserved. Objects are tracked via a `KernelObject` base class with reference counting. The `GUEST_FUNCTION_HOOK` macro connects guest kernel API calls to host implementations.

**Key Insight:** The semantics are preserved exactly—only the implementation changes from kernel objects to user-space atomics.

---

### Modern Features: Extensions Without Source

**The Problem:** Adding ultrawide support, high frame rate fixes, and new UI requires modifying game behavior without access to source code.

**The Solution:** Mid-assembly hooks (`midasm_hook` in TOML config) inject custom code at specific instruction addresses. These hooks receive relevant registers, modify them, and optionally skip or redirect execution. Aspect ratio patches adjust viewport calculations and UI positioning. HFR patches correct timing assumptions and fix physics/camera interpolation. The options menu and achievement overlay are entirely new ImGui-based UI systems.

**Key Insight:** The recompiled code can be patched at any instruction boundary, enabling surgical modifications to game logic.

---

### The Big Picture

```
┌─────────────────────────────────────────────────────────────────────┐
│                         BUILD TIME                                   │
├─────────────────────────────────────────────────────────────────────┤
│  XEX Binary ──► XenonRecomp ──► C++ Source ──► Native x86-64 Code   │
│  Xenos Shaders ──► XenosRecomp ──► HLSL ──► DXIL/SPIR-V Cache       │
└─────────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────────┐
│                         RUNTIME                                      │
├─────────────────────────────────────────────────────────────────────┤
│                                                                      │
│  ┌──────────────┐    ┌──────────────┐    ┌──────────────┐           │
│  │ Guest Code   │    │ Guest Code   │    │ Guest Code   │           │
│  │ (Recompiled) │    │ (Recompiled) │    │ (Recompiled) │           │
│  └──────┬───────┘    └──────┬───────┘    └──────┬───────┘           │
│         │                   │                   │                    │
│         ▼                   ▼                   ▼                    │
│  ┌─────────────────────────────────────────────────────────────┐    │
│  │                    Platform Bridge Layer                     │    │
│  │  ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌────────┐ │    │
│  │  │ Memory  │ │Graphics │ │  Audio  │ │  Input  │ │  VFS   │ │    │
│  │  │ (o1heap)│ │ (plume) │ │  (SDL)  │ │  (SDL)  │ │(std::fs│ │    │
│  │  └─────────┘ └─────────┘ └─────────┘ └─────────┘ └────────┘ │    │
│  └─────────────────────────────────────────────────────────────┘    │
│                                                                      │
│  ┌─────────────────────────────────────────────────────────────┐    │
│  │                     Host Platform                            │    │
│  │        Windows (D3D12/Vulkan) or Linux (Vulkan)             │    │
│  └─────────────────────────────────────────────────────────────┘    │
│                                                                      │
└─────────────────────────────────────────────────────────────────────┘
```

**The fundamental approach:** Transform the code, not the execution. The game logic runs as native code with its "view" of the world preserved through carefully crafted bridge layers. Each platform API the game calls is intercepted at the boundary and translated to host equivalents that produce the same observable behavior.

---


## Appendix D: Xbox API Implementation Status

This section documents which Xbox 360 APIs were fully implemented, partially implemented (stub with logging), or completely stubbed (no-op).

---

### Implementation Statistics

| Category | Count | Description |
|----------|-------|-------------|
| **Fully Implemented (GUEST_FUNCTION_HOOK)** | ~275 | APIs with complete host implementations |
| **No-Op Stubs (GUEST_FUNCTION_STUB)** | ~28 | APIs that do nothing (safe to skip) |
| **Logging Stubs (LOG_UTILITY STUB)** | ~129 | APIs hooked but with minimal/no implementation |

---

### Fully Implemented APIs (Critical Path)

These APIs have complete host implementations that preserve the expected behavior:

#### Memory Management
| API | Implementation |
|-----|----------------|
| `RtlAllocateHeap` | o1heap virtual allocation |
| `RtlFreeHeap` | o1heap deallocation |
| `RtlReAllocateHeap` | Alloc + copy + free |
| `RtlSizeHeap` | Query allocation size |
| `MmAllocatePhysicalMemoryEx` | o1heap physical allocation with alignment |
| `MmFreePhysicalMemory` | Physical heap deallocation |
| `XAllocMem` | Unified allocation with flags |
| `XFreeMem` | Unified deallocation |

#### File System
| API | Implementation |
|-----|----------------|
| `XCreateFileA` | `std::fstream` with VFS path translation |
| `XReadFile` | `std::fstream::read` |
| `XWriteFile` | `std::fstream::write` |
| `XSetFilePointer` | `std::fstream::seekg/seekp` |
| `XSetFilePointerEx` | 64-bit seek |
| `XGetFileSizeA` | `std::filesystem::file_size` |
| `XGetFileSizeExA` | 64-bit file size |
| `XFindFirstFileA` | `std::filesystem::directory_iterator` |
| `XFindNextFileA` | Iterator advancement |
| `XGetFileAttributesA` | `std::filesystem::status` |
| `NtCreateFile` | VFS file creation |
| `NtClose` | Handle cleanup |

#### Threading & Synchronization
| API | Implementation |
|-----|----------------|
| `ExCreateThread` | `std::thread` + GuestThreadContext |
| `KeDelayExecutionThread` | `std::this_thread::sleep_for` |
| `KeWaitForSingleObject` | `std::atomic::wait` |
| `KeWaitForMultipleObjects` | Multi-object wait loop |
| `KeSetEvent` | `std::atomic::store` + `notify` |
| `KeResetEvent` | `std::atomic::store(false)` |
| `NtCreateEvent` | Event object creation |
| `KeInitializeSemaphore` | Semaphore object creation |
| `KeReleaseSemaphore` | Atomic increment + notify |
| `RtlInitializeCriticalSection` | Mutex initialization |
| `RtlEnterCriticalSection` | Mutex lock |
| `RtlLeaveCriticalSection` | Mutex unlock |
| `KfAcquireSpinLock` | Atomic spinlock |
| `KfReleaseSpinLock` | Atomic unlock |
| `KeSetBasePriorityThread` | Thread priority mapping |
| `NtSuspendThread` | Thread suspension |

#### Content Management (Xam)
| API | Implementation |
|-----|----------------|
| `XamContentCreateEx` | VFS content mounting |
| `XamContentClose` | Content handle cleanup |
| `XamContentCreateEnumerator` | Content iteration setup |
| `XamEnumerate` | Content iteration |
| `XamContentGetDeviceState` | Always returns "ready" |
| `XamContentGetDeviceData` | Returns storage device info |
| `XamContentGetCreator` | Returns current user |
| `XamUserGetSigninState` | Always "signed in" |
| `XamUserGetSigninInfo` | Returns user profile |
| `XamShowMessageBoxUI` | ImGui message box |
| `XamShowDeviceSelectorUI` | Auto-selects default device |
| `XamNotifyCreateListener` | Notification system setup |

#### Audio
| API | Implementation |
|-----|----------------|
| `XAudioRegisterRenderDriverClient` | SDL audio callback registration |
| `XAudioUnregisterRenderDriverClient` | Audio cleanup |
| `XAudioSubmitRenderDriverFrame` | SDL audio queue submission |
| `XMACreateContext` | XMA decoder context |
| `XMAReleaseContext` | XMA decoder cleanup |

#### Input
| API | Implementation |
|-----|----------------|
| `XInputGetState` | SDL controller state translation |
| `XInputSetState` | SDL haptic feedback |
| `XInputGetCapabilities` | Controller capability query |

#### Graphics
| API | Implementation |
|-----|----------------|
| `VdQueryVideoMode` | Returns current display mode |
| `VdSwap` | Swap chain present (stubbed, handled elsewhere) |
| `VdPersistDisplay` | Display state preservation |
| `MmGetPhysicalAddress` | GPU address translation |

#### System
| API | Implementation |
|-----|----------------|
| `KeQueryPerformanceFrequency` | `QueryPerformanceFrequency` / clock |
| `QueryPerformanceCounter` | High-resolution timer |
| `XGetLanguage` | System language detection |
| `XGetGameRegion` | Returns region code |
| `XGetAVPack` | Returns HDMI |
| `ExGetXConfigSetting` | Config value lookup |
| `XexCheckExecutablePrivilege` | Always returns privileged |

---

### Logging Stubs (Hooked but Minimal Implementation)

These APIs are hooked and log their calls but don't perform full functionality. They work because the game doesn't depend on their results:

#### Video/Display (Safe to Stub)
| API | Reason Safe |
|-----|-------------|
| `VdInitializeEngines` | Host GPU init handled separately |
| `VdShutdownEngines` | Cleanup handled by host |
| `VdSetDisplayMode` | Display mode set by host |
| `VdSetGraphicsInterruptCallback` | VBlank handled differently |
| `VdGetCurrentDisplayInformation` | Not used for rendering |
| `VdGetCurrentDisplayGamma` | Gamma handled by host |
| `VdQueryVideoFlags` | Not used |
| `VdCallGraphicsNotificationRoutines` | Not needed |
| `VdInitializeScalerCommandBuffer` | Scaling handled by host |
| `VdEnableRingBufferRPtrWriteBack` | Command buffer not emulated |
| `VdInitializeRingBuffer` | Command buffer not emulated |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | Not emulated |
| `VdHSIOCalibrationLock` | Hardware calibration N/A |
| `VdIsHSIOTrainingSucceeded` | Always succeeds |
| `VdRetrainEDRAM` | EDRAM not emulated |
| `VdEnableDisableClockGating` | Power management N/A |

#### Network (Not Used by Game)
| API | Reason Safe |
|-----|-------------|
| `NetDll_WSAStartup` | No online features |
| `NetDll_WSACleanup` | No online features |
| `NetDll_socket` | No online features |
| `NetDll_closesocket` | No online features |
| `NetDll_setsockopt` | No online features |
| `NetDll_bind` | No online features |
| `NetDll_connect` | No online features |
| `NetDll_listen` | No online features |
| `NetDll_accept` | No online features |
| `NetDll_recv` | No online features |
| `NetDll_send` | No online features |

#### Debug/Development
| API | Reason Safe |
|-----|-------------|
| `DbgPrint` | Debug output not needed |
| `OutputDebugStringA` | Debug output not needed |
| `KeBugCheck` | Triggers debugger trap instead |
| `KeBugCheckEx` | Triggers debugger trap instead |
| `RtlRaiseException` | Exception handling different |

#### System Queries (Return Defaults)
| API | Default Return |
|-----|----------------|
| `MmQueryStatistics` | Returns large available memory |
| `NtQueryVirtualMemory` | Returns valid memory info |
| `MmQueryAddressProtect` | Returns PAGE_READWRITE |
| `KeGetCurrentProcessType` | Returns user mode |

#### Low-Level Kernel (Not Applicable)
| API | Reason Safe |
|-----|-------------|
| `KeLockL2` | Cache management N/A |
| `KeUnlockL2` | Cache management N/A |
| `KeEnterCriticalRegion` | Kernel regions N/A |
| `KeLeaveCriticalRegion` | Kernel regions N/A |
| `ObCreateSymbolicLink` | Kernel objects not emulated |
| `ObDeleteSymbolicLink` | Kernel objects not emulated |

---

### No-Op Stubs (GUEST_FUNCTION_STUB)

These functions are completely stubbed (do nothing) because they're not needed:

#### String Formatting (Handled by Host)
| API | Reason |
|-----|--------|
| `vsprintf` | Game uses these for logging, output discarded |
| `_vsnprintf` | Logging only |
| `sprintf` | Logging only |
| `_snprintf` | Logging only |
| `_snwprintf` | Logging only |
| `vswprintf` | Logging only |
| `_vscwprintf` | Logging only |
| `swprintf` | Logging only |

#### Heap Management
| API | Reason |
|-----|--------|
| `HeapCreate` | Single global heap used |
| `HeapDestroy` | Never destroyed |

#### Graphics (Replaced)
| API | Reason |
|-----|--------|
| `SetGammaRamp` | Gamma handled by host display |
| `D3DXFilterTexture` | Mipmaps generated differently |
| Various D3D helpers | Replaced by plume abstraction |

#### Audio
| API | Reason |
|-----|--------|
| `sub_82E58728` | Volume setter, handled elsewhere |

---

### Implementation Decision Matrix

| API Category | Strategy | Justification |
|--------------|----------|---------------|
| **Memory** | Full implementation | Core functionality, must work correctly |
| **File System** | Full implementation | Game data access is critical |
| **Threading** | Full implementation | Concurrency must be preserved |
| **Synchronization** | Full implementation | Race conditions otherwise |
| **Graphics** | Replaced entirely | Modern API translation required |
| **Audio** | Full implementation | Player-facing feature |
| **Input** | Full implementation | Player-facing feature |
| **Content/Save** | Full implementation | User data must persist |
| **Network** | Stubbed | Game has no online features |
| **Debug** | Stubbed/Logging | Development-only APIs |
| **Video Init** | Stubbed | Replaced by host initialization |
| **Hardware** | Stubbed | Xbox-specific, not applicable |

---

### Key Insight: Stub Safety Analysis

A stub is **safe** when:
1. The caller doesn't check the return value, OR
2. A default return value is acceptable, OR
3. The side effect is not needed for forward progress

A stub is **unsafe** when:
1. The caller stores the return value for later use
2. The function registers callbacks or allocates resources
3. The function modifies global state other code depends on
4. The function is part of an initialization chain

**Rule of thumb:** Stub at phase boundaries (init complete, shutdown), not in the middle of execution paths.

---


## Appendix E: Complete Xbox API Reference

This appendix provides a comprehensive mapping of every Xbox 360 API to its host implementation, organized by subsystem.

---

### E.1 Memory Management APIs

| Xbox API | Host Implementation | Details |
|----------|---------------------|---------|
| `RtlAllocateHeap` | `o1heapAllocate` | Virtual heap allocation |
| `RtlFreeHeap` | `o1heapFree` | Virtual heap deallocation |
| `RtlReAllocateHeap` | `Alloc` + `memcpy` + `Free` | Reallocation pattern |
| `RtlSizeHeap` | Query allocation metadata | Returns stored size |
| `MmAllocatePhysicalMemoryEx` | `o1heapAllocate` (physical) | GPU-aligned allocation (4KB default) |
| `MmFreePhysicalMemory` | `g_userHeap.Free` | Physical heap deallocation |
| `MmGetPhysicalAddress` | Identity function | Returns same address (no MMU) |
| `MmQueryAddressProtect` | Returns `PAGE_READWRITE` | All memory is read/write |
| `MmQueryStatistics` | Stub | Returns large available memory |
| `MmQueryAllocationSize` | Stub | Not implemented |
| `NtAllocateVirtualMemory` | Stub | Uses heap instead |
| `NtFreeVirtualMemory` | Stub | Uses heap instead |
| `NtQueryVirtualMemory` | Stub | Not implemented |
| `XAllocMem` | `AllocPhysical` or `Alloc` | Flag-driven allocation |
| `XFreeMem` | `Free` | Unified deallocation |
| `ExFreePool` | Stub | Pool not implemented |
| `ExAllocatePoolTypeWithTag` | Stub | Pool not implemented |

---

### E.2 Threading APIs

| Xbox API | Host Implementation | Details |
|----------|---------------------|---------|
| `ExCreateThread` | `std::thread` + `GuestThreadContext` | Creates native thread with guest context |
| `ExTerminateThread` | Stub | Thread termination not needed |
| `KeDelayExecutionThread` | `std::this_thread::sleep_for` | Sleep in 100ns units converted to ms |
| `KeSetBasePriorityThread` | Thread priority mapping | Maps Xbox priorities to host |
| `KeQueryBasePriorityThread` | Stub | Priority query |
| `KeSetAffinityThread` | Stub | CPU affinity not enforced |
| `KeResumeThread` | `suspended.notify_all()` | Resumes suspended thread |
| `NtSuspendThread` | `suspended.wait(true)` | Suspends thread |
| `NtResumeThread` | `suspended.notify_all()` | Resumes thread |
| `KeRaiseIrqlToDpcLevel` | Returns 0 | IRQL not meaningful on PC |
| `KfLowerIrql` | No-op | IRQL not meaningful |

---

### E.3 Synchronization APIs

| Xbox API | Host Implementation | Details |
|----------|---------------------|---------|
| `NtCreateEvent` | `CreateKernelObject<Event>` | Creates Event with `std::atomic<bool>` |
| `KeSetEvent` | `atomic.store(true)` + `notify` | Sets event, wakes waiters |
| `KeResetEvent` | `atomic.store(false)` | Resets manual-reset event |
| `NtSetEvent` | `Event::Set()` | Sets event |
| `NtClearEvent` | `Event::Reset()` | Clears event |
| `KeWaitForSingleObject` | `atomic.wait()` | Waits for event/semaphore |
| `KeWaitForMultipleObjects` | Loop with `atomic.wait()` | Wait any/all objects |
| `NtWaitForSingleObjectEx` | Stub | Uses KeWait instead |
| `NtWaitForMultipleObjectsEx` | Stub | Uses KeWait instead |
| `KeInitializeSemaphore` | `CreateKernelObject<Semaphore>` | Creates semaphore |
| `KeReleaseSemaphore` | `atomic += count` + `notify_all` | Releases semaphore count |
| `NtCreateSemaphore` | `CreateKernelObject<Semaphore>` | Creates semaphore handle |
| `NtReleaseSemaphore` | `Semaphore::Release` | Releases count |
| `RtlInitializeCriticalSection` | Init struct fields | Initializes CS |
| `RtlInitializeCriticalSectionAndSpinCount` | Init with spin count | CS with spin |
| `RtlEnterCriticalSection` | `atomic.compare_exchange` + `wait` | Lock mutex |
| `RtlLeaveCriticalSection` | `atomic.store(0)` + `notify` | Unlock mutex |
| `RtlTryEnterCriticalSection` | `atomic.compare_exchange` | Non-blocking lock attempt |
| `KfAcquireSpinLock` | `atomic.compare_exchange` loop | Spin lock acquire |
| `KfReleaseSpinLock` | `atomic.store(0)` | Spin lock release |
| `KeAcquireSpinLockAtRaisedIrql` | Same as `KfAcquireSpinLock` | IRQL variant |
| `KeReleaseSpinLockFromRaisedIrql` | Same as `KfReleaseSpinLock` | IRQL variant |
| `KeEnterCriticalRegion` | Stub | Kernel regions N/A |
| `KeLeaveCriticalRegion` | Stub | Kernel regions N/A |
| `KeLockL2` | Stub | Cache control N/A |
| `KeUnlockL2` | Stub | Cache control N/A |

---

### E.4 Thread-Local Storage APIs

| Xbox API | Host Implementation | Details |
|----------|---------------------|---------|
| `KeTlsAlloc` | Index from free list or increment | Returns TLS slot index |
| `KeTlsFree` | Add index to free list | Frees TLS slot |
| `KeTlsGetValue` | `thread_local vector[index]` | Gets TLS value |
| `KeTlsSetValue` | `thread_local vector[index] = v` | Sets TLS value |

---

### E.5 File System APIs

| Xbox API | Host Implementation | Details |
|----------|---------------------|---------|
| `XCreateFileA` | `std::fstream` + VFS | Opens file with path translation |
| `XReadFile` | `fstream.read()` | Reads file data |
| `XWriteFile` | `fstream.write()` | Writes file data |
| `XSetFilePointer` | `fstream.seekg/seekp` | 32-bit seek |
| `XSetFilePointerEx` | `fstream.seekg/seekp` | 64-bit seek |
| `XGetFileSizeA` | `filesystem::file_size` | Gets file size |
| `XGetFileSizeExA` | `filesystem::file_size` | 64-bit file size |
| `XFindFirstFileA` | `filesystem::directory_iterator` | Begins directory enumeration |
| `XFindNextFileA` | Iterator increment | Continues enumeration |
| `XGetFileAttributesA` | `filesystem::status` | Gets file attributes |
| `XReadFileEx` | `fstream.read()` | Extended read |
| `NtCreateFile` | VFS file creation | Creates/opens file |
| `NtOpenFile` | Stub | Uses NtCreateFile |
| `NtClose` | Handle cleanup | Closes file handle |
| `NtReadFile` | Stub | Uses XReadFile |
| `NtWriteFile` | Stub | Uses XWriteFile |
| `NtReadFileScatter` | Stub | Scatter read not impl |
| `NtQueryInformationFile` | Stub | File info query |
| `NtQueryVolumeInformationFile` | Stub | Volume info |
| `NtQueryDirectoryFile` | Stub | Directory query |
| `NtQueryFullAttributesFile` | Stub | Full attributes |
| `NtSetInformationFile` | Stub | Set file info |
| `NtFlushBuffersFile` | Stub | Flush buffers |
| `FscSetCacheElementCount` | Returns 0 | Cache config ignored |

---

### E.6 Timer/Clock APIs

| Xbox API | Host Implementation | Details |
|----------|---------------------|---------|
| `QueryPerformanceCounter` | `steady_clock::now().count()` | High-resolution timer |
| `QueryPerformanceFrequency` | `steady_clock::period` | Clock frequency |
| `KeQueryPerformanceFrequency` | Returns `49875000` | Xbox PPC frequency |
| `KeQuerySystemTime` | `system_clock::now()` + epoch | FILETIME format |
| `GetTickCount` | `steady_clock` in milliseconds | Millisecond tick count |
| `RtlTimeToTimeFields` | Stub | Time conversion |
| `RtlTimeFieldsToTime` | Stub | Time conversion |

---

### E.7 Memory Operations APIs

| Xbox API | Host Implementation | Details |
|----------|---------------------|---------|
| `memcpy` | Native `memcpy` | Multiple hook addresses |
| `memmove` | Native `memmove` | Overlapping copy |
| `memset` | Native `memset` | Memory fill |
| `RtlFillMemoryUlong` | Stub | Fill with ULONG |
| `RtlCompareMemoryUlong` | Stub | Compare with ULONG |

---

### E.8 String APIs

| Xbox API | Host Implementation | Details |
|----------|---------------------|---------|
| `RtlInitAnsiString` | Fills `XANSI_STRING` struct | String initialization |
| `RtlInitUnicodeString` | Stub | Unicode init |
| `RtlUnicodeToMultiByteN` | Loop copy with truncation | Unicode → ANSI |
| `RtlMultiByteToUnicodeN` | Loop copy | ANSI → Unicode |
| `RtlUnicodeStringToAnsiString` | Stub | String conversion |
| `RtlFreeAnsiString` | Stub | String free |
| `RtlCompareStringN` | Stub | String compare |
| `RtlUpcaseUnicodeChar` | Stub | Uppercase char |
| `vsprintf` | Stub | Format string (logging) |
| `_vsnprintf` | Stub | Format string (logging) |
| `sprintf` | Stub | Format string (logging) |
| `_snprintf` | Stub | Format string (logging) |
| `_snwprintf` | Stub | Wide format string |
| `vswprintf` | Stub | Wide format string |
| `_vscwprintf` | Stub | Wide format count |
| `swprintf` | Stub | Wide format string |

---

### E.9 Content/Storage APIs (Xam)

| Xbox API | Host Implementation | Details |
|----------|---------------------|---------|
| `XamContentCreateEx` | VFS content mount | Mounts content package |
| `XamContentClose` | Handle cleanup | Closes content |
| `XamContentDelete` | Stub | Content deletion |
| `XamContentGetCreator` | Returns current user | Creator query |
| `XamContentCreateEnumerator` | Creates iterator | Content enumeration setup |
| `XamEnumerate` | Iterator advancement | Content enumeration |
| `XamContentGetDeviceState` | Returns "ready" | Device state query |
| `XamContentGetDeviceData` | Returns device info | Storage device info |

---

### E.10 User/Profile APIs (Xam)

| Xbox API | Host Implementation | Details |
|----------|---------------------|---------|
| `XamUserGetSigninState` | Returns "signed in" | Always signed in |
| `XamUserGetSigninInfo` | Returns user profile | Profile data |
| `XamUserReadProfileSettings` | Zeroes buffer | Profile settings |
| `XamShowSigninUI` | Stub | Sign-in UI |
| `XamShowDeviceSelectorUI` | Auto-selects default | Device selector |
| `XamShowMessageBoxUI` | ImGui message box | Message dialog |
| `XamShowMessageBoxUIEx` | Stub | Extended message box |
| `XamShowDirtyDiscErrorUI` | Stub | Disc error UI |
| `XamEnableInactivityProcessing` | Stub | Inactivity processing |
| `XamResetInactivity` | Stub | Reset inactivity timer |
| `XamGetSystemVersion` | Returns version | System version |
| `XamGetExecutionId` | Stub | Execution ID |
| `XamLoaderTerminateTitle` | Stub | Title termination |
| `XamLoaderLaunchTitle` | Stub | Title launch |
| `XamNotifyCreateListener` | Notification setup | Creates listener |

---

### E.11 Input APIs

| Xbox API | Host Implementation | Details |
|----------|---------------------|---------|
| `XamInputGetState` | SDL controller state | Translates to `XINPUT_STATE` |
| `XamInputSetState` | SDL haptic feedback | Controller vibration |
| `XamInputGetCapabilities` | Controller capability query | Returns capabilities |

---

### E.12 Audio APIs

| Xbox API | Host Implementation | Details |
|----------|---------------------|---------|
| `XAudioRegisterRenderDriverClient` | SDL audio callback registration | Registers audio callback |
| `XAudioUnregisterRenderDriverClient` | Audio cleanup | Unregisters callback |
| `XAudioSubmitRenderDriverFrame` | `SDL_QueueAudio` | Submits audio samples |
| `XAudioGetVoiceCategoryVolume` | Stub | Volume query |
| `XAudioGetVoiceCategoryVolumeChangeMask` | Returns 0 | Volume change mask |
| `XMACreateContext` | Stub | XMA decoder context |
| `XMAReleaseContext` | Stub | XMA decoder cleanup |

---

### E.13 Video/Display APIs

| Xbox API | Host Implementation | Details |
|----------|---------------------|---------|
| `XGetVideoMode` / `VdQueryVideoMode` | Returns 1280x720 | Video mode query |
| `VdInitializeEngines` | Stub | GPU init (host handles) |
| `VdShutdownEngines` | Stub | GPU shutdown |
| `VdSwap` | Stub | Swap chain (handled elsewhere) |
| `VdPersistDisplay` | Returns false | Display persistence |
| `VdSetDisplayMode` | Stub | Display mode set |
| `VdGetCurrentDisplayInformation` | Stub | Display info |
| `VdGetCurrentDisplayGamma` | Stub | Gamma info |
| `VdQueryVideoFlags` | Stub | Video flags |
| `VdSetGraphicsInterruptCallback` | Stub | VBlank callback |
| `VdCallGraphicsNotificationRoutines` | Stub | Notifications |
| `VdInitializeScalerCommandBuffer` | Stub | Scaler init |
| `VdInitializeRingBuffer` | Stub | Command buffer |
| `VdEnableRingBufferRPtrWriteBack` | Stub | Ring buffer |
| `VdGetSystemCommandBuffer` | Stub | Command buffer |
| `VdSetSystemCommandBufferGpuIdentifierAddress` | Stub | GPU address |
| `VdHSIOCalibrationLock` | Stub | Hardware calibration |
| `VdIsHSIOTrainingSucceeded` | Stub | HSIO training |
| `VdRetrainEDRAM` | Returns 0 | EDRAM retrain |
| `VdRetrainEDRAMWorker` | Stub | EDRAM worker |
| `VdEnableDisableClockGating` | Stub | Power management |

---

### E.14 System/Region APIs

| Xbox API | Host Implementation | Details |
|----------|---------------------|---------|
| `XGetLanguage` | `Config::Language` | Returns configured language |
| `XGetGameRegion` | Returns 0xFFFF | All regions enabled |
| `XGetAVPack` | Returns 0x1F (HDMI) | AV pack type |
| `ExGetXConfigSetting` | Config lookup | System settings |
| `KeGetCurrentProcessType` | Returns 1 (user mode) | Process type |
| `HalReturnToFirmware` | Stub | Return to dashboard |
| `GlobalMemoryStatus` | Returns large values | Memory status |

---

### E.15 Exception/Debug APIs

| Xbox API | Host Implementation | Details |
|----------|---------------------|---------|
| `DbgPrint` | Stub | Debug output |
| `DbgBreakPoint` | Stub | Debug breakpoint |
| `OutputDebugStringA` | Native (Windows) / Stub | Debug string |
| `KeBugCheck` | `__builtin_debugtrap()` | Triggers debugger |
| `KeBugCheckEx` | `__builtin_debugtrap()` | Triggers debugger |
| `RtlRaiseException` | Stub | Exception raising |
| `RtlUnwind` | Stub | Stack unwinding |
| `RtlCaptureContext` | Stub | Context capture |
| `__C_specific_handler` | Stub | SEH handler |

---

### E.16 Module/Executable APIs

| Xbox API | Host Implementation | Details |
|----------|---------------------|---------|
| `XexCheckExecutablePrivilege` | Returns TRUE | Always privileged |
| `XexGetProcedureAddress` | Stub | Procedure lookup |
| `XexGetModuleSection` | Stub | Section lookup |
| `XexGetModuleHandle` | Stub | Module handle |
| `XexExecutableModuleHandle` | Stub | Module handle |
| `RtlImageXexHeaderField` | Stub | XEX header field |
| `ExRegisterTitleTerminateNotification` | Stub | Termination callback |

---

### E.17 Object Manager APIs

| Xbox API | Host Implementation | Details |
|----------|---------------------|---------|
| `ObReferenceObjectByHandle` | `GetKernelObject` | Handle → object |
| `ObDereferenceObject` | Stub | Object dereference |
| `ObReferenceObject` | Stub | Object reference |
| `ObCreateSymbolicLink` | Stub | Symlink creation |
| `ObDeleteSymbolicLink` | Stub | Symlink deletion |
| `ObIsTitleObject` | Stub | Object query |
| `NtDuplicateObject` | Stub | Object duplication |

---

### E.18 I/O Manager APIs

| Xbox API | Host Implementation | Details |
|----------|---------------------|---------|
| `IoCreateDevice` | Stub | Device creation |
| `IoDeleteDevice` | Stub | Device deletion |
| `IoInvalidDeviceRequest` | Stub | Invalid request |
| `IoCompleteRequest` | Stub | Request completion |
| `IoCheckShareAccess` | Stub | Share access |
| `IoSetShareAccess` | Stub | Share access |
| `IoRemoveShareAccess` | Stub | Share access |

---

### E.19 Network APIs (All Stubbed)

| Xbox API | Host Implementation | Details |
|----------|---------------------|---------|
| `NetDll_WSAStartup` | Stub | Network init |
| `NetDll_WSACleanup` | Stub | Network cleanup |
| `NetDll_socket` | Stub | Socket creation |
| `NetDll_closesocket` | Stub | Socket close |
| `NetDll_setsockopt` | Stub | Socket options |
| `NetDll_bind` | Stub | Socket bind |
| `NetDll_connect` | Stub | Socket connect |
| `NetDll_listen` | Stub | Socket listen |
| `NetDll_accept` | Stub | Socket accept |
| `NetDll_select` | Stub | Socket select |
| `NetDll_recv` | Stub | Socket receive |
| `NetDll_send` | Stub | Socket send |
| `NetDll_inet_addr` | Stub | Address conversion |
| `NetDll___WSAFDIsSet` | Stub | FD set check |
| `NetDll_XNetStartup` | Stub | XNet init |
| `NetDll_XNetGetTitleXnAddr` | Stub | XNet address |

---

### E.20 Cryptography APIs (All Stubbed)

| Xbox API | Host Implementation | Details |
|----------|---------------------|---------|
| `XeCryptBnQwBeSigVerify` | Stub | Signature verification |
| `XeKeysGetKey` | Stub | Key retrieval |
| `XeCryptRotSumSha` | Stub | Rotating sum SHA |
| `XeCryptSha` | Stub | SHA hash |

---

### E.21 Storage File System APIs (All Stubbed)

| Xbox API | Host Implementation | Details |
|----------|---------------------|---------|
| `StfsCreateDevice` | Stub | STFS device creation |
| `StfsControlDevice` | Stub | STFS device control |

---

### E.22 Miscellaneous APIs

| Xbox API | Host Implementation | Details |
|----------|---------------------|---------|
| `XNotifyGetNext` | Notification system | Gets next notification |
| `XMsgStartIORequest` | Message system | IO request |
| `XMsgStartIORequestEx` | Stub | Extended IO request |
| `XMsgInProcessCall` | Message handling | In-process call |
| `KiApcNormalRoutineNop` | Returns 0 | APC routine |
| `KeEnableFpuExceptions` | Stub | FPU exceptions |
| `KeCertMonitorData` | Stub | Cert monitoring |
| `KeDebugMonitorData` | Stub | Debug monitoring |
| `KeTimeStampBundle` | Stub | Timestamp bundle |
| `XboxHardwareInfo` | Stub | Hardware info |
| `ExLoadedCommandLine` | Stub | Command line |

---


## Appendix F: Common Pitfalls & Debugging Guide

This section documents common mistakes and debugging strategies for static recompilation projects.

---

### F.1 Endianness Bugs

**Symptom:** Corrupted data, crashes on pointer dereference, wrong values

**The Problem:** Xbox 360 is big-endian, x86-64 is little-endian. Every multi-byte value crossing the guest/host boundary needs byte-swapping.

**Common Mistakes:**

```cpp
// WRONG: Reading guest value without swap
uint32_t value = *(uint32_t*)guestPtr;  // Bytes are reversed!

// CORRECT: Using be<T> template
be<uint32_t>* guestPtr = ...;
uint32_t value = *guestPtr;  // Automatic byte-swap on read

// CORRECT: Manual swap
uint32_t value = ByteSwap(*(uint32_t*)guestPtr);
```

**Debugging Tips:**
- If a value looks wrong, try `ByteSwap()` on it
- 0x12345678 becomes 0x78563412 when endianness is wrong
- Pointers that look like `0x82XXXXXX` are likely guest addresses (correct)
- Pointers that look like `0xXXXXXX82` are likely endian-swapped (bug)

---

### F.2 Pointer Translation Errors

**Symptom:** Access violation, reading garbage, writing to wrong location

**The Problem:** Guest pointers are 32-bit offsets into mapped memory. Host pointers are 64-bit absolute addresses.

**Common Mistakes:**

```cpp
// WRONG: Using guest address as host pointer
void* ptr = (void*)guestAddress;  // Crash!

// CORRECT: Translate guest to host
void* ptr = g_memory.Translate(guestAddress);

// WRONG: Passing host pointer to guest
ctx.r3.u32 = (uint32_t)hostPtr;  // Wrong address!

// CORRECT: Map host to guest
ctx.r3.u32 = g_memory.MapVirtual(hostPtr);
```

**Debugging Tips:**
- Guest addresses: typically 0x82XXXXXX (code) or 0x20000-0x7FEA0000 (heap)
- Host addresses: typically 0x1XXXXXXXX (mapped memory base)
- If `base + guest == host`, translation is correct

---

### F.3 Unsafe Stub Mistakes

**Symptom:** Game appears to work initially, then crashes or hangs later

**The Problem:** Stubbing a function that has important side effects.

**Unsafe to Stub:**
- Functions that allocate and return objects
- Functions that register callbacks
- Functions that modify global state
- Initialization functions in a chain

**Safe to Stub:**
- Logging/debug output functions
- Hardware-specific queries (return sensible defaults)
- Network functions (if game has no online features)
- Functions at phase boundaries (shutdown, optional features)

**Debugging Tips:**
- Add `LOG_UTILITY()` to stubs to see call frequency
- If stub is called frequently, it may need implementation
- Trace callers to understand what they expect

---

### F.4 Frame Rate / Timing Issues

**Symptom:** Physics too fast/slow, camera jitter, animation glitches at high FPS

**The Problem:** Original game assumed 30 FPS, code uses fixed timesteps or frame-count-based timing.

**Common Patterns to Fix:**

```cpp
// Pattern 1: Fixed delta time assumption
position += velocity * 0.0333f;  // Assumes 30 FPS

// Fix: Use actual delta time
position += velocity * deltaTime;

// Pattern 2: Frame-count-based timing
if (frameCounter % 2 == 0) { ... }  // Every 2 frames at 30 = 15 Hz

// Fix: Use time-based check
if (elapsedTime >= 0.0667f) { ... }  // 15 Hz regardless of FPS

// Pattern 3: Lerp with frame-rate-dependent alpha
value = lerp(value, target, 0.1f);  // Too fast at high FPS

// Fix: Frame-rate-independent lerp
value = lerp(value, target, 1.0f - pow(0.9f, deltaTime * 30.0f));
```

**Debugging Tips:**
- Test at 30 FPS first to verify behavior matches original
- Gradually increase FPS to find where things break
- Look for magic numbers like 0.0333, 0.0166, 30, 60

---

### F.5 Thread Safety Issues

**Symptom:** Intermittent crashes, data corruption, deadlocks

**The Problem:** Host threading may have different timing than Xbox threading.

**Common Mistakes:**

```cpp
// WRONG: Accessing global without synchronization
globalValue = newValue;  // Race condition!

// CORRECT: Use atomic or mutex
std::atomic<int> globalValue;
globalValue.store(newValue);

// WRONG: Assuming order of operations across threads
threadA: setup();
threadB: use();  // May run before setup completes!

// CORRECT: Use synchronization primitive
threadA: setup(); event.Set();
threadB: event.Wait(); use();
```

**Debugging Tips:**
- Run with thread sanitizer if available
- Add logging with thread IDs to trace execution order
- Look for `std::atomic` usage in kernel objects

---

### F.6 Calling Convention Mismatches

**Symptom:** Wrong values in function parameters, stack corruption

**The Problem:** PPC and x86-64 have different calling conventions.

**PPC Calling Convention:**
- r3-r10: Integer/pointer arguments
- f1-f13: Floating-point arguments
- r3: Return value (integer/pointer)
- f1: Return value (float/double)
- r1: Stack pointer
- r13: TLS base

**x86-64 Calling Convention (Windows):**
- RCX, RDX, R8, R9: First 4 arguments
- XMM0-XMM3: Float arguments
- RAX: Return value
- XMM0: Float return

**Debugging Tips:**
- Check `function_args()` template in `function.h`
- Verify argument count and types match between guest and host
- Check if return value is pointer, integer, or float

---

### F.7 Debugging Workflow

**Step 1: Identify the crash location**
```cpp
// Add logging to narrow down
LOG_UTILITY("Entering function X");
// ... code ...
LOG_UTILITY("Checkpoint 1");
// ... code that crashes ...
```

**Step 2: Check guest state**
```cpp
// Log guest registers
LOGF_UTILITY("r3={:08X} r4={:08X} r5={:08X}", 
    ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
```

**Step 3: Verify pointers**
```cpp
// Check if pointer is valid guest address
if (guestAddr >= 0x20000 && guestAddr < 0x7FEA0000)
    LOG_UTILITY("Valid heap address");
else if (guestAddr >= 0x82000000 && guestAddr < 0x83200000)
    LOG_UTILITY("Valid code address");
else
    LOG_UTILITY("INVALID ADDRESS!");
```

**Step 4: Compare with original behavior**
- Use Xbox 360 emulator to trace original execution
- Compare register values at key points
- Verify memory contents match

---

## Appendix G: Quick Reference Card

### Address Translation

```cpp
// Guest → Host
void* host = g_memory.Translate(guest);
void* host = g_memory.base + guest;

// Host → Guest  
uint32_t guest = g_memory.MapVirtual(host);
uint32_t guest = (uint8_t*)host - g_memory.base;
```

### Byte Swapping

```cpp
// Manual swap
uint32_t swapped = ByteSwap(value);

// Automatic with template
be<uint32_t>* ptr = ...;
uint32_t native = *ptr;    // Auto-swaps on read
*ptr = native;             // Auto-swaps on write
```

### Register Access

```cpp
// Integer registers (r0-r31)
ctx.r3.u32;    // Unsigned 32-bit
ctx.r3.s32;    // Signed 32-bit
ctx.r3.u64;    // Full 64-bit

// Float registers (f0-f31)
ctx.f1.f64;    // Double precision
(float)ctx.f1.f64;  // Single precision

// Vector registers (v0-v127)
ctx.v0;        // 128-bit vector
```

### Memory Regions

| Region | Start | End | Purpose |
|--------|-------|-----|---------|
| Null guard | 0x00000000 | 0x00001000 | Trap null pointers |
| Virtual heap | 0x00020000 | 0x7FEA0000 | General allocations |
| XMA I/O | 0x7FEA0000 | 0xA0000000 | Reserved |
| Physical heap | 0xA0000000 | 0x100000000 | GPU resources |
| Code | 0x82000000 | 0x83200000 | Recompiled code |

### Function Hooks

```cpp
// Replace entire function
GUEST_FUNCTION_HOOK(sub_XXXXXXXX, HostFunction);

// Stub (do nothing)
GUEST_FUNCTION_STUB(sub_XXXXXXXX);

// Mid-function hook (in TOML)
[[midasm_hook]]
name = "MyHook"
address = 0x82XXXXXX
registers = ["r3", "f1"]
```

### Common Macros

```cpp
// Load guest value with byte swap
uint32_t val = PPC_LOAD_U32(base + offset);

// Define PPC function
PPC_FUNC(sub_XXXXXXXX) { ... }

// Call original after hook
PPC_FUNC_IMPL(__imp__sub_XXXXXXXX);
```

### Kernel Objects

```cpp
// Create event
auto* event = CreateKernelObject<Event>(manualReset, initialState);
uint32_t handle = GetKernelHandle(event);

// Wait
event->Wait(INFINITE);  // or timeout in ms

// Signal
event->Set();
event->Reset();
```

---

## Appendix H: Glossary

| Term | Definition |
|------|------------|
| **Guest** | The original Xbox 360 code/context being recompiled |
| **Host** | The modern PC platform running the recompiled code |
| **PPCContext** | Structure holding all PowerPC register state |
| **be<T>** | Template wrapper for big-endian values (auto byte-swap) |
| **Midasm Hook** | Hook injected at a specific instruction address |
| **GUEST_FUNCTION_HOOK** | Macro to replace a guest function with host code |
| **GUEST_FUNCTION_STUB** | Macro to make a guest function do nothing |
| **plume** | Graphics abstraction layer for Vulkan/D3D12 |
| **o1heap** | Constant-time memory allocator used for heaps |
| **XenonRecomp** | Tool that translates PPC binary → C++ source |
| **XenosRecomp** | Tool that translates Xenos shaders → HLSL |
| **VFS** | Virtual File System for path translation |
| **TLS** | Thread-Local Storage |
| **TEB** | Thread Environment Block (per-thread data structure) |
| **PCR** | Processor Control Region (per-CPU data structure) |
| **XEX** | Xbox Executable format |
| **Xenos** | Xbox 360 GPU (ATI R500 derivative) |
| **Xenon** | Xbox 360 CPU (PowerPC-based) |
| **EDRAM** | Embedded DRAM on Xbox 360 GPU |
| **XMA** | Xbox Media Audio codec |
| **Xam** | Xbox Application Manager (system APIs) |
| **HSIO** | High-Speed I/O (Xbox 360 GPU/memory bus) |
| **DXIL** | DirectX Intermediate Language (D3D12 shaders) |
| **SPIR-V** | Vulkan shader binary format |
| **ZSTD** | Zstandard compression algorithm |
| **XXH3** | Fast hash algorithm used for shader/pipeline lookup |

---

