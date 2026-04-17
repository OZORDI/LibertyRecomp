# UnleashedRecomp Vertex Pipeline Study

Source: `Reference Projects/UnleashedRecomp-main/UnleashedRecomp/gpu/video.cpp`
(7882 lines) and `.../gpu/video.h`.

All line numbers reference those files unless otherwise noted.

---

## 1. Vertex struct types

### GuestBuffer (video.h:182)
Holds both vertex *and* index buffers. Discriminated by `ResourceType` at
construction:

```cpp
// video.h:182
struct GuestBuffer : GuestResource
{
    std::unique_ptr<RenderBuffer> buffer;
    void* mappedMemory = nullptr;
    uint32_t dataSize = 0;
    RenderFormat format = RenderFormat::UNKNOWN;
    uint32_t guestFormat = 0;
    bool lockedReadOnly = false;
};
```

### GuestVertexElement (video.h:258) — raw guest D3DVERTEXELEMENT9
Fixed byte layout matching the Xenon D3D struct. Read from guest memory as a
terminated array (sentinel = `stream==0xFF`, `type==D3DDECLTYPE_UNUSED`).

```cpp
// video.h:258
struct GuestVertexElement
{
    be<uint16_t> stream;
    be<uint16_t> offset;
    be<uint32_t> type;
    uint8_t method;
    uint8_t usage;
    uint8_t usageIndex;
    uint8_t padding;
};

#define D3DDECL_END() { 255, 0, 0xFFFFFFFF, 0, 0, 0 }
```

### GuestVertexDeclaration (video.h:271) — the cached host-side object
```cpp
// video.h:271
struct GuestVertexDeclaration : GuestResource
{
    XXH64_hash_t hash = 0;
    std::unique_ptr<RenderInputElement[]> inputElements;  // plume/host
    std::unique_ptr<GuestVertexElement[]> vertexElements; // original guest copy
    uint32_t inputElementCount = 0;
    uint32_t vertexElementCount = 0;
    uint32_t swappedTexcoords = 0;  // bitmask -> shared constants
    bool hasR11G11B10Normal = false; // -> spec constant
    bool vertexStreams[16]{};        // which slots actually used
    uint32_t indexVertexStream = 0;  // non-zero => instancing (idx buf as VB)
};
```

### RenderInputElement (Plume SDK type)
Ready-to-submit input-layout entry: `semanticName, semanticIndex, location,
format, slotIndex, alignedByteOffset`. Constructed at video.cpp:1481-1483 and
video.cpp:4981 via its 6-arg ctor. LibertyRecomp also pulls it from rexglue's
plume header.

---

## 2. Hook coverage — guest sub_XXX -> host handler

All in a single `GUEST_FUNCTION_HOOK` block at video.cpp:7800-7852:

| Guest | Host fn | Line |
|-|-|-|
| `sub_82BE6AD0` | `CreateVertexBuffer` | 7822 |
| `sub_82BE6BF8` | `CreateIndexBuffer` | 7823 |
| `sub_82BE6B98` | `LockVertexBuffer` | 7805 |
| `sub_82BE6BE8` | `UnlockVertexBuffer` | 7806 |
| `sub_82BE61D0` | `GetVertexBufferDesc` | 7807 |
| `sub_82BE0428` | **`CreateVertexDeclaration`** | 7842 |
| `sub_82BE02E0` | **`SetVertexDeclaration`** | 7843 |
| `sub_82BE04B0` | `GetVertexDeclaration` | 7815 |
| `sub_82BE0530` | `HashVertexDeclaration` | 7816 |
| `sub_82BE1A80` | `CreateVertexShader` | 7845 |
| `sub_82BE0110` | `SetVertexShader` | 7846 |
| `sub_82BDD0F8` | `SetStreamSource` | 7848 |
| `sub_82BDD218` | `SetIndices` | 7849 |
| `sub_82BE5900` | `DrawPrimitive` | 7838 |
| `sub_82BE5CF0` | `DrawIndexedPrimitive` | 7839 |
| `sub_82BE52F8` | `DrawPrimitiveUP` | 7840 |

