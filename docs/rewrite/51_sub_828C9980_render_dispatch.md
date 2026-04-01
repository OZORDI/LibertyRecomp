# sub_828C9980: RAGE Effect Variable Dispatch (Shader Parameter Setter)

## Identity

**sub_828C9980** (0x828C9980 - 0x828C9A58) is RAGE engine's **grcEffect::SetVariable** --
the central dispatch function for setting shader effect variables (constants, textures,
sampler states) on the Xbox 360 D3D9 rendering pipeline.

It is NOT a "render dispatch loop" or "game tick" function. It is a leaf-level utility
called from rendering code to bind individual shader parameters to effect variable slots.

### Evidence

1. **93 static call sites** across 10 recompiled source files -- consistent with a
   ubiquitous shader parameter setter, not a loop body.

2. **Call site pattern** (every caller follows this exact template):
   ```
   r11 = object->effectInstance  // load effect instance (offset varies: +4, +108, etc.)
   r4  = r11 + 20               // parameter storage area (inline array at offset 20)
   r3  = *(r11 + 24)            // "this" pointer at offset 24 (the grcEffect object)
   r5  = <constant_id>          // shader variable slot index
   r6  = <value>                // value to set (float, int, texture ptr, etc.)
   bl sub_828C9980
   ```

3. **Function signature** (reconstructed from recompiled PPC):
   ```cpp
   void grcEffect::SetVariable(
       grcEffect*  this,     // r3 -- loaded from effectInstance[+24]
       uint32_t*   params,   // r4 -- effectInstance + 20 (variable slot array)
       int         slot,     // r5 -- variable index (1-based)
       uint32_t    newValue  // r6 -- new value for the slot
   );
   ```

4. **Callers include shader setup code** (e.g., sub_822CFA00 in gta4_recomp.9.cpp)
   that calls sub_828C9980 repeatedly with different slot indices and float constants
   from material/shader structures, then calls other virtual methods (vtable[+28])
   on child objects -- classic material render pattern.

## Internal Logic

```
sub_828C9980(this=r3, params=r4, slot=r5, newValue=r6):
    save r27-r31, allocate 128-byte frame
    r27 = this      // grcEffect*
    r29 = params     // uint32_t* variable slot array
    r31 = newValue

    if (slot == 0) goto cleanup     // slot 0 = no-op

    r28 = slot - 1                  // convert to 0-based index
    oldObj = params[r28]            // load current object at slot

    if (oldObj == NULL) goto store_only

    vtable = *(uint32_t*)oldObj     // dereference vtable pointer
    ctr = vtable[+52]              // virtual method at offset 0x34 (Release/GetRefCount)
    call [ctr] with r3=oldObj      // *** THIS IS THE 0x828C99CC CALL SITE ***

    if (retval == newValue) goto store_only
    ... (additional release/addref logic) ...

store_only:
    if (oldObj == NULL && newValue != NULL):
        params[r28] = newValue
        *(uint16_t*)(newValue + 10) += 1   // increment refcount

cleanup:
    clear global flag at [0x83183DE4]      // render state dirty flag
    deallocate frame, return
```

**The call at 0x828C99CC** invokes `vtable[+52]` on the *existing* object in the
variable slot. This is a **Release** or **GetType/GetRefCount** virtual method on a
RAGE graphics resource (grcTexture, grcVertexBuffer, etc.).

## Why 2.3 Million Calls to NULL

### Root Cause

The objects stored in the variable slot array have **uninitialized vtable pointers**.
Specifically:

1. The game's rendering systems create grcEffect instances during world initialization.
2. Each effect has a parameter array (at offset +20) that holds pointers to graphics
   resources (textures, render targets, constant buffers).
3. These resource objects are **allocated but their vtable pointers are still 0x00000000**
   because the GPU/D3D device layer that would construct them properly is not operational
   in the recompiled environment.
4. When sub_828C9980 reads `vtable = *(uint32_t*)oldObj` and then `vtable[+52]`, it gets
   0x00000000 because the vtable pointer itself is null (the object exists but was never
   properly constructed by the D3D runtime).

### The actual flow per call

