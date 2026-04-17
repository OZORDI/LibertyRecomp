# 07 - Stack-Corruption Theory Analysis

**Verdict: REFUTED (as stated), but with partial root-cause alignment.**

The revert-diff-analysis theory that the `[VD-FIX]` hook corrupted the guest PPC
stack via `CreateVertexDeclarationWithoutAddRef` mutating padding bytes does
**not** hold up under code inspection. The mutation is in-bounds and touches
only reserved bytes that the guest never reads. However, a related but narrower
corruption mode is still possible.

---

## 1. What `CreateVertexDeclarationWithoutAddRef` actually mutates

From `LibertyRecomp/gpu/video.cpp:6054-6066`:

```cpp
static GuestVertexDeclaration* CreateVertexDeclarationWithoutAddRef(
    GuestVertexElement* vertexElements)
{
    size_t vertexElementCount = 0;
    auto vertexElement = vertexElements;

    while (vertexElement->stream != 0xFF
           && vertexElement->type != D3DDECLTYPE_UNUSED)
    {
        vertexElement->padding = 0;   // <-- 1 byte per element
        ++vertexElement;
        ++vertexElementCount;
    }
    vertexElement->padding = 0;       // <-- +1 byte on terminator
    ...
}
```

`GuestVertexElement` (video.h:391) is 12 bytes. Layout:

|offset|field|size|
|-|-|-|
|0|stream|be u16|
|2|offset|be u16|
|4|type|be u32|
|8|method|u8|
|9|usage|u8|
|10|usageIndex|u8|
|11|**padding**|u8|

The mutation zeros **only byte 11** of each element visited, up to and
including the `D3DDECL_END()` terminator. Nothing else in the struct is
written, and no bytes *outside* the array are touched.

**Upper bound of touched region.** With `inputCount=20` (AFTER log line 1783),
21 total entries are visited (20 real + terminator), writing 21 bytes. All
writes fall inside the interval `[arr_base + 11, arr_base + 11 + 20*12]`,
well within the 252-byte element buffer.

## 2. Does `sub_82A42A38` (original recomp) also zero padding?

**No.** Disassembly at `gta4_recomp.82.cpp:57961-58038` shows only:

- `lhz r9,0(r30)` - read `stream` half-word
- compare to 255, loop advancing `r10 += 12`

The guest walk reads **only** `stream` per element. Padding byte at offset
11 is neither read nor written by `sub_82A42A38`. The only side effect is
the `sub_821B3608` allocation + `sub_82A42948` memcpy of the input array
into a new guest-heap buffer.

So the hook's `padding = 0` is a **new** write the guest never performs.
However the guest also never **reads** offset 11 anywhere in this path.

## 3. Caller stack frame (`sub_828C09F0` at LR=0x828C0B30)

From `gta4_recomp.66.cpp:54017-54202`:

```
stwu r1,-928(r1)            ; 928-byte frame
...
addi r10,r1,98              ; write pointer starts at sp+98
addi r3,r1,96               ; arg3 for sub_82A42A38 = sp+96
...
loop: writes 12-byte elements starting at sp+96 (element 0)
      per element: stream@+0, offset@+2, type@+4, method@+8,
                   usage@+9, usageIndex@+10
      *** padding byte at +11 of each element is NEVER WRITTEN ***
loc_828C0AE4:               ; after loop
      writes terminator at sp+(96 + 12*count):
      stream = 0x00FF, type = 0xFFFFFFFF  (correct D3DDECL_END)
bl 0x82a42a38               ; r3 = sp+96
```

Element array occupies `[sp+96, sp+96 + 12*(count+1)]`. For `count=20`,
that is `[sp+96, sp+348]`. Frame size 928 bytes -> plenty of headroom on
both sides; no overlap with saved registers (at `[sp-24, sp-8]`) or link
area.

Bytes at offset 11 of each element (sp+107, +119, ..., +347) are
**uninitialized** in this frame. They hold whatever was in memory from the
previous user of that stack slot. The hook overwrites them with zero.

The guest never reads those bytes. The hash computed by
`XXH3_64bits(vertexElements, count * 12)` in the hook does read them, but
that is a host-side concern only.

## 4. Does the host traversal walk past the terminator?

**No.** Task 5 from the prompt asked whether host might walk past the
intended array scanning for a `0xFFFF` stream half-word. Code inspection
refutes this:

- Caller unconditionally writes terminator before branching to
  `sub_82A42A38` (loc_828C0AE4, stores at `r10 + 0..8` with `r8 = 0x00FF0000`
  from `PPC_LOAD_U32(sp+80)`, where `sp+80` was set up with `sth 255` at
  offset 0 and `sth 0` at offset 2 -> big-endian u32 = 0x00FF0000,
  i.e. stream=0x00FF, offset=0x0000).