(LibertyRecomp is missing hooks for the two bolded entries — that is the root
of the `NULL vertexDeclaration` crash in `SanitizePipelineState`.)

---

## 3. VDECL creation + caching mechanics

### CreateVertexDeclarationWithoutAddRef (video.cpp:4825)

Step-by-step:

1. **Count elements** by walking the `D3DVERTEXELEMENT9[]` array until the
   sentinel (video.cpp:4830). Also zeroes the `padding` byte on every element
   including the terminator (4832, 4837) so the XXHASH is deterministic:

```cpp
// 4830
while (vertexElement->stream != 0xFF && vertexElement->type != D3DDECLTYPE_UNUSED) {
    vertexElement->padding = 0;
    ++vertexElement;
    ++vertexElementCount;
}
vertexElement->padding = 0; // Clear the padding in D3DDECL_END()
```

2. **Hash + cache lookup** under `g_vertexDeclarationMutex`:

```cpp
// 4841
XXH64_hash_t hash = XXH3_64bits(vertexElements, vertexElementCount * sizeof(GuestVertexElement));
auto& vertexDeclaration = g_vertexDeclarations[hash];
if (vertexDeclaration == nullptr) { /* build new */ }
```

`g_vertexDeclarations` is declared at video.cpp:454 as
`xxHashMap<GuestVertexDeclaration*>`.

3. **Build `RenderInputElement[]`** for the host pipeline. A static
   `locations[]` table (video.cpp:4859-4878) maps `(usage, usageIndex)` ->
   shader input location 0..15 — this is what matches the recompiled HLSL's
   semantics:

```cpp
// 4859
constexpr Location locations[] = {
    { D3DDECLUSAGE_POSITION, 0, 0 },
    { D3DDECLUSAGE_NORMAL,   0, 1 },
    { D3DDECLUSAGE_TANGENT,  0, 2 },
    { D3DDECLUSAGE_BINORMAL, 0, 3 },
    { D3DDECLUSAGE_TEXCOORD, 0, 4 },
    ...
};
```

