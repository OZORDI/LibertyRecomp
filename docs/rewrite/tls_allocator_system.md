# TLS Allocator System — Full Trace

## TLS Memory Layout

### Addressing Chain
```
r13 -> PCR (Processor Control Region)
PCR[0] = tls_ptr -> TLS block (separate allocation)
TLS[offset] = the actual slot value
```

Two paths create threads:

**XThread path** (`xthread.cpp`):
- `pcr_address_` = `SystemHeapAlloc(0x2D8)`
- `tls_static_address_` = `SystemHeapAlloc(tls_slots * 4 + tls_extended_size)`
- Default: 1024 slots x 4 bytes = 4096 bytes
- `pcr->tls_ptr = tls_static_address_`
- `ctx.r13 = pcr_address_`

**GuestThread path** (`guest_thread.cpp`):
- Single contiguous block: `[PCR 0xAB0][TLS 0x700][TEB 0x2E0][Stack 0x80000]`
- `pcr->tls_ptr = tlsAddr` (points to TLS region within block)
- `ctx.r13 = pcrAddr`

### Offset Verification

| Slot | Decimal | Hex | Name | Fits in 0x700 (1792)? |
|-|-|-|-|-|
| TLS[1668] | 1668 | 0x684 | push/pop refcount | Yes |
| TLS[1672] | 1672 | 0x688 | saved (previous) allocator | Yes |
| TLS[1676] | 1676 | 0x68C | current allocator | Yes |
| TLS[1680] | 1680 | 0x690 | target/default allocator | Yes |

All 4 slots are 32-bit pointers, 4 bytes apart, within the TLS allocation.

---

## State Machine

The TLS allocator is a **one-deep stack with refcount optimization**.

### States
```
UNINITIALIZED:  [1668]=0  [1672]=0  [1676]=0     [1680]=0
INITIALIZED:    [1668]=0  [1672]=0  [1676]=alloc  [1680]=alloc
PUSHED (same):  [1668]=N  [1672]=0  [1676]=alloc  [1680]=alloc
PUSHED (diff):  [1668]=0  [1672]=old [1676]=new   [1680]=new
```

### Transitions
```
INIT (sub_82849940):
  [1676] = descriptor[8]
  [1680] = descriptor[8]
  (refcount and saved remain 0 from TLS zero-init)

SET TARGET (caller responsibility):
  [1680] = new_allocator  (done by caller before push)

PUSH (sub_828470E0, 163 call sites):
  if [1676] == [1680]:     // same allocator, no swap needed
    [1668]++               // increment refcount
  else:                    // different allocator, swap
    [1672] = [1676]        // save current
    [1676] = [1680]        // activate target

POP (sub_82847120, 168 call sites):
  if [1668] > 0:           // nested same-alloc push
    [1668]--               // decrement refcount
  else:                    // refcount==0, restore
    [1676] = [1672]        // restore saved
    [1672] = 0             // clear saved
```

---

## Function-by-Function Analysis

### sub_82849940 — Thread TLS Initialization
**File**: `gta4_recomp.55.cpp` line 8598
**Called by**: Indirectly via function pointer (thread entry callback registered with ExCreateThread)
**Called**: 1 site (function table only)

**Behavior**:
1. Copies 44-byte thread descriptor from r3 to stack (via sub_82A00DC0 = memcpy variant)
2. Computes global thread manager address: `lis -31975 + 10624` = **0x83192980**
3. Acquires critical section at manager+716 (sub_821D5F70)
4. Links descriptor into manager's linked list at +708, increments count at +704
5. Reads from descriptor after `lwsync` barrier:
   - `stack+80` (descriptor[0]) = callback function pointer -> mtctr
   - `stack+84` (descriptor[4]) = callback argument -> r3 (unused here, passed to callback)
   - `stack+88` (descriptor[8]) = allocator pointer -> written to TLS
6. **WRITES**:
   - `TLS[1676] = descriptor[8]` (stwx r10, r11, r6 where r6=1676)
   - `TLS[1680] = descriptor[8]` (stwx r10, r11, r5 where r5=1680)
7. Releases critical section (stores 0 to lock)
8. Calls callback via bctrl

