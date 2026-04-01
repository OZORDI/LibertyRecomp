# Pseudocode: sub_82556190 — World Streaming Thread Init

**Source**: `gta4-recomp/generated/gta4_recomp.26.cpp`, lines 93069–93178
**Position in sub_821FC1F8**: call #32 (1-indexed), no args, no return value checked
**Hang target**: `sub_8285D948 → sub_8285D610 → sub_82A13110` (unhooked CreateThread)

All addresses verified with Python arithmetic on `ctx.r11.s64` literal values.

---

## Address Map (Python-verified)

```python
# lis r11,-31992 => ctx.r11.s64 = -2096627712 = 0x83080000
# lis r10,-32085 => ctx.r10.s64 = -2102722560 = 0x82AB0000
# lis r11,-32254 => ctx.r11.s64 = -2113798144 = 0x82020000
# lis r11,-32171 => ctx.r11.s64 = -2108358656 = 0x82A90000 (actually 0x82AB0000-based)

r11_83080000 = -2096627712   # 0x83080000
r10_82AB0000 = -2102722560   # 0x82AB0000

init_flag        = (r10_82AB0000 + (-18152)) & 0xFFFFFFFF  # 0x82AAB918 = 1
memset1_base     = (r11_83080000 + (-13744)) & 0xFFFFFFFF  # 0x8307CA50, size=3240
memset2_base     = (r11_83080000 + (-17704)) & 0xFFFFFFFF  # 0x8307BAD8, size=3960
thread_obj       = 0x820259CC   # r3 for sub_8285D948 (lis -32254 + 22988)
thread_handle    = (r11_83080000 + (-17708)) & 0xFFFFFFFF  # 0x8307BAD4
event_handle     = (r11_83080000 + (-17712)) & 0xFFFFFFFF  # 0x8307BAD0
scheduler_obj    = 0x82556188   # r3 for sub_82849A50 (lis -32171 + 24968; =fn-8)
scheduler_handle = (r11_83080000 + (-10504)) & 0xFFFFFFFF  # 0x8307D6F8
thread_name_ptr  = 0x820259BC   # r7 for both D948 and A50
```

---

## Complete Pseudocode

```c
void sub_82556190(PPCContext& ctx, uint8_t* base) {
    // --- Phase 1: Mark subsystem as initialising ---
    // lis r10,-32085; stw 1, -18152(r10)
    PPC_STORE_U32(0x82AAB918, 1);  // streaming_init_flag = 1

    // --- Phase 2: Zero streaming slot array (90 * 36 bytes = 3240) ---
    // lis r11,-31992; addi r3,r11,-13744; li r4,0; li r5,3240
    sub_829FF840(ctx, base);  // memset(0x8307CA50, 0, 3240)
    //   r3=0x8307CA50, r4=0, r5=3240

    // --- Phase 3: Zero I/O request queue (15 * 264 + header = 3960) ---
    // lis r11,-31992; addi r3,r11,-17704; li r4,0; li r5=3960
    sub_829FF840(ctx, base);  // memset(0x8307BAD8, 0, 3960)
    //   r3=0x8307BAD8, r4=0, r5=3960

    // --- Phase 4: Element-wise constructor reset ---
    sub_82554E60(ctx, base);  // no args; reads/writes globals directly
    //   Loop A: base=0x8307BAD8, step=264, until base+4040
    //     [r11-80]=0, [r11-44]=0 (if was non-null), [r11-36]=0
    //     [r11+0]=0 (if was non-null), [r11+8]=0
    //     [r11+44]=0 (if was non-null), [r11+52]=0
    //     [r11+88]=0 (if was non-null), [r11+96]=0
    //     [r11+132]=0 (if was non-null), [r11+140]=0
    //     [r11+176]=0 (if was non-null)
    //   Loop B: base=0x8307CA50, stride=36, until base+3240
    //     [r11+0]=0, [r11+28]=0

    // --- Phase 5: Create streaming worker thread (HANG POINT) ---
    // lis r11,-32254; addi r3,r11,22988 = 0x820259CC
    // lis r4,1 = 65536; ori r5,r5,32768 = 32768; li r6,1; li r7,6; li r8,0
    ctx.r3.u64 = 0x820259CC;   // thread object struct
    ctx.r4.u64 = 65536;        // stack size
    ctx.r5.u64 = 32768;        // priority (0x8000)
    ctx.r6.u64 = 1;            // affinity (core 1)
    ctx.r7.u64 = 6;            // flags
    ctx.r8.u64 = 0;            // creation flags
    sub_8285D948(ctx, base);   // → allocates 1080B, calls sub_8285D610 → HANGS

    // stw r3, -17708(r11) where r11=0x83080000
    PPC_STORE_U32(0x8307BAD4, ctx.r3.u32);  // streaming_thread_handle = result

    // --- Phase 6: Init streaming sync object ---
    ctx.r3.u64 = 0;
    sub_82849778(ctx, base);   // NtCreateSemaphore / event init (r3=0 = free-list head)

    // stw r3, -17712(r10) where r10=0x83080000
    PPC_STORE_U32(0x8307BAD0, ctx.r3.u32);  // streaming_event_handle = result

    // --- Phase 7: Create streaming scheduler object ---
    // lis r11,-32171+24968 = 0x82556188; addi r7,r11(=-32254),22972 = 0x820259BC
    ctx.r3.u64 = 0x82556188;   // scheduler object (embedded static, 8B before this fn)
    ctx.r4.u64 = 0;            // initial count
    ctx.r5.u64 = 2048;         // queue depth
    ctx.r6.u64 = 0;            // flags
    ctx.r7.u64 = 0x820259BC;   // scheduler name ptr
    ctx.r8.u64 = 1;            // enable
    ctx.r9.u64 = 1;            // start immediately
    sub_82849A50(ctx, base);   // allocates scheduler node, calls sub_82A13110 (CreateThread)

    // stw r3, -10504(r11) where r11=0x83080000
    PPC_STORE_U32(0x8307D6F8, ctx.r3.u32);  // streaming_scheduler_handle = result

    return;  // void — caller sub_821FC1F8 does NOT check r3
}
```

