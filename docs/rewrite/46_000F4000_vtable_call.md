# 46: MISSING-FUNC 0x000F4000 Vtable Call Analysis

## Summary

The indirect call to `0x000F4000` from `0x821911C4` (inside `sub_821910D0`) is caused by
the XAudio render device object at `audioManager->field_64` having a corrupted or
uninitialized vtable pointer at runtime. The address `0x000F4000` is NOT a valid Xbox 360
function, hardware register, or recomp address. It is the result of dereferencing
corrupted/garbage memory through a vtable dispatch chain.

**This is NOT the root cause of the stack guard page spam.** The MISSING-FUNC handler
returns cleanly without stack corruption. However, it IS a symptom of the same underlying
problem: the XAudio render device object's vtable data in `.rdata` does not contain
function pointers -- it contains RTTI metadata structures.

## Function Identification

### sub_821910D0 -- RAGE Audio Renderer "Pump" Function

- **File**: `gta4_recomp.2.cpp` line 1888
- **Caller**: `sub_8219A2B8` (line 23787) -- thin wrapper that loads the audio manager
  global and tail-calls `sub_821910D0`
- **Called via**: indirect dispatch (function table lookup), likely from an audio thread

**What it does**:
1. Enters a critical section on `0x82B28338` (audio lock)
2. Checks `audioManager->field_304` (thread count)
3. If zero: calls `sub_8218FFB0` (audio buffer swap at raised IRQL) and `sub_82191228`
   (XAudio volume/frame submission)
4. If nonzero: signals `KeSetEvent`, waits via `KeWaitForMultipleObjects` (audio semaphore
   + shutdown event)
5. **At 0x821911B0**: loads `audioManager->field_64` (XAudio render device object),
   dereferences its vtable, calls vtable slot 17 (offset 68)
6. Leaves critical section, returns

### The Vtable Call at 0x821911C4

```
lwz  r3, 64(r30)     ; r3 = audioManager->field_64 (XAudioRenderDevice*)
lwz  r11, 0(r3)      ; r11 = *(obj+0)  -- vtable pointer
lwz  r11, 68(r11)    ; r11 = vtable[17] -- function at offset 68
mtctr r11
bctrl                 ; call -> resolves to 0x000F4000
```

## Object Construction Chain

### Audio Manager Global: `0x831D53EC`

The audio manager object pointer is stored at guest address `0x831D53EC`
(`lis r11, -31971; lwz r30, 21484(r11)` = `0x831D0000 + 0x53EC`).

**Construction** (in the parent init function at `gta4_recomp.1.cpp` line 70560+):
1. Memory allocator vtable call allocates the object (372 bytes)
2. `sub_821903B8` -- constructor, sets vtable to `0x820BE4C0`, stores object to global
3. `sub_82190B48` -- full initialization (creates audio threads, allocates sub-objects,
   registers XAudio client)

### XAudioRenderDevice at field_64

Created inside `sub_82190B48` -> `sub_82199EC8`:

1. `sub_8219A770` creates an AudioFormat descriptor on stack
2. `sub_8219A440` calls `XAudioRegisterRenderDriverClient`-like setup
3. Allocator vtable call allocates **72 bytes** for the render device object
4. `sub_8219A848` initializes base class, sets `*(obj+0) = 0x820BE808`
5. **Line 23342**: `stw r11,0(r31)` overwrites `*(obj+0) = 0x820BE768`
6. `sub_8219A098` calls `XAudioRegisterRenderDriverClient` via the render device
7. `stw r31,0(r27)` stores the object pointer into `audioManager->field_64`

### Vtable at 0x820BE768 -- THE PROBLEM

The address `0x820BE768` is in the `.rdata` section of the XEX PE image
(`.rdata`: VA `0x00000400`, size `0xE5D78`, guest range `0x82000400`-`0x820E6178`).

**Binary content at `0x820BE768`** (from `default.bin`):

