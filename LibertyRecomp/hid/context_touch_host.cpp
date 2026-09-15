#include "context_touch_host.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <deque>
#include <mutex>
#include <unordered_map>
#include <vector>

#include <rex/input/absolute_pointer.h>
#include <rex/platform.h>

#include "input/context_touch_controls.h"
#include "input/context_touch_settings.h"

#if !REX_PLATFORM_CONSOLE
#include <SDL3/SDL.h>
#include <rex/input/sdl/physical_device_inventory.h>
#elif REX_PLATFORM_NX
#include <switch.h>
#endif

namespace TouchHost {
#if REX_PLATFORM_NX
void UpdateSwitchWindow(bool focused, int& width, int& height);
#endif
namespace {

std::mutex geometry_mutex;
float logical_width = 0.0f;
float logical_height = 0.0f;
float safe_x = 0.0f;
float safe_y = 0.0f;
float safe_width = 0.0f;
float safe_height = 0.0f;
rex::ui::GuestOutputTransform presentation;

uint64_t Timestamp() {
  return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(
      std::chrono::steady_clock::now().time_since_epoch()).count());
}

void PublishGeometryLocked(uint64_t timestamp) {
  auto& service = rex::input::GetAbsolutePointerService();
  service.SetLogicalSize(logical_width, logical_height, timestamp);
  if (!presentation.IsValid() || logical_width <= 0.0f || logical_height <= 0.0f) return;
  const float scale_x = float(presentation.surface_width) / logical_width;
  const float scale_y = float(presentation.surface_height) / logical_height;
  service.UpdatePresentation(presentation, int32_t(std::lround(safe_x * scale_x)),
      int32_t(std::lround(safe_y * scale_y)), int32_t(std::lround(safe_width * scale_x)),
      int32_t(std::lround(safe_height * scale_y)), timestamp);
}

#if !REX_PLATFORM_CONSOLE
SDL_Window* sdl_window = nullptr;
std::mutex event_mutex;
std::deque<SDL_Event> pending_events;
rex::input::sdl::PhysicalDeviceInventory sdl_device_inventory;

void RefreshSDLGeometry(uint64_t timestamp) {
  if (!sdl_window) return;
  int width = 0, height = 0, pixels_x = 0, pixels_y = 0;
  SDL_GetWindowSize(sdl_window, &width, &height);
  SDL_GetWindowSizeInPixels(sdl_window, &pixels_x, &pixels_y);
  if (width <= 0 || height <= 0 || pixels_x <= 0 || pixels_y <= 0) {
    rex::input::GetAbsolutePointerService().CancelAll(timestamp);
    return;
  }
  SDL_Rect safe{0, 0, width, height};
  SDL_Rect reported_safe{};
  if (SDL_GetWindowSafeArea(sdl_window, &reported_safe) &&
      reported_safe.w > 0 && reported_safe.h > 0)
    safe = reported_safe;
  std::lock_guard lock(geometry_mutex);
  logical_width = float(width);
  logical_height = float(height);
  safe_x = float(safe.x);
  safe_y = float(safe.y);
  safe_width = float(safe.w);
  safe_height = float(safe.h);
  if (presentation.surface_width != uint32_t(pixels_x) ||
      presentation.surface_height != uint32_t(pixels_y)) {
    // The renderer replaces the provisional full-window transform with the
    // exact final output rectangle before submission of the next frame.
    presentation = {0, uint32_t(pixels_x), uint32_t(pixels_y), uint32_t(pixels_x),
        uint32_t(pixels_y), 0, 0, uint32_t(pixels_x), uint32_t(pixels_y),
        uint32_t(pixels_x), uint32_t(pixels_y)};
  }
  PublishGeometryLocked(timestamp);
}
#elif REX_PLATFORM_NX
struct Contact { float x = 0.0f; float y = 0.0f; };
std::unordered_map<uint32_t, Contact> switch_contacts;
uint64_t last_sampling_number = 0;
AppletOperationMode last_operation_mode = AppletOperationMode_Handheld;
bool switch_focused = false;
bool switch_initialized = false;
bool switch_wait_for_release = false;
bool switch_usb_initialized = false;
uint64_t next_switch_inventory_ns = 0;

void RefreshSwitchDeviceInventory(uint64_t timestamp, bool force) {
  if (!force && timestamp < next_switch_inventory_ns) return;
  next_switch_inventory_ns = timestamp + 1000000000;
  auto& service = rex::input::GetAbsolutePointerService();
  HidMouseState mouse{};
  if (hidGetMouseStates(&mouse, 1) > 0)
    service.ReplacePhysicalMice((mouse.attributes & HidMouseAttribute_IsConnected) ?
        std::vector<uint64_t>{1} : std::vector<uint64_t>{}, timestamp);

  constexpr std::array ids{HidNpadIdType_No1, HidNpadIdType_No2, HidNpadIdType_No3,
      HidNpadIdType_No4, HidNpadIdType_No5, HidNpadIdType_No6, HidNpadIdType_No7,
      HidNpadIdType_No8, HidNpadIdType_Handheld};
  std::vector<uint64_t> controllers;
  for (const auto id : ids) {
    const uint32_t styles = hidGetNpadStyleSet(id);
    const auto connected = [id](auto read, bool require_joycon = false) {
      HidNpadCommonState state{};
      if (read(id, &state, 1) == 0 || !(state.attributes & HidNpadAttribute_IsConnected))
        return false;
      // A handheld logical slot alone is not proof that either Joy-Con exists.
      return !require_joycon || (state.attributes &
          (HidNpadAttribute_IsLeftConnected | HidNpadAttribute_IsRightConnected)) != 0;
    };
    if (((styles & HidNpadStyleTag_NpadFullKey) && connected(hidGetNpadStatesFullKey)) ||
        ((styles & HidNpadStyleTag_NpadHandheld) && connected(hidGetNpadStatesHandheld, true)) ||
        ((styles & HidNpadStyleTag_NpadJoyDual) && connected(hidGetNpadStatesJoyDual, true)) ||
        ((styles & HidNpadStyleTag_NpadJoyLeft) && connected(hidGetNpadStatesJoyLeft)) ||
        ((styles & HidNpadStyleTag_NpadJoyRight) && connected(hidGetNpadStatesJoyRight)))
      controllers.push_back(uint64_t(id) + 1);
  }
  service.ReplaceGameControllers(controllers, timestamp);

  if (switch_usb_initialized) {
    UsbHsInterfaceFilter filter{};
    filter.Flags = UsbHsInterfaceFilterFlags_bInterfaceClass |
        UsbHsInterfaceFilterFlags_bInterfaceSubClass | UsbHsInterfaceFilterFlags_bInterfaceProtocol;
    filter.bInterfaceClass = USB_CLASS_HID;
    filter.bInterfaceSubClass = 1;  // USB HID boot interface.
    filter.bInterfaceProtocol = 1;  // Keyboard boot protocol.
    std::array<UsbHsInterface, 16> interfaces{};
    s32 count = 0;
    // This query reads descriptors from usb:hs; it never acquires interfaces,
    // submits USB transfers, or waits for an attached device to respond.
    if (R_SUCCEEDED(usbHsQueryAllInterfaces(&filter, interfaces.data(), sizeof(interfaces), &count)))
      service.ReplacePhysicalKeyboards(count > 0 ? std::vector<uint64_t>{1} :
          std::vector<uint64_t>{}, timestamp);
  }
}
#endif

}  // namespace

