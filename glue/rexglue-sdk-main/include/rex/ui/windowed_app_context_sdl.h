/**
 * @file        ui/windowed_app_context_sdl.h
 * @brief       SDL3 implementation of the windowed app UI loop context
 *
 * @copyright   Copyright (c) 2026 Tom Clay <tomc@tctechstuff.com>
 *              All rights reserved.
 *
 * @license     BSD 3-Clause License
 *              See LICENSE file in the project root for full license text.
 */

#pragma once

#include <cstdint>
#include <unordered_map>

#include <SDL3/SDL_events.h>

#include <rex/ui/windowed_app_context.h>

namespace rex::ui {

class WindowSDL;

class SDLWindowedAppContext final : public WindowedAppContext {
 public:
  SDLWindowedAppContext() = default;
  ~SDLWindowedAppContext() override;

  // Initializes the SDL video subsystem and registers the custom event types.
  // Must be called (and must succeed) before any other use.
  bool Initialize();

  void NotifyUILoopOfPendingFunctions() override;
  void PlatformQuitFromUIThread() override;

  int RunMainMessageLoop();

  // Custom SDL event type carrying a coalesced paint request for a window
  // (user.windowID identifies the target).
  uint32_t paint_event_type() const { return paint_event_type_; }

  // Window registry for event routing. UI thread only.
  void RegisterWindow(SDL_WindowID id, WindowSDL* window) { windows_.emplace(id, window); }
  void UnregisterWindow(SDL_WindowID id) { windows_.erase(id); }

 private:
  WindowSDL* GetWindow(SDL_WindowID id) const {
    auto it = windows_.find(id);
    return it != windows_.end() ? it->second : nullptr;
  }

  void ProcessEvent(SDL_Event& event);

  std::unordered_map<SDL_WindowID, WindowSDL*> windows_;
  uint32_t wakeup_event_type_ = 0;
  uint32_t paint_event_type_ = 0;
};

}  // namespace rex::ui
