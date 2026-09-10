#include "input/context_touch_layout.h"

#include <array>

#include <catch2/catch_test_macros.hpp>

namespace gta4::input {
namespace {

ContextTouchViewport TestViewport() {
  return {
      .output_width = 1280.0f,
      .output_height = 720.0f,
      .physical_output_x = 64.0f,
      .physical_output_y = 36.0f,
      .physical_output_width = 1920.0f,
      .physical_output_height = 1080.0f,
      .physical_surface_width = 2048.0f,
      .physical_surface_height = 1152.0f,
      .safe_x = 32.0f,
      .safe_y = 18.0f,
      .safe_width = 1216.0f,
      .safe_height = 684.0f,
      .generation = 7,
      .valid = true,
      .focused = true,
  };
}

size_t CountKind(const ContextTouchLayout& layout, ContextTouchControlKind kind) {
  size_t count = 0;
  for (size_t index = 0; index < layout.control_count; ++index) {
    if (layout.controls[index].kind == kind) {
      ++count;
    }
  }
  return count;
}

const ContextTouchControl* FindKind(const ContextTouchLayout& layout,
                                    ContextTouchControlKind kind) {
  for (size_t index = 0; index < layout.control_count; ++index) {
    if (layout.controls[index].kind == kind) {
      return &layout.controls[index];
    }
  }
  return nullptr;
}

bool HasKey(const ContextTouchLayout& layout, rex::ui::VirtualKey key) {
  for (size_t index = 0; index < layout.control_count; ++index) {
    if (layout.controls[index].kind == ContextTouchControlKind::kButton &&
        layout.controls[index].key == key) {
      return true;
    }
  }
  return false;
}

TEST_CASE("context touch layout is hidden for invalid presentation") {
  ContextTouchViewport viewport = TestViewport();
  viewport.focused = false;
  const auto layout = BuildContextTouchLayout(ContextTouchMode::kOnFoot, viewport);
  CHECK(layout.control_count == 0);
}

TEST_CASE("context touch layouts preserve proven context families") {
  const ContextTouchViewport viewport = TestViewport();
  const auto on_foot = BuildContextTouchLayout(ContextTouchMode::kOnFoot, viewport);
  CHECK(CountKind(on_foot, ContextTouchControlKind::kMovementStick) == 1);
  CHECK(CountKind(on_foot, ContextTouchControlKind::kLookSurface) == 1);

  const auto phone = BuildContextTouchLayout(ContextTouchMode::kPhone, viewport);
  CHECK(CountKind(phone, ContextTouchControlKind::kMovementStick) == 1);
  CHECK(CountKind(phone, ContextTouchControlKind::kLookSurface) == 0);

  const auto passenger = BuildContextTouchLayout(ContextTouchMode::kVehiclePassenger, viewport);
  CHECK(CountKind(passenger, ContextTouchControlKind::kMovementStick) == 0);
  CHECK(CountKind(passenger, ContextTouchControlKind::kLookSurface) == 1);

  const auto helicopter = BuildContextTouchLayout(ContextTouchMode::kVehicleHelicopter, viewport);
  CHECK(CountKind(helicopter, ContextTouchControlKind::kMovementStick) == 1);

  const auto unknown_driver =
      BuildContextTouchLayout(ContextTouchMode::kVehicleDriverUnknown, viewport);
  CHECK(CountKind(unknown_driver, ContextTouchControlKind::kMovementStick) == 0);
  CHECK(CountKind(unknown_driver, ContextTouchControlKind::kLookSurface) == 1);
  CHECK(HasKey(unknown_driver, rex::ui::VirtualKey::kF));
  CHECK(HasKey(unknown_driver, rex::ui::VirtualKey::kCapital));
  CHECK_FALSE(HasKey(unknown_driver, rex::ui::VirtualKey::kW));

  const auto parachute = BuildContextTouchLayout(ContextTouchMode::kParachuteDeployed, viewport);
  CHECK(CountKind(parachute, ContextTouchControlKind::kMovementStick) == 1);
  CHECK(CountKind(parachute, ContextTouchControlKind::kLookSurface) == 0);
}

TEST_CASE("minigame controls only expose recently observed query kinds") {
  const ContextTouchViewport viewport = TestViewport();
  const auto empty = BuildContextTouchLayout(ContextTouchMode::kMinigame, viewport);
  CHECK(empty.control_count == 0);

  constexpr std::array queries = {
      TouchScriptControl{TouchScriptQueryKind::kAnalogueSticks, 0},
      TouchScriptControl{TouchScriptQueryKind::kControlPressed, 17},
  };
  const auto learned = BuildContextTouchLayout(ContextTouchMode::kMinigame, viewport, queries);
  CHECK(CountKind(learned, ContextTouchControlKind::kScriptButton) == 1);
  CHECK(CountKind(learned, ContextTouchControlKind::kMovementStick) == 1);

  constexpr std::array button_only = {
      TouchScriptControl{TouchScriptQueryKind::kControlPressed, 17},
  };
  const auto without_stick =
      BuildContextTouchLayout(ContextTouchMode::kMinigame, viewport, button_only);
  const ContextTouchControl* learned_button =
      FindKind(learned, ContextTouchControlKind::kScriptButton);
  const ContextTouchControl* only_button =
      FindKind(without_stick, ContextTouchControlKind::kScriptButton);
  REQUIRE(learned_button);
  REQUIRE(only_button);
  CHECK(learned_button->center_x == only_button->center_x);
  CHECK(learned_button->center_y == only_button->center_y);
}

TEST_CASE("touch axes preserve sign and clamp to GTA action extent") {
  CHECK(ContextTouchAxis(0.0f, 100.0f) == 0);
  CHECK(ContextTouchAxis(-100.0f, 100.0f) == -255);
  CHECK(ContextTouchAxis(100.0f, 100.0f) == 255);
  CHECK(ContextTouchAxis(1000.0f, 100.0f) == 255);
  CHECK(ContextTouchAxis(1.0f, 0.0f) == 0);
}

TEST_CASE("touch key latch keeps same-action owners and sub-poll taps") {
  ContextTouchKeyLatch latch;
  constexpr uint64_t epoch = 41;
  latch.Press(rex::ui::VirtualKey::kSpace, epoch);
  latch.Press(rex::ui::VirtualKey::kSpace, epoch);
  latch.Release(rex::ui::VirtualKey::kSpace);

  std::array<uint8_t, 256> down{};
  std::array<uint8_t, 256> pressed{};
  latch.Collect(epoch, down, pressed);
  const size_t space = static_cast<uint16_t>(rex::ui::VirtualKey::kSpace);
  CHECK(down[space] == 1);
  CHECK(pressed[space] == 1);

  latch.Release(rex::ui::VirtualKey::kSpace);
  down.fill(0);
  pressed.fill(0);
  latch.Collect(epoch, down, pressed);
  CHECK(down[space] == 1);
  CHECK(pressed[space] == 1);
  down.fill(0);
  pressed.fill(0);
  latch.Collect(epoch + 1, down, pressed);
  CHECK(down[space] == 0);
  CHECK(pressed[space] == 0);

  latch.Cancel();
  pressed.fill(0);
  latch.Collect(epoch, down, pressed);
  CHECK(pressed[space] == 0);
}

TEST_CASE("physical output transform changes invalidate a touch layout") {
  const ContextTouchViewport viewport = TestViewport();
  const auto first = BuildContextTouchLayout(ContextTouchMode::kOnFoot, viewport);
  ContextTouchViewport moved = viewport;
  moved.physical_output_x += 1.0f;
  const auto second = BuildContextTouchLayout(ContextTouchMode::kOnFoot, moved);
  CHECK_FALSE(ContextTouchLayoutEquivalent(first, second));
}

TEST_CASE("touch overlay transform preserves host output letterboxing") {
  const ContextTouchViewport viewport = TestViewport();
  const ContextTouchOverlayTransform transform =
      BuildContextTouchOverlayTransform(viewport, 1024.0f, 576.0f);
  REQUIRE(transform.valid);
  CHECK(transform.offset_x == 32.0f);
  CHECK(transform.offset_y == 18.0f);
  CHECK(transform.scale_x == 0.75f);
  CHECK(transform.scale_y == 0.75f);

  ContextTouchViewport invalid = viewport;
  invalid.physical_surface_width = 0.0f;
  CHECK_FALSE(BuildContextTouchOverlayTransform(invalid, 1024.0f, 576.0f).valid);
}

}  // namespace
}  // namespace gta4::input
