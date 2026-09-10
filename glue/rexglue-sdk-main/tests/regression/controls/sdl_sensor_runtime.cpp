#include <array>
#include <atomic>
#include <mutex>
#include <optional>
#include <vector>
#include <cassert>
#include <iostream>
#include <rex/input/input_driver.h>
#include <rex/input/motion_sample_cache.h>
#include <SDL3/SDL.h>
#define private public
#include <rex/input/sdl/sdl_input_driver.h>
#undef private

int main() {
  SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
  assert(SDL_Init(SDL_INIT_GAMEPAD | SDL_INIT_EVENTS));
  SDL_VirtualJoystickSensorDesc sensor{SDL_SENSOR_ACCEL, 60.0f};
  SDL_VirtualJoystickDesc desc{};
  SDL_INIT_INTERFACE(&desc);
  desc.type=SDL_JOYSTICK_TYPE_GAMEPAD;
  desc.naxes=SDL_GAMEPAD_AXIS_COUNT;
  desc.nbuttons=SDL_GAMEPAD_BUTTON_COUNT;
  desc.nsensors=1;
  desc.sensors=&sensor;
  desc.name="Liberty controls sensor regression";
  desc.SetSensorsEnabled=[](void*,bool){return true;};
  const auto id=SDL_AttachVirtualJoystick(&desc);
  assert(id);
  rex::input::sdl::SDLInputDriver driver(nullptr,0,true);
  assert(SDL_AddEventWatch(&rex::input::sdl::SDLInputDriver::EventWatch,&driver));
  // Open ONLY the virtual device; do not inventory or open physical devices.
  driver.OpenControllerLocked(id);
  const auto slot=driver.GetControllerIndexFromInstanceID(id);
  assert(slot);
  auto* pad=driver.controllers_[*slot].sdl;
  auto* joystick=SDL_GetGamepadJoystick(pad);
  assert(joystick&&SDL_GamepadSensorEnabled(pad,SDL_SENSOR_ACCEL));
  const float gravity[3]={0.0f,9.80665f,0.0f};
  assert(SDL_SendJoystickVirtualSensorData(joystick,SDL_SENSOR_ACCEL,1,gravity,3));
  {
    // Real SDL dispatches sensor callbacks under its joystick lock. The
    // production driver already holds this controller lock at that boundary.
    std::lock_guard lock(driver.controllers_mutex_);
    SDL_UpdateGamepads();
  }
  rex::input::MotionState first{},second{};
  assert(driver.motion_samples_.Read(id,SDL_GetTicksNS(),first));
  assert(first.valid_samples&rex::input::kMotionSensorAccelerometer);
  assert(first.accelerometer_sensor_timestamp_ns==1);
  assert(first.accelerometer_host_timestamp_ns!=0);
  float cached[3]{};
  for (unsigned i=0;i<20;++i)assert(SDL_GetGamepadSensorData(pad,SDL_SENSOR_ACCEL,cached,3));
  assert(driver.motion_samples_.Read(id,SDL_GetTicksNS(),second));
  assert(second.sequence==first.sequence);
  assert(second.accelerometer_host_timestamp_ns==first.accelerometer_host_timestamp_ns);
  std::cout<<"PASS real SDL cached getters do not renew the production sensor cache\n";
  SDL_Delay(2);
  assert(SDL_SendJoystickVirtualSensorData(joystick,SDL_SENSOR_ACCEL,2,gravity,3));
  {
    std::lock_guard lock(driver.controllers_mutex_);
    SDL_UpdateGamepads();
  }
  assert(driver.motion_samples_.Read(id,SDL_GetTicksNS(),second));
  assert(second.sequence==first.sequence+1);
  assert(second.acceleration_m_s2==first.acceleration_m_s2);
  assert(second.accelerometer_host_timestamp_ns>first.accelerometer_host_timestamp_ns);
  std::cout<<"PASS identical measurements advance only on actual SDL sensor delivery\n";
  SDL_RemoveEventWatch(&rex::input::sdl::SDLInputDriver::EventWatch,&driver);
  driver.CloseControllerLocked(*slot,"regression-complete");
  assert(!driver.motion_samples_.Read(id,SDL_GetTicksNS(),second));
  assert(SDL_DetachVirtualJoystick(id));
  SDL_Quit();
  std::cout<<"PASS sensor callback lock ordering and device teardown\n";
}
