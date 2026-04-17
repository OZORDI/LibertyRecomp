# 01 — Shader-Derived Vertex Declaration

Agent 1 of 10. Investigation into whether the NULL `vertexDeclaration`
dereference at `video.cpp:5115` can be root-cause fixed by deriving a
per-shader default `GuestVertexDeclaration` from the Xenos vertex shader
bytecode.

## Hypothesis

The vertex format for every Xenos vertex shader is **fully encoded inside
the compiled shader bytecode**: the game's HLSL compiler emits a list of
`VertexElement` records (one per `vfetch` instruction in the pre-shader)
as part of the Xenos shader container. `XenosRecomp` already parses these
records to emit correct HLSL/MSL input structs; the same data could be
exported at codegen time as a per-shader `GuestVertexDeclaration` that
the runtime substitutes when the game never calls `SetVertexDeclaration`
(e.g., the HTML/HUD `DrawQuadFP → DrawPrimitiveUP` path that crashes).

## Evidence For

### 1. XenosRecomp already decodes vertex elements

- `tools/XenosRecomp/XenosRecomp/shader.h:57-78` defines
  `VertexElement { address:12, usage:4, usageIndex:4 }` packed into the
  `VertexShader::vertexElementsAndInterpolators[]` array, with
  `vertexElementCount` giving the length.
- `shader_recompiler.cpp:1726-1771` iterates exactly
  `vertexShader->vertexElementCount` and produces one input-struct field
  per element, looking up the corresponding attribute slot in the
  `USAGE_LOCATIONS` table (lines 78-122). The `vertexElements` map keyed
  by `address` is used again in `shader_recompiler.cpp:216-250` for the
  `vfetch` swizzle logic — so the element table is **authoritative**: the
  recompiler relies on it being complete and correct.
- The `DeclUsage` enum (`shader.h:39-55`) is a bit-for-bit match for the
  Xbox 360 D3D9 `D3DDECLUSAGE` values declared in
  `LibertyRecomp/gpu/video.h:371-389`.

### 2. The data flows from real Xenos bytecode loaded by the game

- Game shader bins live in `LibertyRecompLib/shader/rage_shaders/` (92
  effect groups). Those `.bin` files ARE the Xenos containers parsed by
  XenosRecomp at codegen (see `main.cpp:129-149` — container magic
  `0x102A1100`, `virtualSize + physicalSize` parsed directly).
- The relevant HUD/HTML shaders most likely live in
  `rage_shaders/gta_im/` (immediate mode) — 6 VS variants totalling
  2.8 KB, small enough to be the shader bound for `sub_8227F2E8`'s
  `DrawQuadFP` path (POSITIONT + COLOR + UV).

### 3. Xenia validates the design

- `tools/xenia-master-1/src/xenia/gpu/shader.h:700-714` defines the exact
  same concept: `Shader::VertexBinding { fetch_constant, stride_words,
  attributes[] }`, where each attribute carries a
  `ParsedVertexFetchInstruction`. Xenia reconstructs vertex bindings by
  parsing `vfetch` opcodes (FetchOpcode::VertexFetch = 0, also present in
  `tools/XenosRecomp/XenosRecomp/shader_code.h:175`), combined with the
  vertex fetch constants bound by the game — EXACTLY what the
  pre-shader + element-table emission does.
- This is the industry-standard pattern for Xenos emulation: no Xbox 360
  title explicitly declares a D3D9-style IA layout; the layout is
  derived from `vfetch` + `vertexElements`.

### 4. The current shader-cache carries the bytecode forward, not the
   layout

- `LibertyRecompLib/shader/shader_cache.h:6-18` defines
  `ShaderCacheEntry { hash, dxilOffset, dxilSize, spirvOffset,
  spirvSize, airOffset, airSize, specConstantsMask, filename[256],
  guestShader* }`. There is **no vertex-element field**. The information
  is extracted by XenosRecomp then **discarded** — it exists only as
  text in the regenerated HLSL/MSL source.
