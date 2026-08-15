#include <rex/diagnostics/gta4_transition.h>

#include <algorithm>
#include <array>
#include <atomic>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <string>
#include <thread>

#include <rex/chrono/clock.h>
#include <rex/cvar.h>
#include <rex/logging.h>
#include <rex/thread.h>

REXCVAR_DEFINE_STRING(gta4_transition_diagnostics, "off", "GTA IV/Diagnostics",
                      "Loading-to-gameplay trace: off, metadata, or pcm")
    .allowed({"off", "metadata", "pcm"})
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart)
    .debug_only();

namespace rex::diagnostics::gta4_transition {
namespace {

constexpr uint32_t kEventCapacity = 65536;
constexpr uint32_t kPeriodicLoadingTickInterval = 60;
constexpr uint32_t kSteadyEventSamplingInterval = 32;
constexpr uint32_t kFalseCandidatePresentLimit = 120;
constexpr uint32_t kPresentsAfterFirstDraw = 3;
constexpr uint32_t kTraceVersion = 1;
constexpr uint32_t kPcmSampleRate = 48000;
constexpr uint32_t kPcmMaximumChannels = 6;
constexpr uint32_t kPcmSecondsBefore = 1;
constexpr uint32_t kPcmSecondsAfter = 2;
constexpr uint32_t kPcmMaximumFrames = 144000;
constexpr uint32_t kPcmMaximumSamples = 864000;

enum class CapturePhase : uint8_t {
  kIdle,
  kLoading,
  kWorldActivation,
  kTeardown,
  kFirstDraw,
  kFinalizing,
  kShutdown,
};

enum class CompletionReason : uint32_t {
  kCompleted,
  kNoIndexedDraw,
  kLoadingEnded,
  kShutdown,
};

struct TraceHeader {
  std::array<char, 8> magic;
  uint32_t version;
  uint32_t header_size;
  uint32_t event_size;
  uint32_t event_count;
  uint32_t dropped_events;
  CompletionReason completion_reason;
  uint64_t host_tick_frequency;
  uint64_t transition_id;
  uint64_t first_host_tick;
  uint64_t last_host_tick;
};

static_assert(sizeof(TraceHeader) == 64);

struct EventSlot {
  TransitionEvent event{};
  std::atomic<bool> ready{false};
};

#pragma pack(push, 1)
struct WaveHeader {
  std::array<char, 4> riff;
  uint32_t riff_size;
  std::array<char, 4> wave;
  std::array<char, 4> fmt;
  uint32_t fmt_size;
  uint16_t format;
  uint16_t channels;
  uint32_t sample_rate;
  uint32_t bytes_per_second;
  uint16_t bytes_per_frame;
  uint16_t bits_per_sample;
  std::array<char, 4> data;
  uint32_t data_size;
};
#pragma pack(pop)

static_assert(sizeof(WaveHeader) == 44);

class Coordinator {
 public:
  Coordinator() : writer_([this] { WriterMain(); }) {}

  ~Coordinator() { Shutdown(); }

  uint64_t active_transition_id() const {
    const CapturePhase phase = phase_.load(std::memory_order_acquire);
    if (phase == CapturePhase::kIdle || phase == CapturePhase::kFinalizing ||
        phase == CapturePhase::kShutdown) {
      return 0;
    }
    return transition_id_.load(std::memory_order_acquire);
  }

  bool dense_capture() const {
    const CapturePhase phase = phase_.load(std::memory_order_acquire);
    return phase == CapturePhase::kTeardown ||
           phase == CapturePhase::kFirstDraw ||
           phase == CapturePhase::kFinalizing;
  }

