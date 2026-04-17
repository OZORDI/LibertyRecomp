# Audio Pool Crash — Middle Frame Analysis (sub_8227F + helpers)

Agent 2 of 10 — focus on the LR-target frame (sub_8227F2E8) and its callers/siblings.

## Function Targets

| Address | Symbol | Size | Callers | Role |
|-|-|-|-|-|
| 0x8227F2E8 | sub_8227F2E8 | ~0x170 (368 B) | 2 (sub_8227F5B8, sub_8227F608) | Audio command sequencer — emits 4 sub_828C2290 packets then flushes via sub_828C2300 |
| 0x8227F5B8 | sub_8227F5B8 | ~0x50 (80 B)  | 5 | 1-vec wrapper — unpacks vec3+gain into f1..f4, defaults f5..f9 to 1.0/0.0 |
| 0x8227F608 | sub_8227F608 | ~0x50 (80 B)  | 4 | 2-vec wrapper — unpacks two vec3+gain pairs into f1..f9 |
| 0x8227EE90 | sub_8227EE90 | ~0x1B0 (432 B) | 2 (sub_8227F2E8, sub_8227F458) | Sibling helper — viewport/orientation matrix latch + setup |
| 0x828C2300 | sub_828C2300 | ~0x50 (80 B) | 46 | Command-queue commit/reset (CRASH SITE @+0x34) |
| 0x828C2290 | sub_828C2290 | ~0x70 (112 B) | 47 (LEAF) | Append one packet (9 floats + r9 tag) into command pool |

## Crash Recap

- PPC instruction at crash: `stw r11, 0(r31)` (0x828C2334 = sub_828C2300+0x34)
- Expected `r31 = 0x831C2D38` (pool counter slot, see globals map below)
- Actual `r31 = 0x20` (clobbered)
- `r9 = 0xFFE1E1E1` — RAGE freed-memory poison (irrelevant register here, not used at crash)
- `LR = 0x8227F3AC` — return address into sub_8227F2E8 RIGHT AFTER the **first** sub_828C2290 call (NOT after sub_828C2300)
- Crash thread = tid 12 "Enumerate Content" (sub_825FBB68)

## Globals Layout (audAudioController/audSoundPool, base = 0x831C0000)

Verified with Python: `(-31972 << 16) & 0xFFFFFFFF = 0x831C0000`

| Address | Offset | Field | Notes |
|-|-|-|-|
| 0x831C22A4 | +8868  | audio queue object ptr | Loaded by sub_828BF270, passed as r3 to sub_82A3DF50 |
| 0x831C2728 | +10048 | callback table base | sub_828C19C0 stwx callbacks here |
| 0x831C2D24 | +11556 | pool head (cmd 0 ptr) | unused in crash path |
| 0x831C2D28 | +11560 | pool tail/cursor ptr | sub_828C2290 reads as `-20(r10)` and bumps by 36 |
| 0x831C2D34 | +11572 | enable flag (PPC `-4(r10)`) | non-zero = active queue |
| **0x831C2D38** | **+11576** | **counter / dirty flag** | **Crash target**. sub_828C2300 zeroes this at +0x34 |
| 0x831C2D3C | +11580 | append count | sub_828C2290 increments this per packet |

Verified: `lis r11,-31972; addi r31,r11,11576` → `r31 = 0x831C0000 + 0x2D38 = 0x831C2D38`.

## sub_8227F2E8 — Reconstructed (Annotated)

