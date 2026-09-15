#include "input/context_touch_layout.h"
#include "input/context_touch_settings.h"
#include "gta4_aspect_policy.h"
#include "touch_samples.h"
#include <rex/input/input.h>

#include <array>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>

using namespace gta4::input;

ContextTouchViewport View(float width, float height) {
  ContextTouchViewport v;
  v.output_width = v.logical_width = v.safe_width = width;
  v.output_height = v.logical_height = v.safe_height = height;
  v.physical_output_width = v.physical_surface_width = width;
  v.physical_output_height = v.physical_surface_height = height;
  v.valid = v.focused = v.host_space = true;
  v.generation = 1;
  return v;
}

int main(int argc, char** argv) {
  assert(argc == 2);
  const std::filesystem::path settings = std::filesystem::path(argv[1]) / "touch-controls.ini";
  constexpr std::array sizes = {std::array{800.0f,360.0f}, std::array{1280.0f,720.0f},
      std::array{1024.0f,768.0f}, std::array{360.0f,800.0f}, std::array{480.0f,320.0f},
      std::array{360.0f,320.0f}};
  for (const auto size : sizes) {
    for (const float scale : {0.65f,1.0f,1.8f}) {
      for (const int overlay : {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}) {
       for (const bool left_handed : {false,true}) {
        for (auto mode : {ContextTouchMode::kOnFoot, ContextTouchMode::kVehicleAutomobile,
                          ContextTouchMode::kVehicleBike, ContextTouchMode::kVehicleBoat,
                          ContextTouchMode::kVehicleHelicopter, ContextTouchMode::kVehiclePassenger,
                          ContextTouchMode::kVehicleDriverUnknown, ContextTouchMode::kPhone,
                          ContextTouchMode::kParachuteFreefall, ContextTouchMode::kParachuteDeployed,
                          ContextTouchMode::kMinigame}) {
          ContextTouchLayoutOptions options{.contextual=true,.armed=overlay!=1 && overlay!=11,
              .free_aim_available=true,.can_enter_vehicle=overlay==1};
          options.phone_visible=overlay==2 || overlay==7 || overlay==12;
          options.editing=overlay==3 || overlay==8 || overlay==13 || overlay==15;
          options.melee=overlay==9 || overlay==10;
          options.scoped_zoom=overlay==4 || overlay==14 || overlay==15;
          options.left_handed=left_handed;
          options.button_scale=scale;
          if (!options.editing) {
            // Synthetic normalized HUD fixture; production reserves the exact
            // observed sprite rectangle instead of a hardcoded screen corner.
            options.weapon_hud_bounds = MapContextTouchHudBounds(
                {0.75f,0.125f,0.875f,0.25f},View(size[0],size[1]));
            assert(options.weapon_hud_bounds);
          }
          std::array scripts = {
              TouchScriptControl{TouchScriptQueryKind::kRawButton,16,0,1,1},
              TouchScriptControl{TouchScriptQueryKind::kControlHeld,40,0,1,1},
              TouchScriptControl{TouchScriptQueryKind::kControlHeld,24,0,1,1},
              TouchScriptControl{TouchScriptQueryKind::kRawButton,17,0,1,1}};
          const bool activity_sticks=overlay==6 || overlay==10 || overlay==11 ||
              overlay==12 || overlay==13 || overlay==14;
          if(activity_sticks) scripts[0].kind=TouchScriptQueryKind::kAnalogueSticks;
          const auto layout=BuildContextTouchLayout(mode,View(size[0],size[1]),
              overlay>=5 ? std::span<const TouchScriptControl>(scripts) : std::span<const TouchScriptControl>{},options);
          assert(layout.control_count <= layout.controls.size());
          const auto require_action = [&](TouchAction action) {
            for (size_t i = 0; i < layout.control_count; ++i) {
              if (layout.controls[i].visible && layout.controls[i].action == action) return;
            }
            std::cerr << "Missing action " << int(action) << " size=" << size[0] << ',' << size[1]
                      << " scale=" << scale << " overlay=" << overlay << " mode=" << int(mode)
                      << " mirrored=" << left_handed << '\n';
            std::abort();
          };
          if (mode == ContextTouchMode::kOnFoot) {
            require_action(TouchAction::kFire);
            if (!options.phone_visible) {
              for (const auto action : {TouchAction::kRunSprint, TouchAction::kJumpClimb,
                   TouchAction::kAim, TouchAction::kWeaponWheel, TouchAction::kCameraCycle,
                   TouchAction::kLookBehind, TouchAction::kPhone}) require_action(action);
              if (options.can_enter_vehicle || options.editing) require_action(TouchAction::kContext);
              if (!options.melee) {
                require_action(TouchAction::kCover);
                require_action(TouchAction::kCrouch);
                if (options.armed) {
                  require_action(TouchAction::kReload);
                  require_action(TouchAction::kFreeAim);
                }
              }
              if (options.scoped_zoom) {
                require_action(TouchAction::kZoomIn);
                require_action(TouchAction::kZoomOut);
              }
            }
          } else if (mode >= ContextTouchMode::kVehicleAutomobile &&
                     mode <= ContextTouchMode::kVehiclePassenger) {
            require_action(TouchAction::kContext);
            if (!options.phone_visible) {
              for (const auto action : {TouchAction::kVehicleFire, TouchAction::kCameraCycle,
                   TouchAction::kLookBehind, TouchAction::kPhone}) require_action(action);
              if (mode == ContextTouchMode::kVehicleHelicopter) {
                require_action(TouchAction::kHeliAction);
              } else {
                for (const auto action : {TouchAction::kHeadlights, TouchAction::kRadioPrevious,
                     TouchAction::kRadioNext}) require_action(action);
                if (mode != ContextTouchMode::kVehiclePassenger) require_action(TouchAction::kHorn);
              }
            }
          }
          if (activity_sticks) require_action(TouchAction::kActivityRightStick);
          if (options.phone_visible || mode == ContextTouchMode::kPhone) {
            for (const auto action : {TouchAction::kPhoneUp, TouchAction::kPhoneDown,
                 TouchAction::kPhoneLeft, TouchAction::kPhoneRight, TouchAction::kPhoneAccept,
                 TouchAction::kPhoneBack}) require_action(action);
          }
          if (options.editing) {
            for (const auto action : {TouchAction::kEditDone, TouchAction::kEditReset,
                 TouchAction::kEditSmaller, TouchAction::kEditLarger, TouchAction::kEditOpacity,
                 TouchAction::kEditHandedness, TouchAction::kEditFloating, TouchAction::kEditCameraSpeed,
                 TouchAction::kEditAimSpeed, TouchAction::kEditVehicleSpeed,
                 TouchAction::kEditFlightSpeed, TouchAction::kEditInvertY}) require_action(action);
          }
          for(size_t i=0;i<layout.control_count;++i) {
            const auto& c=layout.controls[i];
            assert(c.action != TouchAction::kMore && c.action != TouchAction::kPause);
            assert(c.action < TouchAction::kNativeA || c.action > TouchAction::kNativeRightStick);
            if(!c.visible || c.kind==ContextTouchControlKind::kLookSurface) continue;
            if (options.weapon_hud_bounds) {
              const auto& bounds = *options.weapon_hud_bounds;
              const double x = std::clamp(double(c.center_x),double(bounds.left),double(bounds.right));
              const double y = std::clamp(double(c.center_y),double(bounds.top),double(bounds.bottom));
              assert(std::hypot(c.center_x-x,c.center_y-y) > c.radius);
            }
            if (c.action == TouchAction::kPhone) {
              // Both shipped HUD profiles, retail horizontal adjustment,
              // modern/legacy hosts and safe insets must leave the contextual phone utility clear.
              const gta4::aspect::Extent extent{uint32_t(size[0]),uint32_t(size[1])};
              for (const auto& fixture : touch_samples::kRadarStyleBounds) {
                const gta4::aspect::Rect authored{fixture[0],fixture[1],fixture[2],fixture[3]};
                for (const bool modern : {false, true}) {
                  for (const double inset : {0.0, 0.05, 0.12}) {
                    const auto transform = gta4::aspect::TopLeftRadarViewport(authored,inset,1.0,
                        modern ? gta4::aspect::RadarLayout(extent) : gta4::aspect::Transform{});
                    const auto normalized = transform.Map(authored);
                    const gta4::aspect::Rect radar{normalized.left*size[0],normalized.top*size[1],
                        normalized.right*size[0],normalized.bottom*size[1]};
                    const double nearest_x = std::clamp(double(c.center_x),radar.left,radar.right);
                    const double nearest_y = std::clamp(double(c.center_y),radar.top,radar.bottom);
                    if (std::hypot(c.center_x-nearest_x,c.center_y-nearest_y) < c.radius) {
                      std::cerr << "Radar overlap " << c.label.data() << " size=" << size[0] << ',' << size[1]
                                << " circle=" << c.center_x << ',' << c.center_y << ',' << c.radius
                                << " radar=" << radar.left << ',' << radar.top << ',' << radar.right << ',' << radar.bottom
                                << " scale=" << scale << " overlay=" << overlay << " mirrored=" << left_handed << '\n';
                      return 1;
                    }
                  }
                }
              }
            }
            if (!(c.center_x-c.radius>=-0.01f && c.center_y-c.radius>=-0.01f &&
                  c.center_x+c.radius<=size[0]+0.01f && c.center_y+c.radius<=size[1]+0.01f)) {
              std::cerr<<"Out of bounds "<<c.label.data()<<" size="<<size[0]<<','<<size[1]
                       <<" scale="<<scale<<" mode="<<int(mode)<<'\n';
              return 1;
            }
            assert(ContextTouchControlContains(c,layout.viewport,c.center_x,c.center_y));
            assert(!ContextTouchControlContains(c,layout.viewport,
                                                c.center_x+c.radius,c.center_y+c.radius));
            for(size_t j=0;j<i;++j) {
              const auto& other=layout.controls[j];
              if(!other.visible || other.kind==ContextTouchControlKind::kLookSurface) continue;
              if(std::hypot(c.center_x-other.center_x,c.center_y-other.center_y)+0.01f < c.radius+other.radius) {
                std::cerr<<"Overlapping "<<c.label.data()<<'/'<<other.label.data()
                         <<" size="<<size[0]<<','<<size[1]<<" scale="<<scale<<" overlay="<<overlay<<'\n';
                return 1;
              }
            }
          }
        }
       }
      }
    }
  }
  std::cout<<"PASS circular controls avoid overlap and stay in bounds across sizes, handedness, modes and overlays\n";

  for (const auto mode : {ContextTouchMode::kFrontend, ContextTouchMode::kMap}) {
    for (const bool contextual : {false, true}) {
      for (const bool editing : {false, true}) {
        const auto menu = BuildContextTouchLayout(mode, View(1280,720), {},
            {.contextual=contextual, .phone_visible=true, .armed=true,
             .can_enter_vehicle=true, .editing=editing});
        assert(menu.control_count == 0);
      }
    }
  }
  std::cout<<"PASS native pause and map have no overlay controls, including editor options\n";

  const auto v=View(800,360);
  ContextTouchLayoutOptions options{.contextual=true};
  const auto unknown=BuildContextTouchLayout(ContextTouchMode::kVehicleDriverUnknown,v,{},options);
  bool exit=false,throttle_available=false,horn=false,radio=false;
  for(size_t i=0;i<unknown.control_count;++i) {
    const auto& c=unknown.controls[i]; if(!c.visible)continue;
    exit|=c.action==TouchAction::kContext && c.pad_buttons==rex::input::X_INPUT_GAMEPAD_Y;
    throttle_available|=c.action==TouchAction::kAccelerate && c.trigger_side==2;
    horn|=c.action==TouchAction::kHorn && c.pad_buttons==rex::input::X_INPUT_GAMEPAD_LEFT_THUMB;
    radio|=c.action==TouchAction::kRadioNext && c.pad_buttons==rex::input::X_INPUT_GAMEPAD_DPAD_RIGHT;
  }
  assert(exit&&throttle_available&&horn&&radio);
  std::cout<<"PASS unknown vehicle retains direct semantic controls without More or a raw controller grid\n";

  for(const bool armed : {false,true}) {
    options={.contextual=true,.armed=armed};
    const auto foot=BuildContextTouchLayout(ContextTouchMode::kOnFoot,v,{},options);
    bool attack=false,enter=false;
    for(size_t i=0;i<foot.control_count;++i) {
      const auto& c=foot.controls[i]; if(!c.visible)continue;
      attack|=c.action==TouchAction::kFire && c.trigger_side==2;
      enter|=c.action==TouchAction::kContext;
    }
    assert(attack&&!enter);
    options.can_enter_vehicle=true;
    const auto nearby=BuildContextTouchLayout(ContextTouchMode::kOnFoot,v,{},options);
    bool enter_nearby=false;
    for(size_t i=0;i<nearby.control_count;++i)
      enter_nearby|=nearby.controls[i].visible && nearby.controls[i].action==TouchAction::kContext;
    assert(enter_nearby);
  }
  std::cout<<"PASS on-foot Attack stays available armed or unarmed and Enter requires native eligibility\n";

  options={.contextual=true,.phone_visible=true};
  const auto phone=BuildContextTouchLayout(ContextTouchMode::kVehicleAutomobile,v,{},options);
  bool throttle=false,phone_accept=false,camera=false;
  for(size_t i=0;i<phone.control_count;++i) {
    const auto& c=phone.controls[i]; if(!c.visible)continue;
    throttle|=c.action==TouchAction::kAccelerate;
    phone_accept|=c.action==TouchAction::kPhoneAccept;
    camera|=c.kind==ContextTouchControlKind::kLookSurface;
  }
  assert(throttle&&phone_accept&&camera);
  auto relabeled=phone;relabeled.controls[0].label[0]='!';relabeled.controls[0].icon_id[0]='x';
  assert(ContextTouchLayoutEquivalent(phone,relabeled));
  std::cout<<"PASS phone overlays driving; text and icon updates preserve action geometry\n";

  ContextTouchControl circle;circle.radius=50;circle.visible=true;
  auto anisotropic=View(100,100);anisotropic.host_space=false;
  anisotropic.physical_output_width=150;anisotropic.physical_output_height=100;
  assert(!ContextTouchControlContains(circle,anisotropic,40,0));
  assert(ContextTouchControlContains(circle,anisotropic,30,0));
  std::cout<<"PASS unequal output scaling uses the rendered circle for hit testing\n";

  ConfigureContextTouchSettings(settings);
  auto preferences=GetContextTouchPreferences();
  preferences.left_handed=true;preferences.button_scale=std::numeric_limits<float>::quiet_NaN();
  preferences.opacity=100;SetContextTouchPreferences(preferences);
  assert(GetContextTouchPreferences().button_scale==1.0f);
  assert(GetContextTouchPreferences().opacity==1.0f);
  assert(FlushContextTouchSettings());
  ConfigureContextTouchSettings(settings.parent_path()/"other.ini");
  ConfigureContextTouchSettings(settings);
  assert(GetContextTouchPreferences().left_handed);
  options={.contextual=true};
  auto custom=BuildContextTouchLayout(ContextTouchMode::kOnFoot,View(1280,720),{},options);
  assert(SetContextTouchPlacement(custom,TouchAction::kJumpClimb,640,450,40));
  ApplyContextTouchSavedLayout(custom);
  bool placed=false;
  for(size_t i=0;i<custom.control_count;++i)if(custom.controls[i].action==TouchAction::kJumpClimb)
    placed=custom.controls[i].center_x==640&&custom.controls[i].center_y==450;
  assert(placed);
  assert(!SetContextTouchPlacement(custom,TouchAction::kJumpClimb,-10,450,40));
  RestoreContextTouchLayout(custom);assert(FlushContextTouchSettings());
  SetContextTouchEditorOpen(true);HandleContextTouchEditorAction(TouchAction::kEditDone,custom);
  assert(!ContextTouchEditorOpen());
  std::cout<<"PASS preferences persist, invalid values are bounded, placements validate and reset\n";
}
