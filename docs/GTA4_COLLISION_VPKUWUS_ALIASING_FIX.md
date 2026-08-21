# GTA IV fall-through-world collision bug: `vpkuwus` register-aliasing failure

## Status

Fixed and validated in LibertyRecomp on 2026-08-21.

The player and vehicles fell through visually intact roads because RexGlue generated
incorrect host code for one legal form of the PowerPC/VMX `vpkuwus` instruction:
the destination vector register aliasing either source vector register. GTA IV uses
`vpkuwus v0, v0, v13` while quantizing Bullet AxisSweep3 broadphase AABBs. The old
scalarized translation wrote part of `v0` before it had finished reading `v0`,
corrupting the Z-min broadphase endpoint to `0xFFFE` (`65534`). The floating-point
AABBs were correct, but their quantized endpoint ordering was impossible, so valid
road contacts disappeared from the broadphase pair cache.

The fix is in RexGlue's instruction generator, not in GTA IV game logic:

- load both complete source vectors before storing the destination;
- clamp unsigned 32-bit elements to `0xFFFF`;
- pack the clamped vectors to unsigned 16-bit elements in one alias-safe expression;
- test both `vD == vA` and `vD == vB` for `vpkuwus` and `vpkuwus128`;
- regenerate the recompiled game and rebuild it.

Do not work around this defect by disabling collision, forcing characters upward,
changing road assets, expanding AABBs, or replacing the game's physics routine.
Those approaches hide an instruction-semantics bug and will corrupt other users of
the same vector instruction.

## Scope

This document is for:

- RexGlue and other static-recompilation maintainers;
- emulator/JIT maintainers implementing PowerPC VMX instructions;
- LibertyRecomp contributors investigating streamed-world collision;
- developers diagnosing cases where correct floating-point bounds disappear after
  broadphase quantization.

The confirmed defect was in the Xbox 360 recompilation path. It was not a damaged
GTA IV collision archive, a missing XBN/WBN file, a renderer bug, or an error in the
retail game's collision logic.

## User-visible symptom

At the beginning of GTA IV gameplay:

- roads and buildings rendered normally;
- the player and current vehicle fell through the road surface;
- both could continue falling until reaching water or another independently handled
  condition;
- cutscenes could appear correct because their actors and cameras are not evidence
  that streamed static-world collision is functioning;
- the failure affected broad areas, rather than one malformed road polygon.

This combination initially looks like missing collision data. The renderer and the
physics streaming system are separate, however, so visible roads prove only that the
render assets were present.

## Relevant collision pipeline

The useful simplified path is:

```text
map sector request
  -> XBN/WBN resource load and fixup
  -> composite bound expansion
  -> static BVH child-body creation
  -> simulator ID assignment
  -> deferred physics insertion queue
  -> local AABB transformed to world AABB
  -> AxisSweep3 float-to-integer quantization
  -> min/max endpoint insertion and sorting
  -> broadphase overlap-pair cache
  -> collision filtering
  -> BVH narrowphase and contacts
```

The bug was at the quantization boundary. Everything before it could succeed while
the player still fell through the world.

## What the investigation ruled out

The first diagnostic captures established all of the following:

- collision sectors were requested;
- XBN assets loaded and published successfully;
- published pointers and asset entries were non-null;
- top-level sector AABBs were finite and ordered;
- composites expanded into static child bodies;
- the child bounds were GTA IV BVH bounds;
- simulator IDs were assigned successfully;
- static bodies were queued to the physics level;
- the deferred insertion queue drained;
- the inserted IDs were not immediately removed;
- world-space floating-point dynamic and static AABBs were finite, ordered, and
  overlapping where contact was expected.

Those observations moved the investigation past streaming and into broadphase state.

## Instrumentation strategy

All probes were placed behind the existing physics diagnostics category and called
the original guest functions without changing their return values or physics state.
The probes were added to:

```text
glue/rexglue-sdk-main/gta4-recomp/src/gta4_physics_trace.cpp
```

The decisive probe families were:

| Probe | Purpose |
|---|---|
| `broadphase-batch-aabb` | Record the float min/max and simulator ID entering bulk insertion. |
| `post-drain-static-membership` | Verify streamed static IDs remained addressable in broadphase storage. |
| `axis-sweep3-set-aabb` | Observe dynamic AxisSweep3 AABB updates. |
| `axis-sweep3-static-quantized` | Decode static handles and their stored endpoints. |
| `axis-sweep3-dynamic-static-detail` | Join float overlap, swept overlap, endpoint indices, and quantized endpoint positions for one dynamic/static pair. |
| `axis-sweep3-filter` | Observe group/mask filtering decisions. |
| `axis-sweep3-pair-add` | Observe candidate insertion into the pair cache. |
| `axis-sweep3-update` | Count pairs that survived the complete update. |

