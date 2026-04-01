# REWRITE ARCHITECTURE: GTA IV Xbox 360 Recompiled Engine Systems

Synthesized from 73 rewrite research docs + 21 port analysis docs.
Date: 2026-03-28

---

## Table of Contents

- [A. Allocator System Rewrite](#a-allocator-system-rewrite)
- [B. Streaming System Rewrite](#b-streaming-system-rewrite)
- [C. Engine Init Rewrite](#c-engine-init-rewrite)
- [D. Critical Section Strategy](#d-critical-section-strategy)
- [E. Thread Model](#e-thread-model)
- [F. Global State Map](#f-global-state-map)

---

## A. Allocator System Rewrite

### A.1 Original Architecture

The RAGE allocator is a 3-class hierarchy rooted in TLS[1676]:

```
TLS[1676] -> rage::sysMemMultiAllocator @ 0x82B29500 (vtable 0x8208480C)
  sub_allocs[0] = sysMemScopedLockAllocator @ 0x82B295EC (vtable 0x82084AE4)
  sub_allocs[1] = sysMemBuddyAllocator @ 0x82B29528 (vtable 0x820848CC)
  sub_allocs[2] = sysMemBuddyAllocator @ 0x82B29528 (same instance)
  sub_allocs[3] = sysMemBuddyAllocator @ 0x82B29528 (same instance)
```

**MultiAllocator** (40 bytes): Routes alloc by `flags` argument to sub_allocs[flags]. Routes free by ownership scan (sub->vtable[18] = Owns). Indices 1 and 3 are "persistent" -- free is suppressed unless TLS[1688]/TLS[1704] override.

**ScopedLockAllocator** (~2300 bytes): Two-tier allocator under a single global critical section at `0x83192960`.
- **Small path** (size <= 64, align <= 16): 4 slab pools (8/16/32/64 byte bins, 16KB pages, 2044/1022/511/255 elements per slab). Implemented by `sub_82871828`. Freelists are intrusive. New slabs allocated from the large path.
- **Large path** (size > 64): Segregated free-list with 16 size-class buckets. Best-fit search with Floyd's tortoise cycle detection. 16-byte block headers. Alignment splits create front/tail remnants. Implemented by `sub_82848750`.

**BuddyAllocator** (192 bytes): Fixed-block pool with bitmap free-tracking, managing a 272MB region with 8KB blocks. Constructed with `sub_82847C08(base, 8192, 34816, lock)`.

### A.2 TLS Push/Pop State Machine

Four TLS slots per thread form a one-deep allocator stack:

| Offset | Purpose |
|-|-|
| TLS[1668] | Push/pop refcount (same-allocator nesting) |
| TLS[1672] | Saved (previous) allocator |
| TLS[1676] | Current allocator (read by operator new/delete) |
| TLS[1680] | Target/default allocator |

**Push** (`sub_828470E0`, 163 call sites): If current == target, increment refcount. Otherwise, save current to [1672], write target to [1676].

**Pop** (`sub_82847120`, 168 call sites): If refcount > 0, decrement. Otherwise, restore [1672] to [1676], clear [1672].

Additionally: 86 direct TLS[1676] reads for vtable dispatch, 46 TLS[1680] reads, and several inline push/pop patterns in generated code.

### A.3 Current Broken State

`__imp__sub_8218BE28` (the original allocator vtable dispatch) has no definition in codegen output -- the address 0x8218BE28 falls inside sub_8218BC28's body and was never split. **ALL allocations go through the fallback path** (`SystemHeapAlloc`), which:
1. Uses the host page allocator (`BaseHeap::AllocRange`)
2. Acquires `global_critical_region_` (a single `std::recursive_mutex`)
3. Does a linear page table scan under the lock
4. Serializes ALL game allocations across ALL threads through one mutex

This produces the "ALLOC FALLBACK storm" -- 4608+ particle emitter allocations plus all subsystem init allocations contend on one lock.

### A.4 Native Replacement Design

**Goal**: Replace the entire RAGE allocator hierarchy with a native allocator that the generated code dispatches to via the same TLS mechanism.

**Option 1: Drop-in native allocator behind RAGE vtable**

Write a native C++ class that implements the RAGE allocator vtable interface (19 methods at minimum). Install it at the same guest address so all 86 direct TLS[1676] reads and 163 push/pop sites work unchanged.

Key vtable slots to implement:
| Slot | Offset | Method | Native equivalent |
|-|-|-|-|
| 2 | +8 | Allocate(this, size, align, flags) | `mimalloc mi_malloc_aligned` or `jemalloc je_mallocx` |
| 3 | +12 | Free(this, ptr) | `mi_free` / `je_dallocx` |
| 6 | +24 | Realloc | `mi_realloc_aligned` |
| 7 | +28 | GetUsedMemory | Stat query |
| 8 | +32 | GetTotalMemory | Stat query |
| 18 | +72 | Owns(ptr) | Range check or always-true |

The MultiAllocator dispatch by flags can be collapsed: the native allocator handles all size classes internally. Free's ownership scan becomes unnecessary with a universal allocator.

**Option 2: Hook all 86+46 TLS read sites**

Replace `sub_8218BE28`, `sub_821B3510`, `sub_821B3538`, `sub_821B3560` with native hooks that bypass TLS entirely and call the native allocator directly. Leave TLS[1676] populated with a valid pointer so any missed inline sites don't crash, but the hot paths go native.

Functions to hook:
| Address | Role | Call sites |
|-|-|-|
| sub_8218BE28 | malloc (16-align) | Hooked (currently fallback) |
| sub_8218BE50 | malloc (explicit align) | Hooked (currently fallback) |
| sub_821B3510 | operator new | Many (generated code) |
| sub_821B3538 | operator new (aligned) | Many (generated code) |
| sub_821B3560 | operator delete | Many (generated code) |
| sub_828470E0 | Push allocator | 163 sites |
| sub_82847120 | Pop allocator | 168 sites |
| sub_828493E0 | ScopedLock Allocate | Via vtable |
| sub_828494D8 | ScopedLock Free | Via vtable |

**Recommended approach**: Option 2 (hook dispatch functions). The inline TLS reads in generated code will still work because TLS[1676] stays populated with a valid allocator object. The hooked functions intercept the most common paths.

**Lock strategy**: The native allocator (mimalloc/jemalloc) has internal per-thread caches and fine-grained locking, eliminating the single-CS bottleneck at 0x83192960. The push/pop hooks become no-ops or thin wrappers since the native allocator doesn't need scoped switching.

### A.5 Init Sequence

The original init chain (from `.ctors`):
```
sub_82A5AE68 -> sub_821B3770:
  1. Construct ScopedLockAllocator(0x82B295EC, 96MB, 1)
  2. Create CS at 0x82B295E8 (208KB)
  3. Allocate 272MB region, construct BuddyAllocator(0x82B29528)
  4. Construct MultiAllocator(0x82B29500)
  5. Register sub-allocators: [scoped-lock, buddy, buddy, buddy]
  6. Write TLS[1676] = TLS[1680] = 0x82B29500
```

For the native rewrite:
1. Hook `sub_821B3770` (or `sub_82A5AE68`)
2. Construct the native allocator
3. Write a guest-memory stub at 0x82B29500 with a valid vtable pointing to native implementations
4. Write TLS[1676] = TLS[1680] = 0x82B29500
5. Skip the 96MB + 272MB guest allocations entirely

### A.6 Separate Heap Objects (0x8312B7D0/0x8312B7D8)

These global heap objects (physical/virtual) use vtable 0x82000970 and back the NtAllocateVirtualMemory path. They are initialized by `sub_828AFFB8` and are NOT reached from operator new. They serve kernel-level page allocation. These can remain as-is since `BaseHeap` already handles them on the host side.

---

## B. Streaming System Rewrite

### B.1 Original Architecture

Three layers:

**VFS Handler Manager** (global at `0x82B07278`, ~3KB struct): Maps resource names to device paths. 12 handler slots x 256 bytes. Handler count at +0xC00, active count at +0xC04.

**Device Mount Table** (global at `0x831AB940`): Array of `DeviceMountEntry` (276 bytes each). Each entry: 264-byte prefix string, prefix length, device pointer array (supports stacked/layered devices). Resolution tries highest index first (overlay semantics).

**Device Objects** (25-slot vtable): Open, Read, Write, Seek, Close, GetSize, FindFirst/Next/Close, etc. The game's internal devices (`fiDeviceLocal`, `fiPackfile`) ultimately call NtCreateFile/NtReadFile, which RexGlue intercepts.

### B.2 Path Resolution

`sub_8284F468` (Find): Iterates registered search paths, builds full path via `sub_8284F0C0`, tries opening via `sub_8285AA68`.

`sub_82855460` (Device Lookup): 8 hardcoded prefix checks (strncmp) then registered device table walk. Longest match wins.

Prefix routing:
| Prefix | Device address |
|-|-|
| `device:` (7) | 0x82B08004 |
| `platformfs` (10) | 0x82B08004 |
| `game:` (5) | 0x82B07EF0 (via sub_828708C8) |
| `update:` (7) | 0x82B07EF0 |
| `dvdfs:` (6) | 0x82B07EF0 |
| `hdd:` (4) | 0x82B07EF0 |
| `cd:` (3+`:`) | 0x82B07EF0 |

### B.3 Resource Loading Pipeline

```
sub_82852DD0 (OpenAndProcess)
  -> sub_8284F468 (Find on device)
    -> sub_8284F0C0 (BuildPath)
    -> sub_8285AA68 (TryOpen via device vtable[4])
  -> sub_82852D18 (Process)
    -> sub_82852A50 (Lookup/Create in hash table at coordinator+24)
    -> callback->vtable[8] (actual resource load/fixup)
    -> sub_8284FA58 (FreeResources cleanup)
  -> sub_8285B088 (CloseDevice via vtable[40])
```

### B.4 Current Interception Architecture

RexGlue intercepts at two levels:

**NT kernel level** (`xboxkrnl_io.cpp`): NtCreateFile, NtReadFile, NtWriteFile, etc. All resolve through the VFS symlink/device registry. All I/O is forced synchronous (`if (true || ...)` forces sync path).

**CRT/Win32 level** (`crt/file.cpp`): CreateFileA, ReadFile, etc. Call into the same VFS.

**LibertyRecomp overlay** (`file_system.cpp`, `vfs.cpp`): Game-specific path remapping, mod overlay, RPF DUMP fallback, button prompt swapping.

The game's own VFS device classes (`fiDeviceLocal`, `fiPackfile`) run as recompiled PPC code. They call CreateFileA/ReadFile, which are intercepted. No device-level vtable hooks are needed.

### B.5 Streaming Coordinator

Global at `0x831E55EC`. 48-byte struct with 3 hash maps (12 bytes each) and config flags.

Flags at +40 default to 0x3 (bits 0-1). Bit 17 (thread-safety lock enable) is **never set** at runtime. The TLS allocator save/restore in streaming functions is always skipped.

### B.6 Known Hangs

1. **sub_82955BE0** (XAudio streaming init): Waits for XMA decoder hardware. Needs stub.
2. **sub_8286C238** (pgStreamable tree visitor): Called via callback->vtable[2] during resource processing. Infinite loop on corrupt linked list or null vtable in resource entries. Current recommendation: hook `sub_82852D18` to skip the visitor.
3. **sub_8285A1B0** (GPU ring buffer dispatch): Called during module registration. Already handled by `sub_8285A8B0` stub.

### B.7 Native Replacement Strategy

**Do NOT replace the game's VFS layer.** The recompiled `fiDeviceLocal` and `fiPackfile` code works correctly. File I/O is already intercepted at NtCreateFile/ReadFile.

**Replace the async model**: The streaming system is forced synchronous. For better performance, implement true async I/O:
1. Replace `NtReadFile` with async variants that use platform-native async I/O (io_uring on Linux, kqueue on macOS, IOCP on Windows)
2. Signal the existing IO completion ports (`XIOCompletion`) after async completion
3. The game's streaming workers (`sub_8284CFD8` ring buffer, semaphore-seeded) can then work as designed

**Replace the resource processing callback**: Hook `sub_82852D18` to skip `callback->vtable[8]` (the pgStreamable tree visitor `sub_8286C238`). The visitor performs in-memory pointer fixups for Xbox 360 memory layout, which are unnecessary on the host where the flat address space makes guest pointers work directly.

---

## C. Engine Init Rewrite

### C.1 Master Init Sequence

The top-level game state machine `sub_82142230` drives boot through 7 states:

| State | Function | Purpose | Category |
|-|-|-|-|
| 0 | sub_822414E8 | Platform/system init | Keep (calls through) |
| 1 | sub_8223DDA8 | Pre-game readiness | Stub (XAM readiness) |
| 2 | sub_8223DEE8 | Save data / profile check | Stub (force success) |
| 3 | (complex gate) | Sign-in detection, content transition | Rewrite (write memory state directly) |
| 4 | sub_822440F8 | Outer scene creation SM | Keep (calls sub_82242910) |
| 5 | sub_822422E0 | Scene dispatch | Keep (level selection) |
| 6 | sub_822438B0 | Inner scene creation SM (8 states) | Keep (runs sub_82242910) |

### C.2 Sub_82242910: The 15-State Scene Creation State Machine

This is the core init pipeline. Each state's disposition:

| State | Key call | Disposition | Reason |
|-|-|-|-|
| 0 | sub_8223DAA0 (readiness) | **Stub** | XAM readiness check; write flag directly |
| 1 | sub_826CBA70, sub_8223F9F0 | **Keep** | Network/XAM notification (hooks already handle) |
| 2 | sub_8223CB60 | **Stub** | Sign-in re-check; write error code 6 |
| 3 | sub_826CBA70, sub_82240AB0 | **Keep** | Network readiness, content validation |
| 4 | sub_8223DB20, sub_8223F308 | **Rewrite** | Reads platformMode; must be 3. Write scene gate values |
| 5 | sub_8223F9F0 | **Keep** | XAM notification dispatch |
| 6 | sub_8284AAE0 (scene load) | **Keep** | Core streaming; must run |
| 7 | sub_8223CB60, sub_8223F9F0 | **Stub** | Error recovery path |
| 8 | sub_8284AB10, sub_8284B490 | **Keep** | Scene object creation |
| 9 | sub_82240B78, sub_8284ABA0 | **Keep** | Post-load setup |
| 10 | sub_8284ABD0, sub_8284B490 | **Keep** | Resource activation |
| 11 | platformMode check | **Rewrite** | Validate mode is in {0,1,3,4}; force 3 |
| 12 | sub_822417B0 | **Keep** | Scene creation dispatch (the critical path) |
| 13 | (error/reset) | **Stub** | Error recovery; clear flags, reset to 0 |
| 14 | sub_822417B0 (second pass) | **Keep** | Scene finalization |

### C.3 Audio Engine Init: sub_82478AF8

80 calls across 17 phases. Called from `sub_821FC1F8` (master init), call #30.

| Phase | Calls | Disposition | Notes |
|-|-|-|-|
| 1-3: Environment init | sub_826225E0/648/6B0 | **Keep** | Pure computation |
| 4-8: AudioManager ctor | new(176), sub_8294BD68 | **Keep** | Object construction |
| 9-13: Renderer detection | _stricmp x5 | **Rewrite** | Force renderer_type=0 (default) |
| 14-19: XAudio graph init | sub_82953088, sub_82955838 | **Keep** | Graph configuration |
| 20: Main resource load | sub_8284D220 | **Keep** | Loads audio description |
| 21-22: Graph config | sub_829530B0, sub_82953A68 | **Keep** | Bind resource to graph |
| 23-25: Device init | sub_826BDC18 | **Keep** | Audio device enumeration |
| 26: Device vtable call | `*(0x831CC904)->vt[1]` | **Stub if null** | Null guard needed |
| 27-28: Audio enable | sub_8296A278(1) | **Keep** | Enable output |
| 29-30: Stream start | sub_8228A1E0 | **Keep** | Start audio thread |
| 31: RPF streaming | sub_825FD6B8 | **Keep** | Start RPF stream |
| **32**: XAudio stream | **sub_82955BE0** | **STUB** | **Known hang point** (waits for XMA hardware) |
| 33-38: Path + alloc | sub_8284F310, sub_827C2420 | **Keep** | Streaming manager activation |
| 39-54: Bank loading | sub_8284D220 x7 | **Keep** | Audio resource banks |
| 55: Voice block | sub_827ADB48 | **Keep** | Voice/audio block creation |
| 56-59: Viewports | sub_821B3510(N*176+16) | **Keep** | Viewport array allocation |
| 60-62: Pool config | sub_827ACCA0 x3 | **Keep** | Streaming pool sizes |
| 63-72: Streaming init | sub_8287AC38, etc. | **Keep** | Streaming graph init |
| 73-80: Camera + entity | sub_821B3510(112), inline loop | **Keep** | Camera object + entity array |

**Critical**: Call 32 (`sub_82955BE0`) must be stubbed -- it is the XAudio streaming init that hangs waiting for XMA decoder hardware. All subsequent phases (33-80) depend on getting past this call.

### C.4 Core Engine Init: sub_8218C600

Called from `game_init.cpp` Phase 1. Contains GPU init chain:

| # | Function | Purpose | Disposition |
|-|-|-|-|
| 5 | sub_82850AF0 | GPU availability check | **Keep** (returns false) |
| 6 | sub_82850B60 | GPU init mode | **Keep** (sets pipeline mode) |
| 7 | sub_8218BE28(472) | Allocate render context | **Keep** (hooked allocator) |
| 8-13 | sub_82857028..sub_82856C90 | GPU buffers/state | **Keep** (GPU allocs fail gracefully) |

GPU allocations via `sub_82A10EB0` are already redirected to `SystemHeapAlloc`. Structures get allocated but vtable pointers stay 0x00000000. This is handled by the extensive GPU stub infrastructure (30+ hooks in imports.cpp).

### C.5 Post-Init System Activation

After scene creation SM completes, `sub_82142230` activates these systems in sequence:

1. `sub_821B5A68` -- text/locale system
2. `sub_82220118(0x82BCF998, 0)` -- streaming world setup
3. `sub_8222DB48(0x82BEFA40)` -- world manager init
4. `sub_8214AD88` -- post-load setup
5. `sub_821ED6D8(0x82B978B0, 0, 0, 1)` -- game system activation
6. `sub_82145820` -- DLC detection

After this, the render loop takes over via `sub_82856F08`. The main game SM does NOT loop per-frame -- it runs once during boot.

---

## D. Critical Section Strategy

### D.1 The Convoy Effect Problem

`global_critical_region` is a single `std::recursive_mutex` acquired at **124 sites across 24 files**:

| Module | Sites | Operations |
|-|-|-|
| xmemory.cpp | 21 | All memory alloc/free/protect |
| kernel_state.cpp | 17 | Kernel object management |
| object_table.cpp | 13 | Handle lookup/create |
| shared_memory.cpp | 10 | GPU shared memory |
| content_manager.cpp | 7 | Content enumeration |
| module.cpp | 6 | Module management |
| virtual_file_system.cpp | 5 | VFS device/symlink ops |
| processor.cpp | 5 | Thread management |
| Others (threading, audio, etc.) | 40+ | Various |

**Every** `RtlEnterCriticalSection` slow path and **every** `RtlLeaveCriticalSection` with waiters must acquire this lock via `GetNativeObject`. This creates convoy effects where independent critical sections become coupled through the global lock.

### D.2 Critical Section Addresses (Guest-Side)

| Address | Purpose | Contention level |
|-|-|-|
| 0x83192960 | RAGE allocator lock (all alloc/free) | **Extreme** -- every allocation |
| 0x82B2835C | Heap critical section (sub_8218ED50) | High -- heap vtable dispatch |
| 0x83192C4C | Thread manager CS (sub_82849940) | Low -- thread init only |
| 0x8286833C | Audio render CS (sub_821910D0) | Medium -- per-frame audio |
| Various per-object | Streaming resource locks (+1024) | Medium -- per-stream |

### D.3 The First-Time Init Amplification

On first use of any CS slow path, `GetNativeObject` must allocate a new `XEvent` under the global lock. During startup, many threads initialize their critical sections simultaneously, creating a thundering herd on the global lock.

### D.4 Recommended Fix Strategies

**Strategy 1: Per-CS native mutex (minimum change)**

Replace `RtlEnterCriticalSection` / `RtlLeaveCriticalSection` to use a per-CS host mutex stored alongside the guest CS, bypassing `GetNativeObject` entirely. This eliminates the `global_critical_region` dependency for all guest CS operations.

Implementation:
1. On first Enter of any guest CS, allocate a `std::mutex` and stash its pointer in the CS's `wait_list_flink`/`wait_list_blink` fields
2. All subsequent Enter/Leave operations use the stashed native mutex
3. No `XEvent` creation, no `global_critical_region` acquisition

**Strategy 2: Lock-free allocator (eliminates hottest CS)**

Replace the RAGE allocator with a lock-free native allocator (mimalloc, jemalloc). This eliminates contention at 0x83192960 entirely. Combined with Strategy 1, this reduces global_critical_region contention by ~50% (21 xmemory sites + all allocator CS waits).

**Strategy 3: Remove global_critical_region from non-critical paths**

Audit the 124 acquisition sites. Many can use fine-grained locks:
- `object_table.cpp`: Use a reader-writer lock (reads are dominant)
- `virtual_file_system.cpp`: Use per-device locks
- `shared_memory.cpp`: Use per-region locks

### D.5 Which CS Addresses Are Independent

The following guest CS addresses protect completely independent resources and can safely use separate native locks:

- 0x83192960 (allocator) vs 0x8286833C (audio) vs 0x83192C4C (thread mgr)
- Per-object streaming resource locks (different objects, different locks)
- Per-CS in game code (save system, profile system, etc.)

The only dependency is when one operation (e.g., audio rendering) needs to allocate memory (which would acquire the allocator CS). With a native lock-free allocator, this dependency vanishes.

---

## E. Thread Model

### E.1 Thread Inventory

| Thread | Entry point | Priority | Stack | Purpose |
|-|-|-|-|-|
| Main | (app entry) | Normal | 1MB+ | Game state machine, frame dispatch |
| Audio Primary | sub_821909D0 | +15 (high) | 64KB | Audio buffer drain + render |
| Audio Secondary (x5) | sub_82190A98 | +15 | 64KB | Additional audio processing |
| Streaming Worker | sub_825FBCAC loop | Normal | Default | Resource I/O + sleep(16ms) |
| Render | sub_821910D0 | Normal | Default | GPU command submission |

### E.2 Main Thread Init Flow

```
main entry
  -> CRT .ctors (allocator init: sub_821B3770)
  -> sub_82120000 (game_init: core engine, game manager, profile, subsystems)
  -> sub_82140000 (RAGE engine init)
  -> sub_821FC1F8 (master init, 80-call audio sequence via sub_82478AF8)
  -> sub_82142230 (game state machine: states 0-6, scene creation)
  -> [returns] -> render loop via sub_82856F08
```

### E.3 Audio Thread Lifecycle

Created by `sub_82190B48` during audio init (call 30 in master init):

1. `ExCreateThread` with start routine sub_821909D0 or sub_82190A98
2. `KeSetBasePriorityThread(thread, 15)` -- real-time priority
3. `KeResumeThread` -- start execution

**Primary worker** loop:
```
LOOP:
  KeWaitForSingleObject(event_complete @ 0x831D52F0, INFINITE)
  if [device+300] == 0: shutdown
  sub_8218FFB0(device)  // DPC buffer drain
  sub_82191228(device, 1)  // render audio frame
  KeSetEvent(event_audReady @ 0x831D52CC)
  goto LOOP
```

**Secondary worker** loop: Same pattern but waits on semaphore `0x831D52DC` and calls `sub_82191228(device, 0)`.

**Shutdown**: Set `[device+300] = 0`. Primary releases semaphore for secondaries. All threads exit.

### E.4 Streaming Worker

The streaming thread at `sub_825FBCAC` runs:
1. Release scoped lock (`RtlLeaveCriticalSection` on streaming resource lock)
2. Sleep 16ms (`sub_82A1A200` -> `KeDelayExecutionThread`)
3. Loop back

The streaming model is forced synchronous: `PPC_STORE_U32(0x830F589C, 1)` sets sync mode. All I/O completes inline on the calling thread. Async worker threads are effectively dead.

### E.5 Synchronization Requirements

| Pair | Mechanism | Notes |
|-|-|-|
| Main <-> Audio | Events (0x831D52CC, 0x831D52F0, 0x831D5310) + Semaphore (0x831D52DC) | Must be properly initialized |
| Main <-> Streaming | Forced sync (0x830F589C = 1) | No real async coordination |
| Main <-> Render | CS at 0x8286833C + events | Per-frame rendering |
| Any <-> Allocator | CS at 0x83192960 (or native lock-free) | Hottest contention point |

---

## F. Global State Map

### F.1 Allocator Globals

| Address | Size | Content | Init | Rewrite? |
|-|-|-|-|-|
| 0x82B29500 | 40 | sysMemMultiAllocator instance | .ctors | Replace with native stub |
| 0x82B29528 | 192 | sysMemBuddyAllocator instance | .ctors | Eliminate (native alloc) |
| 0x82B295E8 | 4 | Lock/CS pointer | .ctors | Eliminate |
| 0x82B295EC | ~2300 | sysMemScopedLockAllocator instance | .ctors | Eliminate (native alloc) |
| 0x82B29EE4 | 4 | Init flags bitmask | .ctors | Eliminate (unconditional init) |
| 0x82B07070 | 4 | Global debug/mode flag (-1 enables small path) | Init | Write -1 (enable fast path if kept) |
| 0x83192960 | 28 | Global allocator CRITICAL_SECTION | Init | Replace with native mutex or eliminate |
| 0x8312B7D0 | 4 | Physical heap ptr | sub_828AFFB8 | Keep (kernel-level) |
| 0x8312B7D8 | 4 | Virtual heap ptr | sub_828AFFB8 | Keep (kernel-level) |

### F.2 Streaming Globals

| Address | Size | Content | Init | Rewrite? |
|-|-|-|-|-|
| 0x82B07278 | ~3KB | VFS Handler Manager | Engine init | Keep (game VFS works) |
| 0x831AB940 | var | Device Mount Table | Engine init | Keep (populated by recomp code) |
| 0x831E55EC | 4 | Streaming coordinator ptr | sub_821B3CE8 | Keep (48-byte bookkeeping) |
| 0x830F589C | 4 | Sync streaming mode flag | Hook | Keep (forced to 1) |
| 0x830F5820 | 4 | Streaming-pending flag | Hook | Keep (cleared by hook) |

### F.3 Audio Globals

| Address | Size | Content | Init | Rewrite? |
|-|-|-|-|-|
| 0x831CC904 | 4 | Audio device manager ptr | sub_826BDC18 | Keep (vtable calls need null guard) |
| 0x831CC944 | 4 | Audio engine object alt ptr | sub_82478AF8 | Keep |
| 0x831CDA47 | 1 | Audio error flag | sub_82478AF8 | Keep (cleared to 0) |
| 0x831CDA65 | 1 | Audio initialized flag | sub_82478AF8 | Keep (set to 1) |
| 0x831D52CC | var | Audio ready event | sub_82190B48 | Keep (kernel sync) |
| 0x831D52DC | var | Audio work semaphore | sub_82190B48 | Keep (kernel sync) |
| 0x831D52F0 | var | Audio frame complete event | sub_82190B48 | Keep (kernel sync) |
| 0x831D5310 | var | Audio shutdown event | sub_82190B48 | Keep (kernel sync) |
| 0x831D53EC | 4 | Global audio device ptr | sub_82190B48 | Keep |
| 0x82FF5360 | 4 | g_pEngineObject (AudioManager) | sub_82478AF8 | Keep |
| 0x82FF5364 | 4 | g_pManager1 (XAudio graph) | sub_82478AF8 | Keep |
| 0x82FF5368 | 4 | g_pManager2 (VoiceBlock) | sub_82478AF8 | Keep |
| 0x82FF536C | 4 | g_pViewportArray | sub_82478AF8 | Keep |
| 0x82FF5374 | 4 | g_pCameraObj | sub_82478AF8 | Keep |

### F.4 Thread Manager Globals

| Address | Size | Content | Init | Rewrite? |
|-|-|-|-|-|
| 0x83192980 | var | RAGE Thread Manager | sub_828499E8 | Keep |
| 0x83192C40 | 4 | Thread count | sub_82849940 | Keep |
| 0x83192C4C | 12+ | Thread manager CS | sub_82849940 | Keep (low contention) |

### F.5 State Machine Variables (Must Be Initialized)

| Address | Type | Required value | Purpose |
|-|-|-|-|
| 0x82BF9848 | u32 | 0 (start) | Scene creation state counter |
| 0x82BF9838 | u32 | 0 (start) | Inner scene state |
| 0x82BF9844 | u32 | 3 (forced) | platformMode (must be 3 for base game) |
| 0x82BF9B70 | u32 | 0xFFFFFFFF (-1) | XAM readiness (no dialog pending) |
| 0x82A9546C | u32 | 0 | Error code (none) |
| 0x82A95478 | u32 | 0 | Episode index (base game) |
| 0x82A9547C | u8 | 0 | Episode DLC flag (no DLC) |
| 0x82B39504 | u32 | 0 | Episode index (base game -> level 12) |
| 0x82A22D04 | u32 | 0 | USER_STATE (0 = signed in) |
| 0x82B94554 | u32 | 0 | STAGE_COUNTER (reset by sub_821E4398) |
| 0x831D5327 | u8 | 1 | XAM ready flag |

### F.6 GPU/Render State (Can Be Eliminated in Native Code)

| Address | Content | Notes |
|-|-|-|
| 0x831C2EF8 | Resource dictionary ptr (NULL) | sub_827A9A20 null-guarded |
| 0x83124AF4 | GPU command buffer ptrs (stale) | Never used (GPU stubbed) |
| 0x83169C00 | Render state table (37 entries) | Unused (host rendering) |
| 0x82FF2360 | Entity tracking array (12KB) | Zeroed by audio init |
| 0x7FC80000+ | GPU MMIO range | Registered but unused |

---

## Implementation Priority

### Phase 1: Allocator (Highest Impact)

1. Hook `sub_821B3770` to construct native allocator, write TLS[1676]
2. Hook operator new/delete/malloc to dispatch directly to native allocator
3. Make push/pop (`sub_828470E0`/`sub_82847120`) into no-ops
4. Verify: particle emitter storm (4608 allocs) completes without fallback

### Phase 2: Critical Sections (Unblocks Threads)

1. Replace `RtlEnterCriticalSection` / `RtlLeaveCriticalSection` with per-CS native mutex
2. Eliminate `global_critical_region` dependency for guest CS operations
3. Verify: no convoy effects during init

### Phase 3: Audio Init Fix

1. Stub `sub_82955BE0` (XAudio streaming init hang)
2. Null-guard vtable calls on `0x831CC904` (audio device manager)
3. Verify: audio init completes through all 80 calls

### Phase 4: Streaming Resource Processing

1. Hook `sub_82852D18` to skip vtable[2] tree visitor (`sub_8286C238`)
2. Verify: streaming activation completes without hanging
3. Consider true async I/O replacement for performance

### Phase 5: Render Pipeline (Requires Host GPU)

1. Populate vtable entries for scene render objects (vtable[6], vtable[10])
2. Populate audio endpoint vtable (vtable[17])
3. Route through host GPU rendering pipeline
4. Eliminate 2.37M MISSING-FUNC calls from sub_8291DF00
