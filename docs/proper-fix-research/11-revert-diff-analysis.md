# 11 — Revert-Diff Analysis: VD-FIX Hook BEFORE vs AFTER

Forensic comparison of two LibertyRecomp run logs bracketing the three-hook VD-FIX patch
(sub_82A42A38 / sub_82A42930 / sub_828BF1F8).

- `/tmp/liberty_BEFORE.log` — 614 MB, 10.9 M lines, first crash at line **5903**
- `/tmp/liberty_AFTER.log`  — 1.17 GB, 23 M lines, first crash at line **1809**

Crash symbols (first occurrence):

|Run|PC|Fault guest addr|Meaning|
|-|-|-|-|
|BEFORE|`sub_828C2290 + 0xA4`|`0x20`|draw-state dispatch to NULL base (inlined pipeline-state path)|
|AFTER|`sub_82A00DC0 + 0xB44`|`0xC000`|memcpy write through bogus guest dst pointer (page boundary)|

Note: the caller asserted BEFORE's first crash was
`FlushRenderStateForRenderThread+0x6AC → SanitizePipelineState+0x184`. That pattern **does**
dominate from line 8859 onward, but the *first* BEFORE crash is actually
`sub_828C2290 + 0xA4` (line 5903) with fault at `0x20`. SanitizePipelineState
(`_ZL21SanitizePipelineStateR13PipelineState + 0x184`) is the **second** distinct crash
(line 8859+). Both are variants of the same underlying bug: `g_pipelineState.vertexDeclaration`
is NULL when `CreateGraphicsPipelineInRenderThread` runs.

---

## 1. Chronological markers before first crash

### Shared preamble (both logs, identical ordering through line 1782)
```
GTAIV_CreateTexture #1..#3 — 720x1 fmt=0x28280106
GTAIV_CreateVertexBuffer #1, #2 — length=0x2d0
```

### Line 1783+: FIRST DIVERGENCE

BEFORE (no hooks):
```
1783 CreatePSFromBytecode #1: gta_im_vs0.bin (CACHE HIT)
...  CreatePSFromBytecode #2..#6
...  CreateShaderFromBytecode #1..#8
1797 CreateRAGERT #1..#7 (interleaved with sub_82A55DC0 rect init at #5)
...  CreatePSFromBytecode #7..#21 (gta_default shaders)
...  CreateShaderFromBytecode #9..#42
...  [WATCH] F608 seq=1..N (rage poison 0xFFE1E1E1)
...  [SetVS] / [SetPS] / [GTA4] SetVertexShader / SetPixelShader
...  [WATCH] DAB0 seq=1, DF50 seq=1, DrawPrimUP seq=1 (vdecl=NULL)
...  [WATCH] ProcDP seq=1 entry, pre-flush
...  [WATCH] Flush seq=1 ENTER → ... → pre-CreatePipeline
...  [WATCH] CreateGPL seq=1 vs=0x4ff18f000 ps=0x4ff18b000 vdecl=0x0 → CRASH
```

AFTER (with hooks):
```
1783 [VD-FIX] Create seq=1 LR=0x828C0B30 arr=0x7010F7F0 guest_vd=0x20000360 host_vd=0x4ff193000 inputCount=20
1784 [VD-FIX] Create seq=2 LR=0x828C0B30 arr=0x7010F7F0 guest_vd=0x20094460 host_vd=0x4ff192000 inputCount=20
1785 CreatePSFromBytecode #1..#6 (gta_im vs)
1791 CreateShaderFromBytecode #1..#8 (gta_im ps)
1799 CreateRAGERT #1..#4
1803 sub_82A55DC0 rect init
1806 CreateRAGERT #5  → CRASH
```

**Delta**: AFTER fired 2 `[VD-FIX] Create` events at line 1783–1784 (LR=0x828C0B30 —
same call site, back-to-back, on the `gta_im` immediate-mode shader init path).
Both used the same guest-stack input array at 0x7010F7F0.
After those 2 calls, the log sequence **matches BEFORE** for ~20 more lines
and then stops: AFTER never reaches any `gta_default_*` shader loads, never issues
any `[WATCH]` events, never logs a `SetVS`/`SetPS`, never enters `Flush`/`CreateGPL`.

