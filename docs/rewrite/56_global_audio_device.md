# 56: Global Audio Device at 0x831D53EC -- Full Initialization Research

## 1. Global Location

Address `0x831D53EC` is in the `.bss`/`.data` range (0x82A8xxxx-0x833xxxxx). It holds a pointer to the RAGE audio device object ("audDevice"). Computed as `lis r11,-31971` (0x831D0000) + `addi/lwz 21484` (0x53EC).

## 2. Three Stores to 0x831D53EC

### Store 1: `sub_821903B8` -- Base Constructor (gta4_recomp.2.cpp:5-116)

```
sub_821903B8(r3=this, r4=inner_unk):
  [this+0] = 0x820BE4D4   (base class vtable, overwritten to 0x820BE4C0)
  [this+4] = 1             (refcount)
  [this+8] = r4            (inner/owner IUnknown)
  [this+68..79]  = self-referencing linked list node (priority 24)
  [this+80..91]  = self-referencing linked list node (priority 16)
  [this+92..103] = self-referencing linked list node (priority 24)
  [this+116,120] = list cross-links
  [this+132] = 1.0f (0x3F800000) -- master volume
  [this+136] = 1.0f -- second volume
  [0x831D53EC] = this       // <-- GLOBAL STORE
```

**Note**: Offset 64 is NOT initialized by this constructor. Only offsets 0, 4, 8, 68-136 are written.

### Store 2: `sub_821901D8` -- Top-Level Audio Init (gta4_recomp.1.cpp:70501)

```
sub_821901D8(r3=device, r4=config):
  sub_821902E0()                      // allocate device memory
  sub_8219A440(tag=0x61620005, &ptr)  // CoCreateInstance-like factory
  [vtable+20](ptr, 372)              // init 372-byte device object
  sub_82190B48(device, config)        // FULL device initialization
     -> ExCreateThread(sub_8219A2B8)  // creates audio render thread
     -> sub_82199EC8(config, &device[64])  // CREATE ENDPOINT (sets offset 64!)
  if (status >= 0):
    [0x831D53EC] = device   // <-- GLOBAL STORE (redundant, already set by sub_821903B8)
```

### Store 3: `sub_821904B8` -- Destructor/Reset (gta4_recomp.2.cpp:154-886)

```
sub_821904B8(r3=device):
  ...
  // Release endpoint at [device+64]:
  r3 = [device+64]
  if (r3 != 0):
    [vtable+12](r3)         // Release() -- vtable[3] is destructor
    [device+64] = 0          // clear endpoint
  [0x831D53EC] = 0           // <-- GLOBAL STORE (clears to NULL)
```

## 3. Object Structure at 0x831D53EC

The audio device object is 372+ bytes allocated via COM-like factory `sub_8219A440`:

| Offset | Size | Contents | Set By |
|--------|------|----------|--------|
| 0 | 4 | Vtable pointer (0x820BE4C0) | sub_821903B8 |
| 4 | 4 | Ref count (initially 1) | sub_821903B8 |
| 8 | 4 | Inner/owner IUnknown pointer | sub_821903B8 |
| 12 | 1 | Type/flags byte | sub_8219A848 |
| 16-23 | 8 | Linked list node | sub_8219A848 |
| 24-31 | 8 | Linked list node | sub_8219A848 |
| 40 | 4 | Zero | sub_8219A848 |
| **60** | **4** | **Driver sub-object pointer** | **sub_82190B48 (via sub_821924D0)** |
| **64** | **4** | **Audio endpoint interface** | **sub_82199EC8** |
| 68-79 | 12 | Linked list node (priority 24) | sub_821903B8 |
| 80-91 | 12 | Linked list node (priority 16) | sub_821903B8 |
| 92-103 | 12 | Linked list node (priority 24) | sub_821903B8 |
| 116 | 4 | List cross-link -> offset 92 | sub_821903B8 |
| 120 | 4 | List cross-link -> offset 104 | sub_821903B8 |
| 124 | 4 | Buffer array pointer | sub_82190B48 |
| 128 | 1 | Voice count | sub_82190B48 |
| 132 | 4 | Master volume (1.0f) | sub_821903B8 |
| 136 | 4 | Second volume (1.0f) | sub_821903B8 |
| 140 | 4 | Voice category change mask (runtime) | sub_82191228 |
| 300 | 4 | Current processing thread ID | sub_821910D0 |
| 304 | 4 | Pending work/thread count | sub_82190B48 |
| 308-331 | 24 | Thread handle array (6 slots x 4) | sub_82190B48 |

