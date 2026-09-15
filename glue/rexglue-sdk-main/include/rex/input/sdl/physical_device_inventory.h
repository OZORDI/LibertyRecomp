#pragma once

#include <atomic>
#include <cstdint>

#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_pen.h>
#include <SDL3/SDL_touch.h>

namespace rex::input::sdl {

inline bool IsPhysicalMouseId(SDL_MouseID id) noexcept {
  return id != 0 && id != SDL_TOUCH_MOUSEID && id != SDL_PEN_MOUSEID;
}

// SDL's event watch may mark an inventory dirty from an input thread. Device
// enumeration and native registry queries always run on the SDL main thread.
class PhysicalDeviceInventory {
 public:
  void MarkDirty() noexcept { dirty_.store(true, std::memory_order_release); }
  void Refresh(uint64_t timestamp_ns, bool force = false);

 private:
  std::atomic<bool> dirty_{true};
  uint64_t next_refresh_ms_ = 0;
};

}  // namespace rex::input::sdl
