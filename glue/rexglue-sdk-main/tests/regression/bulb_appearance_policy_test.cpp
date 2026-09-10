// Standalone diagnostic-policy regression. No title state or graphics device.
#include "graphics/gta4_native/native_bulb_appearance.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <limits>
using namespace rex::graphics::gta4_native;
static size_t checks = 0;
static void Check(bool result, const char* name) {
  if (!result) { std::cerr << "FAILED: " << name << '\n'; std::abort(); }
  ++checks;
}
static void Store(std::vector<uint8_t>& bytes, size_t at, float value) {
  const auto bits = std::bit_cast<uint32_t>(value);
  bytes.at(at) = uint8_t(bits >> 24); bytes.at(at + 1) = uint8_t(bits >> 16);
  bytes.at(at + 2) = uint8_t(bits >> 8); bytes.at(at + 3) = uint8_t(bits);
}
int main() {
  for (size_t i = 0; i < kBulbFixtures.size(); ++i) {
    const auto& f = kBulbFixtures[i];
    Check(FindBulbFixture(f.vertex_hash, f.vertex_bytes, f.indices) == int(i), "exact fixture");
    Check(FindBulbFixture(f.vertex_hash, f.vertex_bytes, f.indices + 1) == -1, "reject wrong indices");
    Check(FindBulbFixture(f.vertex_hash, f.vertex_bytes + 1, f.indices) == -1, "reject wrong bytes");
    Check(FindBulbFixture(f.vertex_hash ^ 1, f.vertex_bytes, f.indices) == -1, "reject wrong geometry");
  }
  std::vector<uint8_t> pc(4096);
  Store(pc, 208 * 16, 20); Store(pc, 45 * 16 + 12, 1);
  Store(pc, 39 * 16, 1); Store(pc, 46 * 16, 2);
  for (size_t c = 0; c < 4; ++c) Store(pc, 51 * 16 + c * 4, 1);
  const auto original = pc;
  constexpr uint64_t regular = 0xF634FCEBA607E8A5ull, night = 0x07675BF8EF5E48DCull;
  Check(InspectBulbEmission(night, pc).state == "emission-on", "night positive input");
  Check(InspectBulbEmission(regular, pc).state == "emission-on", "regular positive input");
  Check(InspectBulbEmission(0, pc).state == "unknown-shader-contract", "never guess unknown shader");
  Check(!InspectBulbEmission(night, std::span<const uint8_t>{}).known, "missing constants remain unknown");
  Check(pc == original, "inspection did not mutate inputs");
  Store(pc, 45 * 16 + 12, 0);
  Check(InspectBulbEmission(night, pc).state == "off-night-gate", "night gate zero");
  Check(InspectBulbEmission(regular, pc).state == "emission-on", "regular ignores night gate");
  pc = original; Store(pc, 208 * 16, 0);
  Check(InspectBulbEmission(night, pc).state == "off-material-multiplier", "zero material multiplier");
  pc = original; for (size_t c = 0; c < 3; ++c) Store(pc, 51 * 16 + c * 4, 0);
  Check(InspectBulbEmission(night, pc).state == "off-color-tint", "zero tint");
  pc = original; Store(pc, 51 * 16 + 12, 0);
  Check(InspectBulbEmission(night, pc).state == "emission-on-zero-alpha-input", "alpha distinguished from RGB emission");
  pc = original; Store(pc, 45 * 16 + 12, std::numeric_limits<float>::quiet_NaN());
  Check(!InspectBulbEmission(night, pc).known, "NaN gate unknown");
  pc = original; Store(pc, 208 * 16, std::numeric_limits<float>::infinity());
  Check(!InspectBulbEmission(regular, pc).known, "infinite multiplier unknown");
  std::vector<uint8_t> vc(4096), vertices(24);
  for (size_t i = 0; i < 4; ++i) Store(vc, (8 + i) * 16 + i * 4, 1);
  Store(vc, 44 * 16, 1280); Store(vc, 44 * 16 + 4, 720);
  Store(vc, 44 * 16 + 8, 1.0f / 1280); Store(vc, 44 * 16 + 12, 1.0f / 720);
  Store(vertices, 0, 0); Store(vertices, 4, 0); Store(vertices, 8, 0.5f);
  const auto center = InspectBulbScreenRegion(vertices, 0, 24, vc);
  Check(center.known && center.visible, "center projected");
  Check(center.normalized[0] < center.normalized[2] && center.normalized[1] < center.normalized[3], "valid region extent");
  Store(vertices, 0, 4);
  const auto outside = InspectBulbScreenRegion(vertices, 0, 24, vc);
  Check(outside.known && !outside.visible, "offscreen is not switched-off");
  Store(vertices, 0, 0); Store(vc, 11 * 16 + 12, -1);
  Check(!InspectBulbScreenRegion(vertices, 0, 24, vc).known, "behind-camera projection unproven");
  Store(vc, 11 * 16 + 12, 1);
  Check(!InspectBulbScreenRegion(vertices, 0, 0, vc).known, "invalid stride");
  Check(!InspectBulbScreenRegion(vertices, vertices.size(), 24, vc).known, "invalid offset");
  Check(!InspectBulbScreenRegion(vertices, 0, 24, {}).known, "missing transform");
  Store(vc, 44 * 16 + 8, std::numeric_limits<float>::infinity());
  Check(!InspectBulbScreenRegion(vertices, 0, 24, vc).known, "nonfinite projection correction remains unknown");
  std::cout << "BULB_APPEARANCE_POLICY_PASS checks=" << checks << '\n';
}
