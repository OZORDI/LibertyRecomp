# GPU Initialization Blocking Point Research

## Executive Summary

The game blocks during initialization at `sub_82850028`, which calls `TLS[1676]->vtable[15]()`. This is an Xbox 360 GPU device vtable method for resource creation. The vtable chain leads to invalid/uninitialized memory on PC.

---

## Blocking Call Chain (Traced Dec 21, 2025)

```
sub_8218C600 (Worker Init) ENTER
├── sub_82192578 ✅ EXIT (storage init)
├── sub_82851548 ✅ EXIT  
├── sub_82856C38 ✅ EXIT
├── sub_8285A0E0 ✅ EXIT
├── sub_82850748 ✅ EXIT
├── sub_82856C90 ✅ EXIT (video/shader init)
│
└── vtable[1] = sub_82857240 (Render context method)
    └── sub_82856BA8 (GPU state setup)
        └── sub_8286DA20 (GPU resource alloc wrapper)
            └── sub_8286D668 (GPU resource init)
                └── sub_8286BAE0 (GPU buffer setup)
                    ├── sub_8221B7A0 ✅ EXIT
                    ├── sub_829E5C38 ✅ EXIT (GPU format query)
                    └── sub_82850028 (GPU resource create) ❌ BLOCKS
                        └── TLS[1676]->vtable[15]() ❌ BLOCKS
```

---

## Root Cause Analysis

### The Blocking Code (ppc_recomp.66.cpp:93221-93249)
```cpp
ctx.r11.u64 = PPC_LOAD_U32(ctx.r13.u32 + 0);     // r11 = TLS base  
ctx.r10.s64 = 1676;                               // offset 1676
ctx.r3.u64 = PPC_LOAD_U32(ctx.r10.u32 + ctx.r11.u32);  // r3 = TLS[1676] = GPU device
ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);      // r11 = device->vtable
ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 60);    // r11 = vtable[15] (offset 60)
ctx.ctr.u64 = ctx.r11.u64;
PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);              // ❌ CALL BLOCKS HERE
```

### What TLS[1676] Should Contain
- On Xbox 360: GPU rendering device/context object allocated during `VdInitializeEngines`
- The object has a vtable with GPU hardware interface methods
- `vtable[15]` (offset 60) = GPU resource validation/creation method

### Why It Blocks on PC
1. `TLS[1676]` is used for both memory manager (offset 1676) and GPU device
2. Our memory allocator hooks set up a memory manager at this offset
3. But the GPU device vtable methods are never initialized
4. When `vtable[15]` is called, it jumps to garbage/NULL causing hang

---

## Existing Codebase Patterns

### Pattern 1: Memory Manager at TLS[1676]
From `imports.cpp`:
```cpp
constexpr uint32_t MEM_MGR_OFFSET = 1676;  // Offset to memory manager pointer
// Memory allocator reads: [r13+0] -> [+1676] -> [vtable+8]
```

### Pattern 2: GpuMemAllocStub (gpu/video.cpp)
The codebase already has GPU memory allocation stubbed:
```cpp
static uint32_t GpuMemAllocStub(uint32_t size, be<uint32_t>* outOffset) {
    // Return fake GPU memory offsets
    return 1; // Success
}
GUEST_FUNCTION_HOOK(sub_829DFAD8, GpuMemAllocStub);
```

### Pattern 3: PM4 Bypass Strategy
From RENDERER_REFERENCE.md:
- **Reject PM4 decoding** - hook the D3D-like wrapper layer instead
- **Hook layer 1 functions** in 0x829C/0x829D range
- Let game's own init run with stubbed GPU memory

---

## Proposed Solution Approaches

### Approach A: Hook sub_82850028 Directly (Recommended)
**Strategy:** Intercept the GPU resource creation function and return success.

```cpp
PPC_FUNC(sub_82850028) {
    // sub_82850028 creates GPU resources
    // Parameters: r3=type, r4-r10 = resource params
    // The vtable[15] call validates/creates the resource
    
    // Instead of calling vtable[15], just return success
    // The game's own structures will be set up by other code
    
    ctx.r3.s32 = 0;  // Return 0 = success (negative = error per line 93332)
    return;
}
```

**Pros:**
- Simple, targeted fix
- Follows existing GpuMemAllocStub pattern
- Lets game's own init code handle structure setup

**Cons:**
- May need to track allocated resources if draws fail later
- Need to verify return value semantics

### Approach B: Set Up Proper GPU Device Vtable at TLS[1676]
**Strategy:** During VdInitializeEngines, create a GPU device object with proper vtable.

```cpp
uint32_t VdInitializeEngines() {
    // Create GPU device object with our vtable
    uint32_t gpuDevice = AllocGuestMemory(64);
    uint32_t gpuVtable = AllocGuestMemory(128);
    
    // Set up vtable methods pointing to our hooks
    PPC_STORE_U32(gpuVtable + 60, ADDRESS_OF_OUR_RESOURCE_CREATE);
    
    // Store device at TLS[1676]
    uint32_t tlsBase = PPC_LOAD_U32(ctx.r13.u32 + 0);
    PPC_STORE_U32(tlsBase + 1676, gpuDevice);
    
    g_gpuRingBuffer.enginesInitialized = true;
    return 1;
}
```

**Pros:**
- More complete solution
- Works for all vtable methods

**Cons:**
- More complex
- Risk of conflicts with memory manager at same offset

### Approach C: Hook vtable[15] Function Directly
**Strategy:** Find the actual function address at vtable[15] and hook it.

**Pros:**
- Most precise fix

**Cons:**
- vtable[15] address is runtime-determined
- May vary between calls

---

## Recommended Implementation Plan

### Phase 1: Quick Fix (Approach A)
1. Hook `sub_82850028` to return success immediately
2. Test if game progresses past initialization
3. Monitor for any crashes during rendering

### Phase 2: Validation
1. Add tracing to verify resource handles are valid
2. Check if draws attempt to use these resources
3. Implement proper resource creation if needed

### Phase 3: Complete GPU Device (if needed)
1. If draws fail, implement proper GPU device vtable
2. Route vtable methods to host Metal/Vulkan

---

## Files to Modify

| File | Change |
|------|--------|
| `LibertyRecomp/kernel/imports.cpp` | Add hook for `sub_82850028` |
| `LibertyRecomp/gpu/video.cpp` | Alternative location if GPU-related |
| `docs/REWRITE_PLAYBOOK.md` | Document the fix |

---

## Test Plan

1. Build with `sub_82850028` hook returning success
2. Run game and trace:
   - Does `sub_82857240` exit?
   - Does `sub_8218C600` exit?
   - Does game reach main loop (`sub_828529B0`)?
   - Does `VdSwap` get called?
3. If successful, monitor for rendering issues

---

## Related Functions Analyzed

| Function | Purpose | Status |
|----------|---------|--------|
| `sub_82857240` | Render context vtable[1] | Traced |
| `sub_82856BA8` | GPU state setup | Traced |
| `sub_8286DA20` | GPU resource alloc wrapper | Traced |
| `sub_8286D668` | GPU resource init | Traced |
| `sub_8286BAE0` | GPU buffer setup | Traced |
| `sub_82850028` | GPU resource create | **BLOCKING** |
| `sub_829DFAD8` | GpuMemAlloc | Already stubbed |
| `sub_829E43E8` | GPU texture create | Called by sub_82850028 |
| `sub_829E44D8` | GPU 3D texture create | Called by sub_82850028 |

---

## Document History
- 2025-12-21: Initial research and tracing completed
