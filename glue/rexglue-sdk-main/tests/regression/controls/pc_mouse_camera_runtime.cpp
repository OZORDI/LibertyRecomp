// Executes the actual generated integration blocks and the derived hook blocks
// against the same guest-memory fixture. It checks that mouse movement changes
// the camera angle while leaving the stored controller filter state identical.
#include "pc_mouse_camera_blocks.inc"
#include <cassert>
#include <cmath>

int main() {
  for (const double dt : kCameraTimesteps) {
    for (int camera = 0; camera < 3; ++camera) {
      Fixture reference = MakeFixture(camera, dt);
      Fixture native = reference;
      requested_mouse = {};
      RunReference(camera, reference);
      RunNative(camera, native);
      assert(reference.memory.words == native.memory.words);

      for (const auto rotation : kCameraRotations) {
        reference = MakeFixture(camera, dt);
        native = reference;
        requested_mouse = rotation;
        RunReference(camera, reference);
        RunNative(camera, native);
        const auto spec = kCameraFields[camera];
        for (const auto field : {spec.pitch_filter, spec.yaw_filter}) {
          assert(reference.memory.words.at(kCamera + field) ==
                 native.memory.words.at(kCamera + field));
        }
        if (camera == 0) {
          // Weapon helper exposes its transient pitch rate; heading is in
          // the stack output subsequently constrained by the retail helper.
          assert(std::abs((native.ctx.f30.f64 - reference.ctx.f30.f64) * dt * 30.0 -
                          rotation.pitch) < kCameraTolerance);
          assert(std::abs(Read(native.memory, kStackYaw) - Read(reference.memory, kStackYaw) -
                          rotation.yaw) < kCameraTolerance);
        } else {
          assert(std::abs(Read(native.memory, kCamera + spec.pitch) -
                          Read(reference.memory, kCamera + spec.pitch) - rotation.pitch)
                 < kCameraTolerance);
          assert(std::abs(Read(native.memory, kCamera + spec.yaw) -
                          Read(reference.memory, kCamera + spec.yaw) - rotation.yaw)
                 < kCameraTolerance);
        }
      }
      if (camera == 2) {
        for (const double direction : {-1.0, 1.0}) {
          native = MakeFixture(camera, dt);
          requested_mouse = {direction * 100.0, 0};
          RunNative(camera, native);
          assert(std::abs(Read(native.memory, kCamera + kCameraFields[camera].yaw) -
                          direction * 1.3) < kCameraTolerance);
        }
      }
    }
  }
}
