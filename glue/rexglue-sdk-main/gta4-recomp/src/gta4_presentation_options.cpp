#include "gta4_presentation_options.h"

#include <atomic>
#include <mutex>
#include <string_view>

#include <rex/cvar.h>

REXCVAR_DEFINE_BOOL(
    gta4_skip_intro, false, "GTA IV/Frontend",
    "Skip legal and logo presentation at next launch; retain startup initialization")
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);
REXCVAR_DEFINE_BOOL(gta4_disable_tlad_film_grain, false, "GTA IV/Frontend",
                    "Use TLAD's grain-free composite passes; no effect in other episodes");
REXCVAR_DEFINE_BOOL(gta4_trace_presentation_options, false, "GTA IV/Diagnostics",
                    "Bounded startup presentation and TLAD composite-pass events")
    .debug_only();

namespace gta4::presentation {
namespace {
std::atomic<bool> skip_intro{false};
std::atomic<bool> disable_grain{false};
std::atomic<bool> trace{false};
std::once_flag callbacks;
bool ParseBool(std::string_view value) noexcept {
  return value == "true" || value == "1" || value == "yes";
}
}  // namespace

void InitializeOptions() {
  // Registry callbacks run under its mutex: they only publish the value, and
  // must never call back into GetFlagByName or a guest function.
  std::call_once(callbacks, [] {
    rex::cvar::RegisterChangeCallback(
        "gta4_disable_tlad_film_grain", [](std::string_view, std::string_view value) {
          disable_grain.store(ParseBool(value), std::memory_order_relaxed);
        });
    rex::cvar::RegisterChangeCallback("gta4_trace_presentation_options",
                                      [](std::string_view, std::string_view value) {
                                        trace.store(ParseBool(value), std::memory_order_relaxed);
                                      });
  });
  skip_intro.store(ParseBool(rex::cvar::GetFlagByName("gta4_skip_intro")),
                   std::memory_order_relaxed);
  disable_grain.store(ParseBool(rex::cvar::GetFlagByName("gta4_disable_tlad_film_grain")),
                      std::memory_order_relaxed);
  trace.store(ParseBool(rex::cvar::GetFlagByName("gta4_trace_presentation_options")),
              std::memory_order_relaxed);
}

bool SkipIntroAtLaunch() noexcept {
  return skip_intro.load(std::memory_order_relaxed);
}
bool DisableTladFilmGrain() noexcept {
  return disable_grain.load(std::memory_order_relaxed);
}
bool TraceEnabled() noexcept {
  return trace.load(std::memory_order_relaxed);
}
}  // namespace gta4::presentation
