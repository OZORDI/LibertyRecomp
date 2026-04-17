# Audio Pool Crash — Agent 6: Sibling Caller of `sub_828BF270`

**File**: `06-sibling-caller-827bf.md`
**Agent**: 6 of 10
**Crash**: `sub_828C2300+0x34` (`stw r11, 0(r31)`); r31=0x20 (expected 0x831C2D38), r9=0xFFE1E1E1 (RAGE freed-poison), LR=0x8227F3AC.
**Targets**: `sub_827BF3C0` (sibling caller of `sub_828BF270`), `sub_828BF270` (verify contract), and the immediate caller chain needed to make sense of them.

---

## Executive Summary (read this first)

Two surprises emerged that change the working theory:

1. **`sub_828BF270` is NOT a 4-instruction leaf.** The recomp scaffold shows it as 3 PPC instructions, but the third is `b 0x82a3df50` — a **tail call** to `sub_82A3DF50`. So `sub_828BF270` is best modelled as a 1-line forwarder. It is *register-safe* with respect to non-volatile registers (it touches only `r11`, `r3`, and the LR slot already saved by the caller).
2. **The supplied `LR=0x8227F3AC` does NOT correspond to a return from `sub_828BF270`.** It is the return address slot for `bl 0x828c2290` inside `sub_8227F2E8` (the first of FOUR consecutive `bl sub_828C2290` calls before the eventual `bl sub_828C2300` at +0x140). This means the crash is not "callee `sub_828BF270` returned and immediately blew up the parent" — it is "we are deep inside the queue-flush state machine and arrived at `sub_828C2300` for the *flush* call after a sequence of `sub_828C2290` *enqueue* calls."

Combined with `r9=0xFFE1E1E1` (RAGE `EE` freed-memory pattern for an audio/queue pool block) and r31 reading 0x20 (an offset, not a pointer — strongly suggesting a stack-restore from a clobbered save area), the corruption is upstream of `sub_828BF270`'s callee chain. **`sub_828BF270` is innocent of the corruption.** The differential below proves it.

---

## Global Map (`lis -31972` = `0x831C0000` region)

All four functions touch the same module's `.bss` cluster. Verified via Python:

|Offset|Address|Used by|Purpose|
|-|-|-|-|
|+0x22A4 (8868)|0x831C22A4|`sub_828BF270`|Pool A: ptr loaded at `[ea]+0`, passed as `r3` to `sub_82A3DF50`|
|+0x2D24 (11556)|0x831C2D24|`sub_828C2300` `[r31-16]`, `sub_828C2290` `[r10-20]`|Pool B: "currently-active item" ptr|
|+0x2D38 (11576)|0x831C2D38|`sub_828C2300` `r31`, `sub_828C2290` `[r10+0]`|Pool B: per-frame item counter|
|+0x3DD8 (15832)|0x831C3DD8|`sub_828C6500`/`sub_828C6568`|Pool B: "scratch DMA staged" byte flag (+15833)|
|+0x3DDC (15836)|0x831C3DDC|`sub_828C6568`/`sub_828C6500`|Pool B: scratch buffer ptr (freed by `sub_827BAEE0`)|
|+0x3DE0 (15840)|0x831C3DE0|`sub_828C60A0`|Pool B: alternate "item open" ptr|

**Critical:** Pool A (offset 0x22A4) and Pool B (offsets 0x2D24..0x3DE0) are **2708 bytes apart** in the same module .bss but are logically distinct: `sub_828BF270` operates on Pool A, the crash site `sub_828C2300` operates on Pool B.

```py
python3 -c "
base = 0x831C0000
print(f'Pool A (sub_828BF270): 0x{base+8868:08X}')
print(f'Pool B (sub_828C2300): 0x{base+11576:08X}  (counter)')
print(f'Pool B currently-active ptr: 0x{base+11556:08X}')
print(f'Distance A→B counter: {11576-8868} bytes')
"
# Pool A (sub_828BF270): 0x831C22A4
# Pool B (sub_828C2300): 0x831C2D38  (counter)
# Pool B currently-active ptr: 0x831C2D24
# Distance A→B counter: 2708 bytes
```

---

## `sub_828BF270` — Full reconstruction (THE LEAF in question)

