# 10 — Synthesis: Integrated Fix Plan

Agent 10 of 10 (SYNTHESIS). Reads inputs from siblings 01, 02, 04, 05,
06, 07, 08, 09 (sibling 03 not published at synthesis time —
gap noted below).

**Rule reaffirmed: NO IMPLEMENTATION. This file is design-only.**

---

## Issue A — NULL `vertexDeclaration` deref at `video.cpp:5115`

### Inputs

|Sibling|Hypothesis|Verdict|Root-cause score|
|-|-|-|-|
|01 shader-derived decl|Emit per-VS default decl from Xenos bytecode|VALIDATED as architecturally correct; depends on prerequisite #3|High (7/10)|
|02 missing API hook|`sub_82A42930` = `SetVertexDeclaration`; not hooked|CONFIRMED with full caller graph and state-block evidence|Highest (10/10)|
|04 Xenia vfetch pattern|Xenia derives layout from `vfetch` ucode, not from guest decl|Confirmed as reference architecture|High (7/10) as long-term target|
|05 grcDevice default|"RAGE binds a default decl at boot"|REJECTED (no such default); investigation re-identifies sibling 02's missing hook|Highest (same finding as 02)|

### Convergence

Siblings 02 and 05 converge independently on the same root cause:
**the guest-side `SetVertexDeclaration` function in GTA IV is not hooked**.
Sibling 05 names `sub_829C9440` (from the ppc-mcp-server rendering-tools
registry). Sibling 02 names `sub_82A42930` with 6 named callers and a
direct crash-trace to `sub_828C21D0+0x88` (Python-verified:
`0x828C2258 - 0x828C21D0 == 0x88`).

**These are two different candidate addresses.** Sibling 02's
`sub_82A42930` is supported by a disassembled caller graph
(`sub_828BF1F8`, `sub_828BFC00`, `sub_828C0688`, `sub_828C0848`,
`sub_828C15C8`, `sub_82A415A8`) whose semantics exactly match
`IDirect3DDevice9::SetVertexDeclaration`. Sibling 05's address comes
from a pre-existing MCP tool table that sibling 05 itself flags as
"not a discovered top-level function". Priority goes to sibling 02.

Siblings 01 and 04 are **complementary**, not competing: they address
the deeper question "what should the decl be if the game never created
one through the D3D9 API at all?" — i.e. the `DrawPrimitiveUP`-style
immediate-mode path where no declaration handle exists.

### Ranked fixes for Issue A

**#1 — Hook `sub_82A42930` as `SetVertexDeclaration` (sibling 02).**

Rationale:
* Closes a clear symmetry gap: `sub_82A42760` (SetVS) and
  `sub_82A424A8` (SetPS) are hooked; the sibling VD hook is simply
  absent.
* Fixes the crash for **all 46 callers** of `sub_828C21D0` plus every
  indexed-draw caller of `sub_82A42930`.
* Uses existing host helpers `CreateVertexDeclarationWithoutAddRef` /
  `ProcSetVertexDeclaration` — no new architecture.
* The hard part is a **helper** `ResolveDeclElements(handle)` that
  decodes the pre-built state block at `dword_831C2298` /
  `dword_831C23CC` / `dword_82B0B530[]` into a
  `GuestVertexElement[]`. Handle-layout reverse-engineering is
  required before implementation.

**#2 — Per-shader default decl derived from Xenos bytecode
(sibling 01 / sibling 04).**

Rationale:
* This is the architecturally purest fix and the Xenia-authoritative
  approach (sibling 04 confirms Xenia never uses a host vertex
  declaration at all).
* Strictly broader than #1: handles any path where the game omits
  decl binding entirely (e.g. `DrawPrimitiveUP`-only paths with no
  state-block). 
* Sibling 01's Alternative #1 (runtime `vfetch` parsing in a
  `CreateVertexShader` hook, vs a XenosRecomp schema change) is the
  recommended variant — no shader-cache ABI bump, no full regen, no
  cross-project churn.
* Acts as a **safety net** under #1 (substitutes only when
  `pipelineState.vertexDeclaration == nullptr && pipelineState
  .vertexShader->defaultVertexDeclaration != nullptr`).

### Rejected / deprioritised

