# RtlCriticalSection Complete Contention Map

## Summary

| Metric | Count |
|-|-|
| Direct RtlEnterCriticalSection calls | 82 |
| Scoped lock (sub\_8285FF50) calls | 419 |
| Total CS acquisitions | 501 |
| Unique CS-acquiring functions | 403 |
| Unique static CS addresses | 7 |
| Object-member CS patterns | 8 |
| Stack-local CS (scoped only) | 414 |
| Cross-CS functions (deadlock risk) | 0 static-static |
| KeEnterCriticalRegion + RtlEnterCS combos | 4 |

## Architecture

### RtlEnterCriticalSection Implementation

The implementation in `xboxkrnl_rtl.cpp` operates directly on guest `X_RTL_CRITICAL_SECTION` structs using atomics. It does **NOT** funnel through `global_critical_region_`. The contention path is:

1. Check recursive ownership (fast path)
2. Spin loop (spin\_count iterations of atomic CAS on `lock_count`)
3. If spin fails: `atomic_inc(&lock_count)` then `xeKeWaitForSingleObject()` (blocks on the CS header as a KEVENT)

`global_critical_region_` is used separately by KernelState for host-side operations (thread registration, module loading, dispatch queue, notify listeners). It is **not** involved in guest CS operations.

### sub\_8285FF50 (Scoped Lock Helper)

Located at `gta4_recomp.56.cpp:14206`. This is RAGE's scoped critical section lock:
- Takes r3 = pointer to 8-byte scoped lock struct, r4 = pointer to the actual CS
- Stores r4 at [r3+4], sets [r3+0] = 1 (lock acquired flag)
- Checks if CS is initialized (non-zero at [r4+0]); if so, calls `RtlEnterCriticalSection(r4)`
- Matching destructor `sub_8285FFA0` decrements the flag and calls `RtlLeaveCriticalSection` when it reaches 0

The scoped lock struct is always allocated on the stack (e.g., `r1+80`, `r1+88`, etc.) with the CS pointer passed via r4. The **actual CS address is in r4**, not r3.

## Static/Global CS Addresses

### CS 0x82B2835C — XNotify/Notification System

| Field | Value |
|-|-|
| Init site | sub\_82A5AE28 (gta4\_recomp.72.cpp:1869) |
| Acquisitions | 26 direct |
| Unique functions | 26 |
| Subsystem | XNotifyListener notification dispatch |

All 26 functions are notification handlers in the 0x8218Exxx-0x8218Fxxx range:

- sub\_8218ED50, sub\_8218EDB8, sub\_8218EE20, sub\_8218EE90
- sub\_8218EEE8, sub\_8218EF40, sub\_8218EF98, sub\_8218EFF8
- sub\_8218F070 (calls sub\_82192F20 while holding CS)
- sub\_8218F0B8, sub\_8218F110, sub\_8218F178, sub\_8218F1D0
- sub\_8218F228, sub\_8218F280, sub\_8218F2D8, sub\_8218F350
- sub\_8218F3D0, sub\_8218F4E8, sub\_8218F568, sub\_8218F5E8
- sub\_8218F668, sub\_8218F700 (calls sub\_821929C8 while holding CS)
- sub\_8218F760 (calls sub\_82191F30 while holding CS)
- sub\_8218F7C0, sub\_8218F9A8

**Pattern**: Each function acquires the CS, calls a virtual function via indirect call (bctrl on vtable), then releases. This is the XNotify system dispatching notifications to listeners under lock.

### CS 0x82B2833C — XNotify Event Wait

| Field | Value |
|-|-|
| Init site | sub\_82A5AE08 (gta4\_recomp.72.cpp:1830) |
| Acquisitions | 1 direct |
| Unique functions | 1 |
| Subsystem | Notify event signaling |

Used by sub\_821910D0 which:
- Acquires CS 0x82B2833C
- Calls `KeSetEvent`, `KeWaitForMultipleObjects`
- This is the notification dispatch thread coordinator

### CS 0x82B27ED8 — Title Terminate Registration

| Field | Value |
|-|-|
| Acquisitions | 2 direct |
| Unique functions | 2 |
| Subsystem | ExRegisterTitleTerminateNotification |

- sub\_821904B8: Registers terminate notification callback
- sub\_82191E38: Also registers terminate notification callback

