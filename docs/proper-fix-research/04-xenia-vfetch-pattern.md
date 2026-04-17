# 04 — Xenia's vfetch-derived vertex format pattern

Agent 4 of 10. Research date: 2026-04-17.

## TL;DR

Xenia does **not** use a host-API vertex declaration at all. It never builds a D3D12 `INPUT_LAYOUT` or Vulkan `VkVertexInputAttributeDescription` for Xbox 360 draws. Instead every translated shader fetches vertex data itself from a 512 MB bindless raw buffer view of guest RAM. The format, stride and offset of every attribute are **recovered from the `vfetch_full` / `vfetch_mini` microcode instructions inside the vertex shader**, not from any `IDirect3DVertexDeclaration9` the guest may or may not have set.

This is the architecturally clean answer to the LibertyRecomp crash. The hypothesis ("extract vertex format from the Xenos vertex shader's vfetch instructions") is confirmed — that is literally how the reference emulator works. A direct port is non-trivial, but the pattern is real and authoritative.

## 1. Where it lives

Primary references inside the Xenia tree (master branch, github.com/xenia-project/xenia):

- `src/xenia/gpu/ucode.h` — `VertexFetchInstruction` (96-bit / 3-dword instruction)
- `src/xenia/gpu/xenos.h` — `VertexFormat` enum (what maps to FLOAT4 / D3DCOLOR / etc.)
- `src/xenia/gpu/shader.h` — `Shader::VertexBinding` + `Shader::VertexBinding::Attribute`
- `src/xenia/gpu/shader.cc` / `shader_translator.cc` — `Shader::GatherVertexFetchInformation`, `ParseVertexFetchInstruction`
- `src/xenia/gpu/d3d12/d3d12_command_processor.cc` — consumes `vertex_bindings()` at draw time, binds the shared-memory raw SRV

## 2. The ucode instruction (the single source of truth)

`VertexFetchInstruction` is three 32-bit words. Word-by-word (confirmed from `ucode.h`):

**Word 0**
|field|bits|
|-|-|
|opcode|5|
|src register|6|
|src addressing mode|1|
|dst register|6|
|dst addressing mode|1|
|must-be-one|1|
|constant index|5|
|constant index selector|2|
|prefetch count|3|
|source swizzle|2|

**Word 1**
|field|bits|
|-|-|
|dst swizzle|12|
|format component flag|1|
|numeric format flag|1|
|signed repeating fraction mode|1|
|index rounded|1|
|**vertex format**|**6**|
|reserved|2|
|exp adjust|6|
|is mini-fetch|1|
|is predicated|1|

**Word 2**
|field|bits|
|-|-|
|stride (dwords)|8|
|offset (dwords)|23|
|predicate condition|1|

So every `vfetch_full` carries its own format (6-bit enum), its stride in dwords, and its offset in dwords. A matching `vfetch_mini` reuses the previous full fetch's base/stride/rounding and changes only offset/format/swizzle. There is no separate "vertex declaration" in Xenos hardware.

## 3. The VertexFormat enum (xenos.h)

Verified values (the ones relevant to the GTA IV HTML-text crash are bolded):

|enum|name|bits|D3D9 name|
|-|-|-|-|
|0|kUndefined|-|-|
|**6**|**k_8_8_8_8**|**32**|**D3DDECLTYPE_D3DCOLOR / UBYTE4N**|
|7|k_2_10_10_10|32|D3DDECLTYPE_DEC3N|
|16|k_10_11_11|32|-|
|17|k_11_11_10|32|-|
|25|k_16_16|32|SHORT2/USHORT2|
|26|k_16_16_16_16|64|SHORT4/USHORT4|
|31|k_16_16_FLOAT|32|FLOAT16_2|
|32|k_16_16_16_16_FLOAT|64|FLOAT16_4|
|33|k_32|32|-|
|34|k_32_32|64|-|
|35|k_32_32_32_32|128|-|
|**36**|**k_32_FLOAT**|**32**|**D3DDECLTYPE_FLOAT1**|
|**37**|**k_32_32_FLOAT**|**64**|**D3DDECLTYPE_FLOAT2**|
|**38**|**k_32_32_32_32_FLOAT**|**128**|**D3DDECLTYPE_FLOAT4**|
|57|k_32_32_32_FLOAT|96|D3DDECLTYPE_FLOAT3|

