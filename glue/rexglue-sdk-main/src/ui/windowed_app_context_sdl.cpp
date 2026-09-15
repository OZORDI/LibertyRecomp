/**
 * @file        ui/windowed_app_context_sdl.cpp
 * @brief       SDL3 implementation of the windowed app UI loop context
 *
 * @copyright   Copyright (c) 2026 Tom Clay <tomc@tctechstuff.com>
 *              All rights reserved.
 *
 * @license     BSD 3-Clause License
 *              See LICENSE file in the project root for full license text.
 */

#include <rex/ui/windowed_app_context_sdl.h>

#include <cstdlib>
#include <vector>

#include <SDL3/SDL.h>

#include <rex/logging.h>
#include <rex/platform.h>
#include <rex/ui/window_sdl.h>

namespace rex::ui {

SDLWindowedAppContext::~SDLWindowedAppContext() {
  // Execute leftover pending functions before the loop machinery goes away,
  // mirroring the shutdown contract documented in WindowedAppContext.
  ExecutePendingFunctionsFromUIThread();
  if (SDL_WasInit(SDL_INIT_VIDEO)) {
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
  }
}

bool SDLWindowedAppContext::Initialize() {
  // Touch is routed as a distinct multi-pointer stream. Disable both SDL
  // compatibility synthesis directions; WindowSDL also filters their reserved
  // IDs defensively in case a platform backend still produces one.
  if (!SDL_SetHintWithPriority(SDL_HINT_TOUCH_MOUSE_EVENTS, "0", SDL_HINT_OVERRIDE) ||
      !SDL_SetHintWithPriority(SDL_HINT_MOUSE_TOUCH_EVENTS, "0", SDL_HINT_OVERRIDE)) {
    REXLOG_WARN("Failed to disable SDL touch/mouse compatibility synthesis: {}", SDL_GetError());
  }
#if REX_PLATFORM_MAC
  // Games need held keys to repeat. SDL's macOS default enables the system
  // accent/character chooser while text input is active, so override it before
  // initializing video (when SDL registers its Cocoa application defaults).
  if (!SDL_SetHintWithPriority(SDL_HINT_MAC_PRESS_AND_HOLD, "0", SDL_HINT_OVERRIDE)) {
    REXLOG_ERROR("Failed to disable the macOS press-and-hold character chooser: {}",
                 SDL_GetError());
    return false;
  }

  // AppKit's system fullscreen Space intentionally constrains a window to the
  // display safe area on Macs with a camera housing. LibertyRecomp renders its
  // own fullscreen UI and receives SDL's safe-area rectangle separately, so
  // use SDL's supported non-Space fullscreen path. On Cocoa this creates a
  // borderless window from the complete display bounds, allowing the Metal
  // drawable to cover the auxiliary regions beside the camera housing.
  // NSPrefersDisplaySafeAreaCompatibilityMode=false in the app bundle prevents
  // macOS from shrinking those display bounds back to compatibility mode.
  if (!SDL_SetHintWithPriority(SDL_HINT_VIDEO_MAC_FULLSCREEN_SPACES, "0", SDL_HINT_OVERRIDE)) {
    REXLOG_ERROR("Failed to select full-display macOS fullscreen: {}", SDL_GetError());
    return false;
  }
#endif
#if REX_PLATFORM_GNU_LINUX
  // The Linux surface type the presenters consume is XcbWindow, so force X11
  // and keep SDL off Wayland where no surface shim exists yet.
  SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "x11");
#endif
  if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
    REXLOG_ERROR("SDL_InitSubSystem(SDL_INIT_VIDEO) failed: {}", SDL_GetError());
    return false;
  }
  uint32_t first = SDL_RegisterEvents(2);
  if (first == 0) {
    REXLOG_ERROR("SDL_RegisterEvents failed: {}", SDL_GetError());
    return false;
  }
  wakeup_event_type_ = first;
  paint_event_type_ = first + 1;
  return true;
}

void SDLWindowedAppContext::NotifyUILoopOfPendingFunctions() {
  // SDL_PushEvent is thread-safe by SDL contract.
  SDL_Event event{};
  event.type = wakeup_event_type_;
  SDL_PushEvent(&event);
}

