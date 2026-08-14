#version 450

layout(set = 0, binding = 0) uniform sampler2D source_image;

layout(push_constant) uniform ReflectionMipFilterConstants {
  uint destination_level;
  uint reserved_0;
  uint reserved_1;
  uint reserved_2;
} reflection_constants;

layout(location = 0) out vec4 output_color;

void main() {
  ivec2 source_extent = textureSize(source_image, 0);
  ivec2 source_maximum = source_extent - ivec2(1);
  ivec2 source_origin = ivec2(gl_FragCoord.xy) * 2;

  vec4 color = texelFetch(source_image, min(source_origin, source_maximum), 0);
  color += texelFetch(source_image,
                      min(source_origin + ivec2(1, 0), source_maximum), 0);
  color += texelFetch(source_image,
                      min(source_origin + ivec2(0, 1), source_maximum), 0);
  color += texelFetch(source_image,
                      min(source_origin + ivec2(1, 1), source_maximum), 0);
  color *= 0.25;

  float level_weight = pow(1.0 / (float(reflection_constants.destination_level) + 1.0),
                           0.25);
  output_color = vec4(color.rgb * level_weight, 1.0);
}
