# Audio-pool crash — Inner crash trio

Agent 1/10 deliverable. Self-contained analysis of the three innermost functions
on the crashing call stack.

## Header

| function | va | size | callers | leaf | hot | hooked |
|-|-|-|-|-|-|-|
| `sub_828C2300` | `0x828C2300` | `0x50` (80 B) | 46 | no | yes (top-500) | no |
| `sub_828BF270` | `0x828BF270` | `0x10` (16 B) | 2 | yes | no | no |
| `sub_828C2290` | `0x828C2290` | `0x70` (112 B) | 267 | yes | yes (top-500) | no |

All three live in the same `gta4_recomp.66.cpp` translation unit and share the
same `0x831C0000`-based global block at `+0x2D2X`.

The crash:

- PC = `sub_828C2300+0x34` = `0x828C2334` (instruction `stw r11, 0(r31)`)
- r31 expected = `0x831C2D38` (built two instructions earlier from `lis/addi`)
- r31 actual = `0x20` (clobbered, not even page-mapped)
- r9 = `0xFFE1E1E1` (RAGE freed-memory poison — a freed object reached the call)
- LR = `0x8227F3AC` (`sub_8227F2E8 + 0xC4`, just past `bl 0x828c2290`)
- Thread tid 12 "Enumerate Content" (sub_825FBB68)

## What this function family does (high-level)

This is the **immediate-mode primitive batcher** sitting in front of the small-block
"scratch" allocator. The global at `0x831C0000 + 11576` (`0x831C2D38` and
adjacent) is the one-and-only batch state slot. There is no per-thread copy; the
whole batcher is a singleton.

```
sub_828C21D0(primType, vertCount)   ==> Begin()       writes -16/-8/+0/+4 of the slot
sub_828C2290(f1..f8, r9)            ==> EmitVertex()  reads -4/-20/+0 to advance
sub_828C2300()                      ==> End()         flushes via 828BF270, zeros slot
sub_828BF270()                      ==> commit/reset of the upstream block-pool
sub_82A3DF50(p)                     ==> *(p+48) = *(p+13428)  (pool top reset)
```

`sub_8227F2E8` on the crashing thread is the LR donor — its body issues the
canonical sequence `21D0 (begin) -> 2290 x4 (emit 4 verts) -> 2300 (end)`.
The crash happens **inside that End()**.

---

## 1. `sub_828C2300` — End() on the immediate-mode batcher

Real recomp scaffold (annotated):

```cpp
void sub_828C2300() {
    // standard prologue:
    //   mflr r12; stw r12,-8(r1); std r31,-16(r1); stwu r1,-96(r1)
    // Caller-saved r31 lives at OLD r1 - 16  (i.e. NEW r1 + 80).

    // Materialise &g_immBatchSlot:
    //   lis  r11, -31972      ; r11 = 0x831C0000
    //   addi r31, r11, 11576  ; r31 = 0x831C2D38   <-- the singleton slot
    auto* slot = (ImmBatchSlot*)(GUEST_BASE + 0x831C2D38);

    // if (slot->buffer != nullptr) { commit(); slot->buffer = nullptr; }
    if (slot->buffer != nullptr) {            // lwz r11,-16(r31) ; cmplwi cr6,r11,0 ; beq cr6,...
        sub_828BF270();                       // bl 0x828BF270  (commit pool tx)
        slot->buffer = nullptr;               // li r11,0 ; stw r11,-16(r31)
    }

    // Always zero the count field even on the no-buffer path:
    slot->vertCountOrFlag = 0;                // li r11,0 ; stw r11,0(r31)   <-- CRASH (+0x34)

    // epilogue:
    //   addi r1,r1,96 ; lwz r12,-8(r1) ; mtlr r12 ; ld r31,-16(r1) ; blr
}
```

Register trace:

