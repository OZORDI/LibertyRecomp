# XAudio Endpoint Vtable Initialization Research

## Problem

In `sub_821910D0` (the XAudio render thread worker), the vtable dispatch at lines 2014-2024 of `gta4_recomp.2.cpp` reads a corrupted vtable entry:

```
r3  = PPC_LOAD_U32(r30 + 64)     // endpoint object from audio device
r11 = PPC_LOAD_U32(r3 + 0)       // vtable pointer
r11 = PPC_LOAD_U32(r11 + 68)     // slot 17 (offset 0x44) = 0x000F4000
PPC_CALL_INDIRECT_FUNC(r11)      // MISSING-FUNC: 0x000F4000 out of code range
```

## 1. What r30 Points To

`r30` is the global RAGE audio device object. It is loaded from address `0x831D53EC` (computed as `0x831D0000 + 21484`).

`sub_8219A2B8` (line 23787) is the thin wrapper/thread entry that loads this global and tail-calls `sub_821910D0`:

```
r11 = 0x831D0000       // lis r11,-31971
r3  = [r11 + 21484]    // lwz r3,21484(r11) -> [0x831D53EC]
sub_821910D0(r3)        // tail call
```

## 2. All Writes to 0x831D53EC

Three locations in the generated code store to `[0x831D53EC]`:

### Store 1: `sub_821903B8` constructor (gta4_recomp.2.cpp line 104-105)

The audio device constructor. After populating the object's internal fields (linked lists at offsets 68/80/92/104, volume at offsets 132/136), stores the `this` pointer to the global:

```
// line 62-63: store vtable 0x820BE4C0 at [r31+0]
stw r10, 0(r31)        // r10 = 0x820BE4C0 (audio device vtable)
...
// line 104-105: register as global audio device
stw r31, 21484(r11)    // [0x831D53EC] = this
```

**Audio device vtable**: `0x820BE4C0` (computed from `lis -32244; addi -6976`).

### Store 2: Destructor path (gta4_recomp.2.cpp line 766-767)

In the audio device destructor/reset function around line 740+. After releasing the endpoint at offset 64 via `[obj+0][12](obj)` (virtual destructor), stores `r22` (which is 0) to the global:

```
stw r22, 64(r23)       // clear endpoint pointer
stw r11, 21484(r10)    // [0x831D53EC] = r22 (likely null or replacement)
```

### Store 3: `sub_821901D8` initializer (gta4_recomp.1.cpp line 70612-70613)

The full audio system initializer. After calling `sub_8219A440` (COM factory) and `sub_821903B8` (constructor), `sub_82190B48` (endpoint setup), stores the constructed object to the global if initialization succeeded:

```
stw r31, 21484(r11)    // [0x831D53EC] = newly constructed audio device
```

## 3. How the Endpoint Object at [audioDevice + 64] Gets Constructed

### Call chain

```
sub_821901D8
  -> sub_821902E0  (allocate/setup audio device)
  -> sub_8219A440  (COM factory: tag 0x61820005 = XAudioRegisterRenderDriverClient)
  -> sub_821903B8  (audio device constructor: stores vtable 0x820BE4C0)
    -> sub_82190488
      -> sub_821904B8
        -> sub_82190B48  (full audio subsystem init)
```

### sub_82190B48 (gta4_recomp.2.cpp line 1108)

This is the main audio subsystem initialization function. At the END (lines 1664-1693), it creates the endpoint:

```
r31 = r29 + 64              // &audioDevice[64] -- pointer to endpoint slot
r3  = [r24 + 8]             // params from caller
r4  = r31                   // output pointer for endpoint
sub_82199EC8(r3, r4)        // CREATE THE ENDPOINT OBJECT
```

After `sub_82199EC8` returns, the endpoint pointer is stored at `[audioDevice + 64]`.

Then immediately (lines 1677-1693), it reads the endpoint and calls through it:

```
r11 = [r31 + 0]             // [endpoint + 0] = vtable (should be 0x820BE768)
r3  = [r11 + 68]            // [vtable + 68] = slot 17 function pointer
r11 = [r3 + 0]              // dereference through the returned sub-object
r11 = [r11 + 28]            // sub-object vtable slot 7
call r11                     // SetVolume or similar
```

**Note**: The init function at line 1683-1684 does `r3 = [r11+68]` which reads vtable slot 17 as an OBJECT POINTER, then dereferences through it (`[r3+0]`, `[r11+28]`). This is NOT the same call pattern as sub_821910D0 (which calls slot 17 directly). This means vtable[17] is expected to be a POINTER TO AN OBJECT, not a function pointer -- at least in the init code path. However, in sub_821910D0 (line 2020-2024), it calls slot 17 directly as a function.

Actually, re-reading sub_821910D0 lines 2014-2024 more carefully:

```
r3     = [r30 + 64]          // endpoint object
r11    = [r3 + 0]            // endpoint vtable
r11    = [r11 + 68]          // vtable[17]
ctr    = r11
bctrl                        // call vtable[17](endpoint)
```

This IS a direct call to vtable[17]. The function at vtable[17] should return an HRESULT (checked with `blt cr6` at line 2027).

