# Audio Pool Helpers @ 0x828C6xxx — Agent 5 Report

Crash-context investigation of the four "tail" helpers called by `sub_8227F2E8`
(LR=0x8227F3AC at fault), siblings of the crashing `sub_828C2300+0x34`
(`stw r11,0(r31)` with r31=0x20).

All four functions live in
`glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.67.cpp`.

## 1. Summary table

|fn|addr|size|callers|callees|leaf?|hot?|
|-|-|-|-|-|-|-|
|sub_828C60A0|0x828C60A0|0x10 (16 B)|28|0|YES|no|
|sub_828C64C8|0x828C64C8|0x38 (56 B)|27|1 (sub_828C8F38, tail)|YES (no `bl`)|no|
|sub_828C6500|0x828C6500|0x68 (104 B)|28|1 (sub_827BAEE0)|no|no|
|sub_828C6568|0x828C6568|0xB8 (184 B)|29|2 (sub_828C20B0, savegprlr)|no|no|

Caller set is virtually identical for all four (~28 callers, 100% overlap with
the crash-site siblings sub_828C2300 / sub_828C21D0 / sub_828C2290): the four
form a fixed *open / lookup / record / commit / close* sequence used by every
audio-asset emitter in the engine. The crashing routine `sub_8227F2E8` calls
them in canonical order:

```
sub_828C19C0 (slot register, dispatch on r3=0..36)
sub_8227EE90 (audio listener cfg)
sub_828C6568 (lookup) -> writes 0x831C3DD9 (PoolCtx.flags) + 0x831C3DE0 (PoolCtx.cur)
sub_828C64C8 (insert) -> writes 0x831C3DDC (PoolCtx.staging)
sub_828C21D0 (record begin) -> writes pool slot @ 0x831C2D28..0x831C2D3C
sub_828C2290 x4 (5x float param push, 36-byte stride)
sub_828C2300 (record end / handle release)  <-- CRASH SITE
sub_828C6500 (commit -> sub_827BAEE0 dispatch)
sub_828C60A0 (close: PoolCtx.cur = 0)
```

## 2. Reconstructed C++

### 2.1 sub_828C60A0 — close-context (zero `cur`)

```cpp
// Tail of the open/use/close sequence: clears AudioPoolCtx.cur.
// Single 16-byte leaf store. Mirrors the BC9 deadlock-style "release flag".
void AudioPool_CloseContext(/* r3 = unused, kept ABI-stable for callers */)
{
    *(uint32_t*)0x831C3DE0 = 0;   // PoolCtx.cur (last-prepared record ptr)
}
```

Address derivation (Python):
`(-31972 << 16) + 15840 == 0x831C0000 + 0x3DE0 == 0x831C3DE0`.

### 2.2 sub_828C64C8 — stage-record into pool (insert / promote)

```cpp
// r3 = AudioContext*, r4 = entryIndex
// Tail-calls sub_828C8F38(ctx, base+entry*32, base+entry*32+8,
//                          base+entry*32+16, base+entry*32+24).
//
// AudioContext layout (read here):
//   +20      : (passed as r6 to sub_828C8F38; sub-array head)
//   +24 (r11): pointer to a parent header.
//
// parent header (r11) layout:
//   +8       : passed as r7 (child A list)
//   +16      : passed as r4 (child B list)
//   +24      : passed as r5 (child C list)
//
// PoolCtx.staging (0x831C3DDC) cached for the call so that the
// dispatcher (sub_828C8F38) and finalizer (sub_828C6500) can
// see exactly what was published.
void AudioPool_Insert(AudioContext* ctx, uint32_t entryIndex)
{
    uint8_t*  hdr     = (uint8_t*)PPC_LOAD32(ctx + 24);
    uint32_t  stride  = (entryIndex << 5);                // *32
    uint32_t  poolBase= PPC_LOAD32(*(uint32_t*)0x831C3DE0 + 8);
    uint32_t  rec     = poolBase + stride;
    *(uint32_t*)0x831C3DDC = rec;                         // PoolCtx.staging

    sub_828C8F38(/*r3=*/rec,
                 /*r4=*/hdr + 16,
                 /*r5=*/hdr + 24,
                 /*r6=*/ctx + 20,
                 /*r7=*/hdr + 8);
}
```