| step | op | reg | value | source |
|-|-|-|-|-|
| +0x00 | mflr   | r12 | LR-in (`0x8227F3AC`) | LR |
| +0x04 | stw    | mem | r12 -> `oldR1-8` | save LR |
| +0x08 | std    | mem | r31 -> `oldR1-16` (caller's r31 lives here) | save r31 |
| +0x0C | stwu   | r1  | newR1 = oldR1 - 96 | open frame |
| +0x10 | lis    | r11 | `0x831C0000` | sign-ext imm |
| +0x14 | addi   | r31 | `0x831C2D38` | + 11576 |
| +0x18 | lwz    | r11 | `*(slot->buffer)` from `0x831C2D28` | branch test |
| +0x24 | bl     | --- | call `sub_828BF270` if buffer != null | --- |
| +0x2C | stw    | mem | 0 -> `slot->buffer` (`0x831C2D28`) | clear |
| +0x30 | li     | r11 | 0 | preload zero |
| +0x34 | **stw**| **mem** | **0 -> `0(r31) = 0x831C2D38`** | **CRASH** |
| +0x38 | addi   | r1  | r1 + 96 | close frame |
| +0x44 | ld     | r31 | `*(newR1-16) == *(oldR1-16)` | restore caller's r31 |

The crash store uses `r31`. Inside this function r31 is set **once** by the
`lis/addi` pair at +0x10/+0x14, and is not touched again before +0x34. So if
r31 is `0x20` at +0x34, **the recomp's `ctx.r31` was overwritten between +0x14
and +0x34**. The only window for that is the `bl sub_828BF270` at +0x24.

### Pool object struct layout

The "pool" `sub_828C2300` operates on is the singleton `ImmBatchSlot` at
`0x831C2D38`. By correlating `sub_828C21D0` (writer) and `sub_828C2290`
(consumer), the layout is unambiguous:

```cpp
// VA 0x831C2D28  (== global @ base+11576-16)
struct ImmBatchSlot {
    void*    buffer;        // +0x00  (slot-relative -16)  vertex buffer ptr from sub_828BF248
    void*    primContext;   // +0x04  (slot-relative -12)  unused / pad seen as 0
    uint32_t primType;      // +0x08  (slot-relative  -8)  r3 of Begin() (e.g. 4 = trilist)
    uint32_t pad0C;         // +0x0C  (slot-relative  -4)  not written by Begin
    uint32_t vertCount;     // +0x10  (slot-relative   0)  r4 of Begin(); cleared in End()
    uint32_t writeCursor;   // +0x14  (slot-relative  +4)  zeroed in Begin (10 ; stw r10,4(r11))
};
// sub_828C2300's r31  -> &ImmBatchSlot.vertCount    (i.e. 0x831C2D38)
// sub_828C2290's r10  -> &ImmBatchSlot.writeCursor  (i.e. 0x831C2D3C)
//   r10 - 4   == &vertCount   (read as gate flag)
//   r10 - 20  == &buffer      (current write pointer; advanced by 36 bytes/vertex)
//   r10 +  0  == &writeCursor
```

The "vertex" written by sub_828C2290 is a **36-byte struct** (8 floats + 1 u32),
written through `slot->buffer` and the buffer is bumped by 36 each emit.

---

## 2. `sub_828BF270` — commit small-block pool transaction

Real recomp scaffold (full body — only 4 instructions):

```cpp
void sub_828BF270() {
    // lis  r11, -31972      ; r11 = 0x831C0000
    // lwz  r3,  8868(r11)   ; r3  = *(uint32_t*)0x831C22A4   (g_smallBlockPool)
    // b    sub_82A3DF50     ; tail-call (NOT a bl)
    void* pool = *(void**)(GUEST_BASE + 0x831C22A4);
    return sub_82A3DF50(pool);   // tail call — preserves caller LR
}

void sub_82A3DF50(void* pool) {
    // lwz r11, 13428(r3)   ; r11 = *(uint32_t*)(pool + 0x3474)
    // stw r11, 48(r3)      ; *(uint32_t*)(pool + 0x30) = r11
    // blr
    *((uint32_t*)pool + 12) = *((uint32_t*)pool + 0xD1D);   // pool->topPtr = pool->savedTop
}
```

This is a **transaction commit / reset** on a bump allocator: it copies a saved
top-pointer (at byte offset `+0x3474`) into the live top-pointer (at byte
offset `+0x30`). It allocates no memory itself.

### Stack-frame analysis (the load-bearing question)

`sub_828BF270`:

- No `mflr`, no `stwu`, no `std`/`stw` to anything via r1.
- Touches r11 and r3 only.
- **Last instruction is `b`, not `bl`** — tail call. The PPC ABI says the
  caller's LR/red-zone is preserved through a tail call exactly as if the
  callee had returned itself.

`sub_82A3DF50`:

- No frame either. Two memory ops, both addressed off r3 (the pool pointer in
  the small-block heap), not off r1.

| fn | uses r1? | writes through r1? | red-zone risk? |
|-|-|-|-|
| sub_828BF270 | no | no | none |
| sub_82A3DF50 | no | no | none |

The Xenon ABI gives a 224-byte negative red-zone below r1 that leaf functions
may use without reserving a frame. Even using the maximum red-zone, a leaf
**writes below `r1`**, never above it — and `sub_828C2300`'s saved-r31 sits at
`oldR1 - 16` which is **at the top of its own frame's negative-offset save area**,
ABOVE the new r1 by exactly 80 bytes. Specifically: `oldR1 - 16 == newR1 + 80`.
Nothing inside `sub_828BF270` or `sub_82A3DF50` is capable of touching that
slot via r1, because neither one ever reads or writes through r1.

### Memory access table — sub_828BF270

| op | addr expression | size | direction |
|-|-|-|-|
| lwz r3 | `0x831C0000 + 8868` = `0x831C22A4` | 4 | load (g_smallBlockPool) |

(Then tail-calls sub_82A3DF50, whose ops are: `lwz pool+0x3474`, `stw pool+0x30`.)

---

## 3. `sub_828C2290` — EmitVertex() (the innocent-but-PC-reported leaf)

Real recomp scaffold (annotated):

```cpp
void sub_828C2290(float f1, float f2, float f3, float f4,
                  float f5, float f6, float f7, float f8,
                  uint32_t r9 /* packed colour or vertex flags */) {
    // r10 = &slot.writeCursor  (0x831C2D3C)
    auto* cursor = (ImmBatchSlot*)(GUEST_BASE + 0x831C2D38); // -4 to land on vertCount

    uint32_t vertCount = cursor->vertCount;       // lwz r11,-4(r10)   == load slot+0x10
    uint8_t* writePtr  = (uint8_t*)cursor->buffer; // lwz r11,-20(r10) == load slot+0x00

    // If (vertCount == 0) || (writePtr != nullptr): bump cursor, no write.
    // Otherwise (vertCount != 0 && writePtr == nullptr): emit vertex, bump cursor.
    //
    // Reading the recomp literally:
    //   if (vertCount == 0) goto emit;     (cmpwi cr6,r11,0  beq -> emit)
    //   if (writePtr  != 0) goto emit;     (cmplwi cr6,r11,0 bne -> emit)
    //   /* fall-through: nothing to write, just bump writeCursor and return */
    //   cursor->writeCursor++; return;

    if (vertCount != 0 && writePtr == nullptr) {
        // The "fast skip" branch: only the cursor counter advances.
        cursor->writeCursor += 1;
        return;
    }

    // Emit a 36-byte vertex through writePtr (which is non-null here):
    //   stfs f1, 0(r11)   stfs f2, 4(r11)   stfs f3, 8(r11)
    //   stfs f4,12(r11)   stfs f5,16(r11)   stfs f6,20(r11)
    //   stw  r9,24(r11)
    //   stfs f7,28(r11)   stfs f8,32(r11)
    Vertex36* v = (Vertex36*)writePtr;
    v->x = f1;  v->y = f2;  v->z = f3;
    v->nx = f4; v->ny = f5; v->nz = f6;
    v->packedColour = r9;
    v->u = f7;  v->v = f8;

    // Advance buffer ptr by 36 bytes:
    //   addi r8,r11,36   ;  stw r8,-20(r10)
    cursor->buffer = (uint8_t*)cursor->buffer + 36;

    // Bump global writeCursor:
    //   lwz r8,0(r10) ; addi r8,r8,1 ; stw r8,0(r10)
    cursor->writeCursor += 1;
}
```

It is a leaf, has **no stack frame at all**, never touches r1, never modifies
r31, and never modifies LR. The "PC reported here" hit is purely the
symbolicator's nearest-function rounding — the actual crashing instruction is
inside `sub_828C2300` (next function up at +0x70 bytes), not here.

### Memory access table — sub_828C2290 (slow-path / emit case)

| op | addr expression | size | direction |
|-|-|-|-|
| lwz r11 | `slot+0x10` (`0x831C2D38`) | 4 | load vertCount |
| lwz r11 | `slot+0x00` (`0x831C2D28`) | 4 | load buffer |
| stfs f1..f8 | `buffer + {0,4,8,12,16,20,28,32}` | 4 ea | store |
| stw r9     | `buffer + 24` | 4 | store packed colour |
| stw r8     | `slot + 0x00` | 4 | store advanced buffer |
| lwz r8     | `slot + 0x14` | 4 | load writeCursor |
| stw r8     | `slot + 0x14` | 4 | store cursor+1 |

**Crash relevance**: the load of `slot->buffer` could explode if `r9 ==
0xFFE1E1E1` was the previous emitter's freed colour and `slot->buffer` was
freed/reused — **but the registered crash is at sub_828C2300+0x34, not in
sub_828C2290**. EmitVertex would only fault on the `stfs` stores against a
freed `buffer`. So: r9=0xFFE1E1E1 means the **caller** (sub_8227F2E8 or what
called *it*) was operating on freed state and fed poison into the batch.

---

## Stack-frame analysis: sub_828BF270's effect on caller r31

Recall sub_828C2300's prologue:

```
0x828C2300  mflr  r12             ; r12 = caller LR
0x828C2304  stw   r12, -8(r1)     ; *(oldR1 - 8)  = caller LR
0x828C2308  std   r31, -16(r1)    ; *(oldR1 - 16) = caller r31  (8 bytes, std)
0x828C230C  stwu  r1, -96(r1)     ; newR1 = oldR1 - 96
```

So the caller's r31 lives at `oldR1 - 16`, equivalently `newR1 + 80`. The
restore at +0x44 is `ld r31, -16(r1)` AFTER `addi r1,r1,96` — that's
`ld r31, oldR1 - 16`, i.e. it reloads the same slot.

Within `sub_828C2300`, r31 is reassigned to `0x831C2D38` at +0x14. From that
moment until the epilogue, the **register** r31 is the singleton slot pointer.
The caller's value is only on the stack.

`sub_828BF270` and its tail-callee `sub_82A3DF50`:

- Do not allocate a stack frame.
- Do not write through r1 (no `stw _, _(r1)`, no `std _, _(r1)`, no `stwx`
  through r1).
- Do not write through any register that points into r1's frame.

**Verdict: sub_828BF270 is INCAPABLE of clobbering its caller's saved r31**,
because that slot is at `oldR1 - 16` — accessible only via r1, and neither
sub_828BF270 nor sub_82A3DF50 ever touch r1.

There is one subtlety the recomp introduces: in the recompiled translation,
`ctx.r31` is a member of a per-thread `PPCContext` struct, not an actual
machine register. Whether it "survives" a `bl` through to the next reference
depends on what the recompiler's PPC_FUNC_PROLOGUE / call-thunk does. In the
generated code for `sub_828C2300`:

```cpp
ctx.r31.s64 = ctx.r11.s64 + 11576;        // ctx.r31 = 0x831C2D38
// ...
ctx.r11.u64 = PPC_LOAD_U32(ctx.r31.u32 + -16);  // OK — ctx.r31 used here
// ...
ctx.lr = 0x828C2328;
sub_828BF270(ctx, base);                  // <-- ctx is passed BY REFERENCE
// li r11,0
ctx.r11.s64 = 0;
PPC_STORE_U32(ctx.r31.u32 + -16, ctx.r11.u32);  // uses ctx.r31 again
// li r11,0   ;loc_828C2330
ctx.r11.s64 = 0;
PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);    // CRASH: ctx.r31 == 0x20
```

`sub_828BF270` is invoked with the same `ctx&`. Its body reads:

```cpp
PPC_FUNC_IMPL(__imp__sub_828BF270) {
    PPC_FUNC_PROLOGUE();
    ctx.r11.s64 = -2095316992;                         // 0x831C0000
    ctx.r3.u64  = PPC_LOAD_U32(ctx.r11.u32 + 8868);    // load g_smallBlockPool
    sub_82A3DF50(ctx, base);                           // tail call
    return;
}
```

`sub_828BF270` clobbers `ctx.r11` and `ctx.r3`. **It does not touch `ctx.r31`.**
`sub_82A3DF50` likewise touches only `ctx.r11`. So in the recompiled code path,
the value of `ctx.r31` going into the post-call `PPC_STORE_U32` should remain
`0x831C2D38`, the same value set at +0x14.

If at the actual runtime crash `ctx.r31` is `0x20`, that is **not** the work of
sub_828BF270 itself. Possible causes (for follow-up agents):

1. The crash was caught by the symbolicator one or two instructions late, and
   the *true* faulting instruction is not at `+0x34` but inside the deeper
   commit chain (a faulty pool pointer at `0x831C22A4`).
2. The `ctx` object itself was relocated / reallocated mid-call (per-thread
   context torn down by another thread).
3. r31 was clobbered by a call **further up** the recursion — sub_8227F2E8
   correctly preserves r30/r31 in its own prologue/epilogue, so corruption
   would have to come from a still-earlier ancestor that mishandles the std/ld
   pair (or from the host's signal frame mis-decoding the GPRs).

The `r9 = 0xFFE1E1E1` poison is a much stronger signal: **r9 is the packed
colour passed into sub_828C2290**, sourced from `lwz r9, 0(r30)` in
sub_8227F2E8 where `r30 = *(uint32_t*)(stack+300)` (a content-enumeration
result list pointer). The list element it dereferenced has been freed. So the
upstream defect is: enumerate-content thread is reading a freed item descriptor
and feeding poison into the immediate-mode batcher.

## Conclusion

> **Is `sub_828BF270` capable of clobbering its caller's saved r31?**

**No.** Concrete evidence:

1. `sub_828BF270` is a 4-instruction leaf-tail-call: `lis r11; lwz r3; b
   sub_82A3DF50`. It has no `mflr`, no `stwu`, no `std`/`stw`/`stbx`/etc. via r1.
2. Tail-callee `sub_82A3DF50` is also frame-less and touches only `r3`/`r11`,
   addressing memory only off the small-block pool pointer in `r3`.
3. Caller `sub_828C2300` saves r31 at `oldR1 - 16`, accessible only via r1.
   Neither callee touches r1.
4. In the recompiled implementation, `sub_828BF270` mutates `ctx.r11` and
   `ctx.r3` only; `ctx.r31` is left untouched across the call.

The actual crash mechanism is therefore **not** in this trio. The trio's role
is: sub_828C2300 is the End() of a singleton immediate-mode batcher that was
fed a freed object (poisoned r9 = 0xFFE1E1E1) by the Enumerate Content thread.
The downstream misbehaviour (r31 reading as `0x20`) is an artefact of either
late symbolication, a corrupted PPCContext, or a much earlier callee on the
same thread mishandling its red-zone — not of sub_828BF270.

Recommended follow-up:

- Verify what writes to `0x831C22A4` (the small-block pool pointer that
  sub_828BF270 dereferences) — if this is null/garbage on the Enumerate
  Content thread, sub_82A3DF50's `lwz r11, 13428(r3)` would itself fault, and
  the symbolicator may have rounded back to the End() store.
- Inspect sub_8227F2E8's `r30 = *(stack+300)` source: this is a content
  descriptor pointer fetched after `sub_8227EE90` and survives several
  intermediate calls. If `*(r30)` is `0xFFE1E1E1` poison, the descriptor was
  freed by another thread between fetch and use.
- Check whether the immediate-mode batcher singleton at `0x831C2D38` has any
  thread-affinity guard. The Enumerate Content thread (tid 12) reusing the same
  global as the render thread is a clear races-with-rendering hazard.
