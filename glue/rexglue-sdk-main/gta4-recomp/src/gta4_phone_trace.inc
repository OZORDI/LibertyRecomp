// Included inside gta4_native_hooks.cpp's private namespace, after guest access helpers.
// Only immutable metadata and bounded diagnostic bookkeeping live here.
std::atomic<uint8_t*> g_phone_guest_base{nullptr};
std::atomic<uint64_t> g_phone_run{0};
std::atomic<uint64_t> g_phone_event{0};
std::atomic<uint32_t> g_phone_frames_remaining{0};
std::atomic<uint32_t> g_phone_command_count{0};
std::mutex g_phone_arm_mutex;
bool g_phone_armed_once = false;
std::filesystem::file_time_type g_phone_last_arm_write{};
uint32_t g_phone_arm_runs = 0;
thread_local PhoneTraceContext g_phone_context{};
std::mutex g_phone_dc_mutex;
std::unordered_map<uint32_t, PhoneTraceContext> g_phone_dc_origins;

bool PhoneCaptureActive() {
  return PhoneTraceConfig().enabled &&
         g_phone_frames_remaining.load(std::memory_order_acquire) != 0;
}
uint64_t NextPhoneEvent() { return g_phone_event.fetch_add(1, std::memory_order_relaxed) + 1; }

PhoneGuestState CapturePhoneGuestState(uint8_t* base, uint32_t device) {
  PhoneGuestState s{};
  if (!base || !device) return s;
  for (uint32_t i = 0; i < kRenderTargetCount; ++i)
    s.words[i] = LoadU32(base, device + kRenderTargetBase * sizeof(uint32_t) + i * sizeof(uint32_t));
  s.words[4] = LoadU32(base, device + kDepthStencilOffset);
  s.words[5] = LoadU32(base, device + kVertexDeclarationOffset);
  s.words[6] = LoadU32(base, device + kIndexBufferOffset);
  for (uint32_t i = 0; i < 6; ++i) s.words[7 + i] = LoadU32(base, device + 12640 + i * sizeof(uint32_t));
  s.words[13] = LoadU32(base, device + 11848);
  for (uint32_t i = 0; i < 4; ++i) s.words[14 + i] = LoadU32(base, device + 12668 + i * sizeof(uint32_t));
  s.words[18] = LoadU32(base, kCurrentViewportGlobal);
  s.words[19] = LoadU32(base, 0x831D4DD0);
  s.fixed_hash = XXH3_64bits(GuestPointer(base, device + kReplayFixedStateOffset), kReplayFixedStateSize);
  s.constants_hash = XXH3_64bits(GuestPointer(base, device + kReplayShaderConstantsOffset), kReplayShaderConstantsSize);
  s.fetch_hash = XXH3_64bits(GuestPointer(base, device + kReplayFetchStateOffset), kReplayFetchStateSize);
  s.booleans = (uint64_t(LoadU32(base, device + kReplayVertexBooleansOffset)) << 32) |
               LoadU32(base, device + kReplayPixelBooleansOffset);
  return s;
}

void MaybeArmPhoneTrace(uint8_t* base) {
  if (!PhoneTraceConfig().enabled || !IsNativeMode() || PhoneCaptureActive()) return;
  if (!PhoneTraceConfig().allow_closed && !LoadU8(base, 0x831D4DD4)) return;
  std::lock_guard lock(g_phone_arm_mutex);
  if (PhoneCaptureActive() || g_phone_arm_runs >= 8) return;
  if (!PhoneTraceConfig().arm_file.empty()) {
    std::error_code error;
    const auto stamp = std::filesystem::last_write_time(PhoneTraceConfig().arm_file, error);
    if (error || (g_phone_armed_once && stamp == g_phone_last_arm_write)) return;
    g_phone_last_arm_write = stamp;
  } else if (g_phone_armed_once) {
    return;
  }
  g_phone_armed_once = true;
  ++g_phone_arm_runs;
  g_phone_command_count.store(0, std::memory_order_relaxed);
  const uint64_t run = uint64_t(std::chrono::system_clock::now().time_since_epoch().count());
  g_phone_guest_base.store(base, std::memory_order_relaxed);
  g_phone_run.store(run, std::memory_order_relaxed);
  g_phone_frames_remaining.store(PhoneTraceConfig().frames, std::memory_order_release);
  PhoneTraceLog("arm-state", fmt::format("run={} cycle={} phone-visible={} lineage={} allow-closed={}",
      run, g_phone_arm_runs, LoadU8(base, 0x831D4DD4), PhoneTraceConfig().lineage, PhoneTraceConfig().allow_closed));
  PhoneTraceLog("armed", fmt::format("run={} frames={} command-limit={} readbacks={} probe-first={} probe-count={} phone-target-id={} phone-wrapper={:08X}",
      run, PhoneTraceConfig().frames, PhoneTraceConfig().command_limit, PhoneTraceConfig().readbacks,
      PhoneTraceConfig().probe_first, PhoneTraceConfig().probe_count,
      LoadU32(base, 0x831D4DDC), LoadU32(base, 0x831D4DD0)));
}

PhoneTraceContext NewPhoneContext(uint32_t device) {
  PhoneTraceContext c = g_phone_context;
  c.run = g_phone_run.load(std::memory_order_relaxed);
  c.event = NextPhoneEvent();
  c.device = device;
  uint8_t* base = g_phone_guest_base.load(std::memory_order_relaxed);
  if (base && device) {
    c.guest_frame = LoadU32(base, device + kSubmittedFrameOffset);
    c.technique = LoadU32(base, kActiveDeferredTechniqueGlobal);
    c.pass = LoadU32(base, 0x831C3DDC);
    c.applied = CapturePhoneGuestState(base, device);
  }
  return c;
}