* **Sibling 05 "seat a canonical decl at boot".** Sibling 05
  themselves flags this as a fallback; it leaves every draw rendering
  the same wrong layout.
* **Any null-guard at `video.cpp:5115`.** All four siblings reject
  this. A null-guard only pushes the crash to `video.cpp:5178`
  (`desc.inputElements = pipelineState.vertexDeclaration
  ->inputElements.get()`).
* **Raising the Sonic `#if 0` block at `video.cpp:9415`.** Those
  addresses are Sonic-06, not GTA IV; they do not exist in the
  binary.

---

## Issue B — Latent UAF / "CBaseDC color slots read post-free"

### Inputs

|Sibling|Hypothesis|Verdict|Root-cause score|
|-|-|-|-|
|06 bump-arena flip handler|"Flip handler missing"|PARTIALLY WRONG — flip handler `sub_821BF990` exists|Medium (5/10), re-identifies ordering race|
|07 DC arena drain fence|Host fence OK; guest arena wraps without fence between producer and consumer|CONFIRMED|Highest (10/10) for Issue B|
|08 poison vs color|`0xFFE1E1E1` at HUD site is legitimate light-gray, NOT poison|CONFIRMED; crash is downstream of color|Issue B itself is MIS-SPECIFIED|
|09 capture by value|Capture-by-value already happens; UAF is on singleton mgr at `0x831C22A4`, not on color|CONFIRMED|Highest (10/10) — color is a red herring|

### Convergence

Siblings 08 and 09 independently demonstrate that **the "CBaseDC color
UAF" as originally stated does not exist**. The `0xFFE1E1E1` value at
the watchpoint is (a) a legitimate HUD light-gray produced by
`sub_821506E8` / `sub_821F1EE0` reading a static glyph table at
`0x82DF1F60+44*i+36`, OR (b) an unrelated poisoned pointer at the
singleton mgr slot `0x831C22A4` — NOT a freed DC color.

Sibling 09 pins the real UAF: the singleton `mgr` pointer at
`0x831C22A4` is freed while still referenced; writers are
`sub_828C01E0` and `sub_828C0338`, both from `sub_828C47E8`.

Sibling 07 names a **second, larger** UAF vector: the 1 MB DC arena at
`dword_82B38B58` silently wraps (`B74=0`) on overflow without fencing
the render-thread drain at `sub_821BB2D0`, so in-flight DCs can be
overwritten mid-dispatch.

Sibling 06 refutes its own hypothesis but surfaces the same underlying
ordering question: is there a `sub_821BF990` / `sub_821BB2D0` race on
host where the flip happens before the consumer drains?

### Ranked fixes for Issue B

**#1 — Arena drain/wrap guard (sibling 07 Option 1).**

Hook `sub_821BB3D8` to detect the overflow-wrap condition
(`B74 + aligned_size >= 0xFA000`, Python-verified 1 024 000) and
either:
* Triple-buffer the arena with a host-managed index ring (the
  `dword_82B38B58[]` array layout hints at triple buffering that the
  retail code never finishes using), OR
* Block (stall) the producing guest thread until the render-thread
  consumer reaches the matching `CEndDrawListDC` seq at
  `sub_821BB2D0`.

The triple-buffer variant is safer (no deadlock risk when producer =
consumer thread, e.g. `sub_821B5A08` called from `sub_82140088`).

**#2 — Singleton mgr lifetime (sibling 09).**

Investigate `0x831C22A4`, writers `sub_828C01E0` / `sub_828C0338`,
parent `sub_828C47E8`, grandparent `sub_828D4C88`. Determine whether
the poisoned value originates in the source slots `dword_831C3094`
(`0x831C23E4`) or `0x831C2294` before it is copied into the singleton
mgr slot. This is a narrower fix than #1 but fixes an orthogonal
defect that sibling 08 independently pointed to.

### What should NOT be done for Issue B

* Hook `sub_82A3DAB0` / `sub_828C2290` to "copy the color earlier"
  (sibling 09: already by-value, no-op).
