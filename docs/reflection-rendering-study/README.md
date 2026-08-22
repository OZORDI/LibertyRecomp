# GTA IV reflection-resolution scaling specification

## Scope

This note covers the Xbox 360 version of GTA IV used by LibertyRecomp and asks two questions:

1. What reflection techniques does the original renderer use for mirrors, vehicles, world materials, glass, interiors, wet surfaces, and water?
2. What is the safest route to higher-resolution reflections or a modern replacement?

This document records the research and implementation contract for the GTA4-native RexGlue renderer. It does not require or seek parity with the separate legacy `LibertyRecomp/gpu/video.cpp` renderer: the native renderer owns Vulkan images, attachments, resolves, samplers, and mip generation directly. Retail code is used only to identify GTA IV's reflection families, capture camera, and authored mip sequence. Missing-water and fall-through-floor investigations remain out of scope.

Evidence is labelled as follows:

- **Proven:** directly visible in the retail Xbox 360 disassembly or current LibertyRecomp source.
- **Strong inference:** multiple local symbols and established rendering behavior agree, but the exact shader has not yet been decoded.
- **Hypothesis:** plausible and testable, but it needs a GPU capture or live trace before implementation.

## Implemented GTA4-native resolution controls

The Vulkan `gta4-native` path now classifies only the six proven named resources at `sub_828DC7F0`, leaves their guest-visible dimensions unchanged, and sends host-only logical/physical metadata to the render worker. Host surface and resolved-texture allocation, viewport, scissor, clear rectangles, partial resolve rectangles, destination points, and per-draw half-pixel constants use the physical extent. Failed oversized allocations retry at the original extent.

The restart-required controls are:

| Cvar | Values | Default |
|---|---|---|
| `gta4_reflection_resolution` | `original`, `1080p`, `full` | `1080p` |
| `gta4_reflection_resolution_cap` | `1080p`, `1440p`, `display` | `1440p` |
| `gta4_mirror_reflection_resolution` | `inherit`, `original`, `1080p`, `full` | `inherit` |
| `gta4_water_reflection_resolution` | `inherit`, `original`, `1080p`, `full` | `inherit` |
| `gta4_environment_reflection_resolution` | `inherit`, `original`, `1080p`, `full` | `inherit` |
| `gta4_reflection_aa` | `original`, `off`, `2x`, `4x` | `original` |

`gta4_reflection_capture_distance` is runtime-adjustable rather than restart-required. Its values are `original` (40), `extended` (60), and `far` (80), with `original` as the default. It changes only the exterior environment-reflection camera at the proven `CRenderPhaseReflection` projection call; mirror, water, and interior camera ranges are untouched.

The exact physical mappings are:

| Selection | Mirror/water | Environment paraboloid |
|---|---:|---:|
| `original` | 320x180 | 256x256 |
| `1080p` | 1920x1080 | 1024x1024 |
| `full` with the default 1440p cap | 2560x1440 | 2048x2048 |

Display-relative planar targets use display height and retain a 16:9 capture instead of stretching across an ultrawide display. The square environment target is 1024 through 1080p, 2048 above 1080p through 2160p, and 4096 only above 2160p. This implementation does not patch guest resource descriptors and does not change the legacy `video.cpp` renderer.

Reflection AA is a native attachment policy, independent of resolution. `original` preserves the title's requested count; `off`, `2x`, and `4x` request 1, 2, or 4 Vulkan samples. The host chooses from the intersection of color, depth, stencil, sampled-color, and sampled-depth capabilities, falling from 4x to 2x to 1x as a matched family. Captures render multisampled and the existing native resolve writes a single-sample texture before it is sampled or filtered.

The environment color texture now allocates a complete physical mip chain. GTA IV still renders its authored levels first. When the final guest-defined level resolves, GTA4-native generates only the extra physical tail with linear Vulkan blits:

| Physical base | Guest-authored levels | Physical levels | Native tail |
|---:|---:|---:|---:|
| 256 | 9 | 9 | none |
| 1024 | 9 | 11 | levels 9-10 |
| 2048 | 9 | 12 | levels 9-11 |
| 4096 | 9 | 13 | levels 9-12 |

The Vulkan sampler exposes the complete physical chain for the live environment map. This is native resource-quality behavior; it does not add an emulator resolution scale or translate legacy GPU state.

The implementation is Release-build validated but still requires a fresh in-game run and GPU capture at each AA/resolution combination. An older process cannot validate a newly linked renderer dylib.

For one 32-bit color and one 32-bit depth allocation per family, with a full color mip chain on the environment map, the three families consume approximately 1.46 MiB at Original, 40.97 MiB at 1080p, and 93.58 MiB at Full with the 1440p cap before MSAA, alignment, transient copies, and backend overhead. A 2048-square environment capture with 4x MSAA is therefore experimental rather than a default.

The figures are reproduced by this Python calculation:

```python
MIB = 1024 * 1024

def mip_texels(extent):
    total = 0
    while extent:
        total += extent * extent
        extent //= 2
    return total

for name, width, height, environment in (
    ("Original", 320, 180, 256),
    ("1080p", 1920, 1080, 1024),
    ("Full (1440p cap)", 2560, 1440, 2048),
):
    planar = 2 * 2 * width * height * 4
    paraboloid = (mip_texels(environment) + environment * environment) * 4
    print(name, f"{(planar + paraboloid) / MIB:.2f} MiB")
```

## Executive conclusion

GTA IV does not use one reflection system. The retail Xbox 360 binary contains separate render phases for water reflection, mirror reflection, general reflection, and interior reflection. At least four families are visible:

- A dedicated low-resolution planar-style mirror pass with a 320x180 color target and matching depth target.
- A separate 320x180 projected water-reflection color/depth pair.
- A shared 256x256 live dual-paraboloid environment map. Local shader disassembly proves it feeds deferred vehicle paint, wet-road/puddle lighting, vehicle glass, and several forward reflective materials.
- True cube-map material variants coexist with the live 2D paraboloid path; increasing the live map cannot improve authored/static cubemap assets.

The current worktree also contains an attempted fullscreen SSR path, but it is not a working replacement. Its graphics pipelines are never created, its inverse matrices are identity placeholders, it has no material roughness or reliable normal input, and its shader performs linear screen-space marching despite the “Hi-Z” comment. `ReflectionResolution` is currently configuration surface only; it is not connected to original reflection-target allocation.

The recommended route is therefore:

1. Instrument and preserve the original reflection families, then fix subresource resolve, format, and allocation correctness.
2. Scale the named targets directly in GTA4-native's Vulkan resource graph.
3. Restore correct reflective material/shader coverage.
4. Add material-aware SSR as a supplement, retaining the original maps as off-screen and rough-surface fallback.
5. Keep planar rendering for true mirrors. Treat hardware ray tracing as an optional future tier, never the baseline.

This gives a visible quality improvement early and avoids replacing known game behavior with a global, unstable post-process.

## What the Xbox 360 renderer actually contains

### Separate render phases

The retail RTTI/vtable excavation identifies distinct classes:

| Render phase | Retail evidence | Likely responsibility |
|---|---|---|
| `CRenderPhaseWaterReflection` | `gta_iv/xex_excavation_retail/method_symbols.txt` around lines 4325-4328 | Reflected scene rendered for water |
| `CRenderPhaseMirrorReflection` | Same file around lines 4326 and 4329-4330 | Dedicated mirror view |
| `CRenderPhaseReflection` | Same file around lines 4335, 4337, and 4339-4340 | Outdoor/dynamic environment map |
| `CRenderPhaseInteriorReflection` | Same file around lines 4334, 4336, 4338, and 4341 | Interior-specific environment map |

This separation is **proven**. It is important architecturally: water, mirrors, and vehicle/world environment reflections must not be treated as one texture or one pass.

### Dedicated mirrors: 320x180 render-to-texture

`sub_822BCC20` creates resources named `MIRROR_DT` and `MIRROR_RT`. Both calls pass width 320 and height 180. It then loads the `mirror` shader and resolves handles for:

- `reflectiontexture`
- `normbuffertexture`
- `mirrorParams`

Evidence:

- Retail assembly: `gta_iv/xex_excavation_retail/default_decrypted.xex.asm`, lines 839485-839645.
- Recompiled body: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.12.cpp`, lines 64221-64440.
- The render-phase vtable: `gta_iv/xex_excavation_retail/vtables_with_addrs.txt`, around lines 13059-13075.

The allocation size and dedicated phase are **proven**. The phase functions `sub_82520D40`, `sub_82521490`, and `sub_82521628` cover visibility/list setup, reflected-camera and clip-plane setup, and execution. This makes a reflected-camera planar implementation **proven at the phase level**; a GPU capture should still verify all projection conventions.

The mirror pixel shader samples four adjacent projected 2D reflection texels and averages them. That fixed softening filter magnifies the already-low target resolution. A resolution upgrade should scale or retune the offsets rather than blindly preserving an increasingly wide screen-space blur. Mirror work is also visibility/content gated, so a stale mirror is not automatically proof of a fixed every-frame update cadence.

At four times the original linear dimensions, the pair would become 1280x720. Assuming one 32-bit color and one 32-bit depth surface, storage rises from about 0.44 MiB to about 7.03 MiB before alignment, MSAA, transient copies, and backend overhead. These figures are planning estimates, not observed allocation sizes.

### General environment reflection: 256x256 paraboloid map

`sub_822D93E8` conditionally creates `REFLECTION_MAP_DEPTH` and `REFLECTION_MAP_COLOUR`, each at 256x256. The same renderer contains:

- A `CRenderPhaseReflection` class.
- A separate `CRenderPhaseInteriorReflection` class.
- A render-phase name `paraboloid`.
- Shader parameters `ParabTexture` and `ReflectionParams`.
- A `paraboloid_draw` technique and `paraboloid_corona` pass.

Evidence:

- Target allocation: `gta_iv/xex_excavation_retail/default_decrypted.xex.asm`, lines 877855-877933.
- Reflection phase and `paraboloid` string: the same assembly around lines 1680358-1680371 and the render-phase vtables near lines 31485-31540.
- Deferred/reflection shader handles: the same assembly around lines 1622980-1622993.
- Sky paraboloid technique: the same assembly around lines 2630282-2630288.

The 256x256 target and paraboloid-named pipeline are **proven**. Local shader disassembly and binding code also prove that it is a shared 2D paraboloid environment source rather than a speculative association. `sub_822D93E8` stores `REFLECTION_MAP_COLOUR` in global `dword_830329B4`; `sub_822B08B0` forwards it into `sub_824F6118`; and `sub_824F78D8` resolves `ParabTexture`, `ReflectionParams`, `paraboloid_corona`, and `refMipBlur` handles.

The exterior phase projection in `sub_825225E8` uses a 90-degree field of view, a 0.1 near plane, and a 40.0 far plane. This is a separate quality limit: more pixels cannot recover distant geometry excluded from the capture. Exterior/interior/water passes appear designed to update on eligible frames, but their exact cadence remains unproven and should be logged rather than assumed.

The checked-out FusionFix research provides another important clue: it looks up `REFLECTION_MAP_COLOUR` by name, walks every level after the base level, and supplies a replacement mip/blur pass with per-level pixel offsets. It disables the PC game's original exterior/interior reflection mip/blur pass when its reflection-MSAA option is active. This is **proven for the PC port**, not automatically the Xbox 360 implementation, but it makes preservation of the reflection map's mip chain and roughness blur a first-class compatibility requirement rather than an optional polish step.

Evidence: `tools/GTAIV.EFLC.FusionFix-master/source/reflectionmsaa.ixx`, lines 63-70, 116-257, and 431-445.

At four times the original linear dimensions, this target pair would become 1024x1024. With an RGBA8 color target including a full mip chain plus one base-level D32 depth target, estimated storage is about 9.33 MiB before alignment, MSAA, staging, and backend overhead. The corresponding estimates are about 0.58 MiB at 256x256 and 2.33 MiB at 512x512.

### Proven consumers of the shared paraboloid map

The original renderer is hybrid even before any modern SSR is added:

| Surface/material | Proven original behavior | Upgrade implication |
|---|---|---|
| Opaque vehicle paint/body | Four deferred-lighting variants—directional/ambient with dry/wet permutations—fetch the 2D `ParabSampler`. Vehicle paint shaders do not each own a separate environment texture. | Scale and prefilter the shared map; preserve deferred bindings and wet/dry variants. |
| Wet roads and puddles | The wet deferred variants sample the same `ParabSampler`; `dReflectionParams` modulates the response. There is no evidence of a separate puddle RT or original screen-space reflection pass. | Do not replace wetness with a global SSR overlay. Feed SSR only through a material/roughness mask and keep the paraboloid fallback. |
| Vehicle glass | `gta_vehicle_vehglass_ps0.bin` exposes a 2D `EnvironmentSampler`; `sub_8250B190` binds the same global reflection map as `EnvironmentTex`. | Preserve a forward transparent path; ordinary opaque SSR composition is insufficient. |
| Reflective world/glass shaders | `gta_glass_*reflect` and `gta_reflect` use a 2D environment sampler with `VS_Transform_paraboloid`. | These benefit from the live map scale and correct mip filtering. |
| True cubemap shaders | `gta_cubemap_reflect` and `gta_normal_cubemap_reflect` perform cube fetches. | Audit cube assets, faces, resolves, and mip quality independently; the 256x256 live-map setting does not govern them. |

The four deferred shader binaries are `deferred_lighting_ps0.bin` through `deferred_lighting_ps3.bin` under `LibertyRecompLib/shader/rage_shaders/deferred_lighting/`. Their exported program names distinguish shadowed/unshadowed and dry/wet reflection variants. This is direct shader evidence, not a naming-only inference.

FusionFix's checked-out `visualsettings.dat` includes `heightReflect.width`, `heightReflect.height`, `heightReflect.specularoffset`, and `misc.CrossPMultiplier.GlobalEnvironmentReflection`. These affect capture/appearance behavior; the first two are not render-target pixel dimensions and must not be mistaken for the 256x256 allocation.

### Water reflection: a separate 320x180 pair

The same viewport setup function creates `WATER_REFLECTION_DEPTH` and `WATER_REFLECTION_COLOUR` at 320x180. That confirms water reflection has its own scene render rather than sharing the mirror target or the 256x256 general environment map.

Evidence: `gta_iv/xex_excavation_retail/default_decrypted.xex.asm`, lines 877703-877782.

The water shader binds a projected reflection texture together with reflection depth, normals, a surface texture, and projection/scale parameters, then perturbs the projected lookup. This is materially different from both the shared paraboloid map and a true mirror. Its scale and filtering need an independent quality control.

This document does not diagnose missing water rendering. Any future reflection scaler must nevertheless classify this pair separately so the water team can opt in without inheriting unrelated mirror or SSR behavior.

### Formats and MSAA: do not guess during implementation

The retail creation calls prove 32-bit surface arguments for the three color/depth pairs, but this research has not decoded every enum strongly enough to label the exact formats. RGBA8-like color and a 24/8-class depth/stencil format are plausible; they are not yet proven. Descriptor evidence and FusionFix's Xbox reflection-MSAA handling indicate that MSAA is likely involved, but the exact sample count still needs a live trace. Log the requested format, actual host format, sample count, resolve mode, mip, and face for each named resource before changing allocation sizes.

### Reflective material families

The game's shader/preset names show multiple consumers and shading models:

- `gta_cubemap_reflect`
- `gta_normal_cubemap_reflect`
- `gta_normal_spec_cubemap_reflect`
- `gta_reflect`, `gta_normal_reflect`, and `gta_spec_reflect`
- `gta_glass_reflect` and `gta_glass_normal_spec_reflect`
- `gta_mirror`
- `gta_ped_reflect`
- Many `gta_vehicle_spec_reflect*` variants covering alpha, damage, bump maps, UV bump maps, and two-layer materials

Evidence: `LibertyRecomp/gpu/sps_preset_table.h`, especially lines 72-145 and 179-199, plus retail strings in `gta_iv/xex_excavation_retail/all_strings_with_addrs.txt`.

This proves that reflection response is material-specific. It does not by itself prove which texture each shader samples. The working classification to validate is:

| Material family | Original source to verify | Modern treatment |
|---|---|---|
| True mirrors | Dedicated mirror color/depth pair | Keep planar; raise target resolution; optionally add selective RT later |
| Vehicle paint/chrome | Shared dynamic 2D paraboloid map plus specular/normal terms in deferred lighting | Restore original shader first; supplement with SSR; retain map fallback |
| Building/world reflective materials | Cube or paraboloid environment source depending on shader | Higher-resolution probes/maps; SSR only where the material mask opts in |
| Glass/windows | Environment reflection plus transparency/refraction | Forward transparent pass with probe fallback; SSR needs pre-transparent depth or a separate glass path |
| Pedestrian reflective variants | Environment source with skin/clothing-specific shader | Preserve original material behavior; use SSR conservatively |
| Wet roads/puddles | Wet deferred-lighting variants sample the shared paraboloid map and use reflection parameters | Material mask + environment fallback + SSR; avoid reflecting every opaque pixel |
| Water | Dedicated water reflection pass | Keep independently configurable and owned by the water renderer |

## Current LibertyRecomp state

### Native renderer boundary

The repository contains two historically separate renderer implementations:

- macOS desktop returns through the RexGlue/Graine build and selects the Vulkan-only `gta4-native` graphics plugin (`CMakeLists.txt`, `glue/CMakeLists.txt`, and `glue/rexglue-sdk-main/gta4-recomp/src/gta4_app.cpp`).
- Other builds may still contain the custom `LibertyRecomp/gpu/video.cpp` path.

This feature intentionally targets GTA4-native. It must not reuse `video.cpp` allocation structures, resolution-scale machinery, or shader translation. If the legacy renderer is retained for other products, it needs a separate design and test effort; it is not a parity requirement for this native feature.

### Backend correctness blockers found by audit

These are independent of the unused reflection-quality setting and should be resolved or ruled out with traces before claiming broad reflection fidelity:

1. **GTA4-native rejects nonzero cube faces during resolve.** It creates six-layer cube images and cube views, but `RecordResolve` rejects `destination_slice_or_face != 0`; its barriers and copy layers also use base array layer zero. Runtime-generated cubemap faces 1–5 therefore cannot resolve correctly.
2. **The legacy path loses cube/array identity in critical places.** Its RAGE RT factory creates six layers but records the resource as a plain texture, pending-copy behavior special-cases only `ArrayTexture`, and color framebuffer attachment does not select a per-face view. True cube-map work needs face-correct color views and copy/resolve routing.
3. **Legacy MRT support is incomplete.** `SetRenderTarget` supports only color slot zero, whereas GTA4-native captures and validates up to four render targets. A trace must determine whether any reflection/deferred variant needs auxiliary slots before relying on the legacy path.
4. **Legacy unknown formats silently fall back to RGBA8.** GTA4-native instead uses an explicit supported-format set and rejects unsupported resolves. Each reflection target's requested color/depth format and sample count must be inventoried before scaling.
5. **Legacy backing-size estimation is unsafe for larger targets.** `sub_82A55DC0` derives bytes per pixel from a format bitfield, performs 32-bit products, then aligns. Replace this with checked 64-bit block-geometry calculations before enabling the largest tier.
6. **Legacy shader misses can look like missing reflections rather than errors.** Cache misses create dummy shaders and later skip draws. The cache lists many reflection variants and `mirror_ps0`, but the audited DXIL fields are empty and `docs/SHADER_PIPELINE.md` still documents Windows output as empty. Required reflection shader misses should be surfaced explicitly, and every supported backend needs actual shader binaries before parity can be claimed.

The first two issues primarily threaten true cube-map resources, not the proven 2D paraboloid map. They still matter to the requested “all surfaces” scope. The format and backing-size issues directly constrain safe target upscaling.

### Original targets are created at guest-requested size

The RAGE render-target hook `sub_828BEC78` reads the guest width and height and creates host textures using those exact values. Cube targets use an array of six 2D images, while ordinary targets remain 2D. There is no reflection-specific scale in this path.

Evidence: `LibertyRecomp/gpu/video.cpp`, lines 9377-9455.

The same path forces `mipLevels = 1` for type-1/type-3 render-target textures. If the Xbox reflection resource reaches this hook through that branch, the original reflection mip/blur behavior cannot be represented. Live tracing must confirm the resource type and call route, but this is a concrete fidelity risk independent of base resolution.

The generic texture path similarly assigns the requested width and height directly to `RenderTextureDesc` and the guest resource metadata.

Evidence: `LibertyRecomp/gpu/video.cpp`, lines 3979-4059.

### The legacy `ReflectionResolution` setting remains disconnected

The configuration declares `EReflectionResolution::{Eighth, Quarter, Half, Full}` and defaults to `Half`. Low-end defaults select `Quarter`. Repository-wide references currently stop at declaration, serialization, and the low-end-default assignment; no render-target allocator or render phase consumes the value.

Evidence:

- `LibertyRecomp/user/config.h`, lines 153-159.
- `LibertyRecomp/user/config_def.h`, line 116.
- `LibertyRecomp/user/config.cpp`, lines 371-377.
- `LibertyRecomp/gpu/video.cpp`, line 2065.

Therefore changing the legacy option cannot presently sharpen original GTA IV reflections. The GTA4-native path instead consumes the explicit cvars documented above.

### Current SSR is scaffolding, not a usable replacement

The worktree contains `ssr_raytrace_ps.hlsl`, `ssr_composite_ps.hlsl`, and `PostProcessRenderer::ApplySSR`, but several blockers are visible:

- `m_ssrRaytracePipeline` and `m_ssrCompositePipeline` are declared and checked but never created in `CreatePipelines`.
- The code explicitly calls the path a stub and returns if either pipeline is absent.
- Only SPIR-V SSR binaries are included; there are no corresponding DXIL or Metal SSR shader includes/assets in the current tree.
- Inverse view and projection matrices are filled with identity placeholders.
- The shader reconstructs normals from neighboring depth samples; the declared normal texture is not used.
- There is no roughness/material texture, even though constants mention a roughness cutoff.
- Its depth linearization and `depth >= 1.0` sky rejection assume conventional near-to-far depth, while LibertyRecomp explicitly enables inverted/reverse-Z behavior in its render and upscaler paths.
- The ray marcher performs fixed linear stepping through the full-resolution depth texture; no hierarchical depth pyramid is created or sampled.
- No temporal history, motion-vector reprojection, disocclusion rejection, or denoiser is present.
- The composite blends by hit confidence alone, so it has no reliable way to distinguish chrome, wet asphalt, skin, matte walls, glass, or UI.
- The call site passes the active render target as both the input scene color and output, which needs explicit backend-safe feedback handling or ping-pong storage.
- The call is gated on camera projection validity, not on a dedicated SSR enable/quality option.

Evidence:

- Pipeline setup: `LibertyRecomp/gpu/postprocess_renderer.cpp`, lines 357-541.
- SSR body: the same file, lines 1458-1602.
- Call site: `LibertyRecomp/gpu/video.cpp`, lines 3517-3597.
- Shader inputs and traversal: `LibertyRecomp/gpu/shader/hlsl/ssr_raytrace_ps.hlsl`.

Do not try to “turn on” this path as-is. It would either remain inactive or produce physically and materially incorrect fullscreen reflections once the missing pipelines were added.

### Reflective shader coverage may be a fidelity blocker

The generated SPS table maps the dedicated cubemap, world, glass, and pedestrian reflection families to correspondingly named FXC shaders. In contrast, all `gta_vehicle_spec_reflect*` variants currently map to `gta_default`.

Evidence: `LibertyRecomp/gpu/sps_preset_table.h`, lines 179-199.

That mapping does not prove the live game is currently drawing vehicles with `gta_default`; the table may be installer metadata or a fallback path rather than the final shader selected at runtime. It is nonetheless a high-priority trace target. If active, increasing a reflection render target will not restore vehicle reflections that the material shader never samples.

## Recommended implementation strategy

### Phase 0: capture and classify before changing quality

Add bounded diagnostics first:

1. Log reflection-related target creation with guest name when available, requested dimensions, format, MSAA, type, caller/LR, guest handle, and host texture identifier.
2. Tag `MIRROR_*`, `REFLECTION_MAP_*`, `WATER_REFLECTION_*`, and any interior reflection targets in graphics debuggers.
3. Record set-target, viewport/scissor, clear, draw count, resolve, and sample events for each tagged resource.
4. Capture one exterior daytime frame, one rainy night frame, one vehicle close-up, one interior, one actual mirror, and one glass-heavy scene.
5. Add debug views for reflection source, depth, material mask, and final composite.

Useful trace points:

- Guest resource initialization: `sub_822BCC20` and `sub_822D93E8`.
- RAGE target creation hook: `PPC_FUNC_HOOK(sub_828BEC78)` in `LibertyRecomp/gpu/video.cpp`.
- Target binding and resolve paths: `SetRenderTarget`, `ProcSetRenderTarget`, `D3DDevice_EndTiling`, `g_pendingResolves`, and the native graphics `RecordResolve` path.
- Mirror render phase: retail `CRenderPhaseMirrorReflection` functions `sub_82520D40`, `sub_82521490`, and `sub_82521628`.
- General/interior phases: retail functions in the `sub_82521FF0` through `sub_82522FE0` range identified by `method_symbols.txt`.

Exit criterion: a table showing, for each surface family, exactly which render target and shader is sampled in a captured frame. Do not implement a global quality override before this table exists.

### Phase 1: upscale the original technique

This is the best first deliverable because it improves sharpness while preserving GTA IV's visibility rules, art direction, off-screen coverage, and material bindings.

#### Preferred architecture: host-side render-target scale

Keep guest-visible dimensions and coordinates unchanged, but allocate a larger host image for tagged reflection resources and scale their viewport, scissor, resolve rectangles, and texel-dependent offsets in the host renderer. This follows the same broad model as emulator internal-resolution scaling: guest logic continues to think the target is 320x180 or 256x256 while host rendering stores more samples.

Advantages:

- Guest camera and render-phase logic remain unchanged.
- The named target remains compatible with code that expects original dimensions.
- One mechanism can scale mirror, water, general, and interior reflection families independently.

Risks to handle explicitly:

- Half-pixel and texel-center offsets from the D3D9/Xenos path.
- Resolve source/destination coordinates and partial rectangles.
- MSAA sample layout and any EDRAM emulation assumptions.
- Shader constants containing inverse target dimensions.
- Readback or CPU-visible render-target content.
- Resource aliases and backup surfaces.
- Mip generation and sampling LOD.

The interception point is GTA4-native's named-resource hook around `sub_828DC7F0`, where the name and requested width/height are still available. It recognizes only the six proven names: `MIRROR_RT`, `MIRROR_DT`, `WATER_REFLECTION_COLOUR`, `WATER_REFLECTION_DEPTH`, `REFLECTION_MAP_COLOUR`, and `REFLECTION_MAP_DEPTH`. Color/depth members receive identical resolution and AA policy, resources are recreated only at initialization or renderer restart, and the override never applies to deferred, UI, shadow, or arbitrary script-created targets.

The implemented native interface uses `Original`, `1080p`, and capped `Full` semantics. The 256x256 paraboloid family always maps to an explicitly documented square extent rather than inheriting the planar 16:9 dimensions.

Xenia documents both the value and danger of this model: resolution-scaled render-to-texture data needs dedicated storage and GTA IV specifically needs resolve-edge handling for half-pixel gaps. See the sources section.

#### Alternative: patch guest allocations

Hook or patch the complete logic around `sub_822BCC20` and `sub_822D93E8` so the game itself requests larger resources. This is simpler in concept but riskier: render phases may also hard-code 320x180 or 256x256 viewports, texel offsets, projection parameters, or resolve rectangles. If used, the implementation must update every dependent value and preserve the entire original function behavior; a partial logic stub is not acceptable.

#### Quality tiers

Use explicit relative tiers rather than the current ambiguous `Eighth/Quarter/Half/Full` names:

| Tier | Mirror/water pair | General reflection pair | Intended platforms |
|---|---:|---:|---|
| Original | 320x180 | 256x256 | Compatibility/reference |
| 1080p | 1920x1080 | 1024x1024 | Default native desktop tier after runtime validation |
| Full (1440p cap) | 2560x1440 | 2048x2048 | High-end optional tier |
| Full (display cap) | 16:9 from display height | 1024/2048/4096 square tiers | Experimental display-relative tier |

Do not couple MSAA to resolution. Provide a separate reflection-AA choice, because FusionFix found value in applying MSAA independently to vehicle, water, interior, and mirror reflection maps and also warns that behavior varies by API and driver.

### Phase 1.5: restore original reflective materials

Before judging the resolution change:

1. Confirm the live SPS/FXC selection for every `*_reflect*` family.
2. Verify that cube/paraboloid texture types, array faces, view dimensions, and sampler states survive resource translation.
3. Verify reflection intensity/Fresnel/specular constants against an Xbox 360 reference capture.
4. Investigate the `gta_vehicle_spec_reflect* -> gta_default` preset mapping.
5. Validate normal-map decoding. FusionFix reports that correcting vehicle/ped normal-map quality makes vehicle reflections less blocky without changing reflection resolution.
6. Validate anisotropic-filter interaction and reflection MSAA separately; both have caused GTA IV PC reflection issues according to FusionFix.
7. Restore and verify the reflection color mip chain and blur/prefilter path. A sharper base map without correct roughness mips can make vehicle paint shimmer or look unnaturally mirror-like.
8. Treat capture content and capture resolution as separate controls. If the 40.0-unit general-reflection far plane is too restrictive, test a modest optional distance tier with measured draw-count cost; do not silently change it as part of the resolution setting.
9. Retune the mirror shader's four-tap projected filter for the new texel size.
10. Improve true cubemap asset/face/mip handling separately; the live paraboloid scale cannot affect those materials.

This phase may produce a larger quality gain than resolution scaling if reflective shaders are currently falling back or sampling incorrectly.

### Phase 2: hybrid environment maps plus SSR

SSR should add local contact detail that the original 256x256 environment map cannot provide, not replace every reflection source.

Required inputs:

- Resolved scene radiance before UI.
- Linear or correctly reconstructable depth.
- Stable view-space or world-space normals.
- A reflection eligibility mask and roughness/specular value derived from GTA IV materials.
- Current and previous camera matrices.
- Motion vectors, or a well-defined camera-only fallback with lower quality.
- A hierarchical depth pyramid.
- Original cube/paraboloid/planar reflection source for misses and screen-edge fallback.

Recommended pass order:

1. Finish opaque geometry and lighting.
2. Preserve scene color in a separate readable texture.
3. Build the depth pyramid.
4. Classify reflective pixels from material/shader identity and roughness.
5. Trace SSR at a configurable rate.
6. Reject invalid history using depth/normal/disocclusion tests and denoise temporally/spatially.
7. Fill misses from the original environment map.
8. Composite before transparent glass, particles, HUD, and tone mapping unless a captured GTA IV pass requires otherwise.
9. Render transparent reflective materials with their own forward path and the same probe fallback.

AMD's FidelityFX SSSR documentation is a useful architecture reference: it uses hierarchical depth, roughness-driven rate, motion vectors, variance history, denoising, and an environment-map fallback. The SDK implementation itself officially targets Direct3D 12 and Vulkan, so LibertyRecomp should either port the algorithmic ideas to the existing render abstraction and Metal shaders or use a smaller custom cross-platform SSR implementation.

For a smaller first implementation, use perspective-correct screen-space DDA or hierarchical-Z traversal rather than the current projected-endpoint linear stepping. McGuire and Mara's screen-space ray-tracing paper explains how DDA visits contiguous screen pixels without the severe over- or undersampling produced by uniform camera-space steps.

#### Why the original map must remain

SSR cannot see objects outside the current screen, objects hidden behind foreground geometry, or most geometry behind the camera. It also becomes unstable near screen edges and under rapid disocclusion. The original environment map provides stable off-screen content; dedicated planar mirrors provide the correct reflected viewpoint. A hybrid composite is both more faithful and more robust than pure SSR.

### Phase 3: optional hardware-ray-traced reflections

Ray tracing is a future quality tier, not the next task.

It requires:

- A host-visible representation of streamed GTA IV geometry.
- Stable instance transforms and material identifiers.
- Bottom- and top-level acceleration-structure lifetime management.
- Skinned/deformable vehicle and pedestrian update policy.
- Alpha-tested vegetation and transparent/glass policy.
- Reflection shading that can reuse or approximate GTA IV's lights, textures, and material constants.
- Denoising, temporal history, and environment fallback.
- Capability checks and non-RT fallbacks on every backend.

The practical hybrid is SSR for cheap on-screen hits, hardware rays only for SSR misses or selected mirror/vehicle pixels, and original probes for remaining misses. AMD's Hybrid Reflections sample uses the same broad division. Full replacement would be substantially more invasive than improving the original render targets.

## Cross-platform quality plan

| Tier | Technique | Data requirements | Expected reach |
|---|---|---|---|
| Baseline | Original reflection passes at original resolution | Existing game render targets | All supported platforms |
| Fidelity+ | Original passes at 2x/4x host scale, optional reflection MSAA | Scaled target/resolve support | All platforms, tier selected by memory/performance |
| Hybrid Lite | Half-resolution SSR with depth-derived normals and probe fallback | Scene color, depth, camera matrices, material mask | Broad reach, but visual limits must be explicit |
| Hybrid Full | Hi-Z SSR with real normals, roughness, motion, temporal denoise, probe fallback | Additional buffers and stable frame history | Desktop/current high-end console targets; custom Metal path required |
| RT Hybrid | SSR plus rays for misses/selected pixels | Geometry bridge, acceleration structures, denoiser | Capability-gated desktop/modern Apple GPU; platform-specific console work |

Platform cautions:

- **macOS/iOS:** use Metal-native compute and runtime ray-tracing capability checks. Do not assume all Apple devices support ray tracing.
- **Windows/Linux:** Vulkan offers the widest shared path; D3D12 can use equivalent shaders or FidelityFX integration where available.
- **PS4/Switch/Android/older iOS:** prioritize original-target scaling and a conservative SSR-lite mode. Hardware RT cannot be a dependency.
- **All platforms:** allow independent resolution per reflection family and fail back to the original maps rather than disabling reflections.

For scale, one 1920x1080 RGBA16F SSR result is about 15.82 MiB, while a half-resolution result is about 3.96 MiB. A seven-level R32 depth pyramid is about 10.55 MiB at full resolution or 2.64 MiB at half resolution. Normals, roughness/material data, motion, variance/history, and scratch storage are additional, which is why half-resolution SSR and compact eligibility masks are the sensible starting point on bandwidth-limited targets.

## Decision matrix

| Option | Visual gain | Fidelity to GTA IV | Engineering risk | Cross-platform reach | Recommendation |
|---|---|---|---|---|---|
| Raise original target resolution | High sharpness gain; no new reflected content | Highest | Medium due to coordinate/resolve scaling | Highest | Implement first |
| Fix material/shader mapping | Potentially very high if fallbacks are active | Highest | Medium | Highest | Investigate in parallel with Phase 1 |
| Add MSAA to reflection maps | Reduces jagged geometry edges, not texture blur | High | Medium/API-sensitive | High with per-backend validation | Separate option after scaling works |
| Replace all reflections with fullscreen SSR | Adds local detail but has holes, edge loss, and material errors | Low | High | Medium | Do not do this |
| Hybrid original maps + material-aware SSR | Best balance of stability and local detail | High when tuned | High | Medium-high with custom backend work | Phase 2 target |
| Full ray-traced replacement | Highest theoretical accuracy | Can diverge from original art direction | Very high | Low | Future experimental tier only |

## Validation plan

### Reference images

Capture identical camera/time/weather scenes from:

- Original Xbox 360 hardware if available.
- Xenia at 1x internal scale.
- Xenia at a higher render-target scale.
- LibertyRecomp original, 2x, and 4x reflection tiers.

### Surface test matrix

- Vehicle paint, chrome, glass, rims, damage, and two-layer materials.
- A true mirror at front-facing and grazing camera angles.
- Exterior glass by day and at night.
- Interior reflections during transitions through portals/doors.
- Dry asphalt versus rain-controlled wet asphalt.
- Pedestrian reflective clothing/material variants.
- Water reflection only as a render-target integration check owned by the water investigation.
- Bright coronas, emissive signs, trees, terrain, alpha-tested vegetation, and transparent particles.

### Failure checks

- Reflection disappears at screen edges or when the source moves off-screen.
- Left/right or top/bottom half-pixel gaps after resolve.
- Incorrect aspect ratio or offset in mirror/water targets.
- Stale or one-frame-late environment maps.
- Camera cuts smear temporal reflection history.
- Reflections appear on matte walls, skin, sky, fog, particles, or HUD.
- Glass either loses reflection or reflects UI/post-processing.
- Cube/paraboloid seams become more visible at higher resolution.
- Guest code reads a scaled resource using original pitch/size assumptions.
- Mobile/tile GPUs exceed transient-memory budgets.

### Performance telemetry

Record per family:

- Target allocation size and transient peak.
- Draw count and triangles rendered into the reflection view.
- Reflection-pass GPU duration.
- Resolve/copy duration.
- SSR classification, trace, denoise, and composite duration.
- History rejection percentage and SSR hit/miss ratio.

Every tier needs a deterministic fallback when allocation, feature support, or performance budget is insufficient.

## Concrete code-entry checklist

1. Add a `ReflectionFamily` tag to host texture/surface metadata.
2. Classify target creation by resource name/caller around `sub_822BCC20`, `sub_822D93E8`, `sub_828DC7F0`, and `sub_828BEC78`.
3. Add exact diagnostics for requested/actual format, samples, mip, face, resolve source/destination, and shader cache misses.
4. Keep cube-map work separate from these proven 2D reflection families; add face-aware native resolves before scaling any runtime cube capture.
5. Reject unsupported native formats explicitly and keep checked host allocation limits.
6. Trace MRT use during the reflection/deferred phases before expanding the six-target policy.
7. Replace the unused resolution enum with explicit `Original/2x/4x/DisplayRelative` semantics, or clearly map the existing values.
8. Apply name-based host scale to matching color/depth pairs while preserving guest logical dimensions.
9. Scale target binding, viewport/scissor, copies, resolves, and texel-dependent shader offsets consistently.
10. Allocate the required reflection mip levels and restore the original or an equivalent per-mip prefilter/blur path.
11. Add capture/debug labels and an on-screen reflection-source view.
12. Audit live SPS/FXC selection, especially vehicle reflection fallbacks, in both graphics paths.
13. Validate original reflection scaling on all active backends before touching SSR.
14. Remove or quarantine the current SSR stub until it has real pipelines, resource binding, non-identity inverse matrices, material inputs, ping-pong output, resize handling, and tests.
15. Implement SSR behind an independent experimental option with original-map fallback.
16. Promote SSR only after the surface and platform matrices pass.

## Sources

### Project and binary evidence

- Retail Xbox 360 assembly: `gta_iv/xex_excavation_retail/default_decrypted.xex.asm`.
- Retail RTTI/vtable mapping: `gta_iv/xex_excavation_retail/method_symbols.txt` and `vtables_with_addrs.txt`.
- Generated mirror initialization: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.12.cpp`.
- Generated target initialization: `glue/rexglue-sdk-main/gta4-recomp/generated/gta4_recomp.14.cpp`.
- Recovered pseudocode: `gta_iv/xex_excavation_retail/pseudocode/sub_822BCC20_0x822BCC20.c`, `sub_822D93E8_0x822D93E8.c`, `sub_824F78D8_0x824F78D8.c`, and `sub_822B08B0_0x822B08B0.c`.
- Disassembled RAGE shader assets: `LibertyRecompLib/shader/rage_shaders/deferred_lighting/`, `water/`, `mirror/`, `gta_vehicle_vehglass/`, and the `gta_*reflect/` families.
- Host GPU hooks and post-process call site: `LibertyRecomp/gpu/video.cpp`.
- GTA4-native hooks and graphics backend: `glue/rexglue-sdk-main/gta4-recomp/src/gta4_native_hooks.cpp` and `glue/rexglue-sdk-main/src/graphics/gta4_native/graphics_system.cpp`.
- Current SSR scaffolding: `LibertyRecomp/gpu/postprocess_renderer.cpp` and `gpu/shader/hlsl/ssr_*.hlsl`.
- Material/preset inventory: `LibertyRecomp/gpu/sps_preset_table.h`.
- Checked-out FusionFix reflection mip/MSAA work: `tools/GTAIV.EFLC.FusionFix-master/source/reflectionmsaa.ixx`.