| Offset | Value        | Analysis                                |
|--------|--------------|-----------------------------------------|
| +0     | `0x820BE774` | Self-referencing `.rdata` pointer (+12)  |
| +4     | `0x820E512C` | `.rdata` pointer (RTTI TypeDescriptor?)  |
| +8     | `0x00000000` | Null                                    |
| +12    | `0x82A3CB70` | .text function pointer (valid)          |
| +16    | `0x00000001` | Small integer                           |
| +20    | `0x00000000` | Null                                    |
| +24    | `0xFFFFFFFF` | Sentinel (-1)                           |
| +28    | `0x00000000` | Null                                    |
| +32    | `0x00000040` | Small integer (64)                      |
| +36    | `0x820BE758` | `.rdata` self-ref pointer               |
| ...    | ...          | ...                                     |
| **+68**| **`0x00000002`** | **Small integer, NOT a function ptr** |

This is clearly **NOT a C++ vtable** (table of function pointers). The data pattern
(self-referencing `.rdata` pointers, small integers, 0xFFFFFFFF sentinels) matches
**MSVC RTTI Class Hierarchy Descriptor** or a RAGE engine custom type metadata structure.

### Why 0x000F4000 Instead of 0x00000002?

The binary shows that `*(0x820BE768 + 68) = 0x00000002`. But the runtime log reports
`0x000F4000`. This discrepancy means **one of**:

1. **The vtable pointer in the object has been corrupted** -- the object's first DWORD
   is no longer `0x820BE768` but some other value, and `*(other_value + 68) = 0x000F4000`
2. **The object at `audioManager->field_64` is different** -- field_64 was overwritten
   with a dangling pointer or freed memory
3. **The `.rdata` section was modified at runtime** -- another part of the code wrote
   into the vtable area

Hypothesis 1 or 2 is most likely. The XAudio render device is a heap-allocated 72-byte
object. If heap corruption overwrites either the object's vtable pointer or the
audioManager's field_64 pointer, the vtable dispatch reads garbage memory.

## Impact of the MISSING-FUNC Call

### PPC_CALL_INDIRECT_FUNC Behavior for Out-of-Range Addresses

From `rex/ppc/context.h` lines 127-140:

```cpp
#define PPC_CALL_INDIRECT_FUNC(x)                                                        \
  do {                                                                                   \
    uint32_t _icf_addr = uint32_t(x);                                                   \
    bool _icf_in_range = (_icf_addr >= uint32_t(PPC_CODE_BASE) &&                       \
                          _icf_addr <  uint32_t(PPC_CODE_BASE) + uint32_t(PPC_CODE_SIZE)); \
    PPCFunc* _icf_fn = _icf_in_range ? PPC_LOOKUP_FUNC(base, _icf_addr) : nullptr;      \
    if (_icf_fn) {                                                                       \
      _icf_fn(ctx, base);                                                                \
    } else {                                                                             \
      fprintf(stderr, "[MISSING-FUNC] indirect call to %08X (in_range=%d) from %08X\n",  \
              _icf_addr, (int)_icf_in_range, uint32_t(ctx.lr));                          \
      fflush(stderr);                                                                    \
    }                                                                                    \
  } while (0)
```

When `0x000F4000` is the target:
- `0x000F4000 < PPC_CODE_BASE (0x82140000)` -- out of range
- `_icf_fn = nullptr`
- Prints `[MISSING-FUNC] indirect call to 000F4000 (in_range=0) from 821911C4`
- **Returns without modifying any registers**
- **No stack corruption, no crash, no abort**

### Post-Call Behavior

After the MISSING-FUNC return, `sub_821910D0` continues at line 2025:

```
cmpwi cr6, r3, 0     ; r3 still holds the object pointer from the lwz (some heap addr)
blt   cr6, loc_821911F0  ; branch if r3 < 0 (signed)
```

Since `r3` holds a heap pointer (positive address like `0x83xxxxxx` which is negative
in signed 32-bit), the branch IS taken (to `loc_821911F0`), skipping the atomic increment.
The function then clears `audioManager->field_300`, copies some counter array, leaves the
critical section, and returns 0. **No corruption occurs.**

### Why 42 Repetitions?