**TLS access**: WRITES TLS[1676], TLS[1680]. READS none (besides TLS base via r13).

### sub_828470E0 — Push Allocator (Primary)
**File**: `gta4_recomp.55.cpp` line 2436
**Call sites**: 163 across 16 generated files
**Not hooked** (runs as generated code)

**Behavior**:
1. `r11 = *(r13)` — load tls_ptr
2. `r9 = TLS[1676]` (current), `r10 = TLS[1680]` (target)
3. If `r9 == r10` (current == target):
   - `TLS[1668]++` (increment refcount)
   - return
4. If `r9 != r10` (current != target):
   - `TLS[1676] = r10` (current = target)
   - `TLS[1672] = r9` (saved = old current)
   - return

**TLS access**: READS [1676], [1680], [1668]. WRITES [1676] or [1668], and optionally [1672].

### sub_82847120 — Pop Allocator (Primary)
**File**: `gta4_recomp.55.cpp` line 2475
**Call sites**: 168 across 16 generated files
**Not hooked** (runs as generated code)

**Behavior**:
1. `r11 = *(r13)` — load tls_ptr
2. `r10 = TLS[1668]` (refcount)
3. If `r10 > 0`:
   - `TLS[1668]--` (decrement refcount)
   - return
4. If `r10 == 0`:
   - `r7 = TLS[1672]` (saved allocator)
   - `TLS[1672] = 0` (clear saved)
   - `TLS[1676] = r7` (restore previous allocator)
   - return

**TLS access**: READS [1668], [1672]. WRITES [1668] or ([1672], [1676]).

### sub_827D85E0 — Push Allocator (Hooked, Unused)
**File**: EXCLUDED from codegen. Only exists as hook in `imports.cpp` line 837.
**Call sites in generated code**: **ZERO**
**Hook wraps**: `__imp__sub_827D85E0` (extern forward-decl only, no implementation exists)

The hook guards against TLS[1680]==0 causing TLS[1676] to be zeroed. If target is invalid but current is valid, it fakes a same-allocator push (increments refcount) instead.

**Important**: This function has zero callers in the generated code. The hooks exist as defensive guards but may never fire. The actual push used by GTA IV is sub_828470E0.

### sub_827D8620 — Pop Allocator (Hooked, Unused)
**File**: EXCLUDED from codegen. Only exists as hook in `imports.cpp` line 870.
**Call sites in generated code**: **ZERO**

The hook guards against TLS[1672]==0 causing TLS[1676] to be zeroed on pop. If refcount==0 and saved==0 but current!=0, it skips the pop entirely.

**Same note**: Zero callers. The actual pop is sub_82847120.

### sub_8218BE28 — malloc (Hooked)
**File**: EXCLUDED from codegen. Hook in `imports.cpp` line 901.
**Behavior**: Reads TLS[1676]. If valid allocator (vtable in 0x82000000-0x84000000), calls through to original. If invalid/null, routes to RexGlue SystemHeapAlloc.

**TLS access**: READS [1676] only.

### sub_8218BE50 — malloc with alignment (Hooked)
**File**: EXCLUDED from codegen. Hook in `imports.cpp` line 939.
**Behavior**: Same as sub_8218BE28 but r4 = explicit alignment. Falls back to SystemHeapAlloc with rounded-up size.

**TLS access**: READS [1676] only.

### sub_821B3510 — operator new
**File**: `gta4_recomp.2.cpp` line 83062
**Not hooked** (runs as generated code)

**Behavior**:
1. `r11 = *(r13)` — load tls_ptr
2. `r3 = TLS[1676]` — load current allocator
3. `r11 = *(r3)` — load vtable
4. `r11 = vtable[8]` — load Allocate method (index 2)
5. Tail call: `Allocate(allocator, size=r4, align=16, flags=0)`

**TLS access**: READS [1676] only.

### sub_821B3538 — operator new (aligned variant)
**File**: `gta4_recomp.2.cpp` line 83088
**Behavior**: Same as sub_821B3510 but r5 = caller-specified alignment.

**TLS access**: READS [1676] only.

### sub_821B3560 — operator delete
**File**: `gta4_recomp.2.cpp` line 83116
**Behavior**: If ptr != 0, reads TLS[1676], dispatches vtable[12] (Free).