3 instructions; tail-calls into a 3-instruction setter. Both functions only touch `r3` and `r11`.

```cpp
// 0x828BF270  size 0x10
extern uint8_t* g_module_bss;             // base = 0x831C0000
extern uint32_t g_poolA_param;            // [g_module_bss + 8868] = *(u32*)0x831C22A4

// Generated:
//   lis r11,-31972        ; r11 = 0x831C0000
//   lwz r3,8868(r11)      ; r3 = *(u32*)0x831C22A4 (Pool A obj ptr)
//   b   0x82A3DF50        ; tail-call (NOT bl — no return here)
inline void sub_828BF270(/*unused*/) {
    void* poolA_obj = *(void**)(g_module_bss + 8868);
    sub_82A3DF50(poolA_obj);   // tail-call; LR untouched after caller's bl
}
```

### `sub_82A3DF50` — what `sub_828BF270` actually does

```cpp
// 0x82A3DF50  size 0x10  (5 callers — generic field-copy helper)
//   lwz r11,13428(r3)
//   stw r11,48(r3)
//   blr
inline void sub_82A3DF50(void* obj) {
    uint8_t* p = (uint8_t*)obj;
    *(uint32_t*)(p + 48) = *(uint32_t*)(p + 13428);
    // i.e. obj->field_30 = obj->field_3474
}
```

**Net effect of `sub_828BF270`:** load Pool A's primary object pointer from .bss, then copy a `u32` from offset +0x3474 into offset +0x30 of that object. That's it.

### Register-safety audit (the key question)

PPC ABI non-volatile registers: r13–r31, f14–f31, v20–v31, CR2/3/4. `sub_828BF270` and `sub_82A3DF50` together touch only `r3`, `r11`, and (for the original `bl` from the caller) the LR slot the caller already saved. **`r31` is provably untouched** in both functions' scaffolds. So whatever set r31=0x20 in `sub_828C2300` did not come from this callee chain.

---

## `sub_827BF3C0` — Full reconstruction (sibling caller)

The other (and only other) caller of `sub_828BF270`. 0x210 bytes, 10 callees, called once (from `sub_827C0C08` at offset deep in a 0x8C0 function — likely a per-frame audio/effect tick). Operates on a 768+ byte struct passed in `r3`.

