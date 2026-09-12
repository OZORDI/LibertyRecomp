#include "graphics/gta4_native/native_profile_detail.h"
#include <filesystem>
#include <fstream>
namespace p = rex::graphics::gta4_native::profile;
namespace a = rex::graphics::gta4_native::attribution;
namespace f = rex::graphics::gta4_native::performance;
int main(int argc, char **argv) {
  if (argc != 2)
    return 2;
  const std::filesystem::path directory(argv[1]);
  std::filesystem::create_directories(directory);
  p::CaptureMetadata metadata;
  metadata.capture_id = 12345;
  metadata.host_frequency = 1000000;
  metadata.gpu_timestamp_period_ns = 1;
  metadata.gpu_timestamp_valid_bits = 36;
  metadata.device_name = "test device \"quoted\"\nline";
  metadata.shader_names = {{123, "vertex.hlsl"}, {456, "pixel.hlsl"}};
  std::vector<p::FrameDetail> details;
  std::ofstream csv(directory / "native-performance-latest.csv");
  csv << "frame,capture_index,schema_version,capture_id,capture_sequence,"
         "native_submission,frame_slot,gpu_frame_ms,cpu_frame_interval_ms,cpu_"
         "publish_ms,counter_texture_images_live,counter_texture_image_"
         "evictions,flags\n";
  for (uint32_t i = 0; i < 4; ++i) {
    p::FrameDetail frame;
    frame.frame = 100 + i;
    frame.sequence = i + 1;
    frame.slot = i % 2;
    frame.submission = i + 15;
    frame.width = 1280;
    frame.height = 800;
    frame.display_width = 2560;
    frame.display_height = 1600;
    p::CpuRecorder recorder;
    const uint64_t start = 1000 + i * 100000;
    recorder.Begin(start);
    auto outer = recorder.Enter(p::CpuOp::kPublishFrame, start);
    recorder.SetPhase(p::CpuPhase::kRecording);
    recorder.SetContext({42, 4, 123, 456});
    auto command = recorder.Enter(p::CpuOp::kRecordCommand, start + 1000);
    auto driver = recorder.Enter(p::CpuOp::kDriverDraw, start + 2000);
    recorder.Leave(driver, start + 6000);
    recorder.Leave(command, start + 8000);
    recorder.Leave(outer, start + 10000);
    frame.cpu = recorder.Finish(start + 10000, 2);
    frame.paint = {i + 1, start - 500, start, 50 + i, 10,     0,
                   10,    20,          30,    500,    99 + i, 0};
    frame.transport.Observe({start - 100, 100, 10, 20, 30}, start, 5);
    frame.detailed_gpu = true;
    frame.query_count = 3;
    frame.query_budget = 16;
    frame.slices.push_back({f::GpuRange::kMirrorReflections, 0, 1, 0, 0,
                            4000000, 0, 4000000, true});
    frame.slices.push_back(
        {f::GpuRange::kComposite, 1, 2, 1, 4000000, 0, 0, 0, false});
    a::NativeResolvedPassRegion region;
    region.region.coarse_range = uint32_t(f::GpuRange::kMirrorReflections);
    region.region.classification = a::NativePassClassification::kKnown;
    region.region.pass_key_count = 1;
    region.region.key.vertex_shader_hash = 123;
    region.region.key.pixel_shader_hash = 456;
    region.region.draw_count = 4;
    region.region.command_start = 1;
    region.region.command_end = 5;
    region.available = true;
    region.elapsed_ticks = 4000000;
    frame.regions.push_back(region);
    region.available = false;
    region.elapsed_ticks = 0;
    frame.regions.push_back(region);
    details.push_back(std::move(frame));
    csv << 100 + i << ',' << i << ",2,12345," << i + 1 << ',' << i + 15 << ','
        << i % 2 << ',';
    if (i != 2)
      csv << 4;
    csv << ',' << (i ? 12 : 1000) << ",10," << 100 + i << ',' << i * 20 << ','
        << (i ? 0 : 1) << '\n';
  }
  csv.close();
  return p::ExportProfileDetails(directory, details, metadata) ? 0 : 1;
}