Address derivation:
`(-31972 << 16) + 15836 == 0x831C3DDC` (staging),
`+15840 == 0x831C3DE0` (cur, dereferenced via `lwz r10,8(r10)` so cur is a
pointer-to-record-pool, not an integer).

### 2.3 sub_828C6500 — commit / drain (close insert)

```cpp
// r3 unused.
//   if (PoolCtx.flags @0x831C3DD9 != 0) {     // user supplied a real entry
//       sub_827BAEE0(PoolCtx.staging);        // walk hdr+0x18 list, dispatch
//   }
//   PoolCtx.staging = 0;
void AudioPool_Commit()
{
    uint8_t flags = *(uint8_t*)0x831C3DD9;
    if (flags != 0)
        sub_827BAEE0((void*)*(uint32_t*)0x831C3DDC);
    *(uint32_t*)0x831C3DDC = 0;
}
```

Note: even on the early-out path the staging pointer is zeroed — every
`sub_828C6500` call leaves `PoolCtx.staging == 0`. This is the symmetric
counterpart of `sub_828C64C8` which writes it.

### 2.4 sub_828C6568 — lookup / open-context (dispatch on user state)

```cpp
// r3 = AudioContext*, r4 = subIndex, r5 = flagByte, r6 = optHeader
// returns r3 = uint16_t at rec+12, or 0 if no match.
//
// Reads global "user/owner" at 0x82B0D1F8 (see crash-context: same -1
// sentinel pattern used by USER_STATE @0x82A22D04 elsewhere).
uint16_t AudioPool_Open(AudioContext* ctx,
                        uint32_t subIndex,
                        uint8_t  flagByte,
                        uint8_t* optHeader)
{
    uint32_t userId = *(uint32_t*)0x82B0D1F8;
    uint32_t channel;

    if (userId == 0xFFFFFFFFu) {                          // "no active user"
        channel = sub_828C20B0(/*idx=*/1);                // table[1] @0x831C2740
        if (channel != 0)
            channel = *(uint32_t*)(0x82B0D1F8 - 4);       // shadow @0x82B0D1F4
        else
            channel = *(uint32_t*)0x82B0D200;             // fallback chan
    } else {
        channel = userId;                                  // direct
    }

    uint8_t  selector;
    if (optHeader != nullptr) {
        selector = (uint8_t)(uintptr_t)optHeader;          // r29 reused as byte
    } else {
        // (channel + channel*2) -> 3*channel offset into a u8 sub-table
        uint8_t* sub = (uint8_t*)(channel * 3
                                  + PPC_LOAD32(ctx + 64));
        selector = sub[subIndex];                          // ctx+0x40 base
    }

    if (selector == 0)
        return 0;

    *(uint8_t*)0x831C3DD9 = flagByte;                      // PoolCtx.flags

    uint32_t poolBase = PPC_LOAD32(PPC_LOAD32(ctx + 24) + 0);
    uint32_t rec      = poolBase + (selector * 16) - 16;   // 16-byte stride, -1
    *(uint32_t*)0x831C3DE0 = rec;                          // PoolCtx.cur

    return PPC_LOAD16(rec + 12);                           // u16 @rec+12
}
```

`sub_828C20B0` is just `return *(uint32_t*)(0x831C2740 + (idx<<2));` — a 4-byte
table read of the per-pool descriptor at base `0x831C0000 + 10048 == 0x831C2740`
(this is the same dispatch table consulted by `sub_828C19C0`, slot 1
specifically).

## 3. Consolidated pool-state struct (the global at `0x831C0000`)

This is the data structure being torn up by the crash. All offsets verified by
Python from the recomp `lis -31972 ; addi/lwz` pairs.