### sub_82199EC8 (gta4_recomp.2.cpp line 23207)

This is the endpoint object factory. Key steps:

1. **Calls `sub_8219A770`** (line 23270) -- calculates total allocation size needed
2. **Calls `sub_8219A440`** (line 23297) with tag `0x61820006` -- COM factory, allocates a 72-byte object via allocator vtable[5] (allocate method at vtable+20)
3. **Calls `sub_8219A848`** (line 23331) -- basic endpoint init
4. **Stores vtable 0x820BE768** at `[endpoint + 0]` (line 23342-23343):
   ```
   r11 = 0x820C0000 + (-6296) = 0x820BE768
   stw r11, 0(r31)    // endpoint->vtable = 0x820BE768
   ```
5. **Calls `sub_8219A098`** (line 23344-23346) with `(endpoint, params)` -- detailed endpoint init
6. On success, stores endpoint pointer at `[r27]` which is `[audioDevice + 64]` (line 23353-23354)

### sub_8219A098 (gta4_recomp.2.cpp line 23476)

This initializes the endpoint's internal state:

1. Calls `sub_8219AA18` (line 23538) -- sets up XAudio format descriptors (48000 Hz, 6 channels)
2. Allocates a 28-byte sub-object via allocator vtable[5] (line 23559-23567, size=28)
3. **Stores two vtable pointers** in the sub-object (lines 23588-23597):
   ```
   r10 = 0x820C0000 + (-6676) = 0x820BE5CC   // stored at [sub+0]
   r9  = 0x820C0000 + (-6632) = 0x820BE5F8   // stored at [sub+4]
   ```
4. **Stores sub-object at `[endpoint + 68]`** (line 23610-23611):
   ```
   stw r3, 68(r30)    // endpoint->renderState = sub-object
   ```

## 4. What Xbox 360 Interface Has Method 17 at Offset 68?

The endpoint vtable at `0x820BE768` is **NOT** an Xbox 360 system DLL interface. It is a **RAGE engine class vtable** stored in the game's own .rdata section.

On Xbox 360, XAudio2 COM interfaces are:

| Interface | Method count | Notes |
|-----------|-------------|-------|
| IXAudio2 | ~12 methods | Main engine, CreateMasteringVoice/CreateSourceVoice |
| IXAudio2MasteringVoice | ~5 methods (inherited from IXAudio2Voice) | DestroyVoice, SetVolume, etc. |
| IXAudio2SourceVoice | ~10 methods | Start, Stop, SubmitSourceBuffer, etc. |

None of these have exactly 17+ methods accessed at offset 68. The object at `[audioDevice + 64]` is RAGE's `audRenderEndpoint` class (or similar), wrapping the XAudio2 render driver.

The vtable at 0x820BE768 belongs to the game code (0x820xxxxx range, within .rdata below code at 0x82140000). Method 17 at `[vtable + 68]` should be a game function pointer like `0x8219xxxx`.

## 5. RexGlue XAudio Implementation

RexGlue provides kernel-level XAudio exports in `glue/rexglue-sdk-main/src/kernel/xboxkrnl/xboxkrnl_audio.cpp`:

**Implemented (with real logic)**:
- `XAudioGetSpeakerConfig` -- returns 0x00010001 (stereo)
- `XAudioGetVoiceCategoryVolumeChangeMask` -- returns 0 (no changes)
- `XAudioGetVoiceCategoryVolume` -- returns 1.0f
- `XAudioEnableDucker` -- no-op success
- `XAudioRegisterRenderDriverClient` -- registers client with AudioSystem, returns 0x4155xxxx handle
- `XAudioUnregisterRenderDriverClient` -- unregisters client
- `XAudioSubmitRenderDriverFrame` -- submits PCM frame to SDL2 backend

**Stubbed (no-op, return 0)**:
- `XAudioRenderDriverInitialize`
- `XAudioRenderDriverLock`
- `XAudioSetVoiceCategoryVolume`
- Plus ~15 more (ducker, diagnostics, MEC, etc.)

RexGlue does NOT create any guest-visible COM objects or vtables. The `XAudioRegisterRenderDriverClient` returns a synthetic handle (0x4155xxxx), not an object pointer. The game's RAGE engine builds its own wrapper objects around this handle.

## 6. Where 0x000F4000 Comes From

### The vtable at 0x820BE768 is in the game's .rdata

Address `0x820BE768` is at offset `0xBE768` from the image base (`0x82000000`). The XEX binary is encrypted (AES) and basic-compressed. After decryption and decompression by `XexModule::ReadImageBasicCompressed`, the .rdata content is written to guest memory.

The raw XEX file at offset `header_size + 0xBE768` contains encrypted ciphertext (verified: `0xFEDDB4AD, 0x3B3257A7, ...`). The decrypted content should contain valid game function pointers (0x82xxxxxx).

### Root cause analysis

**The value 0x000F4000 at vtable slot 17 has three possible explanations:**

1. **Decryption/decompression bug**: The .rdata region around 0x820BE768 was not correctly decrypted. A partial decryption failure would produce values that look like they COULD be addresses but are actually ciphertext residue or corrupted data. The value 0x000F4000 being page-aligned is suspicious but could be coincidental.

