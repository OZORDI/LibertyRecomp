/**
 * @file native_fixed_function_policy_test.cpp
 * @brief GTA IV native-renderer packed fixed-function state tests.
 */

#include <catch2/catch_test_macros.hpp>

#include "graphics/gta4_native/native_fixed_function_policy.h"
#include "graphics/gta4_native/native_shader_booleans.h"

namespace rex::graphics::gta4_native {
namespace {

TEST_CASE("GTA IV native blend controls decode independently",
          "[gta4-native][graphics][fixed-function]") {
  constexpr uint32_t packed = 3u | (5u << 5) | (7u << 8) | (11u << 16) | (2u << 21) | (13u << 24);
  constexpr NativeBlendControlState state = DecodeNativeBlendControl(packed);
  CHECK(state.source_color == 3u);
  CHECK(state.color_operation == 5u);
  CHECK(state.destination_color == 7u);
  CHECK(state.source_alpha == 11u);
  CHECK(state.alpha_operation == 2u);
  CHECK(state.destination_alpha == 13u);
}

TEST_CASE("GTA IV native blend enable follows expanded target equations",
          "[gta4-native][graphics][fixed-function]") {
  constexpr uint32_t identity = 0x00010001u;
  constexpr uint32_t additive = 0x01010101u;
  constexpr uint32_t subtract = 0x00210021u;
  constexpr uint32_t identity_with_reserved_bits = 0xE001E001u;

  CHECK(IsNativeBlendControlIdentity(identity));
  CHECK(IsNativeBlendControlIdentity(identity_with_reserved_bits));
  CHECK_FALSE(IsNativeBlendControlEnabled(identity));
  CHECK(IsNativeBlendControlEnabled(additive));
  CHECK(IsNativeBlendControlEnabled(subtract));

  constexpr std::array<uint32_t, kNativeFixedRenderTargetCount> controls = {identity, additive,
                                                                            identity, subtract};
  CHECK(NativeBlendEnableMask(controls, 0b1111u) == 0b1010u);
  CHECK(NativeBlendEnableMask(controls, 0b0101u) == 0u);
  CHECK(NativeBlendEnableMask(controls, 0b1010u) == 0b1010u);
}

TEST_CASE("GTA IV native color write masks preserve all render targets",
          "[gta4-native][graphics][fixed-function]") {
  constexpr uint32_t packed = 0xA53Cu;
  CHECK(NativeColorWriteMaskForTarget(packed, 0) == 0xCu);
  CHECK(NativeColorWriteMaskForTarget(packed, 1) == 0x3u);
  CHECK(NativeColorWriteMaskForTarget(packed, 2) == 0x5u);
  CHECK(NativeColorWriteMaskForTarget(packed, 3) == 0xAu);
  CHECK(NativeColorWriteMaskForTarget(packed, 4) == 0u);
  CHECK(AnyNativeColorWriteEnabled(packed));
  CHECK_FALSE(AnyNativeColorWriteEnabled(0));

  constexpr uint32_t inactive_target_writes = 0xFFF0u;
  CHECK_FALSE(AnyNativeColorWriteEnabledForTargets(inactive_target_writes, 0b0001u));
  CHECK(AnyNativeColorWriteEnabledForTargets(inactive_target_writes, 0b0010u));
  CHECK_FALSE(AnyNativeColorWriteEnabledForTargets(inactive_target_writes, 0u));
  CHECK(AnyNativeColorWriteEnabledForTargets(0x000Fu, 0b0001u));
}

TEST_CASE("GTA IV native color writes are normalized to shader outputs and formats",
          "[gta4-native][graphics][fixed-function]") {
  constexpr uint32_t format_component_masks = 0xF13Fu;
  CHECK(NormalizeNativeColorWriteMask(0xFFFFu, 0b0101u, format_component_masks) == 0x0F0Fu);
  CHECK(NativeColorTargetMaskFromWriteMask(0x0F0Fu) == 0b0101u);

  // RT1 has two components. Writing red enables the attachment and marks its
  // nonexistent blue/alpha components writable for the host fast path.
  CHECK(NormalizeNativeColorWriteMask(0x0010u, 0b0010u, format_component_masks) == 0x00D0u);
  // A write solely to a nonexistent component must not claim the attachment.
  CHECK(NormalizeNativeColorWriteMask(0x0200u, 0b0100u, format_component_masks) == 0u);
  CHECK(NormalizeNativeColorWriteMask(0xFFFFu, 0u, format_component_masks) == 0u);
}

TEST_CASE("GTA IV native packed scissor applies signed window offset",
          "[gta4-native][graphics][fixed-function]") {
  constexpr uint32_t top_left = 10u | (20u << 16);
  constexpr uint32_t bottom_right = 110u | (220u << 16);
  constexpr uint32_t window_offset = 0x7FFCu | (3u << 16);
  constexpr NativePackedScissorState state =
      DecodeNativePackedScissor(top_left, bottom_right, window_offset);
  CHECK(state.rectangle == std::array<int32_t, 4>{6, 23, 106, 223});
  CHECK(state.window_offset == std::array<int32_t, 2>{-4, 3});
  CHECK_FALSE(state.window_offset_disable);

  constexpr NativePackedScissorState disabled =
      DecodeNativePackedScissor(top_left | 0x80000000u, bottom_right, window_offset);
  CHECK(disabled.rectangle == std::array<int32_t, 4>{10, 20, 110, 220});
  CHECK(disabled.window_offset_disable);
}

TEST_CASE("GTA IV native no-cull winding remains distinct",
          "[gta4-native][graphics][fixed-function]") {
  constexpr NativeCullRasterState none_ccw = DecodeNativeCullRasterState(0);
  constexpr NativeCullRasterState none_cw = DecodeNativeCullRasterState(4);
  CHECK(none_ccw.cull_face == NativeCullFace::kNone);
  CHECK_FALSE(none_ccw.front_face_clockwise);
  CHECK(none_cw.cull_face == NativeCullFace::kNone);
  CHECK(none_cw.front_face_clockwise);
  CHECK_FALSE(DecodeNativeCullRasterState(3).valid());
}

TEST_CASE("GTA IV native polygon mode follows surviving Xenos faces",
          "[gta4-native][graphics][fixed-function]") {
  constexpr uint32_t dual_front_lines_back_triangles = 1u << 3 | 1u << 5 | 2u << 8;
  CHECK(SelectNativePolygonMode(dual_front_lines_back_triangles, 0) == NativePolygonMode::kLine);
  CHECK(SelectNativePolygonMode(dual_front_lines_back_triangles, 1) == NativePolygonMode::kFill);
  CHECK(SelectNativePolygonMode(0, 4) == NativePolygonMode::kFill);
}

TEST_CASE("GTA IV native sample mask preserves replicated Xenos coverage",
          "[gta4-native][graphics][fixed-function]") {
  CHECK(SelectNativeSampleMask(0xFFFFu, 1) == NativeSampleMaskSelection{true, 0x1u});
  CHECK(SelectNativeSampleMask(0xFFFFu, 2) == NativeSampleMaskSelection{true, 0x3u});
  CHECK(SelectNativeSampleMask(0xFFFFu, 4) == NativeSampleMaskSelection{true, 0xFu});
  CHECK(SelectNativeSampleMask(0xAAAAu, 2) == NativeSampleMaskSelection{true, 0x1u});
  CHECK(SelectNativeSampleMask(0x5555u, 4) == NativeSampleMaskSelection{true, 0x3u});
  CHECK_FALSE(SelectNativeSampleMask(0xFFF1u, 4).representable);
  CHECK_FALSE(SelectNativeSampleMask(0xFFFFu, 3).representable);
}

TEST_CASE("GTA IV native pipeline sample mask preserves guest topology or rejects it",
          "[gta4-native][graphics][fixed-function]") {
  CHECK(SelectNativePipelineSampleMask(0xFFFFu, 1, 1) == NativeSampleMaskSelection{true, 0x1u});
  CHECK(SelectNativePipelineSampleMask(0xAAAAu, 2, 2) == NativeSampleMaskSelection{true, 0x1u});
  CHECK(SelectNativePipelineSampleMask(0x5555u, 4, 4) == NativeSampleMaskSelection{true, 0x3u});
  CHECK(SelectNativePipelineSampleMask(0xFFFFu, 2, 4) == NativeSampleMaskSelection{true, 0xFu});
  CHECK(SelectNativePipelineSampleMask(0u, 4, 1) == NativeSampleMaskSelection{true, 0u});
  CHECK_FALSE(SelectNativePipelineSampleMask(0xFFFFu, 4, 3).representable);
  CHECK_FALSE(SelectNativePipelineSampleMask(0x1111u, 4, 2).representable);
  CHECK_FALSE(SelectNativePipelineSampleMask(0xFFF1u, 4, 4).representable);
  CHECK_FALSE(SelectNativePipelineSampleMask(0x1010u, 1, 1).representable);
}

TEST_CASE("GTA IV native depth bias rejects unequal two-sided state",
          "[gta4-native][graphics][fixed-function]") {
  constexpr NativeDepthBiasFaceState front{true, 1u, 2u};
  constexpr NativeDepthBiasFaceState back{true, 3u, 4u};
  CHECK_FALSE(SelectNativeDepthBias(0, front, back).representable);
  CHECK(SelectNativeDepthBias(1, front, back) == NativeDepthBiasSelection{true, back});
  CHECK(SelectNativeDepthBias(2, front, back) == NativeDepthBiasSelection{true, front});
  CHECK(SelectNativeDepthBias(4, front, front) == NativeDepthBiasSelection{true, front});
}

TEST_CASE("GTA IV native stencil mask reference fallback is explicit",
          "[gta4-native][graphics][fixed-function]") {
  constexpr NativeStencilMaskRefState front{1u, 0x7Fu, 0x3Fu};
  constexpr NativeStencilMaskRefState back{2u, 0xFFu, 0xF0u};
  CHECK(SelectNativeStencilMaskRef(true, true, 0, front, back) ==
        NativeStencilMaskRefSelection{true, true, front, back});
  CHECK_FALSE(SelectNativeStencilMaskRef(true, false, 0, front, back).representable);
  CHECK(SelectNativeStencilMaskRef(true, false, 1, front, back) ==
        NativeStencilMaskRefSelection{true, false, back, back});
  CHECK(SelectNativeStencilMaskRef(true, false, 2, front, back) ==
        NativeStencilMaskRefSelection{true, false, front, front});
  CHECK(SelectNativeStencilMaskRef(false, false, 0, front, back) ==
        NativeStencilMaskRefSelection{true, false, front, front});
}

}  // namespace
}  // namespace rex::graphics::gta4_native


