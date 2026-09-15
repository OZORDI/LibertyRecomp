#include "gta4_sony_feedback.h"
#include "gta4_sony_action_policy.h"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <limits>
#include <mutex>
#include <unordered_map>

#include <SDL3/SDL_timer.h>
#include <rex/input/input_trace.h>
#include <rex/runtime.h>

#include "gta4_init.h"
#if !defined(GTA4_SONY_LEGACY_HOST)
#include "gta4_touch_coordinator.h"
#endif

#if defined(GTA4_SONY_PRIMARY_PLAYER_ALIAS)
void GTA4_RunWithPrimaryPlayerInfoAlias(PPCContext&, uint8_t*,
                                       void (*)(PPCContext&, uint8_t*));
#endif

namespace {
namespace sony = rex::input::sony;

// Derived with Python from generated retail PPC; see verify_gta4_sony_sites.py.
constexpr uint32_t kPrimaryPlayer = 0x82A98778;
constexpr uint32_t kPlayerInfoTable = 0x82C01C70;
constexpr uint32_t kPlayerGenerationTable = 0x82C01C30;
constexpr uint32_t kRetailPlayerCount = 16;
constexpr uint32_t kGameplayControl = 0x82B2A2F0;
constexpr uint32_t kControlSpan = 4204;
constexpr uint32_t kControlUserOffset = 3412;
constexpr uint32_t kActionArray = 2328;
constexpr uint32_t kActionStride = 12;
constexpr uint32_t kLastInput = 4200;
constexpr uint32_t kInputClock = 0x82C6C2A4;
constexpr uint32_t kEpisode = 0x82B39504;
constexpr uint32_t kGameMode = 0x82B3950C;
constexpr uint32_t kAliveThreshold = 0x82000D68;
constexpr uint32_t kCutscene = 0x82B977F0;
constexpr uint32_t kMinigame = 0x82BA1D40;
constexpr uint32_t kPauseVisible = 0x82BFA144;
constexpr uint32_t kPauseTransition = 0x82BFA13C;
constexpr uint32_t kFrontendWidgetIndex = 0x82BFA10C;
constexpr uint32_t kFrontendWidgets = 0x82CC7BD0;
constexpr uint32_t kFrontendWidgetFlags = 0x82CC7BDC;
constexpr uint32_t kPhoneCreated = 0x831D4DD4;
constexpr uint32_t kPhoneOffscreen = 0x831D534C;
constexpr uint32_t kPhoneIndex = 0x82B3A0F0;
constexpr uint32_t kPhoneObjects = 0x82B39990;

bool GuestSpan(uint8_t* base, uint32_t address, size_t size, bool writable = false) {
  if (!base || !address || !size || size > std::numeric_limits<uint32_t>::max()) return false;
  const uint64_t last = uint64_t{address} + size - 1;
  if (last > std::numeric_limits<uint32_t>::max()) return false;
  auto* kernel = REX_KERNEL_STATE();
  auto* memory = kernel ? kernel->memory() : nullptr;
  auto* heap = memory ? memory->LookupHeap(address) : nullptr;
  if (!heap || heap != memory->LookupHeap(static_cast<uint32_t>(last))) return false;
  const auto access = heap->QueryRangeAccess(address, static_cast<uint32_t>(last));
  using rex::memory::PageAccess;
  return access == PageAccess::kReadWrite || access == PageAccess::kExecuteReadWrite ||
         (!writable && (access == PageAccess::kReadOnly || access == PageAccess::kExecuteReadOnly));
}

float FloatAt(uint8_t* base, uint32_t address) {
  return std::bit_cast<float>(REX_LOAD_U32(address));
}

bool FrontendActive(uint8_t* base) {
  if (!GuestSpan(base, kPauseVisible, 1) || !GuestSpan(base, kPauseTransition, 4) ||
      !GuestSpan(base, kFrontendWidgetIndex, 4)) return true;
  if (REX_LOAD_U8(kPauseVisible)) {
    const uint32_t transition = REX_LOAD_U32(kPauseTransition);
    return transition != 2 && transition != 6;
  }
  // sub_8224EEF8(0) -> sub_8229C4B8. A negative widget sentinel is inactive.
  const uint32_t index = REX_LOAD_U32(kFrontendWidgetIndex);
  if (index == static_cast<uint32_t>(-90)) return false;
  if (index >= 3 || !GuestSpan(base, kFrontendWidgetFlags + index, 1) ||
      !GuestSpan(base, kFrontendWidgets + index * 4, 4)) return true;
  if (!REX_LOAD_U8(kFrontendWidgetFlags + index)) return false;
  const uint32_t widget = REX_LOAD_U32(kFrontendWidgets + index * 4);
  return !GuestSpan(base, widget, 3110) || REX_LOAD_U8(widget + 3109) != 0;
}

bool PhoneVisible(uint8_t* base) {
  if (!GuestSpan(base, kPhoneCreated, 1) || !GuestSpan(base, kPhoneOffscreen, 1) ||
      !GuestSpan(base, kPhoneIndex, 4)) return true;
  if (!REX_LOAD_U8(kPhoneCreated) || REX_LOAD_U8(kPhoneOffscreen)) return false;
  const uint32_t index = REX_LOAD_U32(kPhoneIndex);
  if (index >= 64 || !GuestSpan(base, kPhoneObjects + index * 4, 4)) return true;
  const uint32_t object = REX_LOAD_U32(kPhoneObjects + index * 4);
  return !GuestSpan(base, object, 18) || !REX_LOAD_U8(object + 17);
}

struct GuestSnapshot {
  GTA4SonyLocalPlayer player{};
  bool valid = false;
};

// A scoped output pointer permits using the existing alias callback without
// passing host addresses through guest registers or retaining guest pointers.
thread_local GuestSnapshot* query_output = nullptr;

void ReadAliased(PPCContext&, uint8_t* base) {
  auto& out = *query_output;
  const uint32_t index = REX_LOAD_U32(kPrimaryPlayer);
  if (index >= kRetailPlayerCount) return;
  const uint32_t info = REX_LOAD_U32(kPlayerInfoTable + index * 4);
  if (!GuestSpan(base, info, 1404) || !GuestSpan(base, kGameplayControl, kControlSpan)) return;
  const uint32_t ped = REX_LOAD_U32(info + 1400);
  if (!GuestSpan(base, ped, 2692)) return;
  const uint32_t user = REX_LOAD_U32(kGameplayControl + kControlUserOffset);
  if (user >= 4) return;
  auto& player = out.player;
  player.user = user;
  player.ped = ped;
  const auto device = sony::GetService().ReadDevice(user);
  player.generation = device.generation;
  auto& state = player.state;
  // Opaque identity values only: the service never interprets these as an
  // address. Include the retail generation to invalidate reused player slots.
  state.player_identity = ped ^ std::rotl(REX_LOAD_U32(kPlayerGenerationTable + index * 4), 16);
  state.episode = static_cast<int>(REX_LOAD_U32(kEpisode));
  const uint32_t data = REX_LOAD_U32(ped + 544);
  if (GuestSpan(base, data, 204)) {
    state.wanted = static_cast<int>(std::min(REX_LOAD_U32(data + 200), 6u));
  }
  if (REX_LOAD_U32(ped + 572) & 0x20000000u) {
    const uint32_t vehicle = REX_LOAD_U32(ped + 2688);
    if (GuestSpan(base, vehicle, 3908)) {
      player.vehicle = vehicle;
      state.in_vehicle = true;
      state.driver = REX_LOAD_U32(vehicle + 3904) == ped;
    }
  }
  state.context_identity = state.in_vehicle ? player.vehicle ^ uint32_t{state.driver}
                                           : state.player_identity;
  const float health = FloatAt(base, ped + 484);
  const float threshold = FloatAt(base, kAliveThreshold);
  const bool playing = REX_LOAD_U32(info + 1232) == 2 && std::isfinite(health) &&
                       std::isfinite(threshold) && health >= threshold;
  bool title_owned = false;
#if !defined(GTA4_SONY_LEGACY_HOST)
  title_owned = GTA4_TouchTitleInputOwned();
#endif
  state.active = gta4::sony::GameplayOwned(
      playing, REX_LOAD_U32(info + 1200), REX_LOAD_U32(kGameMode) == 1,
      REX_LOAD_U32(ped + 2496), FrontendActive(base), PhoneVisible(base),
      REX_LOAD_U32(kMinigame) != 0, title_owned || REX_LOAD_U32(kCutscene) != 0);
  state.gestures_owned = state.active;
  const uint32_t aim = kGameplayControl + kActionArray + 6 * kActionStride;
  state.aiming = state.active && rex::input::mnk::DecodeActionMagnitude(
                                    REX_LOAD_U8(aim), REX_LOAD_U8(aim + 2)) > 127;

  // Checked equivalent of sub_823D4D60: both object+600 and manager+32
  // contain pointers, not inline CWeapon structures. Unarmed/special fallback
  // slots are deliberately excluded from trigger resistance.
  const uint32_t manager = ped + 640;
  const uint32_t slot = REX_LOAD_U32(manager);
  const uint32_t object = REX_LOAD_U32(manager + 20);
  if (slot > 0 && slot < 10 && GuestSpan(base, object, 604)) {
    const uint32_t weapon = REX_LOAD_U32(object + 600);
    if (GuestSpan(base, weapon, 36) &&
        REX_LOAD_U32(weapon + 20) == REX_LOAD_U32(manager + 36 + slot * 8)) {
      // Local fire dispatch sub_8226F428 admits state 0, or state 1 for a
      // continuing shot. Other states are rejected even with rounds left in
      // the clip, so partial-clip reloads must release trigger resistance too.
      const uint32_t weapon_state = REX_LOAD_U32(weapon + 24);
      state.can_fire = state.active && REX_LOAD_U16(weapon + 28) != 0 &&
                       (weapon_state == 0 || weapon_state == 1);
    }
  }
  if (state.active && state.in_vehicle && state.driver) {
    state.trigger_context = sony::TriggerContext::kVehicle;
  } else if (state.active && state.can_fire && !state.in_vehicle) {
    state.trigger_context = sony::TriggerContext::kWeapon;
  }

  // Entity position selection in sub_822343C0: matrix+48 or inline+16.
  const uint32_t matrix = REX_LOAD_U32(ped + 32);
  uint32_t position = 0;
  if (matrix && GuestSpan(base, matrix, 60)) position = matrix + 48;
  if (!matrix) position = ped + 16;
  if (position) {
    player.position_valid = true;
    for (size_t axis = 0; axis < player.position.size(); ++axis) {
      player.position[axis] = FloatAt(base, position + static_cast<uint32_t>(axis) * 4);
      player.position_valid &= std::isfinite(player.position[axis]);
    }
  }
  out.valid = true;
}

GuestSnapshot ReadSnapshot(PPCContext& context, uint8_t* base) {
  GuestSnapshot out;
  constexpr std::array globals = {kPrimaryPlayer, kEpisode, kGameMode, kAliveThreshold,
                                  kCutscene, kMinigame};
  for (const uint32_t address : globals) {
    if (!GuestSpan(base, address, 4)) return out;
  }
  if (!GuestSpan(base, kPlayerInfoTable, 64) ||
      !GuestSpan(base, kPlayerGenerationTable, 64)) return out;
  struct QueryScope {
    GuestSnapshot* previous = query_output;
    explicit QueryScope(GuestSnapshot& value) { query_output = &value; }
    ~QueryScope() { query_output = previous; }
  } scope(out);
  PPCContext nested = context;
#if defined(GTA4_SONY_PRIMARY_PLAYER_ALIAS)
  GTA4_RunWithPrimaryPlayerInfoAlias(nested, base, ReadAliased);
#else
  ReadAliased(nested, base);
#endif
  return out;
}

struct ControlHistory {
  uint64_t last_seen = 0;
  std::array<gta4::sony::ActionHistory, gta4::sony::kActions.size()> actions{};
};
struct TraceState {
  uint64_t generation = 0;
  sony::Model model = sony::Model::kNone;
  bool snapshot = false;
  bool enabled = false;
  bool active = false;
  bool owned = false;
  bool in_vehicle = false;
  bool driver = false;
  bool aiming = false;
  bool can_fire = false;
  int wanted = 0;
  int episode = 0;
  sony::TriggerContext triggers = sony::TriggerContext::kNone;
  bool operator==(const TraceState&) const = default;
};
struct UserReplay {
  gta4::sony::PulseQueue queue;
  gta4::sony::ActionEpoch actions;
  uint64_t generation = 0;
  uint32_t player = 0;
  uint32_t context = 0;
  bool owned = false;
  bool in_vehicle = false;
  bool driver = false;
  TraceState last_trace;
  uint32_t trace_count = 0;
  std::unordered_map<uint32_t, ControlHistory> history;
};
std::mutex replay_mutex;
std::array<UserReplay, 4> users;
uint64_t poll_sequence = 0;

}  // namespace