2. **The vtable was correctly loaded but slot 17 is a data field, not a function pointer**: In some MSVC class layouts, vtables can contain type metadata or RTTI pointers mixed with function pointers. If slot 17 is actually a data field (e.g., an interface pointer or size constant), then 0x000F4000 could be a legitimate value that should not be called. However, the code in sub_821910D0 calls it as a function, making this unlikely.

3. **The endpoint object was replaced/overwritten after construction**: The initialization in sub_82199EC8 stores vtable 0x820BE768 correctly, but between construction and the render thread's first call to sub_821910D0, the object at `[audioDevice + 64]` was replaced with a different object (perhaps via the destructor path at store 2), and the replacement has a different vtable or corrupted data.

### Most likely cause: Explanation 3

The evidence points to the endpoint object being replaced or partially destroyed:

- Store 2 (destructor, line 766-767) explicitly nulls `[audioDevice + 64]` and then writes a new value to the global `0x831D53EC`
- The game's audio system goes through multiple init/teardown cycles during startup
- If the render thread starts before the final endpoint is fully constructed, it reads a stale or intermediate object

Alternatively, the endpoint may never have been constructed at all if `sub_82199EC8` failed early (e.g., the COM factory `sub_8219A440` with tag 0x61820006 failed), leaving `[audioDevice + 64]` pointing to whatever was in memory (heap allocation with value 0x000F4000 as metadata).

## 7. XAudio2Create / Audio Device Initialization Function

The audio device initialization chain is:

```
sub_821901D8(r3=this, r4=params)        // entry point
  sub_821902E0(r3=this, r4=&stack_var)  // allocate/prepare
  sub_8219A440(r3=0x61820005, r4=allocation_result, r5=&output)
                                         // COM factory: XAudioRegisterRenderDriverClient
  sub_821903B8(r3=output, r4=xaudio_iface)
                                         // constructor: stores vtable 0x820BE4C0
    sub_82190B48(r3=audioDevice, r4=format_params)
                                         // full init: threads, semaphores, endpoint
      sub_82199EC8(r3=endpoint_params, r4=&audioDevice[64])
                                         // endpoint factory: vtable 0x820BE768
```

The `sub_8219A440` function (line 24024) is a generic COM-like allocator. It:
1. Calls `[obj+0][0](obj)` -- AddRef on the allocator
2. Calls `[obj+0][8](obj, format_params)` -- SetFormat
3. Calls `[obj+0][12](obj, tag)` -- CreateInstance with tag (0x61820005 or 0x61820006)

These are calls through the **memory allocator's vtable** (the object at `[endpoint + 8]` in sub_82190B48). The allocator is a RAGE heap manager, NOT an XAudio2 interface.

There is no explicit `XAudio2Create` call in the game code. Xbox 360 games access XAudio through kernel exports (`XAudioRegisterRenderDriverClient`, etc.) rather than COM creation. The RAGE engine wraps these kernel-level APIs in its own class hierarchy.

## Summary of Findings

| Item | Finding |
|------|---------|
| **r30 (audio device)** | Global at 0x831D53EC, object with vtable 0x820BE4C0 |
| **[r30+64] (endpoint)** | Created by sub_82199EC8, vtable 0x820BE768 |
| **Vtable 0x820BE768** | In game .rdata at offset 0xBE768, RAGE engine class |
| **Slot 17 (offset 68)** | Should be a 0x82xxxxxx function pointer; reads as 0x000F4000 |
| **0x000F4000** | Not a valid code address; likely heap data or failed init |
| **XAudio2Create** | Not used; game uses kernel XAudio exports wrapped by RAGE |
| **RexGlue XAudio** | Provides kernel stubs; does NOT create guest COM objects |
| **XAudioRenderDriverInitialize** | STUBBED (no-op) in RexGlue |
| **Root cause** | Endpoint object at [audioDevice+64] either (a) never fully constructed because sub_82199EC8 failed silently, or (b) was replaced/overwritten between construction and first render thread use |

### Key vtable addresses in .rdata

| Address | Used for | Stored at |
|---------|----------|-----------|
| 0x820BE4C0 | Audio device vtable | `[audioDevice + 0]` |
| 0x820BE4D4 | Audio device vtable (variant) | sub_821903B8 line 23 |
| 0x820BE768 | Endpoint vtable | `[endpoint + 0]` |
| 0x820BE5CC | Endpoint render-state sub-object vtable | `[renderState + 0]` |
| 0x820BE5F8 | Endpoint render-state sub-object vtable2 | `[renderState + 4]` |

### Recommended investigation

1. Add runtime logging in `sub_82199EC8` to verify it actually completes successfully and stores a valid endpoint pointer at `[audioDevice + 64]`
2. Add a watchpoint on `[audioDevice + 64]` to detect if/when the endpoint pointer changes between construction and the first call from sub_821910D0
3. Verify .rdata integrity by dumping the decrypted content at 0x820BE768 + 68 at runtime (immediately after XEX load, before any game code runs) to confirm whether the vtable data is correct in memory
