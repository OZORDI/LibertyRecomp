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

layout(set = 0, binding = 0) uniform sampler2D color_gamma_tex;
layout(location = 0) out vec2 output_edges;

void main() {
  vec2 texcoord = gl_FragCoord.xy * SMAA_RT_METRICS.xy;
  vec4 offset[3];
  SMAAEdgeDetectionVS(texcoord, offset);
  output_edges = SMAALumaEdgeDetectionPS(texcoord, offset, color_gamma_tex);
}
