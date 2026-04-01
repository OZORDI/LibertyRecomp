# 65: vtable_prepopulate.h System Audit

## 1. System Overview

The vtable pre-population system writes function pointers into guest memory vtable
addresses before game code runs. It ensures that C++ virtual dispatch (load vtable
pointer from object, index into vtable, call function pointer) resolves to valid
recompiled function addresses.

### Files

| File | Role |
|------|------|
| `/Users/Ozordi/Downloads/LibertyRecomp/gta_iv/gta4_vtables.txt` | Source data: 148 lines, format `vtable_address,entry_count` |
| `/Users/Ozordi/Downloads/LibertyRecomp/gta_iv/vtable_prepopulate.h` | Auto-generated: 4110 lines, 3809 `PPC_STORE_U32` calls for 146 vtables |
| `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/kernel/memory.cpp` | Includes vtable_prepopulate.h + 33 manual `PPC_STORE_U32` calls for 12 additional vtable groups |

### Call Chain

```
Memory::Init()
  -> rex_memory_->InitializeFunctionTable(PPC_CODE_BASE, PPC_CODE_SIZE, ...)
  -> Register all PPCFuncMappings via SetFunction()
  -> PopulateFunctionTableAndVtables()
       -> InsertFunction() for a few manual stubs
       -> PrePopulateVtables(base)        // 146 auto-generated vtables
       -> 12 manual vtable groups          // 0x8207xxxx range
       -> XAM sign-in state + readiness flags
```

## 2. How PrePopulateVtables Works

`PrePopulateVtables(uint8_t* base)` is a static function in `vtable_prepopulate.h`.
It takes the guest memory base pointer and executes ~3809 `PPC_STORE_U32` calls.

Each call writes a 32-bit big-endian value (a function pointer) to a guest address:

```cpp
PPC_STORE_U32(0x82140CAC, 0x82140CC0);  // vtable[0] = 0x82140CC0
PPC_STORE_U32(0x82140CB0, 0x82140CD0);  // vtable[1] = 0x82140CD0
```

**Format**: `PPC_STORE_U32(vtable_slot_address, function_pointer_value)`

- `vtable_slot_address`: The guest address where the function pointer lives.
  Consecutive slots are 4 bytes apart (e.g., 0x82140CAC, 0x82140CB0, 0x82140CB4).
- `function_pointer_value`: The guest address of the function to call. These are
  addresses within the `.text` code section that `PPC_CALL_INDIRECT_FUNC` can
  resolve via `PPC_LOOKUP_FUNC`.

### Why This Is Needed

On Xbox 360, the XEX loader maps the PE image into memory and vtables in `.text`
already contain the correct function pointers as static data. In the recompiler,
the generated code range starts at `PPC_CODE_BASE` (0x82140000) and function
dispatch uses a separate lookup table (`PPC_LOOKUP_FUNC`). The vtable data in the
original binary image may be loaded, but the function table entries must also exist
for `PPC_CALL_INDIRECT_FUNC` to resolve them. Pre-populating ensures:

1. The vtable memory locations contain the correct function pointer values.
2. Those function pointer values are addresses that the recompiler has registered
   in its function table.

## 3. Coverage: The 146 Auto-Generated Vtables

All 146 vtables in `gta4_vtables.txt` are in the range **0x82140CAC -- 0x8225BE10**.
This is entirely within the `.text` code section (0x82140000+). The function pointer
values they contain are also in this range (e.g., vtable at 0x82140CAC has entries
pointing to 0x82140CC0, 0x82140CD0, etc.).

**Address range summary**:
- Lowest vtable: `0x82140CAC` (5 entries)
- Highest vtable: `0x8225BE10` (4 entries)
- All entries point to addresses in `0x82140xxx` -- `0x8225xxxx` (code section)
- Total: 3809 function pointer entries across 146 vtables

**Notable large vtables**:
- `0x821498C8`: 254 entries
- `0x821DD9F8`: 246 entries
- `0x8215DB18`: 228 entries
- `0x821518A0`: 205 entries
- `0x821602FC`: 205 entries
- `0x822109FC`: 183 entries
- `0x82210D58`: 154 entries

### Generation Method

The header comment says: "Auto-generated vtable data from GTA IV XEX / Generated
from gta4_vtables.txt and default.bin". No generation script was found in the
repository. The file `gta4_vtables.txt` says "GTA IV Vtables extracted by Ghidra".

**Likely generation process** (inferred):
1. Ghidra analysis identified vtable structures in the `.text` section
2. Vtable addresses and entry counts were exported to `gta4_vtables.txt`
3. A script (not in repo) read `gta4_vtables.txt` + `default.bin`, extracted the
   function pointer values from the binary at each vtable address, and generated
   the C header

The tool is **not in the repository** -- it was likely a one-off Ghidra script or
external Python script.