```cpp
// .data global: gAudioPoolEngine @ 0x831C0000
// (one master block; observed slots only.)
struct AudioPoolEngine {
    /* +8704 (0x831C2200) */ AudioListener*  listenerRoot;     // sub_8227EE90
    /* +8856 (0x831C2298) */ AudioContainer* container;        // sub_828C21D0
    /* +8868 (0x831C22A4) */ AudioMgr*       mgr;              // sub_828BF270
    /* +10048(0x831C2740) */ uint32_t        slotDescTbl[37];  // 36-entry switch
    /* +10212(0x831C27E4) */ uint32_t        modeStore;        // sub_828C19C0
    /* +11560(0x831C2D28) */ void*           recordHandle;     // sub_828C21D0 stw r3,-16
    /* +11568(0x831C2D30) */ uint32_t        recordParam1;     // sub_828C21D0 stw r31,-8
    /* +11576(0x831C2D38) */ void*           recordParam2;     // <-- CRASH r31 target
    /* +11580(0x831C2D3C) */ uint32_t        recordCounter;    // bumped by 2290
    /* +15833(0x831C3DD9) */ uint8_t         poolFlags;        // sub_828C6568 stb
    /* +15836(0x831C3DDC) */ void*           poolStaging;      // sub_828C64C8/6500
    /* +15840(0x831C3DE0) */ void*           poolCur;          // sub_828C6568/60A0
};
```

The two clusters — `recordXxx @0x831C2D28..3C` (managed by
`21D0/2290/2300`) and `poolXxx @0x831C3DD9..E0` (managed by
`64C8/6500/60A0/6568`) — are clearly two halves of the **same** open/close
RAII pair: `6568` opens, `64C8` inserts, `6500` commits, `60A0` closes; in
parallel `21D0` allocates a contained record, `2290` pushes 36-byte float
samples into it, `2300` releases the handle.

## 4. Callee / caller maps

### Callees
|fn|callees|notes|
|-|-|-|
|sub_828C60A0|none|leaf store-zero|
|sub_828C64C8|sub_828C8F38 (tail)|sub_828C8F38 itself calls sub_828C8DB0, sub_828C8E80, sub_828E02E8 → indirect dispatch via per-class vtable|
|sub_828C6500|sub_827BAEE0|sub_827BAEE0 walks `hdr->count @+28` x `hdr+24[i]` u32s, dispatches each via `sub_828E02E8` (`gAudioPoolEngine.mgr` vtable @+64)|
|sub_828C6568|sub_828C20B0|table read of `slotDescTbl[idx]`|

### Callers (top, deduped — same set for all four)
sub_8227F2E8 (crash-stack), sub_8227F200, sub_8227F458,
sub_821D6D18, sub_82272428, sub_822728E0,
sub_822906F0, sub_82293878,
sub_822BCF20, sub_822BD7F0, sub_822BD978,
sub_822CF300, sub_8231FD20, sub_8233F608, sub_824F56F8 + ~13 more.

The 100% overlap of caller sets (28-29 callers each, all in the same numeric
range) is **strong evidence these four are an ABI-stable lifecycle quartet**:
audio emitter wrappers in `0x8227Fxxx`/`0x822Cxxxx`/`0x822Bxxxx` etc. each
dispatch through the same idiom.

## 5. Mutex / event interactions

|fn|KeWait|NtWait|KeRelease|NtRelease|NtSet|verdict|
|-|-|-|-|-|-|-|
|sub_828C60A0|no|no|no|no|no|**unsynchronised**|
|sub_828C64C8|no|no|no|no|no|**unsynchronised**|
|sub_828C6500|no (sub_827BAEE0 also clean)|no|no|no|no|**unsynchronised**|
|sub_828C6568|no (sub_828C20B0 also clean)|no|no|no|no|**unsynchronised**|

None of the four (nor their direct callees: 8F38, 827BAEE0, 828C20B0, 828E02E8)
take, release, set or wait on a kernel sync primitive. **All RMW on
`gAudioPoolEngine.poolXxx` and `recordXxx` is racy by construction.** There is
no `lwarx/stwcx.` either — these are plain unguarded loads/stores. The whole
subsystem assumes a single audio thread.

