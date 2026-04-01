# PPC Analysis: sub_82556190 — Secondary Streaming Thread Init

**Source**: `gta4-recomp/generated/gta4_recomp.26.cpp`, lines 93069–93178
**Function name**: `sub_82556190`
**Position in init chain**: Called from `sub_821FC1F8` (main subsystem init batch), position #33 out of ~47 calls, immediately after `sub_8225C010`.

---

## Function Signature

```
void sub_82556190(PPCContext& ctx, uint8_t* base)
```

No parameters (called with no arguments set before the `bl`). The function reads and writes globals directly.

---

## Purpose

Initialises the **secondary (non-audio) streaming thread subsystem** — the file I/O streaming manager responsible for loading world geometry, textures, and other non-audio assets. It:

1. Zero-initialises two streaming slot arrays
2. Clears the streaming subsystem's internal slot-state table
3. Creates one OS thread via `sub_8285D948` (the **hang point**)
4. Stores the thread handle
5. Creates a streaming OS event/semaphore object via `sub_82849A50`
6. Stores the event handle

---

## Execution Trace (line-by-line)

### Phase 1 — Global flag write (line 93095)

```
r10 = 0x82AB0000
stw 1, -18152(r10)   → STORE_U32(0x82AAB918, 1)
```

Writes `1` to `0x82AAB918`. This is a **streaming subsystem initialised flag** — set before anything else so re-entrant calls are detected.

### Phase 2 — Slot array zero-init, pass 1 (lines 93087–93098)

```
sub_829FF840(r3=0x8307CA50, r4=0, r5=3240)
```

`sub_829FF840` is the game's `memset` equivalent. Zeroes 3240 bytes at `0x8307CA50` — the **streaming slot array** (90 × 36-byte entries).

### Phase 3 — Slot array zero-init, pass 2 (lines 93101–93109)

```
sub_829FF840(r3=0x8307BAD8, r4=0, r5=3960)
```

Zeroes 3960 bytes at `0x8307BAD8` — a second streaming slot table (likely the I/O request queue or pending-load queue).

### Phase 4 — Internal state reset (lines 93110–93112)

```
sub_82554E60()
```

`sub_82554E60` walks a fixed-size array at `0x8307BAD8` (same base) in 264-byte strides over 4040 bytes, clearing handle/state fields; then walks `0x8307CA50` in 36-byte strides over 3240 bytes clearing two words per entry. This is an **element-by-element constructor reset** — ensures each streaming slot object's pointers and counters are null even if the prior memset missed aligned fields.

### Phase 5 — **Thread creation (HANG POINT)** (lines 93113–93131)

Arguments computed:
| Register | Value | Meaning |
|-|-|-|
| r3 | `0x820259CC` | Thread object / context struct |
| r4 | `0x10000` (65536) | Stack size |
| r5 | `0x8000` (32768) | Thread priority |
| r6 | `1` | Processor affinity mask (core 1) |
| r7 | `0x820259BC` | Thread name string pointer |
| r8 | `0` | Creation flags |

```
sub_8285D948(r3=0x820259CC, r4=65536, r5=32768, r6=1, r7=0x820259BC, r8=0)
```

Return value (`r3`) is written to `0x8307BAD4` — the **streaming thread handle**.

### Phase 6 — Streaming sync object (lines 93138–93154)

```
sub_82849778(r3=0)          ; creates/inits event/semaphore — called with null
stw r3, -17712(r11)         → 0x8307BAD0  ← event handle result (null init)
```

`sub_82849778` is a thin wrapper: it calls `sub_82A12EB8` with `r4=passed_r3`, `r5=32767`, `r6=0`, `r3=0`. This is `NtCreateSemaphore` or an equivalent handle factory. With `r3=0` (null handle), it inits a free-list head.

### Phase 7 — Streaming scheduler object (lines 93141–93165)

Arguments:
| Register | Value | Meaning |
|-|-|-|
| r3 | `0x82556188` | Scheduler object (8 bytes before this fn — separate global struct) |
| r4 | `0` | Initial count |
| r5 | `2048` | Queue depth |
| r6 | `0` | Flags |
| r7 | `0x820259BC` | Scheduler name |
| r8 | `1` | Enable flag |
| r9 | `1` | Start flag |

```
sub_82849A50(r3=0x82556188, r4=0, r5=2048, r6=0, r7=0x820259BC, r8=1, r9=1)
```

`sub_82849A50` allocates a scheduler/semaphore node via `sub_82A13110` (= `CreateThread` or `NtCreateSemaphore` depending on the overload), links it into the subsystem's free-list at `r30+708`, increments count at `r30+704`.