Mapping the captured LibertyRecomp stride (FLOAT4 + FLOAT2 + D3DCOLOR + FLOAT2 = 36 bytes = 9 dwords) to Xenos enums:

|offset bytes|D3D9 type|VertexFormat enum|
|-|-|-|
|0|FLOAT4 (pos)|38 (k_32_32_32_32_FLOAT)|
|16|FLOAT2|37 (k_32_32_FLOAT)|
|24|D3DCOLOR|6 (k_8_8_8_8)|
|28|FLOAT2 (uv)|37 (k_32_32_FLOAT)|

(Python-verified; stride dword count = 9.)

This is the exact set of `data_format` values the shader's `vfetch_full`s would carry if we compiled them from the original HLSL — every field in the declaration is 1:1 recoverable from ucode.

## 4. How Xenia turns the ucode into bindings

From `Shader::GatherVertexFetchInformation` (verbatim, `shader_translator.cc`):

```cpp
ParsedVertexFetchInstruction fetch_instr;
if (ParseVertexFetchInstruction(op, previous_vfetch_full, fetch_instr)) {
  previous_vfetch_full = op;
}
...
VertexBinding::Attribute* attrib = nullptr;
for (auto& vertex_binding : vertex_bindings_) {
  if (vertex_binding.fetch_constant == op.fetch_constant_index()) {
    assert_true(!fetch_instr.attributes.stride ||
                vertex_binding.stride_words == fetch_instr.attributes.stride);
    vertex_binding.attributes.push_back({});
    attrib = &vertex_binding.attributes.back();
    break;
  }
}
if (!attrib) {
  assert_not_zero(fetch_instr.attributes.stride);
  VertexBinding vertex_binding;
  vertex_binding.binding_index = int(vertex_bindings_.size());
  vertex_binding.fetch_constant = op.fetch_constant_index();
  vertex_binding.stride_words = fetch_instr.attributes.stride;
  vertex_binding.attributes.push_back({});
  vertex_bindings_.emplace_back(std::move(vertex_binding));
  attrib = &vertex_bindings_.back().attributes.back();
}
attrib->fetch_instr = fetch_instr;
```

Key algorithm:
1. Walk every ucode instruction once, during shader compile (`AnalyzeUcode`).
2. For each `vfetch_full`, parse out `{fetch_constant_index, stride_words, data_format, offset, exp_adjust, is_signed, is_integer, signed_rf_mode, is_index_rounded}`.
3. Group by `fetch_constant_index` into `Shader::VertexBinding`. All fetches that share a fetch constant form one binding; their attributes are accumulated.
4. `vfetch_mini` instructions inherit base/stride/rounded flags from the previous `vfetch_full` in the shader; only offset/format/swizzle change.
5. The resulting `vector<VertexBinding>` is cached on the shader object for the life of the translation.

Shader-level data, not draw-call state. The game can call `DrawPrimitiveUP`, `DrawIndexedPrimitive`, or any other Xenos draw packet — the vertex layout has already been extracted from the shader bytecode at shader-compile time.

## 5. How the bindings reach the GPU

Two-phase at draw time (`d3d12_command_processor.cc`):

1. `regs.GetVertexFetch(vfetch_index)` reads the **fetch constant** the guest wrote into GPU registers. Fetch constants carry `{type, base address, size, stride, format, endian}` and live at XE_GPU_REG_SHADER_CONSTANT_FETCH_XX_0..1. This is the *memory pointer*, not the layout.
2. `shared_memory_->RequestRange(base, size)` and `WriteRawSRVDescriptor(...)` make the guest RAM resident as a bindless raw buffer.
3. The translated shader (DXBC or SPIR-V) contains load instructions that use the layout from `Shader::VertexBinding` to pull each attribute out of that raw buffer. No D3D12 `IASetVertexBuffers` / input layout is ever called.

So Xenia's two-level split is:
- **Layout (stride + per-attribute format/offset)** → derived from shader vfetch ucode at shader-compile time.
- **Pointer (base address + byte size)** → from the fetch-constant register (set by the ring buffer packets `SET_SHADER_CONSTANT` or `LOAD_ALU_CONSTANT`, which is what both `DrawPrimitiveUP` and `DrawIndexedPrimitive` end up writing).

