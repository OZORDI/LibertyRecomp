#include <catch2/catch_test_macros.hpp>

#include "graphics/gta4_native/native_reflection_registry.h"

namespace rex::graphics::gta4_native {
namespace {

NativeReflectionTarget MakeWaterColorRegistration() {
  NativeReflectionTarget target{};
  target.family = ReflectionFamily::kWater;
  target.role = ReflectionRole::kColor;
  target.wrapper = 0x1000;
  target.surface = 0x2000;
  target.texture = 0x3000;
  target.logical_width = 320;
  target.logical_height = 180;
  target.physical_width = 1920;
  target.physical_height = 1080;
  return target;
}

TEST_CASE("GTA IV native reflection release retires paired aliases") {
  NativeReflectionRegistry registry;
  const NativeReflectionTarget target = MakeWaterColorRegistration();
  CHECK(RegisterNativeReflectionTarget(registry, target) == 0);
  CHECK(registry.size() == 2);

  CHECK(EraseNativeReflectionRegistration(registry, target.surface) == 2);
  CHECK(registry.empty());

  RegisterNativeReflectionTarget(registry, target);
  CHECK(EraseNativeReflectionRegistration(registry, target.texture) == 2);
  CHECK(registry.empty());
}

TEST_CASE("GTA IV native reflection registration replaces colliding identity") {
  NativeReflectionRegistry registry;
  const NativeReflectionTarget water = MakeWaterColorRegistration();
  RegisterNativeReflectionTarget(registry, water);

  NativeReflectionTarget replacement = water;
  replacement.family = ReflectionFamily::kMirror;
  replacement.wrapper = 0x4000;
  replacement.texture = 0x5000;
  CHECK(RegisterNativeReflectionTarget(registry, replacement) == 2);
  CHECK_FALSE(registry.contains(water.texture));
  CHECK(registry.contains(replacement.surface));
  CHECK(registry.contains(replacement.texture));
}

TEST_CASE("GTA IV native reflection lookup validates role and logical extent") {
  NativeReflectionRegistry registry;
  const NativeReflectionTarget target = MakeWaterColorRegistration();
  RegisterNativeReflectionTarget(registry, target);

  SurfaceDescriptor surface{};
  surface.handle = target.surface;
  surface.width = target.logical_width;
  surface.height = target.logical_height;
  CHECK(FindNativeReflectionSurface(registry, surface, false) != nullptr);
  CHECK(FindNativeReflectionSurface(registry, surface, true) == nullptr);

  surface.width = 3456;
  surface.height = 2168;
  CHECK(FindNativeReflectionSurface(registry, surface, false) == nullptr);

  CHECK(FindNativeReflectionTexture(registry, target.texture,
                                    target.logical_width,
                                    target.logical_height) != nullptr);
  CHECK(FindNativeReflectionTexture(registry, target.texture, 3456, 2168) ==
        nullptr);
  CHECK(FindNativeReflectionTexture(registry, target.surface,
                                    target.logical_width,
                                    target.logical_height) == nullptr);
}

}  // namespace
}  // namespace rex::graphics::gta4_native