  bool RecordEvent(EventSource source, EventType type, uint32_t guest_pc,
                   uint32_t guest_lr, uint32_t submitted_frame, uint8_t flags,
                   uint64_t value0, uint64_t value1, uint64_t value2,
                   bool allow_finalizing = false) {
    CapturePhase phase = phase_.load(std::memory_order_acquire);
    if (phase == CapturePhase::kIdle || phase == CapturePhase::kShutdown ||
        (phase == CapturePhase::kFinalizing && !allow_finalizing)) {
      return false;
    }

    active_writers_.fetch_add(1, std::memory_order_acq_rel);
    phase = phase_.load(std::memory_order_acquire);
    if (phase == CapturePhase::kShutdown ||
        (phase == CapturePhase::kFinalizing && !allow_finalizing)) {
      active_writers_.fetch_sub(1, std::memory_order_release);
      return false;
    }

    const uint32_t index = write_index_.fetch_add(1, std::memory_order_relaxed);
    if (index >= kEventCapacity) {
      dropped_events_.fetch_add(1, std::memory_order_relaxed);
      active_writers_.fetch_sub(1, std::memory_order_release);
      return false;
    }

    TransitionEvent event{};
    event.host_tick = chrono::Clock::QueryHostTickCount();
    // The reservation ticket is also the sequence so file order is monotonic
    // even when multiple producers are preempted between reservation/write.
    event.sequence = index;
    event.transition_id = transition_id_.load(std::memory_order_relaxed);
    event.value0 = value0;
    event.value1 = value1;
    event.value2 = value2;
    event.guest_pc = guest_pc;
    event.guest_lr = guest_lr;
    event.submitted_frame = submitted_frame;
    event.type = type;
    event.source = source;
    event.flags = flags;
    slots_[index].event = event;
    slots_[index].ready.store(true, std::memory_order_release);
    active_writers_.fetch_sub(1, std::memory_order_release);
    return true;
  }

  void NoteLoadingTick(uint32_t guest_pc, uint32_t guest_lr,
                       bool loading_active, uint32_t loading_state,
                       uint64_t state_bits) {
    if (!loading_active) {
      const CapturePhase phase = phase_.load(std::memory_order_acquire);
      if (phase == CapturePhase::kLoading) {
        const uint32_t inactive_ticks =
            inactive_loading_ticks_.fetch_add(1, std::memory_order_relaxed) + 1;
        if (inactive_ticks >= kFalseCandidatePresentLimit) {
          RecordEvent(EventSource::kCoordinator, EventType::kLoadingEnd,
                      guest_pc, guest_lr, 0, kFlagAfter, loading_state,
                      state_bits, inactive_ticks);
          RequestFinalize(CompletionReason::kLoadingEnded,
                          EventType::kTraceCancelled);
        }
      }
      return;
    }

    inactive_loading_ticks_.store(0, std::memory_order_relaxed);
    BeginCaptureIfIdle();
    if (phase_.load(std::memory_order_acquire) == CapturePhase::kIdle) {
      return;
    }

    const uint32_t tick = loading_tick_count_.fetch_add(1, std::memory_order_relaxed);
    const uint32_t prior = last_loading_state_.exchange(loading_state,
                                                        std::memory_order_relaxed);
    const bool changed = prior != loading_state;
    const bool periodic = (tick % kPeriodicLoadingTickInterval) == 0;
    if (changed || periodic) {
      uint8_t flags = changed ? kFlagStateChanged : kFlagNone;
      if (periodic) {
        flags = static_cast<uint8_t>(flags | kFlagPeriodic);
      }
      RecordEvent(EventSource::kGuest,
                  changed ? EventType::kLoadingStateChange
                          : EventType::kLoadingTick,
                  guest_pc, guest_lr, 0, flags, loading_state, state_bits,
                  tick);
    }
  }

  void BeginWorldActivation(uint32_t guest_pc, uint32_t guest_lr,
                            uint64_t argument_bits) {
    BeginCaptureIfIdle();
    CapturePhase expected = CapturePhase::kLoading;
    phase_.compare_exchange_strong(expected, CapturePhase::kWorldActivation,
                                   std::memory_order_acq_rel);
    RecordEvent(EventSource::kGuest, EventType::kWorldActivationBegin,
                guest_pc, guest_lr, 0, kFlagBefore, argument_bits, 0, 0);
  }

  void NoteTeardown(bool entering, uint32_t guest_pc, uint32_t guest_lr,
                    uint64_t argument_bits) {
    if (entering) {
      CapturePhase expected = CapturePhase::kWorldActivation;
      phase_.compare_exchange_strong(expected, CapturePhase::kTeardown,
                                     std::memory_order_acq_rel);
      if (GetMode() == Mode::kPcm) {
        pcm_boundary_frame_.store(pcm_total_frames_.load(std::memory_order_acquire),
                                  std::memory_order_release);
        pcm_boundary_seen_.store(true, std::memory_order_release);
      }
    }
    RecordEvent(EventSource::kGuest,
                entering ? EventType::kLoadingTeardownBegin
                         : EventType::kLoadingTeardownEnd,
                guest_pc, guest_lr, 0, entering ? kFlagBefore : kFlagAfter,
                argument_bits, 0, 0);
  }

