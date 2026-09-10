// Opt-in source/halo observations only. No physical-light contribution queries,
// GPU substitutions, frame-clock writes, or changes to game arguments/results.
namespace {
struct BulbSourceObserverConfig {
  bool enabled = false;
  std::filesystem::path arm;
};
const BulbSourceObserverConfig& BulbSourceObserverOptions() {
  static const auto config = [] {
    BulbSourceObserverConfig value;
    const char* enabled = std::getenv("REX_GTA4_BULB_SOURCE_TRACE");
    value.enabled = enabled && std::string_view(enabled) == "1";
    if (const char* arm = std::getenv("REX_GTA4_EMISSION_ARM_FILE")) value.arm = arm;
    return value;
  }();
  return config;
}
bool BulbSourceReadable(uint8_t* base, uint32_t address, size_t bytes) {
  if (!base || !address || !bytes || uint64_t(address) + bytes > uint64_t(UINT32_MAX) + 1) return false;
  auto* runtime = rex::Runtime::instance();
  auto* memory = runtime ? runtime->memory() : nullptr;
  auto* heap = memory ? memory->LookupHeap(address) : nullptr;
  const auto end = uint32_t(uint64_t(address) + bytes - 1);
  if (!heap || memory->LookupHeap(end) != heap) return false;
  using rex::memory::PageAccess;
  const auto access = heap->QueryRangeAccess(address, end);
  return access == PageAccess::kReadOnly || access == PageAccess::kReadWrite ||
         access == PageAccess::kExecuteReadOnly || access == PageAccess::kExecuteReadWrite;
}
uint32_t BulbSourceGuestFrame(uint8_t* base) {
  if (!BulbSourceReadable(base, kDeferredDeviceGlobal, 4)) return 0;
  const auto device = LoadU32(base, kDeferredDeviceGlobal);
  return BulbSourceReadable(base, device, kGuestDeviceSize) ? LoadU32(base, device + kSubmittedFrameOffset) : 0;
}
bool BulbSourceObservationActive(uint8_t* base) {
  const auto& config = BulbSourceObserverOptions();
  if (!config.enabled || !IsNativeMode() || !rex::diagnostics::IsEnabled(rex::diagnostics::Category::kLogging)) return false;
  std::error_code error;
  if (!config.arm.empty() && (!std::filesystem::is_regular_file(config.arm, error) || error)) return false;
  const auto frame = BulbSourceGuestFrame(base);
  if (!frame) return false;
  static std::atomic<uint32_t> first{0};
  uint32_t expected = 0; first.compare_exchange_strong(expected, frame, std::memory_order_relaxed);
  return uint64_t(frame) <= uint64_t(first.load(std::memory_order_relaxed)) + 120;
}
struct BulbSourceObserverContext {
  uint64_t event = 0;
  uint32_t frame = 0, object = 0, transform = 0, queued = 0;
};
thread_local BulbSourceObserverContext* g_bulb_source_observer = nullptr;
std::atomic<uint64_t> g_bulb_source_event{0}, g_bulb_queue_event{0}, g_bulb_batch_event{0};
std::string BulbSourceFloats(uint8_t* base, uint32_t address, uint32_t count) {
  if (!count || count > 16 || !BulbSourceReadable(base,address,size_t(count)*4)) return "unreadable";
  std::string result;
  for (uint32_t i=0;i<count;++i) {
    const auto bits = LoadU32(base,address+i*4);
    result += fmt::format("{}{:08X}:{:.8g}",i?",":"",bits,std::bit_cast<float>(bits));
  }
  return result;
}
void TraceBulbCoronaBatch(uint8_t* base, std::string_view phase, uint64_t event) {
  if (!event) return;
  // Generated sub_822072D0 selects one of the queued counts with this index.
  const auto selector = LoadU32(base,0x82B307A4);
  if (selector > 1 || !BulbSourceReadable(base,0x82BAFA80,20)) {
    REXLOG_INFO("gta4-bulb-source: point=corona-batch event={} phase={} selector={} state=unsupported",event,phase,selector);
    return;
  }
  REXLOG_INFO("gta4-bulb-source: point=corona-batch event={} phase={} guest-frame={} selector={} selected-count={} count0={} count1={} producer-count={} "
              "physical-contribution-not-measured=true",
              event,phase,BulbSourceGuestFrame(base),selector,LoadU32(base,0x82BAFA80+selector*4),
              LoadU32(base,0x82BAFA80),LoadU32(base,0x82BAFA84),LoadU32(base,0x82BAFA8C));
}
uint64_t BeginBulbCoronaBatch(uint8_t* base) {
  if (!BulbSourceObservationActive(base)) return 0;
  const auto event=g_bulb_batch_event.fetch_add(1,std::memory_order_relaxed)+1;
  if(event>128)return 0;
  TraceBulbCoronaBatch(base,"before-original",event);return event;
}
} // namespace

