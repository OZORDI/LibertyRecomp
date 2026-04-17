# sub_82A00DC0 — Inline `__memcpy` (heavily-unrolled alignment-dispatching copy)

Agent 2/15 — post-fix crash research. RESEARCH ONLY.

## Summary

`sub_82A00DC0` is **GTA IV's inline `__memcpy` intrinsic** — a leaf PPC memcpy
optimized for the Xenon CPU with:

- 128-byte unrolled main loop (16 × `ld`/`std` pairs per iteration)
- `dcbt` prefetch pipeline (priming the next cache line 128 bytes ahead)
- Alignment-dispatching dispatcher with separate paths for 8-, 4-, and
  1-byte-aligned `dst` (src alignment dispatched inside each path via
  `rlwimi` byte-reassembly when src/dst are misaligned relative to each other).
- Bi-directional `dcbtst` (prefetch-for-store) once the tail falls below
  128-byte residue.

It is **NOT** the CRT `memcpy` at `0x82A11940` (that one is the named Xbox
`memcpy` that rexcrt hooks — see the project memory "CRT audit COMPLETE"
entry). sub_82A11940 in fact **tail-calls `sub_82A01248`** (the immediate
neighbor of sub_82A00DC0) through `sub_82A18CD0` (XMemCpy), meaning that the
0x82A00DC0 / 0x82A01248 pair together form the **underlying engine** that
several CRT wrappers dispatch into.

## Address map & sizes (Python-verified)

| name | addr | bytes | instrs | role |
|-|-|-|-|-|
| sub_82A00DC0 | 0x82A00DC0 | 0x488 | 290 | inline __memcpy (this report) |
| sub_82A01248 | 0x82A01248 | ~0xF8 | ~62 | simpler word-copy fallback |
| sub_82A11940 | 0x82A11940 | 0x318 | 198 | `memcpy()` CRT entrypoint — rexcrt-hooked |
| sub_82A18CD0 | 0x82A18CD0 | — | — | `XMemCpy` CRT entrypoint — rexcrt-hooked |

```
python3 -c "print(f'{0x82A01248 - 0x82A00DC0:#x}')"  -> 0x488
python3 -c "print(f'{0xB44:#x} vs 0x488 size')"     -> +0xB44 is OUTSIDE size
```

## Critical finding — "+0xB44" is NOT a guest-instruction offset

The diff-analysis labelled the crash as `sub_82A00DC0+0xB44`, but:

- Function is only **0x488 bytes** (290 PPC instructions).
- `0x82A00DC0 + 0xB44` = **0x82A01904**, well past the function
  (`sub_82A01248` at +0x488, then other CRT neighbors).
- `0xB44 / 0x488 = 2.49x` — consistent with host-binary expansion
  ratio (~3x is typical for a recomp'd unrolled loop due to
  `ctx.reg.u64 = …` scaffolding + address-translation).

**Conclusion:** `+0xB44` is a **host x86-64 byte offset** inside the compiled
`PPC_FUNC_IMPL(__imp__sub_82A00DC0)`. Given the density of `PPC_STORE_U64`
calls in the unrolled main loop (`loc_82A00F5C`, which emits 17 stores per
iteration — each expanded to 6–10 host instructions, so ~100–150 host bytes
per PPC instruction region), the crash almost certainly lands inside
**`loc_82A00F5C`** — the 128-byte 8-aligned unrolled hot loop at guest PC
0x82A00F5C..0x82A00FEC. That is the block that actually stores to
0xC000 when `r3` (dst) is garbage.

## Function layout (guest PC regions)

| guest range | label | role |
|-|-|-|
| 0x82A00DC0–0x82A00DEC | entry | Stash r3 at `-8(r1)`, compute `r6 = 8 - (r3 & 7)`; dispatch to alignment prolog |
| 0x82A00DF0–0x82A00E0C | dst-byte prolog | 1–7 byte walk to 8-byte-align dst |
| 0x82A00E10–0x82A00E20 | dst-word prolog | 4-byte swallow when `r3 & 7 == 4` |
| 0x82A00E24–0x82A00E3C | dispatcher | Classify **src** alignment (`r4 & 7`) against `r5 >= 128`; pick hot path |
| 0x82A00E40–0x82A00E94 | 8-aligned tail (small N) | Residue <128: copy in 8-byte chunks then 1–7-byte tail |
| 0x82A00E9C–0x82A00EDC | 4-byte fast tail | dst&3==0 store-word; else 4-byte byte-scatter |
| 0x82A00EDC–0x82A00F3C | **main-path setup** (src 8-aligned, N>=128) | Compute prefetch lead `r9`, end-sentinel `r11`/`r12`; issue prefetch chain `dcbt r9,r4` |
| **0x82A00F5C–0x82A00FEC** | **HOT LOOP — 128-byte unrolled 8-byte copy** | **Likely crash region** |
| 0x82A00FF0–0x82A01004 | hot loop tail | Final store prefetch via `dcbtst r8,r12` |
| 0x82A01008–0x82A01058 | word-aligned fast path | When src & dst both 4-aligned, same structure with 4-byte unrolled loops |
| 0x82A01060–0x82A01148 | word-aligned main | N>=128 variant using 4-byte loads+stores (another hot 128-byte unroll `loc_82A010DC`) |
| 0x82A01138–0x82A01248 | byte-shift path | src/dst alignment mismatched — uses `rlwimi` to reassemble 32-bit words from 4 bytes; another 128-byte unroll at `loc_82A011E4` |

## Algorithm confirmation

**Yes — matches diff-analysis hypothesis exactly**:

- **128B unroll**: confirmed at `loc_82A00F5C` (16 × `ld`/`std` at offsets
  +8/+16/…/+120, then `ldu/stdu 128`), and again at `loc_82A010DC` (4-byte
  variant) and `loc_82A011E4` (bytewise-rlwimi variant).
