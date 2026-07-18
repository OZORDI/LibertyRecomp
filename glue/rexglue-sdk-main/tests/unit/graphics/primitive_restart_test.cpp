#include <array>
#include <cstddef>
#include <cstdint>

#include <catch2/catch_test_macros.hpp>

#include <rex/graphics/primitive_restart.h>

namespace rex::graphics {

TEST_CASE("disabled primitive restart sentinel detection", "[graphics][primitive_restart]") {
  const std::array<uint16_t, 3> indices_16_without = {0, 1, 2};
  const std::array<uint16_t, 3> indices_16_with = {0, UINT16_MAX, 2};
  CHECK_FALSE(primitive_restart::IsDisabledRestartSentinelUsed(indices_16_without.data(),
                                                               indices_16_without.size()));
  CHECK(primitive_restart::IsDisabledRestartSentinelUsed(indices_16_with.data(),
                                                         indices_16_with.size()));

  const std::array<uint32_t, 3> indices_32_without = {0, xenos::kVertexIndexMask, 2};
  const std::array<uint32_t, 3> indices_32_with = {0, UINT32_MAX, 2};
  CHECK_FALSE(primitive_restart::IsDisabledRestartSentinelUsed(indices_32_without.data(),
                                                               indices_32_without.size()));
  CHECK(primitive_restart::IsDisabledRestartSentinelUsed(indices_32_with.data(),
                                                         indices_32_with.size()));
}

TEST_CASE("disabled 16-bit restart indices are promoted and endian-normalized",
          "[graphics][primitive_restart]") {
  constexpr std::array<xenos::Endian, 2> endian_modes = {xenos::Endian::kNone,
                                                         xenos::Endian::k8in16};
  constexpr std::array<uint16_t, 3> source = {0x1234, UINT16_MAX, 0xABCD};
  for (xenos::Endian endian : endian_modes) {
    std::array<uint32_t, source.size()> destination = {};
    primitive_restart::ConvertDisabledRestartIndices(destination.data(), source.data(),
                                                     source.size(), endian);
    for (std::size_t i = 0; i < source.size(); ++i) {
      CHECK(destination[i] == xenos::GpuSwap(source[i], endian));
      CHECK(destination[i] != UINT32_MAX);
    }
  }
}

TEST_CASE("disabled 32-bit restart indices are endian-normalized and Xenos-masked",
          "[graphics][primitive_restart]") {
  constexpr std::array<xenos::Endian, 4> endian_modes = {
      xenos::Endian::kNone, xenos::Endian::k8in16, xenos::Endian::k8in32, xenos::Endian::k16in32};
  constexpr std::array<uint32_t, 3> source = {0x12345678, UINT32_MAX, 0xABCDEF01};
  for (xenos::Endian endian : endian_modes) {
    std::array<uint32_t, source.size()> destination = {};
    primitive_restart::ConvertDisabledRestartIndices(destination.data(), source.data(),
                                                     source.size(), endian);
    for (std::size_t i = 0; i < source.size(); ++i) {
      CHECK(destination[i] == (xenos::GpuSwap(source[i], endian) & xenos::kVertexIndexMask));
      CHECK(destination[i] != UINT32_MAX);
    }
  }
}

}  // namespace rex::graphics
