// Asset offsets: grmShader parameters +20/count +28/types +36/names +52;
// grcTexture name +20. Generated .65 sub_828C6620 receives material/model/ordinal.
thread_local FireTraceContext g_fire_context;
thread_local uint8_t* g_fire_base = nullptr;
thread_local uint64_t g_fire_submissions = 0;
std::atomic<uint64_t> g_fire_next_event{1};

bool FireReadable(uint8_t* base, uint32_t address, size_t bytes) {
  if (!base || !address || !bytes || uint64_t(address) + bytes > uint64_t(UINT32_MAX) + 1) return false;
  auto* kernel = REX_KERNEL_STATE();
  auto* memory = kernel ? kernel->memory() : nullptr;
  auto* heap = memory ? memory->LookupHeap(address) : nullptr;
  const auto last = uint32_t(uint64_t(address) + bytes - 1);
  if (!heap || heap != memory->LookupHeap(last)) return false;
  using rex::memory::PageAccess;
  const auto access = heap->QueryRangeAccess(address, last);
  return access == PageAccess::kReadOnly || access == PageAccess::kReadWrite ||
         access == PageAccess::kExecuteReadOnly || access == PageAccess::kExecuteReadWrite;
}
std::string FireGuestName(uint8_t* base, uint32_t address) {
  std::string result;
  const bool whole_readable = FireReadable(base, address, 95);
  for (uint32_t i = 0; i < 95; ++i) {
    if (!whole_readable && !FireReadable(base, address + i, 1)) return {};
    const auto ch = LoadU8(base, address + i);
    if (!ch) return result;
    if (ch < 32 || ch > 126) return {};
    result.push_back(ch >= 'A' && ch <= 'Z' ? char(ch + ('a' - 'A')) : char(ch));
  }
  return {};
}
uint32_t FireGuestFrame(uint8_t* base) {
  if (!FireReadable(base, kDeferredDeviceGlobal, 4)) return 0;
  const uint32_t device = LoadU32(base, kDeferredDeviceGlobal);
  return FireReadable(base, device, kGuestDeviceSize) ? LoadU32(base, device + kSubmittedFrameOffset) : 0;
}
bool FireMaterial(uint8_t* base, uint32_t material, FireTraceContext& output) {
  if (!FireTraceConfig().enabled || !FireReadable(base, material, 88)) return false;
  const auto count = LoadU32(base, material + 28);
  const auto values = LoadU32(base, material + 20);
  const auto types = LoadU32(base, material + 36);
  const auto names = LoadU32(base, material + 52);
  if (!count || count > 64 || !FireReadable(base, values, size_t(count) * 4) ||
      !FireReadable(base, types, count) || !FireReadable(base, names, size_t(count) * 4)) return false;
  for (uint32_t index = 0; index < count; ++index) {
    if (LoadU8(base, types + index) != 0) continue;
    const auto wrapper = LoadU32(base, values + index * 4);
    if (!FireReadable(base, wrapper, 28)) continue;
    const auto name = FireGuestName(base, LoadU32(base, wrapper + 20));
    if (!FireTextureName(name)) continue;
    output.material = material; output.texture_wrapper = wrapper;
    output.bucket = LoadU8(base, material + 9);
    std::memcpy(output.texture_name.data(), name.data(), name.size());
    return true;
  }
  return false;
}
void FireModelEntry(PPCContext& ctx, uint8_t* base) {
  if (!FireTraceConfig().enabled) return;
  const auto frame = FireGuestFrame(base);
  if (frame % FireTraceConfig().interval) return;
  const uint32_t model = ctx.r3.u32, group = ctx.r4.u32;
  if (!FireReadable(base, model, 28) || !FireReadable(base, group, 16)) return;
  const auto count = LoadU16(base, model + 26);
  const auto slots = LoadU32(base, model + 16), materials = LoadU32(base, group + 8);
  const auto material_count = LoadU16(base, group + 12);
  if (!count || count > 128 || !material_count || material_count > 128 ||
      !FireReadable(base, slots, size_t(count) * 2) || !FireReadable(base, materials, size_t(material_count) * 4)) return;
  for (uint32_t index = 0; index < count; ++index) {
    const auto slot = LoadU16(base, slots + index * 2);
    FireTraceContext c;
    if (slot >= material_count || !FireMaterial(base, LoadU32(base, materials + slot * 4), c)) continue;
    FireTraceLog("model-candidate", fmt::format("guest-frame={} caller={:08X} model={:08X} group={:08X} ordinal={} material={:08X} requested-bucket={} material-bucket={} texture={} note=before-generated-culling-and-bucket-selection",
      frame, uint32_t(ctx.lr), model, group, index, c.material, ctx.r5.u32, c.bucket, c.texture_name.data()));
  }
}
struct ScopedFireMaterial {
  FireTraceContext previous = g_fire_context;
  uint8_t* previous_base = g_fire_base;
  uint64_t before = g_fire_submissions;
  bool selected = false, verbose = false;
  ScopedFireMaterial(PPCContext& ctx, uint8_t* base) {
    if (!FireTraceConfig().enabled) return;
    FireTraceContext c;
    selected = FireMaterial(base, ctx.r3.u32, c);
    if (!selected) return;
    c.occurrence = g_fire_next_event.fetch_add(1, std::memory_order_relaxed);
    c.event = c.occurrence; c.model = ctx.r4.u32; c.ordinal = ctx.r5.u32; c.caller = uint32_t(ctx.lr);
    c.guest_frame = FireGuestFrame(base); c.device = LoadU32(base, kDeferredDeviceGlobal);
    if (FireReadable(base, c.model, 28)) {
      const auto geometries = LoadU32(base, c.model + 4);
      const auto count = LoadU16(base, c.model + 26);
      if (c.ordinal < count && FireReadable(base, geometries, size_t(count) * 4)) c.geometry = LoadU32(base, geometries + c.ordinal * 4);
    }
    g_fire_context = c; g_fire_base = base;
    static thread_local std::unordered_set<uint32_t> announced;
    verbose = !announced.contains(c.material) || c.guest_frame % FireTraceConfig().interval == 0;
    if (announced.size() < 4096) announced.insert(c.material);
    if (verbose) FireTraceLog("material-enter", fmt::format("occurrence={} guest-frame={} caller={:08X} model={:08X} geometry={:08X} ordinal={} material={:08X} bucket={} texture-wrapper={:08X} texture={}",
      c.occurrence,c.guest_frame,c.caller,c.model,c.geometry,c.ordinal,c.material,c.bucket,c.texture_wrapper,c.texture_name.data()));
  }
  ~ScopedFireMaterial() {
    if (!selected) return;
    if (verbose) FireTraceLog("material-exit", fmt::format("occurrence={} guest-frame={} model={:08X} material={:08X} submissions={}",
      g_fire_context.occurrence,g_fire_context.guest_frame,g_fire_context.model,g_fire_context.material,g_fire_submissions-before));
    g_fire_context = previous; g_fire_base = previous_base;
  }
};
void FireMaterialPass(PPCContext& ctx, uint8_t* base, bool after) {
  if (!g_fire_context.occurrence) return;
  if (FireReadable(base, kActiveDeferredTechniqueGlobal, 4)) g_fire_context.technique = LoadU32(base, kActiveDeferredTechniqueGlobal);
  if (!after) g_fire_context.pass = ctx.r3.u32;
  if (g_fire_context.guest_frame % FireTraceConfig().interval == 0) FireTraceLog(after ? "pass-applied" : "pass-enter",
    fmt::format("occurrence={} frame={} technique={:08X} pass={:08X} material={:08X}",g_fire_context.occurrence,g_fire_context.guest_frame,g_fire_context.technique,g_fire_context.pass,g_fire_context.material));
}
bool SubmitFireTracedCommand(const void* bytes, size_t size, uint32_t device,
                            const FireTraceContext* captured = nullptr, uint32_t list = 0, uint32_t ordinal = 0) {
  auto c = g_fire_context.occurrence ? g_fire_context : captured ? *captured : FireTraceContext{};
  if (!FireTraceConfig().enabled || !c.occurrence) return SubmitTvTracedCommand(bytes, size, device);
  c.event = g_fire_next_event.fetch_add(1, std::memory_order_relaxed); c.device = device;
  c.replay = list != 0; c.command_list = list; c.replay_ordinal = ordinal;
  if (g_fire_base) c.guest_frame = FireGuestFrame(g_fire_base);
  auto packed = PackFireTraceEnvelope(bytes,size,kTitleCommandAbi,c);
  auto* graphics = GetNativeGraphicsSystem();
  const bool ok = graphics && !packed.empty() && graphics->SubmitTitleCommand(kTitleId,kFireTraceEnvelopeAbi,packed.data(),packed.size());
  ++g_fire_submissions;
  if (!ok || c.guest_frame % FireTraceConfig().interval == 0) {
    CommandHeader h{}; if (size >= sizeof(h)) std::memcpy(&h,bytes,sizeof(h));
    FireTraceLog("title-submit",fmt::format("occurrence={} event={} guest-frame={} type={} accepted={} replay={} list={:08X} ordinal={}",
      c.occurrence,c.event,c.guest_frame,uint32_t(h.type),ok,c.replay,list,ordinal));
  }
  return ok;
}