## 4. The 12 Manual Vtable Groups (memory.cpp)

In addition to the 146 auto-generated vtables, `memory.cpp` manually writes 33
`PPC_STORE_U32` entries for 12 vtable groups, all at addresses in the **0x8207xxxx**
range:

| Vtable Address | Entries | Purpose |
|----------------|---------|---------|
| `0x8207C4A0` | 3 | sub_8280D9B8 cleanup dispatchers |
| `0x8207C3C8` | 1 | sub_8280D8A8 cleanup method |
| `0x8207C3B4` | 1 | sub_8280D5E8 |
| `0x8207C3BC` | 2 | sub_8280D6A8, sub_8280D648 |
| `0x8207C2E4` | 3 | sub_8280D460 + stubs |
| `0x8207C2F4` | 2 | sub_8280D6A8 + stub |
| `0x8207C300` | 3 | sub_8280D460, sub_8280D4A8, sub_8280D560 |
| `0x8207CB94` | 3 | sub_8280F0F0 + stubs |
| `0x8207CC0C` | 3 | sub_8280F700, sub_8280F340, sub_8280F418 |
| `0x8207C518` | 1 | sub_8280DDE8 |
| `0x8207BBAC` | 3 | Resource manager (sub_8280BAD8) |
| `0x8207BB80` | 2 | sub_8280B710, sub_8280B508 |
| `0x8207BB74` | 3 | sub_8280B710 + stub + sub_820DDB00 |
| `0x8207BB98` | 3 | sub_8280B5D0, sub_8280D3A8, stub |

These are in "XEX zero-fill blocks" (BSS-like sections at 0x8207xxxx) that the
original Xbox 360 runtime would have filled during CRT initialization or static
constructors. They are NOT in `.rdata` or `.text` -- they're in uninitialized data
sections that need explicit population.

## 5. Why Audio Vtables Are Missing

### The Three Audio Vtables

| Address | Used By | Section |
|---------|---------|---------|
| `0x820BE4C0` | audDevice base vtable (set by sub_821903B8 constructor) | `.rdata` |
| `0x820BE768` | audRenderEndpoint vtable (set by sub_82199EC8 factory) | `.rdata` |
| `0x820BE818` | Unknown audio sub-object | `.rdata` |

Plus two sub-object vtables:
- `0x820BE5CC` -- stored at endpoint sub-object offset 0
- `0x820BE5F8` -- stored at endpoint sub-object offset 4

### Why They Are Missing

**The auto-generated vtables only cover the `.text` code section (0x82140000+).**
The Ghidra extraction process identified vtables in the `.text` range where function
code and vtable data are interleaved. Vtables in `.rdata` (0x82000400 -- 0x820E6178)
were not extracted.

The `.rdata` section is below `PPC_CODE_BASE` (0x82140000). The address range is:
- `.rdata`: 0x82000400 -- 0x820E6178 (guest addresses)
- Code section starts at: 0x82140000

**The 0x820BE768 problem (from doc 46)**:

The data at `0x820BE768` in `default.bin` contains RTTI/type metadata structures,
NOT function pointers:

```
+0:  0x820BE774  (self-referencing .rdata pointer)
+4:  0x820E512C  (.rdata pointer - RTTI TypeDescriptor)
+8:  0x00000000  (null)
+12: 0x82A3CB70  (.text function pointer - valid)
+68: 0x00000002  (small integer, NOT a function pointer)
```

On Xbox 360, MSVC lays out vtables and RTTI descriptors adjacently in `.rdata`.
The vtable pointer stored in the object points to the start of the function pointer
array, with the RTTI "Complete Object Locator" at vtable[-1]. In this case,
`0x820BE768` appears to point to the RTTI descriptor rather than the function table.

When `sub_821910D0` (audio render pump) does:
```
r3  = [audioDevice + 64]    // endpoint object
r11 = [r3 + 0]              // vtable pointer = 0x820BE768
r11 = [r11 + 68]            // vtable[17] = 0x00000002 (NOT a function)
call r11                     // MISSING-FUNC: 0x000F4000 (corrupted)
```

The call fails because the vtable data is RTTI metadata, not function pointers.

## 6. How to Add Audio Vtables

### Option A: Manual Vtable Entries (Like the 0x8207xxxx Pattern)

Add entries to `PopulateFunctionTableAndVtables()` in `memory.cpp`:

```cpp
// Audio device vtable at 0x820BE4C0 (audDevice)
// Must identify correct function pointers from IDA/Ghidra analysis of default.bin
PPC_STORE_U32(0x820BE4C0 + 0,  0x8219xxxx);  // vtable[0] - destructor
PPC_STORE_U32(0x820BE4C0 + 4,  0x8219xxxx);  // vtable[1] - AddRef
// ... etc

// Audio endpoint vtable at 0x820BE768 (audRenderEndpoint)
PPC_STORE_U32(0x820BE768 + 0,  0x8219xxxx);  // vtable[0]
// ...
PPC_STORE_U32(0x820BE768 + 68, 0x8219xxxx);  // vtable[17] - the critical one
```

