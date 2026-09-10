#include "gta4_presentation_options.h"
#include "gta4_presentation_policy.h"

#include <array>
#include <atomic>
#include <cstdint>
#include <limits>
#include <optional>
#include <span>

#include <rex/diagnostics/policy.h>
#include <rex/logging.h>
#include <rex/runtime.h>

#include "gta4_init.h"

namespace {
namespace policy = gta4::presentation::policy;
constexpr uint32_t kActive = 0x831D5335;
constexpr uint32_t kCurrentScreen = 0x831D5340;
constexpr uint32_t kScreenCount = 0x831D5344;
constexpr uint32_t kIntroPending = 0x831D5348;
constexpr uint32_t kDefinitions = 0x831D5498;
constexpr uint32_t kEpisode = 0x82B39504;  // retail GET_CURRENT_EPISODE, sub_825D4CC8
constexpr uint32_t kEffectLinkOffset = 108;
constexpr uint32_t kCompositeTechniqueOffset = 732;
thread_local bool cold_parser_scope = false;
std::atomic<bool> observe_startup{false};
std::atomic<uint32_t> startup_events{0};
std::atomic<uint32_t> composite_events{0};
std::atomic<uint32_t> composite_changes{0};
struct CompositeTraceKey {
  uint32_t episode, requested, selected;
  bool disabled, eligible;
  bool operator==(const CompositeTraceKey&) const = default;
};
thread_local std::optional<CompositeTraceKey> last_composite;

bool Diagnostics() noexcept {
  return gta4::presentation::TraceEnabled() &&
         rex::diagnostics::IsEnabled(rex::diagnostics::Category::kLogging);
}

bool GuestSpan(uint8_t* base, uint32_t address, std::size_t size, bool writable = false) {
  if (!base || !address || !size || size > std::numeric_limits<uint32_t>::max())
    return false;
  const uint64_t last = uint64_t{address} + size - 1;
  if (last > std::numeric_limits<uint32_t>::max())
    return false;
  auto* kernel = REX_KERNEL_STATE();
  auto* memory = kernel ? kernel->memory() : nullptr;
  auto* heap = memory ? memory->LookupHeap(address) : nullptr;
  if (!heap || heap != memory->LookupHeap(static_cast<uint32_t>(last)))
    return false;
  const auto access = heap->QueryRangeAccess(address, static_cast<uint32_t>(last));
  using rex::memory::PageAccess;
  return access == PageAccess::kReadWrite || access == PageAccess::kExecuteReadWrite ||
         (!writable && (access == PageAccess::kReadOnly || access == PageAccess::kExecuteReadOnly));
}

void TraceStartup(uint8_t* base, const char* point, uint32_t caller) {
  if (!Diagnostics() || !observe_startup.load(std::memory_order_relaxed) ||
      !GuestSpan(base, kActive, kIntroPending - kActive + 1))
    return;
  const uint32_t event = startup_events.fetch_add(1, std::memory_order_relaxed);
  if (event >= 64)
    return;
  const uint32_t index = REX_LOAD_U32(kCurrentScreen);
  uint32_t marker = std::numeric_limits<uint32_t>::max();
  if (index < policy::kMaxScreens &&
      GuestSpan(base, kDefinitions + index * policy::kScreenStride, 16))
    marker = REX_LOAD_U32(kDefinitions + index * policy::kScreenStride + policy::kMarkerOffset);
  REXLOG_INFO(
      "gta4-presentation: event={} point={} caller={:08X} skip={} active={} "
      "intro-pending={} screen={} marker={} count={}",
      event, point, caller, gta4::presentation::SkipIntroAtLaunch(), REX_LOAD_U8(kActive),
      REX_LOAD_U8(kIntroPending), index, marker, REX_LOAD_U32(kScreenCount));
}

class ParserScope final {
 public:
  explicit ParserScope(bool enabled) : previous_(cold_parser_scope) { cold_parser_scope = enabled; }
  ~ParserScope() { cold_parser_scope = previous_; }
  ParserScope(const ParserScope&) = delete;
  ParserScope& operator=(const ParserScope&) = delete;

 private:
  bool previous_;
};
}  // namespace

extern "C" void sub_82145420(PPCContext& ctx, uint8_t* base) {
  const uint32_t caller = ctx.lr;
  const bool cold = policy::IsColdStart(caller, ctx.r3.u32, ctx.r4.u32);
  const bool eligible = cold && GuestSpan(base, kActive, 1) && !REX_LOAD_U8(kActive);
  if (eligible) {
    startup_events.store(0, std::memory_order_relaxed);
    observe_startup.store(Diagnostics(), std::memory_order_relaxed);
    TraceStartup(base, "cold-start-enter", caller);
  }
  const ParserScope scope(eligible && gta4::presentation::SkipIntroAtLaunch());
  // Keep arguments, asset loading, timer setup and publication entirely retail.
  __imp__sub_82145420(ctx, base);
  if (eligible)
    TraceStartup(base, "cold-start-ready", caller);
}