- The runtime therefore has no way to know "this `GuestShader` expects a
  36-byte POSITIONT+COLOR+UV input layout" at bind time.

### 5. No upstream project needs this because they hook explicit creation

- Both `UnleashedRecomp` and `MarathonRecomp` hook
  `D3DDevice_CreateVertexDeclaration` directly
  (`GUEST_FUNCTION_HOOK(sub_82BE0428, CreateVertexDeclaration)` /
  `sub_82547118`, see `MarathonRecomp/gpu/video.cpp:8010`), rely on the
  game ALWAYS calling `SetVertexDeclaration` before every draw, and
  `SanitizePipelineState` simply deref's the pointer with no null check
  (Unleashed `video.cpp:4043`, Marathon equivalent, and Liberty
  `video.cpp:5115`). Their shader cache ALSO omits vertex-layout
  metadata for exactly the same reason.
- GTA IV (RAGE v2) diverges: the Sonic-style D3D9 entry points
  (`D3DDevice_SetVertexDeclaration`, `D3DDevice_DrawVerticesUP`, etc.)
  exist as `REX_EXPORT_STUB` no-ops in
  `glue/rexglue-sdk-main/src/kernel/xam/xam_misc.cpp:42-99` but are
  **never imported** by the game binary. GTA IV is statically linked
  against the Xenos D3D library and the runtime emits PM4 packets
  directly. Consequently: the usual `CreateVertexDeclaration →
  SetVertexDeclaration → ProcSetVertexDeclaration` chain in
  `LibertyRecomp/gpu/video.cpp:6054-6323` is **dead code on the GTA IV
  draw path**. The Sonic hooks at `video.cpp:9420-9461` are gated by
  `#if 0 // Disabled Sonic 06 hooks` at 9415.

### 6. `sub_82A3DF50` is a dirty-state commit, not a UP draw