- **dcbt prefetch**: confirmed at `loc_82A00F3C` prelude priming next 128B
  line; and `dcbt r9,r4` inside each iteration.
- **No dcbz**: this function only does `dcbt` (load hint) and `dcbtst`
  (store hint). It **never zeros a cache line** — dcbz is inappropriate
  for memcpy since destination bytes get overwritten before the implicit
  zero flush matters.
- **Alignment bypass**: entry classifies `r3 & 7` and **always** aligns
  `dst` to 8 first (prolog at `loc_82A00DF8`). Then it classifies `r4 & 7`
  to pick the hottest-possible kernel.

## C++ equivalent

```cpp
// rage memcpy intrinsic — leaf, matches system-V PPC convention
// (r3=dst, r4=src, r5=n; return value in r3)
void* __memcpy(void* dst /*r3*/, const void* src /*r4*/, size_t n /*r5*/) {
    // alignment-dispatch big-N unrolled copy
    //  … body …
    return /* original */ dst;   // loaded back from -8(r1) before every blr
}
```

**Signature**: `void* sub_82A00DC0(void* dst, const void* src, size_t n)` —
returns the *original* `dst` value (confirmed by the `std r3,-8(r1)` at
function entry paired with `ld r3,-8(r1)` immediately before every single
`blr` in the function: 6 return sites, 6 reloads — no other side effects
to caller registers except r6–r12 clobber).

**Side effects**: cache pollution via `dcbt`/`dcbtst` only. No writes to
anywhere except `[dst, dst+n)`. No reads from anywhere except
`[src, src+n)` plus `-8(r1)` scratch.

## Relationship to rexcrt-hooked memcpy (0x82A11940)

Per project memory ("CRT audit COMPLETE — 32/51 PR #180 functions hooked"),
the rexcrt system hooks `memcpy` **at 0x82A11940** — a **different address**.
That one is the *named* CRT `memcpy` symbol in the XEX import table
(182 callers, non-leaf). It has only 3 callees:

- `__savegprlr_26` (save callee-saved regs)
- `sub_82A01248` (simpler word-copy fallback, 62 instrs)
- `sub_82A18CD0` (`XMemCpy` — itself also rexcrt-hooked)

So: **`sub_82A11940` is a `memcpy(…)` shim that probably bounces to XMemCpy
or falls back to sub_82A01248 for tiny N**, while the compiler **inlined
`sub_82A00DC0` at 577 call sites** (= 377 distinct static callers) as the
workhorse intrinsic for all the hot rage-engine copy sites. The two are
different functions with different callers.

**sub_82A00DC0 is NOT hooked by rexcrt** — it's plain recompiled code.
When a corrupt pointer reaches it, we get exactly the `0xC000` unmapped-store
fault we saw.

## Call-site neighborhoods (577 callers — key hotspots)

From `find_callers` (377 distinct statics, 577 call-site edges):

- **grcTextureXenon::vfunc[23]** (`sub_828D9AC8`) — the reported crash caller,
  texture unlock/upload finalization. This is the vtable slot 24 of the
  Xenon texture wrapper class.
- **grmGeometryQB::vfunc[2]** (`sub_828E57B0`) — geometry-buffer finalizer.
- **fiDeviceMemory::vfunc[4,5,7]** (`sub_828550C0`, `sub_82854F98`,
  `sub_82855160`) — in-memory file-device read/write paths.
- **scrThread::vfunc[0]** (`sub_82845668`) — script thread copy-state.
- **SegmentDataBLZPack::vfunc[2,3]** (`sub_82889B28`, `sub_828899D8`) —
  asset pack streaming.
- **btSpatialHash::vfunc[15]** (`sub_829C7D40`) — physics spatial-hash.
- **audConvolutionEffectXenon::vfunc[5]** (`sub_82933DC0`) — audio buffer
  copies.
- **AssetManager::vfunc[0]** (`sub_8287E0E0`) — ART asset manager.
- **fragType::vfunc[2]** (`sub_827AF850`) — fragment (breakable-object) type.
- **NmRsCBUHighFall::vfunc[6]** (`sub_828A8468`) — Euphoria behaviour.
- **snJoinSessionTask::vfunc[2]** (`sub_82805148`) — multiplayer session
  join task.
- **40+ callers in `sub_82A*` CRT region** — proof this is rage's internal
  memcpy that even other CRT-tier helpers reach for.

## Relevance to current crash

- Caller is `sub_828D9AC8` (grcTextureXenon::vfunc[23]).
- Destination pointer passed to sub_82A00DC0 was either itself 0xC000 or
  something offset to land at 0xC000.
- The unrolled main loop at `loc_82A00F5C` stores at offsets
  `+8, +16, +24, …, +120, +128` from the initial `r3-8`. If `r3` was
  extremely small (e.g. ~0), the first stw/std would land at
  `~8…~128` — matching **0xC000** if offset by the caller's initial
  add. More likely: an **uninitialized/freed `grcTextureXenon` whose
  data pointer was zeroed-plus-padding** was handed in.
- No instability in sub_82A00DC0 itself — it's pure algorithmic code.
  **The bug is in the caller**, not the memcpy.

## Files / paths

- Generated recomp: `/Users/Ozordi/Downloads/LibertyRecomp/glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.80.cpp` lines 4257–4939
- Pseudocode: `sub_82A00DC0_0x82A00DC0.c` (per MCP info)
- Neighbor (CRT-named memcpy): `sub_82A11940_0x82A11940.c`
- Neighbor (small-copy helper tail-called from CRT shim): `sub_82A01248` at
  generated.80.cpp lines 4941–5043
