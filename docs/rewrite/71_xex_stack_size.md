# 71: XEX Stack Size -- Original Value and Amplification Factor

## Answer

GTA IV's PE header specifies a **256 KB** (0x40000) stack, both reserve and commit.
The recompiled code currently runs with a **64 MB** minimum enforced in `kernel_state.cpp`,
giving a **256x amplification factor** over the original Xbox 360 value.

---

## 1. Where the Stack Size Comes From

### XEX Header Optional Header

The XEX format stores stack size as optional header key `XEX_HEADER_DEFAULT_STACK_SIZE`
(ID `0x00020200`). The low byte `0x00` means the value is stored **inline** in the
optional header's 4-byte value field -- not as a pointer to external data.

**File**: `glue/rexglue-sdk-main/include/rex/system/util/xex2_info.h`, line 296:
```
XEX_HEADER_DEFAULT_STACK_SIZE = 0x00020200,
```

### How It Is Read

**File**: `glue/rexglue-sdk-main/src/system/user_module.cpp`, line 213:
```cpp
this->xex_module()->GetOptHeader(XEX_HEADER_DEFAULT_STACK_SIZE, &stack_size_);
```

The `GetOptHeader` implementation in `xex_module.cpp` (line 69-98) handles the `0x00`
key-size case by writing the value directly into the output pointer:
```cpp
case 0x00:
    *reinterpret_cast<uint32_t*>(out_ptr) = static_cast<uint32_t>(opt_header.value);
```

If the XEX header does NOT contain this optional key, `GetOptHeader` returns false and
`stack_size_` remains at its default-initialized value of `0` (declared in
`include/rex/system/user_module.h`, line 105).

### Fallback for ELF Modules

For ELF modules (not XEX), a hardcoded 1 MB default is used instead
(`user_module.cpp`, line 174):
```cpp
stack_size_ = 1024 * 1024;  // 1 MB default stack
```

---

## 2. The Original Value: 256 KB

Since we do not have the raw `.xex` file (only the decompressed PE at `gta_iv/default.bin`),
we extracted the stack size from the **PE optional header** inside the decompressed binary.

The PE header at offset 0xF8 in `default.bin` contains:

| PE Field             | Value      | Decimal        |
|----------------------|------------|----------------|
| Machine              | 0x01F2     | PPC (Xbox 360) |
| SizeOfStackReserve   | 0x00040000 | 262,144 = 256 KB |
| SizeOfStackCommit    | 0x00040000 | 262,144 = 256 KB |
| SizeOfHeapReserve    | 0x00100000 | 1,048,576 = 1 MB |
| SizeOfHeapCommit     | 0x00001000 | 4,096 = 4 KB  |

On Xbox 360, the XEX `DEFAULT_STACK_SIZE` optional header overrides the PE stack size.
However, both typically match because the XEX packager reads the PE header value as the
default. For GTA IV, the value is **256 KB (0x40000)**.

This is consistent with the doc 64 estimate ("256KB to 1MB") and typical for Xbox 360
open-world games. The Xbox 360 kernel also supports dynamic stack growth via guard pages,
so the initial 256 KB allocation could grow at runtime on real hardware.

---

## 3. The Current Override: 64 MB

**File**: `glue/rexglue-sdk-main/src/system/kernel_state.cpp`, lines 271-277:
```cpp
// Recompiled PPC functions use 3-8x more stack than original Xbox 360 code
// (larger C++ frames, VMX register saves, etc.). The XEX header typically
// specifies 256KB-1MB which is insufficient. Enforce a 16MB minimum to
// prevent guest stack overflow during deep call chains (world init,
// collision detection recursion, rendering pipeline).
constexpr uint32_t kMinStackSize = 64 * 1024 * 1024;  // 64 MB
uint32_t stack_size = std::max(module->stack_size(), kMinStackSize);
```

Note: The comment says "16MB minimum" but the code actually enforces **64 MB**. The
comment is stale from an earlier iteration.

---

## 4. Amplification Factor

| Metric | Value |
|--------|-------|
| Original XEX/PE stack size | 256 KB (0x40000) |
| Current LibertyRecomp minimum | 64 MB (0x4000000) |
| Amplification factor | **256x** |
| Host thread stack (separate) | 16 MB (xthread.cpp, line 371) |

### Why 256x?

Recompiled PPC-to-x86/ARM code inflates stack frames because:

1. **Register spill**: Xbox 360 PPC has 32 GPRs + 32 FPRs + CR + XER + CTR + LR, all of
   which may be spilled to the host stack frame rather than kept in registers
2. **PPCContext parameter**: Every recompiled function receives a `PPCContext*` and
   frequently reads/writes through it, generating additional stack temporaries
