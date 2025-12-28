# LibertyRecomp Actionable Changes

Based on analysis of UnleashedRecomp patterns and LibertyRecomp's current issues.

---

## Critical Priority (Causing Crashes/Deadlocks)

| File | Location | Current Code | Problem | Recommended Change |
|------|----------|--------------|---------|-------------------|
| `kernel/imports.cpp` | Lines 3658-3666 | Force-acquire critical section after 50 spins: `owningThread.store(thisThread)` | **Corrupts lock ownership** - original owner still thinks it holds the lock, leading to data corruption when both threads modify protected data | Remove force-acquire entirely. Use proper `owningThread.wait(previousOwner)` like UnleashedRecomp |
| `kernel/imports.cpp` | Lines 3633-3670 | `RtlEnterCriticalSection` with `MAX_SPIN_LOOPS = 50` | Timeout too aggressive during init | Either remove timeout or increase to 10000+ with proper yield pattern |
| `kernel/imports.cpp` | Lines 74-82 | `TrackBlockingSemaphoreAddr()` semaphore tracking | Adds overhead and potential race conditions | Remove tracking - let game signal semaphores naturally |
| `kernel/imports.cpp` | Lines 3786-3833 | `VdSwap()` signals ALL tracked events and semaphores | Wrong pattern - signals at wrong time, causes race conditions | Remove bulk signaling. Only signal specific GPU fence events if needed |

---

## High Priority (Blocking/Init Issues)

| File | Location | Current Code | Problem | Recommended Change |
|------|----------|--------------|---------|-------------------|
| `kernel/imports.cpp` | Lines 2571-2595 | `g_trackedEventHandles` tracking in `NtCreateEvent` | Unnecessary complexity, events should be signaled by game code | Remove tracking. UnleashedRecomp doesn't track events |
| `kernel/imports.cpp` | Lines 5265-5285 | `KeInitializeSemaphore` pre-signals blocking semaphores | Pre-signaling can cause double-signal issues | Remove pre-signal. If workers block, that's a sign game code isn't running correctly |
| `kernel/imports.cpp` | Lines 4140-4207 | VBlank timer thread with periodic callbacks | GTA IV may not need VBlank interrupts like Xbox | Make VBlank optional/configurable. UnleashedRecomp stubs `VdSetGraphicsInterruptCallback` |
| `kernel/imports.cpp` | Lines 5330-5381 | `XamTaskSchedule` executes tasks synchronously | May cause re-entrancy issues if task expects async execution | Consider queuing tasks instead of immediate execution |
| `kernel/imports.cpp` | Lines 5691-5740 | `sub_827DAE40` streaming worker hook | Executing task functions directly may bypass required synchronization | Review if task should wait for proper signal first |

---

## Medium Priority (Performance/Cleanup)

| File | Location | Current Code | Problem | Recommended Change |
|------|----------|--------------|---------|-------------------|
| `kernel/imports.cpp` | Lines 100-139 | `PumpSdlEventsIfNeeded()` called from many kernel functions | Adds unpredictable delays during kernel operations | Remove from kernel functions. Handle SDL events only in main loop or dedicated pump locations |
| `kernel/imports.cpp` | Lines 2349-2431 | `NtWaitForSingleObjectEx` with extensive logging | Logging overhead in hot path | Reduce logging - only log first N occurrences |
| `kernel/imports.cpp` | Lines 4420-4492 | `KeWaitForSingleObject` wait tracking with mutex | Mutex in wait path adds contention | Remove wait tracking or make it debug-only |
| `kernel/imports.cpp` | Lines 1700-2254 | Full `NtCreateFile` implementation (500+ lines) | Complex, error-prone | Consider hooking GTA IV's file functions directly like UnleashedRecomp |
| `cpu/guest_thread.cpp` | Lines 114-126 | Debug `fprintf` in `GuestThreadFunc` | Performance overhead | Remove or make debug-only |

---

## Low Priority (Code Quality)

