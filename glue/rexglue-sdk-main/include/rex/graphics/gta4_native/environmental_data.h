#pragma once

#include <array>
#include <cstdint>
#include <type_traits>

namespace rex::graphics::gta4_native {

inline constexpr uint32_t kEnvironmentalDataVersion = 1;

enum class EnvironmentalField : uint64_t {
  kTimeStepSeconds = 0x0000000000000001,
  kMotionBlurScale = 0x0000000000000002,
  kDirectionalMotionBlurLength = 0x0000000000000004,
  kFogStart = 0x0000000000000008,
  kFogDensity = 0x0000000000000010,
  kFogHeightFalloff = 0x0000000000000020,
  kFogAltitudeTweak = 0x0000000000000040,
  kFogPower = 0x0000000000000080,
  kFogColor = 0x0000000000000100,
  kSunDirection = 0x0000000000000200,
  kSunColor = 0x0000000000000400,
  kViewMatrix = 0x0000000000000800,
  kViewInverseMatrix = 0x0000000000001000,
  kProjectionMatrix = 0x0000000000002000,
  kViewProjectionMatrix = 0x0000000000004000,
  kCameraPosition = 0x0000000000008000,
  kCameraAltitude = 0x0000000000010000,
  kEffectSettings = 0x0000000000020000,
};

inline constexpr uint64_t kEnvironmentalFieldMask = 0x000000000003FFFF;

constexpr uint64_t EnvironmentalFieldBit(EnvironmentalField field) {
  return static_cast<uint64_t>(field);
}

struct EnvironmentalEffectSettingsV1 {
  uint32_t enabled_effects = 0;
  uint32_t motion_blur_quality = 0;
};

// Matrices preserve the guest's row-major float[4][4] storage. Consumers must
// perform an explicit convention conversion when binding them to shader blocks.
struct alignas(16) EnvironmentalDataV1 {
  uint32_t version = kEnvironmentalDataVersion;
  uint32_t byte_size = 0;
  uint64_t valid_fields = 0;
  uint64_t source_sequence = 0;

  float time_step_seconds = 0.0f;
  float motion_blur_scale = 0.0f;
  float directional_motion_blur_length = 0.0f;
  float fog_start = 0.0f;
  float fog_density = 0.0f;
  float fog_height_falloff = 0.0f;
  float fog_altitude_tweak = 0.0f;
  float fog_power = 0.0f;
  float camera_altitude = 0.0f;

  std::array<float, 4> fog_color{};
  std::array<float, 4> sun_direction{};
  std::array<float, 4> sun_color{};
  std::array<float, 4> camera_position{};

  std::array<float, 16> view_matrix{};
  std::array<float, 16> view_inverse_matrix{};
  std::array<float, 16> projection_matrix{};
  std::array<float, 16> view_projection_matrix{};

  EnvironmentalEffectSettingsV1 effect_settings{};
  std::array<uint32_t, 3> reserved{};
};

static_assert(std::is_trivially_copyable_v<EnvironmentalEffectSettingsV1>);
static_assert(std::is_trivially_copyable_v<EnvironmentalDataV1>);
static_assert(alignof(EnvironmentalDataV1) == 16);
static_assert(sizeof(EnvironmentalDataV1) == 400);

}  // namespace rex::graphics::gta4_native
