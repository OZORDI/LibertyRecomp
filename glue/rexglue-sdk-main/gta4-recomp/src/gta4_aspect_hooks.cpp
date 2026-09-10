#include "gta4_aspect_hooks.h"

#include <atomic>
#include <bit>
#include <cstring>
#include <deque>
#include <limits>
#include <mutex>
#include <string>
#include <unordered_map>

#include <rex/cvar.h>
#include <rex/diagnostics/policy.h>
#include <rex/runtime.h>
#include "gta4_init.h"

REXCVAR_DEFINE_BOOL(gta4_trace_aspect, false, "GTA IV/Diagnostics",
                    "Bounded camera, UI-layout, and fixed-artwork aspect observations");

namespace gta4::aspect {
namespace {
struct OutputState {
  Extent render, output;
  bool ready = false;
  uint64_t generation = 0;
};
std::mutex output_mutex;
OutputState output_state;
thread_local UiContext ui_context;
thread_local unsigned baked_font_depth = 0;
thread_local unsigned append_depth = 0;
thread_local Point append_origin;
thread_local Transform append_transform;
thread_local uint32_t phone_projection_build = 0;
struct Emission {
  Transform transform;
  bool active = false;
  bool clip_art = false;
  Rect art_quad{}, art_clip{};
};
thread_local Emission emission;
std::atomic<uint32_t> trace_count{0};
std::atomic<uint32_t> loading_label_trace_count{0};

bool Span(uint8_t* base, uint32_t address, size_t size, bool write = false) {
  if (!base || !address || !size || uint64_t(address) + size > uint64_t(UINT32_MAX) + 1)
    return false;
  auto* kernel = REX_KERNEL_STATE();
  auto* memory = kernel ? kernel->memory() : nullptr;
  auto* heap = memory ? memory->LookupHeap(address) : nullptr;
  const uint32_t last = uint32_t(uint64_t(address) + size - 1);
  if (!heap || heap != memory->LookupHeap(last))
    return false;
  const auto access = heap->QueryRangeAccess(address, last);
  using rex::memory::PageAccess;
  return access == PageAccess::kReadWrite || access == PageAccess::kExecuteReadWrite ||
         (!write && (access == PageAccess::kReadOnly || access == PageAccess::kExecuteReadOnly));
}
uint32_t Read(uint8_t* base, uint32_t address) {
  uint32_t bits;
  std::memcpy(&bits, rex::memory::GuestPtr(base, address), sizeof(bits));
  return __builtin_bswap32(bits);
}
void Write(uint8_t* base, uint32_t address, uint32_t value) {
  value = __builtin_bswap32(value);
  std::memcpy(rex::memory::GuestPtr(base, address), &value, sizeof(value));
}
float Float(uint8_t* base, uint32_t address) {
  return std::bit_cast<float>(Read(base, address));
}
void Float(uint8_t* base, uint32_t address, double value) {
  Write(base, address, std::bit_cast<uint32_t>(float(value)));
}
OutputState Output() {
  std::lock_guard lock(output_mutex);
  return output_state;
}
bool Trace() {
  return REXCVAR_GET(gta4_trace_aspect) &&
         rex::diagnostics::IsEnabled(rex::diagnostics::Category::kLogging) &&
         trace_count.fetch_add(1, std::memory_order_relaxed) < 96;
}
// A separate bounded budget keeps loading-mask evidence available after splash
// artwork has consumed the general aspect trace budget.
bool TraceLoadingLabel(const UiContext& layout) {
  return layout.active && layout.role == UiRole::kLoadingLabel && REXCVAR_GET(gta4_trace_aspect) &&
         rex::diagnostics::IsEnabled(rex::diagnostics::Category::kLogging) &&
         loading_label_trace_count.fetch_add(1, std::memory_order_relaxed) < 32;
}
void TraceLoadingMask(const UiContext& layout, const Rect& source, const char* kind) {
  if (!TraceLoadingLabel(layout))
    return;
  const Rect mapped = layout.transform.Pixels(layout.render).Map(source);
  REXLOG_INFO(
      "gta4-aspect-loading: {} pixels source={},{},{},{} mapped={},{},{},{} "
      "scale={},{} anchor-offset={},{}",
      kind, source.left, source.top, source.right, source.bottom, mapped.left, mapped.top,
      mapped.right, mapped.bottom, layout.transform.sx, layout.transform.sy, layout.transform.ox,
      layout.transform.oy);
}
constexpr uint32_t kCurrentViewport = 0x831C2200;
constexpr uint32_t kStartupViewport = 0x831C21F4;
constexpr uint32_t kFontStateIndex = 0x82A935A4;
constexpr uint32_t kFontStates = 0x82B9A118;
// The exact address of the HUD object table is generated below from its PPC lis/addi.
constexpr uint32_t kHudTable = 0x82B39990;

uint32_t Owner(uint8_t* base, uint32_t viewport) {
  if (viewport < 16 || !Span(base, viewport - 16, 16))
    return 0;
  return Read(base, viewport - 16);
}
bool DisplayViewport(uint8_t* base, uint32_t viewport, const OutputState& state) {
  if (!state.ready || !Span(base, viewport, 1000))
    return false;
  const Extent size{Read(base, viewport + 688), Read(base, viewport + 692)};
  return (size.width == state.render.width && size.height == state.render.height) ||
         (size.width == state.output.width && size.height == state.output.height);
}
std::optional<double> CameraAspect(uint8_t* base, uint32_t viewport) {
  const auto state = Output();
  if (!DisplayViewport(base, viewport, state) || !ScreenCameraOwner(Owner(base, viewport)))
    return std::nullopt;
  const double width = Float(base, viewport + 672);
  const double height = Float(base, viewport + 676);
  if (!std::isfinite(width) || !std::isfinite(height) || width <= 0 || height <= 0)
    return std::nullopt;
  return double(state.output.width) / state.output.height * width / height;
}
UiContext MakeContext(UiRole role, Point anchor) {
  const auto state = Output();
  const auto transform = role == UiRole::kMenuBody ? MenuBodyLayout(state.output, anchor.y)
                                                   : Layout(state.output, anchor);
  return {transform, state.render, role, state.ready && role != UiRole::kNone, state.generation};
}
class EmitScope {
 public:
  explicit EmitScope(Emission value) : previous_(emission) {
    if (baked_font_depth)
      emission = {};
    else if (!emission.active)
      emission = value;
  }
  ~EmitScope() { emission = previous_; }