| File | Location | Current Code | Problem | Recommended Change |
|------|----------|--------------|---------|-------------------|
| `kernel/imports.cpp` | Lines 50-88 | Semaphore tracking globals at file top | Global mutable state | Encapsulate in a struct or make optional |
| `kernel/imports.cpp` | Lines 5768-5933 | `BootGlobals` namespace (disabled) | Dead code taking space | Remove if not needed, or document why kept |
| `kernel/imports.cpp` | Lines 211-400 | RPF file mapping tables | Hardcoded offsets | Move to external config or auto-detect |
| `kernel/imports.cpp` | Overall | 12,243 lines | Too large, hard to maintain | Split into multiple files: `file_system.cpp`, `sync_primitives.cpp`, `gpu_stubs.cpp`, etc. |

---

## Specific Code Changes

### 1. Remove Force-Acquire from Critical Section

**Current (`kernel/imports.cpp` ~line 3658):**
```cpp
// After many spins, force acquire the lock to prevent deadlock
if (loopCount > MAX_SPIN_LOOPS) {
    owningThread.store(thisThread);  // DANGEROUS
    cs->RecursionCount = 1;
    return;
}
```

**Should be (UnleashedRecomp pattern):**
```cpp
void RtlEnterCriticalSection(XRTL_CRITICAL_SECTION* cs)
{
    uint32_t thisThread = g_ppcContext->r13.u32;
    assert(thisThread != NULL);

    std::atomic_ref owningThread(cs->OwningThread);

    while (true) 
    {
        uint32_t previousOwner = 0;
        if (owningThread.compare_exchange_weak(previousOwner, thisThread) || previousOwner == thisThread)
        {
            cs->RecursionCount++;
            return;
        }
        owningThread.wait(previousOwner);  // Block until released - no timeout
    }
}
```

---

### 2. Simplify VdSwap

**Current (`kernel/imports.cpp` ~line 3756):**
```cpp
void VdSwap()
{
    // ... frame counting ...
    SDL_PumpEvents();  // REMOVE - don't pump here
    Video::Present();
    
    // Signal ALL tracked Event objects - REMOVE
    for (uint32_t h : g_trackedEventHandles) { ... }
    
    // Signal worker semaphores - REMOVE
    for (uint32_t semHandle : workerSemaphores) { ... }
}
```

**Should be (UnleashedRecomp pattern):**
```cpp
void VdSwap()
{
    LOG_UTILITY("!!! STUB !!!");  // That's it - just a stub
}
```

Or if you need frame presentation:
```cpp
void VdSwap()
{
    Video::Present();  // Only present, no signaling
}
```

---

### 3. Remove Event Tracking from NtCreateEvent

**Current (`kernel/imports.cpp` ~line 2575):**
```cpp
uint32_t NtCreateEvent(be<uint32_t>* handle, void* objAttributes, uint32_t eventType, uint32_t initialState)
{
    Event* evt = CreateKernelObject<Event>(!eventType, !!initialState);
    uint32_t h = GetKernelHandle(evt);
    *handle = h;
    
    // Track this event - REMOVE
    {
        std::lock_guard lock(g_eventTrackMutex);
        g_trackedEventHandles.push_back(h);
    }
    return 0;
}
```

**Should be:**
```cpp
uint32_t NtCreateEvent(be<uint32_t>* handle, void* objAttributes, uint32_t eventType, uint32_t initialState)
{
    *handle = GetKernelHandle(CreateKernelObject<Event>(!eventType, !!initialState));
    return 0;
}
```

---

### 4. Simplify KeInitializeSemaphore

**Current (`kernel/imports.cpp` ~line 5265):**
```cpp
void KeInitializeSemaphore(XKSEMAPHORE* semaphore, uint32_t count, uint32_t limit)
{
    semaphore->Header.Type = 5;
    semaphore->Header.SignalState = count;
    semaphore->Limit = limit;

    auto* object = QueryKernelObject<Semaphore>(semaphore->Header);
    
    // Track blocking semaphores - REMOVE
    uint32_t semAddr = (uint32_t)((uint8_t*)semaphore - g_memory.base);
    if (count == 0) {
        TrackBlockingSemaphoreAddr(semAddr);
        LOGF_WARNING(...);
    }
}
```

