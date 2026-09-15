#include "gta4_mouse_aim_policy.h"
#include "pc_mouse_samples.inc"

#include <cassert>
#include <cmath>
#include <limits>

using namespace gta4::input;

int main() {
  // Exercise the actual policy against both retail pressure partitions.
  for (unsigned threshold = 12; threshold < 255; ++threshold) {
    const auto normal = MouseAimPressure(true, false, threshold);
    const auto alternate = MouseAimPressure(true, true, threshold);
    assert(normal > 10 && normal < threshold);
    assert(alternate > threshold);
    assert(MouseAimPressure(false, false, threshold) == 255);
    assert(MouseAimPressure(false, true, threshold) == 255);
  }
  assert(MouseAimPressure(true, false, 238) == 237);
  assert(MouseAimPressure(true, true, 238) == 255);
  assert(MouseAimPressure(true, true, 255) == 0);
  assert(MouseAimPressure(true, false, 11) == 0);
  assert(!MouseFreeAimWeapon(0));
  assert(MouseFreeAimWeapon(1));
  assert(MouseFreeAimWeapon(2));
  assert(MouseFreeAimWeapon(3));
  assert(MouseFreeAimWeapon(4));
  assert(!MouseFreeAimWeapon(5));
  assert(!MouseFreeAimWeapon(UINT32_MAX));

  MouseAimLatch aim;
  assert(!aim.Update(true, false, false, false, 1, 100));
  assert(aim.Update(true, false, true, true, 1, 100));
  assert(!aim.Update(true, false, false, false, 1, 100));
  assert(!aim.Update(true, true, false, false, 1, 100));
  // A quick down/up between polls still toggles once from the pressed edge.
  assert(aim.Update(true, true, false, true, 1, 100));
  assert(aim.Update(true, true, false, false, 1, 100));
  assert(!aim.Update(true, true, true, true, 1, 100));
  assert(!aim.Update(true, true, true, false, 1, 100));
  assert(aim.Update(true, true, true, true, 1, 100));
  assert(!aim.Update(false, true, false, false, 1, 100));
  assert(!aim.Update(true, true, false, false, 1, 100));
  assert(aim.Update(true, true, false, true, 1, 100));
  assert(!aim.Update(true, true, false, false, 2, 100));
  assert(aim.Update(true, true, false, true, 2, 100));
  assert(!aim.Update(true, true, false, false, 2, 200));

  for (const auto& sample : kMouseSamples) {
    const auto rotation = MouseDisplacement(sample.dx, sample.dy, sample.sensitivity,
                                            sample.invert, sample.fov);
    assert(std::abs(rotation.yaw - sample.yaw) < kTolerance);
    assert(std::abs(rotation.pitch - sample.pitch) < kTolerance);
    for (const double dt : kTimesteps) {
      assert(std::abs(MouseRotationRate(rotation.yaw, dt) * dt * 30.0 - sample.yaw)
             < kTolerance);
    }
  }
  assert(!MouseDisplacement(0, 0, 1, false, 45).moving());
  assert(!MouseDisplacement(std::numeric_limits<double>::quiet_NaN(), 1, 1, false, 45).moving());
  assert(!MouseDisplacement(1, 1, std::numeric_limits<double>::infinity(), false, 45).moving());
  assert(!MouseDisplacement(1, 1, 1, false, 0).moving());
  assert(!MouseDisplacement(1, 1, 1, false, 180).moving());
  assert(MouseRotationRate(1, 0) == 0);
  assert(MouseRotationRate(1, -1) == 0);
  assert(MouseRotationRate(1, std::numeric_limits<double>::quiet_NaN()) == 0);
  for (const auto& sample : kPitchSamples) {
    assert(std::abs(ConstrainMousePitch(sample.angle, sample.displacement,
                                       sample.lower, sample.upper) - sample.applied) < kTolerance);
  }
  for (const auto& sample : kYawSamples) {
    assert(std::abs(WrapMouseYaw(sample.input) - sample.output) < kTolerance);
  }

  MouseCameraEpochs epochs;
  assert(!epochs.Claim(1, 0));
  assert(epochs.Claim(1, 100));
  assert(!epochs.Claim(1, 100));
  assert(epochs.Claim(1, 200));
  assert(epochs.Claim(2, 100));
  for (unsigned camera = 1; camera <= 31; ++camera) assert(epochs.Claim(2, camera));
  assert(!epochs.Claim(2, 200));
  assert(epochs.Claim(3, 200));
}
