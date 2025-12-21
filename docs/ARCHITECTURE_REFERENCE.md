# PPC Architecture Reference

## Purpose
Combines the existing subsystem map, critical execution paths, and data structure references so developers see addresses, call graphs, and memory layouts in one place.

### Contents
1. Address map & subsystem overview
2. Critical boot/render/streaming paths
3. Device, entity, streaming, threading, and kernel structures

---

## 1. Subsystem Address Map
- Core/Init: 0x82120000–0x8219FFFF
- Entity system: 0x821A0000–0x823FFFFF
- World/Physics: 0x82400000–0x825FFFFF
- AI/Scripting: 0x82600000–0x826FFFFF
- Streaming/IO: 0x82700000–0x827FFFFF
- Effects/Shaders: 0x82800000–0x8289FFFF
- Resources/Assets: 0x828A0000–0x829BFFFF
- GPU/Rendering: 0x829C0000–0x829FFFFF
- Kernel imports: 0x82A00000+

### Key rewrite tiers
| Subsystem | Tier | Notes |
|-----------|------|-------|
| GPU/D3D | 1 | Direct hardware interface (hook and replace) |
| Streaming/IO | 1 | Needs async replacement |
| Threading | 1 | Kernel primitives must be reimplemented |
| Entity/AI/Physics | 3 | Pure logic can remain with minimal changes |

---

## 2. Critical Execution Paths
Detailed call graphs for boot, render, streaming, shader, threading, audio, input, and error flows.

### Boot call graph
```
_xstart → sub_8218BEA8 → sub_827D89B8 → sub_8218BEB0 → sub_82120000 → sub_8218C600 → (memory pools, ExCreateThread x9, GPU init, audio init, resource init)
```

### Render path (per frame)
```
sub_827D89B8 → input update → render dispatch → pre-frame setup → frame presentation → sub_829D5388 (Present) → __imp__VdSwap
```

### Streaming path
```
sub_829A1F00 (file open) → sub_827F0000 path resolve → __imp__NtCreateFile → sub_827E8420 reads → __imp__NtReadFile → __imp__NtWaitForSingleObjectEx (if STATUS_PENDING)
```

### Shader path
```
sub_8285E048 (EffectManager::Load) → sub_827E6A70 cache lookup → sub_827E5700 parse FXC → sub_82858758 register → sub_83200000 shader handle ready → SetShader hook (sub_829CD350/sub_829D6690)
```

### Threading path
```
sub_8218C600 → ExCreateThread x9 → render worker entry (sub_827DE858) waiting on semaphores → streaming worker (sub_82193B80) handling requests → signal completion via KeReleaseSemaphore
```

---

## 3. Data Structures
### Device Context (14KB)
```cpp
struct DeviceContext { ... offsets +48 command buffer, +10456 vertexDeclaration, +10932 VS, +10936 PS, +12020-+12032 stream sources, +13580 index buffer, +19480 frame buffer index }
```

### Render resources
- RenderTarget (width, height, format, texturePtr, surfacePtr)
- VertexBuffer/IndexBuffer (dataPtr, size, stride/format)
- ShaderHandle (type, bytecodePtr, size, constantsPtr)

### Streaming structures
- StreamObject (vtable, handle, position, size, buffer)
- ResourceRequest (priority queue nodes)
- RpfHeader/RpfTocEntry (header + TOC fields)

### Entity structures
- CEntity (+transform/matrix, handles, stream flags)
- CPhysical (+physics params, velocities, mass)
- CVehicle, CPed (extended vehicle/ped fields)

### Threading structures
- KEVENT/KSEMAPHORE/RTL_CRITICAL_SECTION/KTHREAD layouts

### I/O and shader structs
- XOVERLAPPED, IO_STATUS_BLOCK, OBJECT_ATTRIBUTES
- FxcHeader, ShaderTableEntry, GameState (pointer base +0x83003D10)

### Memory layout summary
```
0x82000000-0x82FFFFFF: code/heap; 0x83000000+: static data; shader table 0x830E5900; frame buffers 0x7FC00000; global state at 0x83003D10
```

---

## Document History
- 2025-12-20: Consolidated subsystem, call graph, and structure docs into ARCHITECTURE_REFERENCE.md.