**Should be (UnleashedRecomp pattern):**
```cpp
void KeInitializeSemaphore(XKSEMAPHORE* semaphore, uint32_t count, uint32_t limit)
{
    semaphore->Header.Type = 5;
    semaphore->Header.SignalState = count;
    semaphore->Limit = limit;

    auto* object = QueryKernelObject<Semaphore>(semaphore->Header);
    // No tracking, no logging - just init
}
```

---

### 5. Remove SDL Pumping from Kernel Functions

**Remove `PumpSdlEventsIfNeeded()` calls from:**
- `GuestThread::Start` (guest_thread.cpp)
- `KeDelayExecutionThread` (imports.cpp)
- `KeQuerySystemTime` (imports.cpp)
- Any other kernel function

**SDL events should only be pumped in:**
- Main game loop
- Dedicated UI update function
- GameWindow::Update()

---

## Files to Consider Splitting

| Current File | Suggested Split | Contents |
|--------------|-----------------|----------|
| `kernel/imports.cpp` | `kernel/sync.cpp` | Event, Semaphore, CriticalSection, Spinlock |
| `kernel/imports.cpp` | `kernel/file_io.cpp` | NtCreateFile, NtReadFile, NtQueryInformationFile |
| `kernel/imports.cpp` | `kernel/gpu_stubs.cpp` | Vd* functions, GPU ring buffer |
| `kernel/imports.cpp` | `kernel/xam_stubs.cpp` | Xam* functions, user profile, content |
| `kernel/imports.cpp` | `kernel/net_stubs.cpp` | NetDll_* functions |
| `kernel/imports.cpp` | `kernel/memory.cpp` | NtAllocateVirtualMemory, Mm* functions |

---

## Globals to Remove or Encapsulate

| Global | Location | Recommendation |
|--------|----------|----------------|
| `g_semaphoreTrackMutex` | Line 68 | Remove |
| `g_blockingSemaphoreAddrs` | Line 69 | Remove |
| `g_initComplete` | Line 70 | Remove or make part of boot manager |
| `g_eventTrackMutex` | Line 2572 | Remove |
| `g_trackedEventHandles` | Line 2573 | Remove |
| `g_handleWaitCounts` | Line 4425 | Remove or make debug-only |
| `g_mostWaitedHandle` | Line 4426 | Remove or make debug-only |

---

## Summary Priority List

1. **FIRST**: Remove force-acquire from `RtlEnterCriticalSection`
2. **SECOND**: Remove bulk event/semaphore signaling from `VdSwap`  
3. **THIRD**: Remove semaphore and event tracking
4. **FOURTH**: Remove `PumpSdlEventsIfNeeded` from kernel functions
5. **FIFTH**: Test with single-threaded mode if multi-threading still fails
6. **SIXTH**: Consider hooking GTA IV file functions directly instead of NT APIs

---

## Testing After Changes

After each change, test boot to see if:
1. Game reaches further in init
2. Specific deadlock moves to a different location (progress)
3. Crashes occur (indicates the removed code was masking a deeper issue)

If removing a hack causes immediate crash, the underlying problem needs investigation rather than re-adding the hack.

---

## Performance Optimizations from UnleashedRecomp

### 1. Thread-Local Caching (No Mutex Contention)

| Pattern | File | Purpose |
|---------|------|---------|
| `thread_local xxHashMap<std::filesystem::path> s_cache` | mod_loader.cpp:57 | Path resolution cache per thread |
| `thread_local std::vector<uint32_t> s_tlsValues` | imports.cpp:1045 | TLS values without global lock |
| `thread_local std::vector<Event*> s_events` | imports.cpp:1516 | Temp event list for multi-wait |
| `thread_local std::string builtPath` | file_system.cpp:379 | Reusable string buffer |
| `thread_local std::vector<uint8_t> s_fileData` | mod_loader.cpp:410 | Reusable file read buffer |
| `thread_local ComPtr<IDxcCompiler3> s_dxcCompiler` | video.cpp:3900 | Per-thread shader compiler |

