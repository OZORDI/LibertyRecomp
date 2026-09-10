#include <cassert>
#include <iostream>
#include <sys/mman.h>
struct PPCContext {};
#include "input/context_touch_controls.cpp"
review::Setting<bool> gta4_touch_trace{false};

void GTA4_RegisterTouchExtension(GTA4TouchExtension) noexcept {}
bool GTA4_TouchTitleInputOwned() noexcept { return false; }
namespace rex::input {
bool TouchControlsActive() noexcept { return true; }
bool GetTouchPresentationState(TouchPresentationState* p) noexcept {
  *p={}; p->generation=1; p->valid=p->focused=true;
  p->output_width=1280; p->output_height=720;
  p->safe_area_width=1280; p->safe_area_height=720;
  return true;
}
}

int main() {
  using namespace gta4::input;
  constexpr size_t bytes=uint64_t{1} << 32;
  auto* b=static_cast<uint8_t*>(mmap(nullptr,bytes,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON,-1,0));
  assert(b!=MAP_FAILED);
  StoreU32(b,kCurrentPlayerIndexAddress,kMaximumLocalPlayers);
  PPCContext c{};
  BeginPoll(c,b,41,false,false);
  const auto jump=g_runtime.layout.controls[2];
  assert(jump.key==rex::ui::VirtualKey::kSpace);
  rex::input::AbsolutePointerEvent e{};
  e.generation=1; e.pointer_id=1; e.phase=rex::input::AbsolutePointerPhase::kDown;
  e.x=jump.center_x; e.y=jump.center_y;
  assert(OnPointerEvent(e,c,b,41));
  e.phase=rex::input::AbsolutePointerPhase::kUp;
  assert(OnPointerEvent(e,c,b,41));
  std::array<uint8_t,256> down{},pressed{};
  CollectVirtualKeys(41,down,pressed);
  const auto key=static_cast<uint16_t>(rex::ui::VirtualKey::kSpace);
  assert(down[key]&&pressed[key]);
  std::cout<<"PASS complete sub-poll tap reaches held action consumer\n";
  BeginPoll(c,b,42,false,false); down={}; pressed={}; CollectVirtualKeys(42,down,pressed);
  assert(!down[key]&&!pressed[key]);
  std::cout<<"PASS no press repeated in the next epoch\n";
  BeginPoll(c,b,43,false,false); e.phase=rex::input::AbsolutePointerPhase::kDown;
  assert(OnPointerEvent(e,c,b,43));
  StoreU8(b,kPhoneWithMovementFlagAddress,1);
  BeginPoll(c,b,44,false,false); down={}; pressed={}; CollectVirtualKeys(44,down,pressed);
  assert(!down[key]&&!pressed[key]&&g_runtime.layout.mode==ContextTouchMode::kPhone);
  std::cout<<"PASS context cancellation precedes frozen key consumption\n";
  StoreU8(b,kPhoneWithMovementFlagAddress,0);
  StoreU32(b,kMinigameActiveAddress,1);
  ObserveTouchScriptQuery(TouchScriptQueryKind::kControlPressed,17,50);
  ObserveTouchScriptQuery(TouchScriptQueryKind::kControlHeld,17,50);
  ObserveTouchScriptQuery(TouchScriptQueryKind::kControlAnalog,17,50);
  BeginPoll(c,b,50,false,false);
  assert(g_runtime.layout.control_count==1);
  const auto q=g_runtime.layout.controls[0];
  e.pointer_id=2; e.x=q.center_x; e.y=q.center_y;
  assert(OnPointerEvent(e,c,b,50));
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlPressed,17,50)==1);
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlHeld,17,50)==1);
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlAnalog,17,50)==255);
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kRawButton,17,50)==0);
  const auto generation=g_runtime.layout_generation;
  ObserveTouchScriptQuery(TouchScriptQueryKind::kControlAnalog,17,51);
  BeginPoll(c,b,51,false,false);
  assert(g_runtime.layout_generation==generation);
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlPressed,17,51)==0);
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlHeld,17,51)==1);
  e.phase=rex::input::AbsolutePointerPhase::kUp;assert(OnPointerEvent(e,c,b,51));
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlHeld,17,51)==0);
  std::cout<<"PASS canonical minigame action retains held, edge, analog and namespace semantics\n";
  BeginPoll(c,b,52,false,false); e.phase=rex::input::AbsolutePointerPhase::kDown;
  assert(OnPointerEvent(e,c,b,52));
  e.phase=rex::input::AbsolutePointerPhase::kCancel;assert(OnPointerEvent(e,c,b,52));
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlHeld,17,52)==0);
  assert(GetTouchScriptQueryValue(TouchScriptQueryKind::kControlPressed,17,52)==0);
  std::cout<<"PASS cancelled script touch has no residual press\n";
  OnControlsDisabled(c,b,53);
  down={};pressed={};CollectVirtualKeys(53,down,pressed);
  assert(g_runtime.pointers.empty()&&g_runtime.script_refcounts.empty());
  std::cout<<"PASS controls disabled releases every owner\n";
  StoreU32(b,kMinigameActiveAddress,1);
  ObserveTouchScriptQuery(TouchScriptQueryKind::kControlHeld,1000,1000);
  BeginPoll(c,b,1000,false,false);
  assert(g_runtime.layout.control_count==1);
  const auto held_button=g_runtime.layout.controls[0];
  e.pointer_id=22;e.x=held_button.center_x;e.y=held_button.center_y;
  e.phase=rex::input::AbsolutePointerPhase::kDown;
  assert(OnPointerEvent(e,c,b,1000));
  for(uint32_t action=0;action<32;++action)
    ObserveTouchScriptQuery(TouchScriptQueryKind::kControlHeld,action,1001);
  BeginPoll(c,b,1001,false,false);
  const auto still_held=GetTouchScriptQueryValue(TouchScriptQueryKind::kControlHeld,1000,1001);
  std::cerr<<"SATURATED_LAYOUT held="<<still_held<<" controls="<<g_runtime.layout.control_count<<'\n';
  assert(still_held==1);
  std::cout<<"PASS held minigame action survives visible-control capacity saturation\n";
  OnControlsDisabled(c,b,1500);
  ObserveTouchScriptQuery(TouchScriptQueryKind::kAnalogueSticks,0,2000);
  BeginPoll(c,b,2000,false,false);
  assert(g_runtime.layout.control_count==1);
  const auto stick=g_runtime.layout.controls[0];
  assert(stick.kind==ContextTouchControlKind::kMovementStick);
  e.pointer_id=23;e.x=stick.center_x+stick.radius*0.5f;e.y=stick.center_y;
  e.phase=rex::input::AbsolutePointerPhase::kDown;
  assert(OnPointerEvent(e,c,b,2000));
  BeginPoll(c,b,2020,false,false);
  int32_t horizontal=0,vertical=0;
  assert(GetTouchScriptAnalogueSticks(2020,&horizontal,&vertical));
  assert(horizontal>0&&vertical==0);
  e.phase=rex::input::AbsolutePointerPhase::kUp;
  assert(OnPointerEvent(e,c,b,2020));
  BeginPoll(c,b,2021,false,false);
  assert(g_runtime.layout.control_count==0);
  std::cout<<"PASS owned minigame stick survives query gaps and expires after release\n";
  munmap(b,bytes);
}