```cpp
// 0x827BF3C0  size 0x210  (1 caller: sub_827C0C08)
// Argument: r3 = pointer to "AudioEmitter"-like struct (call it Emitter*)
//   - +16    : sub-object passed to sub_826706F8 (geometry/transform helper)
//   - +116   : float scalar (squared in distance falloff)
//   - +128   : v0 vector (loaded into v125 base, scaled)
//   - +692   : output vector array base passed to inner alloc loop
//   - +700   : ptr to "AudioVoice"-like obj; AudioVoice->[+12] = "Channel*"
//   - +764   : u32 passed as r6 to sub_828C6568 (channel/zone id)

void sub_827BF3C0(Emitter* emitter)
{
    // === Setup phase ===
    // (1) Geometry/falloff prep on emitter[16] using stack scratch slots
    float    stack_f80;          // [r1+80]   — scalar scratch
    Vec4     stack_v96;          // [r1+96]   — vector scratch (copy of emitter[128])
    Vec4     stack_v112;         // [r1+112]
    Vec4     stack_v128;         // [r1+128]
    stack_v96 = emitter[128];    // lvx128 v0,r31,128  ;  stvx128 v0,r0,r1+96
    sub_826706F8(&emitter[16],
                 &stack_v96, &stack_v128, &stack_v112);

    // (2) Compute "base gain" from a hard-coded float at .rdata-4808(lis -32244)
    Channel* ch = emitter->voice->channel;       // emitter[700]->[12]
    float baseGain = *(float*)(0x82197000 - 4808);   // some constant
    stack_f80 = baseGain;
    Vec4 v_base = stack_v96;                     // copy out for vector mul
    Vec4 v_gain = vspltw(loadVecLeft(&stack_f80), 0);  // splat scalar to all lanes
    Vec4 v125_acc = vmulfp(v_base, v_gain);      // running accumulator (Vec4 v125)

    // (3) Try to begin a "queue session" against the channel
    int sessionOk = sub_828C6568(ch,
                                 /*r4*/0, /*r5=mode*/1, /*r6*/emitter[764]);
    if (!sessionOk) goto done;       // beq cr6,0x827bf5a4

    // (4) Per-channel state init (some kind of "open queue header")
    sub_828C64C8(ch, 0);

    // === Distance / direction math (the long vector block) ===
    float scalarSq = emitter[116] * emitter[116];
    Vec4 v_dir1   = stack_v112;                  // lvx128 v0,r0,r1+112
    Vec4 v_dir2   = stack_v128;                  // lvx128 v13,r0,r1+128
    Vec4 v_const1 = vspltw(load(0x82197000 - 24580), 0); // lis -32082 + 23996
    float c1      = *(float*)(0x82190000 + 3444);    // lis -32256 + 3444
    float c2      = *(float*)(0x82197000 - 24584);   // lis -32082 + 23992
    float fall    = scalarSq * c1 * c2;
    stack_f80 = fall;

    Vec4 v_lhs    = vspltw(loadVecLeft(&stack_f80), 0);  // splat fall
    Vec4 v_rhs    = vspltw(loadVecLeft(&stack_f80), 0);
    Vec4 v127     = vmulfp(v_dir1, v_lhs);
    Vec4 v126     = vmulfp(v_dir2, v_rhs);
    Vec4 v_sum    = vaddfp(v127, v126);
    v125_acc      = vmaddfp(v_sum, v_const1, v125_acc);  // v125 += v_sum * c

    // (5) Try to allocate 16-byte aligned slots in some output buffer
    void* slot = sub_827BEFF0(/*r3*/&emitter[692],
                              /*r4*/&emitter[692],
                              /*r5*/16);          // count=16
    if (!slot) goto closeAndExit;

    // (6) Write the accumulator into slot[0..15] and tag with a "max range" float
    *(Vec4*)slot = v125_acc;                     // stvx128 v125,r0,r3
    float fMaxRange = *(float*)(0x82190000 + 3400);
    *((float*)slot + 3) = fMaxRange;             // slot[+12]

    // (7) Write 9 more entries (the loop). Each entry interpolates v127/v126
    //     based on sin/cos of a stepped angle.
    Vec4* outp  = (Vec4*)((uint8_t*)slot + 16);
    float dStep = *(float*)(0x82182000 - 30204); // step constant (lis -32254 + (-30204))
    for (int i = 0; i < 9; ++i) {
        float angle = (float)i * dStep;
        // Inline math = sub_829FFE18(sin) and sub_829FFD48(cos)
        stack_f80 = sub_829FFE18(angle);          // sinf
        float c   = sub_829FFD48(angle);          // cosf
        *(float*)((uint8_t*)&stack_f80 + 4) = c;
        Vec4 vsin = vspltw(loadVecLeft(&stack_f80), 0);
        Vec4 vcos = vspltw(loadVecLeft((uint8_t*)&stack_f80 + 4), 0);
        Vec4 v    = vmulfp(v127, vsin);
        v         = vmaddfp(v126, vcos, v);
        v         = vaddfp(v, v125_acc);
        outp[0]   = v;
        ((float*)outp)[3] = fMaxRange;
        outp += 1;
    }

    // (8) THE CALL THAT MATTERS — flush/finalize the per-channel queue session.
    //     This is the OTHER call site of sub_828BF270.
    sub_828BF270();   // <-- siblings of sub_828C2300+bl 0x828bf270

closeAndExit:
    // (9) Close session, then release scratch. Both reload [emitter+700]->[12].
    sub_828C6500(ch);    // releases scratch buffer (g_poolB_scratch / +0x3DDC)
    sub_828C60A0(ch);    // nulls "currently-open" header (+0x3DE0)

done:
    return;
}
```

### Caller of `sub_827BF3C0` (one only): `sub_827C0C08`

A 0x8C0-byte per-frame routine with 22 callees including `sub_821479F8`, `sub_827BE080/130/648`, `sub_827BF3C0`. Likely the per-frame "audio-emitter tick / 3D voice update" routine. Not reproduced in full because it does not affect the pool-corruption analysis (it just calls `sub_827BF3C0` once with a fresh `Emitter*` from a list it owns — that pointer is irrelevant to the global Pool B state at the crash site).

