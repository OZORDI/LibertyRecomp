#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <span>

namespace gta4::touch {

struct Point {
  float x = 0.0f;
  float y = 0.0f;
};

struct Rect {
  float left = 0.0f;
  float top = 0.0f;
  float right = 0.0f;
  float bottom = 0.0f;

  [[nodiscard]] bool valid() const noexcept {
    return std::isfinite(left) && std::isfinite(top) && std::isfinite(right) &&
           std::isfinite(bottom) && right > left && bottom > top;
  }

  [[nodiscard]] bool contains(Point point) const noexcept {
    return valid() && std::isfinite(point.x) && std::isfinite(point.y) &&
           point.x >= left && point.x <= right && point.y >= top && point.y <= bottom;
  }
};

struct FrontendRow {
  Rect bounds{};
  uint32_t channel = 0;
  uint32_t row = 0;
  bool selectable = false;
};

[[nodiscard]] inline std::optional<FrontendRow> HitTestFrontendRow(
    std::span<const FrontendRow> rows, Point point) noexcept {
  for (const FrontendRow& row : rows) {
    if (row.selectable && row.bounds.contains(point)) {
      return row;
    }
  }
  return std::nullopt;
}

[[nodiscard]] inline bool NearlyEqual(float left, float right,
                                      float epsilon = 0x1.0p-8f) noexcept {
  return std::isfinite(left) && std::isfinite(right) &&
         std::abs(left - right) <= epsilon;
}

[[nodiscard]] inline bool SameFrontendRows(std::span<const FrontendRow> left,
                                           std::span<const FrontendRow> right) noexcept {
  if (left.size() != right.size()) {
    return false;
  }
  for (size_t index = 0; index < left.size(); ++index) {
    const FrontendRow& a = left[index];
    const FrontendRow& b = right[index];
    if (a.channel != b.channel || a.row != b.row || a.selectable != b.selectable ||
        !NearlyEqual(a.bounds.left, b.bounds.left) ||
        !NearlyEqual(a.bounds.top, b.bounds.top) ||
        !NearlyEqual(a.bounds.right, b.bounds.right) ||
        !NearlyEqual(a.bounds.bottom, b.bounds.bottom)) {
      return false;
    }
  }
  return true;
}

class FrontendTransaction {
 public:
  struct MoveResult {
    bool consumed = false;
    bool selection_changed = false;
    uint32_t channel = 0;
    uint32_t row = 0;
  };

  [[nodiscard]] bool active() const noexcept { return active_; }
  [[nodiscard]] uint64_t pointer_id() const noexcept { return pointer_id_; }

  bool Begin(uint64_t pointer_id, uint64_t pointer_generation, uint64_t layout_generation,
             const FrontendRow& row) noexcept {
    if (active_ || !row.selectable || !row.bounds.valid()) {
      return false;
    }
    active_ = true;
    armed_ = true;
    pointer_id_ = pointer_id;
    pointer_generation_ = pointer_generation;
    layout_generation_ = layout_generation;
    channel_ = row.channel;
    row_ = row.row;
    return true;
  }

  MoveResult Move(uint64_t pointer_id, uint64_t pointer_generation,
                  uint64_t layout_generation,
                  const std::optional<FrontendRow>& hit) noexcept {
    if (!Matches(pointer_id, pointer_generation, layout_generation)) {
      if (active_ && pointer_id == pointer_id_) {
        Reset();
      }
      return {};
    }
    MoveResult result{.consumed = true};
    if (!hit || !hit->selectable) {
      armed_ = false;
      return result;
    }
    armed_ = true;
    if (hit->channel != channel_ || hit->row != row_) {
      channel_ = hit->channel;
      row_ = hit->row;
      result.selection_changed = true;
      result.channel = channel_;
      result.row = row_;
    }
    return result;
  }

  bool End(uint64_t pointer_id, uint64_t pointer_generation, uint64_t layout_generation,
           const std::optional<FrontendRow>& hit) noexcept {
    if (!Matches(pointer_id, pointer_generation, layout_generation)) {
      if (active_ && pointer_id == pointer_id_) {
        Reset();
      }
      return false;
    }
    const bool activate = armed_ && hit && hit->selectable && hit->channel == channel_ &&
                          hit->row == row_;
    Reset();
    return activate;
  }

  bool Cancel(uint64_t pointer_id, uint64_t pointer_generation) noexcept {
    if (!active_ || pointer_id != pointer_id_) {
      return false;
    }
    const bool matched = pointer_generation == pointer_generation_;
    Reset();
    return matched;
  }

  void Reset() noexcept {
    active_ = false;
    armed_ = false;
    pointer_id_ = 0;
    pointer_generation_ = 0;
    layout_generation_ = 0;
    channel_ = 0;
    row_ = 0;
  }

