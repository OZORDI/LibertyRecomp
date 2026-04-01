# Streaming Resource Processing in RexGlue / LibertyRecomp

Research date: 2026-03-28

## Key Question

Does the RexGlue recomp framework have built-in support for RAGE engine streaming
resource processing, or must LibertyRecomp implement it via hooks?

**Answer: No built-in support. LibertyRecomp must handle all streaming via hooks.**

---

## 1. Function Table Architecture

The function table maps guest PPC addresses to native host function pointers.

| Component | File | Role |
|-|-|
| `PPCFuncMappings[]` | `gta4-recomp/generated/gta4_init.cpp` | Static array of {guest_addr, host_func} pairs |
| `PPC_LOOKUP_FUNC(base, addr)` | `include/rex/ppc/context.h` | Macro: `*(PPCFunc**)(base + IMAGE_BASE + IMAGE_SIZE + (addr - CODE_BASE) * 2)` |
| `PPC_CALL_INDIRECT_FUNC(addr)` | `include/rex/ppc/context.h` | Range-checks addr against CODE_BASE/CODE_SIZE, calls via PPC_LOOKUP_FUNC or logs MISSING-FUNC |
| `Memory::InitializeFromRexGlue()` | `LibertyRecomp/kernel/memory.cpp` | Iterates PPCFuncMappings[], calls `rex_memory_->SetFunction()` for each |
| `Processor::SetFunction()` | `glue/.../src/system/processor.cpp` | Writes to both C++ `unordered_map` and guest memory function table |

Config constants (from `gta4_config.h`):
- `PPC_IMAGE_BASE = 0x82000000`
- `PPC_IMAGE_SIZE = 0x1300000`
- `PPC_CODE_BASE  = 0x82140000`
- `PPC_CODE_SIZE  = 0x94635C`

Code range: `0x82140000` to `0x82A8635C`. Addresses outside this range produce
nullptr from PPC_LOOKUP_FUNC. The function table resides at `base + 0x83300000`
(IMAGE_BASE + IMAGE_SIZE) and extends for CODE_SIZE * 2 bytes = ~0x128C6B8 bytes.

## 2. VTable Scanner (Codegen-Time Only)

`VTableScanner` (`glue/.../src/codegen/vtable_scanner.cpp`) scans the XEX .rdata
section for MSVC RTTI Complete Object Locators (COL signature=0, `.?AV`/`.?AU`
type descriptors). It discovers vtables and their function slots at **codegen time**
to ensure those functions are included in the recompiled output.

This scanner does NOT run at game runtime. It is purely a code generation aid.

## 3. VTable Prepopulation (Runtime)

At runtime, `Memory::PopulateFunctionTableAndVtables()` writes vtable entries
into guest memory so the recompiled code's vtable dispatch works correctly.

Sources:
- **Auto-generated**: `gta_iv/vtable_prepopulate.h` (169KB, 4110 lines)
  - 146 vtables, 3809 total entries
  - Generated from `gta_iv/gta4_vtables.txt` (Ghidra extraction)
  - Coverage: `0x82140CAC` to `0x8225BE10` ONLY
  - **Zero entries in 0x8285xxxx-0x8287xxxx range**
- **Manual**: `LibertyRecomp/kernel/memory.cpp` lines 100-150
  - ~20 additional vtable entries in the `0x8207xxxx` range (zero-fill BSS blocks)
  - Covers resource manager vtables used by `sub_8280BAD8`

## 4. Streaming Hooks (All Custom LibertyRecomp Code)

LibertyRecomp has implemented streaming handling entirely via hooks:

| Hook | Address | Purpose |
|-|-|-|
| `sub_827DF248` (pgStreamer::Init) | `0x827DF248` | Forces sync streaming mode (`0x830F589C=1`) to avoid dead worker threads |
| `sub_8284CFD8` (ring-buffer pool) | `0x8284CFD8` | Seeds semaphore handles for 2 streaming workers via XSemaphore API |
| `sub_82192E00` (streaming init) | `0x82192E00` | Clears streaming-pending flag `0x830F5820` after init (no async completion) |
| `sub_827DE648` (streaming barrier) | `0x827DE648` | Returns immediately instead of spinning on `0x830F5820` |
| `sub_829A2540` (NtSetEvent wrapper) | `0x829A2540` | Guards against invalid handle (0xCDCDCDCD) in RPF streaming workers |
| LOD/distance hooks | various | Modifies streaming distances, pool sizes, slot counts |

The streaming model is forced-synchronous: all I/O completes inline on the
calling thread via `sub_827DE1C0`. No async worker threads survive startup.

## 5. Gap Analysis: 0x8285-0x8287 Range

The vtable prepopulate file has **zero coverage** of the `0x8285xxxx`-`0x8287xxxx`
address range. The `gta4_vtables.txt` Ghidra extraction also has no entries there.

This range contains RAGE engine subsystem vtables that are populated dynamically
at runtime (e.g., resource type registrations, streaming coordinator callbacks).
On Xbox 360, these vtables are written during CGame::Initialise and pgStreamer
setup. In the recomp, if the init code runs correctly, these vtables should be
populated by the recompiled code itself.

The specific addresses from the research prompt:
- `0x8286C238` — not found anywhere in the codebase (no vtable entry, no hook, no config)
- `0x831E55EC` — not found anywhere in the codebase (streaming coordinator global)

These would need to be traced through the recompiled code's execution path to
determine if they are populated naturally or need manual intervention.

## 6. Conclusions

1. **No framework support**: RexGlue provides function table infrastructure and
   codegen-time vtable scanning, but zero runtime streaming support. All
   streaming handling is custom LibertyRecomp hook code.

2. **VTable coverage gap**: The prepopulated vtable range stops at `0x8225BE10`.
   The `0x8285-0x8287` range (RAGE subsystem vtables) is entirely uncovered in
   the static prepopulation. These vtables must either be populated by the
   recompiled game code itself during init, or need new manual entries.

3. **Forced-sync streaming**: The current architecture forces all resource I/O
   to be synchronous. This avoids async completion issues but means any code
   path that depends on async streaming callbacks will never fire naturally.

4. **Resource process callback pattern**: If a streaming resource callback at
   `0x8286C238` needs to fire, it would need either:
   - A PPC_FUNC_HOOK that manually invokes the callback
   - An `InsertFunction()` entry in the function table
   - A manual vtable write in `PopulateFunctionTableAndVtables()`
   - Or verification that the recompiled init code naturally writes it
