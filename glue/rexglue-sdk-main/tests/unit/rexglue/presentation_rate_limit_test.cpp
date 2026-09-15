#include <catch2/catch_test_macros.hpp>
#include <rex/ui/presentation_rate_limit.h>
#include <rex/ui/vulkan/present_mode_policy.h>
#include "gta4_present_mode_policy.h"
#include <array>
#include <limits>
#include <vector>

using rex::ui::PresentationRateLimit;

TEST_CASE("Host UI repaint storms obey the selected title cap", "[display-settings]") {
  for (uint32_t fps : {30u, 40u, 60u, 120u}) {
    PresentationRateLimit limiter;
    uint32_t presented = 0;
    const uint64_t duration = 10'000'000'000;
    // 1000 attempted host redraws per second, even if the title emits less often.
    for (uint64_t now = 1; now < duration; now += 1'000'000) {
      if (limiter.Delay(fps, now)) continue;
      limiter.Presented(now);
      ++presented;
    }
    REQUIRE(presented == fps * 10);
  }
}

TEST_CASE("Host cap spends slots only for successful submissions", "[display-settings]") {
  PresentationRateLimit limiter;
  REQUIRE(limiter.Delay(30, 1000) == 0);
  REQUIRE(limiter.Delay(30, 2000) == 0); // Failed acquisition did not commit.
  limiter.Presented(2000);
  REQUIRE(limiter.Delay(30, 2001) > 0);
  const uint64_t remaining = limiter.Delay(30, 4000);
  REQUIRE(remaining > 0);
  REQUIRE(limiter.Delay(30, 4000) == remaining); // A retry never advances it.
  REQUIRE(limiter.Delay(30, 4000 + remaining) == 0);
}

TEST_CASE("Host cap changes live and never throttles unlocked output", "[display-settings]") {
  PresentationRateLimit limiter;
  for (uint32_t fps : {30u, 40u, 60u, 120u, 0u, 30u, 0u}) {
    REQUIRE(limiter.Delay(fps, 100'000'000) == 0);
    limiter.Presented(100'000'000);
    if (fps) REQUIRE(limiter.Delay(fps, 100'000'001) > 0);
    else REQUIRE(limiter.Delay(fps, 100'000'001) == 0);
  }
}

TEST_CASE("Host cap handles stalls clock reset saturation and timer rounding", "[display-settings]") {
  PresentationRateLimit limiter;
  REQUIRE(limiter.Delay(30, 1000) == 0);
  limiter.Presented(1000);
  REQUIRE(limiter.Delay(30, 9'000'000'000) == 0);
  limiter.Presented(9'000'000'000);
  REQUIRE(limiter.Delay(30, 9'000'000'001) > 0);
  REQUIRE(limiter.Delay(30, 500) == 0);
  limiter.Presented(500);
  REQUIRE(limiter.Delay(30, 501) > 0);
  const auto limit = std::numeric_limits<uint64_t>::max();
  REQUIRE(limiter.Delay(30, limit - 1) == 0);
  limiter.Presented(limit - 1);
  REQUIRE(limiter.Delay(30, limit - 1) == 1);
  REQUIRE(PresentationRateLimit::DelayMilliseconds(0) == 1);
  REQUIRE(PresentationRateLimit::DelayMilliseconds(1) == 1);
  REQUIRE(PresentationRateLimit::DelayMilliseconds(1'000'000) == 1);
  REQUIRE(PresentationRateLimit::DelayMilliseconds(1'000'001) == 2);
  REQUIRE(PresentationRateLimit::DelayMilliseconds(limit) == 1000);
}

TEST_CASE("Host cap keeps rational cadence through small scheduling delays", "[display-settings]") {
  for (uint32_t fps : {30u, 40u, 60u, 120u}) {
    PresentationRateLimit limiter;
    uint64_t now = 1000;
    for (uint64_t i = 0; i < 600; ++i) {
      now += limiter.Delay(fps, now);
      REQUIRE(limiter.Delay(fps, now) == 0);
      limiter.Presented(now);
      now += 700'000; // Simulated work; the rest is an event-loop timer.
    }
    const uint64_t next = now + limiter.Delay(fps, now);
    REQUIRE(next == 1000 + 600ull * 1'000'000'000 / fps);
  }
}

TEST_CASE("VSync menu spellings reach the correct Vulkan mode", "[display-settings]") {
  using namespace rex::ui::vulkan;
  constexpr std::array available{VK_PRESENT_MODE_FIFO_KHR,
      VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_MAILBOX_KHR};
  for (auto mode : {"vsync", "fifo", "immediate", "mailbox"}) {
    const auto title = gta4::presentation::ResolveMode(mode);
    REQUIRE(title.explicit_mode);
    const auto actual = SelectPresentMode(
        {title.prefer_fifo, title.mailbox, title.immediate, false}, available);
    if (std::string_view(mode) == "immediate") {
      REQUIRE_FALSE(title.vsync);
      REQUIRE(actual == VK_PRESENT_MODE_IMMEDIATE_KHR);
    } else if (std::string_view(mode) == "mailbox") {
      REQUIRE(title.vsync);
      REQUIRE(actual == VK_PRESENT_MODE_MAILBOX_KHR);
    } else {
      REQUIRE(title.vsync);
      REQUIRE(actual == VK_PRESENT_MODE_FIFO_KHR);
    }
  }
}

TEST_CASE("Vulkan present selection never requests an unadvertised optional mode", "[display-settings]") {
  using namespace rex::ui::vulkan;
  constexpr std::array optional{VK_PRESENT_MODE_MAILBOX_KHR,
      VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_RELAXED_KHR};
  for (uint32_t supported_mask = 0; supported_mask < 8; ++supported_mask) {
    std::vector<VkPresentModeKHR> available{VK_PRESENT_MODE_FIFO_KHR};
    for (uint32_t bit = 0; bit < optional.size(); ++bit)
      if (supported_mask & (1u << bit)) available.push_back(optional[bit]);
    for (uint32_t options_mask = 0; options_mask < 16; ++options_mask) {
      const PresentModeOptions options{bool(options_mask & 1), bool(options_mask & 2),
                                       bool(options_mask & 4), bool(options_mask & 8)};
      const auto selected = SelectPresentMode(options, available);
      REQUIRE(std::find(available.begin(), available.end(), selected) != available.end());
      if (options.prefer_fifo) REQUIRE(selected == VK_PRESENT_MODE_FIFO_KHR);
    }
  }
  REQUIRE(SelectPresentMode({false, false, true, false}, {}) == VK_PRESENT_MODE_FIFO_KHR);
  REQUIRE(PresentModeOptions{true, false, false, false} !=
          PresentModeOptions{false, false, true, false});
}

TEST_CASE("Host cap survives irregular redraws and mode switches", "[display-settings]") {
  PresentationRateLimit limiter;
  uint64_t now = 1'000'000;
  for (uint32_t fps : {30u, 40u, 60u, 120u, 0u, 120u, 40u, 30u, 60u}) {
    const uint64_t begin = now;
    uint32_t presented = 0;
    const uint64_t end = begin + 2'000'000'000;
    for (; now < end; now += 700'000) {
      if (limiter.Delay(fps, now)) continue;
      limiter.Presented(now);
      ++presented;
    }
    if (fps) REQUIRE(presented == fps * 2);
    else REQUIRE(presented > 1000);
  }
}