void SDLWindowedAppContext::PlatformQuitFromUIThread() {
  REXLOG_INFO("app-quit-trace: source=quit-from-ui-thread");
  // RunMainMessageLoop re-checks HasQuitFromUIThread after every event; a
  // wakeup guarantees SDL_WaitEvent returns promptly if the queue is empty.
  NotifyUILoopOfPendingFunctions();
}

int SDLWindowedAppContext::RunMainMessageLoop() {
  while (!HasQuitFromUIThread()) {
    SDL_Event event;
    if (!SDL_WaitEvent(&event)) {
      REXLOG_ERROR("SDL_WaitEvent failed: {}", SDL_GetError());
      return EXIT_FAILURE;
    }
    ProcessEvent(event);
  }
  return EXIT_SUCCESS;
}

void SDLWindowedAppContext::ProcessEvent(SDL_Event& event) {
  if (event.type == wakeup_event_type_) {
    ExecutePendingFunctionsFromUIThread();
    return;
  }
  if (event.type == paint_event_type_) {
    if (WindowSDL* window = GetWindow(event.user.windowID)) {
      window->HandlePaintEvent(uint32_t(event.user.code));
    }
    return;
  }
  if (event.type >= SDL_EVENT_WINDOW_FIRST && event.type <= SDL_EVENT_WINDOW_LAST) {
    if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
      REXLOG_INFO("app-quit-trace: source=sdl-window-close-requested window={}",
                  event.window.windowID);
    }
    if (WindowSDL* window = GetWindow(event.window.windowID)) {
      window->HandleWindowEvent(event);
    }
    return;
  }
  switch (event.type) {
    case SDL_EVENT_QUIT: {
      REXLOG_INFO("app-quit-trace: source=sdl-quit windows={}", windows_.size());
      if (windows_.empty()) {
        QuitFromUIThread();
        break;
      }
      std::vector<WindowSDL*> windows_to_close;
      windows_to_close.reserve(windows_.size());
      for (const auto& [window_id, window] : windows_) {
        (void)window_id;
        windows_to_close.push_back(window);
      }
      for (WindowSDL* window : windows_to_close) {
        if (window && window->phase() == Window::Phase::kOpen) {
          window->RequestClose();
        }
        if (HasQuitFromUIThread()) {
          break;
        }
      }
      break;
    }
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP: {
      if (WindowSDL* window = GetWindow(event.key.windowID)) {
        window->HandleKeyEvent(event);
      }
      break;
    }
    case SDL_EVENT_TEXT_INPUT: {
      if (WindowSDL* window = GetWindow(event.text.windowID)) {
        window->HandleTextInputEvent(event);
      }
      break;
    }
    case SDL_EVENT_MOUSE_MOTION: {
      if (WindowSDL* window = GetWindow(event.motion.windowID)) {
        window->HandleMouseEvent(event);
      }
      break;
    }
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP: {
      if (WindowSDL* window = GetWindow(event.button.windowID)) {
        window->HandleMouseEvent(event);
      }
      break;
    }
    case SDL_EVENT_MOUSE_WHEEL: {
      if (WindowSDL* window = GetWindow(event.wheel.windowID)) {
        window->HandleMouseEvent(event);
      }
      break;
    }
    case SDL_EVENT_FINGER_DOWN:
    case SDL_EVENT_FINGER_MOTION:
    case SDL_EVENT_FINGER_UP:
    case SDL_EVENT_FINGER_CANCELED: {
      if (WindowSDL* window = GetWindow(event.tfinger.windowID)) {
        window->HandleTouchEvent(event);
      } else if (event.type == SDL_EVENT_FINGER_CANCELED) {
        // Some backends cancel a device after its window association has been
        // cleared. Give every live window the terminal edge; only the owner
        // will recognize the device/finger pair.
        for (const auto& [window_id, window] : windows_) {
          (void)window_id;
          if (window) {
            window->HandleTouchEvent(event);
          }
        }
      }
      break;
    }
    case SDL_EVENT_DROP_FILE: {
      if (WindowSDL* window = GetWindow(event.drop.windowID)) {
        window->HandleDropEvent(event);
      }
      break;
    }
    default:
      break;
  }
}

}  // namespace rex::ui
