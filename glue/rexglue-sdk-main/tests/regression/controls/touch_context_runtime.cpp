#include <cassert>
#include <bit>
#include <cstdint>
#include <iostream>
#include <limits>
#include <string_view>
#include <unordered_map>

#include "input/context_touch_context.h"

namespace {
struct Memory {
  std::unordered_map<uint32_t, uint8_t> bytes;
  size_t reads = 0;

  void U8(uint32_t address, uint8_t value) { bytes[address] = value; }
  void U32(uint32_t address, uint32_t value) {
    for (uint32_t index = 0; index < 4; ++index)
      bytes[address + index] = static_cast<uint8_t>(value >> ((3 - index) * 8));
  }
  void Float(uint32_t address, float value) { U32(address, std::bit_cast<uint32_t>(value)); }
  static bool Read(void* opaque, uint32_t address, void* output, size_t size) {
    auto& memory = *static_cast<Memory*>(opaque);
    ++memory.reads;
    for (size_t index = 0; index < size; ++index) {
      const auto found = memory.bytes.find(address + static_cast<uint32_t>(index));
      if (found == memory.bytes.end()) return false;
      static_cast<uint8_t*>(output)[index] = found->second;
    }
    return true;
  }
  gta4::input::TouchContextSnapshot Facts(bool frontend = false, bool map = false,
                                        gta4::input::TouchContextQueries queries = {}) {
    return gta4::input::ReadTouchContextFacts({this, Read}, frontend, map, queries);
  }
};

constexpr uint32_t kInfo = 0x10000000;
constexpr uint32_t kPed = 0x10010000;
constexpr uint32_t kIntelligence = 0x10020000;
constexpr uint32_t kWeapon = 0x10030000;
constexpr uint32_t kVehicle = 0x10040000;
constexpr uint32_t kScopeWidget = 0x10050000;
constexpr uint32_t kTask = 0x10060000;
constexpr uint32_t kTaskVtable = 0x10070000;
constexpr uint32_t kTransform = 0x10090000;

struct VehicleQuery {
  uint32_t result = kVehicle;
  uint32_t ped = 0;
  uint32_t forward = 0;
  size_t calls = 0;
  static uint32_t Run(void* opaque, uint32_t ped, uint32_t forward) {
    auto& query = *static_cast<VehicleQuery*>(opaque);
    query.ped = ped;
    query.forward = forward;
    ++query.calls;
    return query.result;
  }
  gta4::input::TouchContextQueries Queries() { return {this, Run}; }
};

void PrepareVehicleEntry(Memory& memory) {
  memory.U32(kPed + 544, kInfo);
  memory.U32(kPed + 568, 0);
  memory.U32(kPed + 32, kTransform);
  for (const uint32_t offset : {16u, 20u, 24u, 28u, 48u, 52u, 56u, 60u})
    memory.Float(kTransform + offset, offset == 20 ? 1.0f : 0.0f);
  memory.U32(kVehicle + 40, 0x00800000);
  memory.U32(kVehicle + 4836, 0);
}

Memory HealthyPlayer() {
  Memory memory;
  memory.U32(0x82A98778, 0);
  memory.U32(0x82C01C70, kInfo);
  memory.U32(0x82C01C30, 4);
  memory.U32(0x82B2A2F0 + 3412, 0);
  memory.U32(0x82B3950C, 0);
  memory.Float(0x82000D68, 0.0f);
  memory.U32(0x82B977F0, 0);
  memory.U32(0x82B977FC, 0);
  memory.U8(0x831D5335, 0);
  memory.U32(0x82BA1D40, 0);
  memory.U8(0x831D4DD4, 0);
  memory.U8(0x831D534C, 0);
  memory.U8(0x82AA1B0F, 200);
  memory.U8(0x82FD1E3C, 1);
  memory.U32(kInfo + 1400, kPed);
  memory.U32(kInfo + 1232, 2);
  memory.U32(kInfo + 1200, 0);
  memory.Float(kPed + 484, 100.0f);
  memory.U32(kPed + 2496, 0);
  memory.U32(kPed + 572, 0);
  memory.U32(kPed + 540, kIntelligence);
  memory.U32(kIntelligence + 736, 0);
  for (uint32_t index = 0; index < 14; ++index) memory.U32(kIntelligence + 68 + index * 4, 0);
  memory.U8(0x82B2A2F0 + 2400, 255);
  memory.U8(0x82B2A2F0 + 2402, 255);
  memory.U32(kPed + 640, 0);
  memory.U32(kPed + 640 + 20, 0);
  memory.U32(kPed + 640 + 32, kWeapon);
  memory.U32(kWeapon + 20, 0);
  memory.U32(kWeapon + 28, 0);
  for (uint32_t index = 0; index < 11; ++index) {
    memory.U32(kPed + 640 + 36 + index * 8, 0);
    memory.U32(kPed + 640 + 40 + index * 8, 0);
  }
  memory.U32(0x82CB8AB0 + 12, 0);
  return memory;
}
}

