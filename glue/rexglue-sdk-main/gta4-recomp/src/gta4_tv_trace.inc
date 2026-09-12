// Included in the native title-hook namespace. No guest writes or render repairs.
std::mutex g_tv_detail_mutex;
TvTraceContext g_tv_detail_state{};
std::atomic<uint32_t> g_tv_detail_remaining{0};
std::atomic<uint64_t> g_tv_detail_events{0};
std::atomic<uint8_t*> g_tv_detail_base{nullptr};
std::filesystem::file_time_type g_tv_detail_arm_stamp{};
uint32_t g_tv_detail_cycles = 0;

uint32_t TvDetailScriptTexture() {
  std::lock_guard lock(g_tv_detail_mutex);
  return g_tv_detail_state.script_texture;
}

bool TvDetailActive() {
  return TvTraceConfig().enabled && g_tv_detail_remaining.load(std::memory_order_acquire);
}
void IdentifyTvTarget(uint8_t* base, uint32_t name, uint32_t wrapper) {
  if (!TvTraceConfig().enabled || name != kTvScriptName || !wrapper) return;
  std::lock_guard lock(g_tv_detail_mutex);
  g_tv_detail_base.store(base, std::memory_order_relaxed);
  g_tv_detail_state.script_wrapper = wrapper;
  g_tv_detail_state.script_surface = LoadU32(base, wrapper + kDeferredWrapperSurfaceOffset);
  g_tv_detail_state.script_texture = LoadU32(base, wrapper + kDeferredWrapperTextureOffset);
  TvTraceLog("target", fmt::format("name=script_rt wrapper={:08X} surface={:08X} texture={:08X}",
      wrapper, g_tv_detail_state.script_surface, g_tv_detail_state.script_texture));
}
void ObserveTvBinding(uint8_t* base, uint32_t player, uint32_t caller, bool after, uint32_t result) {
  if (!TvTraceConfig().enabled || !player) return;
  std::lock_guard lock(g_tv_detail_mutex);
  g_tv_detail_base.store(base, std::memory_order_relaxed);
  if (!TvDetailActive() && g_tv_watching.load(std::memory_order_relaxed) &&
      g_tv_detail_cycles < 4 && g_tv_detail_state.script_texture) {
    std::error_code error;
    auto stamp = TvTraceConfig().arm_file.empty() ? std::filesystem::file_time_type{}
        : std::filesystem::last_write_time(TvTraceConfig().arm_file, error);
    if (!error && ((!g_tv_detail_cycles) ||
         (!TvTraceConfig().arm_file.empty() && stamp != g_tv_detail_arm_stamp))) {
      g_tv_detail_arm_stamp = stamp;
      g_tv_detail_state.run = uint64_t(std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::system_clock::now().time_since_epoch()).count());
      ++g_tv_detail_cycles;
      g_tv_detail_remaining.store(TvTraceConfig().frames, std::memory_order_release);
      TvTraceLog("armed", fmt::format("run={} cycle={} frames={} surface={:08X} texture={:08X}",
          g_tv_detail_state.run, g_tv_detail_cycles, TvTraceConfig().frames,
          g_tv_detail_state.script_surface, g_tv_detail_state.script_texture));
    }
  }
  if (!TvDetailActive()) return;
  auto& state = g_tv_detail_state;
  state.player = player;
  state.decoder = LoadU32(base, player);
  state.frame_table = LoadU32(base, player + 8);
  const uint32_t frame_buffers = LoadU32(base, player + 4);
  const uint32_t count = frame_buffers ? LoadU32(base, frame_buffers) : 0;
  state.frame_index = frame_buffers ? LoadU32(base, frame_buffers + 20) : UINT32_MAX;
  state.ready = LoadU8(base, player + 100);
  state.watching = g_tv_watching.load(std::memory_order_relaxed);
  state.plane_wrappers = {}; state.plane_handles = {};
  if (state.frame_table && state.frame_index < count && count <= 16) {
    for (uint32_t plane = 0; plane < 4; ++plane) {
      const uint32_t wrapper = LoadU32(base, state.frame_table + state.frame_index * 16 + plane * 4);
      state.plane_wrappers[plane] = wrapper;
      // grcTextureXenon's D3D texture, verified in the compiled Lock/Unlock methods.
      state.plane_handles[plane] = wrapper ? LoadU32(base, wrapper + 24) : 0;
    }
  }
  if (!after) state.binding_event = ++g_tv_detail_events;
  TvTraceLog(after ? "binding-after" : "binding-before", fmt::format(
      "run={} binding={} player={:08X} decoder={:08X} caller={:08X} ready={} watching={} "
      "buffer-index={} buffer-count={} table={:08X} wrappers={:08X},{:08X},{:08X},{:08X} "
      "handles={:08X},{:08X},{:08X},{:08X} setup-result={} evidence=effect-binding-not-draw",
      state.run,state.binding_event,player,state.decoder,caller,state.ready,state.watching,
      state.frame_index,count,state.frame_table,state.plane_wrappers[0],state.plane_wrappers[1],
      state.plane_wrappers[2],state.plane_wrappers[3],state.plane_handles[0],state.plane_handles[1],
      state.plane_handles[2],state.plane_handles[3],result));
}
TvTraceContext NewTvTraceContext(uint32_t device) {
  std::lock_guard lock(g_tv_detail_mutex);
  auto context = g_tv_detail_state;
  context.event = ++g_tv_detail_events;
  context.device = device;
  const auto base = g_tv_detail_base.load(std::memory_order_relaxed);
  if (base && device) context.guest_frame = LoadU32(base, device + kSubmittedFrameOffset);
  context.watching = g_tv_watching.load(std::memory_order_relaxed);
  return context;
}
bool SubmitTvTracedCommand(const void* command, size_t size, uint32_t device) {
  if (!TvDetailActive()) return SubmitPhoneTracedCommand(command, size, device);
  const auto context = NewTvTraceContext(device);
  const auto envelope = PackTvTraceEnvelope(command,size,kTitleCommandAbi,context);
  const bool accepted = gta4::gpu_pass::Submit(GetNativeGraphicsSystem(),kTvTraceEnvelopeAbi,envelope.data(),envelope.size());
  if (!accepted) TvTraceLog("incomplete",fmt::format("run={} event={} reason=submit-rejected",
      context.run,context.event));
  const auto& header = *static_cast<const CommandHeader*>(command);
  if (header.type == CommandType::kPresent) {
    uint32_t remaining = g_tv_detail_remaining.load(std::memory_order_relaxed);
    for (; remaining && !g_tv_detail_remaining.compare_exchange_weak(remaining,remaining-1,
         std::memory_order_acq_rel);) {}
    if (remaining == 1) TvTraceLog("window-closed",fmt::format("run={} last-event={} guest-frame={}",
        context.run,context.event,context.guest_frame));
  }
  return accepted;
}
