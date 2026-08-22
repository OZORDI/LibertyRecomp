#include <catch2/catch_test_macros.hpp>

#include "graphics/gta4_native/native_msaa_policy.h"

namespace rex::graphics::gta4_native {
namespace {

TEST_CASE("GTA IV native MSAA overrides only title-multisampled scene families") {
  CHECK(ShouldApplyNativeSceneSampleOverride(false, true, true));

  CHECK_FALSE(ShouldApplyNativeSceneSampleOverride(false, true, false));
  CHECK_FALSE(ShouldApplyNativeSceneSampleOverride(false, false, true));
  CHECK_FALSE(ShouldApplyNativeSceneSampleOverride(true, true, true));
}

TEST_CASE("GTA IV native MSAA preserves mixed and forward sample topology") {
  // A family containing any guest 1x attachment is title-authored mixed or
  // forward topology and must remain untouched as a coherent family.
  CHECK_FALSE(ShouldApplyNativeSceneSampleOverride(false, true, false));

  // Reflection families own an independent sample-count policy.
  CHECK_FALSE(ShouldApplyNativeSceneSampleOverride(true, true, true));
}

TEST_CASE("GTA IV native MSAA selects the highest supported requested count") {
  CHECK(SelectSupportedNativeSceneSampleCount(4, 1 | 2 | 4) == 4);
  CHECK(SelectSupportedNativeSceneSampleCount(4, 1 | 2) == 2);
  CHECK(SelectSupportedNativeSceneSampleCount(4, 1) == 1);
  CHECK(SelectSupportedNativeSceneSampleCount(2, 1 | 2 | 4) == 2);
  CHECK(SelectSupportedNativeSceneSampleCount(2, 1) == 1);
  CHECK(SelectSupportedNativeSceneSampleCount(1, 1 | 2 | 4) == 1);
  CHECK(SelectSupportedNativeSceneSampleCount(4, 0) == 0);
}

}  // namespace
}  // namespace rex::graphics::gta4_native