- Host loop condition `stream != 0xFF && type != D3DDECLTYPE_UNUSED`
  terminates as soon as it sees `stream == 0xFF`. `be<uint16_t>` reads
  byte-swap: stream byte layout `00 FF` reads as `0x00FF = 255`. Match
  with `0xFF` terminator literal is exact.

Terminator is always present. Out-of-bounds walk is **not** triggered.

## 5. Callers of `sub_82A42A38`

Single caller only: `sub_828C09F0` at LR=0x828C0B30 (gta4_recomp.66.cpp:54189).
No other `bl 0x82a42a38` sites exist in the recomp. Input is always a
complete, terminated element array built on the caller's 928-byte stack
frame. Never a single in-flight `D3DVERTEXELEMENT9`.

Prompt's hypothesis ("a single in-flight element, not an array") is
refuted by IDA-matching recomp evidence.

## 6. Where does `v16 = 0xBFF8` actually come from?

The crash is in `sub_828D9AC8` vfunc[23] (grcTextureXenon), at a memcpy
dst that came from a Lock shim output. `v16` is populated by one of
`sub_82A44820 / sub_82A44838 / sub_82A44848`. The stack local sits at
`sp+7C..+88` of **a different frame** than sub_828C09F0's.

Only shared state between the two paths is the **guest thread stack** if
both run on the same thread. But sub_828C09F0 returns (`addi r1,r1,928`)
before sub_828D9AC8 runs, so its stack slots are logically released. Any
residual content at `[sp+107, sp+119, ...]` from the `padding=0` writes
would be inside the **released** region; a later callee's frame lives at
a lower `sp` and only overlaps if the new frame is larger than 580 bytes
AND the Lock shim reads from the exact padding offsets. This is a
low-probability coincidence, not a deterministic corruption vector.

## 7. Alternative: what could the hook really have broken?

The revert-diff claims "RexGlue's API mutates its input (zeros padding)."
**There is no RexGlue API `createVertexDeclarationWithoutAddRef`** -
a grep across `glue/rexglue-sdk-main/` returns no matches. The only
function with that name is the static host helper in `video.cpp:6054`.

So the padding-write is from our own helper, and its bounds are safe.

If the actual hook body additionally did:

1. A **host-API** call like `renderer->createVertexDeclaration(desc)` that
   internally iterates its own RenderInputElement array, no guest buffer
   was mutated.
2. Stored a `host_vd` pointer into a guest->host map keyed by `guest_vd`
   - this is pure host-side state, can't corrupt guest memory.
3. The two back-to-back calls at `arr=0x7010F7F0` correspond to two
   different element counts / contents; both use the same stack region
   because the caller reuses its `sp+96` buffer. That is normal guest
   reuse - not corruption.

**The two-back-to-back-calls claim is factually true**, but those calls
only write to *their own* in-array padding bytes. The claim that this
corrupts "nearby guest locals" is **not supported** by the element layout
or frame-size evidence.

## 8. Verdict summary

|Claim|Status|
|-|-|
|Hook mutates input (padding=0 per element)|VALIDATED - video.cpp:6061,6066|
|Mutation touches **only** byte 11 of each element|VALIDATED - single-byte store|
|Mutation extends past array bounds|REFUTED - bounded by terminator|
|Traversal runs off the end looking for 0xFFFF|REFUTED - terminator is written|
|Guest writes padding itself|REFUTED - caller never writes +11|
|sub_82A42A38 reads padding|REFUTED - reads only `stream@0`|
|Corruption of adjacent locals (v19..v22)|REFUTED - locals live in different frame|
|`dst=0xBFF8` caused by hook padding-write|REFUTED - frame-local, ungrounded|

**Overall: the stack-corruption-via-padding-mutation theory is REFUTED.**

The padding-zero writes are harmless. The true regression cause must lie
elsewhere - most likely in how the host side interacts with the GPU
renderer before it is fully initialized (the AFTER timeline crashes
during initial texture upload, well before shader-cache priming is done).

## 9. Recommended re-focus

- Investigate whether the hook's **host-side** RexGlue render command
  queue ran before `renderer->initialize()` completed - a race on
  `g_renderer` could give bogus return pointers.
- Check whether `host_vd` allocation (via `g_userHeap.AllocPhysical`)
  could **itself** be at address `0xBFF8` on first use (small physical
  heap offset). That would be a heap-pointer confusion.
- The **revert is still correct**, but its justification should reference
  a renderer-timing or heap-confusion hypothesis, not a stack-mutation
  one.
