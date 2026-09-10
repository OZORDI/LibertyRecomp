// Opt-in help-box observation. Sources are the compiled generated .7
// sub_82223CF8, .6 sub_821F32B8/32F8/6E38, .4 DC constructors/executors,
// and .0 sub_82146790. No game state, geometry, or command order is changed.
namespace {
struct HelpTraceConfig {
  bool enabled = false;
  uint32_t every = 1;
  uint32_t limit = 120000;
};
const HelpTraceConfig& HelpConfig() {
  static const HelpTraceConfig config = [] {
    HelpTraceConfig c;
    const char* flag = std::getenv("REX_GTA4_HELP_TRACE");
    c.enabled = flag && std::strcmp(flag, "1") == 0;
    auto number = [](const char* name, uint32_t fallback, uint32_t maximum) {
      const char* text = std::getenv(name);
      if (!text || !*text) return fallback;
      char* end = nullptr;
      const unsigned long value = std::strtoul(text, &end, 10);
      return end != text && !*end && value > 0 && value <= maximum
          ? uint32_t(value) : fallback;
    };
    c.every = number("REX_GTA4_HELP_TRACE_EVERY", 1, 3600);
    c.limit = number("REX_GTA4_HELP_TRACE_LIMIT", 120000, 1000000);
    return c;
  }();
  return config;
}
bool HelpEnabled() {
  return rex::diagnostics::IsEnabled(rex::diagnostics::Category::kLogging) &&
         HelpConfig().enabled;
}
struct HelpText {
  std::string quoted = "\"\"";
  uint64_t hash = 0;
  uint32_t bytes = 0;
  bool terminated = true;
};
HelpText ReadHelpText(uint8_t* base, uint32_t address) {
  HelpText out;
  if (!base || !address) return out;
  std::string raw;
  raw.reserve(400);
  for (uint32_t i = 0; i < 400; ++i) {
    const uint8_t value = LoadU8(base, address + i);
    if (!value) break;
    raw.push_back(char(value));
  }
  out.bytes = uint32_t(raw.size());
  out.terminated = raw.size() < 400;
  out.hash = XXH3_64bits(raw.data(), raw.size());
  out.quoted = "\"";
  for (unsigned char value : raw) {
    if (value == '"' || value == '\\') out.quoted.push_back('\\');
    if (value >= 32 && value < 127) out.quoted.push_back(char(value));
    else out.quoted += fmt::format("\\x{:02X}", value);
  }
  out.quoted.push_back('"');
  return out;
}
struct HelpFontState {
  uint32_t index = UINT32_MAX, address = 0, color = 0;
  uint8_t enabled = 0, mode = 0;
  bool valid = false;
};
HelpFontState ReadHelpFont(PPCContext& ctx, uint8_t* base) {
  HelpFontState state;
  state.index = LoadU32(base, 0x82A935A4);
  if (state.index == UINT32_MAX) {
    const uint32_t tls = ctx.r13.u32 ? LoadU32(base, ctx.r13.u32) : 0;
    if (!tls) return state;
    state.index = (LoadU32(base, tls + 8) >> 2) & 1u;
  }
  if (state.index > 1) return state;
  state.address = 0x82B9A118 + state.index * 68;
  state.enabled = LoadU8(base, state.address + 20);
  state.mode = LoadU8(base, state.address + 21);
  state.color = LoadU32(base, state.address + 28);
  state.valid = true;
  return state;
}
struct HelpCall {
  uint64_t id = 0;
  uint32_t frame = 0, object = 0, slot = 0, text_submits = 0;
  uint32_t choices = 0, constructed = 0, finalized = 0, immediate = 0;
  uint32_t text_depth = 0;
  int32_t hour = 0, hour_override = -1, effective_hour = 0, positive_gate = 0;
  bool sample = true;
  bool last_font_valid = false, last_font_background = false;
};
struct HelpBackground {
  uint64_t call = 0, id = 0;
  uint32_t frame = 0, object = 0, dc = 0, token = 0, vtable = 0, bytes = 0;
  uint32_t color = 0, texture = 0, caller = 0;
  uint64_t payload_hash = 0;
  const char* kind = "unknown";
  const char* origin = "unknown";
  bool sampled = false, finalized = false;
};
struct HelpExecution {
  HelpBackground background;
  uint8_t* base = nullptr;
  uint32_t accepted = 0, rejected = 0, captured = 0;
};
thread_local HelpCall* g_help_call = nullptr;
thread_local HelpExecution* g_help_execution = nullptr;
std::atomic<uint64_t> g_help_calls{0}, g_help_backgrounds{0};
std::atomic<uint32_t> g_help_incomplete{0};
std::mutex g_help_dc_mutex;
std::unordered_map<uint32_t, HelpBackground> g_help_dcs;
void HelpIncomplete(std::string_view reason, uint64_t call, uint32_t dc = 0) {
  if (g_help_incomplete.fetch_add(1, std::memory_order_relaxed) < 64)
    REXLOG_WARN("gta4-help-trace: point=incomplete call={} dc={:08X} reason={}", call, dc, reason);
}
const char* HelpChoiceSite(uint32_t caller) {
  switch (caller) {
    case 0x82223FE0: case 0x82223FF0: return "layout";
    case 0x82224B1C: case 0x82224B2C: return "text-render";
    case 0x82224500: return "separate-icon-background";
    default: return "other";
  }
}
HelpBackground HelpNewBackground(const char* kind, uint32_t caller) {
  HelpBackground bg;
  bg.id = g_help_backgrounds.fetch_add(1, std::memory_order_relaxed) + 1;
  bg.call = g_help_call->id;
  bg.frame = g_help_call->frame;
  bg.object = g_help_call->object;
  bg.kind = kind;
  bg.origin = g_help_call->text_depth ? "font-background" : "explicit-help-background";
  bg.caller = caller;
  bg.sampled = g_help_call->sample;
  return bg;
}
void HelpConstructed(uint8_t* base, uint32_t dc, uint32_t caller,
                     const char* kind, uint32_t bytes, uint32_t color_offset,
                     uint32_t texture_offset) {
  // Production layout capture also covers the frontend's inline Append path.
  gta4::aspect::FinalizeDc(base, dc);
  if (!g_help_call || !dc) return;
  ++g_help_call->constructed;
  auto bg = HelpNewBackground(kind, caller);
  bg.dc = dc;
  bg.bytes = bytes;
  bg.token = LoadU32(base, dc + 4);
  bg.vtable = LoadU32(base, dc);
  bg.color = LoadU32(base, dc + color_offset);
  bg.texture = texture_offset ? LoadU32(base, dc + texture_offset) : 0;
  bg.payload_hash = XXH3_64bits(GuestPointer(base, dc + 8), bytes - 8);
  {
    std::lock_guard lock(g_help_dc_mutex);
    if (g_help_dcs.size() >= 4096 && !g_help_dcs.contains(dc)) {
      HelpIncomplete("pending-background-capacity", bg.call, dc);
      return;
    }
    if (g_help_dcs.contains(dc)) HelpIncomplete("unconsumed-background-address-reused", bg.call, dc);
    g_help_dcs[dc] = bg;
  }
  if (bg.sampled)
    REXLOG_INFO("gta4-help-trace: point=background-built call={} bg={} frame={} "
                "origin={} kind={} creator={:08X} dc={:08X} token={:08X} vtable={:08X} "
                "color={:08X} alpha={} texture={:08X} payload={:016X}",
                bg.call, bg.id, bg.frame, bg.origin, bg.kind, caller, dc, bg.token, bg.vtable,
                bg.color, bg.color >> 24, bg.texture, bg.payload_hash);
}
void HelpExecute(PPCContext& ctx, uint8_t* base, uint32_t expected_vtable,
                 void (*original)(PPCContext&, uint8_t*)) {
  if (!HelpEnabled()) { original(ctx, base); return; }
  HelpBackground bg;
  {
    std::lock_guard lock(g_help_dc_mutex);
    const auto found = g_help_dcs.find(ctx.r3.u32);
    if (found == g_help_dcs.end()) { /* Original must run outside the lock. */ }
    else { bg = found->second; g_help_dcs.erase(found); }
  }
  if (!bg.id) { original(ctx, base); return; }
  // .0 sub_82146790 changes length bits 0x0003FF80, not constructor identity.
  const bool identity = LoadU32(base, bg.dc) == expected_vtable && bg.vtable == expected_vtable &&
      ((LoadU32(base, bg.dc + 4) ^ bg.token) & 0xFFFC007F) == 0 &&
      XXH3_64bits(GuestPointer(base, bg.dc + 8), bg.bytes - 8) == bg.payload_hash;
  if (!identity) {
    HelpIncomplete("background-identity-mismatch", bg.call, bg.dc);
    original(ctx, base);
    return;
  }
  if (!bg.finalized) HelpIncomplete("background-executed-without-observed-finalize", bg.call, bg.dc);
  HelpExecution execution{bg, base};
  auto* previous = g_help_execution;
  g_help_execution = &execution;
  if (bg.sampled)
    REXLOG_INFO("gta4-help-trace: point=background-execute call={} bg={} "
                "build-frame={} execute-frame={} dc={:08X} kind={} origin={} "
                "color={:08X} alpha={} finalized={}",
                bg.call, bg.id, bg.frame, GetNativeLightSubmittedFrame(base), bg.dc,
                bg.kind, bg.origin, bg.color, bg.color >> 24, bg.finalized);
  original(ctx, base);
  g_help_execution = previous;
  if (bg.sampled)
    REXLOG_INFO("gta4-help-trace: point=background-execute-end call={} bg={} "
                "native-accepted={} native-captured={} native-rejected={} gpu-verified=false",
                bg.call, bg.id, execution.accepted, execution.captured, execution.rejected);
}
void HelpImmediate(PPCContext& ctx, uint8_t* base, const char* kind, uint32_t color,
                   void (*original)(PPCContext&, uint8_t*)) {
  if (!g_help_call || g_help_execution) { original(ctx, base); return; }
  ++g_help_call->immediate;
  HelpExecution execution{HelpNewBackground(kind, uint32_t(ctx.lr)), base};
  execution.background.color = color;
  auto* previous = g_help_execution;
  g_help_execution = &execution;
  const auto& bg = execution.background;
  if (bg.sampled)
    REXLOG_INFO("gta4-help-trace: point=background-immediate call={} bg={} frame={} "
                "kind={} origin={} caller={:08X} color={:08X} alpha={}",
                bg.call, bg.id, bg.frame, kind, bg.origin, bg.caller, color, color >> 24);
  original(ctx, base);
  g_help_execution = previous;
  if (bg.sampled)
    REXLOG_INFO("gta4-help-trace: point=background-execute-end call={} bg={} "
                "native-accepted={} native-captured={} native-rejected={} gpu-verified=false",
                bg.call, bg.id, execution.accepted, execution.captured, execution.rejected);
}
}  // namespace

