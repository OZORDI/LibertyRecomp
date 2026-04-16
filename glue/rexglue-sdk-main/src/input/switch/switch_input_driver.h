#pragma once
/**
 * Switch (libnx) input driver for ReXGlue.
 * Maps up to 8 Npads + Handheld onto X360 user indices 0..7.
 */

#include <rex/input/input_driver.h>

#if REX_PLATFORM_NX

#include <array>
#include <cstdint>

#include <switch/services/hid.h>

namespace rex::input::nx {

class SwitchInputDriver final : public InputDriver {
 public:
  explicit SwitchInputDriver(rex::ui::Window* window, size_t window_z_order);
  ~SwitchInputDriver() override;

  X_STATUS Setup() override;

  X_RESULT GetCapabilities(uint32_t user_index, uint32_t flags,
                           X_INPUT_CAPABILITIES* out_caps) override;
  X_RESULT GetState(uint32_t user_index, X_INPUT_STATE* out_state) override;
  X_RESULT SetState(uint32_t user_index, X_INPUT_VIBRATION* vibration) override;
  X_RESULT GetKeystroke(uint32_t user_index, uint32_t flags,
                        X_INPUT_KEYSTROKE* out_keystroke) override;

 private:
  struct UserSlot {
    HidNpadIdType id;
    uint32_t packet_number;
    uint16_t last_buttons;
    HidVibrationDeviceHandle vibration[2];
    bool vibration_ready;
    HidSixAxisSensorHandle sixaxis[2];
    bool sixaxis_ready;
  };

  static HidNpadIdType IndexToId(uint32_t user_index);
  bool ReadPad(UserSlot& slot, uint32_t* buttons_out,
               HidAnalogStickState* l_out, HidAnalogStickState* r_out);
  void EnsureHandles(UserSlot& slot, uint32_t style_set);

  std::array<UserSlot, 8> slots_{};
  bool initialized_ = false;
};

}  // namespace rex::input::nx

#endif  // REX_PLATFORM_NX