  void EndWorldActivation(uint32_t guest_pc, uint32_t guest_lr,
                          uint64_t result_bits) {
    RecordEvent(EventSource::kGuest, EventType::kWorldActivationEnd,
                guest_pc, guest_lr, 0, kFlagAfter, result_bits, 0, 0);
  }

  void FirstIndexedDraw(uint32_t guest_pc, uint32_t guest_lr,
                        uint32_t submitted_frame, uint64_t draw_id) {
    const CapturePhase phase = phase_.load(std::memory_order_acquire);
    if (phase == CapturePhase::kIdle || phase == CapturePhase::kFinalizing ||
        phase == CapturePhase::kShutdown) {
      return;
    }
    bool expected = false;
    const bool first = saw_first_draw_.compare_exchange_strong(
        expected, true, std::memory_order_acq_rel);
    RecordEvent(EventSource::kRenderer, EventType::kFirstIndexedDraw,
                guest_pc, guest_lr, submitted_frame,
                first ? kFlagStateChanged : kFlagNone, draw_id, 0, 0);
    if (first) {
      phase_.store(CapturePhase::kFirstDraw, std::memory_order_release);
      presents_after_first_draw_.store(0, std::memory_order_relaxed);
    }
  }

  void Present(uint32_t submitted_frame, uint64_t present_id) {
    const CapturePhase phase = phase_.load(std::memory_order_acquire);
    if (phase == CapturePhase::kIdle || phase == CapturePhase::kFinalizing ||
        phase == CapturePhase::kShutdown) {
      return;
    }
    RecordEvent(EventSource::kPresenter, EventType::kPresent, 0, 0,
                submitted_frame, kFlagNone, present_id, 0, 0);

    if (saw_first_draw_.load(std::memory_order_acquire)) {
      const uint32_t count =
          presents_after_first_draw_.fetch_add(1, std::memory_order_relaxed) + 1;
      if (count >= kPresentsAfterFirstDraw) {
        visual_capture_complete_.store(true, std::memory_order_release);
        if (GetMode() != Mode::kPcm ||
            pcm_capture_complete_.load(std::memory_order_acquire)) {
          RequestFinalize(CompletionReason::kCompleted,
                          EventType::kTraceComplete);
        }
      }
      return;
    }

    if (phase == CapturePhase::kTeardown ||
        phase == CapturePhase::kWorldActivation) {
      const uint32_t count =
          presents_without_draw_.fetch_add(1, std::memory_order_relaxed) + 1;
      if (count >= kFalseCandidatePresentLimit) {
        RequestFinalize(CompletionReason::kNoIndexedDraw,
                        EventType::kTraceCancelled);
      }
    }
  }