void Initialize(const std::filesystem::path& settings_path) {
  gta4::input::ConfigureContextTouchSettings(settings_path);
  gta4::input::InitializeContextTouchControls();
#if REX_PLATFORM_NX
  hidInitializeTouchScreen();
  hidInitializeMouse();
  // An absent service can leave SM GetService waiting for registration. Use
  // usb:hs only when another host component or the loader already provides it.
  if (serviceIsActive(usbHsGetServiceSession()) ||
      smGetServiceOverride(smEncodeName("usb:hs")) != INVALID_HANDLE)
    switch_usb_initialized = R_SUCCEEDED(usbHsInitialize());
  switch_initialized = true;
  last_operation_mode = appletGetOperationMode();
#endif
}

void Cancel() {
  rex::input::GetAbsolutePointerService().SetFocused(false, Timestamp());
  gta4::input::FlushContextTouchSettings();
}

void Shutdown() {
  Cancel();
  gta4::input::ShutdownContextTouchControls();
#if REX_PLATFORM_NX
  if (switch_usb_initialized) usbHsExit();
  switch_usb_initialized = false;
  switch_initialized = false;
  switch_contacts.clear();
#endif
}

void UpdatePresentation(uint32_t width, uint32_t height,
                        uint32_t output_width, uint32_t output_height) {
  std::lock_guard lock(geometry_mutex);
  presentation = {0, width, height, width, height,
      (int32_t(width) - int32_t(output_width)) / 2,
      (int32_t(height) - int32_t(output_height)) / 2,
      output_width, output_height, output_width, output_height};
  PublishGeometryLocked(Timestamp());
}