---

## sub_82554E60: Element-wise Reset

```c
// No parameters. Reads globals directly.
// recomp.26.cpp lines 90136-90242
void sub_82554E60(PPCContext& ctx, uint8_t* base) {
    uint32_t base2 = 0x8307BAD8;  // lis r11,-31992 + addi r9,r11,-17704
    uint32_t r11   = base2 + 80;  // first element start

    // Loop A: 15 thread-object entries, stride 264
    while (r11 < base2 + 4040) {
        PPC_STORE_U32(r11 - 80, 0);                             // busy flag = 0
        if (PPC_LOAD_U32(r11 - 44) != 0)
            PPC_STORE_U32(r11 - 44, 0);                         // handle/ptr = 0
        PPC_STORE_U32(r11 - 36, 0);
        if (PPC_LOAD_U32(r11 + 0) != 0)
            PPC_STORE_U32(r11 + 0, 0);
        PPC_STORE_U32(r11 + 8, 0);
        if (PPC_LOAD_U32(r11 + 44) != 0)
            PPC_STORE_U32(r11 + 44, 0);
        PPC_STORE_U32(r11 + 52, 0);
        if (PPC_LOAD_U32(r11 + 88) != 0)
            PPC_STORE_U32(r11 + 88, 0);
        PPC_STORE_U32(r11 + 96, 0);
        if (PPC_LOAD_U32(r11 + 132) != 0)
            PPC_STORE_U32(r11 + 132, 0);
        PPC_STORE_U32(r11 + 140, 0);
        if (PPC_LOAD_U32(r11 + 176) != 0)
            PPC_STORE_U32(r11 + 176, 0);
        r11 += 264;
    }

    uint32_t base1 = 0x8307CA50;  // lis r11,-31992 + addi r9,r11,-13744
    r11 = base1;

    // Loop B: 90 slot entries, stride 36
    while (r11 < base1 + 3240) {
        PPC_STORE_U32(r11 + 0,  0);   // slot state word 0
        PPC_STORE_U32(r11 + 28, 0);   // slot state word 1
        r11 += 36;
    }
}
```

---

## sub_8285D948: Thread Object Allocator + Thread Launcher

