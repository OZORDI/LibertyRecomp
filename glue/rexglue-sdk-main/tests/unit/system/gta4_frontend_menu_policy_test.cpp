#include <catch2/catch_test_macros.hpp>

#include "../../../gta4-recomp/src/gta4_frontend_menu_policy.h"

namespace policy = gta4::frontend_menu::policy;

TEST_CASE("GTA IV frontend slider follows the visible rows",
          "[system][gta4][frontend][draw-distance]") {
  CHECK(policy::IsSliderRowVisible(3, 0, 12));
  CHECK(policy::IsSliderRowVisible(3, 3, 12));
  CHECK_FALSE(policy::IsSliderRowVisible(3, 4, 12));
  CHECK_FALSE(policy::IsSliderRowVisible(12, 0, 12));
  CHECK_FALSE(policy::IsSliderRowVisible(-1, 0, 12));
  CHECK_FALSE(policy::IsSliderRowVisible(0, 0, 0));
  CHECK(policy::SliderStartY(0.03125, 0.125, 0) == 0.15625);
  CHECK(policy::SliderStartY(0.03125, 0.125, 3) == 0.0625);
}

TEST_CASE("GTA IV frontend native keys fit the retail inline field",
          "[system][gta4][frontend]") {
  CHECK(policy::FitsInlineKey(0));
  CHECK(policy::FitsInlineKey(15));
  CHECK_FALSE(policy::FitsInlineKey(16));
  CHECK_FALSE(policy::FitsInlineKey(17));
}

TEST_CASE("GTA IV frontend primary copy requires a terminal sentinel and valid capacity",
          "[system][gta4][frontend]") {
  CHECK(policy::CanInstallPrimary(
      {.rows = 0x1000, .count = 12, .capacity = 16, .sentinel_index = 11}));
  CHECK_FALSE(
      policy::CanInstallPrimary({.rows = 0, .count = 12, .capacity = 16, .sentinel_index = 11}));
  CHECK(policy::CanInstallPrimary(
      {.rows = 0x1000, .count = 12, .capacity = 12, .sentinel_index = 11}));
  CHECK_FALSE(policy::CanInstallPrimary(
      {.rows = 0x1000, .count = 12, .capacity = 11, .sentinel_index = 11}));
  CHECK_FALSE(policy::CanInstallPrimary(
      {.rows = 0x1000, .count = 12, .capacity = 16, .sentinel_index = 10}));
  CHECK_FALSE(policy::CanInstallPrimary(
      {.rows = 0x1000, .count = 20, .capacity = 21, .sentinel_index = 19}));
}

TEST_CASE("GTA IV frontend advanced page never exceeds retail physical slots",
          "[system][gta4][frontend]") {
  CHECK(policy::CanBuildAdvanced(1));
  CHECK(policy::CanBuildAdvanced(20));
  CHECK_FALSE(policy::CanBuildAdvanced(0));
  CHECK_FALSE(policy::CanBuildAdvanced(21));
}

TEST_CASE("GTA IV frontend native options paginate within retail physical slots",
          "[system][gta4][frontend]") {
  const auto single = policy::PlanNativePage(17, 0);
  CHECK(single.first_item == 0);
  CHECK(single.item_count == 17);
  CHECK_FALSE(single.has_previous);
  CHECK_FALSE(single.has_next);
  CHECK(single.RowCount() == 20);

  const auto first = policy::PlanNativePage(33, 0);
  CHECK(first.item_count == 16);
  CHECK_FALSE(first.has_previous);
  CHECK(first.has_next);
  CHECK(first.RowCount() == 20);

  const auto middle = policy::PlanNativePage(33, 16);
  CHECK(middle.first_item == 16);
  CHECK(middle.item_count == 15);
  CHECK(middle.has_previous);
  CHECK(middle.has_next);
  CHECK(middle.RowCount() == 20);

  const auto last = policy::PlanNativePage(33, 31);
  CHECK(last.first_item == 31);
  CHECK(last.item_count == 2);
  CHECK(last.has_previous);
  CHECK_FALSE(last.has_next);
  CHECK(last.RowCount() == 6);

  CHECK(policy::PlanNativePage(0, 0).item_count == 0);
  CHECK(policy::PlanNativePage(33, 33).item_count == 0);
}