  void CapturePcm(const float* samples, uint32_t frame_count,
                  uint32_t channel_count, uint32_t sample_rate) {
    if (!samples || !frame_count || !channel_count ||
        channel_count > kPcmMaximumChannels || sample_rate != kPcmSampleRate ||
        GetMode() != Mode::kPcm ||
        pcm_capture_complete_.load(std::memory_order_acquire)) {
      return;
    }
    const CapturePhase phase = phase_.load(std::memory_order_acquire);
    if (phase == CapturePhase::kIdle || phase == CapturePhase::kFinalizing ||
        phase == CapturePhase::kShutdown) {
      return;
    }

    uint32_t expected_channels = pcm_channels_.load(std::memory_order_relaxed);
    if (!expected_channels) {
      pcm_channels_.compare_exchange_strong(expected_channels, channel_count,
                                            std::memory_order_relaxed);
      expected_channels = pcm_channels_.load(std::memory_order_relaxed);
    }
    if (expected_channels != channel_count ||
        pcm_writer_.test_and_set(std::memory_order_acquire)) {
      pcm_dropped_frames_.fetch_add(frame_count, std::memory_order_relaxed);
      return;
    }

    uint64_t total_frames = pcm_total_frames_.load(std::memory_order_relaxed);
    uint32_t capture_frames = frame_count;
    if (pcm_boundary_seen_.load(std::memory_order_acquire)) {
      const uint64_t boundary =
          pcm_boundary_frame_.load(std::memory_order_acquire);
      const uint64_t post_frames = uint64_t{kPcmSampleRate} * kPcmSecondsAfter;
      const uint64_t end_frame = boundary + post_frames;
      if (total_frames >= end_frame) {
        capture_frames = 0;
      } else {
        capture_frames = static_cast<uint32_t>(
            std::min<uint64_t>(capture_frames, end_frame - total_frames));
      }
    }
    for (uint32_t frame = 0; frame < capture_frames; ++frame) {
      const uint64_t destination_frame =
          (total_frames + frame) % kPcmMaximumFrames;
      const uint64_t destination_sample = destination_frame * channel_count;
      const uint64_t source_sample = uint64_t{frame} * channel_count;
      for (uint32_t channel = 0; channel < channel_count; ++channel) {
        pcm_samples_[destination_sample + channel] =
            samples[source_sample + channel];
      }
    }
    total_frames += capture_frames;
    pcm_total_frames_.store(total_frames, std::memory_order_release);
    pcm_writer_.clear(std::memory_order_release);

    if (pcm_boundary_seen_.load(std::memory_order_acquire)) {
      const uint64_t boundary =
          pcm_boundary_frame_.load(std::memory_order_acquire);
      const uint64_t post_frames = uint64_t{kPcmSampleRate} * kPcmSecondsAfter;
      if (total_frames >= boundary + post_frames) {
        pcm_capture_complete_.store(true, std::memory_order_release);
      }
    }
  }

  void Shutdown() {
    CapturePhase prior = phase_.exchange(CapturePhase::kShutdown,
                                         std::memory_order_acq_rel);
    if (prior == CapturePhase::kShutdown) {
      return;
    }
    if (prior != CapturePhase::kIdle) {
      completion_reason_.store(CompletionReason::kShutdown,
                               std::memory_order_release);
      flush_pending_.store(true, std::memory_order_release);
    }
    {
      std::lock_guard<std::mutex> lock(writer_mutex_);
      writer_shutdown_.store(true, std::memory_order_release);
    }
    writer_cv_.notify_one();
    if (writer_.joinable() && writer_.get_id() != std::this_thread::get_id()) {
      writer_.join();
    }
  }

 private:
  void BeginCaptureIfIdle() {
    CapturePhase expected = CapturePhase::kIdle;
    if (!phase_.compare_exchange_strong(expected, CapturePhase::kLoading,
                                        std::memory_order_acq_rel)) {
      return;
    }
    transition_id_.fetch_add(1, std::memory_order_relaxed);
    dropped_events_.store(0, std::memory_order_relaxed);
    loading_tick_count_.store(0, std::memory_order_relaxed);
    inactive_loading_ticks_.store(0, std::memory_order_relaxed);
    last_loading_state_.store(UINT32_MAX, std::memory_order_relaxed);
    presents_without_draw_.store(0, std::memory_order_relaxed);
    presents_after_first_draw_.store(0, std::memory_order_relaxed);
    saw_first_draw_.store(false, std::memory_order_relaxed);
    visual_capture_complete_.store(false, std::memory_order_relaxed);
    pcm_boundary_seen_.store(false, std::memory_order_relaxed);
    pcm_capture_complete_.store(false, std::memory_order_relaxed);
    pcm_boundary_frame_.store(0, std::memory_order_relaxed);
    pcm_total_frames_.store(0, std::memory_order_relaxed);
    pcm_channels_.store(0, std::memory_order_relaxed);
    pcm_dropped_frames_.store(0, std::memory_order_relaxed);
    RecordEvent(EventSource::kCoordinator, EventType::kLoadingBegin, 0, 0,
                0, kFlagBefore, 0, 0, 0);
  }

  void RequestFinalize(CompletionReason reason, EventType final_event) {
    CapturePhase phase = phase_.load(std::memory_order_acquire);
    while (phase != CapturePhase::kIdle &&
           phase != CapturePhase::kFinalizing &&
           phase != CapturePhase::kShutdown) {
      if (phase_.compare_exchange_weak(phase, CapturePhase::kFinalizing,
                                      std::memory_order_acq_rel)) {
        RecordEvent(EventSource::kCoordinator, final_event, 0, 0, 0,
                    reason == CompletionReason::kCompleted ? kFlagAfter
                                                           : kFlagError,
                    static_cast<uint64_t>(reason),
                    dropped_events_.load(std::memory_order_relaxed), 0,
                    true);
        completion_reason_.store(reason, std::memory_order_release);
        flush_pending_.store(true, std::memory_order_release);
        writer_cv_.notify_one();
        return;
      }
    }
  }