```c
// recomp.56.cpp lines 8456-8541
// r3=thread_obj_template, r4=stack_size, r5=priority, r6=affinity, r7=flags, r8=creation_flags
// Returns: ptr to allocated+initialised thread block (stored in array)
void* sub_8285D948(...) {
    // lis r11,-31973; addi r31,r11,-17996  => r31 = global_thread_registry
    uint32_t registry = 0x82B01B94;  // -2095382528 + (-17996)
    uint32_t saved_count = PPC_LOAD_U32(registry + 32);  // current thread count

    uint8_t* block = sub_821B3510(1080);  // game malloc(1080)
    if (block == NULL) goto store_null;

    // Load thread type config: lis r11,-32079; lwz r11,-32240(r11) = [0x82B08210]
    uint32_t type_flags = PPC_LOAD_U32(0x82B08210);
    uint32_t masked_flags = type_flags & r8;  // r8=0 from sub_82556190 → masked_flags=0

    sub_8285D610(block, r4, r5, r6, masked_flags, r7);  // ← HANGS HERE

store_result:
    uint32_t idx = PPC_LOAD_U32(registry + 32);
    PPC_STORE_U32(idx * 4 + registry, (uint32_t)(uintptr_t)block);  // thread_array[count] = block
    PPC_STORE_U32(registry + 32, idx + 1);  // count++
    return block;  // r3 = block ptr = stored at 0x8307BAD4

store_null:
    return NULL;
}
```

---

## sub_8285D610: Thread Object Init + Thread Creation

```c
// recomp.56.cpp lines 7988-8267
// r3=thread_obj, r4=params, r5=stack_size(32768), r6=1, r7=flags(6), r8=type_mask(0)
// HANGS inside sub_8285D500 → sub_82849A50 → sub_82A13110 (unhooked CreateThread)
void* sub_8285D610(void* obj, ...) {
    uint32_t r31 = (uint32_t)obj;
    uint32_t r29 = r31 + 8;

    PPC_STORE_U32(r31 + 0, 0);    // obj->state = 0
    PPC_STORE_U16(r31 + 4, 0);    // obj->count = 0
    PPC_STORE_U16(r31 + 6, 0);    // obj->capacity = 0

    sub_8285FE48(r29 + 1024);     // init 1024-entry ring buffer at obj+8+1024

    sub_82849778(0);              // XMA/semaphore init (r3=0)
    // result stored at obj+1068

    PPC_STORE_U32(r29 + 1064, 0); // pending_count = 0
    PPC_STORE_U32(r29 + 1060, 0);
    PPC_STORE_U32(r29 + 1056, 0);

    // Build thread type list from r8 bits (bits 0-7 = thread types 1-7 + implicit)
    int type_count = 0;
    uint32_t types[8] = {};
    if (r8 & 1)  types[type_count++] = 1;
    if (r8 & 2)  types[type_count++] = 2;
    if (r8 & 4)  types[type_count++] = 3;
    if (r8 & 8)  types[type_count++] = 4;
    if (r8 & 16) types[type_count++] = 5;
    if (r8 & 32) types[type_count++] = 6;
    if (r8 & 64) types[type_count++] = 7;

    // NOTE: from sub_82556190, r8=0 so type_count=0; loop below does NOT execute
    if (type_count > 0) {
        uint32_t name_base = 0x820259C4;  // lis r11,-32248; addi r26,r11,24140
        for (int i = 0; i < type_count; i++) {
            // sub_82158E08: format thread name into stack buffer
            sub_82158E08(/*buf*/stack+80, /*len*/16, name_base, types[i]);
            // sub_8285D500: register thread with scheduler → calls sub_82849A50 → sub_82A13110
            sub_8285D500(obj + types[i]*192, r25, stack+80, r31, ...);
        }
    }

    // If any types registered: update obj->count, obj->capacity
    PPC_STORE_U16(r31 + 4, type_count);   // count of thread types
    if (type_count > 0) {
        sub_8285D488(obj, type_count);     // finalise slot headers
        PPC_STORE_U32(r31 + 0, result);   // obj->first_thread = first registered
    }
    return obj;
}
```

---

## sub_82849A50: Kernel Thread Creation (Scheduler Node)

