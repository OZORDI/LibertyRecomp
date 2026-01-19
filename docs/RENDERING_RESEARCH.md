# GTA IV Renderer - Complete Technical Reference

> **Purpose:** Comprehensive documentation of all rendering subsystems derived from recompiled PPC code analysis. This document serves as the authoritative reference for implementing host-side rendering.

---

## Table of Contents

1. [Architecture Overview](#1-architecture-overview)
2. [Device Context Structure](#2-device-context-structure)
3. [PM4 Command Buffer System](#3-pm4-command-buffer-system)
4. [D3D Wrapper Functions](#4-d3d-wrapper-functions)
5. [Shader System](#5-shader-system)
6. [Draw Call Implementation](#6-draw-call-implementation)
7. [Render Pipeline Flow](#7-render-pipeline-flow)
8. [Host Implementation Guide](#8-host-implementation-guide)

---

## 1. Architecture Overview

### 1.1 Rendering Layer Stack

```
┌─────────────────────────────────────────────────────────────┐
│ Layer 4: RAGE Engine (Game Logic)                           │
│   sub_828529B0, sub_828507F8, sub_82856F08                 │
├─────────────────────────────────────────────────────────────┤
│ Layer 3: RAGE D3D Wrapper (State Management)                │
│   sub_829D8860, sub_829D4EE0, sub_829CD350, sub_829D6690   │
├─────────────────────────────────────────────────────────────┤
│ Layer 2: PM4 Command Buffer (GPU Commands)                  │
│   sub_829D7E58, sub_829D8568, sub_829D7740                 │
├─────────────────────────────────────────────────────────────┤
│ Layer 1: Xenos GPU Hardware (Xbox 360)                      │
│   Ring buffer submission, VdSwap                            │
└─────────────────────────────────────────────────────────────┘
```

### 1.2 Recompilation Strategy

**Key Insight:** The game builds PM4 command packets for the Xenos GPU. These packets are Xbox 360-specific and must be bypassed.

**Approach:**
1. Hook Layer 3 (D3D Wrapper) functions
2. Extract render state from device context structure
3. Issue native GPU commands (Vulkan/D3D12)
4. Stub Layer 2 (PM4) functions to no-op

### 1.3 Key Global Addresses

| Address | Description |
|---------|-------------|
| `0x83042DEC` | Render context pointer |
| `0x828D2E38+19188` | Device pointer global |
| `0x830E5900` | Shader table (128 slots × 4 bytes) |
| `0x830E6630` | Shader linked list head |
| `0xA824E9C0` | GPU ring buffer base |

---

## 2. Device Context Structure

The D3D device context is a **~19KB structure** in guest memory containing all render state.

### 2.1 Command Buffer Management

```
Offset    Size    Field                   Description
------    ----    -----                   -----------
+48       4       commandBufferPtr        Current write position in PM4 buffer
+56       4       commandBufferLimit      End of PM4 buffer (flush trigger)
```

**Observed Behavior:** When `device[48] > device[56]`, the function `sub_829D8568` is called to flush the buffer.

### 2.2 Render State Flags

```
Offset    Size    Field                   Description
------    ----    -----                   -----------
+16       8       dirtyFlags              64-bit dirty state bitmask
+24       4       renderTargetDirty       RT change flags
+10908    4       deferredFlag            Deferred rendering mode
+10912    4       dirtyMask               State dirty mask
+10940    1       shaderFlags             Shader state (bits 3,4,5,6,7)
+10941    1       drawFlags               Draw state (bit 4 = pending)
+10943    1       commandFlags            Command buffer (bit 3 = active)
```

**PowerPC Origin:** These flags are loaded via `lbz` (load byte) and tested with `rlwinm` (rotate left word immediate then AND with mask) for bit extraction.

### 2.3 Shader State

```
Offset    Size    Field                   Description
------    ----    -----                   -----------
+10900    4       indexBufferBase         Base address for indexed draws
+10932    4       currentVertexShader     Active VS handle (0=none, -1=invalidate)
+10936    4       currentPixelShader      Active PS handle (0=none)
+12700    4       vertexShaderCache       Cached VS handle
+12704    4       pixelShaderCache        Cached PS handle
+12708    4       shaderValidFlag         1 = valid shader bound
+12720    20      shaderSlots[5]          Shader constant buffer slots
+12432    20      shaderParams[5]         Shader parameter references
```

### 2.4 Vertex Input State

```
Offset    Size    Field                   Description
------    ----    -----                   -----------
+10372    4       vertexFormatFlags       Vertex format (bits 12-15)
+10456    4       vertexDeclaration       Vertex declaration pointer
+12020    4       streamSource0           Vertex buffer slot 0
+12024    4       streamSource1           Vertex buffer slot 1
+12028    4       streamSource2           Vertex buffer slot 2
+12032    4       streamSource3           Vertex buffer slot 3
+13580    4       indexBuffer             Index buffer pointer
```

### 2.5 Render Target State

```
Offset    Size    Field                   Description
------    ----    -----                   -----------
+1780     32      renderTargetArray[4]    Packed RT params (8 bytes each)
+12428    4       depthStencilTarget      Depth/stencil surface
+12452    16      renderTargetSlots[4]    RT surface pointers
```

### 2.6 Texture State

```
Offset    Size    Field                   Description
------    ----    -----                   -----------
+12536    76      textureSlots[19]        Texture pointers (slots 0-18)
+1152     456     samplerDescs[19]        Sampler descriptors (24 bytes each)
```

### 2.7 Frame/Present State

```
Offset    Size    Field                   Description
------    ----    -----                   -----------
+13500    4       gpuStatePtr             GPU state reference
+13508    4       presentCmdPtr           Present command buffer
+13512    4       presentCmdLimit         Present buffer limit
+13592    4       displayWidthPacked      Display width >> 17
+13596    4       displayHeightPacked     Display height info
+14888    4       gpuContextPtr           GPU context reference
+16544    4       frameCounter            Frames presented
+19480    4       frameBufferIndex        Current backbuffer
```

---

## 3. PM4 Command Buffer System

### 3.1 Architecture

The Xenos GPU uses PM4 (Packet Manager 4) command packets, inherited from ATI R500 architecture.

```
┌──────────────────────────────────────────────────────────────────┐
│                    PM4 COMMAND FLOW                               │
├──────────────────────────────────────────────────────────────────┤
│                                                                   │
│  D3D Wrapper Functions                                           │
│       │                                                           │
│       ▼                                                           │
│  ┌────────────────────────────────────────┐                      │
│  │ sub_829D7E58 (PM4 Packet Builder)      │                      │
│  │ Writes PM4 commands to guest buffer    │                      │
│  └────────────────┬───────────────────────┘                      │
│                   │                                               │
│                   ▼                                               │
│  ┌────────────────────────────────────────┐                      │
│  │ Guest Command Buffer (device[48])      │                      │
│  │ ~4KB buffer in guest memory            │                      │
│  └────────────────┬───────────────────────┘                      │
│                   │ When full (device[48] > device[56])          │
│                   ▼                                               │
│  ┌────────────────────────────────────────┐                      │
│  │ sub_829D8568 (Buffer Flush)            │ ◄─ BYPASS            │
│  │ Submits to GPU ring buffer             │                      │
│  └────────────────┬───────────────────────┘                      │
│                   │                                               │
│                   ▼                                               │
│  ┌────────────────────────────────────────┐                      │
│  │ GPU Ring Buffer (0xA824E9C0)           │ ◄─ BYPASS            │
│  │ Primary ring for GPU execution         │                      │
│  └────────────────────────────────────────┘                      │
│                                                                   │
└──────────────────────────────────────────────────────────────────┘
```

### 3.2 PM4 Packet Types Observed

| Opcode | Name | Description |
|--------|------|-------------|
| 0x05C8 | PM4_SET_CONTEXT_REG | Set shader context register |
| 0x0578 | PM4_WAIT_FOR_IDLE | Wait for GPU idle |
| 0x057C | PM4_DRAW_INDX | Draw indexed primitives |
| 0xC0003C00 | SET_CONSTANT (VS) | Vertex shader constant |
| 0xC0005400 | SET_CONSTANT (PS) | Pixel shader constant |
| 0xC0006000 | SET_SHADER | Bind shader program |
| 0x003F | PM4_INDIRECT_BUFFER | Execute indirect buffer |
| 0x0048 | PM4_ME_INIT | Microengine initialization |

### 3.3 sub_829D7E58 - PM4 Packet Builder

**Function Summary:**
Core function that builds PM4 command packets for all draw and state operations.

**Location:** `ppc_recomp.135.cpp:33462`

**Observed Behavior:**
```
sub_829D7E58(device, cmdPtr, flags, param1, param2)
    │
    ├── r11 = device, r9 = cmdPtr, r5 = flags
    │
    ├── Flag processing:
    │   ├── r8 = flags & 0x6 (bits 1-2)
    │   ├── r10 = (flags >> 8) & 0x3F (primitive count)
    │   └── If r8 == 0: flags |= 6
    │
    ├── MAIN DRAW PACKET (if flags & 0x2):
    │   ├── Set device[10943] |= 0x08
    │   ├── Write 0x5C8 (PM4_SET_CONTEXT_REG), 0x20000
    │   ├── Write 0x1057C (PM4_DRAW_INDX), param1, param2
    │   ├── Write 0x578 (PM4_WAIT_FOR_IDLE), primitiveCount
    │   ├── Write device[12708] | 0xC0003C00 (VS constant)
    │   ├── Write 19, GPU vertex addr, count, -1, 256
    │   └── Write device[12708] | 0xC0005400 (PS constant)
    │
    ├── CLEANUP PACKET (if flags & 0x4):
    │   ├── Clear device[10943] &= 0xF7
    │   ├── Write 0xC0003C01 (PM4_EVENT_WRITE)
    │   └── Write 0x0BAD0F0D (sentinel marker)
    │
    └── Return updated cmdPtr
```

**Likely Original Intent:**
This is a generic PM4 packet builder used by all draw and state change functions. The flag system allows callers to request different combinations of setup, draw, and cleanup packets.

**Decompiler Artifacts:**
- Register spilling visible in r30/r31 saves
- Branch lowering creates multiple label targets
- Strength reduction on bit operations

**Confidence Level:** HIGH

### 3.4 sub_829D8568 - Buffer Flush

**Function Summary:**
Flushes the accumulated PM4 command buffer to the GPU ring buffer.

**Location:** `ppc_recomp.135.cpp` (called 73 times)

**Observed Behavior:**
```
sub_829D8568(device)
    │
    ├── If device[10908] set: Return (deferred mode)
    │
    ├── Calculate buffer size:
    │   └── size = device[48] - buffer_start
    │
    ├── If size == 0: Return
    │
    ├── Build PM4_INDIRECT_BUFFER command
    ├── Submit to ring buffer
    ├── Reset device[48] to buffer start
    │
    └── Return new buffer position
```

**PowerPC Origin Notes:**
- Uses `cmplw` for unsigned comparison of buffer pointers
- `blr` for return with value in r3

**Confidence Level:** HIGH

---

## 4. D3D Wrapper Functions

### 4.1 Function Address Map

| Address | Function | D3D Equivalent | Priority |
|---------|----------|----------------|----------|
| `0x829D8860` | sub_829D8860 | DrawPrimitive | P1 |
| `0x829D4EE0` | sub_829D4EE0 | DrawIndexedPrimitive | P1 |
| `0x829D5388` | sub_829D5388 | Present | P1 |
| `0x829CD350` | sub_829CD350 | SetVertexShader | P2 |
| `0x829D6690` | sub_829D6690 | SetPixelShader | P2 |
| `0x829D3728` | sub_829D3728 | SetTexture | P2 |
| `0x829C9070` | sub_829C9070 | SetStreamSource(0) | P3 |
| `0x829C9120` | sub_829C9120 | SetStreamSource(1) | P3 |
| `0x829C91D0` | sub_829C91D0 | SetStreamSource(2) | P3 |
| `0x829C9280` | sub_829C9280 | SetStreamSource(3) | P3 |
| `0x829C96D0` | sub_829C96D0 | SetIndices | P3 |
| `0x829CA240` | sub_829CA240 | SetRenderTarget | P3 |
| `0x829CA360` | sub_829CA360 | SetDepthStencilSurface | P3 |
| `0x829D1310` | sub_829D1310 | SetViewport | P3 |
| `0x829D1058` | sub_829D1058 | SetScissorRect | P4 |
| `0x829C9440` | sub_829C9440 | SetVertexDeclaration | P4 |
| `0x829D14E0` | sub_829D14E0 | SetSamplerState | P4 |
| `0x829D3400` | sub_829D3400 | CreateTexture | P4 |

### 4.2 sub_829D8860 - DrawPrimitive

**Function Summary:**
Issues a non-indexed draw call using the current render state.

**Location:** `ppc_recomp.135.cpp:35060`

**Signature:** `DrawPrimitive(device, primitiveType, startVertex, primitiveCount)`

**Register Mapping (PPC ABI):**
- `r3` = device context → saved to r31
- `r4` = primitive type → saved to r30
- `r5` = start vertex → saved to r29
- `r6` = primitive count → saved to r28

**Observed Behavior:**
```
sub_829D8860(device, primType, startVtx, primCount)
    │
    ├── PROLOGUE:
    │   ├── __savegprlr_28 (save r28-r31, LR)
    │   └── Allocate 128-byte stack frame
    │
    ├── COMMAND BUFFER CHECK:
    │   ├── r4 = device[48] (current position)
    │   ├── r11 = device[56] (limit)
    │   └── If r4 > r11: sub_829D8568(device) // Flush
    │
    ├── BUILD DRAW COMMAND:
    │   ├── r7 = primCount, r6 = startVtx, r5 = primType
    │   └── sub_829D7E58(device, cmdPtr, primType, startVtx, primCount)
    │
    ├── UPDATE STATE:
    │   └── device[48] = r3 (new buffer position)
    │
    └── EPILOGUE: __restgprlr_28
```

**Primitive Type Encoding:**
| Value | Type | Description |
|-------|------|-------------|
| 1 | D3DPT_POINTLIST | Point list |
| 2 | D3DPT_LINELIST | Line list |
| 3 | D3DPT_LINESTRIP | Line strip |
| 4 | D3DPT_TRIANGLELIST | Triangle list |
| 5 | D3DPT_TRIANGLEFAN | Triangle fan |
| 6 | D3DPT_RECTLIST | Quad list (Xenos) |
| 8 | D3DPT_TRIANGLESTRIP | Triangle strip |
| 13 | D3DPT_QUADLIST | Quad primitives |

**PowerPC Origin Notes:**
- `mflr r12` saves link register for nested calls
- `cmplw cr6` unsigned compare for buffer bounds
- `ble cr6` conditional branch on compare result

**Confidence Level:** HIGH

### 4.3 sub_829D4EE0 - UnifiedDraw (DrawIndexedPrimitive)

**Function Summary:**
Unified draw function handling both indexed and non-indexed rendering paths.

**Location:** `ppc_recomp.135.cpp:26172`

**Signature:** `UnifiedDraw(device, isIndexed)`

**Register Mapping:**
- `r3` = device context
- `r4` = isIndexed flag (0 = non-indexed, non-zero = indexed)

**Observed Behavior:**
```
sub_829D4EE0(device, isIndexed)
    │
    ├── sub_829DC848(device, 1)     // Begin draw batch
    ├── sub_829CD350(device, 0)     // Flush shader state
    │
    ├── BUFFER CHECK:
    │   └── If device[48] > device[56]: sub_829D8568(device)
    │
    ├── NON-INDEXED PATH (isIndexed == 0):
    │   ├── Write PM4 header 1401
    │   ├── Calculate vertex base from device[10900]+4
    │   └── Write index type 0x100
    │
    ├── INDEX FORMAT DETECTION (device[13580]):
    │   ├── 0 or 1 → 16-bit indices
    │   ├── 2 → 32-bit little-endian
    │   ├── 4 → 32-bit big-endian
    │   └── 0x80000000 → GPU default
    │
    ├── BUILD DRAW PACKET:
    │   ├── r9 = device[11844] (blend state)
    │   ├── Encode: (indexFormat << 8) | blendBits | isIndexed
    │   └── sub_829D7E58(device, cmdPtr, 0, stateAddr, flags)
    │
    ├── sub_829DC848(device, 0)     // End draw batch
    │
    └── device[10941] |= 0x10       // Set draw pending flag
```

**Likely Original Intent:**
This function abstracts both DrawPrimitive and DrawIndexedPrimitive into a single unified interface, selecting behavior based on the isIndexed flag.

**Confidence Level:** HIGH

### 4.4 sub_829CD350 - SetVertexShader

**Function Summary:**
Binds a vertex shader to the device context, updating shader cache and issuing PM4 commands.

**Location:** `ppc_recomp.135.cpp:7123`

**Signature:** `SetVertexShader(device, shaderHandle)`

**Register Mapping:**
- `r3` = device context
- `r4` = shader handle (0 = clear, non-zero = bind)

**Observed Behavior:**
```
sub_829CD350(device, shaderHandle)
    │
    ├── CLEAR PATH (shaderHandle == 0):
    │   ├── device[12708] = 0 (clear valid flag)
    │   ├── Check device[10940] flags (bits 3,4,5,6)
    │   ├── Compare shader cache slots:
    │   │   └── device[12432-12448] vs device[12720-12736]
    │   └── device[12700] = -1 (invalidate)
    │
    ├── BIND PATH (shaderHandle != 0):
    │   ├── device[12700] = shaderHandle
    │   └── device[12708] = 1 (mark valid)
    │
    ├── UPDATE STATE:
    │   ├── device[10932] = shaderHandle (active VS)
    │   └── device[10936] = 0 (clear PS)
    │
    ├── BUFFER CHECK + FLUSH
    │
    └── WRITE PM4 COMMAND:
        ├── Write 0xC0006000 (SET_SHADER header)
        └── Write shaderHandle
```

**PowerPC Origin Notes:**
- `rlwinm` used extensively for bit field extraction
- `lbz` loads byte flags from device context
- Multiple conditional branches for flag checking

**Confidence Level:** HIGH

### 4.5 sub_829D6690 - SetPixelShader

**Function Summary:**
Binds a pixel shader to the device context. Structurally similar to SetVertexShader.

> **Note:** Despite the D3D-style name, this function sets **both** vertex and pixel shaders simultaneously, unlike separate D3D SetVertexShader/SetPixelShader calls.

**Location:** `ppc_recomp.135.cpp:29837`

**Signature:** `SetPixelShader(device, vsHandle, psHandle)`

**Register Mapping:**
- `r3` = device context (→ r31)
- `r4` = vertex shader handle
- `r5` = pixel shader handle

**Observed Behavior:**
```
sub_829D6690(device, vsHandle, psHandle)
    │
    ├── Update GPU state flags:
    │   ├── r11 = device[13500] (gpuStatePtr)
    │   └── r11[108] |= 64 (set dirty bit)
    │
    ├── CLEAR PATH (vsHandle == 0 && psHandle == 0):
    │   ├── device[12708] = 0
    │   ├── Check device[10940] flags
    │   ├── Compare shader cache slots
    │   ├── device[12704] = 0
    │   └── device[12700] = -1
    │
    ├── BIND PATH:
    │   ├── device[12700] = vsHandle
    │   ├── device[12704] = psHandle
    │   └── device[12708] = 1
    │
    ├── UPDATE STATE:
    │   ├── device[10932] = vsHandle
    │   └── device[10936] = psHandle
    │
    ├── BUFFER CHECK + FLUSH
    │
    └── WRITE PM4 COMMANDS:
        ├── Write 0xC0006000, vsHandle (VS)
        └── Write 0xC0006100, psHandle (PS)
```

**Likely Original Intent:**
Sets both vertex and pixel shaders simultaneously. The additional GPU state flag update at device[13500][108] suggests this triggers a full shader pipeline reconfiguration.

**Confidence Level:** HIGH

### 4.6 sub_829C9070 - SetStreamSource0

**Function Summary:**
Binds a vertex buffer to stream slot 0, updating format flags.

**Location:** `ppc_recomp.134.cpp:5172`

**Signature:** `SetStreamSource0(device, vertexBuffer)`

**Observed Behavior:**
```
sub_829C9070(device, vertexBuffer)
    │
    ├── r8 = device[12432] (current VB slot 0)
    ├── device[12020] = vertexBuffer
    │
    ├── If r8 == 0: Return
    │
    ├── FORMAT CHECK:
    │   ├── r10 = r8[28] (VB format descriptor)
    │   ├── format = (r10 >> 16) & 0xF
    │   ├── Valid: 2 (FLOAT2), 3 (FLOAT3), 10 (FLOAT4), 12 (UBYTE4)
    │   └── Others: Return
    │
    ├── FORMAT CHANGE CHECK:
    │   ├── swapState = (r10 >> 19) & 0x1
    │   └── If unchanged: Return
    │
    ├── UPDATE FORMAT:
    │   ├── r8[28] = new format
    │   ├── device[10372] = (device[10372] & 0xFFF0FFFF) | formatBits
    │   └── device[16] |= (1 << 56) // dirty flag
    │
    └── Return
```

**PowerPC Origin Notes:**
- Uses `rlwinm` for bit field extraction/insertion
- `rldicr` for 64-bit rotation (dirty flag)
- Early returns via `beqlr` and `bnelr`

**Confidence Level:** HIGH

### 4.7 sub_829C96D0 - SetIndices

**Function Summary:**
Binds an index buffer. Simplest D3D function - single store operation.

**Location:** `ppc_recomp.134.cpp:6319`

**Signature:** `SetIndices(device, indexBuffer)`

**Observed Behavior:**
```
sub_829C96D0(device, indexBuffer)
    │
    ├── device[13580] = indexBuffer
    │
    └── Return
```

**Companion - GetIndices (sub_829C96D8):**
```
sub_829C96D8(device)
    │
    └── Return device[13580]
```

**Confidence Level:** HIGH

### 4.8 sub_829D5388 - Present (VdSwap)

**Function Summary:**
Submits the completed frame to the display via VdSwap kernel call.

**Location:** `ppc_recomp.81.cpp:41226`

**Signature:** `Present(device, surface, flags)`

**Register Mapping:**
- `r3` = device (→ r31)
- `r4` = surface (→ r28)
- `r5` = flags (→ r26)

**Observed Behavior:**
```
sub_829D5388(device, surface, flags)
    │
    ├── device[16544]++ (frame counter)
    │
    ├── Copy surface params:
    │   └── sub_82990830(stack+128, surface+28, 24)
    │
    ├── FORMAT CONVERSION:
    │   ├── If format == 50 → 3
    │   └── If format == 7 → 27
    │
    ├── DISPLAY PARAMS:
    │   ├── width = device[13592] >> 17
    │   └── height from device[13596]
    │
    ├── BUILD SWAP DESCRIPTOR
    │
    ├── ═══════════════════════════
    │   VdSwap(swapDesc, ...)
    │   @ address 0x829D55D4
    │   ═══════════════════════════
    │
    ├── device[48] = r30 + 256
    │
    └── If flags != 0: sub_829DC778(device, buffer)
```

**Confidence Level:** HIGH

---

## 5. Shader System

### 5.1 Shader Table Layout

**Global Address:** `0x830E5900`

```
┌────────────────────────────────────────────────────────────┐
│ Shader Table (128 slots × 4 bytes = 512 bytes)             │
├────────────────────────────────────────────────────────────┤
│ Slot 0:  [shader_ptr]  ← gta_im shader                    │
│ Slot 1:  [shader_ptr]                                      │
│ Slot 2:  [shader_ptr]                                      │
│ ...                                                         │
│ Slot 127: [shader_ptr]                                     │
└────────────────────────────────────────────────────────────┘
```

**Linked List Head:** `0x830E6630`

**Guest Shader Heap:** `0x83100000+`

### 5.2 Shader Loading Flow

```
sub_82858758 (Shader Loader)
    │
    ├── Parse shader name (e.g., "embedded:/gta_im")
    ├── Allocate slot from shader table
    ├── Load shader bytecode:
    │   ├── Vertex shader → offset 0x270 in structure
    │   └── Pixel shader → offset 0x370 in structure
    ├── Add to linked list
    └── Return shader handle
```

### 5.3 Shader Handle Format

**Observed handles:**
- VS: `0x80000270` (heap base + VS offset)
- PS: `0x80000370` (heap base + PS offset)

**Host Implementation:** Hook `sub_82858758` to return pre-compiled native shaders.

---

## 6. Draw Call Implementation

### 6.1 State Extraction

For each draw call, extract from device context:

```cpp
struct DrawCallState {
    // Shaders
    uint32_t vertexShader = PPC_LOAD_U32(device + 10932);
    uint32_t pixelShader  = PPC_LOAD_U32(device + 10936);
    bool shaderValid      = PPC_LOAD_U32(device + 12708) != 0;
    
    // Vertex Buffers
    uint32_t vbSlot0 = PPC_LOAD_U32(device + 12020);
    uint32_t vbSlot1 = PPC_LOAD_U32(device + 12024);
    uint32_t vbSlot2 = PPC_LOAD_U32(device + 12028);
    uint32_t vbSlot3 = PPC_LOAD_U32(device + 12032);
    
    // Index Buffer
    uint32_t indexBuffer = PPC_LOAD_U32(device + 13580);
    
    // Blend State
    uint32_t blendState = PPC_LOAD_U32(device + 11844);
};
```

### 6.2 Index Format Detection

From `sub_829D4EE0`:

```cpp
uint32_t ibState = PPC_LOAD_U32(device + 13580);

IndexFormat format;
switch (ibState) {
    case 0: case 1: format = INDEX_16BIT; break;
    case 2: format = INDEX_32BIT_LE; break;
    case 4: format = INDEX_32BIT_BE; break;
    case 0x80000000: format = INDEX_GPU_DEFAULT; break;
    default: format = INDEX_FROM_STACK; break;
}
```

---

## 7. Render Pipeline Flow

### 7.1 Frame Execution

```
sub_82856F08 (Main Loop)
    │
    ├── mftb (read time base)
    ├── Calculate delta time
    │
    ▼
sub_828529B0 (Orchestrator)
    │
    ├── Input/Timing processing
    │
    ▼
[Render State Setup]
    ├── sub_829CA360 → device[12428] = depthStencil
    ├── sub_829CA240 → device[1780+idx*8] = RT
    ├── sub_829D1310 → device[12688] = viewport
    ├── sub_829D3728 → device[12536+slot*4] = texture
    ├── sub_829CD350 → device[10932] = VS
    ├── sub_829D6690 → device[10936] = PS
    ├── sub_829C9070 → device[12020] = VB
    └── sub_829C96D0 → device[13580] = IB

[Draw Calls]
    ├── sub_829D8860 (DrawPrimitive)
    └── sub_829D4EE0 (UnifiedDraw)
        ├── sub_829D7E58 ◄── BYPASS
        └── sub_829D8568 ◄── BYPASS

[Frame End]
    └── sub_829D5388 (Present)
        ├── device[16544]++
        └── VdSwap
```

---

## 8. Host Implementation Guide

### 8.1 Hook Priority

| Priority | Function | Action |
|----------|----------|--------|
| P1 | `sub_829D5388` | Video::Present() |
| P1 | `sub_829D8860` | Native DrawPrimitive |
| P1 | `sub_829D4EE0` | Native DrawIndexed |
| P2 | `sub_829CD350` | Bind VS from cache |
| P2 | `sub_829D6690` | Bind PS from cache |
| P2 | `sub_829D3728` | Bind texture |
| P3 | `sub_829C9070` | Bind VB slot 0 |
| P3 | `sub_829C96D0` | Bind IB |
| P4 | `sub_829D7E58` | STUB: return cmdPtr |
| P4 | `sub_829D8568` | STUB: no-op |

### 8.2 DrawPrimitive Hook

```cpp
GUEST_FUNCTION_HOOK(sub_829D8860, DrawPrimitive);

static void DrawPrimitive(PPCContext& ctx, uint8_t* base) {
    uint32_t device = ctx.r3.u32;
    uint32_t primType = ctx.r4.u32;
    uint32_t startVertex = ctx.r5.u32;
    uint32_t primCount = ctx.r6.u32;
    
    // Extract state from device context
    DrawCallState state;
    state.vertexShader = PPC_LOAD_U32(device + 10932);
    state.pixelShader = PPC_LOAD_U32(device + 10936);
    state.vbSlot0 = PPC_LOAD_U32(device + 12020);
    
    // Submit native draw command
    RenderCommand cmd;
    cmd.type = RenderCommandType::DrawPrimitive;
    cmd.primitiveType = ConvertPrimitiveType(primType);
    cmd.startVertex = startVertex;
    cmd.primitiveCount = primCount;
    g_renderQueue.enqueue(cmd);
    
    ctx.r3.u32 = device;
}
```

### 8.3 PM4 Bypass Stubs

```cpp
// Skip PM4 packet building
GUEST_FUNCTION_HOOK(sub_829D7E58, PM4PacketBuilder);
static void PM4PacketBuilder(PPCContext& ctx, uint8_t* base) {
    ctx.r3.u64 = ctx.r4.u64;  // Return cmdPtr unchanged
}

// Skip PM4 buffer flush
GUEST_FUNCTION_HOOK(sub_829D8568, PM4BufferFlush);
static void PM4BufferFlush(PPCContext& ctx, uint8_t* base) {
    ctx.r3.u32 = PPC_LOAD_U32(ctx.r3.u32 + 48);
}
```

---

## Appendix: Device Context Offsets

| Offset | Field | Used By |
|--------|-------|---------|
| +48 | commandBufferPtr | All draw |
| +56 | commandBufferLimit | All draw |
| +10932 | currentVertexShader | SetVS |
| +10936 | currentPixelShader | SetPS |
| +10940 | shaderFlags | SetVS/PS |
| +10941 | drawFlags | UnifiedDraw |
| +11844 | blendState | UnifiedDraw |
| +12020 | streamSource0 | SetStreamSource |
| +12700 | vertexShaderCache | SetVS |
| +12704 | pixelShaderCache | SetPS |
| +12708 | shaderValidFlag | SetVS/PS |
| +13580 | indexBuffer | SetIndices |
| +16544 | frameCounter | Present |

---

## Files Referenced

| File | Content |
|------|---------|
| `ppc_recomp.134.cpp` | SetStreamSource, SetIndices |
| `ppc_recomp.135.cpp` | Draw functions, shaders, PM4 |
| `ppc_recomp.81.cpp` | Present/VdSwap |
| `ppc_recomp.66.cpp` | Main loop |

---

## 9. Advanced Rendering Systems - Execution Traces

This section contains deep execution traces of the AO, reflection, lighting, water, material, and G-buffer systems discovered through PPC code analysis.

### 9.1 Function Address Range Map

| Address Range | Subsystem | Primary File | Description |
|---------------|-----------|--------------|-------------|
| `0x8284xxxx` | Materials/Shaders | `ppc_recomp.110.cpp` | Effect rendering, material setup |
| `0x8285xxxx` | Shaders/Effects | `ppc_recomp.110-111.cpp` | Shader loading, effect management |
| `0x8286xxxx` | Lighting/Scene | `ppc_recomp.111-112.cpp` | Light data, scene rendering |
| `0x8287xxxx` | Lighting Core | `ppc_recomp.112-113.cpp` | Light structures, shadow setup |
| `0x8288xxxx` | Lighting Advanced | `ppc_recomp.113-114.cpp` | Advanced lighting, AO-related |
| `0x828Exxxx` | Scene Effects | `ppc_recomp.120.cpp` | Post-processing, scene effects |
| `0x828Fxxxx` | Water System | `ppc_recomp.121.cpp` | Water rendering, reflections |
| `0x829CAxxxx` | Render Targets | `ppc_recomp.134.cpp` | RT management, G-buffer |

### 9.2 Render Target System (G-Buffer Analysis)

**Key Functions:**
- `sub_829CA240` - SetRenderTarget (main RT switching)
- `sub_829CA360` - SetDepthStencilSurface

#### sub_829CA240 - SetRenderTarget Execution Trace

```
sub_829CA240(device, index, surface, offset, unknown, flags, mipLevel)
    │
    ├── PROLOGUE:
    │   ├── r31 = device, r29 = index, r30 = surface
    │   ├── r26 = mipLevel, r6 = offset
    │   └── Stack frame: 144 bytes
    │
    ├── SURFACE PARAMETER EXTRACTION (if surface != 0):
    │   ├── r10 = surface[24] (base address)
    │   ├── r9 = surface[28] (size/pitch)
    │   ├── Calculate: r10 += offset
    │   ├── Calculate: r9 -= offset
    │   ├── Extract format bits: (r10 >> 12) & 0xFFF
    │   ├── Calculate RT slot: (17 - index) * 8
    │   └── Store to device[1780 + slot] = packed RT info
    │
    ├── RT SLOT UPDATE:
    │   ├── slot_offset = (index + 3113) * 4
    │   ├── r28 = device[slot_offset] (previous RT)
    │   │
    │   ├── If previous RT exists:
    │   │   ├── Check device[10908] (deferred mode)
    │   │   ├── If deferred: store to previous_rt[8]
    │   │   └── Else: check dirty mask device[10912]
    │   │
    │   └── If dirty mask matches:
    │       ├── Check command buffer: device[13508] vs device[13512]
    │       ├── If full: call sub_829D5B60 (flush)
    │       ├── Build PM4 packet with RT reference
    │       └── Update device[13508] (cmd ptr)
    │
    ├── STORE NEW RT:
    │   ├── device[slot_offset] = surface
    │   ├── device[12520 + index] = mipLevel & 0xFF
    │   │
    │   └── If mipLevel changed:
    │       ├── Load device[16] (64-bit dirty flags)
    │       ├── Set bit 19 (0x80000)
    │       └── Store updated flags
    │
    └── EPILOGUE: __restgprlr_26
```

**Device Context RT Offsets:**
| Offset | Field | Description |
|--------|-------|-------------|
| +1780 | `renderTargetParams[4]` | 8 bytes each, packed RT info |
| +10908 | `deferredMode` | Deferred rendering flag |
| +10912 | `dirtyMask` | RT dirty mask for PM4 |
| +12452+ | `renderTargetSlots[4]` | RT surface pointers |
| +12520+ | `renderTargetMips[4]` | Mip levels per RT |
| +13508 | `cmdBufferPtr` | Current command position |
| +13512 | `cmdBufferLimit` | Command buffer limit |

#### sub_829CA360 - SetDepthStencilSurface Execution Trace

```
sub_829CA360(device, depthSurface)
    │
    ├── PROLOGUE:
    │   ├── r31 = device, r29 = depthSurface
    │   └── Stack frame: 128 bytes
    │
    ├── PREVIOUS DEPTH CHECK:
    │   ├── r30 = device[12428] (current depth surface)
    │   │
    │   ├── If r30 != 0:
    │   │   ├── Check device[10908] (deferred mode)
    │   │   ├── If deferred: r30[8] = device[10908]
    │   │   └── Else: check dirty mask
    │   │
    │   └── If dirty and not deferred:
    │       ├── Build PM4 packet
    │       └── Update command buffer
    │
    ├── STORE NEW DEPTH:
    │   └── device[12428] = depthSurface
    │
    └── EPILOGUE
```

**Key Finding:** GTA IV uses **forward rendering** with selective RT switching. There is no full G-buffer - depth is the primary auxiliary buffer used for post-processing.

### 9.3 Lighting System Execution Traces

**Key Functions in 0x8287xxxx-0x8288xxxx range:**

#### sub_82878748 - Light Structure Initialization

```
sub_82878748(lightStruct, sourceData)
    │
    ├── PROLOGUE:
    │   ├── r31 = lightStruct, r30 = sourceData
    │   └── Stack: 112 bytes
    │
    ├── INITIALIZE LIGHT:
    │   ├── Call sub_827E93F8 (matrix/vector init)
    │   ├── lightStruct[0] = vtable @ 0x830Exxxx + 31756
    │   ├── lightStruct[8] = 0 (type flags)
    │   ├── lightStruct[9] = 0 (active flag)
    │   ├── lightStruct[10] = 1 (count/ref)
    │   ├── lightStruct[12] = 0 (reserved)
    │   └── lightStruct[16] = 0 (reserved)
    │
    ├── COPY SOURCE DATA:
    │   ├── r3 = sourceData
    │   ├── Call sub_8221B7A0 (memory copy)
    │   └── lightStruct[20] = result
    │
    └── Return lightStruct
```

#### sub_82878FE8 - Light Parameter Update

```
sub_82878FE8(lightPtr)
    │
    ├── LOAD LIGHT DATA:
    │   ├── r11 = lightPtr[0] (vtable)
    │   ├── Extract light type from flags
    │   │
    │   ├── DIRECTIONAL LIGHT (type 0):
    │   │   ├── Load direction vector (12 bytes)
    │   │   └── Load color (12 bytes)
    │   │
    │   ├── POINT LIGHT (type 1):
    │   │   ├── Load position (12 bytes)
    │   │   ├── Load color (12 bytes)
    │   │   ├── Load radius/falloff
    │   │   └── Load intensity
    │   │
    │   ├── SPOT LIGHT (type 2):
    │   │   ├── Load position (12 bytes)
    │   │   ├── Load direction (12 bytes)
    │   │   ├── Load color (12 bytes)
    │   │   ├── Load inner/outer cone angles
    │   │   └── Load falloff params
    │   │
    │   └── AMBIENT (type 3):
    │       └── Load ambient color (12 bytes)
    │
    └── Return
```

**Estimated Light Structure (24+ bytes per light):**
```
Offset  Size  Field
+0      4     vtable pointer
+4      4     flags (type in bits 0-3)
+8      1     type (0=dir, 1=point, 2=spot, 3=ambient)
+9      1     active
+10     2     refCount
+12     4     reserved
+16     4     paramOffset
+20     4     dataPtr
```

### 9.4 Water System Execution Traces

**Key Function: sub_828F22D0** - Water Surface Loader

```
sub_828F22D0(waterName)
    │
    ├── PROLOGUE:
    │   ├── r27 = waterName (256-char path)
    │   ├── Stack frame: 2032 bytes (large for texture data)
    │   └── Initialize r31=0, r25=0, r30=stack+128
    │
    ├── COPY NAME TO STACK:
    │   ├── memcpy(stack+128, waterName, 256)
    │   └── Call sub_8266A778
    │
    ├── PARSE WATER TYPE:
    │   ├── Call sub_8298FEB0 with type=47 (water identifier)
    │   ├── r26 = 0x82040000 - 504 (water manager ptr)
    │   │
    │   ├── If parse failed (r3 == 0):
    │   │   ├── Copy to extended buffer (stack+1712)
    │   │   ├── Call sub_827E0B78, sub_827E0740
    │   │   ├── Try alternate parse sub_827DFC70
    │   │   └── r30 = result
    │   │
    │   └── r28 = parsed water handle
    │
    ├── LOAD WATER RESOURCE:
    │   ├── r29 = water callback table @ 0x81FFC9F8
    │   ├── Call sub_827E0898(waterMgr, path, callbacks, 1, 1)
    │   ├── r28 = loaded resource handle
    │   │
    │   └── If load failed, try alternate paths:
    │       ├── Append ".wtr" suffix
    │       ├── Try "_pc.wtr" suffix
    │       ├── Try "_ps3.wtr" suffix
    │       └── Retry load
    │
    ├── WATER TYPE DISPATCH (string matching):
    │   ├── Compare with "water" → sub_828EDEF0 (basic water)
    │   ├── Compare with "river" → sub_828ECAD8 (river)
    │   ├── Compare with "ocean" → sub_828ECAD8 (ocean)
    │   ├── Compare with "fountain" → sub_8292F838 (fountain)
    │   ├── Compare with "pool" → sub_8291E558 (pool)
    │   ├── Compare with "swamp" → sub_828F8CC8 (swamp)
    │   ├── Compare with "rain" → sub_8290CEB0 (rain puddles)
    │   └── Compare with "wave" → sub_8294E2C0 (waves)
    │
    ├── ALLOCATE WATER OBJECT:
    │   ├── Size varies by type: 160-576 bytes
    │   ├── Call sub_8218BE28 (allocator)
    │   └── Call type-specific constructor
    │
    ├── INITIALIZE WATER:
    │   ├── Call sub_826F26B8 (bind to scene)
    │   ├── If success: add to water list
    │   └── If fail: cleanup and return 0
    │
    └── Return water object or 0
```

**Water Type Sizes:**
| Type | Size | Constructor |
|------|------|-------------|
| Basic Water | 160 | sub_828EDEF0 |
| River/Ocean | 224 | sub_828ECAD8 |
| Fountain | 272 | sub_8292F838 |
| Pool | 576 | sub_8291E558 |
| Swamp | 224 | sub_828F8CC8 |
| Rain Puddles | 240 | sub_8290CEB0 |
| Waves | 240 | sub_8294E2C0 |

### 9.5 Material System Execution Traces

**Key Function: sub_8284C200** - Material Effect Rendering

```
sub_8284C200(materialCtx)
    │
    ├── PROLOGUE:
    │   ├── r31 = materialCtx
    │   ├── Save vector registers v125-v127
    │   └── Stack: 19264 bytes (large for temp buffers)
    │
    ├── MATERIAL SETUP:
    │   ├── r11 = materialCtx + 80 (effect params)
    │   ├── r8 = materialCtx[637] & 0x7F (material flags)
    │   ├── r10 = materialCtx[12] (shader reference)
    │   ├── f0 = 0.0f (default value)
    │   ├── Store f0 to materialCtx[324]
    │   │
    │   ├── Load vectors:
    │   │   ├── v127 = materialCtx[32] (diffuse color)
    │   │   └── v126 = materialCtx[64] (specular/normal)
    │   │
    │   └── Calculate lighting contribution:
    │       ├── f12 = r10[52] (light intensity)
    │       ├── f13 = r11[4] * f12 (modulated)
    │       ├── f11 = r10[56] (falloff)
    │       ├── f10 = r11[0] (base)
    │       ├── f9 = r10[48] (ambient)
    │       ├── f13 = f12 * f11 + f13 (accumulate)
    │       ├── f13 = -(f10 * f9 + f13) (final)
    │       └── Store f13 to materialCtx[408]
    │
    ├── VECTOR MATH SETUP:
    │   ├── v13 = load direction vector
    │   ├── v8 = splat(0.0f)
    │   ├── v10, v9 = load constants
    │   ├── v7 = load material scale
    │   ├── v0 = vmsum3fp(v13, v13) (dot product)
    │   ├── v0 = vrsqrtefp(v0) (inverse sqrt)
    │   ├── Normalize: v0 = v13 * v0
    │   └── Transform: v0 = v0 * v7 + v12
    │
    ├── RENDER SETUP:
    │   ├── Call sub_82323788 (frustum setup)
    │   ├── Call sub_828D7220 (render state)
    │   ├── Call sub_82811E28 (texture bind)
    │   │
    │   └── If texture valid:
    │       ├── Call sub_82323888 (UV setup)
    │       ├── Update v126, v127 from result
    │       └── Store to materialCtx[152]
    │
    ├── EFFECT RENDER:
    │   ├── Call sub_828112F8 (draw material)
    │   └── Cleanup temporary buffers
    │
    └── EPILOGUE: Restore vectors, return
```

**Material Context Structure (estimated):**
```
Offset  Size  Field
+0      4     vtable
+12     4     shaderRef
+32     16    diffuseColor (vector)
+48     16    reserved
+64     16    specularParams (vector)
+80     16    effectParams
+152    4     textureHandle
+324    4     lightScale
+408    4     finalLightContrib
+637    1     materialFlags
```

### 9.6 Scene Effect System (0x828Exxxx)

**Key Functions:**
- `sub_828E0AB8` - Effect destructor/cleanup
- `sub_828EDEF0` - Basic water effect init
- `sub_828ECAD8` - River/ocean effect init

These functions in `ppc_recomp.120.cpp` handle:
1. Post-processing effect initialization
2. Water surface effect setup
3. Reflection texture management
4. Screen-space effect rendering

### 9.7 Ambient Occlusion Discovery

**Key Finding:** GTA IV Xbox 360 does **NOT** have native SSAO. The lighting system uses:
1. Baked ambient occlusion in lightmaps
2. Per-vertex AO stored in vertex color alpha
3. Contact shadows for small-scale occlusion

**Evidence from traces:**
- No depth-based AO compute in lighting functions
- Material flags include AO multiplier (materialCtx[637])
- Light contributions pre-modulated with baked AO

**Host-Side SSAO Implementation:** Must be added as post-process using:
1. Captured depth buffer from `sub_829CA360`
2. Reconstructed normals from depth
3. GTAO algorithm (see `gpu/shader/hlsl/ssao_gtao_ps.hlsl`)

### 9.8 Reflection System Discovery

**Water Reflections:** Handled in water type constructors with:
1. Planar reflection for still water (pools)
2. Cubemap reflections for ocean/rivers
3. SSR-like approximation for rain puddles

**Material Reflections:** Environment mapping via:
1. Cubemap lookup in material shaders
2. Fresnel-based blending
3. Roughness from material properties

**Host-Side SSR Implementation:** Must use:
1. Previous frame color buffer
2. Depth buffer
3. Normal reconstruction
4. Ray marching (see `gpu/shader/hlsl/ssr_raytrace_ps.hlsl`)

### 9.9 Deferred Rendering Analysis

**Key Finding:** GTA IV uses **forward rendering** with deferred elements:

1. **Forward Base Pass:**
   - All geometry rendered with full lighting
   - Single RT (color) + depth buffer
   - No MRT G-buffer

2. **Deferred Elements:**
   - Shadow map rendering (separate pass)
   - Reflection probes (pre-rendered cubemaps)
   - Light accumulation for many lights

3. **Device Context Flags:**
   - `device[10908]` - Deferred mode flag
   - When set, RT changes are queued not applied
   - Flush occurs at pass boundaries

**Implications for Host Rendering:**
- Can intercept forward pass for depth capture
- Post-processing (SSAO, SSR) applied after forward pass
- No G-buffer reconstruction needed - use depth only

---

## 10. Implementation Roadmap for SSAO/SSR

### 10.1 Phase 1: Depth Buffer Capture

```cpp
// Hook sub_829CA360 to capture depth surface
static void HookSetDepthStencil(PPCContext& ctx, uint8_t* base) {
    uint32_t device = ctx.r3.u32;
    uint32_t depthSurface = ctx.r4.u32;
    
    // Extract depth format from surface
    if (depthSurface != 0) {
        uint32_t format = PPC_LOAD_U32(depthSurface + 24);
        // Capture for post-processing
        g_capturedDepth = ConvertGuestDepth(depthSurface);
    }
    
    // Call original
    sub_829CA360(ctx, base);
}
```

### 10.2 Phase 2: Camera Data Extraction

```cpp
// Extract from known guest addresses
struct CameraData {
    float nearClip;    // 0x81802648
    float farClip;     // 0x82053CD8
    float fov;         // 0x8180D8EC
    float aspect;      // 0x8180F4D8
    float4x4 view;     // device[10932] shader constant
    float4x4 proj;     // device[10936] shader constant
};
```

### 10.3 Phase 3: SSAO Integration

1. After main forward pass (before UI)
2. Bind captured depth
3. Dispatch GTAO compute
4. Blend result with color buffer

### 10.4 Phase 4: SSR Integration

1. After SSAO (needs AO-modulated color)
2. Bind color, depth, construct normals
3. Dispatch ray march
4. Composite reflections

---

## 11. Refined Deep Traces - Shader & Device Context Analysis

### 11.1 Device Context Complete Offset Map

Based on deep analysis of `sub_829CD350` (SetVertexShader), `sub_829CD4C0` (SetShaderConstants), and related functions:

```
Device Context Structure (~22000 bytes)
═══════════════════════════════════════════════════════════════════

SECTION: Core State (0-1000)
┌─────────┬──────┬────────────────────────────────────────────────┐
│ Offset  │ Size │ Field                                          │
├─────────┼──────┼────────────────────────────────────────────────┤
│ +16     │ 8    │ dirtyFlags64 (64-bit state dirty mask)         │
│ +24     │ 8    │ renderTargetDirty64                            │
│ +40     │ 8    │ shaderDirty64                                  │
│ +48     │ 4    │ cmdBufferCurrent                               │
│ +56     │ 4    │ cmdBufferLimit                                 │
└─────────┴──────┴────────────────────────────────────────────────┘

SECTION: Render State (10368-10944)
┌─────────┬──────┬────────────────────────────────────────────────┐
│ Offset  │ Size │ Field                                          │
├─────────┼──────┼────────────────────────────────────────────────┤
│ +10368  │ 4    │ vertexShaderParams (bits 0-1: format type)     │
│ +10908  │ 4    │ deferredRenderMode                             │
│ +10912  │ 4    │ dirtyMask                                      │
│ +10932  │ 4    │ currentVertexShader                            │
│ +10936  │ 4    │ currentPixelShader                             │
│ +10940  │ 1    │ shaderStateFlags                               │
│         │      │   bit 0: shader bound                          │
│         │      │   bit 2: vertex shader dirty                   │
│         │      │   bit 3: pixel shader dirty                    │
│         │      │   bit 4: force rebind                          │
│         │      │   bit 5: batch mode                            │
│         │      │   bit 6: deferred mode                         │
│ +10943  │ 1    │ constantFlags                                  │
│         │      │   bit 4: pixel constants dirty                 │
│         │      │   bit 5: vertex constants dirty                │
└─────────┴──────┴────────────────────────────────────────────────┘

SECTION: Vertex Buffers (11844-12180)
┌─────────┬──────┬────────────────────────────────────────────────┐
│ Offset  │ Size │ Field                                          │
├─────────┼──────┼────────────────────────────────────────────────┤
│ +11844  │ 4    │ vertexDeclaration (bits 12-14: format)         │
│ +12179  │ 1    │ vertexBufferDirty                              │
└─────────┴──────┴────────────────────────────────────────────────┘

SECTION: Render Targets (12428-12540)
┌─────────┬──────┬────────────────────────────────────────────────┐
│ Offset  │ Size │ Field                                          │
├─────────┼──────┼────────────────────────────────────────────────┤
│ +12428  │ 4    │ depthStencilSurface                            │
│ +12432  │ 20   │ streamSources[5] (4 bytes each)                │
│ +12452  │ 68   │ renderTargetSlots[17] (4 bytes each)           │
│ +12520  │ 17   │ renderTargetMips[17] (1 byte each)             │
└─────────┴──────┴────────────────────────────────────────────────┘

SECTION: Shader Constants (12640-13228)
┌─────────┬──────┬────────────────────────────────────────────────┐
│ Offset  │ Size │ Field                                          │
├─────────┼──────┼────────────────────────────────────────────────┤
│ +12640  │ 28   │ viewportRect                                   │
│ +12668  │ 16   │ scissorRect                                    │
│ +12700  │ 4    │ activeShaderHandle (-1 = none)                 │
│ +12704  │ 4    │ shaderType                                     │
│ +12708  │ 4    │ shaderBoundFlag                                │
│ +12720  │ 20   │ prevStreamSources[5]                           │
│ +12740  │ 4    │ constantCount                                  │
│ +12744  │ 4    │ constantBaseRegister                           │
│ +12748  │ 240  │ constantBuffer[15] (16 bytes per constant)     │
│ +12984  │ 8    │ constantPackedData (per-constant)              │
│ +12988  │ 116  │ constantMasks[~15] (8 bytes each)              │
│ +13104  │ 60   │ computedConstantIndices[15]                    │
│ +13164  │ 4    │ totalConstantSize                              │
│ +13168  │ 4    │ alignedConstantSize                            │
│ +13172  │ 4    │ maxVSConstantRegister                          │
│ +13176  │ 4    │ maxPSConstantRegister                          │
│ +13180  │ 4    │ constantSetFlags                               │
│ +13352  │ 156  │ instanceData                                   │
│ +13508  │ 4    │ cmdWritePtr                                    │
│ +13512  │ 4    │ cmdBufferEnd                                   │
└─────────┴──────┴────────────────────────────────────────────────┘
```

### 11.2 SetVertexShader (sub_829CD350) Detailed Trace

```
sub_829CD350(device, shaderHandle)
    │
    ├── ARGUMENTS:
    │   ├── r3 = device context
    │   └── r4 = shader handle (0 = clear, non-0 = bind)
    │
    ├── CASE: shaderHandle == 0 (Clear shader):
    │   ├── device[12708] = 0 (clear bound flag)
    │   ├── Check device[10940] shader state flags:
    │   │   ├── If bit 3 (0x08) set: skip validation
    │   │   ├── If bit 2 (0x04) set: skip validation
    │   │   └── If device[12179] != 0: skip validation
    │   │
    │   ├── VALIDATION (if bit 4 (0x10) set):
    │   │   └── Set r11 = 1 (force rebind)
    │   │
    │   ├── BATCH MODE CHECK (bit 5 (0x20) set):
    │   │   ├── Compare device[12432..12448] with device[12720..12736]
    │   │   │   (current vs previous stream sources)
    │   │   └── If all match or null: r11 = 1, else r11 = 0
    │   │
    │   ├── Combine flags: r11 |= (device[10940] & 0xFE)
    │   └── device[12700] = -1 (invalid handle)
    │
    ├── CASE: shaderHandle != 0 (Bind shader):
    │   ├── device[12700] = shaderHandle
    │   ├── device[12708] = 1 (set bound flag)
    │   └── device[10940] &= 0xFE (clear bit 0)
    │
    ├── FINALIZE:
    │   ├── device[10940] = r11 (updated flags)
    │   ├── device[12704] = 0 (reset type)
    │   ├── device[10932] = shaderHandle (current VS)
    │   ├── device[10936] = 0 (clear PS)
    │   │
    │   ├── Flush check: device[48] > device[56]?
    │   │   └── If yes: call sub_829D8568 (Buffer Flush)
    │   │
    │   └── BUILD PM4 PACKET:
    │       ├── Write 0xC0006000 (PM4 header: SET_CONSTANT)
    │       ├── Write shaderHandle
    │       └── Update device[48] (cmd ptr)
    │
    └── Return
```

### 11.3 SetShaderConstants (sub_829CD4C0) Detailed Trace

```
sub_829CD4C0(device, flags, count, singleValue, dataPtr, startReg, extraFlags)
    │
    ├── ARGUMENTS:
    │   ├── r3 = device context
    │   ├── r4 = flags
    │   │   ├── bit 0 (0x01): enable/apply constants
    │   │   ├── bit 1 (0x02): vertex shader constants
    │   │   └── bit 2 (0x04): pixel shader constants
    │   ├── r5 = constant count
    │   ├── f1 = single float value (for scalar sets)
    │   ├── r6 = constant data pointer
    │   ├── r7 = start register
    │   └── r9 = extra flags
    │
    ├── INITIALIZE:
    │   ├── device[12740] = count
    │   ├── maxVS = 0, maxPS = 0
    │   │
    │   └── If count > 0:
    │       ├── Loop over each constant (16 bytes each):
    │       │   ├── Read from dataPtr+0: startRegVS
    │       │   ├── Read from dataPtr+4: endRegVS
    │       │   ├── Read from dataPtr+8: startRegPS
    │       │   ├── Read from dataPtr+12: endRegPS
    │       │   │
    │       │   ├── Track max: maxVS = max(maxVS, endRegVS)
    │       │   ├── Track max: maxPS = max(maxPS, endRegPS)
    │       │   │
    │       │   ├── Store to device[12748+i*16]: constant data
    │       │   ├── Store to device[12984+i*8]: packed (masked)
    │       │   └── dataPtr += 16
    │       │
    │       └── End loop
    │
    ├── VERTEX SHADER CONSTANTS (flags & 0x02):
    │   ├── device[10943] |= 0x20 (mark VS constants dirty)
    │   │
    │   ├── If (flags & 0x04): also set 0x10 (PS dirty)
    │   │
    │   ├── Calculate alignment based on device[10368] format:
    │   │   ├── Format 0: alignment=16, stride=32, pitch=80
    │   │   ├── Format 1: alignment=8, stride=32, pitch=80
    │   │   └── Format 2+: alignment=16, stride=16, pitch=40
    │   │
    │   ├── totalSize = align(stride + maxVS, alignment)
    │   ├── device[13164] = totalSize
    │   │
    │   ├── Compute indices for each constant:
    │   │   ├── Read device[12984+i*8]: packed offset
    │   │   ├── Compute linear index
    │   │   └── Store to device[13104+i*4]
    │   │
    │   ├── alignedSize = ((pitch + maxVS - 1) / pitch) * pitch
    │   ├── device[13168] = alignedSize
    │   │
    │   ├── Update device[16] |= 256 (dirty flag)
    │   │
    │   └── Check deferred mode (device[10943] & 0x20):
    │       ├── If not deferred AND format matches:
    │       │   └── device[40] &= ~256 (clear dirty)
    │       └── Else: device[40] |= 256 (set dirty)
    │
    ├── APPLY CONSTANTS (flags & 0x01):
    │   ├── device[13172] = maxVS
    │   ├── device[13176] = maxPS
    │   ├── device[13180] = flags
    │   │
    │   ├── If NOT deferred mode:
    │   │   ├── Calculate constant deltas
    │   │   ├── Build viewport rect at device[12640]
    │   │   ├── Call sub_829CA9C8 (SetViewport)
    │   │   ├── Call sub_829CA0F0 (SetScissor)
    │   │   └── Continue to pixel shader setup
    │   │
    │   └── Else: queue for deferred application
    │
    └── Return
```

### 11.4 Light Parameter Type System

From deep analysis of `sub_82878FE8`, the lighting system uses string-matched parameter types:

```
LIGHT PARAMETER TYPES
═════════════════════════════════════════════════════════════

Type 1: Boolean/Enable
  ├── String: "enable" or "active"
  ├── Size: 4 bytes (stored as int)
  └── Store: result[20] = value

Type 2: Single Float  
  ├── String: "intensity" or "range"
  ├── Size: 4 bytes
  └── Store: stfs f1, 0(r30)

Type 3: Two Floats
  ├── String: "innerAngle" "outerAngle" (spot light)
  ├── Size: 8 bytes
  └── Store: stfs f1, 0(r30); stfs f1, 4(r30)

Type 4: Three Floats (Vector3)
  ├── String: "color" "direction" "position"
  ├── Size: 12 bytes
  └── Store: stfs f1, 0(r30); stfs f1, 4(r30); stfs f1, 8(r30)

Type 5: Four Floats (Vector4)
  ├── String: "colorAlpha" "positionW"
  ├── Size: 16 bytes
  └── Store: +0, +4, +8, +12

Type 6: Shader Name (128 bytes)
  ├── String: "shader"
  ├── Calls: vtable[8] to get shader string
  ├── Then: string compare to resolve shader ID
  └── Store: shader handle to result[0]

Type 7: Integer
  ├── String: "type" "priority"
  ├── Calls: vtable[20] to get int
  └── Store: stw r3, 0(r30)

Type 8: Matrix4x4 (64 bytes, row-padded)
  ├── String: "worldMatrix" "viewMatrix"
  ├── Allocates: 64 bytes via sub_8218BE28
  ├── Calls: vtable[40] four times for rows
  └── Store: 4x float4 at offsets 0, 16, 32, 48

Type 9: Matrix4x4 (alternate binding)
  ├── String: "projMatrix" "invViewMatrix"
  ├── Same as Type 8 but uses vtable[36]
  └── Store: identical layout
```

### 11.5 Water System Structure (~18620 bytes)

From `sub_828F2AC0` (Water initialization) and `sub_828F22D0` (Water loader):

```
WATER OBJECT STRUCTURE
═══════════════════════════════════════════════════════════════

Base Class Fields:
┌─────────┬──────┬────────────────────────────────────────────────┐
│ Offset  │ Size │ Field                                          │
├─────────┼──────┼────────────────────────────────────────────────┤
│ +0      │ 4    │ vtable pointer                                 │
│ +4      │ 4    │ flags                                          │
│ +8      │ 1    │ active                                         │
│ +16     │ 192  │ baseWaterParams (varies by type)               │
└─────────┴──────┴────────────────────────────────────────────────┘

Extended Fields (offset 18448+):
┌─────────┬──────┬────────────────────────────────────────────────┐
│ Offset  │ Size │ Field                                          │
├─────────┼──────┼────────────────────────────────────────────────┤
│ +18448  │ 4    │ waveAmplitude                                  │
│ +18452  │ 4    │ waveFrequency                                  │
│ +18464  │ 64   │ waterTransformMatrix (3x3 + padding)           │
│         │      │   [1,0,0,0] [0,1,0,0] [0,0,1,0] [0,0,0,0]      │
│ +18528  │ 64   │ reflectionMatrix (3x3 + padding)               │
│         │      │   [1,0,0,0] [0,1,0,0] [0,0,1,0] [0,0,0,0]      │
│ +18592  │ 4    │ animTime                                       │
│ +18596  │ 4    │ animSpeed                                      │
│ +18600  │ 4    │ animPhase                                      │
│ +18604  │ 4    │ animScale                                      │
│ +18608  │ 4    │ animOffset                                     │
│ +18612  │ 4    │ animFlags                                      │
└─────────┴──────┴────────────────────────────────────────────────┘

Water Type Constructors (expanded):
┌─────────────────┬───────┬──────────────┬─────────────────────────┐
│ Type String     │ Size  │ Constructor  │ Features                │
├─────────────────┼───────┼──────────────┼─────────────────────────┤
│ "water"         │ 160   │ sub_828EDEF0 │ Basic, single plane     │
│ "river"         │ 224   │ sub_828ECAD8 │ Flow direction, speed   │
│ "ocean"         │ 224   │ sub_828ECAD8 │ Waves, foam, cubemap    │
│ "fountain"      │ 272   │ sub_8292F838 │ Spray particles         │
│ "pool"          │ 576   │ sub_8291E558 │ Planar reflection       │
│ "swamp"         │ 224   │ sub_828F8CC8 │ Murky, no reflection    │
│ "rain"          │ 240   │ sub_8290CEB0 │ Puddles, ripples        │
│ "wave"          │ 240   │ sub_8294E2C0 │ Breaking waves          │
│ "waterfall"     │ 176   │ sub_82946498 │ Vertical flow           │
│ "underwater"    │ 160   │ sub_828F3CE0 │ Caustics, distortion    │
│ "reflection"    │ 224   │ sub_8293FD48 │ Mirror surface          │
│ "mist"          │ 240   │ sub_8290FBC8 │ Fog volume              │
│ "steam"         │ 240   │ sub_8294A7D8 │ Particle emitter        │
│ "spray"         │ 240   │ sub_8294E2C0 │ Water spray particles   │
└─────────────────┴───────┴──────────────┴─────────────────────────┘
```

### 11.6 Lighting Structure (Refined)

From `sub_82870210` and related functions in `ppc_recomp.112.cpp`:

```
LIGHT DATA STRUCTURE (~192 bytes)
═══════════════════════════════════════════════════════════════

┌─────────┬──────┬────────────────────────────────────────────────┐
│ Offset  │ Size │ Field                                          │
├─────────┼──────┼────────────────────────────────────────────────┤
│ +0      │ 4    │ vtable pointer                                 │
│ +4      │ 4    │ stateFlags (0x80000 = dirty)                   │
│ +20     │ 2    │ lightType (0=dir, 1=point, 2=spot, 3=ambient)  │
│ +22     │ 2    │ shadowType                                     │
│ +24     │ 2    │ priority (-1 = auto)                           │
│ +26     │ 1    │ castShadows                                    │
│ +27     │ 1    │ isActive                                       │
│ +28     │ 1    │ isDynamic                                      │
│ +32     │ 16   │ color (float4: r, g, b, intensity)             │
│ +48     │ 16   │ position (float4: x, y, z, w)                  │
│ +64     │ 16   │ direction (float4: x, y, z, range)             │
│ +80     │ 16   │ spotParams (float4: inner, outer, falloff, 0)  │
│ +96     │ 64   │ reserved / shadow matrix                       │
│ +112    │ 16   │ attenuation (float4: const, linear, quad, 0)   │
│ +128    │ 16   │ shadowBias (float4: bias, slope, 0, 0)         │
└─────────┴──────┴────────────────────────────────────────────────┘

Light Type Enum:
  0 = Directional (sun)
  1 = Point (omni)
  2 = Spot (cone)
  3 = Ambient (global)
```

### 11.7 Host Shader Constant Extraction

Based on the analysis, here's how to extract shader constants for SSAO/SSR:

```cpp
// Extract camera matrices from device context
struct ShaderConstants {
    // From device[12640+]: viewport data
    float4 viewport;      // x, y, width, height
    float4 scissor;       // x, y, width, height
    
    // From shader constant buffer device[12748+]
    float4x4 worldMatrix;       // Registers c0-c3
    float4x4 viewMatrix;        // Registers c4-c7
    float4x4 projMatrix;        // Registers c8-c11
    float4x4 worldViewProj;     // Registers c12-c15
    
    // Lighting constants (typically c16+)
    float4 lightDir;            // c16
    float4 lightColor;          // c17
    float4 ambientColor;        // c18
    float4 cameraPos;           // c19
};

void ExtractShaderConstants(uint32_t device, ShaderConstants& out) {
    uint8_t* base = GetGuestMemory();
    
    // Viewport
    memcpy(&out.viewport, base + device + 12640, 16);
    memcpy(&out.scissor, base + device + 12668, 16);
    
    // Matrices from constant buffer (assuming standard layout)
    uint32_t cbBase = device + 12748;
    for (int i = 0; i < 16; i++) {
        // Each constant is 16 bytes, arranged in rows
        float4* row = (float4*)(base + cbBase + i * 16);
        // Assign to appropriate matrix...
    }
}
```

### 11.8 Existing Camera Extraction Implementation

**Already implemented in `camera_extract.cpp` and `camera_extract.h`:**

```
GUEST ADDRESS MAP (Discovered via Execution Trace)
═══════════════════════════════════════════════════════════════

Global Camera Parameters:
┌──────────────┬────────────────────────────────────────────────┐
│ Address      │ Field                                          │
├──────────────┼────────────────────────────────────────────────┤
│ 0x81802648   │ NearClipPlane (float, big-endian)              │
│ 0x82053CD8   │ FarClipPlane (float, big-endian)               │
│ 0x8180D8EC   │ FieldOfView (float, radians)                   │
│ 0x8180F4D8   │ AspectRatio (float)                            │
└──────────────┴────────────────────────────────────────────────┘

CCam Class Structure Offsets:
┌──────────────┬───────┬─────────────────────────────────────────┐
│ Offset       │ Size  │ Field                                   │
├──────────────┼───────┼─────────────────────────────────────────┤
│ +16          │ 48    │ ViewMatrix (3x4, row-major)             │
│ +80          │ 12    │ CameraPosition (float3)                 │
│ +96          │ 4     │ FOV (radians)                           │
│ +100         │ 4     │ NearClip                                │
│ +104         │ 4     │ FarClip                                 │
│ +108         │ 12    │ CameraUp (float3)                       │
│ +120         │ 12    │ CameraRight (float3)                    │
│ +132         │ 12    │ CameraForward (float3)                  │
└──────────────┴───────┴─────────────────────────────────────────┘

Vertex Shader Constant Registers (GTA IV Layout):
┌──────────────┬─────────────────────────────────────────────────┐
│ Register     │ Content                                         │
├──────────────┼─────────────────────────────────────────────────┤
│ c0-c3        │ View Matrix (float4x4, row-major)               │
│ c4-c7        │ Projection Matrix (float4x4, row-major)         │
│ c8-c11       │ World Matrix (varies per-object)                │
│ c12-c15      │ WorldViewProjection (combined)                  │
│ c16+         │ Material/lighting constants                     │
└──────────────┴─────────────────────────────────────────────────┘
```

**Extraction Methods:**

1. **Direct Memory Read** - `ExtractCameraFromGuest()`:
   - Reads global addresses directly from guest memory
   - Requires byte-swap for big-endian PPC data
   - Used when shader constants aren't bound yet

2. **Shader Constant Parse** - `ExtractCameraFromShaderConstants()`:
   - Reads view/projection from VS constant buffer
   - Extracts FOV, near/far from projection matrix elements:
     - `P[1][1] = 1/tan(fovY/2)` → FOV
     - `P[2][2] = f/(n-f)`, `P[3][2] = nf/(n-f)` → near/far
   - Extracts camera position from inverse view matrix translation

---

## 12. Shadow System Analysis

### 12.1 Shadow Map Render Targets

GTA IV uses cascaded shadow maps for directional light (sun):

```
SHADOW MAP SYSTEM
═══════════════════════════════════════════════════════════════

Shadow Cascade Configuration:
┌─────────┬───────────┬──────────────────────────────────────────┐
│ Cascade │ Resolution│ Coverage                                 │
├─────────┼───────────┼──────────────────────────────────────────┤
│ 0       │ 512x512   │ Near (0-15m) - player/vehicles           │
│ 1       │ 512x512   │ Mid (15-50m) - buildings/props           │
│ 2       │ 256x256   │ Far (50-200m) - distant terrain          │
└─────────┴───────────┴──────────────────────────────────────────┘

Shadow Pass Functions:
  sub_828D7220  - Shadow render state setup
  sub_828D7450  - Shadow depth write
  sub_828D76A0  - Shadow cascade selection
  sub_828D7990  - Shadow sampling/PCF

No native VSM/ESM - uses standard depth comparison with 2x2 PCF.
```

### 12.2 Key Findings Summary

| System | Native Support | Host Enhancement |
|--------|---------------|------------------|
| **SSAO** | ❌ None (baked AO only) | ✅ GTAO post-process |
| **SSR** | ❌ None (cubemap/planar only) | ✅ Ray-march post-process |
| **Shadows** | ✅ Cascaded depth maps | ⚠️ Higher res, VSM optional |
| **HDR** | ✅ R16G16B16A16F render targets | ⚠️ Better tone mapping |
| **DoF** | ✅ Simple blur | ⚠️ Bokeh DoF optional |
| **Motion Blur** | ✅ Velocity buffer | ⚠️ Per-pixel MB optional |

---

## Files Referenced

| File | Content |
|------|---------|
| `ppc_recomp.110.cpp` | Material system (sub_8284xxxx) |
| `ppc_recomp.111-112.cpp` | Lighting (sub_8286xxxx-8287xxxx) |
| `ppc_recomp.113.cpp` | Light structures (sub_82878xxx) |
| `ppc_recomp.120.cpp` | Scene effects (sub_828Exxxx) |
| `ppc_recomp.121.cpp` | Water system (sub_828Fxxxx) |
| `ppc_recomp.134.cpp` | Render targets (sub_829CAxxxx) |
| `ppc_recomp.135.cpp` | Draw functions, shaders, PM4 |
| `ppc_recomp.81.cpp` | Present/VdSwap |
| `ppc_recomp.66.cpp` | Main loop |

---

*Analysis derived from GTA IV Xbox 360 recompiled PPC code.*

