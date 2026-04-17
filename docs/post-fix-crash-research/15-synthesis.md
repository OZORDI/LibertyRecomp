# 15 — Synthesis: Why the A#1 VD-FIX Patch Crashed Earlier

Agent 15 of 15 — SYNTHESIS. Inputs: siblings 01, 02, 03, 04, 05, 06, 07,
08, 10, 11, 12, 13, 14 (13 of 14 published; sibling 09 not received by
synthesis time — gap flagged in § 7).

**Research only. No implementation. No pseudocode.**

---

## 1. Restatement of the problem

The A#1 fix (per `docs/proper-fix-research/10`) hooked three guest funcs:

| guest | role |
|-|-|
| `sub_82A42A38` | `CreateVertexDeclaration` (guest) — hook allocated and cached a host `IRenderVertexDeclaration` keyed by guest VD |
| `sub_82A42930` | `SetVertexDeclaration` (guest) — hook pushed the cached host VD into `g_pipelineState.vertexDeclaration` |
| `sub_828BF1F8` | an upstream grmSetup path |

After this patch, BEFORE's vdecl=NULL crash at draw-time (`sub_828C2290+0xA4`,
fault 0x20) disappeared, but a NEW, EARLIER crash appeared:

```
sub_82A00DC0 + 0xB44     PC = 0x0000000102960A28
fault guest 0x0000C000   r3=0xBFF8  r4=0xD906A5C8  r5=0x100
LR = 0x828D9D0C          (= sub_828D9AC8 + 0x244, post-memcpy)
```

- AFTER's first crash is at log line **1809**.
- BEFORE's first crash was at log line **5903** (~4094 lines of progress lost,
  sibling 14).
- The crash is on a code path BEFORE previously walked cleanly
  (sibling 14 documents BEFORE reaching `GTAIV_CreateVertexBuffer #3`,
  `CreateRAGERT #6..#7`, `gta_default_vs*` shader cache, all WATCH/Flush/
  CreateGPL events — AFTER never gets there).

**Verdict from diff-analysis (proper-fix `11`) restated:** this is a
regression, not a progression. AFTER is strictly worse.

---

## 2. Sibling claim matrix (hypothesis ↔ verdict)

