namespace gta4::input {
namespace {

MouseRotation TakeMouseRotation(uint8_t* base, uint32_t control, uint32_t camera,
                                double fov, bool wrap_yaw = true) {
  const InputEpoch epoch = ReadEpoch();
  if (!base || !control || !epoch.valid || !epoch.mouse_camera_candidate ||
      LoadU32(base, control + kControlUserIndexOffset) != epoch.state.user_index ||
      !(LoadFloat(base, kGameplayTimeStepAddress) > 0) ||
      !std::isfinite(LoadFloat(base, kGameplayTimeStepAddress))) return {};
  g_last_supported_mouse_camera_epoch.store(epoch.sequence, std::memory_order_relaxed);
  if (!epoch.mouse_camera_allowed || !epoch.state.mouse_has_motion) return {};
  auto rotation = MouseDisplacement(epoch.state.mouse_dx, epoch.state.mouse_dy,
      epoch.state.mouse_sensitivity, epoch.state.invert_mouse_y, fov);
  if (wrap_yaw) rotation.yaw = WrapMouseYaw(rotation.yaw);
  if (!rotation.moving()) return {};
  // Guest camera updates run on the calling game thread. No host pointers or
  // allocations survive a poll; camera addresses are identity tokens only.
  thread_local MouseCameraEpochs consumed;
  return consumed.Claim(epoch.sequence, camera) ? rotation : MouseRotation{};
}

bool MouseHoldingAim(uint8_t* base, uint32_t control) {
  const auto epoch = ReadEpoch();
  return control && epoch.mouse_camera_allowed && epoch.mouse_aim &&
      LoadU32(base, control + kControlUserIndexOffset) == epoch.state.user_index;
}

void AcceptMouseAngles(uint8_t* base, uint32_t camera, const MouseRotation& mouse,
                       uint32_t pitch, uint32_t yaw, uint32_t previous_pitch,
                       uint32_t previous_yaw) {
  // Retail final-angle jitter suppression snaps small rotations back to the
  // previous angle. Accept axes moved by the mouse before that comparison;
  // untouched controller axes retain their original comparison and state.
  if (mouse.pitch != 0)
    StoreU32(base, camera + previous_pitch, LoadU32(base, camera + pitch));
  if (mouse.yaw != 0)
    StoreU32(base, camera + previous_yaw, LoadU32(base, camera + yaw));
}

}  // namespace
}  // namespace gta4::input

#include "gta4_mouse_camera_guest.inc"