  void WriterMain() {
    rex::thread::set_current_thread_name("GTA4 Transition Trace");
    for (;;) {
      {
        std::unique_lock<std::mutex> lock(writer_mutex_);
        writer_cv_.wait(lock, [this] {
          return flush_pending_.load(std::memory_order_acquire) ||
                 writer_shutdown_.load(std::memory_order_acquire);
        });
      }

      if (flush_pending_.exchange(false, std::memory_order_acq_rel)) {
        while (active_writers_.load(std::memory_order_acquire) != 0) {
          std::this_thread::yield();
        }
        FlushTrace();
      }

      if (writer_shutdown_.load(std::memory_order_acquire)) {
        return;
      }
    }
  }

  void FlushTrace() {
    const uint32_t reserved = write_index_.load(std::memory_order_acquire);
    const uint32_t event_count = std::min(reserved, kEventCapacity);
    for (uint32_t index = 0; index < event_count; ++index) {
      while (!slots_[index].ready.load(std::memory_order_acquire)) {
        std::this_thread::yield();
      }
    }

    const uint64_t first_tick =
        event_count ? slots_[0].event.host_tick : 0;
    const uint64_t last_tick =
        event_count ? slots_[event_count - 1].event.host_tick : 0;
    TraceHeader header{{'L', 'T', 'R', 'C', 'G', 'T', 'A', '4'},
                       kTraceVersion,
                       sizeof(TraceHeader),
                       sizeof(TransitionEvent),
                       event_count,
                       dropped_events_.load(std::memory_order_relaxed),
                       completion_reason_.load(std::memory_order_acquire),
                       chrono::Clock::QueryHostTickFrequency(),
                       transition_id_.load(std::memory_order_relaxed),
                       first_tick,
                       last_tick};

    std::error_code ec;
    const std::filesystem::path directory =
        std::filesystem::temp_directory_path(ec) /
        "libertyrecomp-transition-traces";
    if (!ec) {
      std::filesystem::create_directories(directory, ec);
    }

    bool wrote_binary = false;
    std::filesystem::path binary_path;
    std::filesystem::path json_path;
    if (!ec) {
      const std::string stem =
          "transition_" + std::to_string(header.transition_id) + "_" +
          std::to_string(header.last_host_tick);
      binary_path = directory / (stem + ".ltrc");
      json_path = directory / (stem + ".json");
      std::ofstream binary(binary_path, std::ios::binary | std::ios::trunc);
      if (binary) {
        binary.write(reinterpret_cast<const char*>(&header), sizeof(header));
        for (uint32_t index = 0; index < event_count; ++index) {
          binary.write(reinterpret_cast<const char*>(&slots_[index].event),
                       sizeof(TransitionEvent));
        }
        wrote_binary = binary.good();
      }

      std::array<uint32_t, 7> source_counts{};
      std::array<uint32_t, static_cast<size_t>(EventType::kCount)>
          event_counts{};
      std::array<uint32_t, 8> flag_counts{};
      uint64_t whole_bank_nonfinite_vertex_constants = 0;
      uint32_t invalid_vertex_ranges = 0;
      uint32_t command_arena_wraps = 0;
      for (uint32_t index = 0; index < event_count; ++index) {
        const TransitionEvent& event = slots_[index].event;
        const size_t source = static_cast<size_t>(event.source);
        if (source < source_counts.size()) {
          ++source_counts[source];
        }
        const size_t type = static_cast<size_t>(event.type);
        if (type < event_counts.size()) {
          ++event_counts[type];
        }
        for (size_t flag = 0; flag < flag_counts.size(); ++flag) {
          if (event.flags & (uint8_t{1} << flag)) {
            ++flag_counts[flag];
          }
        }
        if (event.type == EventType::kVertexConstants) {
          whole_bank_nonfinite_vertex_constants += uint32_t(event.value1);
        }
        if (event.type == EventType::kVertexRange &&
            (event.flags & kFlagError)) {
          ++invalid_vertex_ranges;
        }
        if (event.type == EventType::kCommandArenaAllocateEnd &&
            (event.flags & kFlagStateChanged)) {
          ++command_arena_wraps;
        }
      }
      std::ofstream json(json_path, std::ios::trunc);
      if (json) {
        json << "{\n"
             << "  \"schema\": \"libertyrecomp.gta4-transition/1\",\n"
             << "  \"transition_id\": " << header.transition_id << ",\n"
             << "  \"complete\": "
             << (header.completion_reason == CompletionReason::kCompleted
                     ? "true"
                     : "false")
             << ",\n"
             << "  \"completion_reason\": "
             << static_cast<uint32_t>(header.completion_reason) << ",\n"
             << "  \"event_count\": " << header.event_count << ",\n"
             << "  \"dropped_events\": " << header.dropped_events << ",\n"
             << "  \"pcm_dropped_frames\": "
             << pcm_dropped_frames_.load(std::memory_order_relaxed) << ",\n"
             << "  \"host_tick_frequency\": " << header.host_tick_frequency
             << ",\n"
             << "  \"first_host_tick\": " << header.first_host_tick << ",\n"
             << "  \"last_host_tick\": " << header.last_host_tick << ",\n"
             << "  \"source_counts\": [";
        for (size_t index = 0; index < source_counts.size(); ++index) {
          if (index) {
            json << ", ";
          }
          json << source_counts[index];
        }
        json << "],\n"
             << "  \"event_type_counts\": [";
        for (size_t index = 0; index < event_counts.size(); ++index) {
          if (index) {
            json << ", ";
          }
          json << event_counts[index];
        }
        json << "],\n"
             << "  \"flag_bit_counts\": [";
        for (size_t index = 0; index < flag_counts.size(); ++index) {
          if (index) {
            json << ", ";
          }
          json << flag_counts[index];
        }
        json << "],\n"
             << "  \"findings\": {\n"
             << "    \"audio_underruns\": "
             << event_counts[static_cast<size_t>(EventType::kAudioUnderrun)]
             << ",\n"
             << "    \"xma_kicks\": "
             << event_counts[static_cast<size_t>(EventType::kXmaKick)]
             << ",\n"
             << "    \"pipeline_misses\": "
             << event_counts[static_cast<size_t>(EventType::kPipelineMissBegin)]
             << ",\n"
             << "    \"invalid_vertex_ranges\": "
             << invalid_vertex_ranges << ",\n"
             << "    \"command_arena_wraps\": "
             << command_arena_wraps << ",\n"
             << "    \"whole_bank_nonfinite_vertex_constants_unclassified\": "
             << whole_bank_nonfinite_vertex_constants << "\n"
             << "  }\n}\n";
      }
      if (GetMode() == Mode::kPcm) {
        FlushPcm(directory / (stem + ".wav"));
      }
    }

    if (wrote_binary) {
      REXLOG_INFO("GTA IV transition trace {} wrote {} events ({} dropped) to {}",
                  header.transition_id, header.event_count,
                  header.dropped_events, binary_path.string());
    } else {
      REXLOG_WARN("GTA IV transition trace {} could not be written",
                  header.transition_id);
    }

    for (uint32_t index = 0; index < event_count; ++index) {
      slots_[index].ready.store(false, std::memory_order_relaxed);
    }
    write_index_.store(0, std::memory_order_relaxed);
    if (phase_.load(std::memory_order_acquire) != CapturePhase::kShutdown) {
      phase_.store(CapturePhase::kIdle, std::memory_order_release);
    }
  }

