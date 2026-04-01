# What Is Address 0x000F4000?

## Summary

Address `0x000F4000` is **NOT a valid code address**. It is a garbage value read from
an uninitialized or corrupted vtable entry in guest memory. It represents data that
was never meant to be executed as a function pointer.

## Address Properties

| Property | Value |
|----------|-------|
| Hex | 0x000F4000 |
| Decimal | 999,424 |
| Size | 976 KB (~0.95 MB into address space) |
| 4K page aligned | Yes (page 244) |
| 64K page aligned | No |
| In null guard (first 64KB)? | No |
| In first 1MB? | Yes |

## Xbox 360 Memory Map Context

From `glue/rexglue-sdk-main/src/system/xmemory.cpp` (lines 47-56):

```
0x00000000 - 0x3FFFFFFF (1024MB) - virtual 4k pages     <-- 0x000F4000 IS HERE
0x40000000 - 0x7FFFFFFF (1024MB) - virtual 64k pages
0x80000000 - 0x8BFFFFFF ( 192MB) - xex 64k pages        <-- game code lives here
0x8C000000 - 0x8FFFFFFF (  64MB) - xex 64k pages (encrypted)
0x90000000 - 0x9FFFFFFF ( 256MB) - xex 4k pages
0xA0000000 - 0xBFFFFFFF ( 512MB) - physical 64k pages
```

The address 0x000F4000 falls in the **v00000000 heap** (guest virtual, 4K pages,
0x00000000-0x3FFFFFFF). This is the general-purpose user virtual memory region where
`NtAllocateVirtualMemory` places allocations when the game requests pages.

### What is NOT at 0x000F4000

1. **Not the null guard page**. The null guard is only the first 64KB (0x00000000-0x0000FFFF).
   Address 0x000F4000 is 15.25x beyond the null guard boundary.

2. **Not a code address**. All XEX code lives at 0x80000000+. The recompiler's function
   table only covers `PPC_CODE_BASE` to `PPC_CODE_BASE + PPC_CODE_SIZE` (the 0x82xxxxxx
   range). `PPC_CALL_INDIRECT_FUNC` immediately fails its range check for 0x000F4000.

3. **Not a hardware/MMIO register**. On Xbox 360, MMIO lives at high physical addresses
   (GPU registers at 0xEC800000+, etc.). 0x000F4000 is in plain user virtual memory.

4. **Not a system DLL address**. Xbox 360 system DLLs (xboxkrnl.exe, xam.xex,
   xaudio2.xex) load in the 0x80000000+ kernel range, not in user virtual space.

5. **Not a hypervisor address**. The Xbox 360 hypervisor sits at 0x00010000-0x0001FFFF
   on real hardware; 0x000F4000 is well above that.

## How 0x000F4000 Arrives at the Call Site

From the generated recomp code at `gta4_recomp.2.cpp` line 2014-2024, the vtable
dispatch chain is:

```
r30         = PPC_LOAD_U32(0x831D53EC - some_offset)  // global audio device object
r3 (obj)    = PPC_LOAD_U32(r30 + 64)                  // endpoint/voice sub-object
r11 (vtable)= PPC_LOAD_U32(r3 + 0)                    // vtable pointer (first field)
r11 (method)= PPC_LOAD_U32(r11 + 68)                  // slot 17 of vtable = 0x000F4000
PPC_CALL_INDIRECT_FUNC(0x000F4000)                     // MISSING-FUNC
```

The chain requires THREE consecutive valid pointer dereferences to reach 0x000F4000:

1. **audioDevice** at global `0x831D53EC` -- loaded into r30 (valid pointer)
2. **endpoint object** at `[audioDevice + 64]` -- loaded into r3 (valid pointer)
3. **vtable pointer** at `[endpoint + 0]` -- loaded into r11 (points to SOME memory)
4. **vtable slot 17** at `[vtable + 68]` = **0x000F4000** (the bogus value)

## What 0x000F4000 Actually Is

### Most likely: Uninitialized heap data read as a vtable entry

The v00000000 heap starts at guest address 0x00000000. When the game's audio
subsystem allocates objects via `NtAllocateVirtualMemory` with 4K page granularity,
those allocations land in this range. Address 0x000F4000 (page 244) is a plausible
location for a heap allocation made early in the game's init sequence.

The value 0x000F4000 at `[vtable + 68]` means one of:

1. **The vtable itself is in low memory** (around 0x000Exxxx-0x000Fxxxx), and its
   slot 17 contains 0x000F4000. This would happen if the vtable was allocated on the
   heap but never populated with function pointers. On real Xbox 360, the vtable would
   contain addresses like 0x82xxxxxx pointing to code. On the recomp, the vtable memory
   was allocated but the constructor that writes those function pointers either:
   - Was never called (object partially constructed)
   - Wrote addresses that the recomp did not translate

2. **The vtable pointer itself is wrong**. The object at `[audioDevice + 64]` has its
   first dword pointing to some heap data (not a real vtable), and reading offset 68
   from that data happens to yield 0x000F4000.

3. **The endpoint object at `[audioDevice + 64]` was freed and reallocated**. A
   use-after-free scenario where the memory was reclaimed and overwritten with heap
   metadata or other data, and what looks like a "vtable pointer" at offset 0 now
   points into heap data.

### Why 0x000F4000 specifically?

The value `0x000F4000` as big-endian bytes is `00 0F 40 00`. This is:
- 4K-page-aligned (consistent with being heap metadata or a page address)
- Not a recognizable sentinel value (not 0xDEADBEEF, 0xCDCDCDCD, etc.)
- Not zero (so the object's vtable pointer was written SOMETHING, not left null)

This strongly suggests it is **real heap data** that was misinterpreted as a function
pointer, rather than a completely uninitialized field.

## The Audio Object in Question

The object chain comes from the **XAudio render thread worker** `sub_821910D0`:

- `0x831D53EC` holds a pointer to the global **RAGE audio device object**
- `[audioDevice + 64]` is the **IXAudioRenderDriver endpoint** (a COM-style
  interface with a vtable)
- Vtable slot 17 (offset 68 = 0x44) corresponds to the **SubmitPacket / ProcessBuffer**
  method that commits rendered audio to the hardware

On real Xbox 360, this vtable slot would point to a function inside `xaudio2.xex`
(the system audio DLL loaded by the kernel). The XAudio2 system creates COM objects
with vtables pointing into its own code.

In the recomp, the XAudio kernel exports (`XAudioRegisterRenderDriverClient`, etc.)
are implemented as host-side stubs in `xboxkrnl_audio.cpp`. These stubs handle the
high-level API but do NOT create guest-visible COM objects with valid vtables. The
game's RAGE audio engine constructs its own endpoint wrapper objects and expects
them to contain function pointers to real XAudio2 methods. Since those methods
don't exist as recompiled code, the vtable entries contain whatever garbage was
in the heap memory at allocation time.

## RexGlue Memory Layout Verification

RexGlue allocates the full 4GB guest address space as a file-backed mapping
(`xmemory.cpp` line 136-138). The v00000000 heap covers 0x00000000-0x3FFFFFFF with
4K page granularity. The first 64KB is reserved as a null guard (line 184), but
everything from 0x00010000 onward is available for guest allocations.

When `protect_zero` is false (the default), the first 64KB is mapped as
read/write (not no-access), meaning even sub-64KB addresses can be read without
faulting. This means reading the bogus vtable entry at or near 0x000F4000 does NOT
trigger any access violation -- it silently returns whatever data is in that memory.

## Connection to the Crash

The crash sequence documented in `docs/rewrite/41_log_context_transition.md`:

1. Audio thread enters `sub_821910D0`
2. Reaches vtable dispatch at 0x821911C4
3. Reads `[vtable + 68]` = 0x000F4000
4. `PPC_CALL_INDIRECT_FUNC(0x000F4000)` fails range check, hits MISSING-FUNC handler
5. MISSING-FUNC handler silently returns (no-op)
6. Return value in r3 is treated as HRESULT >= 0 (success), so execution continues
7. Audio thread outer loop calls `sub_821910D0` again -- repeats 42 times
8. Eventually triggers stack guard page fault (separate issue -- see doc 41)

## Conclusion

**0x000F4000 is heap garbage.** It is a value sitting in guest virtual memory in the
v00000000 heap (the first 1GB of the Xbox 360 address space, used for general user
allocations). It was never a valid function pointer. It ended up in a vtable slot
because the RAGE audio engine's XAudio endpoint object was constructed in guest
memory, but the vtable entries that should point to XAudio2 COM interface methods
were never populated with valid recompiled function addresses.

### Fix approaches (from doc 47)

1. **Hook `sub_821910D0`** to skip the vtable dispatch entirely -- the recomp's SDL2
   audio backend handles rendering natively, so the guest-side "submit to hardware"
   call is unnecessary
2. **Populate the vtable** at `[endpoint + 0]` with host function pointers that
   forward to the recomp's audio implementation
3. **Stub the endpoint object** so that `[audioDevice + 64]` points to a properly
   constructed object with a no-op vtable