## 4. Offset 64: The Audio Endpoint Interface

### What It Is

Offset 64 is a pointer to RAGE's **XAudio render endpoint interface** -- a COM-like object that wraps the Xbox 360 XAudio render driver. This is NOT `IXAudio2` or `IXAudio2MasteringVoice`. It is a RAGE-specific interface.

### How It's Created

The endpoint is created by `sub_82199EC8(config, &device[64])`:

```
sub_82199EC8(r3=config, r4=&device[64]):
  sub_8219A770(config, &driver)            // Step 1: create XAudio render driver
  sub_8219A440(tag=0x61620006, &endpoint)  // Step 2: allocate 72-byte endpoint
  sub_8219A848(endpoint, driver, 2)        // Step 3: base class init (sets vtable, refcount)
  [endpoint+0] = 0x820BE768               // Step 4: set final vtable (OVERWRITES base)
  sub_8219A098(endpoint, config)           // Step 5: create output chain
  device[64] = endpoint                    // Step 6: store in audio device
```

### Endpoint Object Layout (72 bytes)

| Offset | Contents | Set By |
|--------|----------|--------|
| 0 | Vtable pointer (0x820BE768 at construction time) | sub_82199EC8 |
| 4 | Ref count (1) | sub_8219A848 |
| 8 | Driver/inner object pointer | sub_8219A848 |
| 12 | Type byte (2) | sub_8219A848 |
| 16-23 | Linked list node | sub_8219A848 |
| 24-31 | Linked list node | sub_8219A848 |
| 40 | Zero | sub_8219A848 |
| **68** | **Output chain object** | **sub_8219A098** |

### Vtable at 0x820BE768 -- Binary Analysis

Reading the vtable from `gta_iv/default.bin` at file offset 0xBE768:

| Offset | Binary Value | Interpretation |
|--------|-------------|----------------|
| 0 | 0x820BE774 | Points into .rdata (to 0x82A3CB70 = likely QueryInterface thunk) |
| 4 | 0x820E512C | .text function (AddRef?) |
| 8 | 0x00000000 | NULL |
| 12 | 0x82A3CB70 | CRT function (Release) |
| 16 | 0x00000001 | NOT a function -- this is class metadata |
| ... | ... | Mixed data and pointers |
| **68** | **0x00000002** | **NOT a function pointer** |

**The binary data at `[vtable+68]` is 0x00000002, not 0x000F4000.** This means:
- The .rdata at 0x820BE768 is NOT a standard flat vtable
- It contains a mix of function pointers (offsets 0, 4, 12) and class metadata
- The value at runtime (0x000F4000) differs from the static binary (0x00000002)
- Something modifies this memory region between load and crash

## 5. Why vtable[68] = 0x000F4000 at Runtime (not 0x00000002)

Three possible explanations:

### Theory A: Dynamic vtable (most likely)

The data structure at 0x820BE768 is NOT a vtable. It's a **COM class descriptor / RTTI-like metadata block**. The REAL vtable is established dynamically by the constructor chain. On Xbox 360, the XEX loader patches import thunks into .rdata, or the constructors build the vtable at runtime. In the recomp, neither happens correctly:

1. The binary .rdata is loaded as-is with metadata values (0x00000001, 0x00000002, 0xFFFFFFFF)
2. The constructor `sub_82199EC8` stores 0x820BE768 at `[endpoint+0]` but does NOT populate the entries
3. On original hardware, the XEX loader would have resolved import entries, or additional initialization code fills in the proper function pointers
4. In the recomp, those entries remain as raw binary data (0x00000002)
5. Some other recompiled code or runtime initialization overwrites 0x820BE7AC from 0x00000002 to 0x000F4000

### Theory B: Heap corruption

The endpoint object (72 bytes) is heap-allocated. If another object's write overflows into this region, the vtable pointer at `[endpoint+0]` could be overwritten to point elsewhere, where `[new_ptr+68]` happens to contain 0x000F4000.

### Theory C: The endpoint was never constructed

If `sub_82199EC8` fails partway through (e.g., `sub_8219A770` fails to create the XAudio driver, or `sub_8219A440` returns NULL), then `device[64]` might never be set, or is set to a partially-initialized object. The "vtable pointer" read from `[endpoint+0]` would be whatever was in the heap allocation, and `[heap_garbage+68]` = 0x000F4000.

## 6. Is There an XAudio2Create Call?

**No.** GTA IV on Xbox 360 does NOT use XAudio2. It uses the **low-level Xbox 360 XAudio kernel API**:

| Function | Address | Description |
|----------|---------|-------------|
| `XAudioRegisterRenderDriverClient` | 0x82A75804 | Register audio callback + get driver handle |
| `XAudioSubmitRenderDriverFrame` | 0x82A757E4 | Submit mixed audio frame |
| `XAudioUnregisterRenderDriverClient` | 0x82A757F4 | Unregister client |
| `XAudioGetVoiceCategoryVolumeChangeMask` | 0x82A75774 | Check volume changes |
| `XAudioGetVoiceCategoryVolume` | 0x82A75794 | Get current volume |
| `XAudioGetSpeakerConfig` | 0x82A74DB4 | Get speaker layout |

These are all intercepted by RexGlue in `glue/rexglue-sdk-main/src/kernel/xboxkrnl/xboxkrnl_audio.cpp` and the local overrides in `LibertyRecomp/apu/audio.cpp`.

`XAudioRegisterRenderDriverClient` returns a synthetic driver handle `0x41550000 | index` (from `xboxkrnl_audio.cpp` line 69). This handle is stored at `[[device+64]+68]+24` (the driver sub-object inside the endpoint).

## 7. The Vtable Dispatch in sub_821910D0

```
// gta4_recomp.2.cpp lines 2014-2024
r3  = PPC_LOAD_U32(r30 + 64)     // r30 = audio device; r3 = endpoint
r11 = PPC_LOAD_U32(r3 + 0)       // r11 = [endpoint+0] = vtable ptr
r11 = PPC_LOAD_U32(r11 + 68)     // r11 = vtable[17] = ???
PPC_CALL_INDIRECT_FUNC(r11)      // CALL -- should be ProcessFrame/RenderOutput
```

On Xbox 360, vtable[17] at offset 68 should contain the address of the audio endpoint's **output rendering method** -- the function that commits a mixed audio frame to the hardware output. This is RAGE's equivalent of calling `IXAudio2SourceVoice::SubmitSourceBuffer` in PC XAudio2.

## 8. Full Initialization Call Chain

```
sub_821901D8(device_buf, config)             // TOP-LEVEL AUDIO INIT
  |
  +-> sub_821902E0(config, &stack[80])       // Create render driver wrapper
  |     Returns HRESULT + object in stack[80]
  |
  +-> sub_8219A440(0x61620005, &result)      // COM factory: allocate 372-byte device
  |     [vtable+20](result, 372)             // Placement init
  |
  +-> sub_82190B48(device, config)           // FULL DEVICE INIT
  |     |
  |     +-> sub_821903B8(device, inner)      // Base class constructor
  |     |     Stores device at 0x831D53EC (FIRST global store)
  |     |
  |     +-> sub_821924D0(mixer, format, allocator)  // Create mixer object
  |     |     [device+60] = mixer+8
  |     |
  |     +-> KeInitializeSemaphore(...)       // Audio semaphore at 0x831D52F0
  |     +-> ExRegisterTitleTerminateNotification(...)
  |     +-> [device+304] = 0                 // Clear pending work count
  |     |
  |     +-> LOOP (6 iterations):             // Create 6 audio worker threads
  |     |     ExCreateThread(&handle, sub_8219A2B8, ...)
  |     |     KeSetBasePriorityThread(handle, 15)
  |     |     KeResumeThread(handle)
  |     |     [device+304]++
  |     |     [device+308+i*4] = thread handle
  |     |
  |     +-> sub_82199EC8(config, &device[64])  // CREATE AUDIO ENDPOINT
  |           |
  |           +-> sub_8219A770(config, &driver)     // Create XAudio render driver
  |           |     Reads [0x831D53EC]+60 (mixer sub-object)
  |           |     Calls [mixer_vtable+20] for buffer allocation
  |           |
  |           +-> sub_8219A440(0x61620006, &endpoint) // Allocate 72-byte endpoint
  |           |
  |           +-> sub_8219A848(endpoint, driver, 2)   // Base COM init
  |           |     [endpoint+0] = 0x820BE818 (intermediate vtable)
  |           |     [endpoint+4] = 1 (refcount)
  |           |     [endpoint+8] = driver
  |           |     [endpoint+12] = 2 (type)
  |           |
  |           +-> [endpoint+0] = 0x820BE768   // FINAL VTABLE (overwrite)
  |           |
  |           +-> sub_8219A098(endpoint, config)  // Init output chain
  |           |     sub_8219AA18(endpoint, format) // Create output
  |           |     Allocate 28-byte output chain node
  |           |     [endpoint+68] = output_chain   // Driver wrapper
  |           |
  |           +-> device[64] = endpoint       // STORE endpoint in device
  |
  +-> if (sub_82190B48 succeeded):
        [0x831D53EC] = device   // SECOND global store (redundant)
```