  void FlushPcm(const std::filesystem::path& path) {
    while (pcm_writer_.test_and_set(std::memory_order_acquire)) {
      std::this_thread::yield();
    }
    const uint32_t channels = pcm_channels_.load(std::memory_order_relaxed);
    const uint64_t total_frames =
        pcm_total_frames_.load(std::memory_order_acquire);
    const uint64_t boundary =
        pcm_boundary_frame_.load(std::memory_order_acquire);
    if (!channels || !pcm_boundary_seen_.load(std::memory_order_acquire) ||
        !total_frames) {
      pcm_writer_.clear(std::memory_order_release);
      return;
    }

    const uint64_t pre_frames = uint64_t{kPcmSampleRate} * kPcmSecondsBefore;
    const uint64_t post_frames = uint64_t{kPcmSampleRate} * kPcmSecondsAfter;
    const uint64_t start_frame = boundary > pre_frames ? boundary - pre_frames : 0;
    const uint64_t end_frame = std::min(total_frames, boundary + post_frames);
    const uint64_t frame_count = end_frame > start_frame ? end_frame - start_frame : 0;
    const uint64_t sample_count = frame_count * channels;
    const uint64_t data_size_64 = sample_count * sizeof(float);
    if (!frame_count || data_size_64 > UINT32_MAX) {
      pcm_writer_.clear(std::memory_order_release);
      return;
    }

    const uint32_t data_size = static_cast<uint32_t>(data_size_64);
    const uint16_t bytes_per_frame =
        static_cast<uint16_t>(channels * sizeof(float));
    WaveHeader wave{{'R', 'I', 'F', 'F'},
                    uint32_t{36} + data_size,
                    {'W', 'A', 'V', 'E'},
                    {'f', 'm', 't', ' '},
                    16,
                    3,
                    static_cast<uint16_t>(channels),
                    kPcmSampleRate,
                    kPcmSampleRate * bytes_per_frame,
                    bytes_per_frame,
                    32,
                    {'d', 'a', 't', 'a'},
                    data_size};
    std::ofstream wave_file(path, std::ios::binary | std::ios::trunc);
    if (wave_file) {
      wave_file.write(reinterpret_cast<const char*>(&wave), sizeof(wave));
      for (uint64_t frame = start_frame; frame < end_frame; ++frame) {
        const uint64_t source_frame = frame % kPcmMaximumFrames;
        const float* source = &pcm_samples_[source_frame * channels];
        wave_file.write(reinterpret_cast<const char*>(source),
                        bytes_per_frame);
      }
    }
    pcm_writer_.clear(std::memory_order_release);
  }

