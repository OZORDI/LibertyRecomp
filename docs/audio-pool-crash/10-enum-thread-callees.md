# Agent 10 — Enumerate Content Thread Callees

## Header

Crash:    `sub_828C2300+0x34` (`stw r11, 0(r31)`, r31=0x20)
LR-frame: `sub_8227F2E8`
Thread:   tid=12 "Enumerate Content" = `sub_825FBB68` @ 0x825FBB68 (~0x148 bytes)

| addr | name | size | callees | hot? | leaf? | hooked? |
|-|-|-|-|-|-|-|
| 0x82849790 | sub_82849790 | ~0x48 | sub_82A13040 | no | no | no |
| 0x82849860 | sub_82849860 | ~0x50 | sub_82A12F50 | YES | no | no |
| 0x82849918 | sub_82849918 | ~0x08 | sub_82A12B60 | no | YES | no |
| 0x8285FF50 | sub_8285FF50 | ~0x50 | RtlEnterCriticalSection | YES | no | no |
| 0x8285FFA0 | sub_8285FFA0 | ~0x30 | RtlLeaveCriticalSection | YES | YES | no |

All five are **unhooked** in `LibertyRecomp/`. Verified via `grep -i` across the entire `LibertyRecomp/` source tree — zero references to any of these addresses or symbols.

## Underlying Kernel Targets

Resolved via recomp scaffolds:

```
sub_82849790 -> sub_82A13040 -> sub_82A1A450 -> NtWaitForSingleObjectEx (0-timeout, with retry on STATUS_ALERTED 0x101)
sub_82849860 -> sub_82A12F50 -> NtReleaseSemaphore (+ sub_82A18C38 on negative status)
sub_82849918 -> sub_82A12B60 -> sub_82A1A200 -> KeDelayExecutionThread (timeout in -10000-units)
sub_8285FF50 -> RtlEnterCriticalSection (refcounted-holder pattern)
sub_8285FFA0 -> RtlLeaveCriticalSection (refcounted-holder pattern)
```

## C++ Reconstructions

### sub_82849790 — `bool TryAcquireWaitObject(HANDLE)`

```cpp
// Returns true if the kernel object is signaled within 0 ticks (poll).
// Equivalent to WaitForSingleObject(h, 0) == WAIT_OBJECT_0.
bool sub_82849790(HANDLE h) {
    if (h == nullptr) return false;       // r3 == 0 early-out path
    // sub_82A13040(h, 0) -> sub_82A1A450(h, /*alertable*/0, /*timeout*/0ms)
    NTSTATUS s = NtWaitForSingleObjectEx(h, /*WaitMode*/1, /*Alertable*/0,
                                         /*Timeout*/&zero_timeout);
    // PPC: cntlzw + rlwinm,27,31,31 -> returns (s == 0)
    return s == STATUS_SUCCESS;
}
```

State: pure read (kernel obj). No memory writes. **Cannot corrupt anything.**

### sub_82849860 — `bool ReleaseSemaphoreOne(HANDLE)`

```cpp
// Releases 1 token on a semaphore. Returns true on success, false on failure
// (with a debug-print/log via sub_82A18C38).
bool sub_82849860(HANDLE h) {
    if (h == nullptr) return false;
    LONG prev = 0;
    // sub_82A12F50(h, 1, 0) -> NtReleaseSemaphore(h, 1, &prev)
    NTSTATUS s = NtReleaseSemaphore(h, /*Count*/1, /*PrevCount*/nullptr);
    if (s < 0) {
        sub_82A18C38(/*log err*/);
        return false;
    }
    // PPC: cntlzw,rlwinm,xori 1 -> returns (s >= 0) ? 1 : 0
    return true;
}
```

State: kernel object semaphore count++. No process-memory writes.

### sub_82849918 — `void Sleep(uint32_t ms)`

```cpp
// Tail-call sleep wrapper (8 bytes -- just `b sub_82A12B60`).
// sub_82A12B60(ms, 0) -> sub_82A1A200(ms, /*Alertable*/0)
//   -> NtDelayExecution / KeDelayExecutionThread with -10000 * ms (100ns units).
void sub_82849918(uint32_t ms) {
    KeDelayExecutionThread(/*UserMode*/1, /*Alertable*/0,
                           /*Interval*/-10000LL * (int64_t)ms);
}
```

State: thread-local timer only. No memory writes.

### sub_8285FF50 — `void ScopedRefcountLock_Acquire(holder_t* h, CRITICAL_SECTION* cs)`