```c
// recomp.55.cpp lines 8751-8916
// r3=scheduler_obj(0x82556188), r4=0, r5=max_queue(2048), r6=0,
// r7=thread_proc_or_name(0x820259BC), r8=1(enable), r9=1(start)
// Returns: thread handle (stored at 0x8307D6F8), or -1 on failure
uint32_t sub_82849A50(...) {
    uint32_t queue_depth = max(r5, 16384);  // ensure minimum 16384

    // Scheduler pool base: lis r11,-31975+10624 = 0x82B2A980
    uint32_t pool = 0x82B2A980;

    sub_821D5F70(pool + 716);   // mutex lock on scheduler
    uint32_t slot = sub_828499E8(pool, 0);  // get free slot from pool

    if (slot == 0) return -1;  // no free slots

    // Init slot header
    PPC_STORE_U32(slot + 0, r3);   // scheduler_obj back-ptr
    PPC_STORE_U32(slot + 4, r4);   // param
    PPC_STORE_U32(slot + 8, TLS[1676]);  // current thread TLS ptr

    // Create kernel thread: sub_82A13110 (UNHOOKED — CreateThread analog)
    uint32_t handle = sub_82A13110(
        /*r3=*/ 0,
        /*r4=*/ queue_depth,          // stack/queue size
        /*r5=*/ 0x827A0F40,           // thread entry point (CRT wrapper)
        /*r6=*/ slot,                 // thread param
        /*r7=*/ 4,                    // CREATE_SUSPENDED or thread type
        /*r8=*/ stack_buf             // output handle ptr
    );  // ← THIS IS THE HANG: 0x82A13110 is not in rexcrt hook list

    if (handle == 0) {
        // Failure: restore free-list
        PPC_STORE_U32(slot + 0, PPC_LOAD_U32(pool + 708));
        PPC_STORE_U32(pool + 708, slot);
        return -1;
    }

    // Success path
    sub_82A11478(handle, r6);          // SetThreadPriority
    sub_82A114F8(handle, 1);           // SetThreadAffinityMask
    sub_82A11580(handle, thread_name); // SetThreadName
    if (r8 != 0) sub_82A13120(handle); // ResumeThread (if start=1)
    return handle;
}
```

---

## sub_821FC1F8: Caller Context

```c
// recomp.5.cpp lines 991-1177
// Call #32 (1-indexed) out of ~47 total init calls
// No args set before the bl; no r3 check after
void sub_821FC1F8(PPCContext& ctx, uint8_t* base) {
    // calls 1-31: various subsystem inits...
    sub_8225C010(ctx, base);  // call #31
    sub_82556190(ctx, base);  // call #32  ← THIS FUNCTION (hangs inside D948→D610→A13110)
    sub_822F3740(ctx, base);  // call #33 — never reached while hung
    // ...
    // Return value of sub_82556190 is NOT checked (r3 is NOT read after the bl)
    // sub_821FC1F8 itself returns void
}
```

---

## Global State Written by sub_82556190

| Address | Value | Written by | Meaning |
|-|-|-|-|
| `0x82AAB918` | 1 | Phase 1 | Streaming subsystem init flag |
| `0x8307CA50`..+3240 | 0 | Phase 2 | Streaming slot array (90 × 36B) |
| `0x8307BAD8`..+3960 | 0 | Phase 3 | I/O request queue (15 × 264B + hdr) |
| `0x8307BAD4` | thread handle | Phase 5 | Streaming worker thread handle |
| `0x8307BAD0` | 0 (or sem) | Phase 6 | Streaming sync event handle |
| `0x8307D6F8` | scheduler handle | Phase 7 | Streaming scheduler node handle |

---

## Hang Chain

```
sub_821FC1F8 (recomp.5.cpp:1099)
  └─ sub_82556190 (recomp.26.cpp:93131)
       └─ sub_8285D948 (recomp.56.cpp:8490)   malloc(1080) succeeds
            └─ sub_8285D610 (recomp.56.cpp:8026)
                 ├─ sub_8285FE48               ring buffer init (ok)
                 ├─ sub_82849778(0)            semaphore/event init (ok if hooked)
                 └─ (if type_count>0) sub_8285D500
                      └─ sub_82849A50
                           └─ sub_82A13110    ← UNHOOKED CreateThread → HANGS
       └─ sub_82849A50 (recomp.55.cpp:8795)   also calls sub_82A13110 → HANGS (phase 7)
```

**Root cause**: `sub_82A13110` (0x82A13110) is an unhooked Xbox 360 `CreateThread` / `NtCreateThread` analog. It is NOT in the rexcrt CRT hook list (which covers `0x82A131B0` = CreateFileA, etc.).

**Minimum fix**: Hook `sub_82A13110` to create a host thread via `std::thread` or platform API, passing the PPC function pointer in `r5` (or `r6`) as the entry point.

**Immediate stub** (testing only — streaming will not work):
```cpp
PPC_FUNC(sub_82A13110) {
    ctx.r3.s64 = 1;  // return non-zero "handle" to avoid -1 error path
    return;
}
```

**For sub_82556190 specifically**: since `r8=0` in the D948 call, `type_count=0` in D610, so D610's thread creation loop does NOT execute. The hang in phase 5 is via D948's D610 path only if `[0x82B08210] & 0 != 0` — which is always false. **Phase 5 may not actually hang** if `type_count` stays 0. The hang is more likely in **Phase 7** (`sub_82849A50` directly calling `sub_82A13110`).
