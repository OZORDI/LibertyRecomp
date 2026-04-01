# Research: VBlank-FIX Stub at 0x12BD0000

## 1. Where the Message Is Printed

**File**: `/Users/Ozordi/Downloads/LibertyRecomp/LibertyRecomp/kernel/imports.cpp`, line 616

The message is printed inside `PPC_FUNC_HOOK(sub_82A487B8)` (lines 601-632), which is
a hook on the Xbox 360 VBlank interrupt callback function. The hook:

1. Reads `ctx.r4` as `userData` (the GPU device object pointer, passed via
   `VdSetGraphicsInterruptCallback`)
2. Reads `device[+10900]` (big-endian u32 at `base + userData + 10900`)
3. If that slot is zero, allocates a 64-byte zero-filled guest stub via
   `mem->SystemHeapAlloc(64)` and writes the stub address back into `device[+10900]`
4. Calls the original recompiled function `__imp__sub_82A487B8(ctx, base)`

## 2. What Is device[+10900]?

Offset 10900 (decimal) = **0x2A94** into the Xbox 360 D3D device object.

The device object is the global GPU device pointer, originally set via
`VdGlobalDevice` (kernel export ordinal 0x1BE). The game passes it as `userData`
to `VdSetGraphicsInterruptCallback`, which registers the VBlank interrupt.

Per the code comments at line 590-597:

> The VBlank callback reads device[+10900] and dereferences *(device[+10900]+16)
> to find the frame_done callback. If device[+10900]==0, this is a null deref.

So `device[+0x2A94]` is a **pointer to a frame-done callback descriptor** within
the Xbox 360 D3D device. On real hardware, this would point to a GPU fence/callback
structure. Offset +16 within that structure is the actual function pointer for
the "frame done" callback.

There is also a paired field at `device[+10896]` (0x2A90) logged by the GPU-INIT
diagnostic hook on sub_82A49D08 (line 1037). Both fields are populated by the
GPU initialization path (sub_82A49D08). When GPU init fails to allocate memory
(because `XMemAlloc` returns 0), both remain NULL -- which is exactly what
triggers the VBlank-FIX.

## 3. What Does the Stub at 0x12BD0000 Do?

The stub is a **64-byte block of zeroed guest memory**. It is not a function and
not a vtable. Its purpose is purely structural:

- The recompiled VBlank code (`__imp__sub_82A487B8`) reads `*(device[+10900] + 16)`
  to get the frame_done callback function pointer
- If device[+10900] is NULL, this dereference crashes (null pointer + 16)
- By writing a zero-filled stub address into device[+10900], the dereference
  becomes `*(stub + 16) = 0`, which the recompiled code handles gracefully
  (it checks for NULL and skips the callback call)
- The VBlank code still executes the spinlock-clear at loc_82A48810, which
  signals the main game thread to continue

The stub is allocated once (static `s_vblankStubAddr`) and reused for all
subsequent VBlank interrupts.

## 4. Could This Stub Affect the XAudio Endpoint Vtable?

**No -- they are unrelated objects in unrelated subsystems.**

The VBlank-FIX stub is written to `device[+10900]` on the **GPU D3D device object**.
The XAudio endpoint vtable corruption (0x000F4000 calls from 0x821911C4) is in the
**audio subsystem**:

- sub_821910D0 loads an audio context object from `[0x831D53EC]`
- It reads `obj->field_64` (the "render object"), then `*(renderObj)->vtable[17]`
- The corrupted vtable entry contains 0x000F4000

These are completely separate object hierarchies:
- GPU device at whatever address `userData` holds (populated by VdSetGraphicsInterruptCallback)
- Audio device at `[0x831D53EC]` (populated by the audio initialization path)

The VBlank-FIX does NOT write to any audio memory, does NOT modify any vtable,
and does NOT touch the address 0x000F4000.

## 5. Is 0x12BD0000 in a Valid Guest Memory Range?

**Yes.** The allocation comes from `SystemHeapAlloc(64)` with default flags
(`kSystemHeapDefault = kSystemHeapVirtual`).

