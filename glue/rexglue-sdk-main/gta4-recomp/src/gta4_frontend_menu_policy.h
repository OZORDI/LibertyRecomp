#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace gta4::frontend_menu::policy {

constexpr uint16_t kRetailListSlotCapacity = 20;
constexpr std::size_t kInlineKeyCapacity = 16;

constexpr bool FitsInlineKey(std::size_t size) noexcept {
  return size < kInlineKeyCapacity;
}

enum class Page : uint8_t {
  kDisabled,
  kPrimary,
  kAdvanced,
};

enum class PauseTabDirection : int8_t {
  kPrevious = -1,
  kNext = 1,
};

enum class PauseTabShoulderEdge : uint8_t {
  kNone,
  kPrevious,
  kNext,
  kSimultaneous,
};

constexpr std::array<uint32_t, 7> kPauseTabScreens = {3, 4, 5, 6, 7, 8, 9};

constexpr bool IsPauseTabScreen(uint32_t screen) noexcept {
  for (uint32_t candidate : kPauseTabScreens) {
    if (candidate == screen) {
      return true;
    }
  }
  return false;
}

constexpr PauseTabShoulderEdge ClassifyPauseTabShoulderEdge(
    bool previous_left_down, bool previous_right_down, bool left_down,
    bool right_down) noexcept {
  const bool left_pressed = left_down && !previous_left_down;
  const bool right_pressed = right_down && !previous_right_down;
  if (left_pressed && right_pressed) {
    return PauseTabShoulderEdge::kSimultaneous;
  }
  if (left_pressed) {
    return PauseTabShoulderEdge::kPrevious;
  }
  if (right_pressed) {
    return PauseTabShoulderEdge::kNext;
  }
  return PauseTabShoulderEdge::kNone;
}

constexpr bool ResolvePauseTab(uint32_t current_screen, PauseTabDirection direction,
                               uint32_t& target_screen) noexcept {
  for (std::size_t index = 0; index < kPauseTabScreens.size(); ++index) {
    if (kPauseTabScreens[index] != current_screen) {
      continue;
    }
    if (direction == PauseTabDirection::kPrevious) {
      target_screen = index == 0 ? kPauseTabScreens.back() : kPauseTabScreens[index - 1];
    } else {
      target_screen = index + 1 == kPauseTabScreens.size() ? kPauseTabScreens.front()
                                                           : kPauseTabScreens[index + 1];
    }
    return true;
  }
  return false;
}

struct StockLayout {
  uint32_t rows = 0;
  uint16_t count = 0;
  uint16_t capacity = 0;
  uint16_t sentinel_index = 0;
};

struct NativePageSlice {
  std::size_t first_item = 0;
  std::size_t item_count = 0;
  bool has_previous = false;
  bool has_next = false;

  constexpr std::size_t RowCount() const noexcept {
    constexpr std::size_t kSaveBackAndSentinelRows = 3;
    return item_count + kSaveBackAndSentinelRows + (has_previous ? 1 : 0) +
           (has_next ? 1 : 0);
  }
};

constexpr NativePageSlice PlanNativePage(std::size_t total_items,
                                         std::size_t first_item) noexcept {
  if (first_item >= total_items) {
    return {};
  }
  constexpr std::size_t kSaveBackAndSentinelRows = 3;
  const bool has_previous = first_item != 0;
  const std::size_t remaining = total_items - first_item;
  const std::size_t last_page_capacity =
      kRetailListSlotCapacity - kSaveBackAndSentinelRows - (has_previous ? 1 : 0);
  const bool has_next = remaining > last_page_capacity;
  const std::size_t item_capacity = last_page_capacity - (has_next ? 1 : 0);
  return {
      .first_item = first_item,
      .item_count = remaining < item_capacity ? remaining : item_capacity,
      .has_previous = has_previous,
      .has_next = has_next,
  };
}

constexpr bool CanInstallPrimary(const StockLayout& layout) noexcept {
  return layout.rows != 0 && layout.count != 0 && layout.sentinel_index + 1 == layout.count &&
         layout.count <= layout.capacity && layout.count + 1 <= kRetailListSlotCapacity;
}

constexpr bool CanBuildAdvanced(std::size_t row_count) noexcept {
  return row_count != 0 && row_count <= kRetailListSlotCapacity;
}

struct SelectionViewport {
  std::size_t first = 0;
  std::size_t end = 0;

  constexpr std::size_t Size() const noexcept { return end - first; }
};

constexpr bool IsSliderRowVisible(int32_t row, std::size_t first,
                                  std::size_t visible_capacity) noexcept {
  return row >= 0 && static_cast<std::size_t>(row) >= first &&
         static_cast<std::size_t>(row) - first < visible_capacity;
}

inline double SliderStartY(double row_height, double top,
                           std::size_t first) noexcept {
  // Match retail's single-precision row arithmetic before the draw helper
  // applies the screen/aspect transform to the slider rectangles.
  return double(float(float(row_height + top) - float(row_height * double(first))));
}

constexpr SelectionViewport FollowSelectionViewport(std::size_t item_count,
                                                     std::size_t visible_capacity,
                                                     std::size_t selected,
                                                     std::size_t previous_first) noexcept {
  if (item_count == 0 || visible_capacity == 0) {
    return {};
  }
  if (visible_capacity >= item_count) {
    return {.first = 0, .end = item_count};
  }

  const std::size_t clamped_selected = selected < item_count ? selected : item_count - 1;
  const std::size_t maximum_first = item_count - visible_capacity;
  std::size_t first = previous_first < maximum_first ? previous_first : maximum_first;
  if (clamped_selected < first) {
    first = clamped_selected;
  } else if (clamped_selected - first >= visible_capacity) {
    first = clamped_selected - visible_capacity + 1;
  }
  return {.first = first, .end = first + visible_capacity};
}

constexpr bool OwnsDescriptor(Page page, uint32_t published_rows, uint32_t primary_rows,
                              uint32_t advanced_rows) noexcept {
  switch (page) {
    case Page::kPrimary:
      return primary_rows != 0 && published_rows == primary_rows;
    case Page::kAdvanced:
      return advanced_rows != 0 && published_rows == advanced_rows;
    case Page::kDisabled:
      return false;
  }
  return false;
}

constexpr bool ShouldFreeGuestAllocation(bool owns_published_descriptor,
                                         uint32_t allocation) noexcept {
  return owns_published_descriptor && allocation != 0;
}

constexpr bool ShouldRestorePrimaryBeforeSwitch(Page page, uint32_t target_screen,
                                                uint32_t display_screen) noexcept {
  return page == Page::kAdvanced && target_screen != display_screen;
}

}  // namespace gta4::frontend_menu::policy