void GTA4_SonyEndPoll(PPCContext& context, uint8_t* base) {
  const auto snapshot = ReadSnapshot(context, base);
  const auto options = sony::GetOptions();
  const uint64_t now = SDL_GetTicks();
  std::array<sony::Input, 4> input;
  for (uint32_t user = 0; user < input.size(); ++user) {
    input[user] = sony::GetService().Consume(user, now, options);
    sony::GameState state;
    if (snapshot.valid && snapshot.player.user == user) state = snapshot.player.state;
    state.gestures_owned = state.gestures_owned && options.enabled && options.gestures;
    sony::GetService().Publish(user, input[user].device.generation, state, now);
  }
  std::lock_guard lock(replay_mutex);
  ++poll_sequence;
  for (uint32_t user = 0; user < users.size(); ++user) {
    auto& replay = users[user];
    const auto& device = input[user].device;
    const bool local = snapshot.valid && snapshot.player.user == user;
    const auto state = local ? snapshot.player.state : sony::GameState{};
    replay.owned = input[user].gestures_owned && options.enabled && options.gestures &&
                   state.gestures_owned &&
                   device.model != sony::Model::kNone && device.touchpad;
    replay.generation = device.generation;
    replay.player = state.player_identity;
    replay.context = state.context_identity;
    replay.in_vehicle = state.in_vehicle;
    replay.driver = state.driver;
    replay.actions = replay.queue.Advance(device.generation, state.player_identity,
        state.context_identity, replay.owned, state.in_vehicle, state.driver,
        input[user].gestures, input[user].cinematic);
    const TraceState trace{device.generation, device.model, local, options.enabled,
        state.active, replay.owned, state.in_vehicle, state.driver, state.aiming,
        state.can_fire, state.wanted, state.episode, state.trigger_context};
    if (rex::input::IsInputTraceEnabled() && replay.trace_count < 64 &&
        (!replay.trace_count || replay.last_trace != trace)) {
      replay.last_trace = trace;
      ++replay.trace_count;
      REXLOG_INFO("sony-gameplay: user={} generation={} model={} snapshot={} enabled={} "
                  "active={} gestures={} vehicle={} driver={} aim={} can-fire={} "
                  "wanted={} episode={} triggers={}", user, trace.generation,
                  static_cast<int>(trace.model), trace.snapshot, trace.enabled,
                  trace.active, trace.owned, trace.in_vehicle, trace.driver,
                  trace.aiming, trace.can_fire, trace.wanted, trace.episode,
                  static_cast<int>(trace.triggers));
    }
    // Histories are only addressed again after a fresh writable-span check.
    // Discard objects absent for a poll; no stale pointer is ever dereferenced.
    std::erase_if(replay.history, [](const auto& entry) {
      return entry.second.last_seen + 1 < poll_sequence;
    });
  }
}

