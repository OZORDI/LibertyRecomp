// Called only for an exact mailbox image selected by the native producer.
// Existing swapchain readback storage is reused; no second readback allocation.
namespace {
const std::filesystem::path& FramePixelProbeDirectory() {
  static const std::filesystem::path path = [] {
    const char* value = std::getenv("REX_FRAME_PIXEL_PROBE_OUTPUT");
    return value && *value ? std::filesystem::path(value) : std::filesystem::path{};
  }();
  return path;
}

// P is the presenter's private per-submission probe; identity and bytes have the
// same lifetime. No lookup of "latest frame" is permitted at completion time.
template <typename P>
void WriteFramePixelProbe(const P& probe, uint64_t completed_submission, bool visible) {
  const auto& identity = probe.frame_probe;
  if (!identity.valid()) return;
  if (!FramePixelProbeReadable(probe.pending, probe.paint_submission, completed_submission,
                               probe.mapping != nullptr, visible) ||
      !probe.frame_region.valid() || FramePixelProbeDirectory().empty()) {
    REXLOG_WARN("gta4-frame-pixel: point=unavailable run={} frame={} source={} paint-submission={} reason=incomplete-or-unreadable",
                identity.run, identity.frame, identity.source_sequence, probe.paint_submission);
    return;
  }
  const size_t stride = GetDiagnosticSwapchainProbePixelStride(probe.format);
  if ((stride != 4 && stride != 8) || probe.memory_size < 256 * stride) {
    REXLOG_WARN("gta4-frame-pixel: point=unavailable run={} frame={} source={} reason=unsupported-format-or-capacity",
                identity.run, identity.frame, identity.source_sequence);
    return;
  }
  const auto& directory = FramePixelProbeDirectory();
  std::error_code error;
  std::filesystem::create_directories(directory, error);
  const auto stem = fmt::format("swap-r{}-f{}-s{}-e{}-p{}", identity.run, identity.frame,
                                identity.source_sequence, probe.swapchain_epoch, probe.paint_submission);
  const auto binary = directory / (stem + ".bin");
  const auto metadata = directory / (stem + ".json");
  const auto binary_tmp = directory / (stem + ".bin.tmp");
  const auto metadata_tmp = directory / (stem + ".json.tmp");
  const std::span<const uint8_t> bytes(probe.mapping, 256 * stride);
  if (!error) {
    std::ofstream data(binary_tmp, std::ios::binary);
    data.write(reinterpret_cast<const char*>(bytes.data()), std::streamsize(bytes.size()));
    data.close();
    if (!data) error = std::make_error_code(std::errc::io_error);
  }
  if (!error) std::filesystem::rename(binary_tmp, binary, error);
  const auto& rect = probe.frame_region.rectangle;
  const auto& output = probe.frame_region.output_rectangle;
  const auto& region = identity.normalized_region;
  if (!error) {
    std::ofstream meta(metadata_tmp);
    meta << fmt::format(
        "{{\"schema\":1,\"run\":{},\"frame\":{},\"source_sequence\":{},\"fixture\":{},"
        "\"title_present_id\":{},\"native_submission\":{},\"guest_image\":{},\"guest_view\":{},"
        "\"guest_width\":{},\"guest_height\":{},\"normalized_region\":[{},{},{},{}],"
        "\"mailbox\":{},\"mailbox_version\":{},\"paint_attempt\":{},\"paint_submission\":{},"
        "\"completed_paint_submission\":{},\"paint_slot\":{},\"swapchain_epoch\":{},\"swapchain_index\":{},"
        "\"swapchain_image\":{},\"format\":{},\"swapchain_width\":{},\"swapchain_height\":{},"
        "\"rectangle\":[{},{},{},{}],\"output_rectangle\":[{},{},{},{}],\"grid_axis\":16,\"bytes_per_texel\":{},"
        "\"effect_count\":{},\"final_effect\":{},\"ui_drawers\":{},\"guest_pass\":{},\"full_black_fallback\":{},"
        "\"acquire_result\":{},\"submit_result\":{},\"present_result\":{},\"gpu_completed\":true,"
        "\"present_queued\":{},\"physical_scanout_verified\":false,\"checksum_fnv1a64\":\"{:016X}\",\"binary\":\"{}.bin\"}}\n",
        identity.run, identity.frame, identity.source_sequence, identity.fixture,
        probe.provenance.title_present_id, identity.native_submission, identity.guest_image, probe.frame_guest_view,
        identity.width, identity.height, region[0], region[1], region[2], region[3],
        probe.frame_mailbox, probe.frame_mailbox_version, probe.paint_attempt, probe.paint_submission,
        completed_submission, probe.frame_paint_slot, probe.swapchain_epoch, probe.swapchain_image,
        probe.frame_swapchain_handle, uint32_t(probe.format), probe.frame_swap_width, probe.frame_swap_height,
        rect[0], rect[1], rect[2], rect[3], output[0], output[1], output[2], output[3], stride,
        probe.frame_effect_count, probe.frame_final_effect, probe.frame_ui_drawers, probe.guest_pass, probe.full_black_fallback,
        probe.frame_acquire_result, probe.frame_submit_result, probe.frame_present_result,
        probe.frame_present_result == VK_SUCCESS || probe.frame_present_result == VK_SUBOPTIMAL_KHR,
        FramePixelProbeChecksum(bytes), stem);
    meta.close();
    if (!meta) error = std::make_error_code(std::errc::io_error);
  }
  if (!error) std::filesystem::rename(metadata_tmp, metadata, error);
  REXLOG_INFO("gta4-frame-pixel: point={} run={} frame={} source={} guest-image={} mailbox={} version={} native-submission={} "
              "paint-submission={} completed={} epoch={} image={} format={} present={} metadata={}",
              error ? "unavailable" : "gpu-complete", identity.run, identity.frame, identity.source_sequence,
              identity.guest_image, probe.frame_mailbox, probe.frame_mailbox_version, identity.native_submission,
              probe.paint_submission, completed_submission, probe.swapchain_epoch, probe.frame_swapchain_handle,
              uint32_t(probe.format), probe.frame_present_result, metadata.string());
}
} // namespace