An important earlier blind spot was probing only base broadphase functions. The live
world used `btAxisSweep3_rage`; concrete vtable targets had to be instrumented to
observe the real update and pair-cache path.

## Decisive broken-run evidence

In fresh reproduced run 553, float AABBs overlapped correctly while their Z endpoint
ordering was corrupt:

```text
dynamicAabb=(873.683838,18.646093,25.253675)
         ..(874.313721,19.276667,27.388330)
staticAabb=(751.969238,-90.067383,4.723663)
        ..(959.327148,167.539124,65.116585)
currentOverlap=true

dynamicZ=(824,462,65534,11221)
staticZ=(1606,810,65534,11633)
```

The four integers for each axis were logged as:

```text
(minimum endpoint index,
 maximum endpoint index,
 minimum quantized position,
 maximum quantized position)
```

For X and Y, the minimum index and position preceded the maximum. For Z:

- the minimum endpoint index followed the maximum endpoint index;
- the minimum quantized position was always `65534`;
- the maximum position was around the correct world-region quantization value;
- the decoded float AABB was still finite, ordered, and overlapping.

This proved the defect was introduced between float AABB calculation and quantized
endpoint insertion.

### Capture-wide comparison

The same Python parser was run over the complete available rotated logs for the
broken and fixed captures:

| Metric | Broken run 553 | Fixed run 555 |
|---|---:|---:|
| Dynamic/static detail records | 38,297 | 60,953 |
| Dynamic endpoint inversions | 38,297 | 0 |
| Static endpoint inversions | 38,297 | 0 |
| Quantized endpoint values equal to `65534` | 76,594 | 0 |
| AxisSweep3 updates inspected | 972 | 1,234 |
| Updates retaining static pairs | 0 | 1,234 |
| Maximum retained static pairs in an update | 0 | 19 |

The defect was deterministic, not intermittent: every inspected broken-run
dynamic/static record had inverted Z endpoint state.

## Root cause in RexGlue

The source instruction used by GTA IV was:

```asm
vpkuwus v0, v0, v13
```

`vpkuwus` packs four unsigned 32-bit words from each source into eight unsigned
16-bit elements, saturating values above `0xFFFF`. The destination is allowed to be
the same architectural register as a source. Architecturally, the result must be the
same as if both source values were captured before the destination became visible.

The old RexGlue generator expanded the instruction into eight scalar assignments:

```cpp
for (size_t i = 0; i < 4; i++) {
  destination.u16[7 - i] =
      source_a.u32[3 - i] > 0xFFFF
          ? 0xFFFF
          : static_cast<uint16_t>(source_a.u32[3 - i]);
  destination.u16[3 - i] =
      source_b.u32[3 - i] > 0xFFFF
          ? 0xFFFF
          : static_cast<uint16_t>(source_b.u32[3 - i]);
}
```

That implementation is correct only when `destination`, `source_a`, and `source_b`
are distinct host objects.

### How aliasing corrupted an unread source

When `destination == source_a == v0`, the vector's `u16` and `u32` views share the
same 16-byte storage. A write through `v0.u16[n]` immediately changes half of one
`v0.u32[m]` source element.

The loop's early writes to `u16[3]` and `u16[2]` changed `u32[1]`. A later iteration
then read that already-modified `u32[1]` as if it were an original source word. The
corrupted word exceeded the unsigned 16-bit range and saturated to `0xFFFF`.

A small Python model of the old assignment order produced:

```text
aliased_u16_after_old_codegen =
  [1000, 2000, 3000, 4000, 65535, 65535, 300, 400]
corrupted_pack_lane = 65535
```

The retail code subsequently applies an even-position mask to minimum endpoints.
Python verification of the observed constant is:

```text
0xFFFF & 0xFFFE = 0xFFFE = 65534
```

That explains the otherwise conspicuous repeated value in the physics logs. It was
not an AxisSweep3 sentinel chosen by GTA IV; it was the downstream signature of the
aliased pack corruption.

## Correct RexGlue implementation

The fix is in:

```text
glue/rexglue-sdk-main/src/codegen/builders/vector.cpp
```

The corrected builder emits one expression:

