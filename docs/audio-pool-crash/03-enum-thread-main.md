# 03 — Enumerate Content Thread (sub_825FBB68 + sub_825FB998)

Agent 3 of 10 — investigation of the **tid=12 "Enumerate Content"** worker thread named in the crash dump. This document reconstructs the thread's full life-cycle and proves whether (and how) it can reach the audio-pool crash site at `sub_8227F2E8 → sub_828C2300`.

## Header

|key|value|
|-|-|
|Thread entry|`sub_825FBB68` @ `0x825FBB68` (328 B, 0 callers — entry point)|
|Direct helper|`sub_825FB998` @ `0x825FB998` (280 B, 1 caller = sub_825FBB68)|
|Thread name string|`"Enumerate Content"` @ `0x8203CF88`|
|Sleep period|16 ms (`sub_82849918(16)` ⇒ `KeDelayExecutionThread`)|
|Crashing instruction (per dump)|`stw r11, 0(r31)` at `sub_828C2300+0x34` with `r31=0x20`, `r9=0xFFE1E1E1`, `LR=0x8227F3AC`|

### Globals owned/touched by this thread

|address|computed from|usage|
|-|-|
|`0x83092BC0`|`-13460(r28)` where r28=`0x83096054`|XNotify queue handle (`HXAMENUM` from XNotifyCreateListener)|
|`0x83092BBC`|`-13464(r28)`|XAM content-enumerator handle (HENUM)|
|`0x83092BB8`|`r26+29568` where r26=`0x8308B838`|enumerator-finished callback receiver|
|`0x82AACFE4`|r29 = `lis -32085 + -12316`|content-enum **state byte** (offset 0 = state, offset 4 = "ready" flag)|
|`0x83096054`|r28 = `lis -31991 + 24660`|kernel state base (XNotify+enum handles around it)|
|`0x830960B0`|r27 = `lis -31991 + 24752`|"finalize" object whose vtable[3] = post-process callback|
|`0x83096058`|r10 in `sub_825FB998` (= `lis -31991 + 24664`)|head of intrusive linked-list of registered DLC content records (sentinel at +12); slot +32 = list head|
|`0x83096090`|r25 = `lis -31991 + 24720`|`{int refcount, RTL_CRITICAL_SECTION* pcs}` reentrancy guard for `sub_8285FF50`/`sub_8285FFA0`|
|`0x8308B838`|r26 = `lis -31991 + -18376`|content-context base (offset 264 = "name match buffer", +29568 = sub_82849860 arg)|

## Reconstructed C++ — sub_825FBB68