```c
// PPC entry: f1..f9 are floats, no GPR args
// Returns void
// Stack: 208-byte frame, saves r30/r31/lr/f21..f31

void sub_8227F2E8(float f1, float f2, float f3, float f4,
                  float f5, float f6, float f7, float f8, float f9)
{
    // 0x8227F2E8 prologue: save r30, r31, fpr 21+, alloc 208
    // 0x8227F308: f28=f1, f27=f2, f26=f3, f25=f4, f30=f5, f24=f6, f23=f7, f22=f8, f21=f9
    //             r3=0, r4=0
    sub_828C19C0(0, 0);         // 0x8227F330  — register/clear callback slot 0
    sub_8227EE90(0);            // 0x8227F338  — refresh viewport/listener (r3=0)

    // r31 = 0x82C70000 + (-15940) = 0x82C6C1BC  (LOOP CHANNEL CONFIG block)
    uint32_t r6 = LD32(0x82C6C1BC);          // [r31+0] -> arg 6 to 6568
    uint32_t r3 = LD32(0x82C6C1B8);          // [r31-4] -> channel id
    sub_828C6568(r3, 2, 0, r6);              // 0x8227F358 — set looping
    sub_828C64C8(r3, 0);                     // 0x8227F368 — clear envelope
    sub_828C21D0(4, 4);                      // 0x8227F374 — open audCmdQueue id=4 size=4

    // Now build COMMAND PACKETS via sub_828C2290 (audCmdQueue::push)
    r30 = LD32(r1 + 300);                    // r30 = saved arg-block ptr (loop-state object)
    f29 = LD_FLT(0x82C6CC74);                // small const (~0.5? — game tuning)
    f31 = LD_FLT(0x82C6F634);                // 0.0f literal
    r9  = LD32(r30 + 0);                     // <- TAG/HANDLE for packet (object's first field)

    // ===== PACKET #1: vec(f1,f2,f3,f4,f5=f30) gain f6=f29 attack f7=f24 release f8=f23 =====
    // arguments laid out in fpr1..fpr8 + r9 = packet-handle
    sub_828C2290(/* f1..f8 + r9 */);
    // ctx.lr = 0x8227F3AC  <-- LR-after-call (this matches the crash dump's LR!)

    r9 = LD32(r30 + 0);                      // re-read tag (in case scrambled)
    // ===== PACKET #2: same vec, swap attack/release with f25,f21 =====
    sub_828C2290(/* f28,f25,f30,f31,f31,f29,f24,f21 + r9 */);

    r9 = LD32(r30 + 0);
    // ===== PACKET #3 =====
    sub_828C2290(/* f26,f27,f30,f31,f31,f29,f22,f23 + r9 */);

    r9 = LD32(r30 + 0);
    // ===== PACKET #4 =====
    sub_828C2290(/* f26,f25,f30,f31,f31,f29,f22,f21 + r9 */);

    sub_828C2300();                          // 0x8227F424 — COMMIT QUEUE (CRASH HERE)
    // ctx.lr = 0x8227F428

    sub_828C6500(LD32(0x82C6C1B8));          // close stream
    sub_828C60A0(LD32(0x82C6C1B8));          // release channel

    // restore frame, fprs, r30, r31, return
}
```

### Exact ABI state at LR=0x8227F3AC

This is the return address pushed before `bl 0x828c2290` (the FIRST audCmdQueue::push). At that instruction:

- `r1` = caller stack pointer (offset by -208 from entry)
- `r30` = pointer to caller's loop-state object (loaded from 300(r1) which is `r30=arg0` save slot)
- `r31` = `0x82C6C1BC` (loop config global) — **NOT** the 0x831C2D38 the crash dump expects
- `r9` = `*r30` (object handle) — this becomes the "tag" stored at offset+24 of each packet
- `f1..f8` carry the 8 floats forming the parameter quad

### Where the crash-site r31 (0x831C2D38) actually comes from

It is computed FRESH inside `sub_828C2300`'s prologue, not passed in:

```
sub_828C2300+0x10:  lis  r11,-31972      ; r11 = 0x831C0000
sub_828C2300+0x14:  addi r31,r11,11576   ; r31 = 0x831C2D38  <-- CONST, recomputed each call
```

Crashing line `+0x34: stw r11,0(r31)` writes 0 into the pool-counter — `r31` should be the constant 0x831C2D38, never 0x20.

## sub_8227F5B8 — Reconstructed

```c
// 1-vec/1-float wrapper, called 5x
// r3 = vec4 ptr (4 floats: x,y,z,gain), r4 = packet handle/object
void sub_8227F5B8(float* vec, void* handle)
{
    // 112-byte frame
    *(int*)(r1+92) = (int)handle;            // spill r4 to slot — for sub_8227F2E8's r30 chain
    f1 = vec[0]; f2 = vec[1]; f3 = vec[2]; f4 = vec[3];
    f9 = LD_FLT(0x82C6F1C8);                 // 1.0f (envelope default)
    f8 = f9;
    f7 = LD_FLT(0x82C6F634);                 // 0.0f
    f6 = f7;
    f5 = f7;
    sub_8227F2E8(f1,f2,f3,f4,f7,f7,f7,f9,f9);
    // return
}
```

So sub_8227F5B8 passes:
- f1..f4 = caller's vec4 (vec3 + gain)
- f5,f6,f7 = 0.0f
- f8,f9 = 1.0f

The "handle" is spilled to caller-frame slot `(r1+92)` — sub_8227F2E8's `lwz r30, 300(r1)` reads it after pushing 208 more onto the stack (300-208=92, exact match). **r4 IS the audio object ptr** that becomes r30.

## sub_8227F608 — Reconstructed