* Register latches with a flip-handler callback list (sibling 06:
  RAGE uses a generation counter at `dword_82A925F0` shifted into bit
  18+ of `DC[1]`, not a callback list — rewriting this invariant
  would break `sub_821BB2D0`'s exit check).
* Add a host-side `g_executedCommandList` fence (sibling 07: host
  fence at `video.cpp:3676` is already correct).

---

## Dependency graph

|-|-|
|Issue A #1 (hook `sub_82A42930`)|blocks nothing; independent|
|Issue A #2 (shader-derived default)|depends on A#1 **prerequisite verification** (sibling 01 prereq #3): does `g_pipelineState.vertexShader` see a non-null `GuestShader` at the crash moment? This is a shared prerequisite|
|Issue B #1 (arena guard)|independent of A; independent of B#2|
|Issue B #2 (singleton mgr)|independent; cheaper but narrower|
|`ResolveDeclElements` helper for A#1|blocks A#1 implementation; requires reverse-engineering of state-block layout (`dword_831C2298` etc.)|

**No hard dependencies between Issue A and Issue B fixes.** They can
be implemented in parallel branches.

**Within Issue A**: implement A#1 first (sufficient for the observed
crash at `video.cpp:5115`); add A#2 only if prerequisite telemetry
reveals draws with no state-block (pure-UP paths that never traverse
`sub_82A42930`).

**Within Issue B**: B#2 first (smaller, falsifiable). B#1 is a larger
project and should not be attempted until the singleton mgr is ruled
in or out.

### Recommended implementation order

1. **Telemetry pass.** Log `vertexShader`/`vertexDeclaration` at the
   deref site (already present at `video.cpp:5229-5238`). Log every
   call to `sub_82A42930` (address, handle) and every `CreateVertex
   Declaration*` host-side to confirm the missing-hook diagnosis.
2. **A#1** — Hook `sub_82A42930`. Ship with a permissive
   `ResolveDeclElements` that covers the three state-block bases
   (`dword_831C2298`, `dword_831C23CC`, `dword_82B0B530[]`).