void AttachSDLWindow(SDL_Window* window) {
#if !REX_PLATFORM_CONSOLE
  sdl_window = window;
  const uint64_t timestamp = SDL_GetTicksNS();
  RefreshSDLGeometry(timestamp);
  if (window) sdl_device_inventory.Refresh(timestamp, true);
  rex::input::GetAbsolutePointerService().SetFocused(
      window && (SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS), timestamp);
#else
  (void)window;
#endif
}

static void ProcessSDLEvent(const SDL_Event& event) {
#if !REX_PLATFORM_CONSOLE
  if (!sdl_window) return;
  auto& service = rex::input::GetAbsolutePointerService();
  const uint64_t timestamp = event.common.timestamp;
  const SDL_WindowID id = SDL_GetWindowID(sdl_window);
  if (event.type >= SDL_EVENT_WINDOW_FIRST && event.type <= SDL_EVENT_WINDOW_LAST &&
      event.window.windowID != id) return;
  switch (event.type) {
    case SDL_EVENT_FINGER_DOWN:
    case SDL_EVENT_FINGER_MOTION:
    case SDL_EVENT_FINGER_UP:
    case SDL_EVENT_FINGER_CANCELED: {
      if (event.tfinger.windowID != id ||
          SDL_GetTouchDeviceType(event.tfinger.touchID) != SDL_TOUCH_DEVICE_DIRECT) return;
      RefreshSDLGeometry(timestamp);
      int width = 0, height = 0;
      SDL_GetWindowSizeInPixels(sdl_window, &width, &height);
      const auto phase = event.type == SDL_EVENT_FINGER_DOWN ? rex::input::AbsolutePointerPhase::kDown :
          event.type == SDL_EVENT_FINGER_MOTION ? rex::input::AbsolutePointerPhase::kMove :
          event.type == SDL_EVENT_FINGER_UP ? rex::input::AbsolutePointerPhase::kUp :
          rex::input::AbsolutePointerPhase::kCancel;
      service.SubmitPointer(uint64_t(event.tfinger.touchID), uint64_t(event.tfinger.fingerID),
          phase, event.tfinger.x * float(width), event.tfinger.y * float(height),
          event.tfinger.pressure, timestamp);
      break;
    }
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
      if (event.key.windowID == id && event.key.which != 0) service.NotifyPhysicalInput(timestamp);
      break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
      if (event.button.windowID == id && rex::input::sdl::IsPhysicalMouseId(event.button.which))
        service.NotifyPhysicalInput(timestamp);
      break;
    case SDL_EVENT_MOUSE_MOTION:
      if (event.motion.windowID == id && rex::input::sdl::IsPhysicalMouseId(event.motion.which) &&
          (event.motion.xrel != 0.0f || event.motion.yrel != 0.0f))
        service.NotifyPhysicalInput(timestamp);
      break;
    case SDL_EVENT_MOUSE_WHEEL:
      if (event.wheel.windowID == id && rex::input::sdl::IsPhysicalMouseId(event.wheel.which))
        service.NotifyPhysicalInput(timestamp);
      break;
    case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
    case SDL_EVENT_GAMEPAD_BUTTON_UP:
      service.NotifyPhysicalInput(timestamp);
      break;
    case SDL_EVENT_GAMEPAD_AXIS_MOTION:
      if (std::abs(int(event.gaxis.value)) > 8192) service.NotifyPhysicalInput(timestamp);
      break;
    case SDL_EVENT_WINDOW_FOCUS_LOST:
    case SDL_EVENT_WINDOW_HIDDEN:
    case SDL_EVENT_WINDOW_MINIMIZED:
    case SDL_EVENT_WILL_ENTER_BACKGROUND:
    case SDL_EVENT_DID_ENTER_BACKGROUND:
    case SDL_EVENT_TERMINATING:
    case SDL_EVENT_QUIT:
      service.SetFocused(false, timestamp);
      gta4::input::FlushContextTouchSettings();
      break;
    case SDL_EVENT_WINDOW_FOCUS_GAINED:
    case SDL_EVENT_DID_ENTER_FOREGROUND:
      RefreshSDLGeometry(timestamp);
      sdl_device_inventory.Refresh(timestamp, true);
      service.SetFocused((SDL_GetWindowFlags(sdl_window) & SDL_WINDOW_INPUT_FOCUS) != 0, timestamp);
      break;
    case SDL_EVENT_KEYBOARD_ADDED:
    case SDL_EVENT_KEYBOARD_REMOVED:
    case SDL_EVENT_MOUSE_ADDED:
    case SDL_EVENT_MOUSE_REMOVED:
    case SDL_EVENT_GAMEPAD_ADDED:
    case SDL_EVENT_GAMEPAD_REMOVED:
      break;
    case SDL_EVENT_WINDOW_RESIZED:
    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
    case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
    case SDL_EVENT_WINDOW_SAFE_AREA_CHANGED:
    case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
      service.CancelAll(timestamp);
      RefreshSDLGeometry(timestamp);
      break;
  }
#else
  (void)event;
#endif
}