void GTA4_HelpTraceFinalize(uint8_t* base, uint32_t dc, uint32_t caller) {
  if (!g_help_call || !dc) return;
  HelpBackground bg;
  {
    std::lock_guard lock(g_help_dc_mutex);
    const auto found = g_help_dcs.find(dc);
    if (found == g_help_dcs.end() || found->second.call != g_help_call->id) return;
    found->second.finalized = true;
    bg = found->second;
  }
  ++g_help_call->finalized;
  if (bg.sampled)
    REXLOG_INFO("gta4-help-trace: point=background-finalize call={} bg={} frame={} "
                "dc={:08X} caller={:08X} position=before-original-length-finalizer",
                bg.call, bg.id, GetNativeLightSubmittedFrame(base), dc, caller);
}

void GTA4_HelpTraceNativeSubmission(uint32_t type, uint32_t device,
                                  bool accepted, bool captured) {
  if (!g_help_execution) return;
  using T = rex::graphics::gta4_native::CommandType;
  if (type != uint32_t(T::kDrawPrimitive) && type != uint32_t(T::kDrawPrimitiveUp) &&
      type != uint32_t(T::kDrawIndexedPrimitive)) return;
  auto& execution = *g_help_execution;
  if (captured) ++execution.captured;
  else if (accepted) ++execution.accepted;
  else ++execution.rejected;
  const auto& bg = execution.background;
  if (bg.sampled)
    REXLOG_INFO("gta4-help-trace: point=native-background-submit call={} bg={} "
                "frame={} device={:08X} type={} result={} gpu-verified=false",
                bg.call, bg.id, GetNativeLightSubmittedFrame(execution.base), device, type,
                captured ? "captured-not-submitted" : accepted ? "accepted" : "rejected");
}