### External technical references

- [Xenia: Leaving No Pixel Behind — render-target cache and resolution scaling](https://xenia.jp/updates/2021/04/27/leaving-no-pixel-behind-new-render-target-cache-3x3-resolution-scaling.html)
- [Xenia options: resolution scaling and GTA IV resolve-edge handling](https://github.com/xenia-project/xenia/wiki/Options/815dbf04c9794c5ced10d0241ee69a313b1dd067)
- [FusionFix repository and GTA IV reflection fixes](https://github.com/ThirteenAG/GTAIV.EFLC.FusionFix)
- [FusionFix releases, including reflection MSAA and reflection-quality fixes](https://github.com/ThirteenAG/GTAIV.EFLC.FusionFix/releases)
- [Adrian Courrèges: independent GTA IV dual-paraboloid vertex-warp observation](https://www.adriancourreges.com/blog/page/2/)
- [AMD FidelityFX Stochastic Screen-Space Reflections documentation](https://gpuopen.com/manuals/fidelityfx_sdk/techniques/stochastic-screen-space-reflections/)
- [AMD FidelityFX Hybrid Reflections](https://gpuopen.com/fidelityfx-hybrid-reflections/)
- [McGuire and Mara: Efficient GPU Screen-Space Ray Tracing](https://jcgt.org/published/0003/04/04/)
- [NVIDIA GPU Gems 2: reflection/refraction with planar and cube environment maps](https://developer.nvidia.com/gpugems/gpugems2/part-ii-shading-lighting-and-shadows/chapter-19-generic-refraction-simulation)
- [NVIDIA GPU Gems: image-based lighting and cube-map limitations](https://developer.nvidia.com/gpugems/gpugems/part-iii-materials/chapter-19-image-based-lighting)
- [Apple Metal feature-set tables](https://developer.apple.com/metal/feature-sets/)
- [Microsoft DirectX Raytracing functional specification](https://microsoft.github.io/DirectX-Specs/d3d/Raytracing.html)

## Bottom line

The low resolution is real in the source material: dedicated mirrors and water reflections are 320x180, and the general paraboloid reflection map is 256x256. LibertyRecomp currently preserves those requested dimensions and does not connect its reflection-resolution setting to them. The fastest safe quality win is to scale the original tagged targets and fix any reflective shader fallbacks. A modern system should then layer material-aware SSR over the original environment maps, not replace them; true mirrors should retain their dedicated planar pass, and ray tracing should remain an optional capability tier.