void OnSDLEvent(const SDL_Event& event) {
#if !REX_PLATFORM_CONSOLE
  // SDL watches can run on platform input threads. Window queries and touch
  // device classification are deferred to the main event-pumping thread.
  const bool move = event.type == SDL_EVENT_FINGER_MOTION ||
                    event.type == SDL_EVENT_MOUSE_MOTION;
  const bool relevant = (event.type >= SDL_EVENT_WINDOW_FIRST && event.type <= SDL_EVENT_WINDOW_LAST) ||
      event.type == SDL_EVENT_FINGER_DOWN || event.type == SDL_EVENT_FINGER_UP ||
      event.type == SDL_EVENT_FINGER_CANCELED || move ||
      event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP ||
      event.type == SDL_EVENT_KEYBOARD_ADDED || event.type == SDL_EVENT_KEYBOARD_REMOVED ||
      event.type == SDL_EVENT_MOUSE_ADDED || event.type == SDL_EVENT_MOUSE_REMOVED ||
      event.type == SDL_EVENT_GAMEPAD_ADDED || event.type == SDL_EVENT_GAMEPAD_REMOVED ||
      event.type == SDL_EVENT_MOUSE_BUTTON_DOWN || event.type == SDL_EVENT_MOUSE_BUTTON_UP ||
      event.type == SDL_EVENT_MOUSE_WHEEL || event.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN ||
      event.type == SDL_EVENT_GAMEPAD_BUTTON_UP || event.type == SDL_EVENT_GAMEPAD_AXIS_MOTION ||
      event.type == SDL_EVENT_WILL_ENTER_BACKGROUND || event.type == SDL_EVENT_DID_ENTER_BACKGROUND ||
      event.type == SDL_EVENT_DID_ENTER_FOREGROUND || event.type == SDL_EVENT_TERMINATING ||
      event.type == SDL_EVENT_QUIT;
  if (!relevant) return;
  if (event.type == SDL_EVENT_KEYBOARD_ADDED || event.type == SDL_EVENT_KEYBOARD_REMOVED ||
      event.type == SDL_EVENT_MOUSE_ADDED || event.type == SDL_EVENT_MOUSE_REMOVED ||
      event.type == SDL_EVENT_GAMEPAD_ADDED || event.type == SDL_EVENT_GAMEPAD_REMOVED)
    sdl_device_inventory.MarkDirty();
  if (event.type == SDL_EVENT_WILL_ENTER_BACKGROUND ||
      event.type == SDL_EVENT_DID_ENTER_BACKGROUND || event.type == SDL_EVENT_TERMINATING) {
    rex::input::GetAbsolutePointerService().SetFocused(false, event.common.timestamp);
    gta4::input::FlushContextTouchSettings();
  }
  std::lock_guard lock(event_mutex);
  if (move && pending_events.size() >= rex::input::AbsolutePointerService::kDefaultMaxQueuedMoves) {
    const auto old_move = std::find_if(pending_events.begin(), pending_events.end(), [](const auto& queued) {
      return queued.type == SDL_EVENT_FINGER_MOTION || queued.type == SDL_EVENT_MOUSE_MOTION;
    });
    if (old_move != pending_events.end()) pending_events.erase(old_move);
    else return;
  }
  pending_events.push_back(event);
#else
  (void)event;
#endif
}