---

## Contract of `sub_828BF270` inferred from BOTH call sites

|Aspect|Site A: `sub_828C2300+0x24`|Site B: `sub_827BF3C0+0x1C8`|
|-|-|
|Pool acted on|Pool B (`+11576/+11556` global counter+open-ptr)|Pool A (`+8868` indirect, via `sub_82A3DF50`)|
|Pre-call invariant set up by caller|`if ([+11556] != 0) { call }; [+11556] = 0; [+11576] = 0`|`if (sub_828C6568 succeeded && sub_827BEFF0 returned non-null) { call }`|
|Lock / mutex acquired around call|None visible|None visible (relies on `sub_828C6568` having opened a session via byte-flag at +15833)|
|`r3` setup at call|None (function loads its own from .bss)|None (function loads its own from .bss)|
|Post-call work|`[+11576] = 0`|`sub_828C6500(ch); sub_828C60A0(ch);`|
|What sub_828BF270 actually mutates|Pool A's `obj->field_30 = obj->field_3474`|same — Pool A only|

### Inferred contract

`sub_828BF270` is a **commit/sync hook** that snapshots Pool A's "pending" field into the "current" field. Both call sites use it at "end of an enqueue burst" — site A at flush of the per-frame `sub_828C2290`-fed item array (Pool B), and site B at finalization of a per-channel scatter of 10 vector entries (Pool B's scratch). **Pool A is the side-channel** — likely an "audio command list head" or "DSP voice mailbox" pointer that needs to advance in lockstep when *any* Pool B flush happens. The two pools are siblings, not the same.

The pre-condition `*(u32*)0x831C22A4 != nullptr` is **not checked** by either caller. If `g_poolA_param` is null and an unmapped guest address translates outside the heap window, the recomp would fault inside `sub_82A3DF50` — but that is not where we are crashing.

### Register safety: confirmed

`sub_828BF270` writes only `ctx.r11` and `ctx.r3`. `sub_82A3DF50` writes only `ctx.r11`. Neither touches `ctx.r31`, ctx.f29-31, or any v20-31 vector register. The crash's r31 corruption did **not** come from this leaf.

---

## Diff between the two call sites

|Property|`sub_828C2300` call site|`sub_827BF3C0` call site|
|-|-|
|Caller's own r31|Pool B counter address (0x831C2D38)|Caller's own argument (Emitter*)|
|Guard before `bl 0x828bf270`|`if ([Pool-B-active-ptr] != 0)` |`if (sub_828C6568 ok && sub_827BEFF0 returned non-null)`|
|Stack frame size at call|96 bytes|256 bytes|
|Save of r31 in caller|`std r31,-16(r1)` *before* `stwu` (so save slot is at original `[r1-16]` = post-stwu `[r1+80]`)|`bl __savegprlr_29` then `stwu r1,-256(r1)` (save slot is post-stwu `[r1+256-?]` = compiler-generated)|
|Live values across the call|None — caller is about to NULL the same .bss slot it tested|`v125`/`v126`/`v127` (non-volatile vectors holding accumulator) and r31=Emitter*, r29..r30 also live|
|Order of work after the call|`[+11576] = 0; epilogue`|2 more session-cleanup calls then epilogue|

**Same callee, different pool, different lock discipline.** Site A is "I tested-and-reset a global counter" — the `bl` is fire-and-forget. Site B is "I successfully opened a per-channel session via `sub_828C6568` (which sets the `+15833` byte flag and writes `+15836` ptr)" — the `bl` is part of an explicit open/commit/close sequence.

---

## Conclusion

### Is `sub_828BF270` a plausible source of the crash?

**No.** Three independent reasons:

