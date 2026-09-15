#include <catch2/catch_test_macros.hpp>

#include <array>
#include <bit>
#include <cstdint>

#include "graphics/gta4_native/native_texture_image_identity.h"

using namespace rex::graphics::gta4_native;
namespace xenos = rex::graphics::xenos;

TEST_CASE("Sampler changes reuse texture image identity without changing draw fetches",
          "[native-upload][texture]") {
  xenos::xe_gpu_texture_fetch_t original{};
  original.type = xenos::FetchConstantType::kTexture;
  original.dimension = xenos::DataDimension::k2DOrStacked;
  original.format = xenos::TextureFormat::k_DXT1;
  original.base_address = 7;
  original.mip_address = 8;
  original.mip_max_level = 4;
  original.size_2d.width = 255;
  original.size_2d.height = 255;
  const auto before = std::bit_cast<std::array<uint32_t, 6>>(original);

  auto filtered = original;
  filtered.min_filter = xenos::TextureFilter::kLinear;
  filtered.mag_filter = xenos::TextureFilter::kLinear;
  filtered.lod_bias = -32;
  filtered.clamp_x = xenos::ClampMode::kClampToEdge;
  CHECK(NativeTextureImageFetchEqual(original, filtered));
  CHECK(NativeTextureImageFetchEqual(filtered, original));
  CHECK(std::bit_cast<std::array<uint32_t, 6>>(original) == before);
  CHECK(filtered.min_filter == xenos::TextureFilter::kLinear);
  CHECK(filtered.lod_bias == -32);

  auto different_mips = filtered;
  different_mips.mip_max_level = 5;
  CHECK_FALSE(NativeTextureImageFetchEqual(original, different_mips));
  auto different_storage = filtered;
  different_storage.base_address = 9;
  CHECK_FALSE(NativeTextureImageFetchEqual(original, different_storage));
  auto different_view = filtered;
  different_view.swizzle = 1;
  CHECK_FALSE(NativeTextureImageFetchEqual(original, different_view));
}

TEST_CASE("Texture image identity preserves every storage and view bit",
          "[native-upload][texture]") {
  // Independently derived with Python from the bit positions in xenos.h.
  // Unknown/reserved bits remain significant; only documented sampling fields
  // may disappear from the image identity. This also covers all mip/address bits.
  constexpr std::array<uint32_t, 6> sampler_bits{
      0x0007FC00u, 0x00000800u, 0x00000000u,
      0x7FF80000u, 0xFFFFFC03u, 0x000001FBu};
  for (uint32_t initial : {0u, UINT32_MAX, 0xA5A5A5A5u}) {
    std::array<uint32_t, 6> words;
    words.fill(initial);
    const auto original = std::bit_cast<xenos::xe_gpu_texture_fetch_t>(words);
    for (size_t word = 0; word < words.size(); ++word) {
      for (uint32_t bit = 0; bit < 32; ++bit) {
        auto changed = words;
        const uint32_t mask = uint32_t(1) << bit;
        changed[word] ^= mask;
        const auto fetch = std::bit_cast<xenos::xe_gpu_texture_fetch_t>(changed);
        INFO("word=" << word << " bit=" << bit);
        CHECK(NativeTextureImageFetchEqual(original, fetch) ==
              bool(sampler_bits[word] & mask));
      }
    }
  }
}
