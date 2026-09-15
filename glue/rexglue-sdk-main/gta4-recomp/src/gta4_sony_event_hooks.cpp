#include "gta4_sony_event_hooks.h"

#include <bit>
#include <limits>

#include <rex/memory.h>
#include <rex/runtime.h>

#include "gta4_init.h"
#include "gta4_sony_feedback.h"

namespace {

using rex::input::sony::Event;

// All offsets below come from generated gta4_recomp.31.cpp:824DC670,
// 824DB3E0 and 824DB650. These are borrowed synchronous arguments, never queued.
constexpr uint32_t kPedHealthOffset = 484;
constexpr uint32_t kPedArmorOffset = 2520;
constexpr uint32_t kResponseFlagsOffset = 4;

bool Readable(uint8_t* base, uint32_t address, uint32_t offset, uint32_t bytes,
              uint32_t& field) {
  if (!base || !address || !bytes) return false;
  const uint64_t begin = static_cast<uint64_t>(address) + offset;
  const uint64_t end = begin + bytes - 1;
  if (end > std::numeric_limits<uint32_t>::max()) return false;
  auto* runtime = rex::Runtime::instance();
  auto* memory = runtime ? runtime->memory() : nullptr;
  auto* heap = memory ? memory->LookupHeap(static_cast<uint32_t>(begin)) : nullptr;
  if (!heap || heap != memory->LookupHeap(static_cast<uint32_t>(end)) ||
      heap->QueryRangeAccess(static_cast<uint32_t>(begin), static_cast<uint32_t>(end)) ==
          rex::memory::PageAccess::kNoAccess) {
    return false;
  }
  field = static_cast<uint32_t>(begin);
  return true;
}

gta4::sony::DamageState ReadDamage(uint8_t* base, uint32_t ped, uint32_t response) {
  uint32_t health = 0, armor = 0, flags = 0;
  if (!Readable(base, ped, kPedHealthOffset, sizeof(uint32_t), health) ||
      !Readable(base, ped, kPedArmorOffset, sizeof(uint32_t), armor) ||
      !Readable(base, response, kResponseFlagsOffset, sizeof(uint8_t), flags)) {
    return {};
  }
  return {.valid = true,
          .health = std::bit_cast<float>(REX_LOAD_U32(health)),
          .armor = std::bit_cast<float>(REX_LOAD_U32(armor)),
          .response_flags = REX_LOAD_U8(flags)};
}

bool StillControlled(PPCContext& ctx, uint8_t* base, const GTA4SonyLocalPlayer& before) {
  GTA4SonyLocalPlayer after{};
  return GTA4_SonyReadLocalPlayer(ctx, base, after) && after.state.active &&
         after.user == before.user && after.generation == before.generation &&
         after.ped == before.ped;
}

bool ReadExplosionPosition(uint8_t* base, uint32_t address,
                           std::array<float, 3>& position) {
  // The original uses lvx128 at r7, which aligns down to its vector boundary.
  constexpr uint32_t kVectorAddressMask = 0xFFFFFFF0;
  uint32_t vector = 0;
  if (!Readable(base, address & kVectorAddressMask, 0, 12, vector)) return false;
  position = {std::bit_cast<float>(REX_LOAD_U32(vector)),
              std::bit_cast<float>(REX_LOAD_U32(vector + 4)),
              std::bit_cast<float>(REX_LOAD_U32(vector + 8))};
  return true;
}

}  // namespace

void GTA4_SonyObserveDamage(PPCContext& ctx, uint8_t* base) {
  const uint32_t victim = ctx.r4.u32;
  const uint32_t response = ctx.r5.u32;
  GTA4SonyLocalPlayer player{};
  const bool local = GTA4_SonyReadLocalPlayer(ctx, base, player) &&
                     player.state.active && victim != 0 && victim == player.ped;
  const gta4::sony::DamageState before = local ? ReadDamage(base, victim, response)
                                              : gta4::sony::DamageState{};

  __imp__sub_824DC670(ctx, base);

  if (!before.valid) return;
  const float strength =
      gta4::sony::AppliedDamageStrength(before, ReadDamage(base, victim, response));
  // Keep the validated pre-hit identity for a lethal hit. Requiring an alive
  // player after the original would discard that real damage. Emit validates
  // the same device generation and active publication lease; EndPoll handles
  // the subsequent death/context reset.
  if (strength > 0) {
    GTA4_SonyEmit(player, Event::kDamage, strength);
  }
}

// Consumer builds compose the observer inside gta4_multiplayer_64_hooks.cpp's
// primary-player alias wrapper. Legacy desktop adapters have no such module.
#if !defined(GTA4_SONY_PRIMARY_PLAYER_ALIAS)
extern "C" void sub_824DC670(PPCContext& ctx, uint8_t* base) {
  GTA4_SonyObserveDamage(ctx, base);
}
#endif

extern "C" void sub_8226F428(PPCContext& ctx, uint8_t* base) {
  const uint32_t weapon = ctx.r3.u32;
  const uint32_t owner = ctx.r4.u32;
  GTA4SonyLocalPlayer player{};
  const bool local = GTA4_SonyReadLocalPlayer(ctx, base, player) && player.state.active &&
                     owner != 0 && owner == player.ped;

  __imp__sub_8226F428(ctx, base);

  const uint8_t result = ctx.r3.u8;
  if (local && gta4::sony::SuccessfulLocalShot(weapon, owner, player.ped, result) &&
      StillControlled(ctx, base, player)) {
    GTA4_SonyEmit(player, Event::kShot, 1.0f);
  }
}

extern "C" void sub_822343C0(PPCContext& ctx, uint8_t* base) {
  const uint32_t position_address = ctx.r7.u32;
  GTA4SonyLocalPlayer player{};
  std::array<float, 3> position{};
  const bool observable = GTA4_SonyReadLocalPlayer(ctx, base, player) &&
                          player.state.active && player.position_valid &&
                          ReadExplosionPosition(base, position_address, position);

  __imp__sub_822343C0(ctx, base);

  const uint32_t result = ctx.r3.u32;
  if (!observable) return;
  const float strength = gta4::sony::ExplosionStrength(player.position, position, result);
  if (strength > 0 && StillControlled(ctx, base, player)) {
    GTA4_SonyEmit(player, Event::kExplosion, strength);
  }
}