**TLS access**: READS [1676] only.

### sub_82849860 — Resource event creator (via sub_8284C290 trampoline)
**File**: `gta4_recomp.55.cpp` line 8426
**Behavior**: If r3 != 0, calls sub_82A12F50 (NtCreateEvent). Returns success/failure.

**TLS access**: NONE directly. The Phase 4 hook on sub_8284C290 (its trampoline) does a post-call check of TLS[1676] as a corruption guard.

### sub_8284C290 — Trampoline to sub_82849860 (Phase 4 Hooked)
**File**: `gta4_recomp.55.cpp` line 14750
**Behavior**: Tail calls sub_82849860.
**Hook** (`imports.cpp` line 984): Calls through, then checks if TLS[1676] was corrupted (non-zero but invalid vtable). If corrupted, zeros TLS[1676] and TLS[1680] so the fallback allocator handles subsequent allocations.

---

## Inline TLS Access Patterns

### Direct TLS[1676] reads (86 sites, 25 files)
Pattern: `lwz r11,0(r13); li rX,1676; lwzx rY,rX,r11; lwz vtable,0(rY); lwz method,N(vtable); bctr`
These are allocation/deallocation calls through the current allocator's vtable.

### Direct TLS[1680] reads (46 sites, 11 files)
Same vtable dispatch pattern but using the default allocator. These bypass push/pop and always use the thread's default allocator. Used for system allocations that shouldn't be affected by temporary allocator switches.

### Inline push pattern (found in gta4_recomp.42.cpp and others)
Some functions inline the push logic directly rather than calling sub_828470E0:
```
lwz r30, 0(r13)         // tls_ptr
li r11, 1680
li r28, 1676
lwzx r11, r30, r11      // r11 = TLS[1680] (target)
lwzx r26, r30, r28      // r26 = TLS[1676] (current, saved for later restore)
stwx r11, r30, r28      // TLS[1676] = TLS[1680] (switch to target)
// ... do work with new allocator ...
stwx r26, r30, r28      // TLS[1676] = r26 (restore old)
```
This is a simplified inline push/pop that skips the refcount mechanism entirely. It directly saves and restores TLS[1676].

---

## Thread Initialization Sequence

### XThread Path (RexGlue `xthread.cpp`)
1. `XThread::Setup()` allocates:
   - TLS block: `SystemHeapAlloc(tls_slots*4 + tls_extended_size)` — zeroed
   - PCR block: `SystemHeapAlloc(0x2D8)` — zeroed
2. Sets `pcr->tls_ptr = tls_static_address_`
3. Sets `ctx.r13 = pcr_address_`
4. **TLS slots start at 0** (zeroed by `memory()->Fill`)
5. Game code calls sub_82849940 to initialize TLS[1676] and TLS[1680]

### GuestThread Path (`guest_thread.cpp`)
1. Allocates contiguous block: `[PCR 0xAB0][TLS 0x700][TEB 0x2E0][Stack 0x80000]`
2. Zeroes entire block
3. Sets `pcr->tls_ptr = tlsAddr` (offset 0xAB0 from block start)
4. Sets `ppcContext.r13 = pcrAddr`
5. **Latent bug**: `TLS_DEVICE_OFFSET = 1676` is the same slot as the RAGE current allocator. If `GetGuestDeviceAddr()` ever returns non-zero, it would write a non-allocator pointer to TLS[1676], corrupting the allocator chain. Currently safe because `SetGuestDeviceAddr()` is never called.

### Context Inheritance
When host code dispatches a guest function via `rex::ppc::function.h`:
```cpp
PPCContext newCtx{};
newCtx.r13 = currentCtx->r13;  // inherits TLS from calling thread
```
This means child function calls within the same thread see the same TLS.

---

## RAGE Allocator Vtable Layout

Inferred from vtable offset accesses across generated code:

| Offset | Index | Method |
|-|-|-|
| +0 | 0 | destructor |
| +4 | 1 | (unknown) |
| +8 | 2 | Allocate(size, align, flags) |
| +12 | 3 | Free(ptr) |
| +36 | 9 | (query method, used by sub_824E2B70) |
| +72 | 18 | (query method, used by sub_824E2B90) |
| +204 | 51 | (streaming method, used by inline push callers) |

