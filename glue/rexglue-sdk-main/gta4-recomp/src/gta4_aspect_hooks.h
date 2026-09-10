#pragma once
#include "gta4_aspect_policy.h"
#include <array>
#include <cstdint>
#include <optional>

struct PPCContext;
namespace gta4::aspect {
using GuestFunction = void (*)(PPCContext&, uint8_t*);
enum class UiRole {
  kNone,
  kFixed,
  kMenuBody,
  kMenuFooter,
  kRadar,
  kHelp,
  kComponent,
  kLoadingLabel
};
struct UiContext {
  Transform transform{};
  Extent render{};
  UiRole role = UiRole::kNone;
  bool active = false;
  uint64_t generation = 0;
};
void Publish(Extent render, Extent output);
inline void PublishOutput(Extent render, Extent output) {
  Publish(render, output);
}
UiContext CurrentUi(uint8_t* base);
// Labels, sliders and hit regions share the current menu's authored divider.
UiContext MenuBodyUi(uint8_t* base);
UiContext TextUi(const PPCContext& context, uint8_t* base);
class Scope {
 public:
  explicit Scope(UiRole role, Point anchor = {0.5, 0.5});
  explicit Scope(UiContext context);
  ~Scope();
  Scope(const Scope&) = delete;
  Scope& operator=(const Scope&) = delete;

 private:
  UiContext previous_;
};
class DcScope {
 public:
  DcScope(PPCContext& context, uint8_t* base);

 private:
  Scope scope_;
};
void FinalizeDc(uint8_t* base, uint32_t dc);
void PrepareViewport(PPCContext& context, uint8_t* base);
void DrawQuad(PPCContext& context, uint8_t* base, GuestFunction original, bool textured);
void DrawRadarSection(PPCContext& context, uint8_t* base, GuestFunction original);
void DrawWindow(PPCContext& context, uint8_t* base, GuestFunction original);
// Inverse-layout adjustment lets the two retail columns use additional horizontal room.
class FrontendLayoutScope {
 public:
  FrontendLayoutScope(PPCContext& context, uint8_t* base);
  ~FrontendLayoutScope();
  FrontendLayoutScope(const FrontendLayoutScope&) = delete;

 private:
  uint8_t* base_ = nullptr;
  std::array<uint32_t, 2> addresses_{}, originals_{}, replacements_{};
};
}  // namespace gta4::aspect
