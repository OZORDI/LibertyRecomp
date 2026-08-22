#version 450

layout(set = 0, binding = 0) uniform sampler2D source_image;

void main() {
  gl_FragDepth = texelFetch(source_image, ivec2(gl_FragCoord.xy), 0).x;
}