 private:
  [[nodiscard]] bool Matches(uint64_t pointer_id, uint64_t pointer_generation,
                             uint64_t layout_generation) const noexcept {
    return active_ && pointer_id == pointer_id_ && pointer_generation == pointer_generation_ &&
           layout_generation == layout_generation_;
  }

  uint64_t pointer_id_ = 0;
  uint64_t pointer_generation_ = 0;
  uint64_t layout_generation_ = 0;
  uint32_t channel_ = 0;
  uint32_t row_ = 0;
  bool active_ = false;
  bool armed_ = false;
};

class TapTransaction {
 public:
  bool Begin(uint64_t pointer_id, uint64_t pointer_generation, uint64_t geometry_generation,
             Point point, float output_width, float output_height, Rect bounds) noexcept {
    if (active_ || !ValidOutput(output_width, output_height) || !bounds.contains(point)) {
      return false;
    }
    active_ = true;
    armed_ = true;
    pointer_id_ = pointer_id;
    pointer_generation_ = pointer_generation;
    geometry_generation_ = geometry_generation;
    start_ = point;
    bounds_ = bounds;
    slop_ = std::min(output_width, output_height) * 0x1.47ae14p-6f;
    return true;
  }

  [[nodiscard]] bool active() const noexcept { return active_; }
  [[nodiscard]] uint64_t pointer_id() const noexcept { return pointer_id_; }

  bool Move(uint64_t pointer_id, uint64_t pointer_generation, uint64_t geometry_generation,
            Point point) noexcept {
    if (!Matches(pointer_id, pointer_generation, geometry_generation)) {
      if (active_ && pointer_id == pointer_id_) {
        Reset();
      }
      return false;
    }
    const float delta_x = point.x - start_.x;
    const float delta_y = point.y - start_.y;
    if (!bounds_.contains(point) || delta_x * delta_x + delta_y * delta_y > slop_ * slop_) {
      armed_ = false;
    }
    return true;
  }

  bool End(uint64_t pointer_id, uint64_t pointer_generation, uint64_t geometry_generation,
           Point point) noexcept {
    // Hosts may coalesce movement into the release event. Apply the same
    // travel limit here before deciding whether this gesture was a tap.
    if (!Move(pointer_id, pointer_generation, geometry_generation, point)) {
      return false;
    }
    const bool tapped = armed_ && bounds_.contains(point);
    Reset();
    return tapped;
  }

  void Reset() noexcept {
    active_ = false;
    armed_ = false;
    pointer_id_ = 0;
    pointer_generation_ = 0;
    geometry_generation_ = 0;
    start_ = {};
    bounds_ = {};
    slop_ = 0.0f;
  }

 private:
  static bool ValidOutput(float width, float height) noexcept {
    return std::isfinite(width) && std::isfinite(height) && width > 0.0f && height > 0.0f;
  }

  [[nodiscard]] bool Matches(uint64_t pointer_id, uint64_t pointer_generation,
                             uint64_t geometry_generation) const noexcept {
    return active_ && pointer_id == pointer_id_ && pointer_generation == pointer_generation_ &&
           geometry_generation == geometry_generation_;
  }

  Point start_{};
  Rect bounds_{};
  float slop_ = 0.0f;
  uint64_t pointer_id_ = 0;
  uint64_t pointer_generation_ = 0;
  uint64_t geometry_generation_ = 0;
  bool active_ = false;
  bool armed_ = false;
};

struct MapGestureOutput {
  float pan_x = 0.0f;
  float pan_y = 0.0f;
  int32_t zoom_steps = 0;
  bool waypoint = false;
  bool consumed = false;
  bool cancelled = false;
};

class MapGesture {
 public:
  MapGestureOutput Down(uint64_t pointer_id, uint64_t generation, Point point,
                        float output_width, float output_height) noexcept {
    MapGestureOutput output;
    if (!ValidPoint(point) || !ValidOutput(output_width, output_height)) {
      return output;
    }
    if (contact_count_ && generation != generation_) {
      Reset();
      output.cancelled = true;
    }
    if (!contact_count_) {
      generation_ = generation;
      tap_slop_ = std::min(output_width, output_height) * 0x1.47ae14p-6f;
      pinch_step_ = std::min(output_width, output_height) * 0x1.47ae14p-4f;
    }
    if (Find(pointer_id) || contact_count_ == contacts_.size()) {
      return output;
    }
    Contact* contact = FirstFree();
    if (!contact) {
      return output;
    }
    *contact = {.point = point, .start = point, .pointer_id = pointer_id, .active = true};
    ++contact_count_;
    output.consumed = true;
    if (contact_count_ == contacts_.size()) {
      ever_pinched_ = true;
      suppress_single_contact_ = true;
      pinch_reference_ = ContactDistance();
    }
    return output;
  }

