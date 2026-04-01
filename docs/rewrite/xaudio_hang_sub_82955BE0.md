# Deep Trace: sub_82955BE0 and the "XAudio Streaming Init Hang"

## Critical Finding: sub_82955BE0 Does NOT Block

**sub_82955BE0 is a pure allocation function with no blocking mechanism.** The INIT_PROBE label "xaudio-stream-HANG" is misleading -- this function cannot hang. The actual blocking points are elsewhere in the audio subsystem.

## sub_82955BE0 -- XAudio Streaming Voice Pool Allocator

**File**: `gta4_recomp.63.cpp` line 83107
**Address**: 0x82955BE0 - 0x82955D08
**Stack**: 144 bytes, saves r26-r31

### What It Does

1. Calls `sub_829764D8` -- a once-init guard that checks/sets flag at `0x831CDA80`
2. Sets flag byte at `0x831CD9D9` = 1 (streaming voices initialized)
3. **Loop 1** (6 iterations): Allocates 6 streaming voice objects
   - Each: `sub_821B3510(1488)` -- operator new(1488 bytes)
   - Memsets voice+628 and voice+106 to zero (128 bytes each via `sub_829FF840`)
   - Stores voice buffer: `sub_821B3510(16384)` -- 16KB data buffer per voice
   - Stores pointer at `0x831CC988 + i*4`
4. Sets flag byte at `0x831CD9DA` = 1 (streaming buffers initialized)
5. **Loop 2** (6 iterations): Allocates 6 streaming buffer management objects
   - Each: `sub_821B3510(24)` -- 24-byte control struct
   - Calls `sub_821B3560(0)` -- creates event object (r3=0 → NULL event)
   - Allocates 64KB buffer: `sub_821B3510(65536)`
   - Stores pointer at `0x831CC9A0 + i*4`
6. Returns

### Key Static Data

|Address|Description|
|-|-|
|0x831CC988|Pointer array: 6 streaming voice objects (1488 bytes each)|
|0x831CC9A0|Pointer array: 6 buffer management objects (24 bytes each)|
|0x831CD9D9|Flag: streaming voices initialized (set to 1)|
|0x831CD9DA|Flag: streaming buffers initialized (set to 1)|
|0x831CDA80|Once-init guard flag (checked by sub_829764D8)|

### Sub-call Tree (3 levels)

```
sub_82955BE0
  +-- sub_829764D8            [once-init guard -- trivial, no block]
  +-- sub_821B3510(1488) x6   [operator new -- game allocator, may fallback]
  +-- sub_829FF840(ptr,0,128) [memset -- trivial]
  +-- sub_821B3510(16384) x6  [operator new -- 16KB buffer alloc]
  +-- sub_821B3510(24) x6     [operator new -- 24-byte struct alloc]
  +-- sub_821B3560(0) x6      [operator delete(NULL) -- no-op or event create]
  +-- sub_821B3510(65536) x6  [operator new -- 64KB buffer alloc]
```

**Total allocations**: 18 calls to operator new, totaling ~6*(1488+16384+24+65536) = ~500KB
**No waits, no loops, no syscalls, no indirect vtable calls.** This function CANNOT block.

## sub_8228A1E0 -- Audio Stream Thread Registration

**File**: `gta4_recomp.8.cpp` line 44807
**Address**: 0x8228A1E0 - 0x8228A358
**Stack**: 304 bytes, saves r17-r31

### What It Does

1. Sets flag byte at computed address (marks audio initialized)
2. Calls `sub_82289A38(330, nameStr, 3, float)` -- thread descriptor init
3. Calls `sub_821D0488(...)` -- configures thread parameters (10 registers of function pointers and callbacks)
4. Calls `sub_8250D080(threadTable, ...)` -- **registers** a thread descriptor (does NOT create OS thread)
   - sub_8250D080 copies name strings, stores function pointers and callbacks into a table entry (stride=100 bytes, max 25 entries)
   - Increments entry count at threadTable+2500
5. Calls `sub_82662900(...)` x2 -- registers two additional descriptors (resource streaming sub-threads)
6. Returns

**No blocking.** sub_8250D080 is a data-structure registration, not thread creation. OS threads are created later by the thread scheduler.

## sub_825FD6B8 -- RPF Stream Thread Registration

**File**: `gta4_recomp.35.cpp` line 30353
**Address**: 0x825FD6B8 - 0x825FD7F8
**Stack**: 304 bytes, saves r18-r31

### What It Does

Identical pattern to sub_8228A1E0:

1. Calls `sub_82662900(...)` -- registers RPF streaming descriptor
2. Calls `sub_825FD228(500, nameStr, 3, float)` -- thread descriptor init
3. Calls `sub_821D0488(...)` -- configures thread parameters
4. Calls `sub_8250D080(threadTable, ...)` -- registers thread descriptor
5. Stores result and zeros a vector
6. Returns

**No blocking.** Pure registration.

## Where the REAL Hang Is

The init function `sub_82478AF8` calls these three functions in sequence at addresses 0x82478E2C-0x82478E3C, and execution continues normally after all three return. The probes show:

```
sub_8228A1E0  → ENTER + RETURN  (thread registration OK)
sub_825FD6B8  → ENTER + RETURN  (RPF stream registration OK)
sub_82955BE0  → ENTER + RETURN  (voice pool allocation OK)
```

The real hang occurs LATER in the init, in one of these downstream functions:

### Candidate 1: sub_827C2420 (activate-streaming) -- MOST LIKELY