extern "C" void sub_82145968(PPCContext& ctx, uint8_t* base) {
  const bool apply = cold_parser_scope && ctx.lr == policy::kParserCaller;
  __imp__sub_82145968(ctx, base);
  if (!apply)
    return;
  if (!GuestSpan(base, kScreenCount, sizeof(uint32_t)))
    return;
  const uint32_t count = REX_LOAD_U32(kScreenCount);
  if (!count || count > policy::kMaxScreens ||
      !GuestSpan(base, kDefinitions, std::size_t{count} * policy::kScreenStride, true)) {
    if (Diagnostics())
      REXLOG_WARN("gta4-presentation: point=skip-rejected reason=table-span count={}", count);
    return;
  }
  std::span<uint8_t> table(base + kDefinitions, std::size_t{count} * policy::kScreenStride);
  const auto plan = policy::CollapseIntro(table, count, true);
  if (Diagnostics()) {
    if (plan.status != policy::IntroStatus::kReady) {
      REXLOG_WARN("gta4-presentation: point=skip-rejected reason=unsupported-layout count={}",
                  count);
    } else {
      REXLOG_INFO(
          "gta4-presentation: point=skip-applied records={} prefix={} end-intro={} "
          "assets=unchanged markers=preserved",
          count, plan.prefix_count, plan.end_intro_index);
      for (uint32_t i = 0; i < plan.prefix_count; ++i) {
        const uint32_t row = kDefinitions + i * policy::kScreenStride;
        REXLOG_INFO(
            "gta4-presentation: point=intro-record screen={} duration={} layers={} marker={} "
            "fade={}",
            i, REX_LOAD_U32(row), REX_LOAD_U32(row + 4), REX_LOAD_U32(row + 8),
            REX_LOAD_U32(row + 12));
      }
    }
  }
}

// Observation only: the original routine owns screen advancement and clocks.
extern "C" void sub_82144708(PPCContext& ctx, uint8_t* base) {
  const uint32_t caller = ctx.lr;
  __imp__sub_82144708(ctx, base);
  TraceStartup(base, "screen-advanced", caller);
}

extern "C" void sub_82142230(PPCContext& ctx, uint8_t* base) {
  // This is the stock frontend/profile/DLC workflow, not proof of rendered UI.
  TraceStartup(base, "frontend-workflow-enter", ctx.lr);
  observe_startup.store(false, std::memory_order_relaxed);
  __imp__sub_82142230(ctx, base);
}

extern "C" void sub_822CF300(PPCContext& ctx, uint8_t* base) {
  const uint32_t requested = ctx.r6.u32;
  const uint32_t caller = ctx.lr;
  const bool disable = gta4::presentation::DisableTladFilmGrain();
  const bool trace = Diagnostics();
  if (caller != policy::kCompositeCaller || (!disable && !trace) ||
      !GuestSpan(base, kEpisode, sizeof(uint32_t))) {
    __imp__sub_822CF300(ctx, base);
    return;
  }
  const uint32_t episode = REX_LOAD_U32(kEpisode);
  const uint32_t postfx = ctx.r3.u32;
  bool valid = false;
  if (episode == 1 && policy::IsNoisePass(requested) && ctx.r4.u32 == 0 &&
      GuestSpan(base, postfx, kCompositeTechniqueOffset + sizeof(uint32_t))) {
    const uint32_t effect = REX_LOAD_U32(postfx + kEffectLinkOffset);
    valid = ctx.r5.u32 != 0 && ctx.r5.u32 == REX_LOAD_U32(postfx + kCompositeTechniqueOffset) &&
            GuestSpan(base, effect, 28);
  }
  const uint32_t selected = policy::SelectCompositePass(requested, disable, episode, caller, valid);
  const CompositeTraceKey key{episode, requested, selected, disable, valid};
  const bool changed = trace && (!last_composite || *last_composite != key);
  const bool record_change =
      changed && composite_changes.fetch_add(1, std::memory_order_relaxed) < 32;
  const bool record_sample = trace && composite_events.fetch_add(1, std::memory_order_relaxed) < 96;
  if (trace)
    last_composite = key;
  if (record_sample || record_change) {
    REXLOG_INFO(
        "gta4-presentation: point=composite episode={} disabled={} caller={:08X} "
        "postfx={:08X} remap-eligible={} requested={} selected={} technique={:08X}",
        episode, disable, caller, postfx, valid, requested, selected, ctx.r5.u32);
  }
  // The original helper binds the chosen pass's own constants and texture slots.
  // Never change the script FORCE_NOISE_OFF byte or the profile's preference.
  if (selected != requested)
    ctx.r6.u64 = selected;
  __imp__sub_822CF300(ctx, base);
}