Return value written to `0x8307D6F8` — the **streaming scheduler handle**.

---

## Complete Call List

| Address | Call | Parameters | Purpose |
|-|-|-|-|
| `0x82AAB918` | STORE_U32 | 1 | Mark subsystem as initialising |
| `0x829FF840` | memset | `0x8307CA50`, 0, 3240 | Zero streaming slot array |
| `0x829FF840` | memset | `0x8307BAD8`, 0, 3960 | Zero I/O request queue |
| `0x82554E60` | slot_reset | (none) | Element-wise clear of both arrays |
| `0x8285D948` | create_thread | see Phase 5 | Launch streaming I/O thread |
| `0x82849778` | init_sem | r3=0 | Init event/semaphore free-list |
| `0x82849A50` | create_scheduler | see Phase 7 | Create streaming scheduler object |

---

## Global State Written

| Address | Value | Meaning |
|-|-|-|
| `0x82AAB918` | 1 | Streaming subsystem initialised flag |
| `0x8307CA50`–`0x8307D678` | 0 | Streaming slot array (3240 bytes) zeroed |
| `0x8307BAD8`–`0x8307C97F` | 0 | I/O request queue (3960 bytes) zeroed |
| `0x8307BAD4` | thread handle | Return value from `sub_8285D948` |
| `0x8307BAD0` | 0 | Event handle (sub_82849778 r3=0 returns 0) |
| `0x8307D6F8` | scheduler handle | Return value from `sub_82849A50` |

---

## Path to the Hang: sub_8285D948 → sub_8285D610

### sub_8285D948 (recomp.56.cpp, line 8456)

1. Allocates a 1080-byte block via `sub_821B3510(1080)` (game allocator)
2. If allocation succeeds: loads thread config flags from `0x82B08210`, ANDs with r8 (=0 → r8=0), then calls `sub_8285D610`
3. If allocation fails: skips to the counter-increment path

The allocation check means **if the allocator returns non-null (normal case), sub_8285D610 is always called**.

### sub_8285D610 (recomp.56.cpp, line 7988) — **The actual hang**

1. Inits a 1024-entry ring buffer via `sub_8285FE48`
2. Calls `sub_82849778(r3=0)` — XMA source / streaming source init. On Xbox 360 this blocks on XMA hardware. **On PC this is an unhooked recompiled function that calls `sub_82A12EB8` which maps to `NtCreateSemaphore` — if the semaphore object is not set up, this can block or return a bad handle.**
3. Calls `sub_8285D500` → `sub_82849A50` → `sub_82A13110` — **XCreateThread / NtCreateThread**.

`sub_82A13110` at `0x82A13110` is **not in the rexcrt hook list** (the CRT audit only covers `0x82A131B0` = CreateFileA and similar). It is an unhooked thread-creation function. When called, it attempts to create a native Xbox 360 kernel thread, which on the recomp calls into the host kernel emulation layer. If that layer does not have a `CreateThread` / `NtCreateThread` shim registered for `0x82A13110`, the call either crashes or blocks waiting for a kernel response.

### Full call chain

```
sub_821FC1F8            (init batch, recomp.5.cpp:1099)
  └─ sub_82556190       (this function, line 93131)
       └─ sub_8285D948  (recomp.56.cpp:8490)
            └─ sub_821B3510(1080)  [allocate]
            └─ sub_8285D610        (HANG)
                 ├─ sub_8285FE48   (ring buffer init)
                 ├─ sub_82849778   (XMA/semaphore — may block)
                 └─ sub_8285D500
                      └─ sub_82849A50
                           └─ sub_82A13110  ← XCreateThread (UNHOOKED)
```

---

## Comparison with sub_82478AF8 (Audio Streaming Init)

| Aspect | sub_82478AF8 (audio) | sub_82556190 (world/asset streaming) |
|-|-|-|
| Called from | `sub_821FC1F8` position #31 | `sub_821FC1F8` position #33 |
| Thread function | `sub_8285D948` with audio device args | `sub_8285D948` with streaming manager args |
| Hang path | `sub_8285D610` inside `sub_827AD200` deep chain | `sub_8285D610` directly from `sub_8285D948` |
| `sub_8285D948` r3 | audio device struct | `0x820259CC` (streaming thread obj) |
| Stack size (r4) | 65536 | 65536 |
| Priority (r5) | 32768 | 32768 |
| Slot array | 176-byte voice descriptors | 36-byte streaming slots + 264-byte I/O requests |
| Scheduler | `sub_82849A50` | `sub_82849A50` |