1. **It does not touch `r31`.** The full callee chain `sub_828BF270` → tail-call → `sub_82A3DF50` mutates only `r3`/`r11`. The PPC scaffold respects the ABI here. If the recomp were buggy and silently clobbered r31, both call sites would crash, not just the `sub_828C2300` one.
2. **It operates on Pool A (+0x22A4), not Pool B (+0x2D38).** The crash site stores into Pool B's counter slot. `sub_828BF270` never reads or writes any address in the Pool B cluster.
3. **`r31=0x20` is not a heap pointer or a poisoned freed-block.** It is the literal small integer 32. The most plausible mechanism is that `sub_828C2300`'s prologue did `std r31,-16(r1)` before `stwu r1,-96(r1)` (so the save slot is at original `r1-16`, which becomes `r1+80` after stwu). On the epilogue path it does `addi r1,r1,96` *first*, then `ld r31,-16(r1)` from the new (restored) r1 — i.e. from the slot 16 below the *caller's* r1. **This is correct PPC convention**, but if anything overwrote the caller's red-zone or if the recomp's stwu/`addi r1` math is off by anything, you would reload garbage into r31. The value 0x20 looks like an `extsw` of a small immediate (maybe an old copy of `r4=0` or `r5=10` from the inner `sub_828C64C8`/loop counter living somewhere in the caller's spill region).

The actual r9 poison (`0xFFE1E1E1`) and crash address strongly suggest **the upstream `sub_828C2290` enqueue calls (4 of them in `sub_8227F2E8`) are filling slots in Pool B that point into already-freed RAGE blocks.** When `sub_828C2300` then runs the flush, it walks Pool B, dereferences a freed entry (giving us `r9=0xFFE1E1E1`), and somewhere in that walk a chained store overwrites the Pool B header at offset 0 of one of those freed blocks — which had been re-typed as "stack frame" by an unlucky later allocator. The "0x20" we see in r31 is the offset field at the head of that re-typed block.

### What the differential tells us about pool lifecycle

- **Pool A is a singleton** — only one global ptr in the entire module references it. It is a "header" object (likely a DSP command-list root), and `sub_828BF270` is the only function that touches its field +0x30/+0x3474 pair. Both sites flush it identically with no locking, which means it is **expected to be touched only on one thread** (the audio render thread).
- **Pool B is a per-frame array** — counter at `+11576`, "currently-open item" at `+11556` set by `sub_828C2290`, and the entries themselves live elsewhere (allocated through `sub_828C6568`/`sub_828C64C8`'s `r10+8` chain). `sub_828C2300` is the per-frame finalizer that releases the open-item ptr.
- **The two flushes are coupled** because `sub_828BF270` updates Pool A's `field_30` (a "last-flushed-list head" or similar) every time *either* site flushes. This is a one-way data dependency: Pool B → Pool A.

### Where Agent 6 thinks the corruption originates (educated handoff)

Look at `sub_828C2290` (267 callers, hot, leaf). Its `loc_828C22C0` branch path writes 8 floats and an `r9` value into `[r11+0..32]`, then `stw r8,-20(r10)` updates the active-item ptr to `r11+36`. **If `r11` was an already-freed block, those 9 stores stamp poison-overlay garbage and the next `sub_828C2290` re-uses the same stale ptr** (since `[+11556]` was advanced by 36 bytes per call). After 4 of them in `sub_8227F2E8`, the array is full of dangling `r9` callbacks, and `sub_828C2300`'s flush then iterates them. The crash being at the *write* `stw r11,0(r31)` (not at a deref) in `sub_828C2300` is consistent with `r31` having been clobbered by an earlier store inside the recomp's own translation of the loop, not by `sub_828BF270`.

**Recommendation for the parent agent:** focus on the lifecycle of whatever object is at `[r11]` when `sub_828C2290` is entered — i.e. trace `sub_828C2290`'s allocator path (`[r10-20]` → set by `sub_828C64C8`'s `r10+8` chain → `add r3,r10,r9` where r9 = item_index << 5). The "pool slot allocator" is the suspect, not this leaf.

---

## Cross-reference for other agents

- This MD covers: `sub_827BF3C0`, `sub_828BF270`, `sub_82A3DF50`, plus enough of `sub_828C2300` / `sub_8227F2E8` / `sub_828C2290` / `sub_828C6568` / `sub_828C64C8` / `sub_828C6500` / `sub_828C60A0` / `sub_827BEFF0` / `sub_826706F8` to pin down the contract.
- Sibling agent expected to deep-dive `sub_828C2290`'s allocator chain and the `[+11556]` lifecycle.
- `sub_828BF270` can be **declared safe** for the purposes of the crash hypothesis. It is a 1-line forwarder. Do not waste cycles hooking it.
