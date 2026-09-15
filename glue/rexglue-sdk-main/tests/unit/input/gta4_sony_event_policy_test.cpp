#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <limits>

#include "gta4_sony_event_hooks.h"

namespace gta4::sony {
namespace {

DamageState Before(float health = 100, float armor = 50) {
  return {.valid = true, .health = health, .armor = armor, .response_flags = 0};
}

DamageState Applied(float health, float armor) {
  return {.valid = true, .health = health, .armor = armor, .response_flags = 0xC0};
}

}  // namespace

TEST_CASE("Sony damage feedback uses applied health and armor losses",
          "[input][sony][gta4][events]") {
  CHECK(AppliedDamageStrength(Before(), Applied(75, 50)) == Catch::Approx(0.25f));
  CHECK(AppliedDamageStrength(Before(), Applied(100, 25)) == Catch::Approx(0.25f));
  CHECK(AppliedDamageStrength(Before(), Applied(75, 25)) == Catch::Approx(0.5f));
}

TEST_CASE("Sony damage feedback ignores invulnerability healing and unchanged health",
          "[input][sony][gta4][events]") {
  CHECK(AppliedDamageStrength(Before(), Applied(100, 50)) == 0);
  CHECK(AppliedDamageStrength(Before(), Applied(125, 75)) == 0);
  CHECK(AppliedDamageStrength(Before(), Before(50, 0)) == 0);
}

TEST_CASE("Sony damage feedback rejects hypothetical and already processed responses",
          "[input][sony][gta4][events]") {
  auto calculate_only = Before();
  calculate_only.response_flags = 0x02;
  CHECK(AppliedDamageStrength(calculate_only, Applied(0, 0)) == 0);
  auto calculated = Applied(0, 0);
  calculated.response_flags = 0xC2;
  CHECK(AppliedDamageStrength(Before(), calculated) == 0);
  CHECK(AppliedDamageStrength(Applied(100, 50), Applied(0, 0)) == 0);
  auto rejected = Applied(0, 0);
  rejected.response_flags = 0x80;
  CHECK(AppliedDamageStrength(Before(), rejected) == 0);
}

TEST_CASE("Sony damage feedback has a bounded envelope and clamps overkill",
          "[input][sony][gta4][events]") {
  CHECK(AppliedDamageStrength(Before(100, 0), Applied(99, 0)) == Catch::Approx(0.1f));
  CHECK(AppliedDamageStrength(Before(25, 0), Applied(-100, 0)) == Catch::Approx(0.25f));
  CHECK(AppliedDamageStrength(Before(100, 100), Applied(0, 0)) == 1);
  CHECK(AppliedDamageStrength(Before(0, 100), Applied(-100, 0)) == 0);
}

TEST_CASE("Sony damage feedback rejects missing and invalid guest samples",
          "[input][sony][gta4][events]") {
  CHECK(AppliedDamageStrength({}, Applied(0, 0)) == 0);
  CHECK(AppliedDamageStrength(Before(), {}) == 0);
  CHECK(AppliedDamageStrength(Before(-1, 50), Applied(0, 0)) == 0);
  CHECK(AppliedDamageStrength(Before(100, -1), Applied(0, 0)) == 0);
  CHECK(AppliedDamageStrength(Before(), Applied(std::numeric_limits<float>::quiet_NaN(), 0)) ==
        0);
  CHECK(AppliedDamageStrength(Before(), Applied(0, std::numeric_limits<float>::infinity())) ==
        0);
}

TEST_CASE("Sony gunfire feedback requires the successful controlled owner's weapon",
          "[input][sony][gta4][events]") {
  CHECK(SuccessfulLocalShot(0x1000, 0x2000, 0x2000, 1));
  CHECK_FALSE(SuccessfulLocalShot(0x1000, 0x2000, 0x2000, 0));
  CHECK_FALSE(SuccessfulLocalShot(0x1000, 0x3000, 0x2000, 1));
  CHECK_FALSE(SuccessfulLocalShot(0, 0x2000, 0x2000, 1));
  CHECK_FALSE(SuccessfulLocalShot(0x1000, 0, 0, 1));
}

TEST_CASE("Sony explosions require local creation success and attenuate with distance",
          "[input][sony][gta4][events]") {
  const std::array<float, 3> origin{};
  CHECK(ExplosionStrength(origin, origin, 0) == 0);  // Duplicate or failed creation.
  CHECK(ExplosionStrength(origin, origin, 2) == 0);
  CHECK(ExplosionStrength(origin, origin, 1) == 1);
  CHECK(ExplosionStrength(origin, {25, 0, 0}, 1) == Catch::Approx(0.75f));
  CHECK(ExplosionStrength(origin, {50, 0, 0}, 1) == 0);
  CHECK(ExplosionStrength(origin, {51, 0, 0}, 1) == 0);
}

TEST_CASE("Sony explosion distance uses each world axis and tolerates large coordinates",
          "[input][sony][gta4][events]") {
  // Expected values were calculated with Python, independently of the policy.
  CHECK(ExplosionStrength({0, 0, 0}, {3, 4, 12}, 1) == Catch::Approx(0.9324f));
  CHECK(ExplosionStrength({100, 200, 300}, {103, 204, 312}, 1) ==
        Catch::Approx(0.9324f));
  CHECK(ExplosionStrength({0, 0, 0}, {0, 0, 25}, 1) == Catch::Approx(0.75f));
  const float largest = std::numeric_limits<float>::max();
  CHECK(ExplosionStrength({largest, 0, 0}, {-largest, 0, 0}, 1) == 0);
  CHECK(ExplosionStrength({0, 0, 0}, {std::numeric_limits<float>::quiet_NaN(), 0, 0}, 1) ==
        0);
}

}  // namespace gta4::sony