 private:
  Emission previous_;
};
class NoFontTransform {
 public:
  NoFontTransform() : previous_(emission) {
    ++baked_font_depth;
    emission = {};
  }
  ~NoFontTransform() {
    --baked_font_depth;
    emission = previous_;
  }

 private:
  Emission previous_;
};
struct DcLayout {
  uint32_t token, vtable;
  uint64_t serial;
  UiContext context;
};
std::mutex dc_mutex;
std::unordered_map<uint32_t, DcLayout> dc_layouts;
std::deque<std::pair<uint32_t, uint64_t>> dc_order;
uint64_t next_dc_serial = 0;
constexpr size_t kMaximumDcLayouts = 32768;

bool LayoutDc(uint32_t vtable) {
  return HasUiDrawExecutor(vtable);
}

UiContext DcContext(PPCContext& ctx, uint8_t* base) {
  const uint32_t dc = ctx.r3.u32;
  if (!Span(base, dc, 8))
    return ui_context;
  const uint32_t vtable = Read(base, dc), token = StableDcToken(Read(base, dc + 4));
  std::lock_guard lock(dc_mutex);
  const auto it = dc_layouts.find(dc);
  if (it == dc_layouts.end() || it->second.token != token || it->second.vtable != vtable)
    return ui_context;
  return it->second.context;
}
struct HudAnchor {
  bool specified = false, world_position = false;
  Point anchor;
};
std::mutex hud_mutex;
std::unordered_map<uint32_t, HudAnchor> hud_anchors;
HudAnchor NameAnchor(std::string_view name) {
  if (name.starts_with("HUD_MP_NAME"))
    return {false, true, {}};
  if (name.starts_with("HUD_RADAR") || name.starts_with("HUD_PHONE_MESSAGE") ||
      name == "HUD_TEXT_MESSAGE_ICON" || name == "HUD_SLEEP_MODE_ICON")
    return {true, false, {0, 1}};
  if (name.starts_with("HUD_HELP"))
    return {true, false, {0, 0}};
  if (name == "HUD_SUBITILES")
    return {true, false, {0.5, 1}};
  if ((name.starts_with("HUD_WEAPON_") && name != "HUD_WEAPON_ICON") ||
      (name.starts_with("HUD_BIG_MESSAGE_") && name != "HUD_BIG_MESSAGE_TITLE"))
    return {true, false, {0.5, 0.5}};
  return {};
}
UiContext HudContext(PPCContext& ctx, uint8_t* base) {
  if (ui_context.active && ui_context.role != UiRole::kComponent)
    return ui_context;
  const uint32_t id = ctx.r3.u32;
  if (id >= 256 || !Span(base, kHudTable + id * 4, 4))
    return ui_context;
  const uint32_t object = Read(base, kHudTable + id * 4);
  if (!Span(base, object, 40))
    return ui_context;
  const Point point{Float(base, object + 24), Float(base, object + 28)};
  if (!std::isfinite(point.x) || !std::isfinite(point.y))
    return ui_context;
  HudAnchor policy;
  {
    std::lock_guard lock(hud_mutex);
    const auto it = hud_anchors.find(id);
    if (it != hud_anchors.end())
      policy = it->second;
  }
  return MakeContext(UiRole::kComponent, policy.world_position ? point
                                         : policy.specified    ? policy.anchor
                                                               : ComponentAnchor(point));
}
void DrawHud(PPCContext& ctx, uint8_t* base, GuestFunction original) {
  const Scope scope(HudContext(ctx, base));
  original(ctx, base);
}
uint32_t FontState(PPCContext& ctx, uint8_t* base) {
  if (!Span(base, kFontStateIndex, 4))
    return 0;
  uint32_t channel = Read(base, kFontStateIndex);
  if (channel == UINT32_MAX) {
    if (!Span(base, ctx.r13.u32, 4))
      return 0;
    const uint32_t tls = Read(base, ctx.r13.u32);
    if (tls > UINT32_MAX - 8 || !Span(base, tls + 8, 4))
      return 0;
    channel = (Read(base, tls + 8) >> 2) & 1;
  }
  if (channel > 1)
    return 0;
  const uint32_t address = kFontStates + channel * 68;
  return Span(base, address, 68, true) ? address : 0;
}
}  // namespace

void Publish(Extent render, Extent output) {
  if (!render.width || !render.height || !output.width || !output.height)
    return;
  bool changed;
  {
    std::lock_guard lock(output_mutex);
    changed = !output_state.ready || output.width != output_state.output.width ||
              output.height != output_state.output.height ||
              render.width != output_state.render.width ||
              render.height != output_state.render.height;
    output_state = {render, output, true, output_state.generation + uint64_t(changed)};
  }
  if (changed) {
    const auto layout = Layout(output);
    REXLOG_INFO(
        "gta4-aspect: output={}x{} render={}x{} camera-aspect={} ui-scale={},{} "
        "artwork-offset={},{}",
        output.width, output.height, render.width, render.height,
        double(output.width) / output.height, layout.sx, layout.sy, layout.ox, layout.oy);
  }
}
UiContext CurrentUi(uint8_t* base) {
  if (baked_font_depth)
    return {};
  if (ui_context.active)
    return ui_context;
  const auto state = Output();
  if (!state.ready || !Span(base, kCurrentViewport, 4))
    return {};
  const uint32_t viewport = Read(base, kCurrentViewport);
  if (!DisplayViewport(base, viewport, state))
    return {};
  const bool startup = Span(base, kStartupViewport, 4) && viewport == Read(base, kStartupViewport);
  if (!PrimaryUiOwner(Owner(base, viewport)) && !startup)
    return {};
  return {Layout(state.output), state.render, UiRole::kComponent, true, state.generation};
}
UiContext MenuBodyUi(uint8_t* base) {
  // Nested list, slider and hitbox passes must not compute different snapshots.
  if (ui_context.active && ui_context.role == UiRole::kMenuBody)
    return ui_context;
  double divider_y = kDefaultMenuDividerY;
  // Retail frontend style pointer; entry zero is TOP_position_of_top_line.
  constexpr uint32_t kFrontendStylePointer = 0x82BF9D98;
  if (Span(base, kFrontendStylePointer, sizeof(uint32_t))) {
    const uint32_t style = Read(base, kFrontendStylePointer);
    if (Span(base, style, sizeof(float)))
      divider_y = Float(base, style);
  }
  return MakeContext(UiRole::kMenuBody, {0.5, divider_y});
}
UiContext TextUi(const PPCContext& ctx, uint8_t* base) {
  if (ui_context.active)
    return ui_context;
  auto context = CurrentUi(base);
  if (!context.active || !std::isfinite(ctx.f1.f64) || !std::isfinite(ctx.f2.f64))
    return context;
  const Point anchor = ComponentAnchor({ctx.f1.f64, ctx.f2.f64});
  context.transform.ox = (1 - context.transform.sx) * anchor.x;
  context.transform.oy = (1 - context.transform.sy) * anchor.y;
  return context;
}
Scope::Scope(UiRole role, Point anchor) : previous_(ui_context) {
  switch (role) {
    case UiRole::kFixed:
      anchor = {0.5, 0.5};
      break;
    case UiRole::kMenuBody:
      anchor = {0.5, kDefaultMenuDividerY};
      break;
    case UiRole::kMenuFooter:
      anchor = {0.5, 1};
      break;
    case UiRole::kRadar:
      anchor = {0, 1};
      break;
    case UiRole::kHelp:
      anchor = {0, 0};
      break;
    case UiRole::kLoadingLabel:
      anchor = {1, 1};
      break;
    default:
      break;
  }
  ui_context = MakeContext(role, anchor);
}
Scope::Scope(UiContext context) : previous_(ui_context) {
  ui_context = context;
}
Scope::~Scope() {
  ui_context = previous_;
}
DcScope::DcScope(PPCContext& ctx, uint8_t* base) : scope_(DcContext(ctx, base)) {}

void FinalizeDc(uint8_t* base, uint32_t dc) {
  if (!Output().ready || !Span(base, dc, 8))
    return;
  const uint32_t vtable = Read(base, dc), token = StableDcToken(Read(base, dc + 4));
  std::lock_guard lock(dc_mutex);
  // Invalidate on every publication, including reuse by a non-UI command.
  dc_layouts.erase(dc);
  if (!LayoutDc(vtable) || !ui_context.active)
    return;
  const uint64_t serial = ++next_dc_serial;
  dc_layouts.emplace(dc, DcLayout{token, vtable, serial, ui_context});
  dc_order.emplace_back(dc, serial);
  // Both containers are bounded. Token + vtable prevents address-reuse contamination.
  for (; dc_order.size() > kMaximumDcLayouts; dc_order.pop_front()) {
    const auto [old_dc, old_serial] = dc_order.front();
    const auto it = dc_layouts.find(old_dc);
    if (it != dc_layouts.end() && it->second.serial == old_serial)
      dc_layouts.erase(it);
  }
}
void PrepareViewport(PPCContext& ctx, uint8_t* base) {
  const uint32_t viewport = ctx.r3.u32;
  const auto state = Output();
  if (!DisplayViewport(base, viewport, state) || !Span(base, viewport + 448, 280, true))
    return;
  const uint32_t owner = Owner(base, viewport);
  const bool screen = ScreenCameraOwner(owner), phone = PhoneCameraOwner(owner);
  if (!screen && !phone)
    return;
  PPCContext query = ctx;
  __imp__sub_821ED2F0(query, base);
  const auto screen_aspect = screen ? CameraAspect(base, viewport) : std::optional<double>{};
  const double aspect = screen_aspect ? *screen_aspect : query.f1.f64;
  const double authored = Float(base, viewport + 696);
  const double fov = screen ? ExpandVerticalFov(authored, state.output.aspect()) : authored;
  if (!std::isfinite(fov) || !std::isfinite(aspect) || fov <= 0 || fov >= 179 || aspect <= 0)
    return;
  constexpr double half_radians = 3.14159265358979323846 / 360.0;
  const double tangent = std::tan(fov * half_radians);
  const auto layout = phone ? Layout(state.output, {1, 1}) : Transform{};
  const double expected_x = Float(base, viewport + 720) / (tangent * aspect) * layout.sx;
  const double expected_y = Float(base, viewport + 724) / tangent * layout.sy;
  const double actual_x = Float(base, viewport + 448), actual_y = Float(base, viewport + 468);
  if (!std::isfinite(expected_x) || !std::isfinite(expected_y))
    return;
  if (std::abs(actual_x - expected_x) > std::max(1.0, std::abs(expected_x)) * 1e-5 ||
      std::abs(actual_y - expected_y) > std::max(1.0, std::abs(expected_y)) * 1e-5) {
    // Derived owners can be assigned after the base constructor. At first bind,
    // rebuild both the framing and the shape, not just their aspect quotient.
    PPCContext call = ctx;
    sub_828BDAD8(call, base);
  }
}

void DrawQuad(PPCContext& ctx, uint8_t* base, GuestFunction original, bool textured) {
  auto layout = CurrentUi(base);
  if (!layout.active || emission.active || baked_font_depth) {
    original(ctx, base);
    return;
  }
  Rect bounds{std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(),
              -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity()};
  const std::array<uint32_t, 4> vertices = {ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32};
  for (uint32_t vertex : vertices) {
    if (!Span(base, vertex, 8)) {
      original(ctx, base);
      return;
    }
    const double x = Float(base, vertex), y = Float(base, vertex + 4);
    if (!std::isfinite(x) || !std::isfinite(y)) {
      original(ctx, base);
      return;
    }
    bounds.left = std::min(bounds.left, x);
    bounds.right = std::max(bounds.right, x);
    bounds.top = std::min(bounds.top, y);
    bounds.bottom = std::max(bounds.bottom, y);
  }
  const bool solid = !textured && Span(base, 0x831C2910, 4) && Span(base, 0x831C2D48, 4) &&
                     Read(base, 0x831C2910) == Read(base, 0x831C2D48);
  if (solid)
    layout.transform = CoveringBackground(layout.transform, bounds);
  const EmitScope emit({layout.transform, !layout.transform.identity()});
  original(ctx, base);
}
void DrawRadarSection(PPCContext& ctx, uint8_t* base, GuestFunction original) {
  const DcScope dc(ctx, base);
  const auto layout = CurrentUi(base);
  const EmitScope emit({layout.transform, layout.active && !layout.transform.identity()});
  original(ctx, base);
}
void DrawWindow(PPCContext& ctx, uint8_t* base, GuestFunction original) {
  // Generated sub_821F6E38 forwards normalized measured bounds unchanged to
  // sub_8224D1E0. Its tessellator preserves those units through sub_828C2290.
  const auto layout = CurrentUi(base);
  const EmitScope emit({layout.transform, layout.active && !layout.transform.identity()});
  original(ctx, base);
}
FrontendLayoutScope::FrontendLayoutScope(PPCContext& ctx, uint8_t* base) {
  const auto layout = CurrentUi(base);
  if (!layout.active || layout.transform.sx >= 1 || ctx.r3.u32 > 2)
    return;
  constexpr uint32_t table = 0x82CC7BD0;
  if (!Span(base, table + ctx.r3.u32 * 4, 4))
    return;
  const uint32_t widget = Read(base, table + ctx.r3.u32 * 4);
  if (!Span(base, widget, 3216, true))
    return;
  // Retail has two column advances at +3112/+3116, followed by origin +3120.
  const double left = Float(base, widget + 3120);
  const double first = Float(base, widget + 3112), second = Float(base, widget + 3116);
  if (!std::isfinite(left) || !std::isfinite(first) || !std::isfinite(second) || first <= 0 ||
      second <= 0 || first > 1 || second > 1 || left < 0 || left > 1)
    return;
  const double extra = 1.0 / layout.transform.sx - 1.0;
  base_ = base;
  addresses_ = {widget + 3120, widget + 3116};
  originals_ = {Read(base, addresses_[0]), Read(base, addresses_[1])};
  replacements_ = {std::bit_cast<uint32_t>(float(left - extra * 0.5)),
                   std::bit_cast<uint32_t>(float(second + extra))};
  for (size_t i = 0; i < addresses_.size(); ++i)
    Write(base, addresses_[i], replacements_[i]);
  if (Trace())
    REXLOG_INFO("gta4-aspect: menu-reflow widget={:08X} extra-width={}", widget, extra);
}
FrontendLayoutScope::~FrontendLayoutScope() {
  if (!base_)
    return;
  for (size_t i = 0; i < addresses_.size(); ++i)
    if (Read(base_, addresses_[i]) == replacements_[i])
      Write(base_, addresses_[i], originals_[i]);
}
}  // namespace gta4::aspect