  std::array<EventSlot, kEventCapacity> slots_{};
  std::atomic<CapturePhase> phase_{CapturePhase::kIdle};
  std::atomic<uint64_t> transition_id_{0};
  std::atomic<uint32_t> write_index_{0};
  std::atomic<uint32_t> dropped_events_{0};
  std::atomic<uint32_t> active_writers_{0};
  std::atomic<uint32_t> loading_tick_count_{0};
  std::atomic<uint32_t> inactive_loading_ticks_{0};
  std::atomic<uint32_t> last_loading_state_{UINT32_MAX};
  std::atomic<uint32_t> presents_without_draw_{0};
  std::atomic<uint32_t> presents_after_first_draw_{0};
  std::atomic<bool> saw_first_draw_{false};
  std::atomic<bool> visual_capture_complete_{false};
  std::array<float, kPcmMaximumSamples> pcm_samples_{};
  std::atomic_flag pcm_writer_ = ATOMIC_FLAG_INIT;
  std::atomic<uint64_t> pcm_total_frames_{0};
  std::atomic<uint64_t> pcm_boundary_frame_{0};
  std::atomic<uint64_t> pcm_dropped_frames_{0};
  std::atomic<uint32_t> pcm_channels_{0};
  std::atomic<bool> pcm_boundary_seen_{false};
  std::atomic<bool> pcm_capture_complete_{false};
  std::atomic<CompletionReason> completion_reason_{
      CompletionReason::kCompleted};
  std::atomic<bool> flush_pending_{false};
  std::mutex writer_mutex_;
  std::condition_variable writer_cv_;
  std::atomic<bool> writer_shutdown_{false};
  std::thread writer_;
};

Coordinator& GetCoordinator() {
  static Coordinator coordinator;
  return coordinator;
}

std::atomic<bool> g_initialized{false};

}  // namespace

Mode GetMode() {
  const std::string& value = REXCVAR_GET(gta4_transition_diagnostics);
  if (value == "metadata") {
    return Mode::kMetadata;
  }
  if (value == "pcm") {
    return Mode::kPcm;
  }
  return Mode::kOff;
}

bool IsEnabled() { return GetMode() != Mode::kOff; }

void Initialize() {
  if (!IsEnabled()) {
    return;
  }
  (void)GetCoordinator();
  g_initialized.store(true, std::memory_order_release);
}

uint64_t ActiveTransitionId() {
  return IsEnabled() && g_initialized.load(std::memory_order_acquire)
             ? GetCoordinator().active_transition_id()
             : 0;
}

bool Record(EventSource source, EventType type, uint32_t guest_pc,
            uint32_t guest_lr, uint32_t submitted_frame, uint8_t flags,
            uint64_t value0, uint64_t value1, uint64_t value2) {
  return IsEnabled() && g_initialized.load(std::memory_order_acquire) &&
         GetCoordinator().RecordEvent(source, type, guest_pc, guest_lr,
                                      submitted_frame, flags, value0, value1,
                                      value2);
}

bool RecordSteady(EventSource source, EventType type, uint32_t guest_pc,
                  uint32_t guest_lr, uint32_t submitted_frame, uint8_t flags,
                  uint64_t value0, uint64_t value1, uint64_t value2) {
  if (!IsEnabled() || !g_initialized.load(std::memory_order_acquire)) {
    return false;
  }
  static std::atomic<uint64_t> steady_event_index{0};
  const uint64_t index =
      steady_event_index.fetch_add(1, std::memory_order_relaxed);
  if (!GetCoordinator().dense_capture() &&
      index % kSteadyEventSamplingInterval != 0) {
    return false;
  }
  return GetCoordinator().RecordEvent(source, type, guest_pc, guest_lr,
                                      submitted_frame, flags, value0, value1,
                                      value2);
}

void CapturePcmFloatInterleaved(const float* samples, uint32_t frame_count,
                                uint32_t channel_count,
                                uint32_t sample_rate) {
  if (GetMode() == Mode::kPcm &&
      g_initialized.load(std::memory_order_acquire)) {
    GetCoordinator().CapturePcm(samples, frame_count, channel_count,
                                sample_rate);
  }
}

void NoteLoadingTick(uint32_t guest_pc, uint32_t guest_lr,
                     bool loading_active, uint32_t loading_state,
                     uint64_t state_bits) {
  if (IsEnabled() && g_initialized.load(std::memory_order_acquire)) {
    GetCoordinator().NoteLoadingTick(guest_pc, guest_lr, loading_active,
                                     loading_state, state_bits);
  }
}

void NoteStateDispatch(bool entering, uint32_t guest_pc, uint32_t guest_lr,
                       uint32_t stored_state, uint64_t dispatch_flags) {
  Record(EventSource::kGuest,
         entering ? EventType::kStateDispatchBegin
                  : EventType::kStateDispatchEnd,
         guest_pc, guest_lr, 0, entering ? kFlagBefore : kFlagAfter,
         stored_state, dispatch_flags, 0);
}

void NoteWorldActivationBegin(uint32_t guest_pc, uint32_t guest_lr,
                              uint64_t argument_bits) {
  if (IsEnabled() && g_initialized.load(std::memory_order_acquire)) {
    GetCoordinator().BeginWorldActivation(guest_pc, guest_lr, argument_bits);
  }
}

void NoteLoadingTeardown(bool entering, uint32_t guest_pc, uint32_t guest_lr,
                         uint64_t argument_bits) {
  if (IsEnabled() && g_initialized.load(std::memory_order_acquire)) {
    GetCoordinator().NoteTeardown(entering, guest_pc, guest_lr, argument_bits);
  }
}

void NoteWorldActivationEnd(uint32_t guest_pc, uint32_t guest_lr,
                            uint64_t result_bits) {
  if (IsEnabled() && g_initialized.load(std::memory_order_acquire)) {
    GetCoordinator().EndWorldActivation(guest_pc, guest_lr, result_bits);
  }
}

void NoteFirstIndexedDraw(uint32_t guest_pc, uint32_t guest_lr,
                          uint32_t submitted_frame, uint64_t draw_id) {
  if (IsEnabled() && g_initialized.load(std::memory_order_acquire)) {
    GetCoordinator().FirstIndexedDraw(guest_pc, guest_lr, submitted_frame,
                                      draw_id);
  }
}

void NotePresent(uint32_t submitted_frame, uint64_t present_id) {
  if (IsEnabled() && g_initialized.load(std::memory_order_acquire)) {
    GetCoordinator().Present(submitted_frame, present_id);
  }
}

void Shutdown() {
  if (IsEnabled() && g_initialized.exchange(false, std::memory_order_acq_rel)) {
    GetCoordinator().Shutdown();
  }
}

}  // namespace rex::diagnostics::gta4_transition