**Key Insight**: Use `thread_local` to avoid mutex locks on hot paths. Each thread has its own cache/buffer.

---

### 2. xxHash-Based Fast Lookup Maps

```cpp
// UnleashedRecomp uses xxHash for O(1) lookups
#include <xxHashMap.h>

// Fast hash for path strings
XXH64_hash_t hash = XXH3_64bits(path.data(), path.size());
auto findResult = s_cache.find(hash);
if (findResult != s_cache.end())
    return findResult->second;  // Cache hit!

// Store result (including negative results)
return s_cache.emplace(hash, result).first->second;
```

**Apply to LibertyRecomp**: 
- RPF file lookups
- Guest address to host handle mapping
- Function pointer caches

---

### 3. Dirty State Tracking (Avoid Redundant GPU Calls)

```cpp
// UnleashedRecomp: Only update GPU state when changed
struct DirtyStates {
    bool renderTargetAndDepthStencil;
    bool viewport;
    bool pipelineState;
    bool depthBias;
    bool sharedConstants;
    bool scissorRect;
    bool vertexShaderConstants;
    bool pixelShaderConstants;
    bool indices;
};

template<typename T>
static void SetDirtyValue(bool& dirtyState, T& dest, const T& src) {
    if (dest != src) {
        dest = src;
        dirtyState = true;  // Only mark dirty if value changed
    }
}
```

**Apply to LibertyRecomp**: Track render state changes, only send to GPU when dirty.

---

### 4. Lock-Free Render Command Queue

```cpp
// UnleashedRecomp: Game thread -> Render thread communication
#include <concurrentqueue/blockingconcurrentqueue.h>

static moodycamel::BlockingConcurrentQueue<RenderCommand> g_renderQueue;

// Game thread: enqueue commands (no lock)
void SetRenderState(GuestDevice* device, uint32_t value) {
    RenderCommand cmd;
    cmd.type = RenderCommandType::SetRenderState;
    cmd.setRenderState.value = value;
    g_renderQueue.enqueue(cmd);  // Lock-free!
}

// Render thread: dequeue and process
while (true) {
    RenderCommand cmd;
    g_renderQueue.wait_dequeue(cmd);
    ProcessCommand(cmd);
}
```

**Apply to LibertyRecomp**: Use `moodycamel::BlockingConcurrentQueue` for render commands instead of mutexes.

---

### 5. Async Pipeline Compilation Thread Pool

```cpp
// UnleashedRecomp: Compile shaders in background
static moodycamel::BlockingConcurrentQueue<PipelineStateQueueItem> g_pipelineStateQueue;

// Thread pool with 2/3 of available cores
static std::vector<std::unique_ptr<std::thread>> g_pipelineCompilerThreads = []() {
    size_t threadCount = std::max(2u, (std::thread::hardware_concurrency() * 2) / 3);
    std::vector<std::unique_ptr<std::thread>> threads(threadCount);
    for (auto& thread : threads)
        thread = std::make_unique<std::thread>(PipelineCompilerThread);
    return threads;
}();

// Dynamic thread priority based on loading state
if (*SWA::SGlobals::ms_IsLoading)
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
else
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);
```

**Apply to LibertyRecomp**: Background shader compilation with dynamic priority.

---

### 6. Pipeline State Caching

```cpp
// UnleashedRecomp: Pre-compiled pipeline cache
static const PipelineState g_pipelineStateCache[] = {
#include "cache/pipeline_state_cache.h"  // Serialized pipelines
};

// Hash-based lookup for compiled pipelines
static xxHashMap<std::unique_ptr<RenderPipeline>> g_pipelines;

XXH64_hash_t hash = XXH3_64bits(&pipelineState, sizeof(pipelineState));
auto it = g_pipelines.find(hash);
if (it != g_pipelines.end())
    return it->second.get();  // Use cached pipeline
```