4. **Element conversion** (video.cpp:4881-4947):
   - `POSITION index==2` is skipped entirely (4883).
   - `POSITION index==1` marks the decl as instancing — its stream is recorded
     in `vertexDeclaration->indexVertexStream` (4914).
   - `NORMAL/TANGENT/BINORMAL`: `FLOAT3` is relabeled as `R32G32B32_UINT`
     (it's actually packed bits, shader decodes); otherwise
     `hasR11G11B10Normal = true` (4917-4924).
   - `TEXCOORD`: 16-bit packed types set the `swappedTexcoords` bitmask
     (4926-4941) — pushed to `g_sharedConstants.swappedTexcoords` in
     `ProcSetVertexDeclaration` (5033).
   - `vertexStreams[stream] = true` (4944) records which slots are live.

5. **Dummy-fill missing slots** (video.cpp:4949-4994): `addInputElement()`
   appends a zero-stride slot-15 entry for every required semantic not in the
   declaration (POSITION/NORMAL/TANGENT/BINORMAL/TEXCOORD0-3/COLOR0/BLENDWEIGHT/
   BLENDINDICES). The shader always sees all possible attributes; missing ones
   read from a phantom slot.

6. **Copy & store** (video.cpp:4996-5003):

```cpp
vertexDeclaration->inputElements = std::make_unique<RenderInputElement[]>(inputElements.size());
std::copy(inputElements.begin(), inputElements.end(), vertexDeclaration->inputElements.get());
vertexDeclaration->vertexElements = std::make_unique<GuestVertexElement[]>(vertexElementCount + 1);
std::copy(vertexElements, vertexElements + vertexElementCount + 1, vertexDeclaration->vertexElements.get());
vertexDeclaration->inputElementCount = uint32_t(inputElements.size());
vertexDeclaration->vertexElementCount = vertexElementCount + 1;
```

7. **AddRef** (5006) and return. `CreateVertexDeclaration` (5010) is a thin
   wrapper that AddRefs *twice* (once here, once by caller semantics) — the
   game drops one ref through the normal resource-destructor path
   (video.cpp:720-722) which invokes `~GuestVertexDeclaration()`.

### AllocPhysical
The decl itself lives in guest physical memory (`g_userHeap.AllocPhysical
<GuestVertexDeclaration>`, video.cpp:4846) so the guest can stash a pointer to
it on the device / in resources and round-trip through `MapVirtual`.

---

## 4. Binding flow: SetVertexDeclaration -> g_pipelineState.vertexDeclaration

Three-stage flow (main thread -> queue -> render thread):

### Stage 1 — guest hook enqueues (video.cpp:5017)
```cpp
// 5017
static void SetVertexDeclaration(GuestDevice* device, GuestVertexDeclaration* vertexDeclaration)
{
    RenderCommand cmd;
    cmd.type = RenderCommandType::SetVertexDeclaration;
    cmd.setVertexDeclaration.vertexDeclaration = vertexDeclaration;
    g_renderQueue.enqueue(cmd);
    device->vertexDeclaration = g_memory.MapVirtual(vertexDeclaration);
}
```

`RenderCommand::setVertexDeclaration` is a named union arm at video.cpp:976-979:
```cpp
struct {
    GuestVertexDeclaration* vertexDeclaration;
} setVertexDeclaration;
```

### Stage 2 — render thread dispatch (video.cpp:5291)
The render thread's giant switch picks the right `Proc*`:
```cpp
case RenderCommandType::SetVertexDeclaration:
    ProcSetVertexDeclaration(cmd); break;
```

### Stage 3 — commit to PipelineState (video.cpp:5027)
```cpp
// 5027
static void ProcSetVertexDeclaration(const RenderCommand& cmd)
{
    auto& args = cmd.setVertexDeclaration;
    if (args.vertexDeclaration != nullptr)
    {
        SetDirtyValue(g_dirtyStates.sharedConstants,
            g_sharedConstants.swappedTexcoords, args.vertexDeclaration->swappedTexcoords);

        uint32_t specConstants = g_pipelineState.specConstants;
        if (args.vertexDeclaration->hasR11G11B10Normal)
            specConstants |= SPEC_CONSTANT_R11G11B10_NORMAL;
        else
            specConstants &= ~SPEC_CONSTANT_R11G11B10_NORMAL;
        SetDirtyValue(g_dirtyStates.pipelineState, g_pipelineState.specConstants, specConstants);
    }
    SetDirtyValue(g_dirtyStates.pipelineState, g_pipelineState.vertexDeclaration, args.vertexDeclaration);
}
```

`g_pipelineState` (video.cpp:181) is the single global tracking struct hashed
to look up/create a `RenderPipeline` on every draw. Its layout includes
`vertexDeclaration` (line 126), `vertexStrides[16]` (line 143), `vertexShader`
(124), `instancing` (127), etc. — the full 42-byte-ish struct is XXH3'd at
video.cpp:4139.

### SetVertexShader mirror (video.cpp:5103)
Same pattern — enqueue `SetVertexShader` command, `ProcSetVertexShader`
(video.cpp:5111) sets `g_pipelineState.vertexShader`.

### SetStreamSource (video.cpp:5116)
```cpp
// 5127
static void ProcSetStreamSource(const RenderCommand& cmd)
{
    const auto& args = cmd.setStreamSource;
    SetDirtyValue(g_dirtyStates.pipelineState, g_pipelineState.vertexStrides[args.index],
        uint8_t(args.buffer != nullptr ? args.stride : 0));

    bool dirty = false;
    SetDirtyValue(dirty, g_vertexBufferViews[args.index].buffer,
        args.buffer != nullptr ? args.buffer->buffer->at(args.offset) : RenderBufferReference{});
    SetDirtyValue(dirty, g_vertexBufferViews[args.index].size,
        args.buffer != nullptr ? (args.buffer->dataSize - args.offset) : 0u);
    SetDirtyValue(dirty, g_inputSlots[args.index].stride,
        args.buffer != nullptr ? args.stride : 0u);
    // ... mark vertexStream dirty range
}
```

Strides are written into **three** places: `g_pipelineState.vertexStrides[i]`
(pipeline-hash input), `g_inputSlots[i].stride` (used when building the
RenderPipelineDesc at video.cpp:4115), and the view size at
`g_vertexBufferViews[i].size`. These are all reset to 0 on null buffer to
keep the hash stable.

`g_vertexBufferViews[16]` and `g_inputSlots[16]` are declared at video.cpp:191-192.

---

## 5. DrawPrimitive vs DrawPrimitiveUP — implicit-decl handling

### DrawPrimitive / DrawIndexedPrimitive (video.cpp:4611, 4653)
Trivial — just enqueue the draw and rely on whatever `g_pipelineState.
vertexDeclaration` was most recently `Set`. If the game never called
`SetVertexDeclaration`, `CheckInstancing()` (video.cpp:4581) dereferences a
null `vertexDeclaration` and crashes. **Nothing provides a default.**

```cpp
// 4581
static uint32_t CheckInstancing() {
    uint32_t indexCount = 0;
    SetDirtyValue(g_dirtyStates.pipelineState, g_pipelineState.instancing,
        g_pipelineState.vertexDeclaration->indexVertexStream != 0);  // null deref if unset
    ...
}
```

### DrawPrimitiveUP (video.cpp:4682) — user-pointer path
Difference from DrawPrimitive: the vertex data is **inline in the command**
(copied to an intermediary upload buffer) and **stride[0] is explicit**. The
declaration still has to come from a prior `SetVertexDeclaration`. Unleashed
does *not* synthesize one from the stride:

```cpp
// 4699
static void ProcDrawPrimitiveUP(const RenderCommand& cmd)
{
    const auto& args = cmd.drawPrimitiveUP;
    uint32_t indexCount = CheckInstancing();           // <-- same null-deref risk
    if (indexCount > 0) UnsetInstancingStream();
    SetPrimitiveType(args.primitiveType);
    SetDirtyValue(g_dirtyStates.pipelineState, g_pipelineState.vertexStrides[0],
        uint8_t(args.vertexStreamZeroStride));          // forced stride[0]
    // upload vertex data to transient buffer
    auto allocation = g_uploadAllocators[g_frame].allocate<true>(
        reinterpret_cast<const uint32_t*>(args.vertexStreamZeroData),
        args.vertexStreamZeroSize, 0x4);
    auto& vertexBufferView = g_vertexBufferViews[0];
    vertexBufferView.size = args.primitiveCount * args.vertexStreamZeroStride;
    vertexBufferView.buffer = allocation.buffer->at(allocation.offset);
    g_inputSlots[0].stride = args.vertexStreamZeroStride;
    g_dirtyStates.vertexStreamFirst = 0;
    ...
}
```

The only UP-specific state is: it **overrides** slot-0 buffer/view/stride from
the guest pointer, and it handles `D3DPT_QUADLIST`/`TRIANGLEFAN` emulation via
`g_quadIndexData` / `g_triangleFanIndexData` (video.cpp:4720-4723). It assumes
the decl bound via the most recent `SetVertexDeclaration` describes the UP
vertex format.

**Key takeaway for LibertyRecomp**: nothing magical. DrawPrimitiveUP requires
the guest to have called `SetVertexDeclaration` before it. If GTA IV's code
path calls DrawPrimitiveUP without a prior SetVertexDeclaration, the fix is to
hook `CreateVertexDeclaration` + `SetVertexDeclaration` just like Unleashed —
**not** to fabricate a decl inside UP.

### Fallback/special decls — how Unleashed side-steps the problem
Unleashed has three places that create decls outside the guest-hook path:
1. **Movie renderer**: `ScreenShaderInit` (video.cpp:5919) — stores
   `g_movieVertexDeclaration` at init so the post-init draw always has one.
2. **ImGui** (video.cpp:1480-1483) builds its own `RenderInputElement[3]`
   directly and bypasses `GuestVertexDeclaration` entirely.
3. **Precompilation** (video.cpp:7225) pre-populates `g_vertexDeclarations`
   from the baked `g_vertexDeclarationCache[]` so the cached pipeline states
   can refer to hash-indexed decls.

---

## 6. Mesh VDECL builder (Hedgehog / Sonic codebase)

`CompileMeshPipeline` at video.cpp:6419 is Unleashed's equivalent of GTA IV's
`grmGeometryQB`. The `Mesh` struct (video.cpp:6409) carries a prebuilt
`GuestVertexDeclaration*` — the game engine's Hedgehog runtime already called
`CreateVertexDeclaration` during asset load, so the precompile path just
reuses `mesh.vertexDeclaration`.

When **adding instancing or fur**, Unleashed rebuilds the decl in C++ by
appending extra elements and rehashing (video.cpp:6577-6596):

```cpp
// 6577
GuestVertexElement vertexElements[64];
memcpy(vertexElements, mesh.vertexDeclaration->vertexElements.get(),
    (mesh.vertexDeclaration->vertexElementCount - 1) * sizeof(GuestVertexElement));

if (args.instancing) {
    vertexElements[mesh.vertexDeclaration->vertexElementCount - 1] = { 1, 0,  0x2A23B9, 0, 5, 4 };
    vertexElements[mesh.vertexDeclaration->vertexElementCount + 0] = { 1, 12, 0x2C2159, 0, 5, 5 };
    vertexElements[mesh.vertexDeclaration->vertexElementCount + 1] = { 1, 16, 0x2C2159, 0, 5, 6 };
    vertexElements[mesh.vertexDeclaration->vertexElementCount + 2] = { 1, 20, 0x182886, 0, 10, 1 };
    vertexElements[mesh.vertexDeclaration->vertexElementCount + 3] = { 2, 0,  0x2C82A1, 0, 0, 1 };
    vertexElements[mesh.vertexDeclaration->vertexElementCount + 4] = D3DDECL_END();
}
...
vertexDeclaration = CreateVertexDeclarationWithoutAddRef(vertexElements);
```

The mesh hooks also pull the decl via `m_VertexDeclarationPtr.m_pD3DVertexDeclaration`
(video.cpp:6736, 6749), reinterpreted as `GuestVertexDeclaration*`. This is
key: game-engine objects in Hedgehog hold the guest pointer returned by
`CreateVertexDeclaration`; Unleashed casts it back.

---

## 7. Vertex-stride / format plumbing summary

Flow: guest `SetStreamSource(i, buf, off, stride)` ->
`ProcSetStreamSource` writes to THREE places (video.cpp:5131-5137):

| Field | Purpose |
|-|-|
| `g_pipelineState.vertexStrides[i]` (uint8) | Part of pipeline hash; baked into PSO |
| `g_inputSlots[i].stride` | Used when building `RenderPipelineDesc` at video.cpp:4115 |
| `g_vertexBufferViews[i].size` | Command-list BindVertexBuffers view size |

`SanitizePipelineState` (video.cpp:4041-4045) then zeroes strides for any slot
the decl doesn't use — prevents spurious hash changes from leftover state:
```cpp
for (size_t i = 0; i < 16; i++) {
    if (!pipelineState.vertexDeclaration->vertexStreams[i])
        pipelineState.vertexStrides[i] = 0;
}
```

DrawPrimitiveUP forces `vertexStrides[0]` + `g_inputSlots[0].stride` +
`g_vertexBufferViews[0]` to the UP stride (video.cpp:4708-4716).

---

## 8. Pipeline cache relationship to VDECL

`g_pipelines` (declared as `xxHashMap<std::unique_ptr<RenderPipeline>>` near
video.cpp:399) is indexed by **XXH3 of the entire PipelineState struct**:

```cpp
// 4139
XXH64_hash_t hash = XXH3_64bits(&pipelineState, sizeof(pipelineState));
auto& pipeline = g_pipelines[hash];
if (pipeline == nullptr)
    pipeline = CreateGraphicsPipeline(pipelineState);
```

Because `PipelineState::vertexDeclaration` is a **raw pointer** (video.cpp:126)
and the decl itself is already deduped in `g_vertexDeclarations`, identical
decls from different call sites converge on the same pipeline-cache entry.

`SanitizePipelineState` (video.cpp:4011) is called **before** hashing and
**dereferences** `pipelineState.vertexDeclaration` at line 4043. If the decl
pointer is null here, you crash. This is exactly the LibertyRecomp symptom.

`CreateGraphicsPipeline` (video.cpp:4057) builds the full
`RenderGraphicsPipelineDesc`:
- `desc.inputElements = pipelineState.vertexDeclaration->inputElements.get();`
  (video.cpp:4089)
- `desc.inputElementsCount = pipelineState.vertexDeclaration->inputElementCount;`
  (4090)
- Walks `inputElementCount` building a deduped `inputSlots[16]` keyed on
  `inputElement.slotIndex`, each slot's `stride` taken from
  `pipelineState.vertexStrides[]` (video.cpp:4101-4121).
- `PER_INSTANCE_DATA` classification is set when `instancing` is true and
  slotIndex is neither 0 nor 15 (video.cpp:4117).

### Precompile cache round-trip (video.cpp:7218-7238)
`g_vertexDeclarationCache[]` holds raw `D3DVERTEXELEMENT9[]` blobs baked into
the binary. `g_pipelineStateCache[]` holds PipelineState values whose
`vertexDeclaration` fields are actually **hashes cast to pointers**. On startup
the precompiler:
1. Walks `g_vertexDeclarationCache` and calls `CreateVertexDeclarationWithoutAddRef`
   on each, populating `g_vertexDeclarations[hash]`.
2. Walks `g_pipelineStateCache`, resolves each stashed hash through
   `g_vertexDeclarations[hash]` to get the real pointer, then compiles.

This is how Unleashed avoids JIT compilation stalls — but it depends entirely
on the `CreateVertexDeclaration` plumbing working correctly.

---

## Adoption patterns for LibertyRecomp

1. **Hook CreateVertexDeclaration + SetVertexDeclaration** exactly like
   video.cpp:4825 / 5017. GTA IV's guest shims are the same D3D9 API, only
   the `sub_XXXXXXXX` address differs. Without both hooks,
   `g_pipelineState.vertexDeclaration` is permanently null and
   `SanitizePipelineState` crashes at the first draw.

2. **Deduplicate by XXH3 of the canonical `GuestVertexElement[]`** in an
   `xxHashMap<GuestVertexDeclaration*>` — zero the `padding` byte of every
   element *including* the terminator before hashing (video.cpp:4832, 4837)
   or you will get nondeterministic hashes from guest memory garbage.

3. **Build `RenderInputElement[]` once at VDECL creation, not per draw** —
   storing it on the decl lets the pipeline-hash path use a stable pointer.
   Include a phantom slot-15 dummy for every shader-expected semantic missing
   from the guest decl (video.cpp:4984-4994), otherwise pipeline validation
   fails on unbound inputs.

4. **Keep stride in three synchronized places**: `PipelineState.vertexStrides[i]`
   (for hash), `g_inputSlots[i].stride` (for desc), `g_vertexBufferViews[i].size`
   (for binding). `SanitizePipelineState` must zero strides for slots the decl
   doesn't declare (video.cpp:4043), otherwise pipeline-cache key drifts from
   identical logical state.

5. **DrawPrimitiveUP shares the same VDECL binding** — do not try to synthesize
   a decl from the UP stride. Unleashed only overrides slot-0 stride/view;
   every other input and the shader binding still flow through whatever
   `SetVertexDeclaration` bound last. The correct fix for the Liberty crash
   is hook coverage, not UP-path special-casing.