```
oldObj = params[slot-1]     // non-null (object was allocated)
vtable = *oldObj            // 0x00000000 (vtable pointer never set)
ctr = *(vtable + 52)       // reads *(0x00000000 + 52) = reads address 0x00000034
                            // which in guest memory likely reads as 0x00000000
call [0x00000000]           // MISSING-FUNC fires, prints warning, returns immediately
```

## Why No Stack Overflow

The MISSING-FUNC handler (`PPC_CALL_INDIRECT_FUNC` macro in `rex/ppc/context.h`)
does **not** make a recursive function call when the target is missing. It executes:

```cpp
if (_icf_fn) {
    _icf_fn(ctx, base);        // would call if found
} else {
    fprintf(stderr, "[MISSING-FUNC] ...\n");  // just print
    fflush(stderr);                            // and return
}
```

So each null-vtable call:
1. Enters sub_828C9980 (128-byte stack frame)
2. Reaches `PPC_CALL_INDIRECT_FUNC(0x00000000)` at 0x828C99CC
3. fprintf prints the warning
4. Falls through to the rest of sub_828C9980's logic
5. sub_828C9980 **returns normally** (deallocates its 128-byte frame)
6. The caller continues to the next shader variable set call

**Each call is independent.** The 2.3M calls happen sequentially across many frames of
the game loop, not recursively. The stack depth at any point is only:

```
game_loop -> render_scene -> draw_object -> sub_828C9980 -> MISSING-FUNC (returns)
                                                          <- sub_828C9980 returns
                          -> draw_object -> sub_828C9980 -> MISSING-FUNC (returns)
                                                          <- returns
                          ... (repeat 2.3M times across the run)
```

The stack never accumulates beyond ~5-6 frames deep for this particular code path.

## sub_828C1F94: Render State Switch Dispatch

**0x828C1F94** is NOT a function. It is a **return address** inside sub_828C19C0 --
specifically the instruction after `bl sub_828C8588` at address 0x828C1F8C.

### sub_828C19C0: Render State Selector

This is a large switch-statement function that dispatches render state changes based
on a state ID (r3). It contains:

- A bounds check: `if (r3 > 36) goto 0x828C1F94` (out-of-range, skip to return)
- A **jump table** at 0x828681A4 with 37 entries (states 0-36)
- Each case sets up parameters and calls `sub_828C8588(stateID=r3, value=r4)`

### sub_828C8588: Render State Writer

A trivial function (5 instructions):
```
sub_828C8588(stateID=r3, value=r4):
    addr = 0x83169C00 + (stateID * 4)   // global render state array
    *addr = value                        // store the value
    tail-call sub_828E02E8              // mark state dirty / flush
```

This writes to a **global render state table** at 0x83169C00 (37 slots of 4 bytes =
148 bytes). sub_828E02E8 likely sets a dirty flag to trigger GPU state synchronization.

### Why 828C1F94 Hits NULL

The 37 MISSING-FUNC calls from 0x828C1F94 come from sub_828C8588's tail call to
sub_828E02E8, which dispatches to another vtable-based function. The null dispatch
happens when the GPU device object's state-flush vtable entry is uninitialized.

These 37 calls represent a one-time initialization of all render states -- far fewer
than the 2.3M from 828C99CC.

## sub_828C97E0: Effect Variable Tree Walker

**sub_828C97E0** is the companion to sub_828C9980. It walks a **hierarchical effect
variable structure** (effects can have sub-effects/techniques with their own variables):

```
sub_828C97E0(this, params, obj, slotIndex):
    if (obj == NULL) return

    // Calculate node address from slot index:
    node = params->nodeArray + slotIndex * 48

    if (node->field32 != 0):
        // Read 3D position via vtable[+40] call on obj
        pos = obj->vtable[+40](obj)        // GetPosition or GetBounds
        call sub_828C8A50(this, params, ...)  // set position-related vars

    if (node->field33 != 0):
        // Read secondary property via vtable[+44]
        val = obj->vtable[+44](obj)
        call sub_828C8A50(...)

    if (node->field34 != 0):
        // Read count via vtable[+28], then value via vtable[+24]
        count = obj->vtable[+28](obj)
        value = obj->vtable[+24](obj)       // *** calls to null here too ***
        call sub_828C8A50(...)

    return
```