**Challenge**: The correct function pointer values must be identified from the
original XEX binary. The RTTI metadata at 0x820BE768 is NOT the vtable -- the
actual function pointer array is at a nearby but different address. IDA or Ghidra
disassembly of the constructor (`sub_82199EC8`) would reveal what address the
compiler intended.

### Option B: Hook the Audio Pump to Skip the Vtable Call

Since RexGlue provides native XAudio rendering via SDL, the game's own audio
render endpoint is redundant. Hook `sub_821910D0` to skip the vtable[17] call:

```cpp
InsertFunction(0x821910D0, [](PPCContext& ctx, uint8_t* base) {
    // No-op: audio rendering handled by RexGlue native audio
    ctx.r3.u32 = 0; // Return success
});
```

This is the approach recommended in doc 46 and is the simplest fix.

### Option C: Extend the Auto-Generation to Cover .rdata

Re-run the Ghidra vtable extraction to include the `.rdata` section (0x82000400 --
0x820E6178). This would require:

1. Identifying vtable structures in `.rdata` (distinguishing them from RTTI)
2. For MSVC RTTI layout: the actual vtable is at `COL_address + sizeof(COL)`, and
   the object's vptr points to the start of the function pointer array, with
   `vptr[-1]` being the RTTI Complete Object Locator pointer
3. Adding the new vtables to `gta4_vtables.txt` and regenerating the header

## 7. Other Potentially Missing Vtables in .rdata (0x820xxxxx)

The `.rdata` section spans 0x82000400 -- 0x820E6178 (~922 KB). Any C++ class
whose vtable is in this range (rather than interleaved with code in `.text`) will
have the same problem as the audio vtables.

### Known Missing Audio Vtables

| Address | Class (inferred) | Entry Count | Status |
|---------|------------------|-------------|--------|
| `0x820BE4C0` | audDevice | Unknown | NOT pre-populated |
| `0x820BE4D4` | audDevice base class | Unknown | NOT pre-populated |
| `0x820BE768` | audRenderEndpoint | 18+ (slot 17 needed) | NOT pre-populated |
| `0x820BE808` | audEndpointBase | Unknown | NOT pre-populated |
| `0x820BE818` | Unknown audio object | Unknown | NOT pre-populated |
| `0x820BE5CC` | Endpoint sub-object vtable 1 | Unknown | NOT pre-populated |
| `0x820BE5F8` | Endpoint sub-object vtable 2 | Unknown | NOT pre-populated |

### Systematic Risk: All .rdata Vtables

Any constructor that writes a vtable pointer in the 0x8200xxxx -- 0x820Exxxx range
is potentially affected. To find all such cases, search the generated recomp code
for patterns like:

```
lis  rX, 0x820C   (or 0x820B, 0x820A, etc.)
addi rX, rX, imm
stw  rX, 0(rY)    (store to object+0 = vtable write)
```

A systematic Ghidra/IDA search for "store to offset 0 of newly allocated object"
with values in 0x82000000 -- 0x8213FFFF would identify all .rdata vtables that
need pre-population.

### Quick Grep Estimate

Searching the generated recomp code for `lis` patterns loading 0x820B or 0x820C
(the common prefixes for .rdata vtable addresses) stored to object offset 0 would
reveal additional missing vtables beyond the audio ones. The audio vtables at
0x820BExxx are the most impactful because they're hit during normal startup, but
other subsystems (graphics, physics, networking) may have similar vtables.

## 8. Summary

| Question | Answer |
|----------|--------|
| What does PrePopulateVtables do? | Writes function pointers to guest memory vtable addresses via PPC_STORE_U32 |
| How many auto-generated vtables? | 146 vtables, 3809 entries |
| How many manual vtables? | 12 groups, 33 entries (0x8207xxxx range) |
| Address range covered? | Auto: 0x82140CAC -- 0x8225BE10 (`.text` only); Manual: 0x8207BB74 -- 0x8207CC14 (BSS) |
| Why are audio vtables missing? | They're in `.rdata` (0x820BExxx), below the code section. Ghidra extraction only covered `.text` (0x82140000+) |
| Is there a generation tool? | Not in the repository. Was likely a one-off Ghidra script + `default.bin` reader |
| How to fix audio vtables? | Option B (hook sub_821910D0) is simplest since RexGlue handles audio natively. Option A requires IDA analysis to find correct function pointers |
| Other missing vtables? | Any C++ class with vtable in `.rdata` (0x82000400 -- 0x820E6178) is potentially missing. Systematic search needed |