Both functions are **structurally identical** — they initialise a subsystem's slot arrays, call `sub_8285D948` to create a worker thread, then register a scheduler object. The audio init (`sub_82478AF8`) goes through more intermediate layers before reaching `sub_8285D610`; the streaming init (`sub_82556190`) reaches it in one hop. Both hang for the same root reason: `sub_82A13110` is unhooked.

---

## RTTI / Debug Strings

No RTTI strings appear directly in `sub_82556190`. Adjacent functions use:
- `0x820259BC` — a short string pointer (thread name), likely `"strm"` or `"io_thread"` based on position in data section
- The scheduler at `0x82556188` is 8 bytes before `sub_82556190` in the text section, consistent with a static C++ object whose constructor is `sub_82556190`

---

## Post-sub_82556190 Init Functions — Hang Check

Functions called after `sub_82556190` in `sub_821FC1F8` (#34–47):

| Address | File | Calls 8285D948/D610? |
|-|-|-|
| `sub_822F3740` | recomp.10.cpp | No |
| `sub_8237C250` | recomp.14.cpp | No |
| `sub_822B2010` | recomp.5.cpp | No |
| `sub_823BFA98` | recomp.15.cpp | No |
| `sub_8228E3A0` | recomp.5.cpp | No |
| `sub_822BCC20` | recomp.5.cpp | No |
| `sub_822BDD98` | recomp.5.cpp | No |
| `sub_823A2108` | recomp.14.cpp | No |
| `sub_82552D08` | recomp.26.cpp | No |
| `sub_821CA7E8` | recomp.3.cpp | No |
| `sub_825169C8` | recomp.25.cpp | No |
| `sub_825030B8` | recomp.24.cpp | No |
| `sub_8251CA08` | recomp.25.cpp | No |

**No function after `sub_82556190` in the batch calls `sub_8285D948` or `sub_8285D610`.** The hang is confined to this function.

---

## Is Stubbing Safe?

### Option A: Stub `sub_82556190` entirely (return immediately)

**Side effects:**
- `0x82AAB918` (init flag) will remain 0 — any downstream code checking this may skip expected init
- Streaming slot arrays left uninitialised — world streaming requests will likely crash or NOP
- No streaming thread launched — no assets will stream from disk post-boot
- The game may reach the main loop but will fail to load any geometry or textures

**Safe for:** testing the boot sequence past this hang. **Not safe for:** normal gameplay.

### Option B: Stub `sub_8285D610` to return immediately

**Side effects:**
- Ring buffer at `r3+8..r3+1032` left uninitialised
- Streaming handles never created
- Thread never launched
- `sub_8285D948` returns the allocated 1080-byte block with partial init; this pointer is stored at `0x8307BAD4`

**Same as Option A in practice** — streaming will not function, but the hang is resolved.

### Option C: Hook `sub_82A13110` with a proper CreateThread shim (recommended)

`0x82A13110` needs a host `CreateThread` hook similar to the `NtCreateThread` hooks already present in the kernel layer. Once hooked, `sub_8285D610` will complete normally and streaming threads will run.

### Option D: Hook `sub_82849778` to return 0

Prevents the XMA/semaphore init from blocking. The ring buffer inits but `sub_8285D500` will still call `sub_82A13110`. Partial mitigation only.

---

## Recommended Fix

```cpp
// In kernel/streaming_hooks.cpp or equivalent
// Hook sub_82A13110 (XCreateThread analog at 0x82A13110)
PPC_FUNC(sub_82A13110) {
    // r3=0 (unused), r4=stack_size, r5=thread_func_ptr?, r6=thread_obj?, ...
    // Map to host CreateThread or platform thread API
    // Store host thread handle back into r3
    ctx.r3.u64 = 0; // temporary stub: return null handle (non-blocking)
    return;
}
```

For a full fix, map `sub_82A13110` to `XCreateThread` wrapper analogous to the existing `CreateFileA` / `ReadFile` rexcrt hooks. The thread's entry point will be a recompiled PPC function pointer stored in the thread object at `0x820259CC`; the host thread must call `rex::Runtime::ExecuteFunc(entry, ctx)`.

As an immediate unblock (testing only), stub `sub_8285D610` at `0x8285D610` to zero fields `[0]`, `[4]`, `[6]` and return:

```cpp
PPC_FUNC(sub_8285D610) {
    PPC_STORE_U32(ctx.r3.u32 + 0, 0);
    PPC_STORE_U16(ctx.r3.u32 + 4, 0);
    PPC_STORE_U16(ctx.r3.u32 + 6, 0);
    return;
}
```