```cpp
// holder_t layout (8 bytes): [0]=u32 refcount, [4]=CRITICAL_SECTION*
struct holder_t { uint32_t refcount; PRTL_CRITICAL_SECTION cs; };

void sub_8285FF50(holder_t* h, PRTL_CRITICAL_SECTION cs) {
    h->cs       = cs;     // stw r4, 4(r31)
    h->refcount = 1;      // stw r11, 0(r31)  -- always 1 here
    if (cs->LockCount != 0) {        // lwz r11, 0(r4); r4 = cs
        // Only Enter when the critsec object has been initialized
        // (LockCount field non-zero on initialized critsec on Win32 era;
        // on Xbox360 NT this is the "spin count or Owner" cell).
        RtlEnterCriticalSection(cs);
    }
    // returns h
}
```

State: writes 8 bytes through `holder_t* h` (caller's stack slot). Acquires critsec.

### sub_8285FFA0 — `void ScopedRefcountLock_Release(holder_t* h)`

```cpp
void sub_8285FFA0(holder_t* h) {
    if ((int32_t)h->refcount <= 0) return;          // beqlr cr6
    h->refcount--;                                  // addic. r11,r11,-1
    if (h->refcount != 0) return;                   // bnelr
    PRTL_CRITICAL_SECTION cs = h->cs;               // lwz r3, 4(r3)
    if (cs->LockCount == 0) return;                 // beqlr cr6
    RtlLeaveCriticalSection(cs);                    // tail call
}
```

State: writes 4 bytes (decrements refcount). Releases critsec.

## Call Chain Analysis — Do Any Lead to the Crash?

### The crash function `sub_828C2300`

Disassembly shows it **always** uses a fixed global pointer:

```
lis r11, -31972     ; r11 = 0x831C0000  (Python: (-31972 & 0xFFFF) << 16)
addi r31, r11, 11576 ; r31 = 0x831C2D38
lwz r11, -16(r31)   ; r11 = *(0x831C2D28)  -- handle
... if non-null: bl sub_828BF270 ; release
stw r11, 0(r31)     ; *(0x831C2D38) = 0     <-- offset +0x34 = CRASH SITE
```

So the **expected** r31 is `0x831C2D38`. A crashing `r31=0x20` is impossible from sub_828C2300's own prologue — meaning **the disassembled crash address belongs to a different function** that happens to land at the same offset, OR the LR/FP unwinder lied.

The reported LR-frame `sub_8227F2E8` is a HUD/render fn:
- It calls `sub_828C19C0`, `sub_8227EE90`, `sub_828C6568`, then four `sub_828C2290` calls (each consuming f1..f8 floats — vector batch-render), then `sub_828C2300` (vector clear), then `sub_828C6500` and `sub_828C60A0` (release).
- Callers: `sub_8227F5B8`, `sub_8227F608`. Their callers (`sub_8214DBD0`, `sub_821506E8`, `sub_821B5C90`, `sub_821F1EE0`, `sub_8229D8A8`, `sub_8218D7A8`, `sub_8218E2C0`, `sub_8218E318`, `sub_821F1670`) are all in the 0x8214-0x822A HUD/draw code range.

### Enum thread call paths from sub_825FBB68

```
sub_825FBB68 (loop):
    XNotifyGetNext(0xB, ...)
    if nothing -> sub_82849790(g_event_or_mutex)         [POLL]
    if signaled:
        sub_8285FF50(&local_holder, &g_critsec)          [LOCK]
        if g_state != -1:
            sub_82A12710(...)                            [NtCreateFile or similar IO]
            on success: sub_82A11F50(...)                [string concat / setup path]
                        rexcrt_CloseHandle(handle)
                        sub_825FB998()                   [completion notifier]
            indirect call via vtable[3] of g_callback_obj
        sub_82849860(g_semaphore)                        [release]
        sub_8285FFA0(&local_holder)                      [UNLOCK]
    sub_82849918(16)                                     [Sleep(16)]
    goto loop
```

**None of `sub_82849790`, `sub_82849860`, `sub_82849918`, `sub_8285FF50`, `sub_8285FFA0` ever call `sub_8227F2E8` or `sub_828C2300`.** Verified by exhausting their callee trees: they bottom out in NT syscalls (NtWait, NtRelease, NtDelay, RtlEnter, RtlLeave). None touch the render globals at 0x831C2D28/0x831C2D38.

The vtable dispatch at `bctr` to `vtable[3]` of `g_callback_obj` (loaded from `r27 = 0x82A05E78 + 0x0C`) is the only indirect path out of the enum loop. That callback could be anything — but it would have to itself reach `sub_8227F2E8`, which is part of a render-only call site set.

## Existing Hook Map (LibertyRecomp/ source)

| function | hooked? | notes |
|-|-|-|
| sub_82849790 | NO | unhooked NT wait poll |
| sub_82849860 | NO | unhooked NT semaphore release |
| sub_82849918 | NO | unhooked NT delay |
| sub_8285FF50 | NO | unhooked critsec lock |
| sub_8285FFA0 | NO | unhooked critsec unlock |
| sub_825FBB68 | NO | unhooked enum-loop thread proc |
| sub_828C2300 | NO | unhooked render-vector clear |
| sub_8227F2E8 | NO | unhooked HUD draw fn |

`grep` of the entire `LibertyRecomp/` directory for any of these eight addresses (both `0x` and bare hex, both case variants) returns **zero hits**. The only LibertyRecomp-emulated layer that the enum thread crosses is XAM: `j_XamContentCreateEnumerator` and `XNotifyGetNext`, both of which are implemented in `glue/rexglue-sdk-main/src/kernel/xam/{xam_content,xam_notify}.cpp` (not in the LibertyRecomp project's own source).

## DLC Race Hypothesis

Project memory lists a **DLC auto-installer that runs in a background thread** during `Setup()`. The crash state is described as **"audio pool teardown"**. Plausible races:

1. **DLC zip extraction on background thread → re-mount of HostPathDevice**. Per project memory: *"HostPathDevice snapshots directory at mount time — files must exist before VFS init."* If the auto-installer extracts DLC after VFS init and triggers a re-snapshot or invalidates a `device_t*` while the audio engine holds a stale pointer, a subsequent audio-pool teardown could free buffers from beneath the render thread.

2. **XContentCreate emulation under the enum thread** — `xam_content.cpp` allocates `XCONTENT_DATA` buffers and indexes content packages. Re-enumeration during DLC install could trigger duplicate `XamContentCreate` paths, double-freeing a content package handle that audio code mapped (e.g. SOUNDS.RPF inside a DLC LIVE STFS package).

3. **The recomp's `j_XamContentCreateEnumerator` thunks into LibertyRecomp's emulation, which on Mac may issue host-side directory scans and trigger Cocoa autorelease churn**. If this allocates from the same heap region as the audio pool (very possible under the o1heap allocator), heap corruption in the audio pool free-list would manifest as `r31=0x20` (a tiny offset that looks like a pool-header field shifted by one).

**However:** the crash chain itself is render-thread (HUD draw → `sub_8227F2E8` → `sub_828C2300`). The enum thread does NOT touch the render globals at 0x831C2D38. So the race is not "enum directly corrupts the crash site"; it would be **"DLC install thread (or enum side-effect) corrupts a heap region whose later use by audio-pool/render produces this fault"**. That is consistent with the AUDIO POOL TEARDOWN signal preceding the crash.

## Conclusion

**The Enumerate Content thread is the unlucky thread that surfaces the corruption, not the culprit.**

Evidence:
1. None of the 5 callees (poll, release, sleep, lock, unlock) write to global memory anywhere near the crash region 0x831C2D28/0x831C2D38.
2. The reported crash function `sub_828C2300` only ever uses r31=0x831C2D38; r31=0x20 means the *actual* faulting frame is a different caller (mis-attributed by the unwinder) OR the global `0x831C2D38` was overwritten with `0x20` (single-byte heap corruption shift). Either way, the corruption is not produced by sub_825FBB68's own ops.
3. The LR-frame `sub_8227F2E8` is a HUD/render function (4× `sub_828C2290` float-batch then `sub_828C2300` clear) and its caller chain is entirely 0x8214-0x822A render code — i.e. the render/HUD thread.
4. The "Enumerate Content" thread is just the first thread to issue a syscall after the corruption window, so it's where the fault surfaces in the trace.

**Real culprit candidates** (in priority order):
- DLC auto-installer thread racing audio engine init / VFS re-mount.
- Recomp emulation of `XamContentCreateEnumerator` or `XContentCreate` allocating from the o1heap region shared with the audio pool.
- `glue/rexglue-sdk-main/src/kernel/crt/file.cpp` (modified per `git status`) corrupting the heap if FindFirstFile/FindNextFile state leaks across re-enumerations.

**Recommended next steps for other agents:**
- Audit `xam_content.cpp` `XamContentCreateEnumerator` allocation lifetime.
- Add a watchpoint at `0x831C2D38` (the `sub_828C2300` global) to catch the writer.
- Check whether the DLC auto-installer's extraction thread serializes against `rex::Runtime::Setup()` audio init.
