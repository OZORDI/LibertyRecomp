# sub_821B3510 (operator new) Hang Analysis

## Function Mechanics

### sub_821B3510 — RAGE operator new(size)

Disassembly (gta4_recomp.2.cpp:83062):

```
r11 = PPC_LOAD_U32(r13 + 0)          // TLS block base
r3  = PPC_LOAD_U32(r11 + 1676)       // TLS[1676] = sysMemAllocator*
r4  = size (moved from original r3)
r5  = 16 (default alignment)
r6  = 0  (flags)
r11 = PPC_LOAD_U32(r3 + 0)           // vtable pointer
r11 = PPC_LOAD_U32(r11 + 8)          // vtable[2] = Alloc virtual method
PPC_CALL_INDIRECT_FUNC(r11)          // call Alloc(this, size, align, flags)
```

Key observation: **NO null check on TLS[1676]**. If the allocator pointer is 0, it reads guest address 0 as the vtable, then guest address [vtable+8] as the Alloc function address, and dispatches into whatever function that resolves to.

### Comparison with sub_8218BE28 (malloc)

sub_8218BE28 has a PPC_FUNC_HOOK (imports.cpp:901) that reads TLS[1676], validates it via IsValidAllocator(), and falls back to SystemHeapAlloc if invalid.

sub_821B3510 has only a diagnostic wrapper (imports.cpp:1790) — it logs entry/return but calls `__imp__sub_821B3510` unmodified. No TLS[1676] validation, no fallback.

## Hang Scenario

### Call Sequence in sub_82478AF8

From gta4_recomp.20.cpp:83744, the audio system init function:

| Step|Address|Action|Result|
|-|-|-|-|
|1|0x82478B1C|sub_821B3510(176)|SUCCESS — alloc #1|
|2|0x82478CDC|sub_821B3510(192)|SUCCESS — alloc #2|
|3|0x82478E98|sub_8284F310 — VFS scope enter|counter++|
|4|0x82478E9C|sub_82477670 — streaming tick|OK|
|5|0x82478EA8|store streaming name to object+56|OK|
|6|0x82478EAC|sub_827C2420 — streaming activation|OK but side effects|
|7|0x82478EB8|sub_8284E830 — VFS scope exit|counter--|
|8|0x82478EC4|**sub_821B3510(1024) — HANGS**|never returns|

### Root Cause: TLS[1676] Corruption via Unprotected Push/Pop

#### The protected wrappers (HOOKED in imports.cpp:837-891):
- sub_827D85E0 (push wrapper) — hook prevents writing 0 into TLS[1676]
- sub_827D8620 (pop wrapper) — hook prevents restoring 0 from TLS[1672]

#### The raw push/pop (NOT HOOKED):
- **sub_828470E0** (raw push) — called directly by 20+ call sites
- **sub_82847120** (raw pop) — called directly by 20+ call sites

sub_828470E0 behavior:
```
current = TLS[1676], target = TLS[1680]
if current == target: TLS[1668]++ (refcount), return
else: TLS[1676] = target, TLS[1672] = current
```

sub_82847120 behavior:
```
if TLS[1668] > 0: TLS[1668]--, return
else: TLS[1676] = TLS[1672], TLS[1672] = 0
```

If TLS[1680] is 0 (or an invalid pointer) when a raw push fires, TLS[1676] gets set to 0/invalid. The protective hooks on the wrappers cannot help because **the raw functions are called directly**.

#### Where it fires during the hang path

Inside sub_82852D18 (called from sub_82852DD0, called from sub_827C2420):
- Line 31045 in gta4_recomp.55.cpp: calls sub_828470E0 conditionally (flag bit 17 check)
- Line 31063: calls sub_82847120 to restore

The streaming system also has push/pop pairs in sub_827C2498 (the singleton initializer called from the sub_827C2650 outer function), and throughout the VFS file-resolution chain.

### What Happens After Corruption

If TLS[1676] = 0 when sub_821B3510 executes:
1. `r3 = PPC_LOAD_U32(0)` — reads guest address 0x00000000 (mapped in Xbox 360 memory model)
2. `r11 = PPC_LOAD_U32([garbage])` — reads a "vtable" from whatever data is at address 0
3. `r11 = PPC_LOAD_U32([garbage]+8)` — reads a "method" pointer from random memory
4. `PPC_CALL_INDIRECT_FUNC(r11)` — dispatches to a random function

If the random function happens to resolve to a valid recompiled function that:
- Acquires a critical section (RtlEnterCriticalSection) already held by another thread, OR
- Enters an infinite loop, OR
- Calls NtWaitForSingleObject on a never-signaled event

