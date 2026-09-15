#include <rex/input/absolute_pointer.h>
#include <rex/input/input.h>
#include <cassert>
#include <iostream>
#include <vector>

using namespace rex::input;

std::vector<AbsolutePointerEvent> Drain(AbsolutePointerService& service) {
  std::vector<AbsolutePointerEvent> out;
  AbsolutePointerEvent event;
  while(service.TryDequeue(&event)) out.push_back(event);
  return out;
}

int main() {
  rex::ui::GuestOutputTransform transform;
  transform.revision=1;transform.surface_width=2400;transform.surface_height=1080;
  transform.host_render_target_width=2400;transform.host_render_target_height=1080;
  transform.output_x=300;transform.output_y=0;transform.output_width=1800;transform.output_height=1080;
  transform.guest_width=1280;transform.guest_height=720;
  AbsolutePointerService service(2);
  service.SetLogicalSize(800,360);
  service.UpdatePresentation(transform,90,0,2220,1080,1);
  service.SetFocused(true,2);
  service.AddPhysicalKeyboard(7);service.AddGameController(8);
  assert(!service.TouchControlsActive(TouchControlsMode::kAuto));
  assert(!service.TouchControlsVisible(TouchControlsMode::kAuto));
  assert(service.TouchControlsActive(TouchControlsMode::kOn));
  service.SubmitPointer(11,1,AbsolutePointerPhase::kDown,150,900,1,3);
  service.SubmitPointer(12,1,AbsolutePointerPhase::kDown,900,900,1,4);
  auto events=Drain(service);assert(events.size()==2);
  assert(events[0].pointer_id!=events[1].pointer_id);
  assert(events[0].logical_x==50&&events[0].logical_y==300&&events[0].x<0);
  TouchPresentationState state;assert(service.GetPresentationState(&state));
  assert(state.logical_safe_x==30&&state.logical_safe_width==740);
  assert(!service.TouchControlsVisible(TouchControlsMode::kAuto));
  assert(service.TouchControlsVisible(TouchControlsMode::kOn));
  service.NotifyPhysicalInput(5);
  assert(service.TouchControlsVisible(TouchControlsMode::kOn));
  service.SubmitPointer(11,1,AbsolutePointerPhase::kUp,-100,2000,0,6);
  service.SubmitPointer(12,1,AbsolutePointerPhase::kUp,900,900,0,7);
  auto ends=Drain(service);assert(ends.size()==2);
  assert(ends[0].pointer_id==events[0].pointer_id&&ends[1].pointer_id==events[1].pointer_id);
  assert(!service.TouchControlsVisible(TouchControlsMode::kAuto));
  std::cout<<"PASS On supports attached devices and stable pointer identity; Auto stays hidden while devices exist\n";

  AbsolutePointerService devices;
  devices.UpdatePresentation(transform,90,0,2220,1080,1);devices.SetFocused(true,2);
  assert(devices.TouchControlsActive(TouchControlsMode::kAuto));
  assert(devices.TouchControlsVisible(TouchControlsMode::kAuto));
  devices.SubmitPointer(3,4,AbsolutePointerPhase::kDown,400,400,1,3);Drain(devices);
  devices.AddPhysicalMouse(44,4);
  auto plugged=Drain(devices);
  assert(plugged.size()==1&&plugged[0].phase==AbsolutePointerPhase::kCancel);
  assert(devices.HasPhysicalMouse()&&!devices.TouchControlsActive(TouchControlsMode::kAuto));
  devices.AddPhysicalKeyboard(45,5);devices.AddGameController(46,6);
  assert(devices.TouchControlsActive(TouchControlsMode::kOn));
  devices.RemovePhysicalMouse(44,7);assert(!devices.HasPhysicalMouse());
  assert(!devices.TouchControlsVisible(TouchControlsMode::kAuto));
  devices.RemovePhysicalKeyboard(45,8);assert(!devices.TouchControlsVisible(TouchControlsMode::kAuto));
  devices.RemoveGameController(46,9);assert(devices.TouchControlsVisible(TouchControlsMode::kAuto));
  devices.ReplacePhysicalMice({0,7,8},10);devices.RemovePhysicalMouse(7,11);
  assert(!devices.TouchControlsActive(TouchControlsMode::kAuto));
  devices.ReplacePhysicalMice({},12);assert(devices.TouchControlsActive(TouchControlsMode::kAuto));
  assert(!devices.TouchControlsVisible(TouchControlsMode::kOff));
  std::cout<<"PASS Auto tracks keyboard, mouse and controller presence; hotplug cancels owners and last removal restores controls\n";

  AbsolutePointerService android_devices;
  android_devices.UpdatePresentation(transform,90,0,2220,1080,1);
  android_devices.SetFocused(true,2);
  android_devices.SetAndroidPhysicalKeyboardPresence(true,true,3);
  android_devices.SetAndroidPhysicalMousePresence(true,true,3);
  android_devices.SubmitPointer(23,7,AbsolutePointerPhase::kDown,400,400,1,4);
  assert(Drain(android_devices).size()==1);
  android_devices.SetAndroidPhysicalKeyboardPresence(false,false,5);
  android_devices.SetAndroidPhysicalMousePresence(false,false,5);
  assert(Drain(android_devices).empty());
  // Re-publishing the same successful snapshot must also retain this owner;
  // otherwise the failed query silently invalidated the remembered presence.
  android_devices.SetAndroidPhysicalKeyboardPresence(true,true,6);
  android_devices.SetAndroidPhysicalMousePresence(true,true,6);
  assert(Drain(android_devices).empty());
#if REX_PLATFORM_ANDROID
  assert(android_devices.HasPhysicalKeyboard()&&android_devices.HasPhysicalMouse());
  assert(!android_devices.TouchControlsActive(TouchControlsMode::kAuto));
#endif
  android_devices.SetAndroidPhysicalKeyboardPresence(false,true,7);
  android_devices.SetAndroidPhysicalMousePresence(false,true,7);
  auto detached=Drain(android_devices);
  assert(detached.size()==1&&detached[0].phase==AbsolutePointerPhase::kCancel);
  android_devices.SubmitPointer(23,8,AbsolutePointerPhase::kDown,400,400,1,8);
  assert(Drain(android_devices).size()==1);
  android_devices.SetAndroidPhysicalKeyboardPresence(true,false,9);
  android_devices.SetAndroidPhysicalMousePresence(true,false,9);
  assert(Drain(android_devices).empty());
  assert(!android_devices.HasPhysicalKeyboard()&&!android_devices.HasPhysicalMouse());
  assert(android_devices.TouchControlsActive(TouchControlsMode::kAuto));
  std::cout<<"PASS failed Android device queries retain successful presence and finger ownership\n";

  service.SubmitPointer(11,1,AbsolutePointerPhase::kDown,150,900,1,8);
  auto down=Drain(service);assert(down.size()==1);
  service.SetLogicalSize(360,800,9);
  auto canceled=Drain(service);assert(canceled.size()==1);
  assert(canceled[0].phase==AbsolutePointerPhase::kCancel&&canceled[0].generation==down[0].generation);
  assert(canceled[0].logical_x==50&&canceled[0].logical_y==300);
  assert(service.GetPresentationState(&state));assert(state.generation>canceled[0].generation);
  service.SetFocused(false,10);assert(!service.TouchControlsActive(TouchControlsMode::kOn));
  std::cout<<"PASS logical resize cancels with original coordinates before advancing presentation generation\n";

  auto& global=GetAbsolutePointerService();global.SetLogicalSize(800,360);
  global.UpdatePresentation(transform,90,0,2220,1080,1);global.SetFocused(true,2);
  global.SubmitPointer(22,5,AbsolutePointerPhase::kDown,400,400,1,3);Drain(global);
  SetTouchInputActiveCallback([]{return false;});
  assert(!TouchPointerInputActive());
  assert(!TouchControlsActive());auto modal=Drain(global);
  assert(modal.size()==1&&modal[0].phase==AbsolutePointerPhase::kCancel);
  assert(!TouchControlsActive()&&Drain(global).empty());
  global.SubmitPointer(22,6,AbsolutePointerPhase::kDown,400,400,1,4);
  auto modal_down=Drain(global);assert(modal_down.size()==1);
  SetTouchInputActiveCallback([]{return true;});assert(TouchControlsActive());
  assert(TouchPointerInputActive());
  auto resumed=Drain(global);
  assert(resumed.size()==1&&resumed[0].phase==AbsolutePointerPhase::kCancel);
  assert(global.GetPresentationState(&state));
  assert(state.generation>modal_down[0].generation);
  SetTouchGamepadProvider([](uint32_t user,X_INPUT_GAMEPAD* pad) noexcept {
    if(user!=0)return false;*pad={};pad->buttons=X_INPUT_GAMEPAD_A;return true;
  });
  X_INPUT_GAMEPAD pad{};assert(ReadTouchGamepad(0,&pad)&&pad.buttons==X_INPUT_GAMEPAD_A);
  assert(!ReadTouchGamepad(1,&pad));SetTouchGamepadProvider(nullptr);
  SetTouchInputActiveCallback({});
  std::cout<<"PASS shared host-modal gate cancels touches and native provider respects user admission\n";
}
