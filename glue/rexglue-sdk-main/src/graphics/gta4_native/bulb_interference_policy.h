#pragma once
// Observation selectors and bounded storage arithmetic. Not a rendering policy.
#include <cstddef>
#include <cstdint>
#include <limits>
#include <string_view>

namespace rex::graphics::gta4_native {
inline constexpr uint64_t kBulbFullReadbackBudget = uint64_t{384} * 1024 * 1024;
inline constexpr size_t kBulbFullReadbackCount = 80;
inline constexpr uint32_t kBulbFullMaximumExtent = 4096;
constexpr uint64_t BulbFullReadbackBytes(uint32_t width, uint32_t height,
                                       uint32_t bytes, uint32_t block_extent = 1) {
  if (!width || !height || width > kBulbFullMaximumExtent || height > kBulbFullMaximumExtent ||
      !bytes || bytes > 16 || (block_extent != 1 && block_extent != 4)) return 0;
  return ((uint64_t(width) + block_extent - 1) / block_extent) *
         ((uint64_t(height) + block_extent - 1) / block_extent) * bytes;
}
constexpr bool BulbFullBudgetAccepts(uint64_t used, uint64_t bytes, size_t count) {
  return bytes && used <= kBulbFullReadbackBudget && bytes <= kBulbFullReadbackBudget - used &&
         count < kBulbFullReadbackCount;
}
constexpr bool IsBulbInterferenceShader(uint64_t shader) {
  // Audited ceiling material, fog, exposure adaptation, and final composites.
  // Hashes select observation only; unexpected shaders still appear in the
  // existing sparse writer trace and are not assigned a shader contract here.
  return shader == 0xF634FCEBA607E8A5ull || shader == 0xE3C167BF0D3D05C6ull ||
         shader == 0x12AA6B4951968E28ull || shader == 0x5D2A71133DA823F7ull ||
         shader == 0x9649029E1999BEA9ull;
}
constexpr bool IsBulbCoronaShader(uint64_t shader) {
  return shader == 0x62DFF2DBDC8ED5D6ull || shader == 0x485034673633456Eull;
}
constexpr bool IsBulbInterferenceFamily(uint64_t shader, std::string_view filename) {
  // Trace the complete postfx family, not just the previously suspected final
  // pass. Includes luminance reduction, adaptation, bloom, blur and composite.
  return IsBulbInterferenceShader(shader) || IsBulbCoronaShader(shader) || shader == 0x07675BF8EF5E48DCull ||
         filename == "shader/rage_shaders/rage_postfx_e2.fxc";
}
} // namespace rex::graphics::gta4_native