### CS 0x82B1F6A4 — STFS/Content File Operations

| Field | Value |
|-|-|
| Acquisitions | 3 direct |
| Unique functions | 3 |
| Subsystem | XContent STFS file I/O |
| KeEnterCriticalRegion | Yes (2 of 3 functions) |

- sub\_82A14060: KeSetEvent + KeWaitForSingleObject + KeResetEvent (preceded by KeEnterCriticalRegion)
- sub\_82A14120: Same pattern (preceded by KeEnterCriticalRegion)
- sub\_82A14448: XamTaskSchedule + KeWaitForSingleObject (preceded by KeEnterCriticalRegion)

**Important**: These combine KeEnterCriticalRegion (disables APCs) with RtlEnterCriticalSection. The native rewrite must preserve this ordering.

### CS 0x82B1F728 — Content Manager Lock

| Field | Value |
|-|-|
| Acquisitions | 2 direct |
| Unique functions | 2 |
| Subsystem | STFS content manager |

- sub\_82A18620, sub\_82A18680: Short critical sections in content enumeration

### CS 0x82B1FD8C — File System Device Lock

| Field | Value |
|-|-|
| Init site | sub\_82A6F190 (gta4\_recomp.74.cpp:16541) |
| Acquisitions | 2 direct |
| Unique functions | 2 |
| Subsystem | Content enumeration device |

- sub\_82A245C8: Calls sub\_82A244A0 + sub\_82A243F8 while holding CS
- sub\_82A246A0: Calls sub\_82A243F8 while holding CS

### CS \*0x831D4B50 — Indirect CS (Pointer Dereference)

| Field | Value |
|-|-|
| Acquisitions | 1 direct |
| Unique functions | 1 |
| Subsystem | GPU/render state |

- sub\_82A50ED8: Dereferences pointer at 0x831D4B50 to get CS address, calls sub\_82A50DA0 while holding

### CS \*0x820007F4 — EDRAM/Video Subsystem

| Field | Value |
|-|-|
| Acquisitions | 1 direct |
| Unique functions | 1 |
| KeEnterCriticalRegion | Yes |

- sub\_82A52E38: KeEnterCriticalRegion, then acquires CS at \*0x820007F4, then calls VdRetrainEDRAMWorker + VdRetrainEDRAM + sub\_82A49C38

## Object-Member CS Patterns (Instance Locks)

These CS addresses are computed at runtime from object pointers, not static globals.

### this+1272 — XNotifyListener Objects

| Field | Value |
|-|-|
| Acquisitions | 15 direct + 1 (r30+1272) |
| Unique functions | 16 |
| Init site | sub\_82A27170 (gta4\_recomp.70.cpp:12150) |
| Subsystem | XNotifyListener per-instance lock |

All functions in sub\_82A25xxx-sub\_82A26xxx range:
- sub\_82A25DD8 through sub\_82A26DF8 (15 functions)
- sub\_82A26A18 (uses r30+1272, same object but different register)
- sub\_82A267C0 acquires this+1272 then calls notification handlers sub\_8218EF98, sub\_8218F070

This is the **highest-contention instance lock** — 16 different operations on XNotifyListener objects all serialize on the per-instance CS at offset 1272.

### this+14928 and this+14956 — Graphics Pipeline Object

| Field | Value |
|-|-|
| Acquisitions | 4 direct |
| Pattern | (\*(\*0x8200078C))+14956 and similar |

- sub\_82A47110, sub\_82A47190, sub\_82A471F0: CS at this+14956
- sub\_82A4F0E0: CS at this+14928

These are two per-instance CS on the same graphics pipeline object (initialized together at gta4\_recomp.71.cpp:54606-54611).

### this+32 — Kernel Object Lock

| Field | Value |
|-|-|
| Acquisitions | 2 direct |
| Functions | sub\_82A04A08, sub\_82A04A78 |
| Subsystem | Object manager |

### Various NT Filesystem Instance Locks

| Pattern | Functions | Subsystem |
|-|-|-|
| \*(r29+24) | sub\_82A1AD20 | File handle close (NtClose) |
| \*(\*(\*(r30+80)+24)+12) | sub\_82A1CE00 | File object lock |
| \*(\*(r18+80)+24) | sub\_82A1FAF8 | File share access |
| ((\*(r30+8))+1)+12 | sub\_82A0F2D8 | Object directory lock |