The vtable offsets used (+24, +28, +40, +44) are property accessors on RAGE drawable/
entity objects. When these objects' vtables are null, each accessor call hits
MISSING-FUNC.

## sub_8291DF00: Entity Update (the 8291E144 / 8291E1B0 source)

This function iterates over game entities and calls virtual methods:
- `vtable[+16]` (0x8291E130 call site) -- likely `Update()` or `OnActivate()`
- `vtable[+40]` (0x8291E144 call site) -- likely `GetBounds()` or `GetPosition()`
- `vtable[+24]` (0x8291E1B0 call site) -- likely `GetType()` or `GetFlags()`

This is entity-level update code, not rendering code. The 192 null calls (96 pairs)
represent iteration over uninitialized entity objects during world setup.

## On Real Xbox 360 Hardware

On real hardware, these functions would operate on fully initialized D3D9 device objects:

1. **sub_828C9980** would call `Release()` or `GetRefCount()` on the existing texture/
   resource in a shader variable slot before replacing it with a new one. This is
   standard COM-like reference counting for D3D resources.

2. **sub_828C19C0/sub_828C8588** would write render states to the global state table
   and trigger a GPU command buffer flush via sub_828E02E8, ultimately translating to
   Xbox 360 GPU register writes.

3. **sub_828C97E0** would read spatial/property data from drawable objects and pass
   them as shader constants (world position, scale, material properties).

4. **sub_8291DF00** would update entity transforms, bounds, and type flags as part
   of the scene graph update pass.

## Are These D3D/GPU Device Vtable Calls?

**Partially.** The taxonomy:

| Function | What It Calls | Layer |
|----------|--------------|-------|
| sub_828C9980 | grcResource::Release (vtable+52) | RAGE graphics resource (wraps D3D) |
| sub_828C97E0 | entity::GetPosition (vtable+40,+44) | RAGE entity/drawable |
| sub_828C8588 -> sub_828E02E8 | GPU state flush | RAGE -> D3D device |
| sub_8291DF00 | entity::Update, GetBounds, GetType | RAGE entity |

The null vtable calls are NOT directly on the D3D device object. They are on **RAGE
engine wrapper objects** (grcTexture, grcEffect, rmcDrawable, etc.) whose vtable
pointers were never initialized because the D3D device initialization path is stubbed
in the recompiled environment.

**RexGlue's GPU layer** would need to ensure that when RAGE allocates these wrapper
objects, their vtable pointers get populated with valid function addresses -- either
native implementations or stubs that return sensible defaults (0 for refcounts,
identity transforms for positions, etc.).

## Key Addresses

| Address | Identity |
|---------|----------|
| 0x828C9980 | grcEffect::SetVariable (shader parameter setter) |
| 0x828C99CC | Return address of vtable[+52] call inside SetVariable |
| 0x828C97E0 | grcEffect::SetVariableFromEntity (tree walker) |
| 0x828C8588 | Render state table writer |
| 0x828C19C0 | Render state switch dispatcher (37 states) |
| 0x828C1F94 | Return address after sub_828C8588 call (NOT a function) |
| 0x828C8A50 | Sub-variable setter (called from tree walker) |
| 0x828E02E8 | GPU state dirty/flush handler |
| 0x83169C00 | Global render state table (37 x 4 bytes) |
| 0x83183DE4 | Render state dirty flag (cleared by sub_828C9980) |
| 0x8291DF00 | Entity update function (source of 8291E144/E1B0 calls) |

## Summary

The 2.3M MISSING-FUNC calls from 0x828C99CC are the game's shader system trying to
release/query existing graphics resources before replacing them with new values, on
every draw call, every frame. The objects exist (non-null pointers in the variable
slots) but their vtable pointers are 0x00000000 because the RAGE graphics resource
constructors depend on a D3D device that doesn't exist in the recompiled environment.

Each call is a flat call-return (not recursive), so the stack never accumulates.
The 2.3M count represents roughly `93 call sites * ~400 frames * ~60 variable sets
per draw` over the lifetime of the run.
