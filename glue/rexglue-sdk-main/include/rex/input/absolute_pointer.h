#pragma once

#include <cstddef>
#include <cstdint>
#include <deque>
#include <functional>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <rex/ui/guest_output_transform.h>

namespace rex::input {

struct X_INPUT_GAMEPAD;
using TouchGamepadProvider = bool (*)(uint32_t user_index, X_INPUT_GAMEPAD* state) noexcept;
void SetTouchGamepadProvider(TouchGamepadProvider provider) noexcept;
bool ReadTouchGamepad(uint32_t user_index, X_INPUT_GAMEPAD* state) noexcept;
void SetTouchInputActiveCallback(std::function<bool()> callback);

enum class AbsolutePointerPhase : uint8_t {
  kDown,
  kMove,
  kUp,
  kCancel,
};

// Single-consumer guest-thread event. Coordinates are intentionally unclamped
// guest-frontbuffer pixels so touches in letterbox or overscan regions remain
// distinguishable from touches on guest content.
struct AbsolutePointerEvent {
  uint64_t sequence = 0;
  uint64_t generation = 0;
  uint64_t pointer_id = 0;
  uint64_t timestamp_ns = 0;
  float x = 0.0f;
  float y = 0.0f;
  // Window logical coordinates for host controls, independent of letterboxing.
  float logical_x = 0.0f;
  float logical_y = 0.0f;
  float output_width = 0.0f;
  float output_height = 0.0f;
  float pressure = 0.0f;
  AbsolutePointerPhase phase = AbsolutePointerPhase::kCancel;
};

struct TouchPresentationState {
  uint64_t generation = 0;
  float output_width = 0.0f;
  float output_height = 0.0f;
  // Guest output rectangle in physical surface pixels. Overlay drawing maps
  // guest pixels through this rectangle instead of assuming it fills the
  // window (letterbox and render-target scaling are preserved).
  float physical_output_x = 0.0f;
  float physical_output_y = 0.0f;
  float physical_output_width = 0.0f;
  float physical_output_height = 0.0f;
  float physical_surface_width = 0.0f;
  float physical_surface_height = 0.0f;
  float logical_width = 0.0f;
  float logical_height = 0.0f;
  float logical_safe_x = 0.0f;
  float logical_safe_y = 0.0f;
  float logical_safe_width = 0.0f;
  float logical_safe_height = 0.0f;
  int32_t safe_area_x = 0;
  int32_t safe_area_y = 0;
  int32_t safe_area_width = 0;
  int32_t safe_area_height = 0;
  bool valid = false;
  bool focused = false;
};

enum class TouchControlsMode : uint8_t {
  kAuto,
  kOn,
  kOff,
};

bool ShouldEnableTouchControls(TouchControlsMode mode, bool controller_connected,
                               bool physical_keyboard_connected, bool focused,
                               bool physical_mouse_connected = false) noexcept;

class AbsolutePointerService {
 public:
  static constexpr size_t kDefaultMaxQueuedMoves = 128;

  explicit AbsolutePointerService(size_t max_queued_moves = kDefaultMaxQueuedMoves);

  void UpdatePresentation(const rex::ui::GuestOutputTransform& transform, int32_t safe_area_x,
                          int32_t safe_area_y, int32_t safe_area_width, int32_t safe_area_height,
                          uint64_t timestamp_ns);
  void SetFocused(bool focused, uint64_t timestamp_ns);
  void SetLogicalSize(float width, float height, uint64_t timestamp_ns = 0);
  void NotifyPhysicalInput(uint64_t timestamp_ns = 0);

  void SubmitPointer(uint64_t source_device_id, uint64_t source_pointer_id,
                     AbsolutePointerPhase phase, float physical_x, float physical_y, float pressure,
                     uint64_t timestamp_ns);
  void CancelAll(uint64_t timestamp_ns);

  bool TryDequeue(AbsolutePointerEvent* out_event) noexcept;
  bool GetPresentationState(TouchPresentationState* out_state) const noexcept;