**Apply to LibertyRecomp**: Cache compiled shader pipelines by state hash.

---

### 7. Upload Buffer Pool

```cpp
// UnleashedRecomp: Reusable GPU upload buffers
struct UploadAllocator {
    std::vector<UploadBuffer> buffers;
    uint32_t index = 0;
    uint32_t offset = 0;

    UploadAllocation allocate(uint32_t size, uint32_t alignment) {
        offset = (offset + alignment - 1) & ~(alignment - 1);
        
        if (offset + size > UploadBuffer::SIZE) {
            ++index;  // Move to next buffer
            offset = 0;
        }
        // ... allocate from pool
    }

    void reset() {  // Called each frame
        index = 0;
        offset = 0;  // Reset without freeing memory
    }
};
```

**Apply to LibertyRecomp**: Pool GPU upload buffers per frame instead of allocating each time.

---

### 8. Precompile During Logos

```cpp
// UnleashedRecomp: Use logo display time for shader compilation
void Video::StartPipelinePrecompilation() {
    for (auto& cachedState : g_pipelineStateCache) {
        EnqueueGraphicsPipelineCompilation(cachedState, token, "precompile", true);
    }
}

// In main():
Video::StartPipelinePrecompilation();  // Start before game loop
GuestThread::Start({ entry, 0, 0 });   // Game runs while compiling
```

**Apply to LibertyRecomp**: Queue shader compilation during R* logo screens.

---

### 9. Negative Result Caching

```cpp
// UnleashedRecomp: Cache "file not found" to avoid repeated lookups
std::filesystem::path ModLoader::ResolvePath(std::string_view path) {
    // Check cache first
    auto findResult = s_cache.find(hash);
    if (findResult != s_cache.end())
        return findResult->second;  // Returns empty path if not found before

    // Search mods...
    for (auto& mod : g_mods) { /* ... */ }

    // Cache empty path for "not found" - prevents future searches
    return s_cache.emplace(hash, std::filesystem::path{}).first->second;
}
```

**Apply to LibertyRecomp**: Cache failed RPF lookups to avoid repeated file system checks.

---

### 10. Sampler State Deduplication

```cpp
// UnleashedRecomp: Only create unique sampler states
static xxHashMap<std::pair<uint32_t, std::unique_ptr<RenderSampler>>> g_samplerStates;

// Hash sampler desc, reuse if exists
XXH64_hash_t hash = XXH3_64bits(&samplerDesc, sizeof(samplerDesc));
auto it = g_samplerStates.find(hash);
if (it != g_samplerStates.end())
    return it->second.first;  // Return existing descriptor index

// Create new sampler only if unique
auto sampler = g_device->createSampler(samplerDesc);
g_samplerStates.emplace(hash, {descriptorIndex, std::move(sampler)});
```

**Apply to LibertyRecomp**: Deduplicate sampler states by content hash.

---

## Summary: Optimization Priority

| Priority | Optimization | Effort | Impact |
|----------|-------------|--------|--------|
| **HIGH** | Thread-local path cache | Low | Reduces file I/O contention |
| **HIGH** | Dirty state tracking | Medium | Reduces GPU state changes |
| **HIGH** | Lock-free render queue | Medium | Eliminates render mutex |
| **MED** | Pipeline state caching | Medium | Faster shader loading |
| **MED** | Negative result caching | Low | Reduces failed lookups |
| **MED** | Upload buffer pooling | Medium | Reduces GPU allocations |
| **LOW** | Async pipeline compilation | High | Better loading times |
| **LOW** | Sampler deduplication | Low | Minor GPU savings |

---

## Libraries Used by UnleashedRecomp

| Library | Purpose | Header |
|---------|---------|--------|
| `xxHash` | Fast hashing | `<xxhash.h>` |
| `ankerl::unordered_dense` | Fast hash map | `<ankerl/unordered_dense.h>` |
| `moodycamel::BlockingConcurrentQueue` | Lock-free queue | `<concurrentqueue/blockingconcurrentqueue.h>` |

These are all header-only and easy to integrate.