```cpp
simde_mm_store_si128(
    reinterpret_cast<simde__m128i*>(destination.u16),
    simde_mm_packus_epi32(
        simde_mm_min_epu32(
            simde_mm_load_si128(
                reinterpret_cast<simde__m128i*>(source_b.u32)),
            simde_mm_set1_epi32(0xFFFF)),
        simde_mm_min_epu32(
            simde_mm_load_si128(
                reinterpret_cast<simde__m128i*>(source_a.u32)),
            simde_mm_set1_epi32(0xFFFF))));
```

The source order in the emitted expression matches RexGlue's existing vector lane
layout. The significant semantic properties are:

1. Both complete 128-bit source values are loaded as inputs to the expression.
2. Each unsigned 32-bit element is clamped to `0xFFFF`.
3. `simde_mm_packus_epi32` may then safely perform its signed-input pack because all
   elements are inside the non-negative signed range.
4. The destination receives a single 128-bit store after the sources have been
   evaluated.
5. `vD == vA`, `vD == vB`, and distinct-register forms therefore produce the same
   architectural result.

The generated GTA IV code now has the required shape:

```cpp
// vpkuwus v0,v0,v13
simde_mm_store_si128(
    (simde__m128i*)ctx.v0.u16,
    simde_mm_packus_epi32(
        simde_mm_min_epu32(
            simde_mm_load_si128((simde__m128i*)ctx.v13.u32),
            simde_mm_set1_epi32(0xFFFF)),
        simde_mm_min_epu32(
            simde_mm_load_si128((simde__m128i*)ctx.v0.u32),
            simde_mm_set1_epi32(0xFFFF))));
```

## Regression tests

The pre-existing tests covered saturation with distinct registers only. That is why
the instruction appeared correct despite failing in GTA IV.

Alias cases were added to:

```text
glue/rexglue-sdk-main/tests/ppc/asm/instr_vpkuwus.s
glue/rexglue-sdk-main/tests/ppc/asm/instr_vpkuwus128.s
```

Both suites now cover:

```asm
# Destination aliases source A.
vpkuwus v3, v3, v4

# Destination aliases source B.
vpkuwus v4, v3, v4
```

and the equivalent `vpkuwus128` forms.

### Testing rule for vector instructions

Every vector instruction whose destination can overlap an input should have at least
these cases:

1. all registers distinct;
2. destination equals the first source;
3. destination equals the second source;
4. destination equals every source when the instruction permits it;
5. boundary values immediately below, at, and above each saturation limit;
6. all-zero and all-ones inputs;
7. lane-distinct values so ordering errors cannot hide behind repeated data.

This rule applies beyond pack instructions. Any scalarized host sequence that writes
the destination between source-lane reads deserves the same audit.

## Regeneration and build procedure

Changing the builder does not retroactively change already generated GTA IV C++.
Regenerate before rebuilding the game.

From the repository root:

```bash
cd glue/rexglue-sdk-main/gta4-recomp
../out/mac-arm64/rexglue codegen gta4_manifest.toml
cd ../../..
cmake --build ./out/build/macos-release --target LibertyRecomp
```

Verify at least one known aliasing site:

```bash
rg -n -A 3 -B 2 '// vpkuwus v0,v0,v13' \
  glue/rexglue-sdk-main/gta4-recomp/generated/
```

The generated code must contain complete vector loads followed by one vector store.
It must not contain interleaved scalar `u16` stores and later `u32` reads from the
same register object.

Run the PPC instruction suite when the platform has a working PowerPC test
assembler:

```bash
cmake --build ./out/build/macos-release --target ppc_tests
```

During this investigation, the application built and linked successfully on Apple
Silicon, but the standalone PPC assembly suite could not execute because the bundled
`powerpc-none-elf-as` was a Linux x86-64 executable. This was a test-environment
limitation, not a failing test. The alias cases remain in the suite for compatible
hosts and future CI.

## Runtime validation procedure

The validation run used serialized native rendering only to keep unrelated renderer
changes out of the physics diagnosis:

```bash
REX_GTA4_NATIVE_FRAMES_IN_FLIGHT=1 \
MTL_HUD_ENABLED=1 \
"./out/build/macos-release/LibertyRecomp/Liberty Recompiled.app/Contents/MacOS/Liberty Recompiled" \
  --diagnostics \
  --diagnostics-categories logging,physics
```

Acceptance criteria:

