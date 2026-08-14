#version 450

layout(set = 0, binding = 0) uniform sampler2D source_image;

layout(push_constant) uniform HDRPresentConstants {
  ivec2 source_extent;
  ivec2 destination_extent;
  float hdr_headroom;
  uint output_mode;
} present_constants;

layout(location = 0) out vec4 output_color;

float luma(vec3 color) {
  return dot(color, vec3(0.2126, 0.7152, 0.0722));
}

vec4 fetch_source(ivec2 coordinate) {
  return texelFetch(source_image,
                    clamp(coordinate, ivec2(0),
                          present_constants.source_extent - ivec2(1)),
                    0);
}

// A conservative, single-pass edge resolve for native-resolution output. It
// blends only across a coherent luminance discontinuity, not along it. The
// local-range gate leaves low-contrast texture detail and flat HUD interiors
// untouched while stabilizing thin geometry and high-contrast silhouettes.
vec4 resolve_spatial_edge(ivec2 coordinate) {
  vec4 center = fetch_source(coordinate);
  vec4 north = fetch_source(coordinate + ivec2(0, -1));
  vec4 south = fetch_source(coordinate + ivec2(0, 1));
  vec4 west = fetch_source(coordinate + ivec2(-1, 0));
  vec4 east = fetch_source(coordinate + ivec2(1, 0));

  float center_luma = luma(max(center.rgb, vec3(0.0)));
  float north_luma = luma(max(north.rgb, vec3(0.0)));
  float south_luma = luma(max(south.rgb, vec3(0.0)));
  float west_luma = luma(max(west.rgb, vec3(0.0)));
  float east_luma = luma(max(east.rgb, vec3(0.0)));
  float minimum_luma = min(center_luma,
                           min(min(north_luma, south_luma), min(west_luma, east_luma)));
  float maximum_luma = max(center_luma,
                           max(max(north_luma, south_luma), max(west_luma, east_luma)));
  float local_range = maximum_luma - minimum_luma;
  float edge_threshold = max(0.03125, maximum_luma * 0.125);
  if (local_range <= edge_threshold) {
    return center;
  }

  float vertical_change = abs(north_luma - south_luma);
  float horizontal_change = abs(west_luma - east_luma);
  bool crosses_horizontal_edge = vertical_change >= horizontal_change;
  vec4 negative_sample = crosses_horizontal_edge ? north : west;
  vec4 positive_sample = crosses_horizontal_edge ? south : east;
  float negative_luma = crosses_horizontal_edge ? north_luma : west_luma;
  float positive_luma = crosses_horizontal_edge ? south_luma : east_luma;

  // Require the center to belong to one side of the edge. This rejects
  // symmetric high-frequency texture patterns that a broad FXAA filter would
  // blur indiscriminately.
  float negative_distance = abs(center_luma - negative_luma);
  float positive_distance = abs(center_luma - positive_luma);
  vec4 across_edge = negative_distance < positive_distance ? positive_sample : negative_sample;
  float nearest_side_distance = min(negative_distance, positive_distance);
  float coherence = 1.0 - clamp(nearest_side_distance / max(local_range, 0.0001), 0.0, 1.0);
  float strength = smoothstep(edge_threshold, edge_threshold * 4.0, local_range) * coherence;
  return mix(center, (center + across_edge) * 0.5, min(strength, 0.5));
}