---

## Global RAGE Thread Manager

Address: **0x83192980** (computed from `lis -31975; addi +10624`)

| Offset | Address | Field |
|-|-|-|
| +704 | 0x83192C40 | Thread count (uint32) |
| +708 | 0x83192C44 | Thread descriptor linked list head |
| +712 | 0x83192C48 | Pool initialized flag |
| +716 | 0x83192C4C | Critical section (12 bytes) |
| +728 | 0x83192C58 | Thread descriptor table (16 x 44-byte entries, stride 160) |

sub_828499E8 initializes the descriptor pool: 16 entries, each 44 bytes, linked via first dword as a free-list pointer.

---

## Race Condition Analysis

**TLS is inherently per-thread**: Each thread has its own TLS block accessed via its own r13. No cross-thread TLS access exists in the generated code.

**Safe patterns**:
- All TLS[1668-1680] reads/writes are thread-local
- sub_828470E0/sub_82847120 push/pop only touch the calling thread's TLS
- sub_8218BE28/sub_821B3510 malloc/new only read the calling thread's TLS[1676]

**Shared state with proper locking**:
- Global thread manager at 0x83192980 uses a critical section (+716)
- RAGE allocator objects are shared across threads but have internal thread-safety

**Risk areas**:
1. **Host threads without TLS init**: If a native (host) thread calls guest code without proper r13/TLS setup, TLS[1676] will be 0. The Phase 3 fallback allocator hook catches this.
2. **TLS_DEVICE_OFFSET collision**: `guest_thread.cpp` defines `TLS_DEVICE_OFFSET = 1676`, which is the same slot as the RAGE current allocator. Latent bug — currently safe because `SetGuestDeviceAddr()` is never called.
3. **One-deep stack limitation**: The push/pop only saves ONE previous allocator. If code pushes twice with different allocators without popping, the first saved value is lost. The refcount mechanism prevents this for same-allocator pushes.

---

## What a Native Rewrite Must Replicate

### Minimum viable implementation
1. **Thread-local storage**: 4 uint32 slots per thread (refcount, saved, current, default)
2. **Init**: Set current = default = allocator pointer on thread startup
3. **Push**: Compare current vs default; if same, increment refcount; if different, save current and switch
4. **Pop**: If refcount > 0, decrement; if 0, restore saved and clear
5. **Allocate**: Read current allocator, dispatch through vtable[2]
6. **Free**: Read current allocator, dispatch through vtable[3]

### Design considerations
- The native allocator can use C++ `thread_local` instead of PPC TLS
- The push/pop mechanism is a scoped allocator pattern (like RAII)
- The refcount handles nested same-allocator scopes efficiently
- The one-deep stack is sufficient because the game never nests different allocators more than one level

### Functions to replace

| Address | Role | TLS Access | Call Sites |
|-|-|-|-|
| sub_82849940 | Thread init | W: [1676], [1680] | Indirect (1) |
| sub_828470E0 | Push (primary) | R/W: [1668], [1672], [1676], [1680] | 163 |
| sub_82847120 | Pop (primary) | R/W: [1668], [1672], [1676] | 168 |
| sub_8218BE28 | malloc | R: [1676] | Hooked (excluded) |
| sub_8218BE50 | malloc aligned | R: [1676] | Hooked (excluded) |
| sub_821B3510 | operator new | R: [1676] | Many (via generated) |
| sub_821B3538 | operator new aligned | R: [1676] | Many (via generated) |
| sub_821B3560 | operator delete | R: [1676] | Many (via generated) |
| sub_827D85E0 | Push (unused) | Would R/W all 4 | 0 (hooked only) |
| sub_827D8620 | Pop (unused) | Would R/W all 4 | 0 (hooked only) |

### Inline access sites
- 86 direct TLS[1676] reads across 25 generated files (vtable dispatch to current allocator)
- 46 direct TLS[1680] reads across 11 generated files (vtable dispatch to default allocator)
- Several inline push/pop patterns that bypass the function call mechanism