void GTA4_HelpTraceTextSubmit(PPCContext& ctx, uint8_t* base,
                             void (*original)(PPCContext&, uint8_t*)) {
  if (!g_help_call) { original(ctx, base); return; }
  auto& call = *g_help_call;
  ++call.text_submits;
  const auto font = ReadHelpFont(ctx, base);
  call.last_font_valid = font.valid;
  call.last_font_background = font.enabled != 0;
  const auto text = ReadHelpText(base, ctx.r5.u32);
  if (call.sample)
    REXLOG_INFO("gta4-help-trace: point=text-submit call={} frame={} caller={:08X} "
                "text={} text-hash={:016X} text-terminated={} position={:.7g},{:.7g} "
                "font-state={:08X} font-valid={} font-bg-chosen={} font-bg-mode={} "
                "font-bg-color={:08X} font-bg-alpha={} explicit-bg-before-text={} "
                "evidence=actual-state-at-text-submission",
                call.id, GetNativeLightSubmittedFrame(base), uint32_t(ctx.lr), text.quoted,
                text.hash, text.terminated, ctx.f1.f64, ctx.f2.f64, font.address, font.valid,
                font.enabled != 0, font.mode, font.color, font.color >> 24,
                call.constructed + call.immediate);
  ++call.text_depth;
  original(ctx, base);
  --call.text_depth;
}

