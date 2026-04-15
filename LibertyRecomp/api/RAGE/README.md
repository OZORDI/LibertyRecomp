# RAGE Engine API

This directory contains reverse-engineered structures from GTA IV's RAGE (Rockstar Advanced Game Engine) for Xbox 360.

## Overview

The RAGE engine is Rockstar Games' proprietary game engine used in GTA IV, RDR, and later titles. This API provides C++ structures that mirror the Xbox 360 version's memory layout for use with the LibertyRecomp static recompilation project.

## Files

| File | Description |
|------|-------------|
| `RAGE.h` | Main header with all RAGE engine structures |
| `RAGE.inl` | Inline implementations for math operations and utilities |

## Architecture

### Namespaces

- **`rage`** - Core engine types (graphics, audio, physics, scripting)
- **`GTA4`** - Game-specific classes (entities, peds, vehicles, world)

### Key Components

#### Base Types (`rage` namespace)
- `datBase` - Base class for most RAGE objects
- `pgBase` - Base class for paged/streamed resources
- `Vector2/3/4` - Math vector types
- `Matrix34/44` - Transform matrices
- `atArray<T>` - Dynamic array container
- `atHashMap<K,V>` - Hash map container

#### File I/O
- `fiDevice` - Abstract file device interface
- `fiPackfile` - RPF archive file system

#### Graphics
- `grcDevice` - Graphics device manager
- `grcTexture` - Texture resource
- `grmShader` - Shader program
- `grmShaderGroup` - Shader collection

#### Physics
- `phBound` - Physics collision bound
- `phBoundComposite` - Compound collision shape

#### Scripting
- `scrProgram` - Compiled script program (.SCO)
- `scrThread` - Script execution thread

#### Audio
- `audEngine` - Audio system manager
- `audSound` - Sound instance

### Game Classes (`GTA4` namespace)

#### Entity Hierarchy
```
CEntity
├── CPhysical
│   ├── CDynamicEntity
│   ├── CPed
│   │   └── CPlayerPed
│   ├── CVehicle
│   │   ├── CAutomobile
│   │   ├── CBike
│   │   ├── CBoat
│   │   └── CHeli
│   └── CObject
│       └── CPickup
└── CBuilding
```

#### Singletons
- `CGame` - Main game controller
- `CWorld` - World entity manager
- `CCamera` - Camera system
- `CStreaming` - Resource streaming manager
- `CTimer` - Game timing

#### Pool System
- `CPool<T>` - Fixed-size object pool template
- `g_pedPool`, `g_vehiclePool`, `g_objectPool`, `g_buildingPool` - Global pools

## Memory Layout Notes

⚠️ **IMPORTANT**: All structures must match Xbox 360 memory layout!

- Xbox 360 uses **big-endian** byte order (handled by rexglue PPC memory ops)
- Pointers are **32-bit** on Xbox 360
- Alignment may differ from x86/x64

## Usage

```cpp
#include "RAGE/RAGE.h"
#include "RAGE/RAGE.inl"  // For math operations

using namespace GTA4;

void ProcessPed(CPed* ped)
{
    if (ped && ped->IsAlive())
    {
        rage::Vector3 pos = ped->GetPosition();
        // ...
    }
}
```

## References

- GTA IV Xbox 360 default.xex (XEX2 format)
- rexglue codegen analysis output (switch tables, function signatures)
- RAGE engine research community

## Status

| Component | Status |
|-----------|--------|
| Base types | ✅ Implemented (stubs) |
| Math types | ✅ Implemented |
| Containers | ✅ Implemented (stubs) |
| File I/O | 🔄 Partial |
| Graphics | 🔄 Partial |
| Physics | 🔄 Partial |
| Scripting | 🔄 Partial |
| Audio | 🔄 Partial |
| Entities | ✅ Implemented (stubs) |
| Pools | ✅ Implemented (stubs) |

Legend: ✅ Complete | 🔄 Partial | ❌ Not Started

## TODO

- [ ] Verify structure sizes match Xbox 360 binary
- [ ] Add native function table mappings
- [ ] Document memory offsets for key globals
- [ ] Add more vehicle/ped subtypes
- [ ] Implement weapon system structures
- [ ] Add pathfinding/navigation structures
