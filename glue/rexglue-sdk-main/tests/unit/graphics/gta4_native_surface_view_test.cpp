#include <catch2/catch_test_macros.hpp>

#include <array>

#include <rex/graphics/gta4_native/surface_view.h>

namespace rex::graphics::gta4_native {
namespace {

SurfaceDescriptor MakeSurface(uint32_t surface_info, uint32_t color_info, uint32_t width,
                              uint32_t height) {
  SurfaceDescriptor descriptor{};
  descriptor.handle = 1;
  descriptor.base = surface_info;
  descriptor.address = color_info;
  descriptor.width = width;
  descriptor.height = height;
  descriptor.sample_type = DecodeSurfaceSampleType(surface_info);
  return descriptor;
}

TEST_CASE("GTA IV native surface info decodes guest MSAA from the upper halfword") {
  CHECK(DecodeSurfaceSampleType(0x00800050u) == uint32_t(xenos::MsaaSamples::k1X));
  CHECK(DecodeSurfaceSampleType(0x00420028u) == uint32_t(xenos::MsaaSamples::k4X));
}

TEST_CASE("GTA IV native alternate views share a normalized placement key") {
  GuestSurfaceView full_view{};
  GuestSurfaceView multisample_view{};
  REQUIRE(DecodeGuestSurfaceView(MakeSurface(0x00800050u, 0x03F30001u, 2560, 1440),
                                 false, full_view));
  REQUIRE(DecodeGuestSurfaceView(MakeSurface(0x00420028u, 0x00030001u, 1280, 720),
                                 false, multisample_view));
  CHECK(GetGuestPlacementKey(full_view) == GetGuestPlacementKey(multisample_view));
  CHECK(full_view.msaa_samples == xenos::MsaaSamples::k1X);
  CHECK(multisample_view.msaa_samples == xenos::MsaaSamples::k4X);
}

TEST_CASE("GTA IV native placement keys preserve odd sample-space extents") {
  GuestSurfaceView full_view{};
  GuestSurfaceView multisample_view{};
  REQUIRE(DecodeGuestSurfaceView(MakeSurface(0x00800050u, 0x03F30001u, 1729, 1085),
                                 false, full_view));
  REQUIRE(DecodeGuestSurfaceView(MakeSurface(0x00420028u, 0x00030001u, 865, 543),
                                 false, multisample_view));
  CHECK(GetGuestPlacementKey(full_view) != GetGuestPlacementKey(multisample_view));
  CHECK(multisample_view.sample_width == 1730);
  CHECK(multisample_view.sample_height == 1086);
}

TEST_CASE("GTA IV native reflection mip extents preserve host resolution scaling") {
  const std::array<uint32_t, 4> expected_logical = {256, 128, 64, 32};
  const std::array<uint32_t, 4> expected_physical = {1024, 512, 256, 128};
  for (uint32_t level = 0; level < expected_logical.size(); ++level) {
    CHECK(CalculateNativeMipExtent(256, level) == expected_logical[level]);
    CHECK(CalculateNativeMipExtent(1024, level) == expected_physical[level]);
  }
  CHECK(CalculateNativeMipExtent(1, 31) == 1);
  CHECK(CalculateNativeMipExtent(1, 32) == 1);
}

TEST_CASE("GTA IV native resolve flags reproduce generated default sample selection") {
  CHECK(DecodeResolveSampleSelect(NormalizeResolveSampleFlags(0, 0)) ==
        xenos::CopySampleSelect::k0);
  CHECK(DecodeResolveSampleSelect(NormalizeResolveSampleFlags(0, 1)) ==
        xenos::CopySampleSelect::k01);
  CHECK(DecodeResolveSampleSelect(NormalizeResolveSampleFlags(0, 2)) ==
        xenos::CopySampleSelect::k0123);
  CHECK(DecodeResolveSampleSelect(NormalizeResolveSampleFlags(0x30u, 2)) ==
        xenos::CopySampleSelect::k2);
}

TEST_CASE("GTA IV native resolve sample selection preserves Xenos depth rules") {
  CHECK(SanitizeGuestCopySampleSelect(xenos::CopySampleSelect::k0123,
                                     xenos::MsaaSamples::k4X, false) ==
        xenos::CopySampleSelect::k0123);
  CHECK(SanitizeGuestCopySampleSelect(xenos::CopySampleSelect::k0123,
                                     xenos::MsaaSamples::k4X, true) ==
        xenos::CopySampleSelect::k0);
  CHECK(SanitizeGuestCopySampleSelect(xenos::CopySampleSelect::k23,
                                     xenos::MsaaSamples::k4X, true) ==
        xenos::CopySampleSelect::k2);
}

}  // namespace
}  // namespace rex::graphics::gta4_native