### r28+4 — DPC/Spinlock Combined Pattern

| Field | Value |
|-|-|
| Acquisitions | 2 direct |
| Functions | sub\_82191B70, sub\_82191CC0 |

Both functions acquire CS at r28+4 **and also** call KeRaiseIrqlToDpcLevel + KeAcquireSpinLockAtRaisedIrql. This is the most complex locking pattern — CS + IRQL raise + spinlock.

## Stack-Local CS (Scoped Locks on Local CRITICAL\_SECTION)

| Metric | Value |
|-|-|
| Total call sites | 414 |
| Unique functions | 317 |

These are RAGE engine functions that allocate a CRITICAL\_SECTION on the stack, initialize it, lock it with sub\_8285FF50, do work, then the scoped destructor unlocks. Because each is a **unique local CS per call**, there is no cross-thread contention on these — they protect against recursive re-entry on the same thread or short-lived concurrent access to local data.

### Subsystem Breakdown

| Subsystem | Functions | Address Range |
|-|-|-|
| XboxKrnl IO | 115 | 0x829Dxxxx-0x829Fxxxx |
| RAGE Collection | 65 | 0x82820000-0x82860000 |
| RAGE UI/Menu | 19 | 0x82940000-0x82960000 |
| RAGE Timer/Perf | 18 | 0x82930000-0x82950000 |
| RAGE Streaming | 17 | 0x82390000-0x823B0000 |
| D3D/GPU | 13 | 0x82790000-0x827B0000 |
| RAGE Particle | 13 | 0x82900000-0x82920000 |
| RAGE Render | 11 | 0x82690000-0x826C0000 |
| RAGE Animation | 7 | 0x824A0000-0x824C0000 |
| RAGE Entity/Pool | 7 | 0x824E0000-0x82500000 |
| RAGE Network | 7 | 0x82550000-0x82560000 |
| RAGE Resource/RPF | 6 | 0x82640000-0x82660000 |
| RAGE Vehicle | 5 | 0x82920000-0x82930000 |
| RAGE Audio | 3 | 0x821B0000-0x82200000 |
| RAGE World/Map | 3 | 0x82210000-0x822B0000 |
| RAGE Physics | 2 | 0x82440000-0x82460000 |
| RAGE Memory/TLS | 2 | 0x827D0000-0x827E0000 |
| RAGE Camera | 2 | 0x828C0000-0x828D0000 |
| RAGE Text/Font | 2 | 0x82960000-0x829A0000 |
| RAGE Script | 1 | 0x825F0000-0x82600000 |

## UNRESOLVED CS Addresses

15 call sites (12 direct + 3 scoped) could not resolve the CS address statically because the r3 register value depends on earlier dynamic computation beyond the 40-line lookback window. These are primarily:

| Function | File | Notes |
|-|-|-|
| sub\_8285FE78 | gta4\_recomp.56.cpp:14052 | CS helper (checks if CS initialized before entering) |
| sub\_8285FF50 | gta4\_recomp.56.cpp:14239 | The scoped lock itself (r3=scoped struct, CS comes from r4) |
| sub\_82A08D10 | gta4\_recomp.69.cpp:13668 | Object manager (calls KeBugCheck on failure) |
| sub\_82A1B8B8 | gta4\_recomp.69.cpp:53669 | NT file create |
| sub\_82A1D048 | gta4\_recomp.69.cpp:57209 | NT file info query |
| sub\_82A1D130 | gta4\_recomp.69.cpp:57344 | NT file directory query |
| sub\_82A1E330 | gta4\_recomp.69.cpp:59975 | NT file cleanup |
| sub\_82A1EA08 | gta4\_recomp.69.cpp:61010 | NT file close |
| sub\_82A1F568 | gta4\_recomp.69.cpp:62680 | NT file lock/unlock |
| sub\_82A1F810 | gta4\_recomp.69.cpp:63065 | NT file share access removal |
| sub\_82A20110 | gta4\_recomp.69.cpp:64485 | NT file query standard info |
| sub\_82A207D8 | gta4\_recomp.69.cpp:65434 | NT file set info |
| sub\_823A0238 | gta4\_recomp.14.cpp:91157 | Streaming subsystem |
| sub\_8264C900 | gta4\_recomp.36.cpp:126177 | RPF resource manager |
| sub\_8264CA58 | gta4\_recomp.36.cpp:126392 | RPF resource manager |

