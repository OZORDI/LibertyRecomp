#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

namespace gta4::presentation::policy {

// Generated sub_82145968 parser / sub_82144188 player contract. No installed
// loading-screen file is edited. The final marker is loading art, not a menu.
inline constexpr uint32_t kColdStartCaller = 0x82140048;
inline constexpr uint32_t kParserCaller = 0x82145558;
inline constexpr uint32_t kCompositeCaller = 0x822D0C48;
inline constexpr uint32_t kScreenStride = 400;
inline constexpr uint32_t kMaxScreens = 14;
inline constexpr uint32_t kMaxLayers = 4;
inline constexpr uint32_t kDurationOffset = 0;
inline constexpr uint32_t kLayerCountOffset = 4;
inline constexpr uint32_t kMarkerOffset = 8;
inline constexpr uint32_t kFadeOffset = 12;

enum class Marker : uint32_t {
  kNone = 0,
  kLegal = 1,
  kStartIntro = 2,
  kEndIntro = 3,
  kInitialMain = 4,
  kWaitForAudio = 5,
};

constexpr bool IsColdStart(uint32_t caller, uint32_t intro, uint32_t episodic) noexcept {
  return caller == kColdStartCaller && intro == 1 && episodic == 0;
}

constexpr uint32_t LoadBe32(std::span<const uint8_t> data, std::size_t offset) noexcept {
  return (uint32_t{data[offset]} << 24) | (uint32_t{data[offset + 1]} << 16) |
         (uint32_t{data[offset + 2]} << 8) | uint32_t{data[offset + 3]};
}

constexpr void StoreBe32(std::span<uint8_t> data, std::size_t offset, uint32_t value) noexcept {
  data[offset] = static_cast<uint8_t>(value >> 24);
  data[offset + 1] = static_cast<uint8_t>(value >> 16);
  data[offset + 2] = static_cast<uint8_t>(value >> 8);
  data[offset + 3] = static_cast<uint8_t>(value);
}

enum class IntroStatus { kDisabled, kUnsupported, kReady };
struct IntroPlan {
  IntroStatus status = IntroStatus::kUnsupported;
  uint32_t prefix_count = 0;
  uint32_t end_intro_index = 0;
};

// Validate the complete table before returning any writable range. An unknown
// marker, malformed prefix or truncated span leaves all bytes untouched.
constexpr IntroPlan PlanIntro(std::span<const uint8_t> data, uint32_t count,
                              bool enabled) noexcept {
  if (!enabled)
    return {IntroStatus::kDisabled};
  if (!count || count > kMaxScreens || data.size() != std::size_t{count} * kScreenStride)
    return {};
  uint32_t main_index = count;
  uint32_t start_index = count;
  uint32_t end_index = count;
  uint32_t legal_count = 0;
  for (uint32_t i = 0; i < count; ++i) {
    const std::size_t offset = std::size_t{i} * kScreenStride;
    const auto marker = LoadBe32(data, offset + kMarkerOffset);
    if (LoadBe32(data, offset + kDurationOffset) > 0x7FFFFFFFu ||
        LoadBe32(data, offset + kLayerCountOffset) > kMaxLayers ||
        LoadBe32(data, offset + kFadeOffset) > 3 ||
        marker > static_cast<uint32_t>(Marker::kWaitForAudio))
      return {};
    if (marker == static_cast<uint32_t>(Marker::kInitialMain) && main_index == count)
      main_index = i;
    if (main_index != count)
      continue;
    if (marker == static_cast<uint32_t>(Marker::kLegal))
      ++legal_count;
    if (marker == static_cast<uint32_t>(Marker::kStartIntro)) {
      if (start_index != count)
        return {};
      start_index = i;
    }
    if (marker == static_cast<uint32_t>(Marker::kEndIntro)) {
      if (end_index != count)
        return {};
      end_index = i;
    }
  }
  if (LoadBe32(data, kMarkerOffset) != static_cast<uint32_t>(Marker::kLegal) || legal_count != 1 ||
      start_index == 0 || start_index >= end_index || end_index >= main_index ||
      main_index >= count)
    return {};
  return {IntroStatus::kReady, main_index, end_index};
}

constexpr IntroPlan CollapseIntro(std::span<uint8_t> data, uint32_t count, bool enabled) noexcept {
  const IntroPlan plan = PlanIntro(data, count, enabled);
  if (plan.status != IntroStatus::kReady)
    return plan;
  for (uint32_t i = 0; i < plan.prefix_count; ++i) {
    const std::size_t offset = std::size_t{i} * kScreenStride;
    StoreBe32(data, offset + kDurationOffset, 0);
    StoreBe32(data, offset + kLayerCountOffset, 0);
    StoreBe32(data, offset + kFadeOffset, 0);
  }
  // In particular, leave END_INTRO and WAIT_FOR_AUDIO to the original player.
  return plan;
}

constexpr bool IsNoisePass(uint32_t pass) noexcept {
  return pass == 24 || pass == 25 || pass == 26 || pass == 27;
}

// Verified GTACompositePostFx passes in rage_postfx_e1.fxc. Selecting a pass,
// rather than swapping a PS after binding, also updates the sampler layout.
constexpr uint32_t SelectCompositePass(uint32_t requested, bool disabled, uint32_t episode,
                                       uint32_t caller, bool valid_effect) noexcept {
  if (!disabled || episode != 1 || caller != kCompositeCaller || !valid_effect)
    return requested;
  switch (requested) {
    case 24:
      return 10;
    case 25:
      return 11;
    case 26:
      return 12;
    case 27:
      return 13;
    default:
      return requested;
  }
}

}  // namespace gta4::presentation::policy