Called at 0x82478EAC, after sub_82955BE0. This function:
1. Adds search paths to the streaming module
2. Makes an **indirect vtable call** (`vtable[1]`) on the resource object
3. Calls `sub_82852DD0` (OpenAndProcess) which does file I/O through VFS handlers
4. The VFS handler's Open method can block if:
   - The device path is unmounted
   - An OVERLAPPED I/O completion never signals
   - The TLS allocator context (TLS[1676]) is null, causing fallback allocation storms

See `streaming_activation_sub_827C2420.md` for the full call tree.

### Candidate 2: Audio worker threads blocking on uninitialized events

The audio worker threads (sub_821909D0, sub_82190A98) are created by `sub_82190B48` (called during phase 4-7 init). These threads:
- Wait on `KeWaitForSingleObject(0x831D52F0, INFINITE)` (primary)
- Wait on `KeWaitForSingleObject(0x831D52DC, INFINITE)` (secondary)

These threads are NOT supposed to block the init thread, but if the init thread tries to synchronize with them (e.g., waiting for a "ready" event that the worker was supposed to signal), the init would deadlock.

### Candidate 3: sub_82212EC0 (audio device connection state machine)

Already hooked in imports.cpp (line 2478). On Xbox 360, an async kernel callback sets `struct+2016 = 0` (connected) after XAudio2 device arrival. On PC/macOS, this callback never fires, so the state machine loops forever. **This hook already exists and forces connected state.**

### Candidate 4: sub_821910D0 (audio render thread pump)

The vtable[17] dispatch at `[endpoint+0]+68` reads 0x000F4000 (garbage). The MISSING-FUNC handler silently skips this. The function itself does NOT hang -- it returns cleanly. But if the critical section at 0x82B2834C is held by a dead thread, entry would deadlock. Already analyzed in `47_sub_821910D0_analysis.md`.

## Synchronization Object Map (Audio System)

|Address|Type|Purpose|Who Signals|Who Waits|
|-|-|-|-|-|
|0x831D52CC|KEVENT|"Audio ready"|Primary worker (sub_821909D0)|sub_821910D0 PATH A|
|0x831D52DC|KSEMAPHORE|Thread work semaphore|Primary worker|Secondary workers (sub_82190A98)|
|0x831D52F0|KEVENT|"Frame complete"|sub_821910D0 PATH A|Primary worker (sub_821909D0)|
|0x831D5310|KEVENT|"Shutdown"|Terminate handler|sub_821910D0 PATH A|
|0x82B2834C|CRITICAL_SECTION|Render mutex|sub_821910D0|sub_821910D0|
|0x83137B80|Event byte|Audio worker wakeup|VBlank timer / SDL audio thread|Game audio worker|
|0x83130008|Semaphore|Audio work queue|VBlank timer|Game audio worker|

**Per-frame signaling** (from imports.cpp FrameEnd): `0x83137B80` and `0x83130008` are signaled every frame to keep the audio system alive.

## What Xbox 360 Does vs Recomp

### On Xbox 360 (Normal Behavior)

1. `sub_82955BE0` allocates streaming voice pool -- returns immediately
2. `sub_827C2420` opens audio bank RPFs via VFS -- I/O completes via Xbox kernel async
3. Audio worker threads wake on hardware-signaled events (XAudio render driver DPC)
4. `sub_821910D0` calls `vtable[17]` on the endpoint -- this is the hardware DMA commit (XAudioRenderDriverEndpoint::Present)
5. The audio device connection callback fires from the kernel, transitioning state machine to "connected"

### In Recomp (Why It Breaks)

1. `sub_82955BE0` works fine -- pure allocation
2. `sub_827C2420` may fail on VFS if RPF files aren't properly mounted via HostPathDevice
3. Audio worker threads depend on events being signaled -- the per-frame FrameEnd handler signals them, but if init itself hasn't reached the frame loop yet, workers may starve
4. `vtable[17]` contains garbage (0x000F4000) -- MISSING-FUNC handler skips it (already handled)
5. Audio device connection -- already hooked (sub_82212EC0 hook forces connected state)
6. **The TLS allocator (TLS[1676]) may be null during early init**, causing every allocation inside `sub_82955BE0` to go through the fallback page allocator -- this works but is slow and produces the "ALLOC FALLBACK storm" pattern

## What a Native Rewrite Needs

### For sub_82955BE0: Nothing
This function works correctly as recompiled code. It's pure allocation with no hardware dependencies.

### For the actual hang (downstream):

1. **Streaming activation (sub_827C2420)**: Ensure all RPF files are mounted via HostPathDevice BEFORE this function runs. The VFS handler at `sub_82855460` dispatches by path prefix -- `game:`, `platform:`, `update:` all need working mount points.

2. **Audio worker thread wakeup**: The `FrameEnd()` handler already signals `0x83137B80` and `0x83130008` every frame. But if the init hasn't reached the main loop, these signals don't fire during init. **Consider signaling these events explicitly after sub_82190B48 creates the audio threads.**

3. **Endpoint vtable (0x000F4000)**: Already handled by MISSING-FUNC. But if a clean fix is desired, hook `sub_821910D0` and skip the `vtable[17]` dispatch (the audio render commit is handled natively by SDL).

4. **TLS allocator during init**: The 18 allocations in sub_82955BE0 may hit the fallback path if TLS[1676] is null. The existing `sub_8218BE28` hook handles this gracefully (routes to host page allocator), so this is cosmetic noise, not a bug.

5. **Critical race in sub_82190B48**: Audio worker threads are created BEFORE the endpoint at `[device+64]` is initialized. On Xbox 360, `KeWaitForMultipleObjects` blocks them until they're signaled. If the recomp's wait implementation has timing issues, a thread could read uninitialized `[device+64]` and crash. Consider hooking `sub_82190B48` to either defer thread creation until after endpoint init, or pre-initialize `[device+64]` to null and add a null check in `sub_821910D0`.
