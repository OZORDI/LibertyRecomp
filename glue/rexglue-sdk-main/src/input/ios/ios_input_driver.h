#pragma once
/**
 ******************************************************************************
 * ReXGlue — iOS GameController input driver
 ******************************************************************************
 */

#include <rex/platform.h>

#if REX_PLATFORM_IOS

#include <array>
#include <atomic>
#include <mutex>

#include <rex/input/input_driver.h>

namespace rex::input::ios {

static constexpr size_t HID_IOS_USER_COUNT = 4;

// Opaque pointer that holds the Objective-C bridge (GCController*, haptics,
// last-seen analog/button snapshot). Implemented in ios_input_driver.mm.
struct IOSController;

class IOSInputDriver final : public InputDriver {
 public:
  IOSInputDriver(rex::ui::Window* window, size_t window_z_order);
  ~IOSInputDriver() override;

  X_STATUS Setup() override;

  void OnWindowAvailable(rex::ui::Window* window) override;

  X_RESULT GetCapabilities(uint32_t user_index, uint32_t flags,
                           X_INPUT_CAPABILITIES* out_caps) override;
  X_RESULT GetState(uint32_t user_index, X_INPUT_STATE* out_state) override;
  X_RESULT SetState(uint32_t user_index, X_INPUT_VIBRATION* vibration) override;
  X_RESULT GetKeystroke(uint32_t user_index, uint32_t flags,
                        X_INPUT_KEYSTROKE* out_keystroke) override;

  // Called from Objective-C on controller-connect / disconnect.
  void HandleConnect(void* gc_controller_raw);
  void HandleDisconnect(void* gc_controller_raw);

 private:
  struct Slot {
    IOSController* ios = nullptr;
    X_INPUT_STATE state{};
    uint16_t last_buttons = 0;
    bool state_changed = false;
  };

  Slot* FindSlot(uint32_t user_index);
  void SampleLocked(Slot& slot);

  std::mutex mutex_;
  std::array<Slot, HID_IOS_USER_COUNT> slots_{};

  // NSObject* to the Objective-C observer that bridges NSNotificationCenter
  // connect/disconnect callbacks back into this driver. Stored as void*
  // (__bridge_retained) so this header stays plain C++.
  void* observer_ = nullptr;
  std::atomic<bool> initialized_{false};
};

}  // namespace rex::input::ios

#endif  // REX_PLATFORM_IOS
