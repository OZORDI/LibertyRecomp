#version 450
#extension GL_GOOGLE_include_directive : require

#define SMAA_GLSL_4 1
#define SMAA_INCLUDE_VS 1
#define SMAA_INCLUDE_PS 1

layout(push_constant) uniform SmaaConstants {
  vec4 rt_metrics;
} smaa_constants;

#define SMAA_RT_METRICS smaa_constants.rt_metrics
#include "SMAA.hlsl"

layout(set = 0, binding = 0) uniform sampler2D edges_tex;
layout(set = 0, binding = 1) uniform sampler2D area_tex;
layout(set = 0, binding = 2) uniform sampler2D search_tex;
layout(location = 0) out vec4 output_weights;

void main() {
  vec2 texcoord = gl_FragCoord.xy * SMAA_RT_METRICS.xy;
  vec2 pixcoord = gl_FragCoord.xy;
  vec4 offset[3];
  SMAABlendingWeightCalculationVS(texcoord, pixcoord, offset);
  output_weights = SMAABlendingWeightCalculationPS(
      texcoord, pixcoord, offset, edges_tex, area_tex, search_tex, vec4(0.0));
}