- The crash narrative in the prompt says "DrawPrimitiveUP_Begin →
  commit → host DrawPrimitiveUP". Looking at the actual recomp:
  `sub_82A3DF50` is 16 bytes, leaf: `r11 = *(r3+13428); *(r3+48) = r11`
  — a field copy inside a RAGE `grcDrawPool`. `sub_82A3DAB0` is 1184
  bytes of dirty-mask iteration that dispatches to
  `sub_82A4ACF0`/`sub_82A4B088`/`sub_82A4B148`/`sub_82A4B2C8`/
  `sub_82A4B428` per dirty state bit. This IS a RAGE `grcDrawPool`
  flush, and the crash-trace claim that it leads to host
  `DrawPrimitiveUP → ProcDrawPrimitiveUP` is correct only in aggregate:
  one of the dirty-state handlers (likely `sub_82A4B148` or the PM4
  draw emitter downstream of it) is the true entry into the host draw
  queue. What `sub_82A3DAB0` confirms is that **the engine's state
  commit path runs without any `SetVertexDeclaration` call** — the
  engine operates purely on Xenos register state (fetch constants +
  `vfetch` shader encoding), and the runtime's `g_pipelineState
  .vertexDeclaration` is still whatever it was (nullptr) from boot.

## Evidence Against

### 1. Not every `vfetch` binds a complete stream

- `VertexFetchInstruction` (`shader_code.h:197-231`) has fields for
  format, stride-per-fetch, offset, and prefetch count. For a
  well-formed vertex shader these map cleanly to D3D stream slots — but
  RAGE sometimes composes multiple `vfetch`es into a single stream
  (e.g., skinning), and the `constIndex` field (which GPU fetch
  constant to read) is the actual key. The **stream slot** is NOT in
  the shader; it is in the fetch constant register set by the engine at
  bind time. A per-shader default that guesses "stream 0 for everything
  in the main bucket, stream 1 for the instance bucket, ..." may not
  match engine reality for complex shaders.
- However: the `DrawQuadFP` path at issue is deliberately simple
  (screen-space quad with baked colours + UV). For this path the
  shader-derived layout is sufficient. A general fix would need the
  runtime to also track live vertex-fetch constants (Xenia already does
  this — `register_file.h` + `vertex_bindings_` population at draw
  time).

### 2. Format width needs a reconstruction step

- `VertexElement` encodes `address` (byte offset inside the vertex),
  `usage`, `usageIndex` but NOT the element's type width. The width is
  encoded in the `format` field of the corresponding `vfetch`
  instruction. Deriving a proper `GuestVertexElement { stream, offset,
  type, usage, usageIndex }` requires correlating the element's
  `address` key with the `vfetch` that consumes it (the recompiler
  already does this at `shader_recompiler.cpp:216-250`). So the data is
  present, just requires a small extension of XenosRecomp's parser.

### 3. Runtime-side: `sub_82A3DAB0` does not bind the vertex shader
   pointer that LibertyRecomp's `g_pipelineState` tracks

- The PPC recomp for `sub_82A3DAB0` shows dirty-state bits at offsets
  {1920, 6016, 10368, 10444, 10528, 10548, 10596, 10680} of the
  draw-pool struct — none of those maps obviously to
  `g_pipelineState.vertexShader`. The shader pointer most likely enters
  via `sub_82A4ACF0` (another commit handler). Before we can "look up
  the shader's default decl" at draw time, we first need to be sure the
  bound vertex shader is visible to the runtime at the
  `ProcDrawPrimitiveUP` moment. That visibility itself is a separate
  prerequisite (see `02-missing-api-hook.md` and
  `05-grcdevice-default-decl.md`).

## Fix Design (architectural, no code)

### A. Extend XenosRecomp to emit per-shader input layout metadata

1. In `tools/XenosRecomp/XenosRecomp/shader_recompiler.cpp` where the
   code currently iterates `vertexShader->vertexElementCount` and
   builds the HLSL input struct, **also** populate a parallel
   `std::vector<VertexElement> exportedElements` on the
   `ShaderRecompiler` instance.
2. Second pass: for each exported element, find the `vfetch` whose
   `constIndex` / `address` pair matches, and read `format` + `stride`
   + `offset` from that `VertexFetchInstruction`. Produce a record
   isomorphic to `LibertyRecomp/gpu/video.h::GuestVertexElement
   { stream, offset, type, method, usage, usageIndex }`.
3. In `tools/XenosRecomp/XenosRecomp/main.cpp` where the
   `ShaderCacheEntry` initializer is printed
   (`main.cpp:217-219`), emit an additional per-entry
   `defaultVertexElements` blob (offset+count into a shared byte
   array, same packing pattern as dxil/spirv/air).
4. Extend `LibertyRecompLib/shader/shader_cache.h::ShaderCacheEntry`
   with `uint32_t vertexElementsOffset; uint16_t vertexElementCount;
   bool isVertexShader;` fields (or store a separate parallel table
   keyed by hash).

### B. Runtime: hook CreateVertexShader and pre-build default decl

5. Add a `GuestVertexDeclaration* defaultVertexDeclaration = nullptr;`
   member to `GuestShader` in `LibertyRecomp/gpu/video.h:422-435`.
6. Hook GTA IV's `CreateVertexShader` equivalent (needs symbol
   resolution — candidates live around RAGE's `grmShaderFactory`; see
   strings `"common:/shaders"` at 0x82005c1 referenced by
   `sub_821B3CE8`/`sub_821D6140`).
7. In that hook, after the existing `CreateShader()` path,
   call `CreateVertexDeclarationWithoutAddRef()` with the shader's
   metadata-derived element array, and cache the resulting
   `GuestVertexDeclaration*` on `shader->defaultVertexDeclaration`.

### C. Runtime: substitute when none bound at draw time

8. In `LibertyRecomp/gpu/video.cpp::ProcDrawPrimitiveUP` (line 5902)
   and `FlushRenderStateForRenderThread` — and most importantly in
   `CreateGraphicsPipelineInRenderThread` (5220) BEFORE
   `SanitizePipelineState(pipelineState)` at 5240 — check
   `if (pipelineState.vertexDeclaration == nullptr && pipelineState
   .vertexShader != nullptr && pipelineState.vertexShader
   ->defaultVertexDeclaration != nullptr)`, and substitute. This is
   the architectural fall-through: we always have a valid input layout
   for a bound vertex shader, because the shader intrinsically knows
   its inputs.
9. Simultaneously set `pipelineState.vertexStrides[0] = shader
   ->defaultVertexStride` (derived during the same XenosRecomp pass
   from the `vfetch.stride` of stream 0) so the render pipeline knows
   the stride for `DrawPrimitiveUP` which carries it inline.

### D. Why this is NOT a band-aid

- A band-aid is "early return if nullptr" or "fake a minimal
  FVF-style layout at the deref site". The architectural fix above
  restores information that WAS in the input to codegen and was
  discarded. It leverages data the tool already has. It does not
  mask the problem — it provides the correct value.

## Prerequisites

1. **Confirm the shader bound to the `DrawQuadFP` path.** Best approach:
   add a one-shot logger in
   `LibertyRecomp/gpu/video.cpp::FlushRenderStateForRenderThread`
   printing `g_pipelineState.vertexShader` hash when
   `vertexDeclaration == nullptr`. Cross-reference against
   `g_shaderCacheEntries[]` to identify the filename (likely
   `rage_shaders/gta_im/gta_im_vs0.bin` or similar). This verifies the
   format matches 36 bytes = POSITIONT+COLOR+UV (actually
   `float4 POSITIONT + float2 ? + D3DCOLOR + float2 UV` = 16+8+4+8 =
   36 bytes — consistent with RAGE's FP-vertex struct).
2. **Resolve GTA IV's CreateVertexShader entry point.** The hook must
   run when the game registers its shader with the RAGE
   `grcVertexShader` object, not at codegen. Candidate: the factory
   that dispatches on the `"common:/shaders"` content-store path at
   `sub_821B3CE8`. Without this symbol, step B.6 above can still be
   done lazily (on-demand at first draw) — we compute the default decl
   inside `ProcSetVertexShader` (or equivalent RAGE path) if
   `shader->defaultVertexDeclaration == nullptr`.
3. **Confirm the runtime sees the vertex shader at draw time.** Related
   to `02-missing-api-hook.md` — if GTA IV never binds the
   `GuestShader` to `g_pipelineState.vertexShader`, step C.8 cannot
   detect what the default decl should be. Must verify first.
4. **Add FetchOpcode::VertexFetch format-width reconstruction** (minor
   XenosRecomp change — see Evidence Against #2).

## Risks

- **Incorrect stream assignment.** Multi-stream shaders (skinning,
  instancing) need the guest engine's fetch-constant state to decide
  which stream each element lives on. For the UP/HUD crash path, all
  elements are stream 0 (DrawPrimitiveUP is by definition single-
  stream), so the risk is zero for THAT crash but non-zero for future
  multi-stream rendering.
- **Stride computation.** `DrawPrimitiveUP` hands the runtime a
  `vertexStreamZeroStride` (see `ProcDrawPrimitiveUP` at
  `video.cpp:5885`), which is authoritative. We should use THAT as
  stride, not whatever XenosRecomp derived. The shader-derived layout
  is only for the input-element TYPES and offsets.
- **`vertexStreams[16]` coverage.** `GuestVertexDeclaration
  ::vertexStreams[i]` tells `SanitizePipelineState` which slots carry
  real data (`video.cpp:5115`). A shader-derived default sets only the
  streams the shader actually reads — which is strictly a subset of
  what a custom `CreateVertexDeclaration` call would set, so the
  subsequent `vertexStrides[i] = 0` loop is safe.
- **Shader cache invalidation.** Changing the `ShaderCacheEntry` schema
  forces a full shader recompile (regeneration). The codegen command
  documented in `docs/BUILDING.md:217` already requires this — so
  manageable, but requires a cache-version bump.
- **Rexglue ABI mismatch.** `ShaderCacheEntry` is shared between the
  generator and the runtime. Both must agree on the new schema.
  `glue/rexglue-sdk-main/gta4-recomp/generated/` recomp files don't
  reference `ShaderCacheEntry` directly (it's a LibertyRecompLib-only
  struct) so no rexglue regeneration is required — but we must keep
  field ordering `const` and add new fields at the end to avoid ABI
  risk.

## Alternative Approaches (ranked by proximity to root cause)

1. **Default-decl-per-vertex-shader via runtime vfetch parsing
   (instead of XenosRecomp export).** Same idea but done at
   CreateVertexShader hook time by parsing the Xenos bytecode blob
   directly (`VertexShader*` struct at `shader.h:72-78` is already
   defined). Advantages: no XenosRecomp schema change, works for
   shaders the codegen hasn't seen, matches Xenia's approach more
   closely. Disadvantages: duplicates parsing logic between tool and
   runtime; adds first-draw latency.
2. **Wire up the RAGE `grcDrawPool → grcViewport → grcDevice
   defaultVertexDeclaration` chain.** Alternative root-cause analysis:
   RAGE does actually track a vertex format on the draw pool, but
   LibertyRecomp isn't plumbing it into `g_pipelineState`. See
   `05-grcdevice-default-decl.md`. This is a COMPLEMENTARY hypothesis:
   if the game really does have a pre-built `grcVertexDeclaration`
   object, we should use that directly rather than derive one.
3. **Hook the actual RAGE DrawPrimitiveUP entry point.** If GTA IV has
   its own `grcDrawCommand::DrawPrimitiveUP` (non-D3D9 entry point),
   hooking it with a wrapper that binds a per-shader default decl
   before forwarding would work without any XenosRecomp changes. See
   `02-missing-api-hook.md`.
4. **RAGE `im` / `grcBegin` / `grcEnd` batcher hook.** GTA IV's HTML
   renderer may be using the RAGE immediate-mode batcher (gta_im
   shaders, strings `"common:/shaders"` etc.). Identifying the
   immediate-mode flush function and wiring a default decl into it is
   narrower than deriving from bytecode but loses generality.
5. **REJECTED — null-check the deref.** Early-return at
   `video.cpp:5115` silently drops the draw. The crash vanishes but
   every HUD/text surface is invisible. This is the definition of
   band-aid.
6. **REJECTED — hardcode a POSITIONT FVF-style default decl.**
   Assumes any null decl should be (POSITIONT, COLOR, TEX0). Works for
   the HUD but masks any other shader with a different layout that
   hits the same nullptr path. Silent wrong-rendering > crash.

## Verdict

**Validated as the architecturally-correct root-cause fix for this
specific crash**, with important caveats:

- The shader IS the authoritative source of vertex-input metadata on
  Xenos, and XenosRecomp is already parsing it. Not exposing that
  metadata to the runtime is a codegen-to-runtime information gap that
  the fix closes.
- BUT we first need to confirm the runtime actually sees
  `g_pipelineState.vertexShader != nullptr` at the moment of crash
  (prerequisite #3 above). If GTA IV never binds the `GuestShader` to
  the pipeline state, the default-decl fallback has nothing to fall
  back to, and the real root cause is instead in the binding path
  (see `02-missing-api-hook.md`).
- The shortest-path variant is Alternative #1 (runtime vfetch parsing
  at `CreateVertexShader` hook time) — avoids XenosRecomp schema
  churn, ships faster, is Xenia-proven.
