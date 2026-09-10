#include <cassert>
#include <deque>
#include <iostream>
#include <sys/mman.h>
#include "gta4_init.h"
#include "input/context_touch_controls.cpp"
#include "gta4_keyboard_controller.h"
namespace review {
std::deque<rex::input::AbsolutePointerEvent> events;
rex::input::TouchPresentationState presentation;
bool enabled=true,frontend=false;
}
namespace rex::input {
bool TryDequeueAbsolutePointerEvent(AbsolutePointerEvent* e) noexcept {
 if(review::events.empty())return false;
 *e=review::events.front();review::events.pop_front();return true;
}
bool TouchControlsActive() noexcept {return review::enabled;}
bool GetTouchPresentationState(TouchPresentationState* p) noexcept {*p=review::presentation;return p->valid;}
}
void __imp__sub_8224EEF8(PPCContext& c,uint8_t*){c.r3.u32=review::frontend;}
void __imp__sub_8224EE98(PPCContext&,uint8_t*){}
void __imp__sub_821BF050(PPCContext&,uint8_t*){}
void __imp__sub_8233ABF0(PPCContext&,uint8_t*){}
void word(uint8_t* b,uint32_t a,uint32_t v){*reinterpret_cast<uint32_t*>(b+a)=__builtin_bswap32(v);}
int main(){
 using namespace gta4::input;
 using namespace rex::input;
 constexpr size_t memory_size=4294967296ULL;
 auto* b=static_cast<uint8_t*>(mmap(nullptr,memory_size,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON,-1,0));
 assert(b!=MAP_FAILED);
 auto& p=review::presentation;p.generation=1;p.valid=p.focused=true;
 p.output_width=p.physical_output_width=p.physical_surface_width=1280;
 p.output_height=p.physical_output_height=p.physical_surface_height=720;
 p.safe_area_width=1280;p.safe_area_height=720;
 PPCContext ctx;constexpr uint32_t control=65536;
 mnk::SetNativeControllerCompatibilityBindings(KeyboardControllerBindings());
 InitializeContextTouchControls();
 const auto poll=[&](uint64_t epoch){GTA4_TouchConsumePoll(ctx,b,epoch);};
 const auto find=[&](rex::ui::VirtualKey key){
  const auto layout=GetContextTouchOverlaySnapshot().layout;
  for(size_t i=0;i<layout.control_count;++i)if(layout.controls[i].key==key)return layout.controls[i];
  assert(false);return ContextTouchControl{};
 };
 const auto send=[&](const ContextTouchControl& c,AbsolutePointerPhase phase,uint64_t id){
  AbsolutePointerEvent e{};e.generation=p.generation;e.pointer_id=id;e.phase=phase;
  e.x=c.center_x;e.y=c.center_y;e.output_width=1280;e.output_height=720;review::events.push_back(e);
 };
 const auto down=[](rex::ui::VirtualKey key){return GTA4_TouchVirtualKeyDown(static_cast<uint16_t>(key));};
 poll(100);auto jump=find(rex::ui::VirtualKey::kSpace);
 send(jump,AbsolutePointerPhase::kDown,1);send(jump,AbsolutePointerPhase::kUp,1);poll(101);
 assert(down(jump.key)&&GTA4_TouchVirtualKeyPressed(101,static_cast<uint16_t>(jump.key)));
 GTA4_TouchObserveControlReplay(ctx,b,control,0,101);poll(102);
 assert(!down(jump.key)&&!GTA4_TouchVirtualKeyPressed(102,static_cast<uint16_t>(jump.key)));
 std::cout<<"PASS completed tap lasts one poll, no duplicated edge\n";
 auto fire=find(rex::ui::VirtualKey::kLButton);send(fire,AbsolutePointerPhase::kDown,2);poll(200);
 assert(down(fire.key));word(b,0x82BA1D40,1);poll(201);
 assert(!down(fire.key));GTA4_TouchObserveControlReplay(ctx,b,control,0,201);assert(!down(fire.key));
 std::cout<<"PASS context refresh cancels old action before freeze and replay\n";
 word(b,0x82BA1D40,0);poll(300);auto phone=find(rex::ui::VirtualKey::kUp);
 send(phone,AbsolutePointerPhase::kDown,3);send(phone,AbsolutePointerPhase::kUp,3);poll(301);
 X_INPUT_GAMEPAD pad{};assert(mnk::ReadVirtualControllerCompatibilityGamepad(0,pad));
 assert(static_cast<uint16_t>(pad.buttons)&X_INPUT_GAMEPAD_DPAD_UP);
 assert(!mnk::ReadVirtualControllerCompatibilityGamepad(1,pad));
 poll(302);assert(mnk::ReadVirtualControllerCompatibilityGamepad(0,pad));assert(!pad.buttons);
 std::cout<<"PASS touch phone-open reaches retail D-pad, primary user only, releases next poll\n";
 b[0x831D4DD5]=1;poll(400);
 const std::array<rex::ui::VirtualKey,6> keys={rex::ui::VirtualKey::kUp,rex::ui::VirtualKey::kDown,rex::ui::VirtualKey::kLeft,rex::ui::VirtualKey::kRight,rex::ui::VirtualKey::kReturn,rex::ui::VirtualKey::kBack};
 const std::array<uint16_t,6> buttons={X_INPUT_GAMEPAD_DPAD_UP,X_INPUT_GAMEPAD_DPAD_DOWN,X_INPUT_GAMEPAD_DPAD_LEFT,X_INPUT_GAMEPAD_DPAD_RIGHT,X_INPUT_GAMEPAD_A,X_INPUT_GAMEPAD_B};
 uint64_t epoch=401;
 for(size_t i=0;i<keys.size();++i){auto c=find(keys[i]);send(c,AbsolutePointerPhase::kDown,4);send(c,AbsolutePointerPhase::kUp,4);poll(epoch++);assert(mnk::ReadVirtualControllerCompatibilityGamepad(0,pad));assert(static_cast<uint16_t>(pad.buttons)==buttons[i]);poll(epoch++);}
 std::cout<<"PASS all phone directions, select and back reach matching controller buttons\n";
 b[0x831D4DD5]=0;review::frontend=true;word(b,0x82BFA124,3);poll(500);
 auto back=find(rex::ui::VirtualKey::kBack);send(back,AbsolutePointerPhase::kDown,5);send(back,AbsolutePointerPhase::kUp,5);poll(501);
 assert(mnk::ReadVirtualControllerCompatibilityGamepad(0,pad));assert(static_cast<uint16_t>(pad.buttons)==X_INPUT_GAMEPAD_B);
 std::cout<<"PASS visible map Back control owns its gesture and produces B\n";
 GTA4_SetTouchTitleInputOwned(true);assert(!mnk::ReadVirtualControllerCompatibilityGamepad(0,pad));assert(!down(back.key));
 poll(502);GTA4_SetTouchTitleInputOwned(false);review::frontend=false;
 std::cout<<"PASS host UI ownership clears both virtual sources\n";
 word(b,0x82BA1D40,1);
 ObserveTouchScriptQuery(TouchScriptQueryKind::kControlPressed,17,600);
 ObserveTouchScriptQuery(TouchScriptQueryKind::kControlHeld,17,600);
 ObserveTouchScriptQuery(TouchScriptQueryKind::kControlAnalog,17,600);poll(600);
 auto layout=GetContextTouchOverlaySnapshot().layout;assert(layout.control_count==1);auto action=layout.controls[0];
 send(action,AbsolutePointerPhase::kDown,6);poll(601);
 assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlPressed,17,601)==1);
 assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlHeld,17,601)==1);
 assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlAnalog,17,601)==255);
 ObserveTouchScriptQuery(TouchScriptQueryKind::kControlPressed,5,602);poll(602);
 assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlPressed,17,602)==0);
 assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlHeld,17,602)==1);
 layout=GetContextTouchOverlaySnapshot().layout;
 bool same=false;for(size_t i=0;i<layout.control_count;++i)if(layout.controls[i].script.action==17)same=layout.controls[i].center_x==action.center_x&&layout.controls[i].center_y==action.center_y;
 assert(same);
 poll(620);assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlHeld,17,620)==1);
 send(action,AbsolutePointerPhase::kUp,6);poll(621);assert(!GetTouchScriptQueryValue(TouchScriptQueryKind::kControlHeld,17,621));
 std::cout<<"PASS coherent minigame pressed/held/analog and owner retained across learned actions\n";
 word(b,0x82BA1D40,0);poll(700);jump=find(rex::ui::VirtualKey::kSpace);
 send(jump,AbsolutePointerPhase::kDown,7);send(jump,AbsolutePointerPhase::kCancel,7);poll(701);assert(!down(jump.key));
 send(jump,AbsolutePointerPhase::kDown,8);poll(702);assert(down(jump.key));review::enabled=false;poll(703);assert(!down(jump.key));assert(!mnk::ReadVirtualControllerCompatibilityGamepad(0,pad));
 std::cout<<"PASS cancellation and touch-disable drop held and completed taps\n";
 ShutdownContextTouchControls();munmap(b,memory_size);
}