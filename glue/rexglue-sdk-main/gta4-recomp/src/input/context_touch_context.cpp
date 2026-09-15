#include "input/context_touch_context.h"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstring>
#include <limits>
#include <mutex>
#include <optional>
#include <string_view>

#if !defined(GTA4_TOUCH_CONTEXT_TEST)
#include <rex/input/absolute_pointer.h>
#include <rex/runtime.h>
#include "gta4_init.h"
#include "input/context_touch_controls.h"
#endif

#if defined(GTA4_TOUCH_PRIMARY_PLAYER_ALIAS)
void GTA4_RunWithPrimaryPlayerInfoAlias(PPCContext&, uint8_t*,
                                       void (*)(PPCContext&, uint8_t*));
#endif

namespace gta4::input {
namespace {

// Generated PPC source: player/control facts match sub_825B3320,
// sub_825B56B0 and sub_821B41E8. All address/size derivations checked in Python.
constexpr uint32_t kPlayer = 0x82A98778;
constexpr uint32_t kPlayerInfo = 0x82C01C70;
constexpr uint32_t kPlayerGeneration = 0x82C01C30;
constexpr uint32_t kControl = 0x82B2A2F0;
constexpr uint32_t kGameMode = 0x82B3950C;
constexpr uint32_t kAliveThreshold = 0x82000D68;
constexpr uint32_t kCutscene = 0x82B977F0;
constexpr uint32_t kCutscenePreparation = 0x82B977FC;
constexpr uint32_t kLoadingActive = 0x831D5335;
constexpr uint32_t kMinigame = 0x82BA1D40;
constexpr uint32_t kPhoneCreated = 0x831D4DD4;
constexpr uint32_t kPhoneOffscreen = 0x831D534C;
constexpr uint32_t kWeaponScopeWidgetIndex = 0x82B3A0F0;
constexpr uint32_t kHudWidgets = 0x82B39990;

struct Reader {
  const TouchContextMemory& memory;
  template <typename T>
  std::optional<T> Scalar(uint32_t address) const noexcept {
    T value{};
    if (!address || !memory.read ||
        uint64_t{address} + sizeof(T) > uint64_t{UINT32_MAX} + 1 ||
        !memory.read(memory.opaque, address, &value, sizeof(value))) return std::nullopt;
    if constexpr (sizeof(T) == 4) value = __builtin_bswap32(value);
    if constexpr (sizeof(T) == 2) value = __builtin_bswap16(value);
    return value;
  }
  std::optional<uint32_t> U32(uint32_t object, uint32_t offset = 0) const noexcept {
    if (uint64_t{object} + offset > UINT32_MAX) return std::nullopt;
    return Scalar<uint32_t>(object + offset);
  }
  std::optional<uint8_t> U8(uint32_t object, uint32_t offset = 0) const noexcept {
    if (uint64_t{object} + offset > UINT32_MAX) return std::nullopt;
    return Scalar<uint8_t>(object + offset);
  }
};

void ReadPhone(const Reader& read, TouchContextSnapshot& out) noexcept {
  const auto created = read.U8(kPhoneCreated), offscreen = read.U8(kPhoneOffscreen);
  if (!created || !offscreen) return;
  if (!*created || *offscreen) {
    out.phone_visibility_known = true;
    return;
  }
  // CAN_PHONE_BE_SEEN_ON_SCREEN (sub_8217D3E0) intentionally tests
  // HUD_WEAPON_SCOPE: a visible scope obscures the phone. The widget's
  // index is HUD layout +1624, and sub_821C4B90's next-index counter
  // follows the 64-pointer HUD widget table.
  const auto index = read.U32(kWeaponScopeWidgetIndex);
  if (!index || *index >= 64) return;
  const auto object = read.U32(kHudWidgets, *index * 4);
  if (!object || !*object) return;
  const auto scope_visible = read.U8(*object, 17);
  if (!scope_visible) return;
  out.phone_visibility_known = true;
  out.phone_visible = *scope_visible == 0;
}

// Checked bounded equivalent of sub_82159078 with r5=0, as called by
// IS_PED_IN_COVER (sub_825BE450). The status id is 1054, not the task's 1046.
std::optional<bool> InCover(const Reader& read, uint32_t intelligence) noexcept {
  auto node = read.U32(intelligence, 736);
  if (!node) return std::nullopt;
  uint32_t previous_depth = 0;
  bool first = true;
  for (size_t remaining = 128; *node && remaining; --remaining) {
    const auto flags = read.U32(*node, 8), type = read.U32(*node, 4);
    if (!flags || !type) return std::nullopt;
    const uint32_t depth = (*flags >> 28) & 7;
    if (!first && previous_depth < depth && previous_depth >= 2) return false;
    if (*type == 1054) return true;
    previous_depth = depth;
    first = false;
    node = read.U32(*node, 12);
    if (!node) return std::nullopt;
  }
  return *node ? std::nullopt : std::optional<bool>{false};
}

std::optional<bool> MeleeChain(const Reader& read, uint32_t task,
                             bool skip_inhibited) noexcept {
  for (size_t remaining = 128; task && remaining; --remaining) {
    const auto vtable = read.U32(task), flags = read.U32(task, 12);
    const auto next = read.U32(task, 8);
    if (!vtable || !flags || !next || !*vtable) return std::nullopt;
    const auto type_getter = read.U32(*vtable, 12);
    if (!type_getter) return std::nullopt;
    // sub_82737A08 is the generated side-effect-free getter returning 431;
    // CTaskComplexMelee's slot 3 points to it. No guest vfunc is executed.
    if ((!skip_inhibited || !(*flags & 0x80000000u)) &&
        *type_getter == 0x82737A08) return true;
    task = *next;
  }
  return task ? std::nullopt : std::optional<bool>{false};
}

std::optional<bool> InMelee(const Reader& read, uint32_t intelligence) noexcept {
  // sub_823C0E88 -> sub_82158F28: first occupied primary slot, the first
  // uninhibited extra slot selected by sub_82158ED8, then six secondary slots.
  constexpr std::array<uint32_t, 5> primary = {0, 4, 8, 12, 16};
  constexpr std::array<uint32_t, 3> extra = {44, 48, 52};
  constexpr std::array<uint32_t, 6> secondary = {20, 24, 28, 32, 36, 40};
  if (uint64_t{intelligence} + 68 > UINT32_MAX) return std::nullopt;
  const uint32_t manager = intelligence + 68;
  for (const uint32_t offset : primary) {
    const auto task = read.U32(manager, offset);
    if (!task) return std::nullopt;
    if (!*task) continue;
    const auto found = MeleeChain(read, *task, false);
    if (!found || *found) return found;
    break;
  }
  for (const uint32_t offset : extra) {
    const auto task = read.U32(manager, offset);
    if (!task) return std::nullopt;
    if (!*task) continue;
    const auto flags = read.U32(*task, 12);
    if (!flags) return std::nullopt;
    if (*flags & 0x80000000u) continue;
    const auto found = MeleeChain(read, *task, true);
    if (!found || *found) return found;
    break;
  }
  for (const uint32_t offset : secondary) {
    const auto task = read.U32(manager, offset);
    if (!task) return std::nullopt;
    const auto found = MeleeChain(read, *task, false);
    if (!found || *found) return found;
  }
  return false;
}

void ReadWeapon(const Reader& read, uint32_t ped, TouchContextSnapshot& out) noexcept {
  if (uint64_t{ped} + 640 > UINT32_MAX) return;
  const uint32_t manager = ped + 640;
  // sub_823D52D0's forward cycle predicate, with bounded type lookup from
  // sub_82299AD0. This is availability, not permission to bypass the decoder.
  out.inventory_known = true;
  for (uint32_t index = 0; index < out.weapons.size(); ++index) {
    auto& entry = out.weapons[index];
    entry.slot = index;
    const auto type = read.U32(manager, 36 + index * 8);
    const auto ammo = read.U32(manager, 40 + index * 8);
    if (!type || !ammo || *type >= 60) { out.inventory_known = false; continue; }
    entry.type = *type;
    entry.ammo = static_cast<uint16_t>(*ammo >> 16);
    entry.owned = index == 0 || *type != 0;
    if (!entry.owned) continue;
    // The native weapon type-name table, also used by the weapon data parser.
    // Episode entries deliberately retain their native EPISODIC_N identity.
    if (*type < 58) {
      const auto name = read.U32(0x82A993E8, *type * 4);
      bool terminated = false;
      if (name && *name) {
        for (size_t letter = 0; letter < entry.native_identifier.size(); ++letter) {
          const auto c = read.U8(*name, static_cast<uint32_t>(letter));
          if (!c || *c < 32 || *c > 126) { terminated = c && *c == 0; break; }
          entry.native_identifier[letter] = static_cast<char>(*c);
        }
      }
      if (!terminated) entry.native_identifier = {};
    }
    const auto fire_type = read.U32(0x82CB8AB0, *type * 272 + 12);
    if (!fire_type) { out.inventory_known = false; continue; }
    entry.selectable = index == 0 || entry.ammo != 0 || *fire_type == 1;
  }
  const auto slot = read.U32(manager), object = read.U32(manager, 20);
  if (!slot || !object || *slot > 10) return;
  out.weapon_slot = *slot;
  std::optional<uint32_t> weapon;
  if (*object) weapon = read.U32(*object, 600);
  else if (*slot == 0 || *slot == 10) weapon = read.U32(manager, 32);
  if (!weapon || !*weapon) return;
  const auto type = read.U32(*weapon, 20);
  const auto expected = read.U32(manager, 36 + *slot * 8);
  if (!type || !expected || *type != *expected) return;
  out.weapon_known = true;
  out.weapon_type = *type;
  // IS_CHAR_ARMED excludes these special/unarmed types. This says nothing
  // about firing permission, clip requirements, target lock or reload legality.
  out.armed = *type != 0 && *type != 46;
  if (uint64_t{*weapon} + 28 > UINT32_MAX) return;
  const auto clip = read.Scalar<uint16_t>(*weapon + 28);
  if (clip) { out.clip_known = true; out.clip_ammo = *clip; }
}

void ReadVehicleEntry(const Reader& read, uint32_t ped, TouchContextQueries queries,
                      TouchContextSnapshot& out) noexcept {
  if (!queries.vehicle_player_would_enter || !out.gameplay_allowed ||
      out.base_mode != ContextTouchMode::kOnFoot) return;
  // sub_8223ADB8 uses CPed +544 for the native preferred vehicle and rejects
  // flag 0x40000 at +568. Its script caller sub_825B6A10 passes the ped's
  // transform +16, matching the normal forward vector in sub_823CCBC8.
  const auto flags = read.U32(ped, 568), info = read.U32(ped, 544);
  const auto transform = read.U32(ped, 32);
  if (!flags || (*flags & 0x40000u) || !info || !*info || !transform || !*transform ||
      (*transform & 15u) || uint64_t{*transform} + 64 > uint64_t{UINT32_MAX} + 1) return;
  for (const uint32_t offset : {16u, 20u, 24u, 28u, 48u, 52u, 56u, 60u}) {
    const auto value = read.U32(*transform, offset);
    if (!value || !std::isfinite(std::bit_cast<float>(*value))) return;
  }
  // The game computes proximity, usable entry points and vehicle suitability.
  // Never reuse its previous scratch result or construct an entry task here.
  const uint32_t vehicle =
      queries.vehicle_player_would_enter(queries.opaque, ped, *transform + 16);
  if (!vehicle) return;
  const auto kind = read.U32(vehicle, 40), subtype = read.U32(vehicle, 4836);
  if (!kind || (*kind & 0x03C00000u) != 0x00800000u || !subtype) return;
  // sub_823C9A08 performs this extra gate between selection and entry-task
  // creation: subtype 3 must have bit 0x40 in its byte at +5316.
  if (*subtype == 3) {
    const auto flags = read.U8(vehicle, 5316);
    if (!flags || !(*flags & 0x40u)) return;
  }
  out.can_enter_vehicle = true;
}

std::mutex snapshot_mutex;
TouchContextSnapshot snapshot;
TouchContextGeneration generations;
std::mutex weapon_hud_mutex;
TouchWeaponHudSnapshot weapon_hud;
std::mutex prompt_mutex;
TouchVisiblePromptSnapshot prompt;

struct HelpDraw {
  TouchHelpObserver observer;
  uint32_t text_depth = 0;
};
thread_local std::array<HelpDraw, 8> help_draws;
thread_local size_t help_depth = 0;
HelpDraw* ActiveHelp() noexcept {
  return help_depth && help_depth <= help_draws.size() ? &help_draws[help_depth - 1] : nullptr;
}

struct HelpToken {
  std::array<char, 40> text{};
  size_t length = 0;
  std::optional<uint32_t> binding_action;
};
thread_local HelpToken* active_token = nullptr;

#if !defined(GTA4_TOUCH_CONTEXT_TEST)
bool RuntimeRead(void* opaque, uint32_t address, void* output, size_t size) {
  auto* base = static_cast<uint8_t*>(opaque);
  if (!base || !address || !size || uint64_t{address} + size > uint64_t{UINT32_MAX} + 1)
    return false;
  auto* kernel = REX_KERNEL_STATE();
  auto* memory = kernel ? kernel->memory() : nullptr;
  auto* heap = memory ? memory->LookupHeap(address) : nullptr;
  const auto last = static_cast<uint32_t>(uint64_t{address} + size - 1);
  if (!heap || heap != memory->LookupHeap(last)) return false;
  using rex::memory::PageAccess;
  const auto access = heap->QueryRangeAccess(address, last);
  if (access != PageAccess::kReadOnly && access != PageAccess::kExecuteReadOnly &&
      access != PageAccess::kReadWrite && access != PageAccess::kExecuteReadWrite) return false;
  std::memcpy(output, base + address + REX_PHYS_HOST_OFFSET(address), size);
  return true;
}
thread_local TouchContextSnapshot* capture_output = nullptr;
struct RuntimeVehicleQuery {
  PPCContext& parent;
  uint8_t* base;
};
struct ScopedVehicleQueryContext {
  PPCContext context;
  uint32_t host_csr;
  explicit ScopedVehicleQueryContext(PPCContext& parent)
      : context(parent), host_csr(parent.fpscr.getcsr()) {}
  ~ScopedVehicleQueryContext() { context.fpscr.setcsr(host_csr); }
};
uint32_t QueryVehicleEntry(void* opaque, uint32_t ped, uint32_t forward) {
  auto& query = *static_cast<RuntimeVehicleQuery*>(opaque);
  // Isolate all guest registers, including LR/CR/FPSCR. Reserve an ABI caller
  // frame so nested argument homes cannot overwrite the outer poll's frame.
  constexpr uint32_t kCallerFrame = 128;
  const uint32_t original_sp = query.parent.r1.u32;
  if (original_sp < kCallerFrame || (original_sp & 15u)) return 0;
  auto* kernel = REX_KERNEL_STATE();
  auto* memory = kernel ? kernel->memory() : nullptr;
  const uint32_t stack = original_sp - kCallerFrame;
  auto* heap = memory ? memory->LookupHeap(stack) : nullptr;
  if (!heap || heap != memory->LookupHeap(original_sp - 1)) return 0;
  const auto access = heap->QueryRangeAccess(stack, original_sp - 1);
  using rex::memory::PageAccess;
  if (access != PageAccess::kReadWrite && access != PageAccess::kExecuteReadWrite) return 0;
  ScopedVehicleQueryContext scope(query.parent);
  auto& nested = scope.context;
  nested.r1.u32 = stack;
  const uint32_t backchain = __builtin_bswap32(original_sp);
  std::memcpy(query.base + stack + REX_PHYS_HOST_OFFSET(stack), &backchain, sizeof(backchain));
  nested.r3.u64 = ped;
  nested.r4.u64 = forward;
  nested.r5.u64 = 0;
  __imp__sub_8223ADB8(nested, query.base);
  return nested.r3.u32;
}
void CaptureAliased(PPCContext& context, uint8_t* base) {
  auto& out = *capture_output;
  RuntimeVehicleQuery runtime_query{context, base};
  // This native spatial query is needed only for the visible touch layer.
  // Off and Auto-with-physical-input avoid adding world scans to ordinary play.
  const TouchContextQueries queries = rex::input::TouchControlsVisible()
      ? TouchContextQueries{&runtime_query, QueryVehicleEntry} : TouchContextQueries{};
  out = ReadTouchContextFacts({base, RuntimeRead}, out.frontend, out.map, queries);
}
#endif

}  // namespace

TouchContextSnapshot ReadTouchContextFacts(const TouchContextMemory& memory,
                                          bool frontend, bool map,
                                          TouchContextQueries queries) noexcept {
  TouchContextSnapshot out;
  out.frontend = frontend;
  out.map = map;
  out.native_input_allowed = frontend || map;
  const Reader read{memory};
  ReadPhone(read, out);
  const auto minigame = read.U32(kMinigame), cutscene = read.U32(kCutscene);
  const auto preparation = read.U32(kCutscenePreparation);
  const auto loading = read.U8(kLoadingActive);
  out.minigame_active = minigame && *minigame != 0;
  // sub_82144188 gates loading drawing on this byte; sub_82145770 clears
  // it after teardown. Read only that byte, not the adjacent ready/done flags.
  out.loading = !loading || *loading != 0;
  // sub_821E3788 tests the main state. sub_821EC8C8 also processes native
  // preparation when that state is zero, before playback has begun.
  out.cutscene = !cutscene || !preparation || *cutscene != 0 || *preparation != 0;
  const auto threshold = read.U8(0x82AA1B0F), alternate = read.U8(0x82FD1E3C);
  if (threshold && alternate) {
    out.aim_settings_known = true;
    out.aim_threshold = *threshold;
    out.alternate_aim_setting = *alternate != 0;
  }
  const auto index = read.U32(kPlayer), user = read.U32(kControl, 3412);
  if (user && *user < 4) { out.input_user = *user; out.input_user_known = true; }
  if (!index || *index >= 16 || !out.input_user_known) return out;
  const auto info = read.U32(kPlayerInfo, *index * 4);
  const auto generation = read.U32(kPlayerGeneration, *index * 4);
  if (!info || !*info || !generation) return out;
  const auto ped = read.U32(*info, 1400);
  if (!ped || !*ped) return out;
  const auto health = read.U32(*ped, 484), alive = read.U32(kAliveThreshold);
  const auto state = read.U32(*info, 1232), flags = read.U32(*info, 1200);
  const auto mode = read.U32(*ped, 2496), game_mode = read.U32(kGameMode);
  const auto vehicle_flags = read.U32(*ped, 572);
  if (!health || !alive || !state || !flags || !mode || !game_mode || !vehicle_flags) return out;
  out.valid = true;
  out.player_identity = *ped;
  out.player_generation = *generation;
  const float health_value = std::bit_cast<float>(*health), alive_value = std::bit_cast<float>(*alive);
  out.playing = *state == 2 && std::isfinite(health_value) && std::isfinite(alive_value) &&
                health_value >= alive_value;
  const uint32_t control_flags = *game_mode == 1 ? *flags & 0xFFFFFBFFu : *flags;
  out.gameplay_allowed = out.playing && !control_flags && *mode != 1 &&
                         !out.loading && !out.cutscene && !frontend && !map;
  out.native_input_allowed |= out.playing && !out.loading && !out.cutscene;
  out.base_mode = ContextTouchMode::kOnFoot;
  if (*vehicle_flags & 0x20000000u) {
    const auto vehicle = read.U32(*ped, 2688);
    const auto driver = vehicle && *vehicle ? read.U32(*vehicle, 3904) : std::nullopt;
    const auto vtable = vehicle && *vehicle ? read.U32(*vehicle) : std::nullopt;
    if (!vehicle || !*vehicle || !driver || !vtable) {
      out.gameplay_allowed = false;
      out.base_mode = ContextTouchMode::kVehicleDriverUnknown;
    } else {
      out.vehicle_identity = *vehicle;
      if (*driver != *ped) out.base_mode = ContextTouchMode::kVehiclePassenger;
      else if (*vtable == 0x8200B8D4) out.base_mode = ContextTouchMode::kVehicleHelicopter;
      else if (*vtable == 0x8204205C) out.base_mode = ContextTouchMode::kVehicleBike;
      else if (*vtable == 0x8204356C) out.base_mode = ContextTouchMode::kVehicleBoat;
      else if (*vtable == 0x82041D8C) out.base_mode = ContextTouchMode::kVehicleAutomobile;
      else out.base_mode = ContextTouchMode::kVehicleDriverUnknown;
    }
  }
  ReadWeapon(read, *ped, out);
  const auto intelligence = read.U32(*ped, 540);
  if (intelligence && *intelligence) {
    const auto cover = InCover(read, *intelligence), melee = InMelee(read, *intelligence);
    if (cover) { out.cover_known = true; out.in_cover = *cover; }
    if (melee) { out.melee_known = true; out.melee = *melee; }
  }
  ReadVehicleEntry(read, *ped, queries, out);
  const auto aim_base = read.U8(kControl, 2400), aim_current = read.U8(kControl, 2402);
  out.aiming = aim_base && aim_current && ((*aim_base ^ *aim_current) != 0);
  return out;
}

uint64_t TouchContextGeneration::Advance(const TouchContextSnapshot& value) noexcept {
  if (!generation_ || value.valid != previous_.valid || value.playing != previous_.playing ||
      value.player_identity != previous_.player_identity ||
      value.player_generation != previous_.player_generation ||
      value.vehicle_identity != previous_.vehicle_identity || value.base_mode != previous_.base_mode ||
      value.input_user != previous_.input_user || value.loading != previous_.loading ||
      value.cutscene != previous_.cutscene ||
      value.frontend != previous_.frontend || value.map != previous_.map ||
      value.minigame_active != previous_.minigame_active ||
      value.input_user_known != previous_.input_user_known ||
      value.native_input_allowed != previous_.native_input_allowed ||
      value.gameplay_allowed != previous_.gameplay_allowed) ++generation_;
  previous_ = value;
  return generation_;
}

void CaptureTouchContext(PPCContext& context, uint8_t* base, uint64_t epoch,
                         bool frontend, bool map) noexcept {
#if !defined(GTA4_TOUCH_CONTEXT_TEST)
  const uint64_t presentation_revision = ContextTouchPresentationRevision();
#else
  constexpr uint64_t presentation_revision = 0;
#endif
  TouchContextSnapshot next;
  next.frontend = frontend;
  next.map = map;
#if !defined(GTA4_TOUCH_CONTEXT_TEST)
  struct Scope {
    TouchContextSnapshot* previous = capture_output;
    explicit Scope(TouchContextSnapshot& out) { capture_output = &out; }
    ~Scope() { capture_output = previous; }
  } scope(next);
  PPCContext nested = context;
#if defined(GTA4_TOUCH_PRIMARY_PLAYER_ALIAS)
  GTA4_RunWithPrimaryPlayerInfoAlias(nested, base, CaptureAliased);
#else
  CaptureAliased(nested, base);
#endif
#else
  (void)context;
  (void)base;
#endif
  const auto hud = GetTouchWeaponHudSnapshot();
  uint64_t presentation_generation = 0;
#if !defined(GTA4_TOUCH_CONTEXT_TEST)
  rex::input::TouchPresentationState presentation;
  if (rex::input::GetTouchPresentationState(&presentation) && presentation.valid &&
      presentation.focused) presentation_generation = presentation.generation;
#endif
  std::lock_guard lock(snapshot_mutex);
  next.epoch = epoch;
  next.presentation_revision = presentation_revision;
  next.generation = generations.Advance(next);
  next.weapon_hud_bounds = ReadTouchWeaponHudBounds(hud, next, presentation_generation);
  snapshot = next;
}

TouchContextSnapshot GetTouchContextSnapshot() noexcept {
  std::lock_guard lock(snapshot_mutex);
  return snapshot;
}

void PublishTouchWeaponHudSnapshot(TouchWeaponHudSnapshot value) noexcept {
  std::lock_guard lock(weapon_hud_mutex);
  weapon_hud = value;
}

TouchWeaponHudSnapshot GetTouchWeaponHudSnapshot() noexcept {
  std::lock_guard lock(weapon_hud_mutex);
  return weapon_hud;
}

std::optional<ContextTouchHudBounds> ClipTouchWeaponHudBounds(
    double left, double top, double right, double bottom, uint32_t color) noexcept {
  if (!(color & 0xFF000000) || !std::isfinite(left) || !std::isfinite(top) ||
      !std::isfinite(right) || !std::isfinite(bottom) || right <= left || bottom <= top)
    return std::nullopt;
  ContextTouchHudBounds bounds{float(std::clamp(left, 0.0, 1.0)),
                               float(std::clamp(top, 0.0, 1.0)),
                               float(std::clamp(right, 0.0, 1.0)),
                               float(std::clamp(bottom, 0.0, 1.0))};
  if (bounds.right <= bounds.left || bounds.bottom <= bounds.top) return std::nullopt;
  return bounds;
}

std::optional<ContextTouchHudBounds> ReadTouchWeaponHudBounds(
    const TouchWeaponHudSnapshot& hud, const TouchContextSnapshot& context,
    uint64_t presentation_generation) noexcept {
  if (!context.valid || !context.playing || !context.gameplay_allowed ||
      !context.native_input_allowed || context.frontend || context.map || context.loading ||
      context.cutscene || !hud.bounds || !hud.generation ||
      hud.generation != context.generation || hud.presentation_revision != context.presentation_revision ||
      hud.weapon_type != context.weapon_type || hud.weapon_slot != context.weapon_slot ||
      !presentation_generation || hud.presentation_generation != presentation_generation ||
      context.epoch < hud.epoch || context.epoch - hud.epoch > 2) return std::nullopt;
  const auto& bounds = *hud.bounds;
  if (!std::isfinite(bounds.left) || !std::isfinite(bounds.top) ||
      !std::isfinite(bounds.right) || !std::isfinite(bounds.bottom) ||
      bounds.left < 0.0f || bounds.top < 0.0f || bounds.right > 1.0f || bounds.bottom > 1.0f ||
      bounds.right <= bounds.left || bounds.bottom <= bounds.top) return std::nullopt;
  return bounds;
}

std::optional<TouchScriptControl> DecodeTouchHelpToken(
    std::string_view token, uint32_t native_glyph,
    std::optional<uint32_t> binding_action) noexcept {
  if (native_glyph == 255 || token.size() >= 40) return std::nullopt;
  std::array<char, 40> uppercase{};
  for (size_t index = 0; index < token.size(); ++index) {
    const char c = token[index];
    uppercase[index] = c >= 'a' && c <= 'z' ? char(c - 'a' + 'A') : c;
  }
  token = std::string_view(uppercase.data(), token.size());
  if (token.starts_with("INPUT_") && binding_action && *binding_action < 86)
    return TouchScriptControl{TouchScriptQueryKind::kControlHeld, *binding_action};
  // sub_825D1308's raw pad indices; names and accepted glyphs are from
  // sub_821F2360. The left/right stick image families remain native sticks.
  struct RawToken { std::string_view text; uint32_t glyph; uint32_t button; };
  constexpr std::array raw_tokens = {
      RawToken{"PAD_LT", 289, 4}, RawToken{"PAD_RT", 291, 5},
      RawToken{"PAD_LB", 288, 6}, RawToken{"PAD_RB", 290, 7},
      RawToken{"PAD_UP", 256, 8}, RawToken{"PAD_DOWN", 257, 9},
      RawToken{"PAD_LEFT", 258, 10}, RawToken{"PAD_RIGHT", 259, 11},
      RawToken{"PAD_DPAD_UP", 260, 8}, RawToken{"PAD_DPAD_DOWN", 261, 9},
      RawToken{"PAD_DPAD_LEFT", 262, 10}, RawToken{"PAD_DPAD_RIGHT", 263, 11},
      RawToken{"PAD_START", 292, 12}, RawToken{"PAD_BACK", 293, 13},
      RawToken{"PAD_X", 286, 14}, RawToken{"PAD_Y", 287, 15},
      RawToken{"PAD_A", 284, 16}, RawToken{"PAD_B", 285, 17},
      // Retail resolves these aliases directly before its binding lookup.
      RawToken{"INPUT_PHONE_ACCEPT", 284, 16},
      RawToken{"INPUT_PHONE_CANCEL", 285, 17},
  };
  for (const auto& raw : raw_tokens)
    if (token == raw.text && native_glyph == raw.glyph)
      return TouchScriptControl{TouchScriptQueryKind::kRawButton, raw.button};
  if ((token.starts_with("PAD_LSTICK_") && native_glyph >= 268 && native_glyph <= 275) ||
      (token.starts_with("PAD_RSTICK_") && native_glyph >= 276 && native_glyph <= 283) ||
      (token == "PAD_RSTICK_ROTATE" && native_glyph == 296))
    return TouchScriptControl{TouchScriptQueryKind::kAnalogueSticks, 0};
  return std::nullopt;
}

void TouchHelpObserver::Begin(uint32_t help_object, uint64_t epoch, uint64_t generation) noexcept {
  snapshot_ = {.epoch = epoch, .generation = generation, .help_object = help_object};
  text_submitted_ = false;
}

void TouchHelpObserver::Text(bool submitted) noexcept {
  text_submitted_ |= submitted && snapshot_.help_object != 0;
}

void TouchHelpObserver::Token(const TouchScriptControl& value) noexcept {
  if (!text_submitted_) return;
  auto control = CanonicalTouchScriptControl(value);
  control.input_group = 0;
  control.script_thread = 0;
  control.generation = snapshot_.generation;
  for (size_t index = 0; index < snapshot_.control_count; ++index)
    if (snapshot_.controls[index].kind == control.kind &&
        snapshot_.controls[index].action == control.action) return;
  if (snapshot_.control_count < snapshot_.controls.size())
    snapshot_.controls[snapshot_.control_count++] = control;
}

TouchVisiblePromptSnapshot TouchHelpObserver::Finish() noexcept {
  snapshot_.visible = text_submitted_;
  return snapshot_;
}

TouchVisiblePromptSnapshot GetTouchVisiblePromptSnapshot(uint64_t epoch,
                                                        uint64_t generation) noexcept {
  std::lock_guard lock(prompt_mutex);
  // Help must have actually submitted in this poll or the two preceding polls.
  // The bounded allowance covers draw/input ordering without retaining a prompt
  // across a world transition or an indefinitely running background script.
  if (!prompt.visible || prompt.generation != generation || epoch < prompt.epoch ||
      epoch - prompt.epoch > 2) return {};
  return prompt;
}

uint32_t ReadTouchScriptThread(const TouchContextMemory& memory) noexcept {
  const Reader read{memory};
  const auto thread = read.U32(0x8319277C);
  if (!thread || !*thread) return 0;
  const auto identity = read.U32(*thread, 4), state = read.U32(*thread, 12);
  // GET_ID_OF_THIS_THREAD (sub_825885B0) returns +4. sub_828458C8
  // assigns the incremented 0x83192778 serial on each allocation, and
  // sub_82845AB8 clears it on destruction. IS_THREAD_ACTIVE rejects state 2.
  return identity && state && *state != 2 ? *identity : 0;
}

uint32_t ReadTouchScriptThread(uint8_t* base) noexcept {
#if !defined(GTA4_TOUCH_CONTEXT_TEST)
  return ReadTouchScriptThread(TouchContextMemory{base, RuntimeRead});
#else
  (void)base;
  return 0;
#endif
}

std::optional<uint32_t> ReadTouchParachuteState(const TouchContextMemory& memory) noexcept {
  if (!ReadTouchScriptThread(memory)) return std::nullopt;
  const Reader read{memory};
  const auto thread = read.U32(0x8319277C);
  if (!thread || !*thread) return std::nullopt;
  const auto program = read.U32(*thread, 8), globals = read.U32(0x831927B4);
  // Same compiled parachute_player program and shared-global slot verified
  // by the existing SDK input hooks. A background script never inherits it.
  if (!program || *program != 0x98751695 || !globals || !*globals) return std::nullopt;
  return read.U32(*globals, 0x2A18);
}

std::optional<uint32_t> ReadTouchParachuteState(uint8_t* base) noexcept {
#if !defined(GTA4_TOUCH_CONTEXT_TEST)
  return ReadTouchParachuteState(TouchContextMemory{base, RuntimeRead});
#else
  (void)base;
  return std::nullopt;
#endif
}

void BeginTouchHelpDraw(uint8_t* base, uint32_t help_object) noexcept {
  ++help_depth;
  if (auto* draw = ActiveHelp()) {
    *draw = {};
    const auto current = GetTouchContextSnapshot();
    draw->observer.Begin(base ? help_object : 0, current.epoch, current.generation);
  }
}

void ObserveTouchHelpText(uint8_t* base, uint32_t text) noexcept {
  if (auto* draw = ActiveHelp()) {
    ++draw->text_depth;
#if !defined(GTA4_TOUCH_CONTEXT_TEST)
    const Reader read{{base, RuntimeRead}};
    const auto first = read.U8(text);
    draw->observer.Text(first && *first != 0);
#else
    draw->observer.Text(base && text);
#endif
  }
}

void EndTouchHelpText() noexcept {
  if (auto* draw = ActiveHelp(); draw && draw->text_depth) --draw->text_depth;
}

void EndTouchHelpDraw() noexcept {
  if (auto* draw = ActiveHelp()) {
    const auto finished = draw->observer.Finish();
    std::lock_guard lock(prompt_mutex);
    // Retail calls this draw with distinct pass flags. A pass rejected after
    // another pass submitted text must not erase that poll's visible prompt.
    if (finished.visible || finished.epoch != prompt.epoch ||
        finished.generation != prompt.generation) prompt = finished;
  }
  if (help_depth) --help_depth;
}

void ResolveTouchHelpToken(PPCContext& context, uint8_t* base,
                           void (*original)(PPCContext&, uint8_t*)) {
#if !defined(GTA4_TOUCH_CONTEXT_TEST)
  auto* draw = ActiveHelp();
  if (!draw || !draw->text_depth) { original(context, base); return; }
  HelpToken token;
  const Reader read{{base, RuntimeRead}};
  bool terminated = false;
  for (; token.length < token.text.size(); ++token.length) {
    const auto character = read.U8(context.r3.u32, static_cast<uint32_t>(token.length));
    if (!character || !*character) break;
    if (*character == '~') { terminated = true; break; }
    token.text[token.length] = static_cast<char>(*character);
  }
  if (!terminated) { original(context, base); return; }
  struct Scope {
    HelpToken* previous = active_token;
    explicit Scope(HelpToken& token) { active_token = &token; }
    ~Scope() { active_token = previous; }
  } scope(token);
  original(context, base);
  const auto control = DecodeTouchHelpToken(
      {token.text.data(), token.length}, context.r3.u32, token.binding_action);
  if (control) draw->observer.Token(*control);
#else
  original(context, base);
#endif
}

void ResolveTouchHelpBinding(PPCContext& context, uint8_t* base,
                             void (*original)(PPCContext&, uint8_t*)) {
#if !defined(GTA4_TOUCH_CONTEXT_TEST)
  const uint32_t action = context.r5.u32;
  const uint32_t caller = static_cast<uint32_t>(context.lr);
  original(context, base);
  if (active_token && caller == 0x821F2CEC && context.r3.u8 && action < 86)
    active_token->binding_action = action;
#else
  original(context, base);
#endif
}

}  // namespace gta4::input