  MapGestureOutput Move(uint64_t pointer_id, uint64_t generation, Point point) noexcept {
    MapGestureOutput output;
    Contact* contact = Find(pointer_id);
    if (!contact) {
      return output;
    }
    if (generation != generation_ || !ValidPoint(point)) {
      Reset();
      output.cancelled = true;
      return output;
    }
    const Point previous = contact->point;
    contact->point = point;
    output.consumed = true;
    UpdateTravel(*contact);
    if (contact_count_ == contacts_.size()) {
      const float distance = ContactDistance();
      if (std::isfinite(distance) && pinch_step_ > 0.0f) {
        const float delta = distance - pinch_reference_;
        if (std::abs(delta) >= pinch_step_) {
          // Emit at most one retail-equivalent zoom notch for each pointer
          // event. Besides feeling predictable, this avoids converting an
          // unbounded host coordinate into an overflowing integer step count.
          output.zoom_steps = delta > 0.0f ? 1 : -1;
          pinch_reference_ = distance;
        }
      }
      return output;
    }
    if (!suppress_single_contact_) {
      output.pan_x = point.x - previous.x;
      output.pan_y = point.y - previous.y;
    }
    return output;
  }

  MapGestureOutput Up(uint64_t pointer_id, uint64_t generation, Point point) noexcept {
    MapGestureOutput output;
    Contact* contact = Find(pointer_id);
    if (!contact) {
      return output;
    }
    if (generation != generation_ || !ValidPoint(point)) {
      Reset();
      output.cancelled = true;
      return output;
    }
    contact->point = point;
    UpdateTravel(*contact);
    output.consumed = true;
    const bool waypoint = contact_count_ == 1 && !ever_pinched_ && !contact->moved;
    contact->active = false;
    --contact_count_;
    output.waypoint = waypoint;
    if (!contact_count_) {
      Reset();
    } else {
      suppress_single_contact_ = true;
    }
    return output;
  }

  MapGestureOutput Cancel(uint64_t pointer_id, uint64_t generation) noexcept {
    MapGestureOutput output;
    Contact* contact = Find(pointer_id);
    if (!contact) {
      return output;
    }
    output.consumed = true;
    if (generation != generation_) {
      Reset();
      output.cancelled = true;
      return output;
    }
    contact->active = false;
    --contact_count_;
    suppress_single_contact_ = true;
    if (!contact_count_) {
      Reset();
    }
    return output;
  }

  [[nodiscard]] bool active() const noexcept { return contact_count_ != 0; }

  void Reset() noexcept {
    contacts_ = {};
    contact_count_ = 0;
    generation_ = 0;
    tap_slop_ = 0.0f;
    pinch_step_ = 0.0f;
    pinch_reference_ = 0.0f;
    ever_pinched_ = false;
    suppress_single_contact_ = false;
  }

 private:
  struct Contact {
    Point point{};
    Point start{};
    uint64_t pointer_id = 0;
    bool active = false;
    bool moved = false;
  };

  static bool ValidPoint(Point point) noexcept {
    return std::isfinite(point.x) && std::isfinite(point.y);
  }

  static bool ValidOutput(float width, float height) noexcept {
    return std::isfinite(width) && std::isfinite(height) && width > 0.0f && height > 0.0f;
  }

  Contact* Find(uint64_t pointer_id) noexcept {
    for (Contact& contact : contacts_) {
      if (contact.active && contact.pointer_id == pointer_id) {
        return &contact;
      }
    }
    return nullptr;
  }

  Contact* FirstFree() noexcept {
    for (Contact& contact : contacts_) {
      if (!contact.active) {
        return &contact;
      }
    }
    return nullptr;
  }

  void UpdateTravel(Contact& contact) noexcept {
    const float delta_x = contact.point.x - contact.start.x;
    const float delta_y = contact.point.y - contact.start.y;
    if (delta_x * delta_x + delta_y * delta_y > tap_slop_ * tap_slop_) {
      contact.moved = true;
    }
  }

  [[nodiscard]] float ContactDistance() const noexcept {
    const float delta_x = contacts_[0].point.x - contacts_[1].point.x;
    const float delta_y = contacts_[0].point.y - contacts_[1].point.y;
    return std::sqrt(delta_x * delta_x + delta_y * delta_y);
  }

  std::array<Contact, 2> contacts_{};
  size_t contact_count_ = 0;
  uint64_t generation_ = 0;
  float tap_slop_ = 0.0f;
  float pinch_step_ = 0.0f;
  float pinch_reference_ = 0.0f;
  bool ever_pinched_ = false;
  bool suppress_single_contact_ = false;
};

[[nodiscard]] inline int32_t ScaleMapPan(float delta, float output_extent) noexcept {
  if (!std::isfinite(delta) || !std::isfinite(output_extent) || output_extent <= 0.0f) {
    return 0;
  }
  const float scaled = std::round(delta * 1020.0f / output_extent);
  if (!std::isfinite(scaled)) {
    return 0;
  }
  const float bounded = std::clamp(scaled, -255.0f, 255.0f);
  return static_cast<int32_t>(bounded);
}

}  // namespace gta4::touch
