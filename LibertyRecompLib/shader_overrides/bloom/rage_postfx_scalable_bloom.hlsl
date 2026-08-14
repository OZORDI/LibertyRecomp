// Native replacements for Xbox rage_postfx PS_BlurX / PS_BlurY.
//
// The descriptor and constant offsets are the native XenosRecomp ABI of
// rage_postfx_ps8.bin and rage_postfx_ps9.bin. The broad and center Gaussian
// kernels are the normalized FusionShaders scalable-bloom kernels. Their UV
// spacing is derived from globalScreenSize, so the footprint remains stable
// when the internal render resolution changes.

#define SPEC_CONSTANT_ALPHA_TEST (1u << 1u)
#define SPEC_CONSTANT_ALPHA_TEST_FUNCTION_SHIFT 8u
#define SPEC_CONSTANT_ALPHA_TEST_FUNCTION_MASK 7u

bool AlphaTestPass(float alpha, float reference, uint function) {
  if (function == 0u) return false;
  if (function == 1u) return alpha < reference;
  if (function == 2u) return alpha == reference;
  if (function == 3u) return alpha <= reference;
  if (function == 4u) return alpha > reference;
  if (function == 5u) return isnan(alpha) || isnan(reference) || alpha != reference;
  if (function == 6u) return alpha >= reference;
  if (function == 7u) return true;
  return false;
}

struct PushConstants {
  uint64_t VertexShaderConstants;
  uint64_t PixelShaderConstants;
  uint64_t SharedConstants;
};

[[vk::push_constant]] ConstantBuffer<PushConstants> g_PushConstants;
[[vk::constant_id(0)]] const uint g_SpecConstants = 0;

Texture2D<float4> g_Texture2DDescriptorHeap[] : register(t0, space0);
SamplerState g_SamplerDescriptorHeap[] : register(s0, space4);

struct Interpolators {
  float4 iPos : SV_Position;
  float4 iTexCoord0 : TEXCOORD0;
};

struct PixelShaderOutput {
  float4 oC0 : SV_Target0;
};

static const float kBloomUvScale = 0.0018518;
static const float kBroadCenterWeight = 0.09290778895898182;
static const float kBroadWeights[12] = {
    0.09043987558157364, 0.08342331057487526,
    0.07291734780749201, 0.06039368393189297,
    0.04739913868089060, 0.03525066121688894,
    0.024841674758290057, 0.016588619577573006,
    0.010496752439822546, 0.006293833998248673,
    0.003575956393107322, 0.0019252505598539797,
};
static const float kCenterWeights[5] = {
    0.2932482044098259, 0.22394306564588196,
    0.09969501558123543, 0.025843481100711958,
    0.003894335467257663,
};

float3 SampleBloom(Texture2D<float4> texture, SamplerState bloom_sampler,
                   float2 uv) {
  return texture.Sample(bloom_sampler, uv).rgb;
}

#ifndef XENOS_RECOMP_LATE_FRAGMENT_TESTS
[earlydepthstencil]
#endif
PixelShaderOutput shaderMain(Interpolators input) {
  const uint texture_index =
      vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 0);
  const uint sampler_index =
      vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 416);
  const float alpha_threshold =
      vk::RawBufferLoad<float>(g_PushConstants.SharedConstants + 580);

  // GTA IV's globalScreenSize is (width, height, 1 / width, 1 / height).
  // Multiplying height by its reciprocal pair yields aspect-correct UV steps
  // with a constant screen-space radius, matching FusionShaders.
  const float4 global_screen_size =
      vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + 704, 0x10);
  const float2 axis_texel =
      global_screen_size.yy * global_screen_size.zw * kBloomUvScale;

#if defined(BLOOM_HORIZONTAL)
  const float2 axis = float2(axis_texel.x, 0.0);
#elif defined(BLOOM_VERTICAL)
  const float2 axis = float2(0.0, axis_texel.y);
#else
#error Define exactly one bloom axis.
#endif

  Texture2D<float4> texture = g_Texture2DDescriptorHeap[texture_index];
  SamplerState bloom_sampler = g_SamplerDescriptorHeap[sampler_index];
  const float2 uv = input.iTexCoord0.xy;
  const float3 center = SampleBloom(texture, bloom_sampler, uv);

  float3 center_blur = center * kCenterWeights[0];
  [unroll]
  for (uint radius = 1; radius < 5; ++radius) {
    const float2 offset = axis * float(radius);
    center_blur +=
        (SampleBloom(texture, bloom_sampler, uv + offset) +
         SampleBloom(texture, bloom_sampler, uv - offset)) *
        kCenterWeights[radius];
  }

  float3 broad_detail = max(center - center_blur, 0.0) * kBroadCenterWeight;
  [unroll]
  for (uint radius = 1; radius <= 12; ++radius) {
    const float2 offset = axis * float(radius);
    const float3 positive =
        max(SampleBloom(texture, bloom_sampler, uv + offset) - center_blur, 0.0);
    const float3 negative =
        max(SampleBloom(texture, bloom_sampler, uv - offset) - center_blur, 0.0);
    broad_detail += (positive + negative) * kBroadWeights[radius - 1];
  }

  PixelShaderOutput output;
  output.oC0 = float4(center_blur + broad_detail, 1.0);
  if (g_SpecConstants & SPEC_CONSTANT_ALPHA_TEST) {
    const uint alpha_test_function =
        (g_SpecConstants >> SPEC_CONSTANT_ALPHA_TEST_FUNCTION_SHIFT) &
        SPEC_CONSTANT_ALPHA_TEST_FUNCTION_MASK;
    const bool alpha_test_pass =
        AlphaTestPass(output.oC0.a, alpha_threshold, alpha_test_function);
    clip(alpha_test_pass ? 1.0 : -1.0);
  }
  return output;
}