3. **A#2 safety net** — Only if post-A#1 telemetry still shows
   `vertexDeclaration == nullptr` at any draw. Implement as
   runtime vfetch parsing on a `CreateVertexShader` hook (sibling
   01 Alternative #1, no XenosRecomp schema change).
4. **B#2** — Singleton mgr at `0x831C22A4`: instrument the two
   writers, find where the poisoned pointer enters.
5. **B#1** — Arena wrap guard: design under watchpoints WP1-WP4 from
   sibling 06 before writing any hook. Ship triple-buffer variant.

---

## Risk matrix

|Fix|Arch risk|Integration blast radius|Verify cost|
|-|-|-|-|
|A#1 hook|Low. Mirrors existing `sub_82A42760` pattern exactly.|`ResolveDeclElements` is the only new code path; misinterpreting the state block produces a wrong decl (silent wrong-render) — tractable because layout is a `D3DVERTEXELEMENT9[]` terminated by `D3DDECL_END`.|Medium. Need per-call trace of handle → decoded elements → hash.|
|A#2 safety net|Low-Medium. Adds `defaultVertexDeclaration` field to `GuestShader`; no shader-cache ABI bump if done at runtime (sibling 01 Alt #1).|Touches `ProcSetVertexShader` and `CreateGraphicsPipelineInRenderThread`. No pipeline-hash churn (default is only used when decl is null, producing a distinct PSO).|Low. The first draw after the hook either renders or doesn't.|
|B#1 arena triple-buffer|High. Rewrites the producer/consumer contract for every DC in the game. Any bug silently corrupts all world-rendering.|Very high. Single hook on `sub_821BB3D8` touches every draw.|Very high. Requires WP1-WP4 from sibling 06, frame-level diff against Xenia.|
|B#2 singleton mgr|Low-Medium. Localised to one slot, two writers.|Low. Does not touch draw or pipeline.|Medium. Need to verify the singleton is the actual writer of the bad r11 at the sub_828C2290 crash.|

---

## Validation plan

### For A#1
* **Pre-A#1**: `/tmp/liberty_watch6.log` should show
  `vertexDeclaration == nullptr` at every HUD draw.
* **Post-A#1**: `vertexDeclaration != nullptr` within the first 100
  frames; `ProcSetVertexDeclaration` command count > 0;
  `g_pipelineState.vertexDeclaration->vertexStreams[0]` non-zero for
  the 36-byte FLOAT4+FLOAT2+D3DCOLOR+FLOAT2 HUD path.
* **Correctness probe**: dump the first decoded
  `GuestVertexElement[]` from `ResolveDeclElements` and compare
  against the HLSL input struct for the HUD VS (expected:
  `POSITIONT` (FLOAT4, offset 0) + `TEXCOORD0` (FLOAT2, offset 16) +
  `COLOR0` (D3DCOLOR, offset 24) + `TEXCOORD1` (FLOAT2, offset 28)).
* **Regression**: render a full frame of the main menu and diff the
  captured color-buffer against Xenia reference.

### For A#2
* **Guard**: only activates when A#1's path produced a null decl for
  a draw that still has a bound VS. If this condition never fires
  after A#1, A#2 is dead code — acceptable outcome.
* **Correctness**: per-shader `defaultVertexDeclaration` strides must
  match the game-provided `vertexStrides[0]` for the same draw
  (cross-check at first hit).

### For B#2
* Watchpoint on `0x831C22A4` for both reads and writes; log cycle
  counter and calling function. Poisoned value arrival time must
  precede the reader in `sub_828BF270` / `sub_82A3DF50`.

### For B#1
* WP1-WP4 from sibling 06, plus an explicit counter of "DCs
  dispatched between flip and next drain" — must remain 0 on a
  correctly-triple-buffered host.

---

## Open gaps (require more investigation before any fix ships)

1. **Prerequisite #3 from sibling 01** — is `g_pipelineState
   .vertexShader` non-null at `video.cpp:5115` on the first HUD
   crash? If YES, A#1 is sufficient and A#2 is optional. If NO, a
   separate `SetVertexShader` hook audit is required before either
   fix can land. One hour of telemetry resolves this.
2. **State-block layout for `dword_831C2298` / `dword_831C23CC`** —
   sibling 02 names three candidate arrays but does not disassemble
   any of the producers. Without this, `ResolveDeclElements` cannot
   be written. Requires disassembling one state-block constructor;
   closest candidates live near `sub_828BF1F8` (applier).
3. **Sibling 03 did not publish.** Uncovered area unknown. If
   sibling 03 was researching a fourth Issue A hypothesis or a
   different facet of Issue B, this synthesis may be missing one
   data point. Recommend publishing sibling 03's MD before
   committing to implementation.
4. **Issue B scope question** — the prompt describes Issue B as
   "CBaseDC color slots may be freed before the host renderer reads
   them". Siblings 08/09 show the color is not the failing object.
   B#1 (arena drain) and B#2 (singleton mgr) are the actual defects.
   The synthesis assumes the prompt's wording reflects a broader
   "lifetime on guest data consumed by host draw" concern; if the
   user only meant color specifically, **no Issue B fix is
   warranted** (sibling 09 verdict).
5. **Arena triple-buffer feasibility** — sibling 07 asserts
   `dword_82B38B58[]` is already an array. Need to confirm the
   storage size allocated at `dword_82B38B58` in `.bss` (must be
   ≥ 3 × pointer for a real triple-buffer).
6. **Draw-pool cursor bug at `0x831C2D28`** — sibling 08 notes the
   crash is actually a cursor=0x20 read from this slot, caller
   `sub_828BF248`. Neither A nor B nor any other sibling isolates
   the writer. This may be a **third** defect that masks as
   Issue A/B. Needs its own watchpoint investigation.

---

## Summary recommendation

* **Issue A top fix**: sibling 02's hook on `sub_82A42930` =
  guest `SetVertexDeclaration`. Sibling 01's shader-derived default
  is held in reserve as a safety net.
* **Issue B top fix**: sibling 09's singleton mgr at `0x831C22A4`
  (cheap, falsifiable). Sibling 07's arena drain guard is
  architecturally broader but much riskier and should only follow a
  successful B#2.
* **Implementation order**: telemetry → A#1 → verify → (A#2 if
  needed) → B#2 → (B#1 if needed).
* **Blockers**: sibling 03 not published; state-block layout not
  disassembled; prerequisite #3 telemetry pending.
* **No code written in this synthesis.**