## 6. Conclusions / classification

|fn|role|side-effect|safety|
|-|-|-|-|
|**sub_828C60A0**|context **close**|`PoolCtx.cur = 0` (last-record handle release)|leaf, no lock — racy with any concurrent insert|
|**sub_828C64C8**|record **insert / stage**|`PoolCtx.staging = poolBase + idx*32`, then dispatch via sub_828C8F38|tail-call → caller's frame survives, so an exception in 8F38 unwinds straight through|
|**sub_828C6500**|insert **commit / drain**|if `PoolCtx.flags != 0` then walk staged records via sub_827BAEE0; always `staging = 0`|`flags` is u8 — no atomicity even on PPC|
|**sub_828C6568**|context **open / lookup**|sets `PoolCtx.flags`, `PoolCtx.cur`; returns `u16 @cur+12`|reads `0x82B0D1F8` (user-state-like global) without a barrier — this is the `-1 sentinel` pattern that bit USER_STATE elsewhere|

### Lifecycle ordering bug in the crash chain

The crash site is `sub_828C2300+0x34`: the second word it stores (`stw r11,0(r31)`)
is to `gAudioPoolEngine.recordParam2 @ 0x831C2D38`. r31 was clobbered to 0x20 —
**exactly the offset that `sub_828C21D0` writes into the same struct family**.
That means `sub_828C21D0` (record-begin) had its `r31` (sourced from caller r3)
become a small integer rather than a real pointer. Looking at sub_8227F2E8,
`sub_828C2300` is invoked **with no argument set** (r3 left over from previous
`sub_828C2290` call), and `sub_828C2300` doesn't use r3 itself — but
`sub_828BF270` (its callee, and the only path between entry and the trapping
store) does:

```
sub_828BF270:
    lis  r11,-31972
    lwz  r3, 8868(r11)      ; r3 = gAudioPoolEngine.mgr  @0x831C22A4
    b    sub_82A3DF50       ; tail call: r3 + 13428 -> r3 + 48
```

`sub_82A3DF50` reads `mgr->field_13428` (i.e. `*(u32*)(mgr + 0x3474)`) and
writes it to `mgr->field_48`. **If `mgr` is null or freed (RAGE poison
`0xFFE1E1E1` in r9 confirms freed memory in scope), this corrupts the struct
that `sub_828C2300` then resumes assuming its caller-saved r31 frame —
but `sub_828C2300` reloads r31 from `addi r31,r11,11576` first, so r31 is
**recomputed from a constant** and should be safe.**

Therefore the crash is *not* in this group's logic — but it **is consistent
with**:

1. **`sub_828C6568` being called twice without an intervening `sub_828C60A0`**:
   `PoolCtx.cur` is overwritten while sub_828C2290 is still streaming into the
   record — by the time sub_828C2300 runs, `recordHandle @0x831C2D28` (which
   sub_828BF270 indirectly chases) is dangling.
2. **Missing locks** around `gAudioPoolEngine` accesses: a second audio-system
   path (e.g. an emitter teardown on a worker thread) frees `mgr`, leaving the
   `0xFFE1E1E1` poison r9 carries when the trap fires.
3. **`PoolCtx.flags @0x831C3DD9` is a u8** but is touched from multiple
   call-sites; even on a single core the byte-RMW races vs. the engine reading
   `mgr` 0x3474 bytes deep.

### Recommended fix surface

To stop the crash without altering recomp output, a `PPC_FUNC_HOOK` on
`sub_8227F2E8` could:

- guard the four-stage sequence with a per-emitter lock,
- bail out (return early) if `*(u32*)0x831C22A4 (gAudioPoolEngine.mgr)` is null
  or `0xFFE1E1E1`-poisoned,
- assert `*(u32*)0x831C3DE0 == 0` on entry — this exposes the missing
  `sub_828C60A0` from a previous call.

Hooking on `sub_828C2300` itself (returning early when `r31 < 0x80000000`) is
the minimal-intrusion safety net — it costs one branch and prevents the
guest-page write that takes the process down.