  void ReplacePhysicalKeyboards(const std::vector<uint64_t>& device_ids, uint64_t timestamp_ns = 0);
  void AddPhysicalKeyboard(uint64_t device_id, uint64_t timestamp_ns = 0);
  void RemovePhysicalKeyboard(uint64_t device_id, uint64_t timestamp_ns = 0);
  void ReplacePhysicalMice(const std::vector<uint64_t>& device_ids, uint64_t timestamp_ns = 0);
  void AddPhysicalMouse(uint64_t device_id, uint64_t timestamp_ns = 0);
  void RemovePhysicalMouse(uint64_t device_id, uint64_t timestamp_ns = 0);
  void ReplaceGameControllers(const std::vector<uint64_t>& device_ids, uint64_t timestamp_ns = 0);
  void AddGameController(uint64_t device_id, uint64_t timestamp_ns = 0);
  void RemoveGameController(uint64_t device_id, uint64_t timestamp_ns = 0);
  void SetAndroidPhysicalKeyboardPresence(bool present, bool query_succeeded,
                                          uint64_t timestamp_ns = 0);
  void SetAndroidPhysicalMousePresence(bool present, bool query_succeeded,
                                       uint64_t timestamp_ns = 0);

  bool HasPhysicalKeyboard() const noexcept;
  bool HasPhysicalMouse() const noexcept;
  bool HasGameController() const noexcept;
  bool TouchControlsActive(TouchControlsMode mode) noexcept;
  bool TouchControlsVisible(TouchControlsMode mode) const noexcept;
  size_t queued_move_count() const noexcept;

 private:
  struct SourcePointerKey {
    uint64_t device_id;
    uint64_t pointer_id;

    bool operator==(const SourcePointerKey& other) const {
      return device_id == other.device_id && pointer_id == other.pointer_id;
    }
  };

  struct SourcePointerKeyHash {
    size_t operator()(const SourcePointerKey& key) const noexcept;
  };

  struct ActivePointer {
    uint64_t pointer_id;
    float x;
    float y;
    float pressure;
    float logical_x = 0.0f;
    float logical_y = 0.0f;
  };

  bool PresentationGeometryEqualsLocked(const rex::ui::GuestOutputTransform& transform,
                                        int32_t safe_area_x, int32_t safe_area_y,
                                        int32_t safe_area_width, int32_t safe_area_height) const;
  void ResetLocked(uint64_t timestamp_ns);
  void QueueEventLocked(const AbsolutePointerEvent& event);
  AbsolutePointerEvent MakeEventLocked(AbsolutePointerPhase phase, const ActivePointer& pointer,
                                       uint64_t timestamp_ns);
  void MapPhysicalToGuestLocked(float physical_x, float physical_y, float& guest_x,
                                float& guest_y) const;
  TouchPresentationState BuildPresentationStateLocked() const;

  const size_t max_queued_moves_;
  mutable std::mutex mutex_;
  rex::ui::GuestOutputTransform transform_;
  int32_t safe_area_x_ = 0;
  int32_t safe_area_y_ = 0;
  int32_t safe_area_width_ = 0;
  int32_t safe_area_height_ = 0;
  bool focused_ = false;
  float logical_width_ = 0.0f;
  float logical_height_ = 0.0f;
  bool touch_seen_ = false;
  bool physical_input_latest_ = false;
  uint64_t generation_ = 1;
  uint64_t next_sequence_ = 1;
  uint64_t next_pointer_id_ = 1;
  size_t queued_move_count_ = 0;
  std::deque<AbsolutePointerEvent> events_;
  std::unordered_map<SourcePointerKey, ActivePointer, SourcePointerKeyHash> active_pointers_;
  std::unordered_set<uint64_t> physical_keyboards_;
  std::unordered_set<uint64_t> physical_mice_;
  std::unordered_set<uint64_t> game_controllers_;
  bool android_keyboard_presence_valid_ = false;
  bool android_physical_keyboard_present_ = false;
  bool android_mouse_presence_valid_ = false;
  bool android_physical_mouse_present_ = false;
  bool policy_log_initialized_ = false;
  TouchControlsMode logged_policy_mode_ = TouchControlsMode::kAuto;
  size_t logged_keyboard_count_ = 0;
  size_t logged_mouse_count_ = 0;
  size_t logged_controller_count_ = 0;
  bool logged_focus_ = false;
  bool logged_touch_active_ = false;
};

// Guest-facing, process-wide single-consumer API.
bool TryDequeueAbsolutePointerEvent(AbsolutePointerEvent* out_event) noexcept;
// Device availability is stable while host UI temporarily captures game input.
bool TouchControlsAvailable() noexcept;
bool TouchControlsActive() noexcept;
bool TouchControlsVisible() noexcept;
// Native menu/map touch remains available when the gameplay overlay is Off.
bool TouchPointerInputActive() noexcept;
bool GetTouchPresentationState(TouchPresentationState* out_state) noexcept;

// Host ingress used by the SDL window/input bridge.
AbsolutePointerService& GetAbsolutePointerService() noexcept;
void RefreshAndroidPhysicalKeyboardPresence(uint64_t timestamp_ns = 0) noexcept;

}  // namespace rex::input