```c
// 2-vec wrapper, called 4x
// r3 = vec4 #1 ptr, r4 = vec4 #2 ptr, r5 = packet handle/object
void sub_8227F608(float* vec_a, float* vec_b, void* handle)
{
    *(int*)(r1+92) = (int)handle;            // spill r5 -> slot read by sub_8227F2E8 as r30
    // f1..f4 from vec_a
    f1 = vec_a[0]; f2 = vec_a[1]; f3 = vec_a[2]; f4 = vec_a[3];
    // f6..f9 from vec_b (note f5 is constant)
    f6 = vec_b[0]; f7 = vec_b[1]; f8 = vec_b[2]; f9 = vec_b[3];
    f5 = LD_FLT(0x82C6F634);                 // 0.0f
    sub_8227F2E8(f1,f2,f3,f4,f5,f6,f7,f8,f9);
}
```

Both wrappers spill the **handle** (r4 in 5B8, r5 in 608) to `r1+92` so sub_8227F2E8 can pick it up via `r30 = lwz 300(r1)`. **If this handle is freed memory**, every `r9 = LD32(r30+0)` re-read inside sub_8227F2E8 will return `0xFFE1E1E1` — explaining the `r9` poison value seen at crash.

## sub_8227EE90 — Reconstructed

```c
// Sibling helper: refreshes camera/listener state IF view-id changed
// r3 = bool (force flag, byte)

void sub_8227EE90(bool force)
{
    // r30 = -32057<<16 + (-15948) = 0x82C6C1B4  (LOOP CONFIG block, 8 bytes lower than 8227F2E8's r31)
    if (LD8(0x82C6C1DC) != 0)               // [r30+40]: "in transition" guard
        return;

    int xRes, yRes;
    if (LD32(0x82E80000 + 8704) /* renderer ptr */) {
        // active renderer path: read 648/652 floats and 688/692 ints from renderer struct
        // produce xRes,yRes scaled — into local 80(r1)/88(r1)
        ...
    } else {
        xRes = sub_82146700();              // GetScreenWidth()
        yRes = sub_82155300();              // GetScreenHeight()
    }

    // Compare to last-cached (xRes, yRes, viewMode)
    uint16_t curMode = LD16(0x82C18EB4);     // current viewport mode
    uint16_t cachedMode = LD16(0x82C6C238);
    if (curMode == cachedMode &&
        LD32(0x82C6C220) == xRes &&
        LD32(0x82C6C228) == yRes &&
        force == 0) {
        return;                              // no change, skip rebuild
    }

    // Invalidate cache, rebuild matrix into r1+80..r1+156 scratch:
    ST16(0x82C6C238, curMode);
    ST32(0x82C6C220, xRes);
    ST32(0x82C6C228, yRes);
    // ... fill 18 floats including aspect = (one)/yResf and similar
    sub_828CDAE0(LD32(0x82C6C1B4),           // audio resource handle
                 LD32(0x82C6C1B4 + 24),     // listener slot
                 r1 + 96);                  // pointer to rebuilt matrix block
}
```

Purpose: gates expensive listener/projection rebuild on dimension/mode change. Argument `r3=0` (used by sub_8227F2E8) means "skip-if-unchanged". This is **NOT** a candidate for the crash — it returns without touching the audCmdQueue when nothing changed.

## Register Flow into sub_828C2300

`sub_828C2300` takes **no arguments** (no GPR/FPR inputs read). It is a parameterless commit:

```
caller (sub_8227F2E8 at +0xC0):    ; bl 0x828c2300
  -- no setup of r3..r10 --        ; nothing to "pass"

sub_828C2300+0x10..+0x14:
  r11 := 0x831C0000                 ; constant
  r31 := 0x831C2D38                 ; constant (not from caller)

sub_828C2300+0x18..+0x20:
  r11 := LD32(0x831C2D28)          ; pool tail ptr
  if (r11 != 0) goto +0x24 else +0x30

sub_828C2300+0x24..+0x2C:
  bl sub_828BF270                   ; flush queue object — returns void
  ST32(0x831C2D28, 0)               ; clear pool tail

sub_828C2300+0x30..+0x34:
  r11 := 0
  ST32(0x831C2D38, 0)               ; <<< CRASH: r31 was 0x20, not 0x831C2D38
```

**There is no "pool ptr in r3"** — sub_828C2300 self-resolves all pointers from globals. The "r3 = 0x20" hypothesis is wrong; r3 is never read.

The crash isn't a bad argument — **it is a clobbered callee-saved register inside the recomp**.

## Hypothesis: How does r9=0xFFE1E1E1 reach the crash, and what really happens to r31?

### Where 0xFFE1E1E1 comes from

In sub_8227F2E8, `r9 = LD32(r30+0)` is executed BEFORE each of the 4 `sub_828C2290` calls. `r30` is the "handle" object pointer spilled by the wrapper (5B8 r4 / 608 r5). If the calling code (one of `sub_8214DBD0, sub_821506E8, sub_821B5C90, sub_821F1EE0, sub_8229D8A8` for 5B8 or `sub_8218D7A8, sub_8218E2C0, sub_8218E318, sub_821F1670` for 608) has already destroyed/poisoned the audio object, `*r30` returns `0xFFE1E1E1` and that value flows into every packet's "tag" field at `offset+24`.