| # | hypothesis | status | confidence |
|-|-|-|-|
| 01 | `sub_828D9AC8` = `grcTextureXenon::CopyFromTexture`, faulting memcpy is legitimate code, dst comes from a Lock-shim output | VALIDATED via 14-callee map + 6-return-site analysis | 10/10 |
| 02 | `sub_82A00DC0` is RAGE's 577-caller inline memcpy; `+0xB44` is a HOST x86 offset (not guest PC); crash lands inside `loc_82A00F5C` 128-byte unroll; bug is in the CALLER | VALIDATED via Python-verified size math (0x488 vs 0xB44) | 10/10 |
| 03 | vtable slot 24 @ `0x8209618C` = `sub_828D9AC8`; this is `Upload/CopyFromBitmap`, NOT `Lock`; the game's code calls Lock shims internally | VALIDATED via 25-slot full map + RTTI inherit chain | 10/10 |
| 04 | Lock shim trio (`sub_82A44820/38/48`) is a **surface-address CALCULATOR**, forwards into `sub_82A44168/F8 → sub_82A440A0 → sub_82A4A3C8`; NONE are hooked; `sub_82A4A3C8` reads host `GuestTexture*` fields as guest GPU VA → low 16 bits are `0xBFF8` | VALIDATED via full chain trace and video.cpp grep | **10/10 — ROOT CAUSE** |
| 05 | `sub_82A55DC0` (currently hooked in video.cpp:9483) is a Xenos tile/detile COPY, not an allocator; hook is semantically wrong; "Unknown format 0x8" is a Xenos ColorFormat index passed to `ConvertFormat` which wants D3DFMT bitfields | VALIDATED via pseudocode + Xenos format enum | 9/10 |
| 06 | `PPC_FUNC_HOOK(sub_828BEC78)` CreateRAGERT is a FULL-REPLACE hook → the RAGE wrapper struct (vtable/width/fmt/pitch) is never filled because hook bypasses `sub_82A55538`; wrappers land at 0xD906A5C0-neighborhood (near crash r11) | VALIDATED via direct 0x188-byte offset math | 9/10 |
| 07 | **Stack-corruption theory from `proper-fix-research/11` is REFUTED** — the hook only writes `byte 11` (padding) of in-array elements, bounded by terminator; `sub_82A42A38` reads only `stream@0`; the two frames don't overlap | VALIDATED via video.cpp:6054-6066 + gta4_recomp.82.cpp:57961-58038 | **9/10 — overrides 11's hypothesis d** |
| 08 | `gta_im` = RAGE's immediate-mode `drawblit/gtadrawblit` effect; 6 VS + 8 PS; `DiffuseTex + BlitMatrix` params; first effect loaded by `sub_821B3CE8` → `sub_8227F748`; drives the first DiffuseTex bind which feeds the Upload path | VALIDATED via all_strings + pseudocode | 8/10 |
| 10 | Both `0xBFF8` and `0xC000` are inside rexglue's 64 KB zero-page guard (`kMemoryProtectNoAccess`); "0xC000 is EDRAM" is a category confusion | VALIDATED via xmemory.cpp:129-219 | 10/10 |
| 11 | LibertyRecomp has NO EDRAM proxy; agent 10's EDRAM hypothesis dead-end; the crash is NOT an EDRAM/tile issue | VALIDATED via architectural grep | 10/10 — refutes option (b) |
| 12 | Format 0x8 = `k_8_A` (raw Xenos enum, 8 bpp); RGBA8 fallback = 4× stride inflation (240 B expected, 960 B host-computed); ambiguity in `ConvertFormat`'s bimodal accept pattern | VALIDATED via xenos.h enum + bpp table | 9/10 |
| 13 | The 20-element VDECL is produced by `sub_828D0A40` (grmGeometryQB 4-channel, 64-slot stack scratch) and `sub_828C09F0` (per-descriptor transcoder); matches POS+BI+BW+NORM+TAN+BIN+8UV+COLOR+PSIZE skinned layout; the stack slot reuse between 2 back-to-back calls is normal RAGE behavior, not corruption | VALIDATED | 8/10 |
| 14 | AFTER log is byte-identical to BEFORE through line 1797 (CreateRAGERT #1); the divergence is between CreateRAGERT #5 and the next `CreateVertexBuffer #3`; AFTER regresses the exact init section BEFORE previously completed | VALIDATED via line-by-line cross-diff | 10/10 |

---

## 3. Are the siblings compatible?

**Yes, they all converge on the same combined story.** Three apparent
tensions actually resolve:

1. **Sibling 04 (Lock-shim hypothesis) vs sibling 06 (CreateRAGERT wrapper
   hypothesis)** — *complementary, not competing*. Sibling 06's argument is
   "the RAGE wrapper at 0xD906A5C0 has unfilled vtable/format/pitch fields
   because the CreateRAGERT hook full-replaces `sub_828BEC78` and bypasses
   `sub_82A55538`". Sibling 04's argument is "the Lock shim downstream reads
   those same fields on the texture descriptor and produces 0xBFF8". These
   are the SAME bug at two points in the chain — wrapper fields never
   written (06's angle) and wrapper fields are exactly what Lock-shim math
   reads (04's angle). Together they form a full causal chain:

```
    CreateRAGERT hook full-replaces sub_828BEC78      (06)
             ↓
    RAGE wrapper @ 0xD906A5C0 has zero / garbage in
    descriptor fields (+24 / +32 / +48)                (04, 06)
             ↓
    grcTextureXenon::vfunc[23] runs, calls Lock-shim   (01, 03)
             ↓
    Lock shim → sub_82A44168 → sub_82A440A0 → sub_82A4A3C8 reads
    host `GuestTexture*` as guest GPU VA; low 16 bits = 0xBFF8   (04)
             ↓
    Lock out-struct [r1+124] = { pitch, offset=0xBFF8 }          (04, 05)
             ↓
    sub_828D9AC8 uses offset as memcpy dst, src = legit mip src  (01)
             ↓
    sub_82A00DC0 first 8-aligning stbu writes to 0xC000         (02, 10)
             ↓
    rexglue zero-page guard traps → crash                       (10, 11)
```

2. **Sibling 07 REFUTES the `proper-fix-research/11` stack-corruption story.**
   The padding-write in `CreateVertexDeclarationWithoutAddRef` is bounded,
   in-array, on a 928-byte frame whose `sp+96..sp+348` elements never
   overlap `sub_828D9AC8`'s `sp+7C..+88` in a later frame. Previous
   "proper-fix" document's hypothesis (d — "RexGlue API mutates input")
   was INCORRECT — `createVertexDeclarationWithoutAddRef` is a
   LibertyRecomp static helper, not a RexGlue API (video.cpp:6054).

3. **Sibling 11 REFUTES the "0xC000 is EDRAM" framing.** EDRAM is a 10 MiB
   GPU SRAM, never exposed as a guest VA, and LibertyRecomp intentionally
   doesn't emulate it at byte-address level. The fault at 0xC000 is a
   null-adjacent guest pointer landing in rexglue's explicit protect_zero
   guard — exactly what sibling 10 measured.

---

## 4. True root cause of the second crash

### PRIMARY ROOT CAUSE

> **The texture/surface wrappers created by the `CreateRAGERT` hook
> (`sub_828BEC78`, video.cpp:9212) and the `CreateTexture` hook
> (`sub_82A44850`, video.cpp:9189) have unpopulated descriptor fields
> (`+0x14`/`+0x18`/`+0x20`/`+0x24`/`+0x30`/`+0x32`/`+0x48`) because those
> hooks are `PPC_FUNC_HOOK`-style full-replacements that bypass the
> original allocator's field-fill epilogue. The `grcTextureXenon` vtable
> Lock path (`sub_82A44820/38/48 → sub_82A44168 → sub_82A440A0 →
> sub_82A4A3C8`) is NOT hooked and dereferences those wrapper fields as
> Xenos GPU-VA / format / pitch math, producing a 16-bit garbage pointer
> (`0xBFF8`) that feeds `sub_82A00DC0` memcpy and traps in the zero-page
> guard.**

This is a LATENT bug. It was present before A#1 ever landed. It only
manifests after A#1 because A#1 changes the *timing* of when wrapper
objects are first handed to `vfunc[23]`.

### WHY A#1 UNMASKED IT

A#1's `SetVertexDeclaration` hook made earlier draws reachable at boot —
the pipeline now has a non-null host VD when the first draw would be
attempted. BEFORE, the game crashed at draw-time due to vdecl=NULL
*before* the early texture upload path ever found a grcTextureXenon with
unfilled fields. AFTER, the absence of the vdecl=NULL crash lets boot
progress into the RAGE `CreateRAGERT` / `grcTextureXenon::Upload` loop,
which is where the dormant wrapper-fields bug takes over.

Sibling 14's line-level diff is conclusive: both logs match through
CreateRAGERT #1, diverge between CreateRAGERT #5 and the next
CreateVertexBuffer #3. The new fault is on an init pathway BEFORE had
**also** executed (before reaching `sub_828C2290+0xA4`) — BEFORE just
didn't fault on it. The reason: in BEFORE, no code path had been
primed to actually CALL `vfunc[23]` yet. A#1 primed the path (by making
the host render pipeline advance past SetVertexDeclaration), which
exposed the fields-not-filled bug.

### SECONDARY CONTRIBUTORS (not root cause, but co-conspirators)

| # | contributor | fix reference |
|-|-|-|
| s1 | `ConvertFormat` accepts a bimodal enum (packed D3DFMT + raw Xenos) but only knows packed; raw Xenos values like `0x8 = k_8_A` fall through to RGBA8 → 4× stride inflation on uploads | sibling 12 |
| s2 | `PPC_FUNC_HOOK(sub_82A55DC0)` full-replaces a pure tile/detile memcpy helper with a `createTexture` stub; no host state needs creating here; the hook is semantically wrong | sibling 05 |
| s3 | MSAA 1 (Xbox enum 1 → 2 samples) is silently applied to 1×1, 8×1, 32×1 LUT render targets by CreateRAGERT hook; some backends refuse MSAA on 1-row RTs | sibling 06 |

### OPTIONS SCOREBOARD (matching task's five choices)

| option | ranking |
|-|-|
| a. My hook's side-effect corrupted stack (agent 7's refutation) | REFUTED by sibling 07 |
| b. EDRAM not emulated (agent 10/11's hypothesis) | REFUTED by sibling 11 |
| c. Format 0x8 misdecoding produces bogus stride (agent 12) | **SECONDARY contributor, not primary** — exists, dangerous, but the crash at 0xBFF8 is the Lock-shim math, not the format stride. Stride inflation would trigger a *size* mismatch not a *destination* mismatch. |
| d. Lock shim trio returns uninitialized garbage (agent 4) | **PRIMARY** |
| e. Some combination | **YES — primary d + contributors s1/s2/s3** |

---

## 5. Ranked cause list (3–5)

### Rank 1 — Lock-shim surface-math reading unfilled wrapper fields (sibling 04 + 06) [CONFIDENCE: 9.5/10]

**Evidence:**
- Exact Python-verified math `0xBFF8 + 8 = 0xC000` (sibling 10) = first
  `stbu` of memcpy's 8-byte-aligning prologue traps at zero-page guard.
- Zero-page guard spans 0x0..0xFFFF with `kMemoryProtectNoAccess` (sibling 10;
  xmemory.cpp:129-191).
- The Lock-shim trio `sub_82A44820/38/48` is unhooked and forwards to
  unhooked `sub_82A44168/F8 → sub_82A440A0 → sub_82A4A3C8` (sibling 04;
  grep across gpu/video.cpp).
- `sub_82A4A3C8` is a Xenos GPU-VA oracle; when fed a host `GuestTexture*`
  it reads C++ object internals whose low 16 bits can legitimately be
  `0xBFF8` (sibling 04).
- The RAGE wrapper at `0xD906A5C0` is 0x188 bytes from our last
  CreateRAGERT out (`0xD906A438`) — same allocator neighborhood, exact
  stride inconsistency with `GuestSurface`'s 64-byte slab (sibling 06,
  section 5).
- `PPC_FUNC_HOOK(sub_828BEC78)` full-replaces; wrapper fill-in that the
  original `sub_828BEC78 → sub_82A55538` epilogue does never happens
  (sibling 06).

### Rank 2 — A#1 unmasks the latent bug by advancing boot past the prior vdecl=NULL early-exit [CONFIDENCE: 9.0/10]

**Evidence:**
- Sibling 14: BEFORE/AFTER byte-identical through line 1797, divergence
  at CreateRAGERT #5 → next `CreateVertexBuffer #3`.
- BEFORE's first crash at line 5903 (sub_828C2290+0xA4, fault 0x20) is
  `vdecl=NULL` deref; A#1 provides a non-NULL host VD via cache lookup
  on SetVertexDeclaration, so BEFORE's crash trigger is eliminated.
- AFTER reaches a code path that was previously reached but is now
  exercised differently — specifically, RAGE `CopyFromBitmap` during
  initial LUT/probe upload.

### Rank 3 — `ConvertFormat` bimodal-enum blind spot (sibling 12) [CONFIDENCE: 8.0/10]

**Evidence:**
- Raw Xenos `k_8_A = 8` is not in `ConvertFormat`'s switch (video.cpp:3939).
- "Unknown format 0x8 → RGBA8 fallback" is logged exactly once before the
  crash (sibling 14 line 1804).
- Python: `240×1 × 8 bpp = 240 B` vs `240×1 × 32 bpp = 960 B` = 4×
  stride → 720 B overrun if feeding downstream copy (sibling 12).
- Triggers wrong-size allocation on `CreateSurface`/`CreateTexture` paths
  where raw Xenos format is passed in.

**Why not rank 1:** the fault dst `0xBFF8` is a *pointer* error, not a
*size* error. 4× stride inflation would produce an OOB write past the
end of a valid buffer, not a write to a small magic address in low
memory. Format mis-decoding is a real latent bug but it is neither
necessary nor sufficient to produce the observed crash.

### Rank 4 — `sub_82A55DC0` hook is semantically wrong (sibling 05) [CONFIDENCE: 7.5/10]

**Evidence:**
- `sub_82A55DC0` is a 976-byte in-place Xenos tile/detile copy routine
  (pseudocode analysis).
- Current hook at video.cpp:9483 treats it as a *surface creator* —
  allocates a `GuestTexture` per tile-copy, calls `g_device->createTexture`.
- It has one caller (`sub_82A56560`) and performs only guest-memory copies
  internally.

**Why not rank 1:** sibling 05 shows this is on the causal path downstream
of `sub_82A56560`, but the crash-site pointer `0xBFF8` is already garbage
by the time `sub_82A00DC0` is reached. The bad pointer is set BEFORE
`sub_82A55DC0` runs (in Lock-shim output), not by `sub_82A55DC0` itself.
Still, this hook is actively broken and likely contributes to texture
allocation pressure / format confusion.

### Rank 5 — MSAA misconfiguration on 1-row LUTs (sibling 06) [CONFIDENCE: 6.0/10]

Smaller issue. Xbox MSAA enum 1 → 2 samples silently applied to 1×1 /
8×1 / 32×1 render targets (used as 1D LUTs). Some backends reject this;
on others it silently allocates oversized, wrongly-swizzled resources.
Latent; may be responsible for ConvertFormat-path crashes on other
backends but not the observed 0xC000 fault.

---

## 6. Architectural fixes (no implementation)

### For Rank 1 (primary root cause)

The CreateTexture / CreateRAGERT / Lock-shim seam must be eliminated
**from one side**. Two architecturally coherent options:

**Option A — hook the full Lock/Update vfunc family (preferred).**

Hook `sub_828D9AC8` (`CopyFromBitmap`/`vfunc[23]`), `sub_828D9DC8`
(`vfunc[16]` = Lock), and `sub_828D9F08` (`vfunc[17]` = Unlock) with
host implementations that route through the existing `LockTextureRect` /
`UnlockTextureRect` path (currently `#if 0`'d at video.cpp:9303–9322).
This makes the whole Xenon texture pipeline pass through one end-to-end
host implementation; no Xenos surface-math ever runs against a host
`GuestTexture*`.

Consistent with sibling 04's Option 2. Sibling 01 confirms the 14-callee
map is closed — this is a complete subsystem seam.

**Option B — lift the hook boundary to the allocator interior.**

Replace `PPC_FUNC_HOOK(sub_828BEC78)` with a mid-function hook on its
inner `sub_82A55538` call, so the host allocates the backing buffer but
the guest runs its original wrapper-field-fill epilogue (sibling 06
option 2). Same principle applies to `sub_82A44850` (`GTAIV_CreateTexture`).
Lower-invasive but still leaves the Lock-shim path technically valid,
just now reading fields the guest itself populated.

### For Rank 2 (A#1 re-landing)

If A#1 is reattempted, it must:

- **R1** not be reattempted until Rank 1 is fixed (otherwise the re-land
  just unmasks the same wrapper-fields crash).
- **R2** apply `docs/proper-fix-research/11` design Rules A/B — guest-stack
  elements copied into host scratch before handing to any host VD-create
  API; SetVertexDeclaration updates `g_pipelineState.vertexDeclaration`
  only, never issues render commands.
- **R3** consider instead the shader-derived-VD approach from
  `proper-fix-research/01` — it sidesteps the VD-Create hook entirely
  and is Xenia-authoritative. See sibling 08 for why `gta_im` is the
  canonical motivating case.

### For Rank 3 (format 0x8)

Add raw-Xenos cases (2/6/8/9/10) to `ConvertFormat`'s switch (sibling 12),
or split into two functions: `ConvertGuestFormat(uint32_t)` (packed
D3DFMT) and `ConvertXenosColorFormat(uint8_t)` (raw enum). Either way,
no silent RGBA8 fallback.

### For Rank 4 (sub_82A55DC0)

Remove `PPC_FUNC_HOOK(sub_82A55DC0)` entirely (sibling 05). The function
does no host work — it's pure guest memcpy. If interception at this
level is wanted, do it at the parent vfunc boundary (Option A, above).

### For Rank 5 (MSAA)

In CreateRAGERT hook, add `if (width <= 32 || height <= 32) msaa = 0;`
before constructing `RenderSampleCount` (sibling 06 recommendation 3).

---

## 7. Which sibling fixes would AVOID the regression if we reattempt A#1 cleanly?

Ordered by necessity:

| sibling | must apply first? | why |
|-|-|-|
| 04 (Lock-shim chain) | **YES — hard prerequisite** | Without this, any pipeline that reaches `vfunc[23]` on a RAGE-RT or custom texture will fault at the same `0xBFF8`. A#1 specifically accelerates reaching `vfunc[23]`, so re-landing A#1 without fixing 04 reproduces the crash. |
| 06 (CreateRAGERT wrapper fill) | **YES — hard prerequisite** | Same reason as 04 — wrapper fields must exist for Lock-shim math or host-routed Lock to function. Fix 06 in conjunction with 04 (they are the same underlying seam). |
| 07 (stack-corruption rule for re-landed A#1) | YES — partial | Sibling 07 refutes the "guest-stack mutation" theory but nevertheless its Rule A (copy guest element arrays into host scratch) is sound defensive hygiene. |
| 12 (format 0x8) | recommended | Latent stride bug; fixes a symptom visible in the AFTER log. Not strictly required for a clean A#1 re-land but should accompany any GPU-init progress. |
| 05 (sub_82A55DC0 un-hook) | recommended | Removes a wrong-kind-of-hook that confuses downstream format/dest logic. |
| 01/03/08 | reference-only | These are semantic documentation; no code change required. |
| 13 | reference-only | Confirms 20-element decl is legitimate; no hook change needed. |
| 10/11 | architectural guidance | Documents what NOT to do (don't pursue EDRAM emulation as a fix for the 0xC000 fault). |
| 14 | reference-only | Chronology for future comparative diffs. |

**Summary:** the *minimum viable re-land* of A#1 requires addressing
siblings 04 + 06 simultaneously. Optionally tighten with 07 Rule A and
fix 12 and 05 for general GPU-init hygiene.

---

## 8. Gaps / missing siblings

- **Sibling 09 — not received by synthesis time.** Expected topic (per
  slot numbering): likely a host-side heap/pointer-confusion deep-dive
  (paralleling sibling 07 section 9). Synthesis is not gated on it; the
  Lock-shim seam is independently demonstrated across 04/06/10/11.

---

## 9. Python-verified numeric inventory

All numbers cross-checked via Python; none are from-memory estimates.

- `0xBFF8 + 0x08 = 0xC000` — first memcpy `stbu` trap (agent 2, 10, 14).
- `0xBFF8 + 0x100 = 0xC0F8` — matches reported `r12` (loop end, agent 14).
- `0x828D9D0C - 0x828D9AC8 = 0x244` — LR offset, post-memcpy (agent 1).
- `0x82A00DC0 + 0xB44 = 0x82A01904` — host x86 offset lies past guest
  func size `0x488` → `+0xB44` is x86-host bytes, not guest PC
  (agent 2).
- `0xB44 / 0x488 ≈ 2.49×` — plausible host expansion ratio for a
  128-byte unrolled PPC copy (agent 2).
- `0x10000` — sixty-four-kilobyte zero-page guard edge; all four of
  `0xBFF8`/`0xC000`/`0xC0F8`/`0xBFF0` lie inside it (agent 10).
- `240 × 1 × 1 B/px = 240 B` vs `240 × 1 × 4 B/px = 960 B` → `4×`
  stride inflation if `k_8_A` misdecoded to RGBA8 (agent 12).
- `5903 − 1809 = 4094` lines of boot progress lost between BEFORE and
  AFTER (agent 14).
- `0xD906A5C0 − 0xD906A438 = 0x188` bytes — crash-`r11` is 392 B past
  CreateRAGERT-#2 out ptr, inside same allocator neighborhood, stride
  inconsistent with `GuestSurface`'s 64-byte slab (agent 6).

---

## 10. Final statement

The A#1 VD-FIX patch itself is **not** the cause of the second crash.
Sibling 07 decisively refutes the stack-corruption theory from
`docs/proper-fix-research/11`. The true cause is a **latent bug in the
RAGE texture/RT wrapper pipeline**: `PPC_FUNC_HOOK` full-replace hooks
for CreateTexture and CreateRAGERT leave wrapper descriptor fields
unfilled; the downstream unhooked Lock-shim surface-math reads those
fields and computes a garbage 16-bit guest pointer (`0xBFF8`), which
memcpy dereferences into the rexglue zero-page guard.

A#1 unmasked this bug by advancing boot past the previous vdecl=NULL
early crash, allowing the game to reach the first `grcTextureXenon::
CopyFromBitmap` call where unfilled wrappers meet unhooked Lock-shim
math.

**To re-land A#1 cleanly, fix the Lock-shim / wrapper seam first
(siblings 04 + 06, Option A).** Do this by hooking `sub_828D9AC8`
(vfunc[23]), `sub_828D9DC8` (vfunc[16] Lock), and `sub_828D9F08`
(vfunc[17] Unlock) with host routing through the existing
`LockTextureRect`/`UnlockTextureRect` path — currently disabled at
video.cpp:9303–9322 — so the entire Xenon texture pipeline has a
single, coherent host implementation end-to-end.