## 9. Critical Race Condition

`sub_82190B48` creates 6 audio worker threads (ExCreateThread with entry `sub_8219A2B8`) **BEFORE** calling `sub_82199EC8` to create the endpoint. Each thread immediately starts executing `sub_821910D0`, which reads `[device+64]`:

```
Thread creation:     sub_82190B48 line 1496 (ExCreateThread)
Endpoint creation:   sub_82190B48 line 1672 (sub_82199EC8)
Thread reads +64:    sub_821910D0 line 2014 (lwz r3,64(r30))
```

If a thread runs `sub_821910D0` before `sub_82199EC8` completes, `[device+64]` is **uninitialized** (it was never set by `sub_821903B8`). The thread reads whatever garbage is in the heap allocation at offset 64.

However, in practice the threads wait on `KeWaitForMultipleObjects` at line 1981 (PATH A of sub_821910D0) when `[device+304] != 0`. Since `[device+304]` is set to the thread count by the creation loop, the threads should block until signaled. The endpoint is created after all threads are created, so the race depends on whether any thread escapes PATH A before the endpoint exists.

## 10. Key Addresses Summary

| Address | Description |
|---------|-------------|
| 0x831D53EC | Global audio device pointer |
| 0x820BE4C0 | Audio device vtable (.rdata) |
| 0x820BE768 | Audio endpoint "vtable" (.rdata) -- contains mixed data/ptrs |
| 0x820BE818 | Intermediate COM base vtable (.rdata) |
| 0x82B2833C | Audio render critical section |
| 0x831D52CC | Wait object 1 (audio semaphore) |
| 0x831D52DC | Audio thread semaphore |
| 0x831D52F0 | "Frame ready" KEVENT |
| 0x831D5310 | Wait object 2 (shutdown event) |
| 0x831D53F0 | Atomic frame counter / "new" buffer base |
| 0x831D540C | "Current" audio state buffer base |

## 11. Key Functions

| Address | Name | Role |
|---------|------|------|
| sub_821901D8 | audDevice_Create | Top-level audio device creation |
| sub_821902E0 | audDevice_CreateDriver | Create render driver wrapper |
| sub_821903B8 | audDevice_BaseCtor | Base class constructor, stores global |
| sub_821904B8 | audDevice_Destroy | Destructor, clears global to NULL |
| sub_82190B48 | audDevice_FullInit | Full init: threads + endpoint |
| sub_82199EC8 | audEndpoint_Create | Create endpoint object at device[64] |
| sub_8219A098 | audEndpoint_InitOutput | Init output chain at endpoint[68] |
| sub_8219A770 | audDriver_Create | Create XAudio render driver |
| sub_8219A440 | audFactory_Alloc | COM-like factory/allocator |
| sub_8219A848 | audCOMBase_Init | COM aggregation base class init |
| sub_8219A2B8 | audThread_Entry | Audio thread entry (loads global, calls worker) |
| sub_821910D0 | audThread_Worker | Per-frame audio worker (THE vtable dispatch) |
| sub_82191228 | audThread_RenderFrame | Audio frame renderer (uses endpoint+68) |
| sub_8218FFB0 | audThread_DrainDPC | DPC-level audio buffer drain |

## 12. Root Cause of 0x000F4000

The vtable at `0x820BE768` in `.rdata` is NOT a flat vtable with 18 function pointers. It is a compact descriptor (2 function pointer entries at offsets 0 and 4, then class metadata). The code at `sub_821910D0` reads `[vtable+68]` expecting a function pointer, but offset 68 contains metadata (binary value 0x00000002; runtime value 0x000F4000 after some write modifies the region).

**The vtable 0x820BE768 is missing from `gta_iv/vtable_prepopulate.h`** (which covers 146 vtables but not this one). Even if it were added, only offsets 0, 4, and 12 contain valid function pointers in the binary -- offset 68 does not.

This means either:
1. On Xbox 360, the XEX loader patches additional entries into this descriptor at runtime (import resolution), which the recomp does not replicate
2. The descriptor is not meant to be used as a vtable at all -- the constructor is supposed to build a proper vtable dynamically, and something in that process fails in the recomp
3. The endpoint object is never properly constructed (sub_82199EC8 fails early), and `[device+64]` points to uninitialized heap memory where `[heap+0]` is garbage and `[garbage+68]` = 0x000F4000