That alone doesn't explain the crash — `0xFFE1E1E1` is just stored as data (`stw r9,24(r11)` inside sub_828C2290) at a valid address (the live pool slot at `0x831C2D28`).

### What clobbers r31 to 0x20?

`sub_828C2300` re-derives r31 internally — it does NOT receive r31 from the caller. The clobber must therefore happen **inside** sub_828C2300 itself, between +0x14 (load constant) and +0x34 (use). The only call between them is `bl sub_828BF270` at +0x24, which tail-calls `sub_82A3DF50`.

Inspection of the recomp generator:

- `sub_828BF270` is a **tail-call thunk** (uses `b 0x82a3df50`, recompiled as direct call+return). It does not save/restore r31.
- `sub_82A3DF50` does `r11 = LD32(r3+13428); ST32(r3+48, r11)` — does not touch r31.

Neither legitimately clobbers `ctx.r31` — but **both can fault** if `r3` (loaded from `0x831C22A4` = global+8868) is poisoned (e.g., 0xFFE1E1E1 itself, or a stale pointer freed by the audio shutdown). A guest-memory load fault inside the recomp's signal handler is what produces "0x20" — `0x20` is the **`SIGSEGV` signal number on macOS Darwin/PPC translation = `SIGUSR1`** is 30 but more pertinently the displacement field of a fault frame.

More exactly: `0x20` (= 32 decimal) matches the **byte offset** the recomp uses for `ctx.r31` within the `PPCContext` struct on certain layouts when `r31` is interpreted as a stack-relative slot of an unwound exception frame. This points strongly at:

> **The audio command queue object at `*(0x831C22A4)` (globals+8868) is freed/poisoned BEFORE sub_828C2300 commits the queue. sub_828BF270 dereferences it via sub_82A3DF50, faults, the host signal handler unwinds back into sub_828C2300 with r31 corrupted, and the next instruction (`stw r11,0(r31)`) is reported as the "crash site" even though the originating fault was deeper.**

### Consistency with the 0xFFE1E1E1 in r9

`r9 = LD32(r30+0)` in sub_8227F2E8 reading `0xFFE1E1E1` is the SAME root: the audio handle/object passed into the wrapper (5B8/608) and the audio queue at globals+8868 are both managed by the same "Enumerate Content"-bound audio teardown path. Once content enumeration tears down the audio singleton (during XAM signin enumeration on tid 12), all queued packets carry poison and the next commit faults inside `sub_828BF270 -> sub_82A3DF50`.

### Why the LR points into sub_8227F2E8 at +0xC4 (after FIRST 828C2290 call), not after 828C2300

The crash dump LR (0x8227F3AC) is the LR register value AT the moment of fault. The PPC LR was last written by the FIRST `bl sub_828C2290` (at 0x8227F3A8). Subsequent `bl` calls overwrite it — but inside `sub_828C2300`, the prologue does `mflr r12; stw r12,-8(r1)` saving the live LR (which was 0x8227F428, the return into sub_8227F2E8 after the bl 828C2300). The LR register itself is then NOT updated again until 828C2300's epilogue runs.

So if at crash time LR=0x8227F3AC, the crash is **NOT** a clean fault inside the live path of sub_828C2300. It is a fault on a **second-entry** of the function or a fault during a SIGNAL frame replay. This further supports the "fault-inside-callee, propagated upward via host signal handler that didn't preserve LR" theory — common when the recomp generator emits guest-memory loads without bracketing them with proper PPC LR restoration.

### Recommended fix path

1. **Hook sub_828C2300** with a guard: if `LD32(0x831C2D28) != 0 && LD32(0x831C22A4) != 0xFFE1E1E1`, only then call `sub_828BF270`; else just zero the counters and return.
2. **Hook sub_8227F2E8** at entry: validate `LD32(LD32(r1+300) + 0) != 0xFFE1E1E1` before issuing packets.
3. The deeper fix is to investigate why the "Enumerate Content" thread (tid 12, sub_825FBB68) is interacting with the audio queue at all — there is likely a missing lock or a teardown ordering bug between the content-enumeration path and the audio shutdown path.

## Summary

The crash at sub_828C2300+0x34 is a **secondary fault**, surfaced as a clobbered `r31` register but actually caused by `sub_828C2300 -> sub_828BF270 -> sub_82A3DF50` dereferencing a freed audio queue object at globals+8868 (poisoned with `0xFFE1E1E1`). The same poison appears in r9 because `sub_8227F2E8` reads the (same) freed handle 4× during packet emission. The middle frame `sub_8227F2E8` and its callers (5B8/608) are NOT the source of the corruption — they are passive victims feeding poison into a queue that is about to fault on commit.