// FXAA 3.11 Quality preset 12, matching FusionFix's search pattern and tuning.
// The source is GTA IV's final perceptual frontbuffer, and the immutable
// presentation sampler is linear. Luma is derived from RGB because the native
// present path must preserve the guest frontbuffer's alpha channel.
vec4 resolve_fxaa(ivec2 coordinate) {
  vec2 inverse_extent = 1.0 / vec2(present_constants.source_extent);
  vec2 uv = (vec2(coordinate) + vec2(0.5)) * inverse_extent;
  vec4 center = fetch_source(coordinate);
  float luma_m = luma(max(center.rgb, vec3(0.0)));
  float luma_n = luma(max(textureLod(source_image, uv + vec2(0.0, -1.0) * inverse_extent,
                                     0.0).rgb,
                          vec3(0.0)));
  float luma_s = luma(max(textureLod(source_image, uv + vec2(0.0, 1.0) * inverse_extent,
                                     0.0).rgb,
                          vec3(0.0)));
  float luma_w = luma(max(textureLod(source_image, uv + vec2(-1.0, 0.0) * inverse_extent,
                                     0.0).rgb,
                          vec3(0.0)));
  float luma_e = luma(max(textureLod(source_image, uv + vec2(1.0, 0.0) * inverse_extent,
                                     0.0).rgb,
                          vec3(0.0)));
  float range_max = max(luma_m, max(max(luma_n, luma_s), max(luma_w, luma_e)));
  float range_min = min(luma_m, min(min(luma_n, luma_s), min(luma_w, luma_e)));
  float luma_range = range_max - range_min;
  if (luma_range < max(0.0312, range_max * 0.125)) {
    return center;
  }

  float luma_nw = luma(max(textureLod(source_image, uv + vec2(-1.0, -1.0) * inverse_extent,
                                      0.0).rgb,
                           vec3(0.0)));
  float luma_ne = luma(max(textureLod(source_image, uv + vec2(1.0, -1.0) * inverse_extent,
                                      0.0).rgb,
                           vec3(0.0)));
  float luma_sw = luma(max(textureLod(source_image, uv + vec2(-1.0, 1.0) * inverse_extent,
                                      0.0).rgb,
                           vec3(0.0)));
  float luma_se = luma(max(textureLod(source_image, uv + vec2(1.0, 1.0) * inverse_extent,
                                      0.0).rgb,
                           vec3(0.0)));

  float luma_ns = luma_n + luma_s;
  float luma_we = luma_w + luma_e;
  float edge_horizontal_1 = luma_ns - 2.0 * luma_m;
  float edge_vertical_1 = luma_we - 2.0 * luma_m;
  float luma_ne_se = luma_ne + luma_se;
  float luma_nw_ne = luma_nw + luma_ne;
  float edge_horizontal_2 = luma_ne_se - 2.0 * luma_e;
  float edge_vertical_2 = luma_nw_ne - 2.0 * luma_n;
  float luma_nw_sw = luma_nw + luma_sw;
  float luma_sw_se = luma_sw + luma_se;
  float edge_horizontal_3 = luma_nw_sw - 2.0 * luma_w;
  float edge_vertical_3 = luma_sw_se - 2.0 * luma_s;
  float edge_horizontal = abs(edge_horizontal_3) +
                          abs(edge_horizontal_1) * 2.0 + abs(edge_horizontal_2);
  float edge_vertical = abs(edge_vertical_3) +
                        abs(edge_vertical_1) * 2.0 + abs(edge_vertical_2);
  bool horizontal_span = edge_horizontal >= edge_vertical;

  float negative_luma = horizontal_span ? luma_n : luma_w;
  float positive_luma = horizontal_span ? luma_s : luma_e;
  float negative_gradient = negative_luma - luma_m;
  float positive_gradient = positive_luma - luma_m;
  bool use_negative_pair = abs(negative_gradient) >= abs(positive_gradient);
  float gradient = max(abs(negative_gradient), abs(positive_gradient));
  float length_sign = horizontal_span ? inverse_extent.y : inverse_extent.x;
  if (use_negative_pair) {
    length_sign = -length_sign;
  }

  float paired_luma = (use_negative_pair ? negative_luma : positive_luma) + luma_m;
  float edge_luma = paired_luma * 0.5;
  vec2 edge_origin = uv;
  if (horizontal_span) {
    edge_origin.y += length_sign * 0.5;
  } else {
    edge_origin.x += length_sign * 0.5;
  }
  vec2 search_offset = horizontal_span ? vec2(inverse_extent.x, 0.0)
                                       : vec2(0.0, inverse_extent.y);
  vec2 negative_position = edge_origin - search_offset;
  vec2 positive_position = edge_origin + search_offset;
  float negative_end_luma =
      luma(max(textureLod(source_image, negative_position, 0.0).rgb, vec3(0.0))) - edge_luma;
  float positive_end_luma =
      luma(max(textureLod(source_image, positive_position, 0.0).rgb, vec3(0.0))) - edge_luma;
  float scaled_gradient = gradient * 0.25;
  bool negative_done = abs(negative_end_luma) >= scaled_gradient;
  bool positive_done = abs(positive_end_luma) >= scaled_gradient;

  if (!negative_done) {
    negative_position -= search_offset * 1.5;
  }
  if (!positive_done) {
    positive_position += search_offset * 1.5;
  }
  if (!negative_done || !positive_done) {
    if (!negative_done) {
      negative_end_luma =
          luma(max(textureLod(source_image, negative_position, 0.0).rgb, vec3(0.0))) - edge_luma;
      negative_done = abs(negative_end_luma) >= scaled_gradient;
    }
    if (!positive_done) {
      positive_end_luma =
          luma(max(textureLod(source_image, positive_position, 0.0).rgb, vec3(0.0))) - edge_luma;
      positive_done = abs(positive_end_luma) >= scaled_gradient;
    }
    if (!negative_done) {
      negative_position -= search_offset * 2.0;
    }
    if (!positive_done) {
      positive_position += search_offset * 2.0;
    }
    if (!negative_done || !positive_done) {
      if (!negative_done) {
        negative_end_luma =
            luma(max(textureLod(source_image, negative_position, 0.0).rgb, vec3(0.0))) - edge_luma;
        negative_done = abs(negative_end_luma) >= scaled_gradient;
      }
      if (!positive_done) {
        positive_end_luma =
            luma(max(textureLod(source_image, positive_position, 0.0).rgb, vec3(0.0))) - edge_luma;
        positive_done = abs(positive_end_luma) >= scaled_gradient;
      }
      if (!negative_done) {
        negative_position -= search_offset * 4.0;
      }
      if (!positive_done) {
        positive_position += search_offset * 4.0;
      }
      if (!negative_done || !positive_done) {
        if (!negative_done) {
          negative_end_luma =
              luma(max(textureLod(source_image, negative_position, 0.0).rgb, vec3(0.0))) - edge_luma;
          negative_done = abs(negative_end_luma) >= scaled_gradient;
        }
        if (!positive_done) {
          positive_end_luma =
              luma(max(textureLod(source_image, positive_position, 0.0).rgb, vec3(0.0))) - edge_luma;
          positive_done = abs(positive_end_luma) >= scaled_gradient;
        }
        if (!negative_done) {
          negative_position -= search_offset * 12.0;
        }
        if (!positive_done) {
          positive_position += search_offset * 12.0;
        }
      }
    }
  }

  float negative_distance = horizontal_span ? uv.x - negative_position.x
                                            : uv.y - negative_position.y;
  float positive_distance = horizontal_span ? positive_position.x - uv.x
                                            : positive_position.y - uv.y;
  bool nearest_is_negative = negative_distance < positive_distance;
  bool center_is_darker = luma_m - edge_luma < 0.0;
  bool nearest_has_valid_span = nearest_is_negative
                                    ? ((negative_end_luma < 0.0) != center_is_darker)
                                    : ((positive_end_luma < 0.0) != center_is_darker);
  float nearest_distance = min(negative_distance, positive_distance);
  float edge_offset = 0.5 - nearest_distance / (negative_distance + positive_distance);

  float neighborhood_average =
      ((luma_ns + luma_we) * 2.0 + luma_nw_sw + luma_ne_se) * 0.0833333333;
  float subpixel = clamp(abs(neighborhood_average - luma_m) / luma_range, 0.0, 1.0);
  subpixel = (-2.0 * subpixel + 3.0) * subpixel * subpixel;
  subpixel = subpixel * subpixel * 0.25;
  float pixel_offset = max(nearest_has_valid_span ? edge_offset : 0.0, subpixel);
  vec2 resolved_uv = uv;
  if (horizontal_span) {
    resolved_uv.y += pixel_offset * length_sign;
  } else {
    resolved_uv.x += pixel_offset * length_sign;
  }
  return vec4(textureLod(source_image, resolved_uv, 0.0).rgb, center.a);
}

