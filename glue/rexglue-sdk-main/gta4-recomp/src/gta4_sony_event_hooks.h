#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>

struct PPCContext;

// Called within the existing primary-player alias scope on the consumer host.
// This function invokes __imp__sub_824DC670 exactly once.
void GTA4_SonyObserveDamage(PPCContext& ctx, uint8_t* base);

namespace gta4::sony {

struct DamageState {
  bool valid = false;
  float health = 0;
  float armor = 0;
  uint8_t response_flags = 0;
};

// Retail 824DC670 marks a response processed (0x80) and accepted (0x40).
// Retail 824DB650 tests 0x02 before each health mutation: it is a calculation
// without application. Losses written to the response are therefore not enough.
inline float AppliedDamageStrength(const DamageState& before,
                                   const DamageState& after) {
  constexpr uint8_t kProcessed = 0x80;
  constexpr uint8_t kAccepted = 0x40;
  constexpr uint8_t kCalculateOnly = 0x02;
  if (!before.valid || !after.valid ||
      (before.response_flags & (kProcessed | kCalculateOnly)) ||
      (after.response_flags & kCalculateOnly) ||
      (after.response_flags & (kProcessed | kAccepted)) != (kProcessed | kAccepted) ||
      !std::isfinite(before.health) || !std::isfinite(after.health) ||
      !std::isfinite(before.armor) || !std::isfinite(after.armor) ||
      before.health <= 0 || before.armor < 0) {
    return 0;
  }
  const double health_loss = std::max(
      0.0, static_cast<double>(before.health) - std::max(0.0, double(after.health)));
  const double armor_loss = std::max(
      0.0, static_cast<double>(before.armor) - std::max(0.0, double(after.armor)));
  const double applied_loss = health_loss + armor_loss;
  if (applied_loss <= 0) return 0;
  // An authored feedback envelope, not a change to damage or maximum health.
  return static_cast<float>(std::clamp(applied_loss / 100.0, 0.1, 1.0));
}

inline bool SuccessfulLocalShot(uint32_t weapon, uint32_t owner, uint32_t local_ped,
                                uint8_t result) {
  return weapon != 0 && local_ped != 0 && owner == local_ped && result != 0;
}

inline float ExplosionStrength(const std::array<float, 3>& player,
                               const std::array<float, 3>& explosion,
                               uint32_t result) {
  // 822343C0 returns zero for a nearby duplicate, failed allocation, failed
  // configuration, or a network request that has not created a local explosion.
  if (result != 1) return 0;
  double distance_squared = 0;
  for (std::size_t axis = 0; axis < player.size(); ++axis) {
    if (!std::isfinite(player[axis]) || !std::isfinite(explosion[axis])) return 0;
    const double delta = double(player[axis]) - double(explosion[axis]);
    distance_squared += delta * delta;
  }
  // Authored haptic range of 50 world units; 2500 is its Python-checked square.
  // The function's f1 is damage scale, not a radius, and is deliberately unused.
  return static_cast<float>(std::clamp(1.0 - distance_squared / 2500.0, 0.0, 1.0));
}

}  // namespace gta4::sony