#include <limits>
#include "graphics/gta4_native/native_color_output.h"

namespace rex::graphics::gta4_native {
TEST_CASE("Native output exponent is signed and matches resolve compensation",
          "[gta4-native][graphics][color-output]") {
  for (int32_t e = -32; e <= 31; ++e) {
    const uint32_t encoded = uint32_t(e) & 63u;
    CHECK(NativeColorExponent(encoded << 20) == e);
    CHECK(NativeResolveExponent(encoded << 26) == e);
    CHECK(NativePowerOfTwo(e) == std::ldexp(1.0f, e));
  }
  CHECK(NativeColorExponent(0x03FC0001) == -1);
  CHECK(NativeResolveExponent(0x04000000) == 1);
  CHECK(NativePowerOfTwo(NativeColorExponent(0x03FC0001)) *
        NativePowerOfTwo(NativeResolveExponent(0x04000000)) == 1.0f);
}

TEST_CASE("Native glass scales alpha after coverage and retains positive background weight",
          "[gta4-native][graphics][color-output]") {
  const auto p = NativeColorOutput(0x03FC0001);
  CHECK(p.scale[0] == 0.5f);
  const std::array<float, 4> source = {8.0f, 4.0f, 2.0f, 1.5f};
  const auto result = ApplyNativeColorOutput(source, p);
  CHECK(result == std::array<float, 4>{4.0f, 2.0f, 1.0f, 0.75f});
  CHECK(1.0f - result[3] == 0.25f);
  // Original coverage sees 1.5; a premature scale would incorrectly fail this test.
  CHECK(source[3] > 1.0f);
  CHECK_FALSE(result[3] > 1.0f);
}

TEST_CASE("Native packed floating color clamps only normalized alpha",
          "[gta4-native][graphics][color-output]") {
  const auto p = NativeColorOutput(12u << 16);
  CHECK(ApplyNativeColorOutput({8, -2, 40, 2}, p) ==
        std::array<float, 4>{8, -2, 40, 1});
  CHECK(ApplyNativeColorOutput({8, -2, 40, -2}, p)[3] == 0);
  CHECK(ApplyNativeColorOutput({8, -2, 40, std::numeric_limits<float>::quiet_NaN()}, p)[3] == 0);
  CHECK(ApplyNativeColorOutput({8, -2, 40, std::numeric_limits<float>::infinity()}, p)[3] == 1);
}

TEST_CASE("Native float target and inactive attachment do not get normalized clamps",
          "[gta4-native][graphics][color-output]") {
  const std::array<float, 4> source = {8, -2, 40, 2};
  CHECK(ApplyNativeColorOutput(source, NativeColorOutput(7u << 16)) == source);
  CHECK(ApplyNativeColorOutput(source, NativeColorOutput(0x03FC0001, false)) == source);
  CHECK(ApplyNativeColorOutput(source, NativeColorOutput(0)) ==
        std::array<float, 4>{1, 0, 1, 1});
  CHECK(ApplyNativeColorOutput(source, NativeColorOutput(5u << 16)) ==
        std::array<float, 4>{8, -2, 32, 2});
}

TEST_CASE("Native MRT output contracts retain independent scales and format bounds",
          "[gta4-native][graphics][color-output]") {
  std::array<NativeColorOutputParameters, 4> contracts{
      NativeColorOutput(0x03FC0001), NativeColorOutput((1u << 20) | (7u << 16)),
      NativeColorOutput(0), NativeColorOutput(0, false)};
  const std::array<float, 4> source{4, 2, 1, 1.5f};
  CHECK(ApplyNativeColorOutput(source, contracts[0])[3] == 0.75f);
  CHECK(ApplyNativeColorOutput(source, contracts[1])[3] == 3.0f);
  CHECK(ApplyNativeColorOutput(source, contracts[2])[3] == 1.0f);
  CHECK(ApplyNativeColorOutput(source, contracts[3])[3] == 1.5f);
  CHECK(kNativeColorOutputOffset == 0x360);
  CHECK(sizeof(NativeColorOutputParameters) == 48);
}
TEST_CASE("Native shader Boolean banks preserve every supported bit",
          "[gta4-native][graphics][shader-booleans]") {
  CHECK(PackNativeShaderBooleans(0u, 0u) == 0u);
  CHECK(PackNativeShaderBooleans(0xFFFFu, 0u) == 0x0000FFFFu);
  CHECK(PackNativeShaderBooleans(0u, 0xFFFFu) == 0xFFFF0000u);
  CHECK(PackNativeShaderBooleans(0xFFFFFFFFu, 0xFFFFFFFFu) == 0xFFFFFFFFu);
  for (uint32_t bit = 0; bit < 16; ++bit) {
    const uint32_t mask = 1u << bit;
    CHECK(PackNativeShaderBooleans(mask, 0u) == mask);
    CHECK(PackNativeShaderBooleans(0u, mask) == (mask << 16));
    CHECK(PackNativeShaderBooleans(mask, mask) == (mask | (mask << 16)));
  }
}

TEST_CASE("Native shader Boolean bank packing cannot leak between stages",
          "[gta4-native][graphics][shader-booleans]") {
  CHECK(PackNativeShaderBooleans(0xFFFF0000u, 0u) == 0u);
  CHECK(PackNativeShaderBooleans(0u, 0xFFFF0000u) == 0u);
  CHECK(PackNativeShaderBooleans(0xA5A5u, 0x5A5Au) == 0x5A5AA5A5u);
  // b1 and b129 are the paired deferred-alpha controls. Vertex b8 and b11
  // are real title controls, not unused padding in the shared ABI.
  CHECK(PackNativeShaderBooleans(0x0902u, 0x0002u) == 0x00020902u);
}

}  // namespace rex::graphics::gta4_native
