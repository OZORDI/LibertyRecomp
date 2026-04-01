# Streaming Manager Globals Analysis

## Address Computation

### Global 1: Streaming Request Stack
```
lis(-32080) = ((-32080) & 0xFFFF) << 16 = 0x82B00000
0x82B00000 + 29304 = 0x82B07278
```
- **Address**: `0x82B07278`
- **Type**: Static .bss struct (not heap-allocated)
- **Size**: ~3076 bytes (0xC04)

### Global 2: Streaming Coordinator Pointer
```
lis(-31970) = ((-31970) & 0xFFFF) << 16 = 0x831E0000
0x831E0000 + 21996 = 0x831E55EC
```
- **Address**: `0x831E55EC`
- **Type**: Pointer to heap-allocated 48-byte object

## 0x82B07278 -- Streaming Request Stack

A static structure used by sub_8284F310 (push) and sub_8284E830 (pop) to track active streaming path requests.

### Struct Layout

| Offset | Size | Field |
|-|-|
| 0x000-0x3FF | 1024 | Header/reserved area |
| 0x400+i*256 | 256 | entry[i]: null-terminated path string buffer |
| 0xC00 | 4 | `count`: number of active requests (uint32) |

### Operations
- **sub_8284F310(this, path_str)**: Copies path string into entry[count+4]*256, increments count. Handles `$`-prefixed device paths by stripping the prefix and using the module name from field 3072 as prefix lookup.
- **sub_8284E830(this)**: Decrements count (pops the most recent request).

### Usage in sub_827C2420 (gta4_recomp.50.cpp:57395)
1. Loads 0x82B07278 into r30
2. Calls sub_8284F310(0x82B07278, resource_path) to push a streaming request
3. Reads 0x831E55EC into r29 (the coordinator)
4. Virtual call on the resource object (vtable[1])
5. Calls sub_82852DD0(coordinator, ...) to dispatch the streaming request
6. Calls sub_8284E830(0x82B07278) to pop the request

### Referenced in ~60+ functions across the codebase
Heavy usage files: gta4_recomp.59.cpp, gta4_recomp.51.cpp, gta4_recomp.5.cpp, gta4_recomp.60.cpp, gta4_recomp.61.cpp

## 0x831E55EC -- Streaming Coordinator

A pointer to a heap-allocated 48-byte object that coordinates streaming module dispatch.

### Initialization
- **Function**: sub_827AC578 (gta4_recomp.50.cpp:4729)
- Allocates 48 bytes via sub_821B3510(48)
- Constructs via sub_82852F48
- Stores pointer at 0x831E55EC
- Then calls sub_82852FB0 to register streaming modules

### Teardown
- In sub_827AAD40 area (gta4_recomp.50.cpp:1120): reads pointer, calls sub_82853050 + sub_821B3560 (free), stores NULL to 0x831E55EC

### Struct Layout (48 bytes, initialized by sub_82852F48)

| Offset | Size | Field |
|-|-|-|
| 0 | 4 | Slot 0: entries pointer (u32) |
| 4 | 2 | Slot 0: active count (u16) |
| 6 | 2 | Slot 0: capacity (u16) |
| 8-10 | 3 | Slot 0: padding |
| 11 | 1 | Slot 0: flag (u8) |
| 12 | 4 | Slot 1: entries pointer (u32) |
| 16 | 2 | Slot 1: active count (u16) |
| 18 | 2 | Slot 1: capacity (u16) |
| 19-22 | 3 | Slot 1: padding |
| 23 | 1 | Slot 1: flag (u8) |
| 24 | 4 | Slot 2: entries pointer (u32) |
| 28 | 2 | Slot 2: active count (u16) |
| 30 | 2 | Slot 2: capacity (u16) |
| 31-34 | 3 | Slot 2: padding |
| 35 | 1 | Slot 2: flag (u8) |
| 36 | 4 | Config value = 4 (u32) |
| 40 | 4 | Flags bitmask, init = 3 (bits 0+1) |
| 44 | 4 | Auxiliary pointer, init = 0 (u32) |

### Flag Read in sub_82852D18 (line 31033)
```
lwz r11, 21996(r11)     ; r11 = *(0x831E55EC) = coordinator ptr
lwz r11, 40(r11)        ; r11 = coordinator->flags
rlwinm r31,r11,15,31,31 ; r31 = (flags >> 17) & 1 = bit 17 extraction
```
At init, flags = 3, so bit 17 = 0 (streaming not yet active). This bit is set later during streaming operations to indicate streaming is in progress.

## sub_8285D610/sub_8285D948 Comparison

**These do NOT operate on 0x82B07278.** They are a different subsystem:

| Property | 0x82B07278 (Request Stack) | sub_8285D948 handles |
|-|-|-|
| Parent address | Static at 0x82B07278 | 0x831AB9B4 (module registry) |
| Object size | ~3076 bytes (.bss) | 1080 bytes (heap-alloc) |
| Entry size | 256-byte string buffers | 192-byte module slots |
| Purpose | Track active path requests | Create streaming module handles |
| Init function | Zero-init (.bss) | sub_8285D610 constructs |

sub_8285D948 reads a capability flag from 0x82B08210 (lis(-32079) + lwz(-32240)) and passes it to sub_8285D610 as the `flags` parameter (r8). The 1080-byte handles created by sub_8285D610 contain up to 8 module slots (192 bytes each) at offset 8, with metadata at offsets 0-7 and 1032-1076.

## Init Chain Summary

1. sub_827AC578 is the streaming subsystem init entry point
2. It allocates and constructs the coordinator (48 bytes -> 0x831E55EC)
3. Then calls sub_82852FB0 to register streaming modules into the coordinator slots
4. 0x82B07278 is zero-initialized as .bss; sub_8284F310/sub_8284E830 operate on it as a push/pop request stack throughout the game's lifetime
5. sub_8285D948 creates separate 1080-byte module handles stored under 0x831AB9B4, unrelated to 0x82B07278