```cpp
// Thread entry (Xbox 360 "Enumerate Content" worker)
// Periodically polls XNotify for an XN_SYS_STORAGEDEVICESCHANGED-class event and
// re-walks DLC/save content. Wakes every 16 ms.
void Thread_EnumerateContent_825FBB68()
{
    constexpr void*  XNOTIFY_HANDLE_PTR    = (void*)0x83092BC0;   // [-13460(r28)]
    constexpr void*  XCONTENT_ENUM_PTR     = (void*)0x83092BBC;   // [-13464(r28)]
    constexpr void*  CB_RECEIVER_PTR       = (void*)0x83092BB8;   // [29568(r26)]
    constexpr auto*  POST_PROC_OBJ         = (Vtbl3*)0x830960B0;  // r27
    constexpr auto*  STATE                 = (EnumState*)0x82AACFE4;
    constexpr auto*  TYPE_FILTER           = (TypeFilter*)0x83096090;
    constexpr auto*  NAME_MATCH_BUF        = (uint8_t*)(0x8308B838 + 264);

    uint8_t saw_notify = 0;
    XNOTIFY_FRAME notif_frame;            // r1+88 .. r1+92  (id, param)
    XAM_OVERLAPPED ov;                    // r1+96 .. r1+96+96 — ctor by sub_8285FF50

    for (;;) {
        // ---- (1) drain XNotify queue --------------------------------------
        // signature: XNotifyGetNext(handle, dwMsgFilter=11, &id_out, &param_out)
        if (XNotifyGetNext(*(uint32_t*)XNOTIFY_HANDLE_PTR,
                           /*dwMsgFilter=*/11,                 // XN_SYS_*
                           &notif_frame.id, &notif_frame.param)) {
            saw_notify = 1;
        }

        // ---- (2) gate: only run if a notification fired OR the previous
        //               enum is still in progress (sub_82849790 returns
        //               "wait-not-yet-signaled" status).
        if (!saw_notify) {
            uint32_t hWaitEvt = *(uint32_t*)XCONTENT_ENUM_PTR;
            // sub_82849790 → KeWaitForSingleObject(hWaitEvt, 0 ms) → bool
            if (!sub_82849790(hWaitEvt))
                goto sleep_and_loop;
        }

        // ---- (3) acquire content-enumeration critsec (recursive) ----------
        sub_8285FF50(&ov, TYPE_FILTER);   // EnterCriticalSection on TYPE_FILTER->cs
                                           // and stamp ov so sub_8285FFA0 releases it

        // ---- (4) abort if state == -1 (shutdown) --------------------------
        if (STATE->state == -1)
            goto skip_release;             // (no callback needed)

        // ---- (5) clear "ready" flag if a fresh notify caused this pass ----
        if (saw_notify)
            STATE->ready = 0;

        // ---- (6) issue async XAM enumeration ------------------------------
        // sub_82A11F50 = thin wrapper for XamEnumerate(0, hEnum, buf, cb, &ret, NULL)
        uint32_t pcb_buffer = 0;
        uint32_t hEnumLocal = 0;
        // sub_82A12710 → XamContentCreateEnumerator(0, dev=2, type=0,
        //                                            flags=0, count=96,
        //                                            &cbBuf, &hEnum)
        uint32_t st = sub_82A12710(/*dwUserIndex=*/0, /*dev=*/2,
                                   /*flags=*/0, /*count=*/96,
                                   &pcb_buffer, &hEnumLocal);

        if (st == 0) {
            // sub_82A11F50(0, hEnum, NAME_MATCH_BUF, pcb_buffer, NULL, NULL)
            //   → XamEnumerate(0, hEnum, NAME_MATCH_BUF, pcb_buffer, NULL, NULL)
            sub_82A11F50(hEnumLocal,
                         /*flags=*/0,
                         NAME_MATCH_BUF,
                         pcb_buffer,
                         /*pcRet=*/nullptr,
                         /*pOv=*/nullptr);
            CloseHandle(hEnumLocal);

            if (saw_notify) {
                // ---- (7) sub_825FB998: dispatch enumerated entries to per-
                //                       DLC consumer list (see below)
                bool one_unconsumed = sub_825FB998();
                STATE->ready = (uint8_t)one_unconsumed;
                // call POST_PROC_OBJ->vtbl[3](POST_PROC_OBJ) for processing
                ((void(*)(void*))POST_PROC_OBJ->vtbl[3])(POST_PROC_OBJ);
            }
        } else if (st == 1317 /* ERROR_NO_MORE_FILES */) {
            // skip: nothing to consume
        } else if (saw_notify) {
            // notify-driven re-enumerate failed — still kick post-proc with
            // a default arg so consumers can react to the "device gone" case.
            ((void(*)(void*, int))POST_PROC_OBJ->vtbl[3])(POST_PROC_OBJ, 0);
        }

        // ---- (8) drop critsec ---------------------------------------------
        if (!saw_notify)
            sub_82849860(*(uint32_t*)CB_RECEIVER_PTR);   // KeSetEvent
                                                          // (signal that enum
                                                          //  cycle is done)
    skip_release:
        sub_8285FFA0(&ov);                  // LeaveCriticalSection if held

        saw_notify = 0;

    sleep_and_loop:
        sub_82849918(16);                   // KeDelayExecutionThread(1, 16 ms)
    }
}
```

### Notes on the reconstruction