extern "C" void sub_82223CF8(PPCContext& ctx, uint8_t* base) {
  const gta4::aspect::Scope aspect_scope(gta4::aspect::UiRole::kHelp);
  if (!HelpEnabled() || !base || !ctx.r3.u32) { __imp__sub_82223CF8(ctx, base); return; }
  HelpCall call;
  call.object = ctx.r3.u32;
  // Empty idle calls are not evidence of a rendered help box.
  if (!LoadU8(base, call.object + 828) && !LoadU8(base, call.object + 1235) &&
      !LoadU8(base, call.object + 400) && !LoadU32(base, call.object + 800)) {
    __imp__sub_82223CF8(ctx, base);
    return;
  }
  call.id = g_help_calls.fetch_add(1, std::memory_order_relaxed) + 1;
  if (call.id > HelpConfig().limit) {
    if (call.id == uint64_t(HelpConfig().limit) + 1)
      HelpIncomplete("help-call-limit-exhausted", call.id);
    __imp__sub_82223CF8(ctx, base);
    return;
  }
  call.frame = GetNativeLightSubmittedFrame(base);
  call.hour = int32_t(LoadU32(base, 0x82DF3924));
  call.hour_override = int32_t(LoadU32(base, 0x82DF3928));
  call.effective_hour = call.hour_override == -1 ? call.hour : call.hour_override;
  call.positive_gate = int32_t(LoadU32(base, 0x831D535C));
  call.slot = LoadU8(base, call.object + 828) ? 0 : 1;
  const auto requested = ReadHelpText(base, call.object + 828 + call.slot * 407);
  // Optional thinning always retains message, clock, lane, and presentation changes.
  const uint64_t signature = requested.hash ^ (uint64_t(call.effective_hour) << 32) ^
      (uint64_t(uint32_t(call.positive_gate)) << 8) ^ (uint64_t(ctx.r4.u8) << 56) ^
      LoadU32(base, call.object + 800) ^ (uint64_t(LoadU8(base, call.object + 1234)) << 48);
  thread_local uint64_t last_signature = UINT64_MAX;
  thread_local uint32_t last_frame = 0;
  call.sample = HelpConfig().every == 1 || signature != last_signature ||
      uint32_t(call.frame - last_frame) >= HelpConfig().every;
  if (call.sample) { last_signature = signature; last_frame = call.frame; }
  auto* previous = g_help_call;
  g_help_call = &call;
  if (call.sample)
    REXLOG_INFO("gta4-help-trace: point=help-enter call={} frame={} object={:08X} "
                "thread={:08X} lane={} slot={} requested={} hour={} hour-override={} "
                "effective-hour={} positive-gate-raw={} clock-gate-open={} style={} "
                "help-state={} show-explicit-bg={} icon-request={}",
                call.id, call.frame, call.object, ctx.r13.u32, ctx.r4.u8, call.slot,
                requested.quoted, call.hour, call.hour_override, call.effective_hour,
                call.positive_gate, call.positive_gate > 0 ||
                  (call.effective_hour >= 6 && call.effective_hour <= 20),
                LoadU32(base, 0x82B39504), LoadU32(base, call.object + 800),
                LoadU8(base, call.object + 823), LoadU8(base, call.object + 1234));
  __imp__sub_82223CF8(ctx, base);
  if (call.sample)
    REXLOG_INFO("gta4-help-trace: point=help-end call={} frame={} "
                "text-submissions={} last-font-valid={} last-font-bg-chosen={} "
                "bg-choice-calls={} bg-constructed={} bg-finalized={} bg-immediate={} "
                "bg-emitted={} observation={} gpu-verified=false",
                call.id, GetNativeLightSubmittedFrame(base), call.text_submits,
                call.last_font_valid, call.last_font_background, call.choices,
                call.constructed, call.finalized, call.immediate,
                (call.finalized + call.immediate) != 0,
                call.text_submits ? "text-submitted" : "no-text-submission-on-this-call");
  g_help_call = previous;
}
extern "C" void sub_821F32B8(PPCContext& ctx, uint8_t* base) {
  const uint32_t enabled = ctx.r3.u8, mode = ctx.r4.u8, caller = uint32_t(ctx.lr);
  __imp__sub_821F32B8(ctx, base);
  if (!g_help_call) return;
  ++g_help_call->choices;
  const auto font = ReadHelpFont(ctx, base);
  if (g_help_call->sample)
    REXLOG_INFO("gta4-help-trace: point=background-choice call={} frame={} caller={:08X} "
                "site={} requested={} requested-mode={} font-valid={} applied={} applied-mode={}",
                g_help_call->id, GetNativeLightSubmittedFrame(base), caller,
                HelpChoiceSite(caller), enabled, mode, font.valid, font.enabled, font.mode);
}
extern "C" void sub_821F32F8(PPCContext& ctx, uint8_t* base) {
  const uint32_t color = ctx.r3.u32, caller = uint32_t(ctx.lr);
  __imp__sub_821F32F8(ctx, base);
  if (g_help_call && g_help_call->sample)
    REXLOG_INFO("gta4-help-trace: point=background-color call={} frame={} "
                "caller={:08X} color={:08X} alpha={}",
                g_help_call->id, GetNativeLightSubmittedFrame(base), caller, color, color >> 24);
}
extern "C" void sub_821BF348(PPCContext& ctx, uint8_t* base) {
  const uint32_t dc = ctx.r3.u32, caller = uint32_t(ctx.lr);
  __imp__sub_821BF348(ctx, base);
  HelpConstructed(base, dc, caller, "solid-quad", 48, 44, 40);
}
extern "C" void sub_821BF490(PPCContext& ctx, uint8_t* base) {
  const uint32_t dc = ctx.r3.u32, caller = uint32_t(ctx.lr);
  __imp__sub_821BF490(ctx, base);
  HelpConstructed(base, dc, caller, "textured-quad", 80, 76, 72);
}
extern "C" void sub_821BF628(PPCContext& ctx, uint8_t* base) {
  const uint32_t dc = ctx.r3.u32, caller = uint32_t(ctx.lr);
  __imp__sub_821BF628(ctx, base);
  HelpConstructed(base, dc, caller, "window", 28, 24, 0);
}
extern "C" void sub_821BD2D8(PPCContext& ctx, uint8_t* base) {
  const gta4::aspect::DcScope aspect_scope(ctx, base);
  HelpExecute(ctx, base, 0x8200146C, __imp__sub_821BD2D8);
}
extern "C" void sub_821BD4C8(PPCContext& ctx, uint8_t* base) {
  const gta4::aspect::DcScope aspect_scope(ctx, base);
  HelpExecute(ctx, base, 0x820014A4, __imp__sub_821BD4C8);
}
extern "C" void sub_821BD538(PPCContext& ctx, uint8_t* base) {
  const gta4::aspect::DcScope aspect_scope(ctx, base);
  HelpExecute(ctx, base, 0x820014DC, __imp__sub_821BD538);
}
static void AspectTraceSolidQuad(PPCContext& ctx, uint8_t* base) {
  if (!g_help_call || g_help_execution) { __imp__sub_8227E948(ctx, base); return; }
  HelpImmediate(ctx, base, "solid-quad", LoadU32(base, ctx.r7.u32), __imp__sub_8227E948);
}
extern "C" void sub_8227E948(PPCContext& ctx, uint8_t* base) {
  gta4::aspect::DrawQuad(ctx, base, AspectTraceSolidQuad, false);
}
static void AspectTraceTexturedQuad(PPCContext& ctx, uint8_t* base) {
  if (!g_help_call || g_help_execution) { __imp__sub_8227EA50(ctx, base); return; }
  const uint32_t color_pointer = LoadU32(base, ctx.r1.u32 + 84);
  HelpImmediate(ctx, base, "textured-quad", LoadU32(base, color_pointer), __imp__sub_8227EA50);
}
extern "C" void sub_8227EA50(PPCContext& ctx, uint8_t* base) {
  gta4::aspect::DrawQuad(ctx, base, AspectTraceTexturedQuad, true);
}
static void AspectTraceWindow(PPCContext& ctx, uint8_t* base) {
  HelpImmediate(ctx, base, "window", ctx.r7.u32, __imp__sub_8224D1E0);
}
extern "C" void sub_8224D1E0(PPCContext& ctx, uint8_t* base) {
  gta4::aspect::DrawWindow(ctx, base, AspectTraceWindow);
}
extern "C" void sub_821F6550(PPCContext& ctx, uint8_t* base) {
  // Window rendering flushes preceding glyphs. Do not misattribute that flush
  // to the background's native draws (.9 sub_8224D1E0 starts with this call).
  auto* previous = g_help_execution;
  g_help_execution = nullptr;
  __imp__sub_821F6550(ctx, base);
  g_help_execution = previous;
}