class ScopedPhoneTrace {
 public:
  ScopedPhoneTrace(PhoneTraceScope scope, uint8_t* base, uint32_t object, uint32_t caller,
                   const PhoneTraceContext* origin = nullptr)
      : previous_(g_phone_context), active_(PhoneCaptureActive()), base_(base) {
    if (!active_) return;
    if (origin) g_phone_context = *origin;
    g_phone_context.run = g_phone_run.load(std::memory_order_relaxed);
    g_phone_context.event = NextPhoneEvent();
    if (!g_phone_context.occurrence) g_phone_context.occurrence = g_phone_context.event;
    g_phone_context.scope = uint32_t(scope);
    g_phone_context.object = object;
    g_phone_context.caller = caller;
    if (scope == PhoneTraceScope::kPhoneModel) g_phone_context.model = object;
    event_ = g_phone_context.event;
    PhoneTraceLog("guest-enter", fmt::format("run={} event={} occurrence={} scope={} object={:08X} caller={:08X} dc-event={} dc={:08X} viewport={:08X}",
        g_phone_context.run, event_, g_phone_context.occurrence, g_phone_context.scope, object,
        caller, g_phone_context.dc_event, g_phone_context.dc, LoadU32(base_, kCurrentViewportGlobal)));
  }
  ~ScopedPhoneTrace() {
    if (active_) {
      PhoneTraceLog("guest-exit", fmt::format("run={} event={} occurrence={} scope={} viewport={:08X}",
          g_phone_context.run, event_, g_phone_context.occurrence, g_phone_context.scope,
          LoadU32(base_, kCurrentViewportGlobal)));
      g_phone_context = previous_;
    }
  }
 private:
  PhoneTraceContext previous_{};
  bool active_ = false;
  uint8_t* base_ = nullptr;
  uint64_t event_ = 0;
};

PhoneTraceContext ConsumePhoneDcOrigin(uint8_t* base, uint32_t dc) {
  PhoneTraceContext origin{};
  if (!PhoneCaptureActive() || !dc) return origin;
  const uint32_t token = LoadU32(base, dc + 4);
  std::lock_guard lock(g_phone_dc_mutex);
  auto entry = g_phone_dc_origins.find(dc);
  if (entry != g_phone_dc_origins.end()) {
    if (entry->second.dc_token == token) origin = entry->second;
    else PhoneTraceLog("incomplete", fmt::format("run={} reason=dc-token-mismatch dc={:08X} expected={:08X} actual={:08X}",
             g_phone_run.load(), dc, entry->second.dc_token, token));
    g_phone_dc_origins.erase(entry);
  }
  origin.dc = dc;
  origin.dc_token = token;
  return origin;
}

bool SubmitPhoneTracedCommand(const void* bytes, size_t size, uint32_t device,
                               PhoneTraceContext* replay = nullptr) {
  auto* graphics = GetNativeGraphicsSystem();
  if (!graphics) return false;
  if (!PhoneCaptureActive())
    return gta4::gpu_pass::Submit(graphics, kTitleCommandAbi, bytes, size);
  const uint32_t count = g_phone_command_count.fetch_add(1, std::memory_order_relaxed);
  if (count >= PhoneTraceConfig().command_limit) {
    if (g_phone_frames_remaining.exchange(0))
      PhoneTraceLog("incomplete", fmt::format("run={} reason=command-budget-exhausted count={}", g_phone_run.load(), count));
    return gta4::gpu_pass::Submit(graphics, kTitleCommandAbi, bytes, size);
  }
  PhoneTraceContext context = replay ? *replay : NewPhoneContext(device);
  if (replay) context.applied = CapturePhoneGuestState(g_phone_guest_base.load(), device);
  CommandHeader header{};
  std::memcpy(&header, bytes, sizeof(header));
  PhoneTraceLog("guest-submit", fmt::format("run={} event={} occurrence={} type={} scope={} dc-event={} model={:08X} caller={:08X} replay={} captured={} command-list={:08X} material={:08X} technique={:08X} pass={:08X} mode={} device={:08X} frame={}",
      context.run, context.event, context.occurrence, uint32_t(header.type), context.scope,
      context.dc_event, context.model, context.caller, context.replay, context.captured_event,
      context.command_list, context.material, context.technique, context.pass, context.mode,
      device, context.guest_frame));
  if (header.type == CommandType::kClear || header.type == CommandType::kDrawPrimitive ||
      header.type == CommandType::kDrawPrimitiveUp || header.type == CommandType::kDrawIndexedPrimitive)
    PhoneTraceLog("guest-applied", fmt::format("run={} event={} {}", context.run, context.event, PhoneGuestStateText(context.applied)));
  const auto envelope = PackPhoneTraceEnvelope(bytes, size, kTitleCommandAbi, context);
  const bool accepted = gta4::gpu_pass::Submit(graphics, kPhoneTraceEnvelopeAbi, envelope.data(), envelope.size());
  PhoneTraceLog("guest-result", fmt::format("run={} event={} accepted={}", context.run, context.event, accepted));
  if (accepted && header.type == CommandType::kPresent) {
    uint32_t remaining = g_phone_frames_remaining.load();
    for (; remaining && !g_phone_frames_remaining.compare_exchange_weak(remaining, remaining - 1);) {}
    if (remaining == 1) {
      std::lock_guard lock(g_phone_dc_mutex);
      PhoneTraceLog("window-closed", fmt::format("run={} event={} commands={} unmatched-dc={} gpu-completion=pending", context.run, context.event, count + 1, g_phone_dc_origins.size()));
      g_phone_dc_origins.clear();
    }
  }
  return accepted;
}