void PumpSDLEvents() {
#if !REX_PLATFORM_CONSOLE
  std::deque<SDL_Event> events;
  {
    std::lock_guard lock(event_mutex);
    events.swap(pending_events);
  }
  if (sdl_window) sdl_device_inventory.Refresh(SDL_GetTicksNS());
  for (const auto& event : events) ProcessSDLEvent(event);
#endif
}

void PumpSwitch() {
#if REX_PLATFORM_NX
  if (!switch_initialized) return;
  const uint64_t timestamp = Timestamp();
  auto& service = rex::input::GetAbsolutePointerService();
  const auto mode = appletGetOperationMode();
  const bool focused = appletGetFocusState() == AppletFocusState_InFocus &&
                       mode == AppletOperationMode_Handheld;
  RefreshSwitchDeviceInventory(timestamp, focused != switch_focused || mode != last_operation_mode);
  if (focused != switch_focused || mode != last_operation_mode) {
    service.CancelAll(timestamp);
    switch_contacts.clear();
    switch_wait_for_release = true;
    switch_focused = focused;
    last_operation_mode = mode;
    gta4::input::FlushContextTouchSettings();
  }
  int window_width = 0, window_height = 0;
  UpdateSwitchWindow(appletGetFocusState() == AppletFocusState_InFocus, window_width, window_height);
  {
    std::lock_guard lock(geometry_mutex);
    logical_width = float(window_width);
    logical_height = float(window_height);
    safe_x = safe_y = 0.0f;
    safe_width = logical_width;
    safe_height = logical_height;
    PublishGeometryLocked(timestamp);
  }
  service.SetFocused(focused, timestamp);
  // libnx returns newest first. Consume the whole fresh history in temporal
  // order so a tap between applet-loop iterations still has Down and Up.
  std::array<HidTouchScreenState, 17> states{};
  const int count = hidGetTouchScreenStates(states.data(), int(states.size()));
  for (int index = count; index > 0;) {
    const auto& state = states[--index];
    if (state.sampling_number <= last_sampling_number) continue;
    if (last_sampling_number != 0 && state.sampling_number > last_sampling_number + 1) {
      // An overrun can hide a release/reuse of finger_id. Existing owners must
      // be cancelled and a fresh physical contact required after the gap.
      service.CancelAll(timestamp);
      switch_contacts.clear();
      switch_wait_for_release = true;
    }
    last_sampling_number = state.sampling_number;
    if (!focused) continue;
    if (switch_wait_for_release) {
      if (state.count == 0) switch_wait_for_release = false;
      continue;
    }
    std::unordered_map<uint32_t, Contact> next;
    for (int contact_index = 0;
         contact_index < state.count && contact_index < int(std::size(state.touches));
         ++contact_index) {
      const auto& touch = state.touches[contact_index];
      const Contact point{float(touch.x), float(touch.y)};
      const bool start = (touch.attributes & HidTouchAttribute_Start) != 0;
      const bool end = (touch.attributes & HidTouchAttribute_End) != 0;
      if (start && switch_contacts.contains(touch.finger_id)) {
        service.SubmitPointer(1, touch.finger_id, rex::input::AbsolutePointerPhase::kCancel,
            point.x, point.y, 0.0f, timestamp);
        switch_contacts.erase(touch.finger_id);
      }
      if (end) {
        if (start)
          service.SubmitPointer(1, touch.finger_id, rex::input::AbsolutePointerPhase::kDown,
              point.x, point.y, 1.0f, timestamp);
        service.SubmitPointer(1, touch.finger_id, rex::input::AbsolutePointerPhase::kUp,
            point.x, point.y, 0.0f, timestamp);
        switch_contacts.erase(touch.finger_id);
        continue;
      }
      next.emplace(touch.finger_id, point);
      service.SubmitPointer(1, touch.finger_id,
          switch_contacts.contains(touch.finger_id) ? rex::input::AbsolutePointerPhase::kMove :
                                                    rex::input::AbsolutePointerPhase::kDown,
          point.x, point.y, 1.0f, timestamp);
    }
    for (const auto& [finger, point] : switch_contacts) {
      if (!next.contains(finger))
        service.SubmitPointer(1, finger, rex::input::AbsolutePointerPhase::kUp,
            point.x, point.y, 0.0f, timestamp);
    }
    switch_contacts = std::move(next);
  }
#endif
}

}  // namespace TouchHost