- player and vehicles remain grounded in the original failure area;
- float AABBs remain finite and ordered;
- for every axis, minimum quantized position is less than maximum position;
- minimum endpoint index precedes maximum endpoint index for active overlaps;
- no relevant endpoint is pinned to `65534`;
- dynamic/static pairs survive the AxisSweep3 update;
- no collision assets or bodies disappear during streaming;
- no physics state is changed by instrumentation.

Fixed run 555 satisfied every criterion, and the in-game fall-through symptom was
confirmed gone.

## Why the fix belongs in the instruction translator

Patching the GTA IV collision function itself would be the wrong abstraction layer.
The retail code uses a legal vector instruction with legal register aliasing. Any
correct interpreter, JIT, or static recompiler must preserve that instruction's
architectural behavior.

Fixing RexGlue gives three benefits:

- every `vpkuwus` user is corrected, not just GTA IV collision;
- generated code remains a faithful translation of the guest binary;
- tests can enforce the ISA contract independently of any game.

A game-specific workaround would also risk masking other corrupted lanes that happen
not to produce an immediately visible symptom.

## Guidance for other recompilers and emulators

If another PowerPC implementation shows similar symptoms, do not assume it has this
exact defect. Confirm it independently:

1. Locate the guest instruction used for float-to-endpoint packing.
2. Verify whether the destination aliases either source in the real executable.
3. Capture source registers immediately before the instruction.
4. Capture the destination immediately after it.
5. Compare against an ISA-level reference implementation.
6. Test interpreter and JIT/recompiler paths separately.
7. Fix the instruction implementation if it violates aliasing semantics.
8. Use a game-memory patch only when the game itself is wrong and the exact
   executable version and address are proven.

An implementation that obtains both sources before calling a vector pack primitive
is naturally protected from this specific read-after-write problem. An implementation
that emits per-lane stores must explicitly snapshot aliased inputs first.

## Lessons learned

### Correct assets do not imply correct broadphase state

Successful streaming, body creation, and finite world AABBs were necessary but not
sufficient. Quantization was a separate correctness boundary.

### Numeric sentinels can be transformation artifacts

The repeated `65534` looked like a deliberate broadphase sentinel. Following its data
lineage showed it was `0xFFFF` saturation followed by an even-endpoint mask.

### Tests need legal register overlap

Distinct-register tests validated the arithmetic but not the instruction. Register
aliasing is part of the instruction contract.

### Instrument concrete implementations

Base-class hooks produced misleading zero activity because the live AxisSweep3 vtable
called concrete functions. Always identify the live implementation before concluding
that a subsystem is idle.

### Fix the earliest violated invariant

The earliest violated invariant was not "player has no contact." It was "quantized
minimum endpoint precedes quantized maximum endpoint." Fixing that invariant restored
the entire downstream collision pipeline without changing game logic.

## Source references

- RexGlue instruction builder:
  `glue/rexglue-sdk-main/src/codegen/builders/vector.cpp`
- Alias regression tests:
  `glue/rexglue-sdk-main/tests/ppc/asm/instr_vpkuwus.s`
  and `instr_vpkuwus128.s`
- GTA IV physics diagnostics:
  `glue/rexglue-sdk-main/gta4-recomp/src/gta4_physics_trace.cpp`
- Generated AxisSweep3 code:
  `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.76.cpp`
- PowerPC Vector/S instruction semantics:
  [PowerPC Microprocessor Family: Vector/S Multimedia Extension Technology Programming Environments Manual](https://ps3linux.net/ps3-filez/cellsdk-docs/3.1/arch/vector_simd_pem_v_2.07c_26Oct2006_cell.pdf)
- AxisSweep3 quantization and endpoint ordering:
  [Bullet `btAxisSweep3Internal.h`](https://github.com/bulletphysics/bullet3/blob/master/src/BulletCollision/BroadphaseCollision/btAxisSweep3Internal.h)
- Broadphase filtering and pair-cache contract:
  [Bullet `btOverlappingPairCache.h`](https://github.com/bulletphysics/bullet3/blob/master/src/BulletCollision/BroadphaseCollision/btOverlappingPairCache.h)

## Final conclusion

GTA IV fell through the world because a legal aliased `vpkuwus` instruction was
translated as an alias-unsafe sequence of scalar writes. One unread source word was
overwritten, saturated to `0xFFFF`, masked to `0xFFFE`, and stored as every relevant
Z-min AxisSweep3 endpoint. Correct float AABBs therefore became invalid quantized
intervals, eliminating static-world collision pairs.

Making the translator load both sources before its single destination store removed
all endpoint inversions and restored every inspected static-pair update in the fixed
capture. No GTA IV physics behavior or collision asset required modification.