extern "C" void sub_822077D8(PPCContext& ctx, uint8_t* base) {
  if (!BulbSourceObservationActive(base)) { CallBulbSourceOriginalWithProvenance(ctx,base); return; }
  const auto event=g_bulb_source_event.fetch_add(1,std::memory_order_relaxed)+1;
  if(event>4096){CallBulbSourceOriginalWithProvenance(ctx,base);return;}
  BulbSourceObserverContext source{event,BulbSourceGuestFrame(base),ctx.r3.u32,ctx.r4.u32,0};
  auto* previous=g_bulb_source_observer;g_bulb_source_observer=&source;
  const auto position=BulbSourceFloats(base,source.transform+48,4);
  REXLOG_INFO("gta4-bulb-source: point=entity-begin event={} guest-frame={} caller={:08X} object={:08X} transform={:08X} "
              "position={} input-scales={:.8g},{:.8g},{:.8g} r8={:08X} r9={:08X} r10={:08X}",
              event,source.frame,ctx.lr,source.object,source.transform,position,ctx.f1.f64,ctx.f2.f64,ctx.f3.f64,ctx.r8.u32,ctx.r9.u32,ctx.r10.u32);
  CallBulbSourceOriginalWithProvenance(ctx,base);
  REXLOG_INFO("gta4-bulb-source: point=entity-end event={} guest-frame={} object={:08X} corona-queue-calls={} "
              "no-corona-is-not-no-room-light=true",event,source.frame,source.object,source.queued);
  g_bulb_source_observer=previous;
}
extern "C" void sub_82207118(PPCContext& ctx,uint8_t* base) {
  auto* source=g_bulb_source_observer;
  if(!source){__imp__sub_82207118(ctx,base);return;}
  const auto scale_output=ctx.r5.u32;
  const auto caller=ctx.lr;
  __imp__sub_82207118(ctx,base);
  REXLOG_INFO("gta4-bulb-source: point=source-selection event={} guest-frame={} caller={:08X} return={} output-scale={} "
              "boolean-meaning=raw-original-result",
              source->event,source->frame,caller,ctx.r3.u32,BulbSourceFloats(base,scale_output,1));
}
extern "C" void sub_82207008(PPCContext& ctx,uint8_t* base) {
  if (!BulbSourceObservationActive(base)) { __imp__sub_82207008(ctx,base); return; }
  const auto event=g_bulb_queue_event.fetch_add(1,std::memory_order_relaxed)+1;
  if(event>4096){__imp__sub_82207008(ctx,base);return;}
  auto* source=g_bulb_source_observer;
  if(source)++source->queued;
  const auto frame=BulbSourceGuestFrame(base);
  const auto caller=ctx.lr;
  const auto before=LoadU32(base,0x82BAFA8C);
  const auto position=BulbSourceFloats(base,ctx.r8.u32&~uint32_t{15},4);
  // r31 holds the attribute returned by the generated entity virtual getter at
  // these two call sites only. Never infer it for scripts or vehicle callers.
  const bool entity_call=source && (caller==0x82207DDC || caller==0x82207E30);
  const uint32_t attribute=entity_call?ctx.r31.u32:0;
  const auto rgb=std::array<uint32_t,3>{ctx.r4.u8,ctx.r5.u8,ctx.r6.u8};
  const auto size=ctx.f1.f64;
  std::string attributes="unknown";
  if(entity_call && BulbSourceReadable(base,attribute,96))
    attributes=fmt::format("flags{:08X}:variant{}:params{}",LoadU32(base,attribute+76),LoadU8(base,attribute+88),BulbSourceFloats(base,attribute+36,10));
  __imp__sub_82207008(ctx,base);
  const auto after=LoadU32(base,0x82BAFA8C);
  const bool advanced=before<768 && after==before+1;
  REXLOG_INFO("gta4-bulb-source: point=corona-enqueue event={} source={} guest-frame={} caller={:08X} before={} after={} accepted={} "
              "position={} rgb={},{},{} size={:.8g} attribute={:08X} details={} gpu-drawn=unknown",
              event,source?source->event:0,frame,caller,before,after,advanced,position,rgb[0],rgb[1],rgb[2],size,attribute,attributes);
}
