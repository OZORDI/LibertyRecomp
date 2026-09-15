#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

#include <rex/input/mnk/encoded_action.h>
#include <rex/input/sony_feedback.h>

namespace gta4::sony {

// Verified retail action indices. Radio consumes the release edge; the other
// discrete gestures consume a press edge. Cinematic is an independent hold.
constexpr std::array<uint32_t, 6> kActions = {0, 8, 9, 51, 52, 53};
constexpr std::size_t kCinematic = 3;

struct ActionEpoch {
  std::array<bool, kActions.size()> current{};
  std::array<bool, kActions.size()> previous{};
};

class PulseQueue {
 public:
  ActionEpoch Advance(uint64_t generation, uint32_t player, uint32_t context,
                      bool owned, bool in_vehicle, bool driver,
                      std::span<const rex::input::sony::Gesture> gestures, bool cinematic) {
    const bool changed = generation_ != generation || player_ != player || context_ != context ||
                         in_vehicle_ != in_vehicle || driver_ != driver;
    generation_ = generation;
    player_ = player;
    context_ = context;
    in_vehicle_ = in_vehicle;
    driver_ = driver;
    if (!owned || changed) {
      pending_ = {};
      epoch_ = {};
      return epoch_;
    }
    for (const auto gesture : gestures) {
      std::size_t index = 0;
      switch (gesture) {
        case rex::input::sony::Gesture::kCamera: index = 0; break;
        case rex::input::sony::Gesture::kNextWeapon:
          if (in_vehicle) continue;
          index = 1;
          break;
        case rex::input::sony::Gesture::kPreviousWeapon:
          if (in_vehicle) continue;
          index = 2;
          break;
        case rex::input::sony::Gesture::kNextRadio:
          if (!in_vehicle || !driver) continue;
          index = 4;
          break;
        case rex::input::sony::Gesture::kPreviousRadio:
          if (!in_vehicle || !driver) continue;
          index = 5;
          break;
      }
      // Bound a burst while keeping a release epoch between repeated swipes.
      if (pending_[index] < 8) ++pending_[index];
    }
    epoch_.previous = epoch_.current;
    for (std::size_t index = 0; index < kActions.size(); ++index) {
      epoch_.current[index] = false;
      if (!epoch_.previous[index] && pending_[index]) {
        --pending_[index];
        epoch_.current[index] = true;
      }
    }
    epoch_.current[kCinematic] = in_vehicle && cinematic;
    return epoch_;
  }

 private:
  uint64_t generation_ = 0;
  uint32_t player_ = 0;
  uint32_t context_ = 0;
  bool in_vehicle_ = false;
  bool driver_ = false;
  std::array<uint8_t, kActions.size()> pending_{};
  ActionEpoch epoch_{};
};

struct ActionBytes {
  uint8_t current = 0;
  uint8_t previous = 0;
  bool operator==(const ActionBytes&) const = default;
};

// Retail replay may advance our last current byte into previous, and may
// replay the same control more than once in one poll. Recover the physical/
// keyboard baseline before merging both halves of the synthetic history.
// This also removes an owned release when a menu/context transition cancels
// a radio swipe. New physical input remains authoritative.
class ActionHistory {
 public:
  ActionBytes Merge(uint64_t sequence, uint8_t polarity, ActionBytes observed,
                    bool current, bool previous) {
    const bool same_epoch = initialized_ && sequence_ == sequence;
    ActionBytes baseline = observed;
    if (initialized_) {
      if (same_epoch && observed.current == written_.current && owned_current_) {
        baseline.current = baseline_.current;
      }
      if (owned_current_ && observed.previous == written_.current) {
        baseline.previous = same_epoch ? baseline_.previous : baseline_.current;
      } else if (same_epoch && observed.previous == written_.previous && owned_previous_) {
        baseline.previous = baseline_.previous;
      }
    }
    ActionBytes result{
        rex::input::mnk::MergeActionMagnitude(polarity, baseline.current, current ? 255 : 0),
        rex::input::mnk::MergeActionMagnitude(polarity, baseline.previous, previous ? 255 : 0)};
    initialized_ = true;
    sequence_ = sequence;
    baseline_ = baseline;
    written_ = result;
    owned_current_ = result.current != baseline.current;
    owned_previous_ = result.previous != baseline.previous;
    return result;
  }

 private:
  uint64_t sequence_ = 0;
  ActionBytes baseline_{};
  ActionBytes written_{};
  bool initialized_ = false;
  bool owned_current_ = false;
  bool owned_previous_ = false;
};

// Mirrors IS_PLAYER_CONTROL_ON's mode-specific mask and the retail playing
// predicate. The policy is independent of guest pointers for regression tests.
constexpr bool GameplayOwned(bool playing, uint32_t control_flags, bool network_mode,
                              uint32_t ped_control_mode, bool frontend, bool phone,
                              bool minigame, bool title_owned) {
  const uint32_t disabled = network_mode ? control_flags & 0xFFFFFBFFu : control_flags;
  return playing && !disabled && ped_control_mode != 1 && !frontend && !phone &&
         !minigame && !title_owned;
}

}  // namespace gta4::sony