**VD-FIX Set events**: `0` in AFTER. The hook for SetVertexDeclaration
(`sub_82A42930`) never fires — game crashes before any draw-time VD lookup.

Event tallies before first crash:

|Event|BEFORE (<line 5903)|AFTER (<line 1809)|
|-|-|-|
|VD-FIX Create|0|2|
|VD-FIX Set|0|0|
|CreateShaderFromBytecode|~42|8|
|CreatePSFromBytecode|~21|6|
|CreateRAGERT|~7|5 (crash on #5 epilogue)|
|SetVS / SetPS|~3|0|
|ProcDP / DrawPrimUP|1|0|
|CreateGPL|1 (crashed inside)|0|

---

## 2. Crash classification

**Verdict: REGRESSION.**

AFTER crashes **earlier** in the game's initialization sequence, on a
code path BEFORE never reached at this phase.

- BEFORE's crash is at *first draw-time pipeline creation*
  (immediate-mode watermark draw, `g_pipelineState.vertexDeclaration == NULL`).
  Game completed shader-cache priming, render-target setup, and issued its first
  DrawPrimUP — crashing only when the pipeline hash/state is dereferenced.
- AFTER's crash is during **initial texture upload** (grcTextureXenon::vfunc[23]).
  Game hadn't yet loaded the `gta_default_*` shader family, had completed only
  half of the render-target creation, and never entered the draw dispatch.

Not a progression: AFTER reached **fewer** meaningful engine checkpoints than BEFORE
(no WATCH events, no Flush, no CreateGPL).

Not a sideways shift: the crash site is unrelated to the guarded code path.
The new crash hits a texture-mipmap upload `memcpy` that BEFORE had already passed
at this point in the timeline (texture uploads happened pre-shader-cache in both runs,
confirmed by `GTAIV_CreateTexture #1..#3` logging at line 1778).

---

## 3. Decoding `sub_828D9AC8` and `sub_82A00DC0`

### sub_828D9AC8 — `rage::grcTextureXenon::vfunc[23]`
(vtable slot 24 @ `0x8209612c`, size ~0x300, confirmed via `mcp__liberty-decomp`)

Purpose: **texture-mipmap upload / copy**. Pseudocode summary:
1. Validates texture descriptor `a1` matches source header `a2` (stride, levels, format).
2. For each array slice (`v5`), for each mip level (`v8`):
   - Calls dispatch `sub_82A44820 / sub_82A44838 / sub_82A44848` (grcDevice Lock variants,
     selected by `v18` = format enum at `a2[2]`).
   - Reads Lock output into stack locals `v19/v20/v21/v22` (at sp+78, sp+7C, sp+80, sp+88).
   - If a "size-multiplier" flag `(v28 & 0x100) != 0`:
     - **Calls `sub_82A00DC0(v16, *(u32*)(v7+16), w*h*pixelBytes)`** ← this is our crash
     - `v16 = (v18 != 3) ? v20 : v22`
   - Dispatches `sub_82A42E88` (Unlock).
3. After all levels, copies 6 floats from `a2` (texture extent / dimension metadata)
   to `a1+36..a1+56`.

### sub_82A00DC0 — memcpy
(size ~0x488, 577 callers, leaf, top-500 hot, confirmed via pseudocode inspection)

It's the game's **inlined PPC memcpy**:
- 128-byte block unroll with `dcbt` prefetch
- Three alignment variants (`r3 & 7`, `r4 & 7`): dword, half-dword, byte-shift
- Signature matches classic RAGE/Xenon `memcpy` — arg3 encodes byte count in the low word
  and alignment padding in the high word
- Call site `+0xB44` is well inside the 128-byte inner loop after initial prefix-alignment

Confirmed: this **is** `memcpy`, not `memset` or `XMemSet`.

---

## 4. PPCContext at crash

```
r0=0x0        r1=0x7010F820   r3=0x0000BFF8 (dst)    r4=0xD906A5C8 (src)
r5=0x100 (size=256)           r6=0xFFFFE535          r7=0xFFFFE535
r8=0xFF35FDFF                 r9=0x108               r10=8
r11=0xD906A5C0 (src-8)        r12=0x0000C0F8 (dst+size end)
lr=0x828D9D0C (= sub_828D9AC8 + 0x244, exact memcpy call site from pseudocode)
```

Arithmetic check:
- `r3 + r5 = 0xBFF8 + 0x100 = 0xC0F8` = r12 (loop end bound, matches)
- `r3 + 8 = 0xC000` (first guest-page boundary into which memcpy writes)
- Fault at guest `0xC000` = **first dst store past the end of the mapped guest page at 0xBFF0**

Pattern: **memcpy write-side overrun into an unmapped guest page**.

This is **NOT** a NULL-pointer deref. `dst=0xBFF8` is a small, non-zero,
4-byte-aligned guest address. `src=0xD906A5C8` is a normal high-memory pointer.
Only 8 bytes of writes succeed (the prefix-align block) before the loop enters
the unmapped 0xC000 page.

Interpretation: `v16` (destination for the mip copy) was populated by the
grcDevice Lock shim (`sub_82A44820/sub_82A44838/sub_82A44848`) with a
**bogus value**. On a real console, these would return a pointer into
the texture's backing store; here they returned `0xBFF8`.

---

## 5. Did the hook's side effect cause this?

**Plausible and consistent with evidence, but not conclusively proven from the log alone.**

The three-hook patch did the following at VD-Create time:
1. Called the original (guest) `sub_82A42A38`.
2. Called host `renderer->createVertexDeclarationWithoutAddRef(...)` passing the same
   `D3DVERTEXELEMENT9[]` array that the guest supplied (`arr=0x7010F7F0` — **guest stack**).
3. Stored the `host_vd` in a guest→host map keyed by the returned `guest_vd`.

Evidence compatible with a side-effect regression:

**a. Input array lives on guest stack.**
`arr=0x7010F7F0` is in the guest PPC stack frame of the calling function (LR=0x828C0B30).
RexGlue's `createVertexDeclarationWithoutAddRef` typically **mutates** its input array
(zeros the `padding` field of each `D3DVERTEXELEMENT9`, and may re-pack entries).
Any mutation of the guest stack by a host function is unsafe unless the frame is
explicitly treated as scratch — which it is not here.

**b. The two VD-Create calls fire back-to-back at the same `arr` address.**
Same stack slot (0x7010F7F0), two different guest VDs (0x20000360, 0x20094460). The
calling function is re-using the same stack buffer to build element arrays for both
immediate-mode formats. This is normal guest behavior; what is *not* normal is a
host function writing to that buffer between guest reads.

**c. The crash-side `v16` destination `0xBFF8` is consistent with stack reuse.**
After the two VD-Create calls return, the same guest function (or a sibling on the
same thread) reuses nearby stack slots for texture Lock output. `sub_828D9AC8`'s
stack locals `v19..v22` sit at `sp+78..sp+88`; if the Lock shim reads these back
after a host-side mutation, a truncated-to-16-bit value (`0x0000BFF8`) would
match the register layout (r3 = 0xBFF8, high 16 bits zeroed).

**d. Logged memory pressure change.**
BEFORE never reaches the `[VD-FIX]` area of video.cpp at this time in the run.
AFTER's earlier crash-line position (1809 vs 5903) is not a function of "log spam
from hook" — both the hook logs and the normal event logs account for at most
~10 extra lines. The 4000-line gap reflects real missing work (shader loads, RT
creation, rect init).

The cleanest hypothesis:

> The hook's host-side `createVertexDeclarationWithoutAddRef` mutates the guest
> D3DVERTEXELEMENT9 array on the PPC stack. Nearby stack locals used later by the
> texture-upload path (`sub_828D9AC8` or its Lock callees) are corrupted,
> producing a bogus Lock output pointer that feeds memcpy.

Could also be: the host VD cache kept references to host RenderInterface objects
that are not ready (GPU not yet started), or a vtable-dispatch race. But the
small-value dst pointer pattern (`0xBFF8`) strongly suggests *data*, not a
global/vtable slot — so stack-based is the most economical explanation.

---

## 6. Recommendation

**Revert remains correct** (already done by the user). The current three-hook
approach has an unacceptable risk of corrupting guest stack frames that host
functions cannot see.

Safer variant — two design rules:

### Rule A: Never pass a guest stack pointer directly to a host API that may mutate it.
Before calling `createVertexDeclarationWithoutAddRef`, copy the guest
`D3DVERTEXELEMENT9` array into a **host-owned scratch buffer**:

```cpp
// In hook:
D3DVERTEXELEMENT9 scratch[MAX_VE];
const uint32_t count = ...;  // count elements up to D3DDECL_END terminator
for (uint32_t i = 0; i < count; ++i) {
    // Byte-swap and copy each element from guest memory into host scratch.
    scratch[i] = SwapVertexElement(g_memory.Translate(guestArr + i * sizeof(D3DVERTEXELEMENT9)));
}
IRenderVertexDeclaration* hostDecl = renderer->createVertexDeclarationWithoutAddRef(scratch, count);
g_guestToHostVD[guestVD] = hostDecl;
```

This isolates any host-side mutation to the scratch buffer.

### Rule B: Don't bind host state from inside a guest-time hook.
The original third hook (`sub_82A42930` SetVertexDeclaration) called
`renderer->SetVertexDeclaration(...)` as a render command. But
`g_pipelineState.vertexDeclaration` is pulled at draw-time, in
`FlushRenderStateForRenderThread`. Update the *field*, not a render command:

```cpp
PPC_FUNC_HOOK(sub_82A42930, ctx, base) {
    __imp__sub_82A42930(ctx, base);  // run original
    const uint32_t guestVD = ctx.r4.u32;
    auto it = g_guestToHostVD.find(guestVD);
    if (it != g_guestToHostVD.end()) {
        g_pipelineState.vertexDeclaration = it->second;
        SetDirty(g_dirtyStates.vertexStrides);  // if needed
    }
}
```

### Rule C (optional hardening)
The upstream fix at the `sub_82A42A38` level is still correct in principle (cache
the host VD), but the **real** fix for the vdecl=0 bug is the shader-derived
decl fallback described in `01-shader-derived-vertex-decl.md` and
`08-poison-vs-color-resolution.md`. Those proposals don't require hooking the
guest's CreateVertexDeclaration at all — they synthesize a host VD from the
vertex-shader input signature at draw time, which matches Xenia's known-working
approach.

If Rules A+B don't clear the regression in a quick re-test, drop the VD-Create
hook entirely and implement the shader-derived fallback in
`FlushRenderStateForRenderThread`:

```cpp
if (!g_pipelineState.vertexDeclaration) {
    g_pipelineState.vertexDeclaration =
        GetOrCreateShaderDerivedVD(g_pipelineState.vertexShader,
                                   g_pipelineState.vertexStrides);
}
```

This is the proposal from docs 01 and 10 of this research series. It sidesteps
any guest-stack-mutation risk entirely and covers all draw paths, not just
immediate-mode.

---

## Appendix: key log line references

- BEFORE shader priming starts line 1783 (`CreatePSFromBytecode #1` gta_im_vs0.bin)
- AFTER  shader priming starts line 1785 (same event, 2 VD-Create lines inserted)
- BEFORE first crash line 5903, `sub_828C2290+0xA4`, fault 0x20
- AFTER  first crash line 1809, `sub_82A00DC0+0xB44`, fault 0xC000
- BEFORE `SanitizePipelineState+0x184` crashes start line 8859 (recurring every ~130 lines)
- AFTER  `sub_82A00DC0+0xB44` crashes start line 1813 (recurring every 34 lines — tighter
  signal-handler reentry loop because crash is on a faster-repeating path)

Function metadata used (via mcp__liberty-decomp):

- `sub_828D9AC8` — class `grcTextureXenon_rage::vfunc[23]`, vtable slot 24, size ~0x300
- `sub_82A00DC0` — 577 callers, leaf, top-500 hot, size ~0x488 (memcpy)
- `sub_82A57088` — thunk → `sub_82A56C70`
- `sub_828C2290` — size 108 bytes, but PC+0xA4 is past end (inlined or branch-delay)

Decompiled call site in `sub_828D9AC8` (from `get_function_pseudocode`):

```c
sub_82A00DC0(
    v16,                                       // dst  — from Lock output (v20 or v22)
    *(_DWORD *)(v7 + 16),                      // src  — mip-source ptr from level struct
    *(u16*)(v7+14) * *(u16*)(v7+12) * *(u16*)(v7+2));  // size — w * h * bytesPerPixel
```

This matches observed registers: r3=bogus Lock output, r4=src mip buffer, r5=w*h*bpp byte count.