For non-physical, 4KB page-size allocations, `LookupHeapByType` returns
`heaps_.v00000000`, which covers the guest virtual address range
**0x00000000 - 0x3FFFFFFF** (1GB).

Address 0x12BD0000 falls within this range at ~315MB into the virtual heap.
This is the standard "system heap" used by the kernel for internal structures
(kernel objects, export variable storage, etc.). It is NOT the game's own
heap (which lives in 0x82000000+ for code and 0xA0000000+ for physical).

The allocation is valid, properly committed, and zero-filled.

## 6. Timing Relationship: Does VBlank-FIX Trigger the Audio Vtable Calls?

**The VBlank-FIX is a timing coincidence, not a causal trigger.**

The sequence in the log:

| Line | Event |
|------|-------|
| 1196-81779 | ~80,000 lines of MISSING-FUNC spam (null vtable calls from renderer + game tick) |
| 81780 | `[VBlank-FIX] Allocated stub at guest 0x12BD0000 for device[+10900]` |
| 81781-81822 | 42x `MISSING-FUNC indirect call to 000F4000` from 0x821911C4 |
| 81823 | First stack guard page hit at 0x70000000 |

The VBlank-FIX occurs on the **GPU VSync thread** ("GPU VSync", created by
`StartVBlankTimer`), which fires every 16.67ms. The 0x000F4000 calls come from
**sub_821910D0** on the audio render thread.

The VBlank-FIX and the 0x000F4000 burst are on **different threads**. The VBlank
thread runs sub_82A487B8 which patches device[+10900] and executes spinlock
operations -- none of which touch the audio object at 0x831D53EC or its vtable.

The apparent temporal correlation is because:
1. The VBlank-FIX triggers on the first VBlank interrupt where device[+10900]==0
   (i.e., the first VBlank after GPU init failed to populate the field)
2. The audio vtable corruption has been building since audio init
3. Both happen to manifest around the same time (~80,000 log lines into execution)
   because that is when the game's subsystem initialization reaches the point
   where both GPU frame callbacks and audio render dispatch are active

**The VBlank-FIX does not cause, enable, or unblock the 0x000F4000 calls.**

## 7. Summary of device + 10900 / 0x2A94 References

All references to offset 10900 in the codebase:

| Location | Usage |
|----------|-------|
| imports.cpp:590-597 | Comment explaining the VBlank callback's use of device[+10900] |
| imports.cpp:601-632 | `PPC_FUNC_HOOK(sub_82A487B8)` -- the VBlank-FIX hook itself |
| imports.cpp:1031-1038 | GPU-INIT diagnostic in sub_82A49D08 hook -- logs device[+10896] and device[+10900] after GPU init |
| imports.cpp:1068-1070 | Comment: explains that failed XMemAlloc leaves device[+10896] and device[+10900] NULL |

The root cause of device[+10900] being NULL is documented at line 1063-1074:
`sub_82A10EB0` (the RAGE heap allocator for GPU/physical memory) calls
`sub_82A18920` which calls `XMemAlloc` -- a stub returning 0. This prevents
the GPU device from allocating its internal frame-done callback structure,
leaving device[+10900] permanently NULL until the VBlank-FIX patches it.

## 8. Conclusions

1. **The VBlank-FIX is a correct and necessary workaround** for a missing XMemAlloc
   implementation. Without it, the VBlank callback would null-deref and crash.

2. **It has zero relationship to the XAudio 0x000F4000 vtable corruption.** Different
   thread, different object, different subsystem, different memory region.

3. **0x12BD0000 is a valid system heap address** in the v00000000 virtual heap
   (0x00000000-0x3FFFFFFF). It is not in the game code range, not in the stack
   range, and not near 0x000F4000.

4. **The temporal proximity in the log is coincidental.** Both symptoms appear
   around the same time because multiple subsystems reach their failure points
   simultaneously as the game completes initialization and begins active rendering.

5. **0x000F4000 is NOT related to the stub at 0x12BD0000.** The value 0x000F4000
   (~1MB) sits in the low portion of the v00000000 heap, above the 64KB
   null-protection zone (0x0000-0xFFFF). It is likely uninitialized memory that
   was read as a function pointer from a corrupted audio object vtable.
