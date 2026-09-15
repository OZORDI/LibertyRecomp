#pragma once

#include <array>
#include <chrono>
#include <cstdint>
#include <utility>

#include <rex/input/absolute_pointer.h>
#include <rex/ui/window.h>

#include "input/context_touch_controls.h"

namespace gta4::input {

// Physical mouse events reach this listener after host dialogs and before the
// game's input driver. SDL's touch-generated mouse events are excluded by
// WindowSDL, so this never duplicates a finger's absolute-pointer transaction.
class ContextTouchEditorMouse final : public rex::ui::WindowInputListener,
                                      public rex::ui::WindowListener {
 public:
  explicit ContextTouchEditorMouse(rex::ui::Window* window) : window_(window) {
    if (window_) {
      window_->AddInputListener(this, 63);
      window_->AddListener(this);
    }
  }
  ~ContextTouchEditorMouse() override { Detach(); }

  const char* input_trace_name() const override { return "touch-layout-editor"; }

  void OnMouseDown(rex::ui::MouseEvent& event) override {
    const auto button = static_cast<size_t>(event.button());
    if (!button || button >= swallowed_.size()) return;
    if (!ContextTouchEditorCapturesInput()) {
      forwarded_[button] = true;
      return;
    }
    if (forwarded_[button]) return;
    swallowed_[button] = true;
    event.set_handled(true);
    if (event.button() == rex::ui::MouseEvent::Button::kLeft &&
        IsContextTouchEditorActive() && !pointer_down_) {
      pointer_down_ = true;
      Submit(rex::input::AbsolutePointerPhase::kDown, event);
    }
  }

  void OnMouseMove(rex::ui::MouseEvent& event) override {
    const bool owned = pointer_down_;
    if (pointer_down_) {
      if (IsContextTouchEditorActive()) Submit(rex::input::AbsolutePointerPhase::kMove, event);
      else CancelPointer();
    }
    if (owned || ContextTouchEditorCapturesInput()) event.set_handled(true);
  }

  void OnMouseUp(rex::ui::MouseEvent& event) override {
    const auto button = static_cast<size_t>(event.button());
    if (!button || button >= swallowed_.size()) return;
    // A press which opened the editor already reached the physical driver.
    // Let its release reach that same driver, avoiding a stuck mouse button.
    if (std::exchange(forwarded_[button], false)) return;
    const bool swallowed = std::exchange(swallowed_[button], false);
    if (event.button() == rex::ui::MouseEvent::Button::kLeft && pointer_down_) {
      Submit(IsContextTouchEditorActive() ? rex::input::AbsolutePointerPhase::kUp :
                                           rex::input::AbsolutePointerPhase::kCancel, event);
      pointer_down_ = false;
    }
    if (swallowed || ContextTouchEditorCapturesInput()) event.set_handled(true);
  }

  void OnMouseWheel(rex::ui::MouseEvent& event) override {
    if (ContextTouchEditorCapturesInput()) event.set_handled(true);
  }
  void OnLostFocus(rex::ui::UISetupEvent&) override { Reset(); }
  void OnMinimized(rex::ui::UIEvent&) override { Reset(); }
  void OnResize(rex::ui::UISetupEvent&) override { CancelPointer(); }
  void OnDpiChanged(rex::ui::UISetupEvent&) override { CancelPointer(); }
  void OnClosing(rex::ui::UIEvent&) override { Detach(); }

 private:
  static uint64_t Timestamp() {
    return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count());
  }
  void Submit(rex::input::AbsolutePointerPhase phase, const rex::ui::MouseEvent& event) {
    x_ = static_cast<float>(event.x());
    y_ = static_cast<float>(event.y());
    rex::input::GetAbsolutePointerService().SubmitPointer(
        UINT64_MAX, UINT64_MAX, phase, x_, y_, 1.0f, Timestamp());
  }
  void CancelPointer() {
    if (!std::exchange(pointer_down_, false)) return;
    rex::input::GetAbsolutePointerService().SubmitPointer(
        UINT64_MAX, UINT64_MAX, rex::input::AbsolutePointerPhase::kCancel,
        x_, y_, 0.0f, Timestamp());
  }
  void Reset() {
    CancelPointer();
    forwarded_.fill(false);
    swallowed_.fill(false);
  }
  void Detach() {
    Reset();
    if (!window_) return;
    auto* window = std::exchange(window_, nullptr);
    window->RemoveInputListener(this);
    window->RemoveListener(this);
  }

  rex::ui::Window* window_ = nullptr;
  std::array<bool, 6> forwarded_{};
  std::array<bool, 6> swallowed_{};
  float x_ = 0.0f;
  float y_ = 0.0f;
  bool pointer_down_ = false;
};

}  // namespace gta4::input