3. **C++ prologue/epilogue**: The host compiler adds frame pointers, alignment padding,
   callee-saved register saves (16-byte alignment on macOS ARM64)
4. **No leaf optimization**: PPC leaf functions that used no stack at all become non-leaf
   C++ functions with full stack frames
5. **VMX (Altivec) saves**: 128-bit vector register spills are 16 bytes each

A typical PPC function using 64-128 bytes of stack becomes a C++ function using
400-1000+ bytes. For the deepest call chains in GTA IV (world init, collision BVH
traversal, rendering pipeline), this compounds across 50-200+ nested frames.

### Is 64 MB Enough?

The doc 64 analysis measured overflow at ~5.8 MB with the old (unpatched) stack size.
With 64 MB, there is roughly 11x headroom over the observed peak usage. This should be
sufficient for all GTA IV code paths including deep recursion in collision detection
and scene graph traversal.

---

## 5. Task Thread Stack Sizing

XAM task threads (background work items) also inherit the module's stack size:

**File**: `glue/rexglue-sdk-main/src/kernel/xam/xam_task.cpp`, lines 52-55:
```cpp
uint32_t stack_size = kernel_state()->GetExecutableModule()->stack_size();
stack_size = std::max((uint32_t)0x4000, ((stack_size + 0xFFF) & 0xFFFFF000));
```

This reads `module->stack_size()` which returns the **original XEX value** (256 KB),
NOT the 64 MB override. The override only applies to the main thread in `LaunchModule()`.
Task threads get 256 KB (rounded up to page alignment = 256 KB). The `XThread` constructor
enforces a 16 KB minimum, which is well below 256 KB.

This means task threads may also be vulnerable to stack overflow if they hit deep
recompiled call chains, though they typically run simpler callbacks.

---

## 6. How to Log the Stack Size at Runtime

There is currently no logging of the XEX stack size. To add it, a `REXSYS_INFO` call
could be placed after line 213 in `user_module.cpp`:

```cpp
this->xex_module()->GetOptHeader(XEX_HEADER_DEFAULT_STACK_SIZE, &stack_size_);
REXSYS_INFO("XEX DEFAULT_STACK_SIZE: {} bytes ({} KB)", stack_size_, stack_size_ / 1024);
```

The Xenia reference implementation (`tools/xenia-master-1/src/xenia/kernel/user_module.cc`,
line 566-568) does log it in its `DumpModule` function:
```cpp
case XEX_HEADER_DEFAULT_STACK_SIZE:
    sb.AppendFormat("  XEX_HEADER_DEFAULT_STACK_SIZE: {}\n",
                    static_cast<uint32_t>(opt_header.value));
```

---

## 7. Extracting XEX Header Info Without the Raw XEX

Since only `gta_iv/default.bin` (the decompressed PE) is available in this repo:

- **PE header**: Can be parsed directly. Stack/heap sizes are at standard PE offsets.
  Machine type 0x01F2 confirms Xbox 360 PPC. Verified: 256 KB stack.
- **XEX optional headers**: NOT available. The XEX wrapper (with encryption, compression,
  signatures, and optional headers) was stripped during decompression by XenonRecomp.
- **XenonRecomp** (`tools/XenonRecomp/`): Parses the XEX but does not preserve or log
  the stack size. It defines `XEX_HEADER_DEFAULT_STACK_SIZE` in `xex.h` but never reads it.
- **xex_ctor_finder.py**: Defines the header constant but only looks for constructor tables,
  not stack size.

To dump XEX headers from a raw XEX file, Xenia's `DumpModule()` function or a custom
script using the `parse_xex_header()` function from `xex_ctor_finder.py` would work.

---

## Key Files

| File | Purpose |
|------|---------|
| `glue/rexglue-sdk-main/include/rex/system/util/xex2_info.h` | XEX header key definitions |
| `glue/rexglue-sdk-main/src/system/xex_module.cpp` (line 69) | `GetOptHeader` implementation |
| `glue/rexglue-sdk-main/src/system/user_module.cpp` (line 213) | Reads `DEFAULT_STACK_SIZE` from XEX |
| `glue/rexglue-sdk-main/src/system/kernel_state.cpp` (line 276) | 64 MB minimum override |
| `glue/rexglue-sdk-main/src/system/xthread.cpp` (line 224) | `AllocateStack` implementation |
| `glue/rexglue-sdk-main/src/kernel/xam/xam_task.cpp` (line 52) | Task thread stack sizing |
| `gta_iv/default.bin` | Decompressed PE with 256 KB stack in PE header |
| `tools/XenonRecomp/XenonUtils/xex.h` | XenonRecomp's XEX header key enum |