// FusionShaders adds zero-mean noise immediately before its 8-bit post-effect
// output. Keep the same encoded-space amplitude here: one half of an 8-bit
// code step in either direction. Anchoring the hash to the output pixel keeps
// the pattern stable between frames.
float display_dither(ivec2 coordinate) {
  float noise = fract(dot(vec2(coordinate), vec2(0.75487766, 0.56984029)));
  return noise * (1.0 / 255.0) - (0.5 / 255.0);
}

vec3 dither_encoded_color(vec3 color, ivec2 coordinate) {
  return clamp(color + vec3(display_dither(coordinate)), vec3(0.0), vec3(1.0));
}

vec3 linear_to_srgb(vec3 color) {
  bvec3 use_linear_segment = lessThanEqual(color, vec3(0.0031308));
  vec3 linear_segment = color * 12.92;
  vec3 power_segment = 1.055 * pow(color, vec3(1.0 / 2.4)) - 0.055;
  return mix(power_segment, linear_segment, use_linear_segment);
}

vec3 srgb_to_linear(vec3 color) {
  bvec3 use_linear_segment = lessThanEqual(color, vec3(0.04045));
  vec3 linear_segment = color / 12.92;
  vec3 power_segment = pow((color + 0.055) / 1.055, vec3(2.4));
  return mix(power_segment, linear_segment, use_linear_segment);
}