TEST_CASE("GTA IV frontend viewport follows selection without hard-coded row positions",
          "[system][gta4][frontend][scroll]") {
  CHECK(policy::FollowSelectionViewport(0, 5, 0, 0).Size() == 0);
  CHECK(policy::FollowSelectionViewport(15, 0, 0, 0).Size() == 0);

  const auto all_visible = policy::FollowSelectionViewport(5, 8, 4, 3);
  CHECK(all_visible.first == 0);
  CHECK(all_visible.end == 5);

  const auto initial = policy::FollowSelectionViewport(15, 5, 4, 0);
  CHECK(initial.first == 0);
  CHECK(initial.end == 5);

  const auto one_row_down = policy::FollowSelectionViewport(15, 5, 5, 0);
  CHECK(one_row_down.first == 1);
  CHECK(one_row_down.end == 6);

  const auto stable = policy::FollowSelectionViewport(15, 5, 10, 6);
  CHECK(stable.first == 6);
  CHECK(stable.end == 11);

  const auto one_row_up = policy::FollowSelectionViewport(15, 5, 5, 6);
  CHECK(one_row_up.first == 5);
  CHECK(one_row_up.end == 10);

  const auto clamped_end = policy::FollowSelectionViewport(15, 5, 99, 99);
  CHECK(clamped_end.first == 10);
  CHECK(clamped_end.end == 15);
}

TEST_CASE("GTA IV frontend descriptor ownership is page-specific", "[system][gta4][frontend]") {
  CHECK(policy::OwnsDescriptor(policy::Page::kPrimary, 0x1000, 0x1000, 0x2000));
  CHECK_FALSE(policy::OwnsDescriptor(policy::Page::kPrimary, 0x2000, 0x1000, 0x2000));
  CHECK(policy::OwnsDescriptor(policy::Page::kAdvanced, 0x2000, 0x1000, 0x2000));
  CHECK_FALSE(policy::OwnsDescriptor(policy::Page::kAdvanced, 0x1000, 0x1000, 0x2000));
  CHECK_FALSE(policy::OwnsDescriptor(policy::Page::kDisabled, 0x1000, 0x1000, 0x2000));
}

TEST_CASE("GTA IV frontend frees guest storage only while descriptor ownership is live",
          "[system][gta4][frontend]") {
  CHECK(policy::ShouldFreeGuestAllocation(true, 0x1000));
  CHECK_FALSE(policy::ShouldFreeGuestAllocation(false, 0x1000));
  CHECK_FALSE(policy::ShouldFreeGuestAllocation(true, 0));
}

TEST_CASE("GTA IV frontend restores primary only when advanced leaves Display",
          "[system][gta4][frontend]") {
  CHECK(policy::ShouldRestorePrimaryBeforeSwitch(policy::Page::kAdvanced, 7, 8));
  CHECK_FALSE(policy::ShouldRestorePrimaryBeforeSwitch(policy::Page::kAdvanced, 8, 8));
  CHECK_FALSE(policy::ShouldRestorePrimaryBeforeSwitch(policy::Page::kPrimary, 7, 8));
}

TEST_CASE("GTA IV pause tabs advance, reverse and wrap", "[system][gta4][frontend][input]") {
  uint32_t target = 0;
  CHECK(policy::ResolvePauseTab(3, policy::PauseTabDirection::kPrevious, target));
  CHECK(target == 9);
  CHECK(policy::ResolvePauseTab(9, policy::PauseTabDirection::kNext, target));
  CHECK(target == 3);
  CHECK(policy::ResolvePauseTab(7, policy::PauseTabDirection::kPrevious, target));
  CHECK(target == 6);
  CHECK(policy::ResolvePauseTab(7, policy::PauseTabDirection::kNext, target));
  CHECK(target == 8);
  CHECK_FALSE(policy::ResolvePauseTab(0, policy::PauseTabDirection::kNext, target));
}

TEST_CASE("GTA IV pause tab shoulders use rising edges", "[system][gta4][frontend][input]") {
  using Edge = policy::PauseTabShoulderEdge;
  CHECK(policy::ClassifyPauseTabShoulderEdge(false, false, false, false) == Edge::kNone);
  CHECK(policy::ClassifyPauseTabShoulderEdge(false, false, true, false) == Edge::kPrevious);
  CHECK(policy::ClassifyPauseTabShoulderEdge(false, false, false, true) == Edge::kNext);
  CHECK(policy::ClassifyPauseTabShoulderEdge(false, false, true, true) == Edge::kSimultaneous);
  CHECK(policy::ClassifyPauseTabShoulderEdge(true, false, true, false) == Edge::kNone);
  CHECK(policy::ClassifyPauseTabShoulderEdge(true, false, false, false) == Edge::kNone);
  CHECK(policy::ClassifyPauseTabShoulderEdge(false, false, true, false) == Edge::kPrevious);
}