void GTA4_SonyReplay(PPCContext& context, uint8_t* base, uint32_t control) {
  if (!GuestSpan(base, control, kControlSpan, true)) return;
  const uint32_t user = REX_LOAD_U32(control + kControlUserOffset);
  if (user >= users.size()) return;
  const auto snapshot = ReadSnapshot(context, base);
  const auto options = sony::GetOptions();
  const auto device = sony::GetService().ReadDevice(user);
  const bool service_owned = sony::GetService().OwnsGestures(user, SDL_GetTicks(), options);
  std::lock_guard lock(replay_mutex);
  auto& replay = users[user];
  if (!poll_sequence) return;
  const bool current_owner = service_owned && options.enabled && options.gestures && replay.owned &&
      snapshot.valid && snapshot.player.user == user && snapshot.player.state.gestures_owned &&
      snapshot.player.state.player_identity == replay.player &&
      snapshot.player.state.context_identity == replay.context &&
      snapshot.player.state.in_vehicle == replay.in_vehicle &&
      snapshot.player.state.driver == replay.driver &&
      device.generation == replay.generation && device.model != sony::Model::kNone;
  const auto actions = current_owner ? replay.actions : gta4::sony::ActionEpoch{};
  auto& history = replay.history[control];
  history.last_seen = poll_sequence;
  bool changed = false;
  for (size_t index = 0; index < gta4::sony::kActions.size(); ++index) {
    const uint32_t record = control + kActionArray + gta4::sony::kActions[index] * kActionStride;
    const gta4::sony::ActionBytes before{REX_LOAD_U8(record + 2), REX_LOAD_U8(record + 3)};
    const auto after = history.actions[index].Merge(poll_sequence, REX_LOAD_U8(record), before,
                                                    actions.current[index], actions.previous[index]);
    if (before != after) {
      REX_STORE_U8(record + 2, after.current);
      REX_STORE_U8(record + 3, after.previous);
      changed = true;
    }
  }
  if (changed && current_owner && GuestSpan(base, kInputClock, 4)) {
    REX_STORE_U32(control + kLastInput, REX_LOAD_U32(kInputClock));
  }
}

bool GTA4_SonyReadLocalPlayer(PPCContext& context, uint8_t* base,
                             GTA4SonyLocalPlayer& player) {
  const auto snapshot = ReadSnapshot(context, base);
  player = snapshot.player;
  if (!snapshot.valid || !player.state.active || !sony::GetOptions().enabled) return false;
  const auto device = sony::GetService().ReadDevice(player.user);
  return device.model != sony::Model::kNone && device.generation == player.generation;
}

void GTA4_SonyEmit(const GTA4SonyLocalPlayer& player, sony::Event event, float strength) {
  if (!player.state.active || !std::isfinite(strength)) return;
  sony::GetService().Emit(player.user, player.generation, event,
                          std::clamp(strength, 0.0f, 1.0f), SDL_GetTicks());
}