// Only new strong hooks live here; existing hooks call the shared adapters above.
extern "C" void sub_821ED2F0(PPCContext& ctx, uint8_t* base) {
  const uint32_t viewport = ctx.r3.u32;
  __imp__sub_821ED2F0(ctx, base);
  if (const auto aspect = gta4::aspect::CameraAspect(base, viewport))
    ctx.f1.f64 = *aspect;
}
extern "C" void sub_828BDAD8(PPCContext& ctx, uint8_t* base) {
  using namespace gta4::aspect;
  const uint32_t viewport = ctx.r3.u32;
  const auto aspect = CameraAspect(base, viewport);
  const auto state = Output();
  const uint32_t previous_phone = phone_projection_build;
  const bool phone =
      DisplayViewport(base, viewport, state) && PhoneCameraOwner(Owner(base, viewport));
  phone_projection_build = phone ? viewport : 0;
  uint32_t authored = 0;
  bool changed = false;
  if (aspect && Span(base, viewport + 696, 4, true)) {
    authored = Read(base, viewport + 696);
    const double original = std::bit_cast<float>(authored);
    const double resolved = ExpandVerticalFov(original, state.output.aspect());
    changed = std::isfinite(resolved) && resolved != original;
    if (changed)
      Float(base, viewport + 696, resolved);
    if (Trace())
      REXLOG_INFO(
          "gta4-aspect: camera viewport={:08X} owner={:08X} aspect={} authored-fov={} "
          "resolved-fov={}",
          viewport, Owner(base, viewport), *aspect, original, resolved);
  }
  __imp__sub_828BDAD8(ctx, base);
  // Matrices, cached tangents and frustum remain resolved; the input FOV stays authored.
  if (changed)
    Write(base, viewport + 696, authored);
  phone_projection_build = previous_phone;
}
extern "C" void sub_828BD1D8(PPCContext& ctx, uint8_t* base) {
  using namespace gta4::aspect;
  const uint32_t viewport = ctx.r3.u32;
  if (viewport && phone_projection_build == viewport && Span(base, viewport + 448, 64, true)) {
    std::array<float, 16> matrix;
    for (size_t i = 0; i < matrix.size(); ++i)
      matrix[i] = Float(base, viewport + 448 + uint32_t(i) * 4);
    TransformProjection(matrix, Layout(Output().output, {1, 1}));
    for (size_t i = 0; i < matrix.size(); ++i)
      Float(base, viewport + 448 + uint32_t(i) * 4, matrix[i]);
  }
  __imp__sub_828BD1D8(ctx, base);
}
extern "C" void sub_828C2290(PPCContext& ctx, uint8_t* base) {
  using namespace gta4::aspect;
  if (emission.active && !baked_font_depth) {
    const auto point = emission.transform.Map(Point{ctx.f1.f64, ctx.f2.f64});
    ctx.f1.f64 = point.x;
    ctx.f2.f64 = point.y;
    if (emission.clip_art) {
      ctx.f1.f64 = std::clamp(point.x, emission.art_clip.left, emission.art_clip.right);
      ctx.f2.f64 = std::clamp(point.y, emission.art_clip.top, emission.art_clip.bottom);
      ctx.f7.f64 = std::clamp((ctx.f1.f64 - emission.art_quad.left) /
                                  (emission.art_quad.right - emission.art_quad.left),
                              0.0, 1.0);
      ctx.f8.f64 = std::clamp(
          (ctx.f2.f64 - emission.art_quad.top) / (emission.art_quad.bottom - emission.art_quad.top),
          0.0, 1.0);
    }
  }
  __imp__sub_828C2290(ctx, base);
}
extern "C" void sub_82143C88(PPCContext& ctx, uint8_t* base) {
  using namespace gta4::aspect;
  // Only the actual textured loading layer, not the player's full-output fade quads.
  const auto state = Output();
  if (!state.ready || ctx.lr != 0x821440C0) {
    __imp__sub_82143C88(ctx, base);
    return;
  }
  const Transform transform = Layout(state.output).Pixels(state.render);
  const Rect quad = transform.Map(Rect{ctx.f1.f64, ctx.f2.f64, ctx.f3.f64, ctx.f4.f64});
  if (!(quad.right > quad.left && quad.bottom > quad.top)) {
    __imp__sub_82143C88(ctx, base);
    return;
  }
  const Rect clip =
      transform.Map(Rect{0, 0, double(state.render.width), double(state.render.height)});
  const EmitScope emit({transform, !transform.identity(), true, quad, clip});
  if (Trace())
    REXLOG_INFO("gta4-aspect: fixed-art rect={},{},{},{}", clip.left, clip.top, clip.right,
                clip.bottom);
  __imp__sub_82143C88(ctx, base);
}
extern "C" void sub_8227F458(PPCContext& ctx, uint8_t* base) {
  using namespace gta4::aspect;
  auto layout = CurrentUi(base);
  if (!layout.active || emission.active || baked_font_depth) {
    __imp__sub_8227F458(ctx, base);
    return;
  }
  if (!layout.render.valid()) {
    __imp__sub_8227F458(ctx, base);
    return;
  }
  const Rect bounds{ctx.f1.f64 / layout.render.width, ctx.f2.f64 / layout.render.height,
                    ctx.f3.f64 / layout.render.width, ctx.f4.f64 / layout.render.height};
  if (layout.role == UiRole::kLoadingLabel) {
    // A moving mask can extend past either horizontal screen edge. It is still
    // part of the label composition, not a full-width panel: changing its scale
    // when it crosses an edge would separate it from the adjoining texture.
    // Only an actual whole-output fade bypasses the composition transform.
    const bool full_output =
        std::min(bounds.left, bounds.right) <= 0 && std::max(bounds.left, bounds.right) >= 1 &&
        std::min(bounds.top, bounds.bottom) <= 0 && std::max(bounds.top, bounds.bottom) >= 1;
    if (full_output)
      layout.transform = {};
  } else {
    layout.transform = CoveringBackground(layout.transform, bounds);
  }
  TraceLoadingMask(layout, {ctx.f1.f64, ctx.f2.f64, ctx.f3.f64, ctx.f4.f64}, "solid-mask");
  const EmitScope emit({layout.transform.Pixels(layout.render), !layout.transform.identity()});
  __imp__sub_8227F458(ctx, base);
}
extern "C" void sub_821F6680(PPCContext& ctx, uint8_t* base) {
  using namespace gta4::aspect;
  if (append_depth) {
    // Shadow/outline recursive emissions already have a mapped origin; scale their
    // original offsets once without remapping the origin or font state again.
    ctx.f1.f64 = append_origin.x + (ctx.f1.f64 - append_origin.x) * append_transform.sx;
    ctx.f2.f64 = append_origin.y + (ctx.f2.f64 - append_origin.y) * append_transform.sy;
    __imp__sub_821F6680(ctx, base);
    return;
  }
  const auto layout = CurrentUi(base);
  const uint32_t font = layout.active ? FontState(ctx, base) : 0;
  if (!font || layout.transform.identity()) {
    __imp__sub_821F6680(ctx, base);
    return;
  }
  std::array<uint32_t, kFontScaledOffsets.size()> saved;
  for (size_t i = 0; i < saved.size(); ++i) {
    saved[i] = Read(base, font + kFontScaledOffsets[i]);
    if (!std::isfinite(std::bit_cast<float>(saved[i]))) {
      __imp__sub_821F6680(ctx, base);
      return;
    }
  }
  append_transform = layout.transform;
  append_origin = layout.transform.Map(Point{ctx.f1.f64, ctx.f2.f64});
  if (TraceLoadingLabel(layout)) {
    REXLOG_INFO(
        "gta4-aspect-loading: text normalized source={},{} mapped={},{} "
        "scale={},{} anchor-offset={},{}",
        ctx.f1.f64, ctx.f2.f64, append_origin.x, append_origin.y, layout.transform.sx,
        layout.transform.sy, layout.transform.ox, layout.transform.oy);
  }
  ctx.f1.f64 = append_origin.x;
  ctx.f2.f64 = append_origin.y;
  for (size_t i = 0; i < saved.size(); ++i)
    Float(base, font + kFontScaledOffsets[i],
          std::bit_cast<float>(saved[i]) * FontScale(kFontScaledOffsets[i], layout.transform));
  ++append_depth;
  __imp__sub_821F6680(ctx, base);
  --append_depth;
  for (size_t i = 0; i < saved.size(); ++i)
    Write(base, font + kFontScaledOffsets[i], saved[i]);
}
extern "C" void sub_821F5788(PPCContext& ctx, uint8_t* base) {
  const gta4::aspect::NoFontTransform baked;
  __imp__sub_821F5788(ctx, base);
}
extern "C" void sub_821C4B90(PPCContext& ctx, uint8_t* base) {
  using namespace gta4::aspect;
  std::string name;
  const uint32_t address = ctx.r3.u32;
  if (Output().ready && Span(base, address, 96)) {
    const char* text = reinterpret_cast<const char*>(rex::memory::GuestPtr(base, address));
    const auto* end = static_cast<const char*>(std::memchr(text, 0, 96));
    if (end)
      name.assign(text, end);
  }
  __imp__sub_821C4B90(ctx, base);
  if (!name.empty() && ctx.r3.u32 < 256) {
    std::lock_guard lock(hud_mutex);
    hud_anchors[ctx.r3.u32] = NameAnchor(name);
  }
}
#define ASPECT_HUD_HOOK(address)                            \
  extern "C" void address(PPCContext& ctx, uint8_t* base) { \
    gta4::aspect::DrawHud(ctx, base, __imp__##address);     \
  }
ASPECT_HUD_HOOK(sub_821C4CD8)
ASPECT_HUD_HOOK(sub_821C5148)
ASPECT_HUD_HOOK(sub_821C5480)
ASPECT_HUD_HOOK(sub_821C5660)
ASPECT_HUD_HOOK(sub_821C58E0)
#undef ASPECT_HUD_HOOK
#define ASPECT_DC_HOOK(address)                             \
  extern "C" void address(PPCContext& ctx, uint8_t* base) { \
    const gta4::aspect::DcScope scope(ctx, base);           \
    __imp__##address(ctx, base);                            \
  }
ASPECT_DC_HOOK(sub_821BCF80)
ASPECT_DC_HOOK(sub_821BCFA0)
ASPECT_DC_HOOK(sub_821BD018)
ASPECT_DC_HOOK(sub_821BD138)
ASPECT_DC_HOOK(sub_821BD528)
#undef ASPECT_DC_HOOK
extern "C" void sub_821BD218(PPCContext& ctx, uint8_t* base) {
  gta4::aspect::DrawRadarSection(ctx, base, __imp__sub_821BD218);
}
extern "C" void sub_821BD238(PPCContext& ctx, uint8_t* base) {
  gta4::aspect::DrawRadarSection(ctx, base, __imp__sub_821BD238);
}
extern "C" void sub_822551E0(PPCContext& ctx, uint8_t* base) {
  // This is the menu slider pass, not the footer. The retail compositor
  // sub_82255CC8 draws row labels through sub_8229F0F8, then calls this routine
  // to walk type-101 items and emit their border, track and value fill. Both
  // phases must use the same divider-anchored body coordinates, including
  // immediate draw calls and queued text.
  const gta4::aspect::Scope scope(gta4::aspect::MenuBodyUi(base));
  __imp__sub_822551E0(ctx, base);
}

extern "C" void sub_828BF708(PPCContext& ctx, uint8_t* base) {
  __imp__sub_828BF708(ctx, base);
  if (gta4::aspect::Output().ready)
    ctx.r3.u64 = 1;
}
extern "C" void sub_821F7208(PPCContext& ctx, uint8_t* base) {
  const gta4::aspect::Scope scope(gta4::aspect::TextUi(ctx, base));
  __imp__sub_821F7208(ctx, base);
}

// Both pixel-space textured-rectangle adapters meet here: sub_8227F5B8
// supplies default UVs; sub_8227F608 supplies explicit UVs (generated .10).
// Transform their vertex positions once at the common emitter. UVs, colors,
// blend state and animation inputs stay under the original game's control.
extern "C" void sub_8227F2E8(PPCContext& ctx, uint8_t* base) {
  using namespace gta4::aspect;
  const auto layout = CurrentUi(base);
  if (layout.active && layout.render.valid() && !emission.active && !baked_font_depth)
    TraceLoadingMask(layout, {ctx.f1.f64, ctx.f2.f64, ctx.f3.f64, ctx.f4.f64}, "textured-mask");
  const EmitScope emit({layout.transform.Pixels(layout.render),
                        layout.active && layout.render.valid() && !layout.transform.identity()});
  __imp__sub_8227F2E8(ctx, base);
}

extern "C" void sub_821B5C90(PPCContext& ctx, uint8_t* base) {
  // The retail loading label is a composition, not just font glyphs. Generated
  // .4:821B61E0 submits HUD text, then 821B64B0 / 821B652C / 821B6604 draw
  // the translucent solid masks and moving textured strip. Nested HUD text,
  // queued commands and immediate pixel rectangles must use this same anchor.
  const gta4::aspect::Scope layout_scope(gta4::aspect::UiRole::kLoadingLabel);
  __imp__sub_821B5C90(ctx, base);
}
extern "C" void sub_82255CC8(PPCContext& ctx, uint8_t* base) {
  const gta4::aspect::Scope scope(gta4::aspect::MenuBodyUi(base));
  __imp__sub_82255CC8(ctx, base);
}

// The frontend appends this command inline, bypassing sub_82146790. Capture the
// component at construction as well; StableDcToken survives size publication.
extern "C" void sub_821BF598(PPCContext& ctx, uint8_t* base) {
  const uint32_t dc = ctx.r3.u32;
  __imp__sub_821BF598(ctx, base);
  gta4::aspect::FinalizeDc(base, dc);
}