The guest setting or not setting `IDirect3DVertexDeclaration9` is irrelevant to Xenia — the D3D9 declaration existed only as an API contract on real Xenon and was discarded by the driver when it compiled the vertex shader. Xenia goes straight to the GPU-level truth.

## 6. Does this port to LibertyRecomp?

**Architecturally yes, but expensive.** LibertyRecomp is a static recompiler of PPC + XenosRecomp of HLSL; it does **not** have Xenia's on-the-fly ucode translation path. Three options, ordered by cost:

1. **Full Xenia-style port (correct, large).** Add a ucode analysis pass to XenosRecomp that emits the equivalent of `Shader::VertexBinding` into a host-side metadata table keyed by shader hash. At runtime, on a `DrawPrimitiveUP` with no current host vertex declaration, look up the currently-bound VS in that table and synthesise a vertex declaration on the fly. This moves LibertyRecomp toward Xenia's architecture but requires shipping per-shader binding tables and a runtime declaration builder.
2. **Link-time HLSL introspection (medium).** XenosRecomp already has each VS as HLSL. A build-step tool can walk the HLSL's input struct (it still references `TEXCOORD`/`POSITION` semantics) and emit the same metadata table as option 1, without writing a ucode disassembler. Cheaper than option 1; still proper fix.
3. **Shader-hash → declaration cache populated at first proper draw (small, but hacky and out of scope for "proper fix").** Capture declarations the first time the game uses them correctly, key by currently-bound VS, reuse when missing. This is a band-aid; the rules say no band-aids.

Option 2 is the minimum-footprint proper fix: it produces exactly the same logical output as Xenia's `Shader::VertexBinding` without needing a full ucode analyzer. Option 1 is the Xenia-faithful path.

## 7. GTA IV on Xenia — context for our crash

From xenia-project/game-compatibility#427 (TitleID **545407F2** — GTA IV):

- Status: "playable" with "significant graphics issues".
- Reported: "3D scenes are no longer drawn" after a specific commit; FMVs + loading screens are clean.
- No specific report of HTML-text breakage. This is consistent: Xenia never hits the `vertexDeclaration == nullptr` failure mode at all, because it doesn't use vertex declarations. The GTA IV HTML-text path that crashes us in LibertyRecomp just works as a normal draw on Xenia.
- Also see xenia-canary/xenia-canary#649 ("Graphics device lost issue GTA IV") — unrelated (device-removal, not vertex layout).

This is direct evidence that **deriving the vertex format from the vertex shader is sufficient for GTA IV** — Xenia does exactly that and GTA IV renders geometry without a declaration ever crossing the abstraction boundary.

## 8. Concrete next steps (research only — no implementation)

1. Enumerate the VS shaders that the HTML-text codepath in GTA IV actually binds (from `kernel/render_hooks.cpp` or wherever we trace `D3DDevice_DrawVerticesUP`).
2. Diff their HLSL input structs against the captured 36-byte stride. Expectation: position (FLOAT4) + UV0 (FLOAT2) + color (D3DCOLOR) + UV1 (FLOAT2). This matches both the trace and the RAGE scaleform-style text path.
3. Decide: option 1 (ucode analyzer) vs option 2 (HLSL input-struct scraper) for the proper fix. Option 2 recommended as least-invasive proper-fix that matches Xenia's data model.
4. Prove round-trip: feed the VS's HLSL input-struct layout into a synthesised `GPUVertexDeclaration` and render one HTML text quad. Success criterion is identical to Xenia: no declaration from the guest, format recovered from the shader, pixels on screen.

## 9. Sources

- Xenia source: https://github.com/xenia-project/xenia (files listed in §1).
- Xenia GPU overview: https://xenia-emulator.com/knowledge-base/gpu-emulation/
- Xenia render-target / shared-memory blog: https://xenia.jp/updates/2021/04/27/leaving-no-pixel-behind-new-render-target-cache-3x3-resolution-scaling.html
- GTA IV compatibility: https://github.com/xenia-project/game-compatibility/issues/427 , https://xenia-canary.github.io/GrandTheftAutoIV/
- GPU device-lost issue (not related): https://github.com/xenia-canary/xenia-canary/issues/649
- Historical GPU feasibility post: http://www.noxa.org/blog/2011/02/23/building-an-xbox-360-emulator-part-2-feasibilitygpu/