The NT filesystem functions (sub\_82A1xxxx) all operate on per-file-object instance locks obtained by chasing pointer chains from the file object.

## KeEnterCriticalRegion + RtlEnterCS Combinations

These 4 call sites disable APCs before acquiring a CS, which is the Xbox 360 kernel pattern for protecting against thread suspension while holding a lock:

| Function | CS Address | File |
|-|-|-|
| sub\_82A14060 | 0x82B1F6A4 | gta4\_recomp.69.cpp:39611 |
| sub\_82A14448 | 0x82B1F6A4 | gta4\_recomp.69.cpp:40644 |
| sub\_82A1AD20 | \*(r29+24) | gta4\_recomp.69.cpp:51933 |
| sub\_82A52E38 | \*0x820007F4 | gta4\_recomp.71.cpp:60339 |

## Cross-CS Dependencies (Deadlock Analysis)

**No function acquires two different static CS addresses.** This means there are no static A-then-B vs B-then-A deadlock scenarios in the direct lock code.

However, potential transitive deadlock paths exist through function calls made while holding a CS:

1. **sub\_82A267C0** holds `this+1272` (XNotifyListener) and calls `sub_8218EF98` + `sub_8218F070`, which acquire `0x82B2835C` (global notify CS). This is a legitimate lock ordering: instance lock -> global lock.

2. **sub\_82191B70** holds `r28+4` (CS) and also acquires a spinlock at raised IRQL. This mixed locking discipline must be preserved exactly in the rewrite.

3. **sub\_82A14060/sub\_82A14120** combine KeEnterCriticalRegion + CS 0x82B1F6A4 + KeWaitForSingleObject. Blocking while holding a CS at disabled-APC IRQL is safe on Xbox 360 but may interact poorly with host thread scheduling.

## Contention Matrix

### Thread x CS Analysis

The primary thread contention points are:

| CS | Main Thread | Render Thread | Audio Thread | I/O Worker | Notify Thread |
|-|-|-|-|-|-|
| 0x82B2835C (Notify) | Read (26 handlers) | - | - | - | Dispatch |
| 0x82B2833C (Notify Event) | - | - | - | - | Wait/Signal |
| 0x82B27ED8 (Terminate) | Init only | - | - | - | - |
| 0x82B1F6A4 (STFS) | - | - | - | File I/O | - |
| 0x82B1F728 (Content) | Enumerate | - | - | - | - |
| 0x82B1FD8C (Device) | Enumerate | - | - | - | - |
| this+1272 (Listener) | Add/Remove | - | - | - | Get/Iterate |
| Stack-local (317 fn) | Various | Various | Various | Various | - |

### Hottest Contention Points

1. **0x82B2835C** (26 functions): All notification handler functions compete. On Xbox 360 these ran on a single-core-equivalent with cooperative scheduling; on native they may contend across real threads.

2. **this+1272** (16 functions): XNotifyListener per-instance lock. Heavy if multiple threads process notifications for the same listener.

3. **Stack-local CS** (414 sites): No cross-thread contention by design, but the overhead of `RtlInitializeCriticalSection` + `RtlEnterCriticalSection` + `RtlLeaveCriticalSection` per call adds up for 414 call sites.

## Rewrite Implications

1. **Static CS addresses can become `std::mutex`** — 7 total, trivial replacement
2. **Object-member CS at this+1272 becomes per-instance `std::mutex`** — add mutex field to listener class
3. **Stack-local scoped locks (414 sites) can become no-ops or `std::lock_guard<std::mutex>`** on a per-object basis
4. **KeEnterCriticalRegion pattern (4 sites)**: Must disable thread cancellation or use `pthread_setcancelstate` equivalent
5. **The CS + spinlock + IRQL pattern in sub\_82191B70/sub\_82191CC0**: Needs careful translation to host primitives
6. **No global\_critical\_region bottleneck for guest CS**: The `global_critical_region_` is only used by KernelState host operations, not by the guest's RtlEnter/LeaveCriticalSection