...then it hangs. The PPC_CALL_INDIRECT_FUNC macro (context.h:127-140) prints `[MISSING-FUNC]` and returns harmlessly ONLY if the address has no registered function. If it accidentally resolves to a registered function, it calls it.

### Alternative Hang: Allocator Lock Contention

Even if TLS[1676] is valid, the RAGE allocator's Alloc method acquires an RTL_CRITICAL_SECTION internally. The RtlEnterCriticalSection implementation (xboxkrnl_rtl.cpp:386) uses:
1. Spin loop (up to `spin_count` iterations)
2. If spin fails: `xeKeWaitForSingleObject` — blocks until the critical section is released

If sub_827C2420 triggered a streaming worker thread that acquired the heap critical section and is now blocked waiting for the main thread (e.g., waiting for an event the main thread is supposed to signal), a classic AB-BA deadlock occurs:
- Main thread: holds init progress, waiting on heap lock
- Worker thread: holds heap lock, waiting on streaming event from main thread

## TLS Allocator Slot Layout

| Offset|Name|Purpose|
|-|-|-|
|1668|refcount|push/pop nesting depth counter|
|1672|saved|backup of previous allocator (set by push)|
|1676|current|active sysMemAllocator* — THE allocator|
|1680|target|target allocator for next push operation|

## Recommended Fix

### Option A: Hook sub_821B3510 with TLS[1676] validation (matches sub_8218BE28 pattern)

```cpp
PPC_FUNC_HOOK(sub_821B3510) {
    uint32_t r13 = ctx.r13.u32;
    uint32_t memMgr = ReadTLS(r13, 1676);

    if (!IsValidAllocator(memMgr)) {
        uint32_t size = ctx.r3.u32;
        auto* ks = rex::system::kernel_state();
        auto* mem = ks ? ks->memory() : nullptr;
        if (mem) {
            uint32_t guest = mem->SystemHeapAlloc(size);
            ctx.r3.u32 = guest;
            printf("[OP-NEW FALLBACK] size=%u -> 0x%08X caller=0x%08X\n",
                   size, guest, (uint32_t)ctx.lr);
        } else {
            ctx.r3.u32 = 0;
        }
        return;
    }
    __imp__sub_821B3510(ctx, base);
}
```

### Option B: Hook sub_828470E0 and sub_82847120 with same protection as wrapper hooks

This is the more thorough fix — prevents ALL allocator corruption, not just operator new.

```cpp
PPC_FUNC_HOOK(sub_828470E0) {
    uint32_t r13 = ctx.r13.u32;
    uint32_t cur    = ReadTLS(r13, 1676);
    uint32_t target = ReadTLS(r13, 1680);

    if (!IsValidAllocator(target) && IsValidAllocator(cur)) {
        // Would zero TLS[1676] — just bump refcount instead
        uint32_t refcnt = ReadTLS(r13, 1668);
        WriteTLS(r13, 1668, refcnt + 1);
        return;
    }
    __imp__sub_828470E0(ctx, base);
}

PPC_FUNC_HOOK(sub_82847120) {
    uint32_t r13 = ctx.r13.u32;
    uint32_t refcnt = ReadTLS(r13, 1668);
    uint32_t saved  = ReadTLS(r13, 1672);
    uint32_t cur    = ReadTLS(r13, 1676);

    if (refcnt == 0 && !IsValidAllocator(saved) && IsValidAllocator(cur)) {
        // Would zero TLS[1676] — skip the restore
        return;
    }
    __imp__sub_82847120(ctx, base);
}
```

### Option C: Both A and B (belt and suspenders)

Recommended. Option B prevents corruption at the source. Option A catches any remaining cases where TLS[1676] is still null when operator new fires.

### Additional: Hook sub_821B3538 and sub_821B3560

sub_821B3538 is `operator new(size, alignment)` — same vtable[8] dispatch, same vulnerability.
sub_821B3560 is `operator delete(ptr)` — reads vtable[12] instead of vtable[8], same null-deref risk.

## Key File Locations

- sub_821B3510 implementation: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.2.cpp:83062`
- sub_828470E0 (raw push): `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.55.cpp:2436`
- sub_82847120 (raw pop): `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.55.cpp:2475`
- Existing allocator hooks: `LibertyRecomp/kernel/imports.cpp:828-991`
- Existing sub_821B3510 diagnostic hook: `LibertyRecomp/kernel/imports.cpp:1790`
- sub_82478AF8 (audio init, hang site): `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.20.cpp:83744`
- sub_827C2420 (streaming activation): `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.50.cpp:57394`
- RtlEnterCriticalSection: `tools/rexglue-sdk-main-1/src/kernel/xboxkrnl/xboxkrnl_rtl.cpp:386`
- PPC_CALL_INDIRECT_FUNC macro: `glue/rexglue-sdk-main/include/rex/ppc/context.h:127`