int main() {
  using namespace gta4::input;
  auto memory = HealthyPlayer();
  auto context = memory.Facts();
  assert(context.valid && context.playing && context.gameplay_allowed);
  assert(context.native_input_allowed && context.input_user_known);
  assert(!context.loading && !context.cutscene);
  assert(context.base_mode == ContextTouchMode::kOnFoot);
  assert(context.phone_visibility_known && !context.phone_visible);
  assert(context.aim_settings_known && context.alternate_aim_setting && context.aim_threshold == 200);
  assert(context.weapon_known && !context.armed && context.inventory_known);
  assert(context.weapons[0].selectable);
  assert(context.cover_known && !context.in_cover && context.melee_known && !context.melee);
  assert(!context.can_enter_vehicle);

  memory.U32(kInfo + 1200, 1);
  context = memory.Facts();
  assert(!context.gameplay_allowed && context.native_input_allowed);
  memory.U32(0x82B977F0, 1);
  context = memory.Facts();
  assert(!context.gameplay_allowed && !context.native_input_allowed);
  assert(memory.Facts(true).native_input_allowed);
  std::cout << "PASS independent native/gameplay admission and exact-width aim settings\n";

  memory = HealthyPlayer();
  // The loading flags are adjacent bytes. Ready/done alone are not loading,
  // and a readable active byte does not require its neighbors to be mapped.
  assert(!memory.bytes.contains(0x831D5336) && !memory.bytes.contains(0x831D5337));
  assert(memory.Facts().gameplay_allowed);
  memory.U8(0x831D5336, 1);
  memory.U8(0x831D5337, 1);
  assert(!memory.Facts().loading && memory.Facts().gameplay_allowed);
  for (const uint8_t active : {uint8_t{1}, uint8_t{0x80}, uint8_t{0xFF}}) {
    memory.U8(0x831D5335, active);
    context = memory.Facts();
    assert(context.loading && context.valid && context.playing && !context.cutscene);
    assert(!context.gameplay_allowed && !context.native_input_allowed);
  }
  memory.U32(0x82BA1D40, 1);
  memory.U32(kInfo + 1200, 1);
  context = memory.Facts();
  assert(context.minigame_active && !context.native_input_allowed);
  memory.U8(0x831D5335, 0);
  context = memory.Facts();
  assert(!context.loading && !context.gameplay_allowed && context.native_input_allowed);
  memory.bytes.erase(0x831D5335);
  context = memory.Facts();
  assert(context.loading && context.valid && !context.native_input_allowed);
  std::cout << "PASS exact loading byte blocks gameplay and raw fallback, including missing state\n";

  memory = HealthyPlayer();
  // The native predicate is nonzero, including startup states 1/2 and all
  // playback/stop states. It is not a single playback-state equality test.
  for (const uint32_t state : {1u, 2u, 8u, 9u, 10u, 11u, 0x01000000u, UINT32_MAX}) {
    memory.U32(0x82B977F0, state);
    context = memory.Facts();
    assert(context.cutscene && !context.loading && context.valid && context.playing);
    assert(!context.gameplay_allowed && !context.native_input_allowed);
  }
  memory.U32(0x82B977F0, 0);
  // Native preparation can continue while the main state is idle.
  for (const uint32_t preparation : {1u, 2u, 3u, 4u, 5u}) {
    memory.U32(0x82B977FC, preparation);
    context = memory.Facts();
    assert(context.cutscene && !context.gameplay_allowed && !context.native_input_allowed);
  }
  memory.U32(0x82B977FC, 0);
  assert(!memory.Facts().cutscene && memory.Facts().gameplay_allowed);
  memory.bytes.erase(0x82B977F3);
  assert(memory.Facts().cutscene && !memory.Facts().native_input_allowed);
  memory.U32(0x82B977F0, 0);
  memory.bytes.erase(0x82B977FF);
  assert(memory.Facts().cutscene && !memory.Facts().native_input_allowed);
  std::cout << "PASS cutscene start, pending preparation and unreadable state fail closed\n";

  memory = {};
  memory.U32(0x82B2B044, 2);
  memory.U8(0x831D5335, 1);
  memory.U32(0x82B977F0, 2);
  memory.U32(0x82B977FC, 1);
  for (const bool map : {false, true}) {
    context = memory.Facts(!map, map);
    assert(!context.valid && !context.playing && !context.gameplay_allowed);
    assert(context.loading && context.cutscene && context.input_user_known);
    assert(context.input_user == 2 && context.native_input_allowed);
    assert(context.frontend == !map && context.map == map);
  }
  assert(!memory.Facts().native_input_allowed);
  std::cout << "PASS loading and cutscenes preserve native menu admission without a player\n";

  memory = HealthyPlayer();
  memory.U32(0x82A98778, UINT32_MAX);
  context = memory.Facts();
  assert(!context.valid && !context.gameplay_allowed && !context.native_input_allowed);
  memory = HealthyPlayer();
  memory.U32(kInfo + 1400, UINT32_MAX);
  assert(!memory.Facts().valid);
  memory = HealthyPlayer();
  memory.Float(kPed + 484, __builtin_nanf(""));
  assert(!memory.Facts().playing);
  std::cout << "PASS invalid index, overflow pointer and nonfinite health fail closed\n";

  memory = HealthyPlayer();
  memory.U32(kPed + 572, 0x20000000);
  memory.U32(kPed + 2688, kVehicle);
  memory.U32(kVehicle + 3904, kPed);
  memory.U32(kVehicle, 0x8204205C);
  memory.U8(0x831D4DD4, 1);
  memory.U32(0x82B3A0F0, 0);
  memory.U32(0x82B39990, kScopeWidget);
  memory.U8(kScopeWidget + 17, 0);
  context = memory.Facts();
  assert(context.phone_visible && context.base_mode == ContextTouchMode::kVehicleBike);
  memory.U8(kScopeWidget + 17, 1);
  assert(!memory.Facts().phone_visible);
  memory.U8(kScopeWidget + 17, 0);
  memory.U8(0x831D534C, 1);
  assert(!memory.Facts().phone_visible);
  memory.U32(kVehicle + 3904, kInfo);
  assert(memory.Facts().base_mode == ContextTouchMode::kVehiclePassenger);
  std::cout << "PASS real phone visibility overlays retained vehicle/seat facts\n";

  memory = HealthyPlayer();
  PrepareVehicleEntry(memory);
  VehicleQuery vehicle_query;
  context = memory.Facts(false, false, vehicle_query.Queries());
  assert(context.can_enter_vehicle && vehicle_query.calls == 1);
  assert(vehicle_query.ped == kPed && vehicle_query.forward == kTransform + 16);
  vehicle_query.result = 0;
  assert(!memory.Facts(false, false, vehicle_query.Queries()).can_enter_vehicle);
  vehicle_query.result = kVehicle;
  memory.U32(kVehicle + 40, 0x00400000);
  assert(!memory.Facts(false, false, vehicle_query.Queries()).can_enter_vehicle);
  memory.U32(kVehicle + 40, 0x00800000);
  vehicle_query.result = UINT32_MAX;
  assert(!memory.Facts(false, false, vehicle_query.Queries()).can_enter_vehicle);
  vehicle_query.result = kVehicle;
  assert(!memory.Facts().can_enter_vehicle);
  std::cout << "PASS Enter uses native selection and forward vector, rejects absent or invalid candidates\n";

  memory = HealthyPlayer();
  PrepareVehicleEntry(memory);
  vehicle_query = {};
  memory.U32(kInfo + 1200, 1);
  assert(!memory.Facts(false, false, vehicle_query.Queries()).can_enter_vehicle);
  memory.U32(kInfo + 1200, 0);
  memory.U8(0x831D5335, 1);
  assert(!memory.Facts(false, false, vehicle_query.Queries()).can_enter_vehicle);
  memory.U8(0x831D5335, 0);
  memory.U32(0x82B977FC, 1);
  assert(!memory.Facts(false, false, vehicle_query.Queries()).can_enter_vehicle);
  memory.U32(0x82B977FC, 0);
  assert(!memory.Facts(true, false, vehicle_query.Queries()).can_enter_vehicle);
  assert(!memory.Facts(false, true, vehicle_query.Queries()).can_enter_vehicle);
  memory.U32(kPed + 572, 0x20000000);
  memory.U32(kPed + 2688, kVehicle);
  memory.U32(kVehicle + 3904, kPed);
  memory.U32(kVehicle, 0x82041D8C);
  assert(!memory.Facts(false, false, vehicle_query.Queries()).can_enter_vehicle);
  assert(vehicle_query.calls == 0);
  std::cout << "PASS native vehicle query never runs in menus, loading, cutscenes, control lock or vehicles\n";

  memory = HealthyPlayer();
  PrepareVehicleEntry(memory);
  vehicle_query = {};
  memory.U32(kPed + 568, 0x40000);
  assert(!memory.Facts(false, false, vehicle_query.Queries()).can_enter_vehicle);
  memory.U32(kPed + 568, 0);
  memory.U32(kPed + 544, 0);
  assert(!memory.Facts(false, false, vehicle_query.Queries()).can_enter_vehicle);
  memory.U32(kPed + 544, kInfo);
  for (const uint32_t transform : {0u, kTransform + 1, 0xFFFFFFF0u}) {
    memory.U32(kPed + 32, transform);
    assert(!memory.Facts(false, false, vehicle_query.Queries()).can_enter_vehicle);
  }
  memory.U32(kPed + 32, kTransform);
  memory.Float(kTransform + 20, __builtin_nanf(""));
  assert(!memory.Facts(false, false, vehicle_query.Queries()).can_enter_vehicle);
  memory.Float(kTransform + 20, 1.0f);
  memory.bytes.erase(kTransform + 56);
  assert(!memory.Facts(false, false, vehicle_query.Queries()).can_enter_vehicle);
  assert(vehicle_query.calls == 0);
  std::cout << "PASS blocked ped and corrupt transforms cannot enter the native vehicle query\n";

  memory = HealthyPlayer();
  PrepareVehicleEntry(memory);
  vehicle_query = {};
  memory.U32(kVehicle + 4836, 3);
  assert(!memory.Facts(false, false, vehicle_query.Queries()).can_enter_vehicle);
  for (const uint8_t flags : {uint8_t{0}, uint8_t{1}, uint8_t{0x80}}) {
    memory.U8(kVehicle + 5316, flags);
    assert(!memory.Facts(false, false, vehicle_query.Queries()).can_enter_vehicle);
  }
  memory.U8(kVehicle + 5316, 0x40);
  assert(memory.Facts(false, false, vehicle_query.Queries()).can_enter_vehicle);
  memory.U32(kVehicle + 4836, 0);
  memory.bytes.erase(kVehicle + 5316);
  assert(memory.Facts(false, false, vehicle_query.Queries()).can_enter_vehicle);
  std::cout << "PASS native subtype entry gate is checked after vehicle selection\n";

  const auto weapon_bounds = ClipTouchWeaponHudBounds(0.8, 0.03, 0.95, 0.12, 0xFFFFFFFF);
  assert(weapon_bounds && weapon_bounds->left == 0.8f && weapon_bounds->top == 0.03f &&
         weapon_bounds->right == 0.95f && weapon_bounds->bottom == 0.12f);
  const auto clipped_weapon = ClipTouchWeaponHudBounds(-0.2, 0.1, 0.3, 1.2, 0x01000000);
  assert(clipped_weapon && clipped_weapon->left == 0.0f && clipped_weapon->top == 0.1f &&
         clipped_weapon->right == 0.3f && clipped_weapon->bottom == 1.0f);
  assert(!ClipTouchWeaponHudBounds(0.8, 0.03, 0.95, 0.12, 0x00FFFFFF));
  assert(!ClipTouchWeaponHudBounds(0.95, 0.03, 0.8, 0.12, 0xFFFFFFFF));
  assert(!ClipTouchWeaponHudBounds(0.8, 0.12, 0.95, 0.12, 0xFFFFFFFF));
  assert(!ClipTouchWeaponHudBounds(1.1, 0.03, 1.2, 0.12, 0xFFFFFFFF));
  assert(!ClipTouchWeaponHudBounds(-0.2, -0.3, -0.1, -0.2, 0xFFFFFFFF));
  for (const auto invalid : {std::numeric_limits<double>::quiet_NaN(),
                             std::numeric_limits<double>::infinity()}) {
    assert(!ClipTouchWeaponHudBounds(invalid, 0.03, 0.95, 0.12, 0xFFFFFFFF));
    assert(!ClipTouchWeaponHudBounds(0.8, invalid, 0.95, 0.12, 0xFFFFFFFF));
    assert(!ClipTouchWeaponHudBounds(0.8, 0.03, invalid, 0.12, 0xFFFFFFFF));
    assert(!ClipTouchWeaponHudBounds(0.8, 0.03, 0.95, invalid, 0xFFFFFFFF));
  }
  std::cout << "PASS weapon hit bounds match clipped final sprite and reject transparent or invalid draws\n";

  context = HealthyPlayer().Facts();
  context.epoch = 100;
  context.generation = 10;
  context.presentation_revision = 7;
  TouchWeaponHudSnapshot weapon_hud;
  weapon_hud.epoch = context.epoch;
  weapon_hud.generation = context.generation;
  weapon_hud.presentation_revision = context.presentation_revision;
  weapon_hud.presentation_generation = 12;
  weapon_hud.weapon_type = context.weapon_type;
  weapon_hud.weapon_slot = context.weapon_slot;
  weapon_hud.bounds = weapon_bounds;
  PublishTouchWeaponHudSnapshot(weapon_hud);
  assert(ReadTouchWeaponHudBounds(GetTouchWeaponHudSnapshot(), context, 12));
  for (const uint64_t epoch : {100u, 101u, 102u}) {
    context.epoch = epoch;
    assert(ReadTouchWeaponHudBounds(GetTouchWeaponHudSnapshot(), context, 12));
  }
  for (const uint64_t epoch : {99u, 103u}) {
    context.epoch = epoch;
    assert(!ReadTouchWeaponHudBounds(GetTouchWeaponHudSnapshot(), context, 12));
  }
  context.epoch = 100;
  auto suppressed_hud = weapon_hud;
  suppressed_hud.bounds.reset();
  PublishTouchWeaponHudSnapshot(suppressed_hud);
  assert(!ReadTouchWeaponHudBounds(GetTouchWeaponHudSnapshot(), context, 12));
  PublishTouchWeaponHudSnapshot(weapon_hud);
  assert(ReadTouchWeaponHudBounds(GetTouchWeaponHudSnapshot(), context, 12));
  std::cout << "PASS hidden HUD pass erases weapon hit region and missing draws expire within bounded polls\n";

  for (const auto member : {&TouchContextSnapshot::valid, &TouchContextSnapshot::playing,
                            &TouchContextSnapshot::gameplay_allowed,
                            &TouchContextSnapshot::native_input_allowed}) {
    auto blocked = context;
    blocked.*member = false;
    assert(!ReadTouchWeaponHudBounds(weapon_hud, blocked, 12));
  }
  for (const auto member : {&TouchContextSnapshot::frontend, &TouchContextSnapshot::map,
                            &TouchContextSnapshot::loading, &TouchContextSnapshot::cutscene}) {
    auto blocked = context;
    blocked.*member = true;
    assert(!ReadTouchWeaponHudBounds(weapon_hud, blocked, 12));
  }
  for (const auto member : {&TouchContextSnapshot::generation,
                            &TouchContextSnapshot::presentation_revision}) {
    auto changed = context;
    ++(changed.*member);
    assert(!ReadTouchWeaponHudBounds(weapon_hud, changed, 12));
  }
  for (const auto member : {&TouchContextSnapshot::weapon_type, &TouchContextSnapshot::weapon_slot}) {
    auto changed = context;
    ++(changed.*member);
    assert(!ReadTouchWeaponHudBounds(weapon_hud, changed, 12));
  }
  assert(!ReadTouchWeaponHudBounds(weapon_hud, context, 0));
  assert(!ReadTouchWeaponHudBounds(weapon_hud, context, 13));
  auto invalid_hud = weapon_hud;
  invalid_hud.generation = 0;
  assert(!ReadTouchWeaponHudBounds(invalid_hud, context, 12));
  invalid_hud = weapon_hud;
  invalid_hud.bounds->left = -0.1f;
  assert(!ReadTouchWeaponHudBounds(invalid_hud, context, 12));
  invalid_hud = weapon_hud;
  invalid_hud.bounds->bottom = std::numeric_limits<float>::quiet_NaN();
  assert(!ReadTouchWeaponHudBounds(invalid_hud, context, 12));
  std::cout << "PASS weapon regions reject menu/load/control gates and stale player, weapon or presentation\n";

  memory = HealthyPlayer();
  memory.U32(kPed + 640 + 36 + 2 * 8, 7);
  memory.U32(kPed + 640 + 40 + 2 * 8, 31u << 16);
  memory.U32(0x82CB8AB0 + 7 * 272 + 12, 2);
  memory.U32(kPed + 640 + 36 + 1 * 8, 1);
  memory.U32(0x82CB8AB0 + 1 * 272 + 12, 1);
  memory.U32(0x82A993E8 + 7 * 4, 0x10080000);
  constexpr char weapon_name[] = "PISTOL";
  for (uint32_t index = 0; index < sizeof(weapon_name); ++index)
    memory.U8(0x10080000 + index, weapon_name[index]);
  context = memory.Facts();
  assert(context.inventory_known && context.weapons[2].owned && context.weapons[2].selectable);
  assert(context.weapons[2].type == 7 && context.weapons[2].ammo == 31);
  assert(context.weapons[1].selectable);
  assert(std::string_view(context.weapons[2].native_identifier.data()) == "PISTOL");
  memory.U8(0x10080000, 255);
  context = memory.Facts();
  assert(context.inventory_known && context.weapons[2].native_identifier[0] == 0);
  memory.U32(kPed + 640 + 40 + 2 * 8, 0);
  assert(!memory.Facts().weapons[2].selectable);
  memory.U32(kPed + 640 + 36 + 2 * 8, 60);
  assert(!memory.Facts().inventory_known);
  std::cout << "PASS bounded inventory applies original ownership/ammo/fire-type cycle predicate\n";

  memory = HealthyPlayer();
  memory.U32(kIntelligence + 736, kTask);
  memory.U32(kTask + 4, 1046);
  memory.U32(kTask + 8, 0);
  memory.U32(kTask + 12, 0);
  assert(!memory.Facts().in_cover);
  memory.U32(kTask + 4, 1054);
  assert(memory.Facts().in_cover);
  memory.U32(kTask + 4, 1046);
  memory.U32(kTask + 12, kTask);
  assert(!memory.Facts().cover_known);
  memory.U32(kIntelligence + 736, 0);
  memory.U32(kIntelligence + 68, kTask);
  memory.U32(kTask, kTaskVtable);
  memory.U32(kTaskVtable + 12, 0x82737A08);
  memory.U32(kTask + 8, 0);
  memory.U32(kTask + 12, 0);
  assert(memory.Facts().melee);
  std::cout << "PASS cover status differs from task permission and corrupt chains are bounded\n";

  TouchContextGeneration generations;
  context = HealthyPlayer().Facts();
  const auto first = generations.Advance(context);
  context.phone_visible = context.aiming = context.in_cover = true;
  assert(generations.Advance(context) == first);
  context.presentation_revision = 1;
  assert(generations.Advance(context) == first);
  context.can_enter_vehicle = true;
  assert(generations.Advance(context) == first);
  ++context.player_generation;
  const auto replaced = generations.Advance(context);
  assert(replaced != first);
  context.input_user = 1;
  const auto user_changed = generations.Advance(context);
  assert(user_changed != replaced);
  context.minigame_active = true;
  const auto minigame = generations.Advance(context);
  assert(minigame != user_changed);
  context.weapon_slot = 2;
  assert(generations.Advance(context) == minigame);
  std::cout << "PASS ownership generation changes for player/user replacement, retains phone overlay\n";

  memory = HealthyPlayer();
  TouchContextGeneration transitions;
  const auto gameplay = transitions.Advance(memory.Facts());
  memory.U8(0x831D5335, 1);
  const auto loading = transitions.Advance(memory.Facts());
  assert(loading != gameplay && transitions.Advance(memory.Facts()) == loading);
  memory.U8(0x831D5335, 0);
  const auto loaded = transitions.Advance(memory.Facts());
  assert(loaded != loading && loaded != gameplay);
  memory.U32(0x82B977FC, 1);
  const auto preparing = transitions.Advance(memory.Facts());
  assert(preparing != loaded);
  memory.U32(0x82B977F0, 2);
  memory.U32(0x82B977FC, 0);
  assert(transitions.Advance(memory.Facts()) == preparing);
  memory.U32(0x82B977F0, 0);
  const auto finished = transitions.Advance(memory.Facts());
  assert(finished != preparing && finished != loaded);
  // Menu admission stays true throughout a load, so loading itself must also
  // invalidate ownership independently of the admission booleans.
  const auto menu = transitions.Advance(memory.Facts(true));
  memory.U8(0x831D5335, 1);
  const auto menu_loading = transitions.Advance(memory.Facts(true));
  assert(memory.Facts(true).native_input_allowed && menu_loading != menu);
  memory.U8(0x831D5335, 0);
  assert(transitions.Advance(memory.Facts(true)) != menu_loading);
  std::cout << "PASS load and cutscene boundaries cancel ownership without churn inside a cutscene\n";

  memory = HealthyPlayer();
  memory.U32(0x8319277C, kTask);
  memory.U32(kTask + 4, 51);
  memory.U32(kTask + 8, 0x98751695);
  memory.U32(kTask + 12, 0);
  memory.U32(0x831927B4, kIntelligence);
  memory.U32(kIntelligence + 0x2A18, 3);
  const TouchContextMemory script_memory{&memory, Memory::Read};
  assert(ReadTouchScriptThread(script_memory) == 51);
  assert(ReadTouchParachuteState(script_memory) == 3);
  memory.U32(kTask + 4, 52);
  assert(ReadTouchScriptThread(script_memory) == 52);
  memory.U32(kTask + 8, 1);
  assert(!ReadTouchParachuteState(script_memory));
  memory.U32(kTask + 12, 2);
  assert(ReadTouchScriptThread(script_memory) == 0);
  memory.U32(kTask + 12, 0);
  memory.U32(kTask + 4, 0);
  assert(ReadTouchScriptThread(script_memory) == 0);
  memory.U32(0x8319277C, UINT32_MAX);
  assert(ReadTouchScriptThread(script_memory) == 0);
  std::cout << "PASS native script instance identity survives address reuse and isolates parachute state\n";

  const auto raw = DecodeTouchHelpToken("PAD_B", 285);
  assert(raw && raw->kind == TouchScriptQueryKind::kRawButton && raw->action == 17);
  const auto semantic = DecodeTouchHelpToken("INPUT_CONTEXT", 284, 23);
  assert(semantic && semantic->kind == TouchScriptQueryKind::kControlHeld && semantic->action == 23);
  assert(!DecodeTouchHelpToken("Press B to enter taxi", 285));
  assert(!DecodeTouchHelpToken("INPUT_CONTEXT", 284));
  assert(!DecodeTouchHelpToken("INPUT_CONTEXT", 255, 23));
  assert(!DecodeTouchHelpToken("PAD_B", 284));
  assert(DecodeTouchHelpToken("INPUT_PHONE_ACCEPT", 284)->action == 16);
  assert(DecodeTouchHelpToken("PAD_RSTICK_ROTATE", 296)->kind ==
         TouchScriptQueryKind::kAnalogueSticks);
  TouchHelpObserver observer;
  observer.Begin(0x1234, 100, 9);
  observer.Token(*raw);
  assert(observer.Finish().control_count == 0);
  observer.Text(true);
  observer.Token(*raw);
  observer.Token({TouchScriptQueryKind::kRawButtonPressed, 17});
  observer.Token(*semantic);
  auto prompts = observer.Finish();
  assert(prompts.visible && prompts.control_count == 2 && prompts.controls[1].generation == 9);
  for (uint32_t action = 0; action < 100; ++action)
    observer.Token({TouchScriptQueryKind::kControlHeld, action});
  assert(observer.Finish().control_count == TouchVisiblePromptSnapshot::kMaximumControls);
  observer.Begin(0x1234, 101, 9);
  assert(!observer.Finish().visible);
  uint8_t host;
  BeginTouchHelpDraw(&host, 0x1234);
  ObserveTouchHelpText(&host, 0x5678);
  EndTouchHelpText();
  EndTouchHelpDraw();
  BeginTouchHelpDraw(&host, 0x1234);
  EndTouchHelpDraw();
  assert(GetTouchVisiblePromptSnapshot(0, 0).visible);
  assert(!GetTouchVisiblePromptSnapshot(3, 0).visible);
  assert(!GetTouchVisiblePromptSnapshot(0, 1).visible);
  std::cout << "PASS only accepted native input tokens become bounded visible prompt identities\n";
}