The function is called repeatedly from the audio thread. Each call:
1. Enters critical section
2. Finds `field_304 == 0` (no pending audio threads)
3. Calls the vtable function (fails with MISSING-FUNC)
4. Returns 0

The audio thread likely loops, calling this pump function ~42 times during startup before
the system either stabilizes or the thread exits. The MISSING-FUNC is **benign per-call**
but indicates the XAudio subsystem is non-functional.

## Is This the Root Cause of Stack Guard Page Spam?

**No.** The MISSING-FUNC handler returns cleanly. The stack is not corrupted by this call.

However, both the MISSING-FUNC spam and the stack guard page spam could share a common
upstream cause:

1. **Heap corruption** affecting both the audio render device vtable and stack memory
2. **Missing vtable pre-population** for `.rdata` vtables below the code range -- the
   vtable at `0x820BE768` is NOT in `vtable_prepopulate.h` (which only covers vtables
   in the code range `0x82140000+`), and the `.rdata` data does not contain valid
   function pointers
3. **The XAudio render device creation path** involves multiple allocator vtable calls
   that may themselves hit MISSING-FUNC failures, returning garbage objects

## Address Analysis: 0x000F4000

| Check | Result |
|-------|--------|
| Valid Xbox 360 code address? | No (user space starts at 0x10000, code at 0x82000000+) |
| Xbox 360 hardware register? | No (hardware MMIO at 0xC0000000+, 0xExxxxxxx) |
| In recomp code range? | No (code: 0x82140000 - 0x82A8635C) |
| In recomp image range? | No (image: 0x82000000 - 0x83300000) |
| Found as a constant in binary? | Only 3 times, all unaligned in .data (part of other data) |
| Likely source | Corrupted/uninitialized memory dereference |

## Root Cause: Missing RDATA Vtable Entries

The `.rdata` section at `0x820BE768` contains RTTI metadata, not function pointers. On the
original Xbox 360, MSVC stores vtables in `.rdata` as arrays of function pointers, with RTTI
descriptors nearby but at different addresses. The vtable address `0x820BE768` set by the
constructor likely points to the wrong location in the recomp -- the RTTI descriptor instead
of the actual function table.

**The vtable pre-population system (`vtable_prepopulate.h`) only covers vtables in the code
range (0x82140000+).** Vtables in `.rdata` (0x820xxxxx) are loaded from `default.bin` but
the data at those addresses is RTTI metadata, not function pointer arrays.

## Recommended Fix

The XAudioRenderDevice vtable at `0x820BE768` needs to be identified and pre-populated with
the correct function pointers, OR the audio subsystem needs a hook that bypasses this vtable
call entirely. Options:

1. **Hook `sub_821910D0`** to skip the vtable call at `0x821911C4` -- return 0 (success)
   for the "get render frame count" call, since the XAudio system is handled by RexGlue's
   SDL audio layer anyway

2. **Identify the correct vtable entries** from the XEX's `.rdata` section and add them to
   `vtable_prepopulate.h` or `memory.cpp`'s manual vtable section

3. **Hook `sub_82199EC8`** (XAudioRenderDevice factory) to return a stub object with a
   properly-populated vtable that dispatches to no-op functions

Option 1 is simplest and most robust since the audio rendering is already handled by
RexGlue's native audio pipeline.

## Key Files

- Recomp code: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.2.cpp` (line 1888+)
- PPC_CALL_INDIRECT_FUNC: `glue/rexglue-sdk-main/include/rex/ppc/context.h` (line 127)
- Vtable prepopulation: `gta_iv/vtable_prepopulate.h`
- Manual vtables: `LibertyRecomp/kernel/memory.cpp` (line 97+)
- XAudio kernel impl: `glue/rexglue-sdk-main/src/kernel/xboxkrnl/xboxkrnl_audio.cpp`
- XEX binary: `gta_iv/default.bin`
- Audio manager global: guest address `0x831D53EC`
- XAudioRenderDevice vtable (broken): guest address `0x820BE768` (in `.rdata`)