void main() {
  ivec2 destination_coordinate = ivec2(gl_FragCoord.xy);
  vec2 source_position =
      gl_FragCoord.xy * vec2(present_constants.source_extent) /
      vec2(present_constants.destination_extent);
  ivec2 coordinate = clamp(ivec2(source_position), ivec2(0),
                           present_constants.source_extent - ivec2(1));
  vec4 source = (present_constants.output_mode & 16u) != 0u
                    ? resolve_fxaa(coordinate)
                    : (present_constants.output_mode & 2u) != 0u
                          ? resolve_spatial_edge(coordinate)
                          : fetch_source(coordinate);
  bool source_is_srgb_encoded = (present_constants.output_mode & 4u) != 0u;
  bool output_dither_enabled = (present_constants.output_mode & 8u) != 0u;
  vec3 source_color = max(source.rgb, vec3(0.0));
  vec3 encoded_source_color = clamp(source_color, vec3(0.0), vec3(1.0));
  if (source_is_srgb_encoded && output_dither_enabled) {
    encoded_source_color =
        dither_encoded_color(encoded_source_color, destination_coordinate);
  }
  vec3 linear_color = source_is_srgb_encoded
                          ? srgb_to_linear(encoded_source_color)
                          : source_color;

  if ((present_constants.output_mode & 1u) != 0u) {
    // Apple EDR and VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT define 1.0 as
    // SDR white. Preserve GTA IV's FP16 highlight energy above that point,
    // limiting only values that exceed the display's current headroom.
    float maximum_component = max(max(linear_color.r, linear_color.g), linear_color.b);
    if (maximum_component > present_constants.hdr_headroom) {
      linear_color *= present_constants.hdr_headroom / maximum_component;
    }
    output_color = vec4(linear_color, source.a);
  } else {
    // GTA IV's resolved 8-bit frontbuffer is already sRGB-encoded. Preserve it
    // exactly for SDR output. Only an actual linear FP16 source needs encoding.
    vec3 sdr_color = source_is_srgb_encoded
                         ? encoded_source_color
                         : linear_to_srgb(clamp(linear_color, vec3(0.0), vec3(1.0)));
    if (!source_is_srgb_encoded && output_dither_enabled) {
      sdr_color = dither_encoded_color(sdr_color, coordinate);
    }
    output_color = vec4(sdr_color, source.a);
  }
}
