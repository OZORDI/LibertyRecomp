#pragma once
#include "native_cpu_profile.h"
#include "native_gpu_attribution.h"
#include "native_performance_samples.h"
#include <filesystem>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <string>
#include <utility>

namespace rex::graphics::gta4_native::profile {
struct PaintObservation {
  uint64_t sequence = 0, begin = 0, end = 0, submission = 0, mailbox_version = 0, overwritten = 0;
  uint64_t acquire_ticks = 0, submit_ticks = 0, present_ticks = 0, total_ticks = 0;
  uint32_t guest_frame = 0;
  int32_t result = 0;
};
struct GpuSliceDetail {
  performance::GpuRange range = performance::GpuRange::kUnattributed;
  uint32_t begin_query = UINT32_MAX, end_query = UINT32_MAX, region = UINT32_MAX;
  uint64_t raw_begin = 0, raw_end = 0, relative_begin = 0, elapsed_ticks = 0;
  bool available = false;
};
struct FrameDetail {
  uint32_t frame = 0, slot = UINT32_MAX, width = 0, height = 0, display_width = 0,
           display_height = 0;
  uint64_t sequence = 0, submission = 0, first_command = 0, last_command = 0;
  uint32_t query_count = 0, query_budget = 0, dropped_boundaries = 0, dropped_regions = 0;
  uint64_t readback_ticks = 0, export_snapshot_ticks = 0;
  bool detailed_gpu = false;
  bool modern_shaders = false, disable_tlad_grain = false;
  int32_t query_result = 0;
  CpuFrameData cpu;
  TransportSummary transport;
  PaintObservation paint;
  std::vector<GpuSliceDetail> slices;
  std::vector<attribution::NativeResolvedPassRegion> regions;
};
struct CaptureMetadata {
  uint64_t host_frequency = 0, capture_id = 0;
  double gpu_timestamp_period_ns = 0;
  uint32_t gpu_timestamp_valid_bits = 0, driver_id = 0, process_id = 0;
  bool modern_shaders = false, disable_tlad_grain = false;
  std::string device_name;
  std::vector<std::pair<std::string, std::string>> settings;
  std::vector<std::pair<uint64_t, std::string>> shader_names;
};
inline std::string JsonString(std::string_view value) {
  std::string out = "\"";
  constexpr char hex[] = "0123456789abcdef";
  for (unsigned char c : value) {
    if (c == '"' || c == '\\') {
      out += '\\';
      out += char(c);
    } else if (c < 32) {
      out += "\\u00";
      out += hex[c >> 4];
      out += hex[c & 15];
    } else
      out += char(c);
  }
  out += '"';
  return out;
}
inline std::string HexIdentity(uint64_t v) {
  constexpr char hex[] = "0123456789ABCDEF";
  std::string out(16, '0');
  for (size_t i = 0; i < out.size(); ++i) {
    out[out.size() - 1 - i] = hex[v & 15];
    v >>= 4;
  }
  return out;
}
inline const char* CounterSemantics(performance::Counter c) {
  using C = performance::Counter;
  switch (c) {
    case C::kBufferCaptureReuses:
    case C::kBufferShadowValidations:
    case C::kBufferShadowMismatches:
    case C::kBufferFastPathDisables:
    case C::kTextureImageEvictions:
    case C::kTextureImageEvictedBytes:
    case C::kTextureAllocationRetries:
    case C::kTextureAllocationFailures:
    case C::kTextureAllocationReuses:
    case C::kPipelineSnapshotReuses:
    case C::kShaderSnapshotReuses:
    case C::kZeroDofSkips:
    case C::kPostFxDirectWrites:
      return "process-cumulative";
    case C::kSurfaceImagesLive:
    case C::kSurfaceImageBytes:
    case C::kTextureImagesLive:
    case C::kTextureImageBytes:
    case C::kPersistentBufferResidentBytes:
    case C::kPersistentBufferLiveAllocations:
    case C::kTextureHeapUsage:
    case C::kTextureHeapBudget:
    case C::kProcessPhysicalFootprintBytes:
    case C::kProcessResidentBytes:
    case C::kUploadBufferCapacityBytes:
    case C::kUploadBufferAllocationBytes:
    case C::kPendingTextureReleases:
    case C::kConstantBindingOwners:
    case C::kPipelinesLive:
    case C::kIndexedDescriptorPages:
    case C::kTextureAllocationPoolBytes:
      return "instantaneous-gauge";
    default:
      return "per-sample";
  }
}

// Called only by the export worker after capture completion. No file I/O,
// formatting or JSON construction occurs in draw recording or GPU callbacks.
inline bool ExportProfileDetails(const std::filesystem::path& dir,
                                 const std::vector<FrameDetail>& frames,
                                 const CaptureMetadata& meta) {
  if (!meta.host_frequency || frames.empty() || !std::isfinite(meta.gpu_timestamp_period_ns) ||
      meta.gpu_timestamp_period_ns < 0)
    return false;
  const long double ms = meta.host_frequency ? 1000.0L / meta.host_frequency : 0;
  const long double us = ms * 1000.0L, gpu_ms = meta.gpu_timestamp_period_ns / 1000000.0L;
  std::ofstream cpu(dir / "native-performance-cpu.csv.partial"),
      shaders(dir / "native-performance-cpu-shaders.csv.partial"),
      gpu(dir / "native-performance-gpu-passes.csv.partial"),
      slices(dir / "native-performance-gpu-slices.csv.partial"),
      transport(dir / "native-performance-frames.csv.partial"),
      trace(dir / "native-performance-trace.json.partial"),
      metadata(dir / "native-performance-meta.json.partial");
  if (!cpu || !shaders || !gpu || !slices || !transport || !trace || !metadata)
    return false;
  for (auto* out : {&cpu, &shaders, &gpu, &slices, &transport, &trace, &metadata})
    *out << std::setprecision(12);
  cpu << "frame,sequence,phase,operation,calls,inclusive_ms,self_ms,max_call_ms,max_self_ms,worst_"
         "command,render_phase,vertex_shader,pixel_shader\n";
  shaders << "frame,sequence,render_phase,vertex_shader,pixel_shader,commands,inclusive_ms,self_ms,"
             "max_command_ms,worst_command\n";
  gpu << "frame,sequence,region,range,detail,classification,status,command_begin,command_end,pass_"
         "keys,draws,primitives,gpu_ms,render_phase,reflection_family,vertex_shader,pixel_shader,"
         "shader_family,depth_blend,rt0_handle,rt0_generation,rt0_format,rt0_declared_samples,"
         "depth_handle,depth_generation,depth_format,depth_declared_samples,retail_phase,retail_phase_name,"
         "origin_source,origin_scope,origin_arena,origin_offset,origin_ordinal,origin_list_id\n";
  slices << "frame,sequence,slice,range,status,begin_query,end_query,region,raw_begin,raw_end,"
            "relative_begin_ms,duration_ms\n";
  transport
      << "frame,sequence,slot,submission,render_width,render_height,display_width,display_height,"
         "first_command,last_command,cpu_begin_tick,cpu_end_tick,commands,measured_commands,"
         "producer_capture_sum_ms,capture_lock_sum_ms,queue_lock_sum_ms,backpressure_sum_ms,queue_"
         "dwell_sum_ms,queue_dwell_max_ms,queue_peak,worker_assembly_ms,worker_idle_ms,internal_"
         "flush_ms,internal_flush_count,cpu_scope_clock_reads,clock_probe_ticks,clock_pair_floor_"
         "estimate_ms,cpu_events_omitted,cpu_invalid_scopes,cpu_stack_overflow,shader_keys_"
         "overflow,gpu_detailed,gpu_queries,gpu_query_budget,gpu_dropped_boundaries,gpu_dropped_"
         "regions,gpu_query_result,modern_shaders,disable_tlad_grain,query_readback_ms,profiler_"
         "snapshot_ms,paint_sequence,paint_guest_frame,paint_submission,paint_begin_tick,paint_end_"
         "tick,paint_mailbox_version,paint_observations_overwritten,paint_result,paint_acquire_ms,"
         "paint_submit_ms,paint_present_ms,paint_total_ms\n";
  uint64_t origin = UINT64_MAX;
  for (const auto& frame : frames) {
    if (frame.cpu.enabled)
      origin = std::min(origin, frame.cpu.begin);
    if (frame.paint.sequence)
      origin = std::min(origin, frame.paint.begin);
  }
  if (origin == UINT64_MAX)
    origin = 0;
  trace << "{\"displayTimeUnit\":\"ms\",\"metadata\":{\"clock\":\"host-monotonic\",\"selection\":"
           "\"longest-128-scopes-per-frame; not a full call "
           "trace\",\"gpu_cpu_clock_aligned\":false},\"traceEvents\":[";
  bool first_event = true;
  for (const auto& f : frames) {
    for (const auto& m : f.cpu.measurements) {
      cpu << f.frame << ',' << f.sequence << ',' << CpuPhaseName(m.phase) << ',' << CpuOpName(m.op)
          << ',' << m.calls << ',' << m.inclusive_ticks * ms << ',' << m.self_ticks * ms << ','
          << m.max_ticks * ms << ',' << m.max_self_ticks * ms << ',' << m.worst.command << ','
          << m.worst.render_phase << ',' << HexIdentity(m.worst.vertex_shader) << ','
          << HexIdentity(m.worst.pixel_shader) << '\n';
    }
    for (const auto& s : f.cpu.shaders)
      shaders << f.frame << ',' << f.sequence << ',' << s.render_phase << ','
              << HexIdentity(s.vertex_shader) << ',' << HexIdentity(s.pixel_shader) << ','
              << s.commands << ',' << s.inclusive_ticks * ms << ',' << s.self_ticks * ms << ','
              << s.max_ticks * ms << ',' << s.worst_command << '\n';
    for (const auto& e : f.cpu.events) {
      if (!first_event)
        trace << ',';
      first_event = false;
      trace << "{\"ph\":\"X\",\"pid\":" << meta.process_id
            << ",\"tid\":1,\"name\":" << JsonString(CpuOpName(e.op))
            << ",\"cat\":" << JsonString(CpuPhaseName(e.phase))
            << ",\"ts\":" << (e.begin - origin) * us << ",\"dur\":" << (e.end - e.begin) * us
            << ",\"args\":{\"frame\":" << f.frame << ",\"sequence\":" << f.sequence
            << ",\"self_us\":" << e.self_ticks * us << ",\"command\":" << e.context.command
            << ",\"depth\":" << e.depth
            << ",\"vs\":" << JsonString(HexIdentity(e.context.vertex_shader))
            << ",\"ps\":" << JsonString(HexIdentity(e.context.pixel_shader)) << "}}";
    }
    if (f.paint.sequence && f.paint.end >= f.paint.begin && f.paint.begin >= origin) {
      if (!first_event)
        trace << ',';
      first_event = false;
      trace << "{\"ph\":\"X\",\"pid\":" << meta.process_id
            << ",\"tid\":2,\"name\":\"presenter CPU paint\",\"cat\":\"presentation\",\"ts\":"
            << (f.paint.begin - origin) * us << ",\"dur\":" << (f.paint.end - f.paint.begin) * us
            << ",\"args\":{\"observed_during_frame\":" << f.frame
            << ",\"paint_guest_frame\":" << f.paint.guest_frame
            << ",\"paint_submission\":" << f.paint.submission
            << ",\"paint_sequence\":" << f.paint.sequence
            << ",\"overwritten_observations\":" << f.paint.overwritten
            << ",\"result\":" << f.paint.result << "}}";
    }
    for (size_t i = 0; i < f.regions.size(); ++i) {
      const auto& r = f.regions[i];
      const auto& k = r.region.key;
      const auto& rt = k.color_targets[0];
      const auto& depth = k.depth_target;
      gpu << f.frame << ',' << f.sequence << ',' << i << ','
          << performance::GpuRangeName(performance::GpuRange(r.region.coarse_range)) << ','
          << attribution::NativePassRegionDetailName(r.region.detail) << ','
          << attribution::NativePassClassificationName(r.region.classification) << ','
          << (!r.available      ? "unavailable"
              : r.elapsed_ticks ? "approximate"
                                : "collapsed-or-zero")
          << ',' << r.region.command_start << ',' << r.region.command_end << ','
          << r.region.pass_key_count << ',' << r.region.draw_count << ','
          << r.region.primitive_count << ',';
      if (r.available)
        gpu << r.elapsed_ticks * gpu_ms;
      gpu << ',' << k.render_phase << ',' << k.reflection_family << ','
          << HexIdentity(k.vertex_shader_hash) << ',' << HexIdentity(k.pixel_shader_hash) << ','
          << HexIdentity(k.shader_family) << ','
          << attribution::NativeDepthBlendClassName(k.depth_blend_class) << ','
          << HexIdentity(rt.handle) << ',' << rt.generation << ',' << rt.format << ','
          << rt.sample_count << ',' << HexIdentity(depth.handle) << ',' << depth.generation << ','
          << depth.format << ',' << depth.sample_count << ',';
      if(k.origin.attributed())gpu<<k.origin.retail_phase;
      gpu<<','<<RetailGpuPassName(k.origin.retail_phase)<<','<<GpuPassOriginSourceName(k.origin.source)
         <<','<<k.origin.scope<<','<<HexIdentity(k.origin.arena)<<','<<k.origin.offset
         <<','<<k.origin.ordinal<<','<<k.origin.list_id<<'\n';
    }
    for (size_t i = 0; i < f.slices.size(); ++i) {
      const auto& s = f.slices[i];
      slices << f.frame << ',' << f.sequence << ',' << i << ','
             << performance::GpuRangeName(s.range) << ','
             << (!s.available      ? "unavailable"
                 : s.elapsed_ticks ? "approximate"
                                   : "collapsed-or-zero")
             << ',' << s.begin_query << ',' << s.end_query << ',' << s.region << ',' << s.raw_begin
             << ',' << s.raw_end << ',';
      if (s.available)
        slices << s.relative_begin * gpu_ms;
      slices << ',';
      if (s.available)
        slices << s.elapsed_ticks * gpu_ms;
      slices << '\n';
    }
    const auto& t = f.transport;
    const auto& c = f.cpu;
    const auto& p = f.paint;
    transport << f.frame << ',' << f.sequence << ',' << f.slot << ',' << f.submission << ','
              << f.width << ',' << f.height << ',' << f.display_width << ',' << f.display_height
              << ',' << f.first_command << ',' << f.last_command << ',' << c.begin << ',' << c.end
              << ',' << t.commands << ',' << t.measured_commands << ',' << t.capture_ticks * ms
              << ',' << t.capture_lock_ticks * ms << ',' << t.queue_lock_ticks * ms << ','
              << t.backpressure_ticks * ms << ',' << t.dwell_ticks * ms << ','
              << t.max_dwell_ticks * ms << ',' << t.queue_peak << ','
              << t.worker_assembly_ticks * ms << ',' << t.worker_idle_ticks * ms << ','
              << t.internal_flush_ticks * ms << ',' << t.internal_flushes << ',' << c.clock_reads
              << ',' << c.clock_probe_ticks << ','
              << (static_cast<long double>(c.clock_reads) / 2) * c.clock_probe_ticks * ms << ','
              << c.omitted_events << ',' << c.invalid_scopes << ',' << c.stack_overflows << ','
              << c.shader_overflows << ',' << f.detailed_gpu << ',' << f.query_count << ','
              << f.query_budget << ',' << f.dropped_boundaries << ',' << f.dropped_regions << ','
              << f.query_result << ',' << f.modern_shaders << ',' << f.disable_tlad_grain << ','
              << f.readback_ticks * ms << ',' << f.export_snapshot_ticks * ms << ',' << p.sequence
              << ',' << p.guest_frame << ',' << p.submission << ',' << p.begin << ',' << p.end
              << ',' << p.mailbox_version << ',' << p.overwritten << ',' << p.result << ','
              << p.acquire_ticks * ms << ',' << p.submit_ticks * ms << ',' << p.present_ticks * ms
              << ',' << p.total_ticks * ms << '\n';
  }
  trace << "]}\n";
  metadata
      << "{\"schema_version\":2,\"capture_id\":" << meta.capture_id
      << ",\"host_origin_ticks\":" << origin << ",\"complete\":true,\"samples\":" << frames.size()
      << ",\"host_frequency\":" << meta.host_frequency
      << ",\"gpu_timestamp_period_ns\":" << meta.gpu_timestamp_period_ns
      << ",\"gpu_timestamp_valid_bits\":" << meta.gpu_timestamp_valid_bits
      << ",\"driver_id\":" << meta.driver_id << ",\"device\":" << JsonString(meta.device_name)
      << ",\"cpu_clock\":\"host monotonic ticks; elapsed thread spans, not on-core CPU sampling\""
      << ",\"gpu_timing\":\"approximate queue stage/encoder boundaries; not isolated shader "
         "execution; repeated timestamps may collapse ranges\""
      << ",\"cpu_gpu_clock_calibrated\":false,\"physical_scanout_measured\":false"
      << ",\"cpu_accounting\":\"self_ms is exclusive within the instrumented render-owner call "
         "tree; inclusive_ms contains child calls\""
      << ",\"gpu_accounting\":\"flat slices partition the queue envelope; pass regions are a "
         "separate drill-down, never an additional cost\""
      << ",\"transport_accounting\":\"producer and queue dwell sums overlap commands and GPU work; "
         "paint observations keep their own frame identity\""
      << ",\"instrumentation_cost\":\"clock_pair_floor_estimate_ms is a measured clock-only lower "
         "estimate, not total profiler overhead; compare the same scene with detailed_gpu "
         "false/true\""
      << ",\"event_selection\":\"longest 128 instrumented scopes per frame; aggregates include "
         "omitted events\""
      << ",\"first_sample_policy\":\"retain raw first sample; analyzer excludes it by default "
         "because manual attach and partial producer capture can contaminate it\""
      << ",\"modern_shaders\":" << (meta.modern_shaders ? "true" : "false")
      << ",\"disable_tlad_grain\":" << (meta.disable_tlad_grain ? "true" : "false")
      << ",\"settings\":{";
  bool first = true;
  for (const auto& [key, value] : meta.settings) {
    if (!first)
      metadata << ',';
    first = false;
    metadata << JsonString(key) << ':' << JsonString(value);
  }
  metadata << "},\"counter_semantics\":{";
  for (size_t i = 0; i < performance::kCounterCount; ++i) {
    if (i)
      metadata << ',';
    const auto counter = performance::Counter(i);
    metadata << JsonString(performance::CounterName(counter)) << ':'
             << JsonString(CounterSemantics(counter));
  }
  metadata << "},\"shader_names\":{";
  first = true;
  for (const auto& [hash, name] : meta.shader_names) {
    if (!first)
      metadata << ',';
    first = false;
    metadata << JsonString(HexIdentity(hash)) << ':' << JsonString(name);
  }
  metadata << "}}\n";
  for (auto* out : {&cpu, &shaders, &gpu, &slices, &transport, &trace, &metadata}) {
    out->flush();
    if (!*out)
      return false;
    out->close();
    if (!*out)
      return false;
  }
  for (const char* name : {"native-performance-cpu.csv", "native-performance-cpu-shaders.csv",
                           "native-performance-gpu-passes.csv", "native-performance-gpu-slices.csv",
                           "native-performance-frames.csv", "native-performance-trace.json",
                           "native-performance-meta.json"}) {
    std::error_code error;
    std::filesystem::rename(dir / (std::string(name) + ".partial"), dir / name, error);
    if (error)
      return false;
  }
  return true;
}
}  // namespace rex::graphics::gta4_native::profile