* `sub_82A12710` lives **inside** `sub_82A12690+0x80` — it is the *body* of `XamContentCreateEnumerator` after the parameter-validation prologue (entry `0x82A12690` is the validation gate, falls through to `0x82A126E0: XamContentCreateEx`). Per project memory, `XamContentCreateEnumerator` is implemented as `j_XamContentCreateEnumerator` import.
* `sub_82A11F50` is a 6-instruction shim that shifts argument registers and tail-calls `XamEnumerate`.
* `sub_82849790` = bool-cast of `sub_82A13040` ⇒ `KeWaitForSingleObject(h, 0)` (returns true if signalled).
* `sub_82849860` = bool-inverted `sub_82A12F50` ⇒ `NtReleaseSemaphore(h, 1, NULL)` then `sub_82A18C38` on failure.
* `sub_82849918` = `KeDelayExecutionThread(KernelMode, &interval)` via `sub_82A12B60 → sub_82A1A200`.
* `sub_8285FF50` = recursive-CS enter using `RtlEnterCriticalSection` (only enters if `ov->depth==0`); `sub_8285FFA0` decrements + leaves once depth hits 0.

## Reconstructed C++ — sub_825FB998

```cpp
// Walk the doubly-linked list of registered DLC content records (head sentinel
// at 0x83096058+12, list count at 0x83096054). For every record whose name-key
// is NOT yet associated with the matching XCONTENT_DATA in the just-enumerated
// buffer (NAME_MATCH_BUF at 0x8308B838+264, stride 308), open the content via
// XamContentCreateEx so it appears as a mounted package.
//
// Returns: 0 if every registered record matched (full coverage); 1 if any
// remained unmatched (used by sub_825FBB68 to set STATE->ready).
bool sub_825FB998()
{
    constexpr Listhead*    HEAD          = (Listhead*)(0x83096058 + 12);
    constexpr uint32_t*    pCOUNT        = (uint32_t*)0x83096054;
    constexpr uint8_t*     ENUM_BUF      = (uint8_t*)(0x8308B838 + 264);
    constexpr int          ENUM_STRIDE   = 308;
    constexpr int          REC_NAME_OFF  = 272;   // record->name (offset 272)

    bool ok = true;                               // r22

    for (Listhead* node = HEAD->next; node != HEAD; node = node->next) {
        ContentRec* rec   = (ContentRec*)node;     // (lwz r28, 0(r25))
        uint32_t    count = *pCOUNT;
        uint32_t    matched_idx = 0;

        if (count != 0) {
            uint8_t* enum_entry = ENUM_BUF;
            for (uint32_t i = 0; i < count; ++i, enum_entry += ENUM_STRIDE) {
                // strcmp(rec->name @+272, enum_entry @+0)
                const uint8_t* a = (uint8_t*)rec + REC_NAME_OFF;
                const uint8_t* b = enum_entry;
                int diff;
                do {
                    diff = (int)*a - (int)*b;
                    if (*a == 0 || diff != 0) break;
                    ++a; ++b;
                } while (true);

                if (diff == 0) {
                    matched_idx = i;
                    goto matched_or_done;
                }
            }
            // No match found — fall through with matched_idx == count
        }

    matched_or_done:
        if (matched_idx == count) {
            // unmatched — keep ok=false
            ok = false;
        } else {
            // matched — make sure the entry's instance handle is bound
            uint32_t found_handle = *(uint32_t*)(ENUM_BUF + matched_idx*ENUM_STRIDE + 0);
            if (rec->instance_handle != found_handle) {
                // Close any prior mount, then open the new one.
                XamContentClose(&rec->root_name, /*pOv=*/nullptr);  // sub_82A126F8
                rec->instance_handle = found_handle;
                // sub_82A127A0 → sub_82A12690 → XamContentCreateEx(rec->mount_point,
                //                                                   &rec->data,
                //                                                   flags=3, ...)
                XamContentCreateEx_Wrapped(rec, /*flags=*/3);
            }
        }
    }
    return ok;
}
```

## Thread-to-crash call path: does this thread hit `sub_8227F2E8 / sub_828C2300`?

**No path exists** through `sub_825FBB68` to `sub_8227F2E8`. Verified callee transitive closure:

```text
sub_825FBB68
├─ XNotifyGetNext           (kernel import, hooked in rexglue xam_notify.cpp)
├─ j_XamContentCreateEnumerator  (kernel import, rexglue xam_content.cpp)
├─ sub_82849790  →  sub_82A13040 → sub_82A1A450 → NtWaitForSingleObjectEx
├─ sub_82849860  →  sub_82A12F50 → NtReleaseSemaphore (+ sub_82A18C38 err)
├─ sub_82849918  →  sub_82A12B60 → sub_82A1A200 → KeDelayExecutionThread
├─ sub_8285FF50  →  RtlEnterCriticalSection
├─ sub_8285FFA0  →  RtlLeaveCriticalSection
├─ sub_82A11F50  →  XamEnumerate
├─ rexcrt_CloseHandle  (already hooked: rexcrt_CloseHandle, addr 0x82A11F68)
├─ sub_82A12710  → embedded in sub_82A12690 → XamContentCreateEx
└─ sub_825FB998
   ├─ j_XamContentClose   (kernel import)
   ├─ sub_82A126F8        → XamContentClose body
   └─ sub_82A127A0        → sub_82A12690 → XamContentCreateEx
```

`sub_8227F2E8` is a **HUD/audio-DC drawing helper** (9 floats, calls 4× `sub_828C2290` + 1× `sub_828C2300`) used by `sub_8227F5B8` (text/icon write) and `sub_8227F608` (3D billboard write). Its forward callers are **`sub_8214DBD0` (LOADLANG), `sub_821B5C90`, `sub_8218D7A8`, `sub_821F1670`, `sub_82150E08` (CDrawRadarMapSectionDC family)** — all are **HUD render-thread / DC-submit** functions. None appear in the Enumerate Content thread's transitive closure.

`sub_828C2300` itself has 46 callers — including `CDrawRadarMapSectionDC::vfunc[0]`, `CDrawRadioHudTextDC::vfunc[0]`, `CDrawTriShapeDC::vfunc[0]`. **It's a DrawCommand/DC pool finalizer, not an audio function.** It clears the active flag at `0x831C2D38` and (if non-null) frees the DC-batch object pointer at `0x831C2D28` via `sub_828BF270 → sub_82A3DF50` (= raw `free()` on a heap object loaded from `0x831822A4`).

**Conclusion**: tid=12 ("Enumerate Content") is **not** the thread that produced the faulting `stw r11, 0(r31)`. It is the thread the OS most-recently scheduled / left in a wait, which the macOS crash reporter labelled as the "current" thread. The actual faulting thread is the **render thread** executing `sub_8227F2E8` (a per-frame HUD overlay) — see `02-middle-frame-8227f.md` and `04-pool-helpers-828c1.md` for that side.

## Mutex acquisition map (this thread)

| step | call | object | id |
|-|-|-|-|
| (3) enter | `sub_8285FF50(&ov, TYPE_FILTER)` | `RTL_CRITICAL_SECTION* TYPE_FILTER->cs` (read from `*0x83096090`) | content-enum CS — **not** in F8000CD4 audio-pool family |
| (8) leave | `sub_8285FFA0(&ov)` | same CS | mirror release |

The thread holds **only the content-enumeration CS** while inside steps (4)–(7). It does **not** acquire any audio-engine or DC-pool mutex. Therefore it **cannot deadlock or race against** the `0x831C2D38` pool flag updated in `sub_828C2300`.

The `F8000CD4` event-handle family (audio engine update fence) is acquired by `sub_8227EE90`'s `sub_82146700`/`sub_82155300` callees — those run on the audio mixer thread, not here.

## Existing-hook map

| callee | LibertyRecomp/* hook | rexglue/* hook | gta4-recomp |
|-|-|-|-|
| `XNotifyGetNext` | none | `glue/.../src/system/xnotifylistener.cpp` | import shim |
| `j_XamContentCreateEnumerator` | none | `glue/.../src/kernel/xam/xam_content.cpp` | import shim |
| `XamContentCreateEx` (via `sub_82A12690`) | none | `glue/.../src/kernel/xam/xam_content.cpp` | import shim |
| `XamEnumerate` (via `sub_82A11F50`) | none | `glue/.../src/kernel/xam/xam_enum.cpp` | import shim |
| `j_XamContentClose` | none | `glue/.../src/kernel/xam/xam_content.cpp` | import shim |
| `rexcrt_CloseHandle` (= `sub_82A11F68`) | rexcrt (already integrated, project memory) | rexglue rexcrt | already hooked |
| `RtlEnterCriticalSection` | none (rexglue does it) | rexglue kernel | import shim |
| `RtlLeaveCriticalSection` | none | rexglue kernel | import shim |
| `KeWaitForSingleObject` (via `sub_82A1A450 → NtWaitForSingleObjectEx`) | none | rexglue kernel | import shim |
| `NtReleaseSemaphore` | none | rexglue kernel | import shim |
| `KeDelayExecutionThread` | none | rexglue kernel | import shim |
| `sub_825FBB68` (entry) | **not hooked** | n/a | recomp scaffold |
| `sub_825FB998` | **not hooked** | n/a | recomp scaffold |
| `sub_82849790/860/918` | **not hooked** (thin shims, fine) | n/a | recomp scaffold |
| `sub_8285FF50/FFA0` | **not hooked** (thin CS shims) | n/a | recomp scaffold |

`grep` of `LibertyRecomp/kernel/*` and `LibertyRecomp/patches/*` confirms **no GUEST_FUNCTION_HOOK / PPC_FUNC_HOOK** intercepts any of the callees. Only rexglue handles the kernel imports; recomp executes the rest natively. `LibertyRecomp/kernel/xam.h` declares the prototypes for `XNotifyGetNext`, `XamContentCreateEnumerator`, `XamEnumerate`, `XamContentCreateEx`, `XamContentClose` — the implementations come from rexglue.

## Conclusion

| question | verdict |
|-|-|
| Thread-race between Enumerate Content and audio? | **No.** Disjoint critsecs; no shared global. |
| Use-after-free caused by Enumerate Content thread? | **No.** UAF poison `0xFFE1E1E1` belongs to the DC-batch buffer at `*0x831C2D28`, owned by the render thread. |
| Logic bug in `sub_825FBB68` / `sub_825FB998`? | **No** — both functions are well-formed and only touch their own globals (`0x82AACFE4`, `0x83096054..0x830960B0`, `0x8308B838+264`). |
| Is this thread relevant to the crash? | **Bystander.** It is the most-recently-suspended thread (sleeping in `sub_82849918` ⇒ `KeDelayExecutionThread` 16 ms) and was simply tagged as "current" by the crash reporter. |

### Why was it labelled tid=12 in the dump?

Two plausible explanations:

1. **Most-recent context-switch artifact.** The macOS Mach exception handler captures the thread that had the OS lock when the trap fired; `KeDelayExecutionThread` returns through a host `nanosleep` whose wake interrupts `mach_msg`. The render thread's fault propagated through the recomp dispatcher and the host signal trampoline grabbed whichever Liberty thread was top-of-stack at that instant.
2. **Recomp register clobber via shared `PPCContext`.** If the host runtime binds `PPCContext*` per OS thread but the render-thread fault unwinds into a worker's TLS slot during stack-walk, the unwound `r31` (saved at `r1+(-16)`) is read from the *Enumerate Content* thread's stack frame — yielding the constant `0x20`, which equals `sizeof(XAM_OVERLAPPED_HEADER)` (32 bytes), the offset that `sub_8285FF50` writes back into its own stack at `r1+(-16)` for the saved-r31 slot.

Either way the **real fix** belongs in the render-thread investigation (docs 01/02/04), not here. **No change to `sub_825FBB68` or `sub_825FB998` is required.**

### Hand-off to other agents

* Agent investigating `sub_8227F2E8` — confirm whether a non-volatile register clobber by `sub_828BF270 → sub_82A3DF50` desyncs the host stack frame; the wrapper `sub_82A3DF50` (16 B leaf) is a strong suspect for a guest-malloc `free()` whose host shim isn't preserving `r31`.
* Agent owning `sub_828C2300` — verify the `0x831C2D28` pool head pointer life-cycle: it's freed once but written-to by 46 producers, half of which are HUD vtable slot 0. Add a guarded null-check before `bl 0x828bf270` so a poisoned `0xFFE1E1E1` head doesn't pass the `cmplwi cr6,r11,0` test.
