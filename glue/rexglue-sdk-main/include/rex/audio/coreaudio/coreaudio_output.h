/**
 ******************************************************************************
 * ReXGlue : native macOS CoreAudio output                                    *
 ******************************************************************************
 */

#pragma once

#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <memory>
#include <mutex>
#include <thread>

#include <AudioToolbox/AudioToolbox.h>
#include <CoreAudio/CoreAudio.h>

#include <rex/audio/spsc_frame_ring.h>
#include <rex/thread.h>

namespace rex::audio::coreaudio {

constexpr uint32_t kGuestAudioSampleRate = 48000;
constexpr uint32_t kGuestAudioFramesPerBlock = 256;
constexpr uint32_t kGuestAudioChannels = 6;
constexpr uint32_t kMaximumCoreAudioClients = 8;
constexpr uint32_t kMaximumCoreAudioGuestBlocks = 64;
constexpr uint32_t kCoreAudioRingFrames = kGuestAudioFramesPerBlock * kMaximumCoreAudioGuestBlocks;
constexpr uint32_t kCoreAudioSignalDiagnosticQueueEntries = 128;

enum CoreAudioSignalReason : uint32_t {
  kCoreAudioSignalReasonNone = 0,
  kCoreAudioSignalReasonLoudStart = 1 << 0,
  kCoreAudioSignalReasonClipStart = 1 << 1,
  kCoreAudioSignalReasonPeriodic = 1 << 2,
  kCoreAudioSignalReasonEpisodeEnd = 1 << 3,
  kCoreAudioSignalReasonNonfinite = 1 << 4,
  kCoreAudioSignalReasonDroppedInput = 1 << 5,
};

enum class CoreAudioSignalStage : uint32_t {
  kGuestProducer,
  kPreClampMix,
};

// Fixed-size diagnostic payload. The audio producers and CoreAudio callback
// only publish these records to SPSC queues; formatting and file-backed
// logging happen later on the control thread.
struct CoreAudioSignalDiagnostic {
  uint64_t event_sequence = 0;
  uint64_t source_sequence = 0;
  uint64_t episode_id = 0;
  uint64_t episode_blocks = 0;
  uint64_t episode_frames = 0;
  uint64_t episode_loud_samples = 0;
  uint64_t episode_clipped_samples = 0;
  uint64_t episode_nonfinite_samples = 0;
  uint64_t ring_read_before = 0;
  uint64_t ring_read_after = 0;
  uint64_t ring_write_before = 0;
  uint64_t ring_write_after = 0;
  uint64_t ring_available_before = 0;
  uint64_t ring_available_after = 0;
  uint64_t submitted_blocks = 0;
  uint64_t output_frame_start = 0;
  uint64_t callback_ticks = 0;
  uint64_t device_overloads = 0;
  uint64_t underrun_frames = 0;
  uint32_t reason_flags = kCoreAudioSignalReasonNone;
  CoreAudioSignalStage stage = CoreAudioSignalStage::kGuestProducer;
  uint32_t client_index = UINT32_MAX;
  uint32_t client_mask = 0;
  uint32_t guest_frame_ptr = 0;
  uint32_t frame_count = 0;
  uint32_t channels = 0;
  uint32_t mixed_clients = 0;
  uint32_t loud_samples = 0;
  uint32_t clipped_samples = 0;
  uint32_t positive_clipped_samples = 0;
  uint32_t negative_clipped_samples = 0;
  uint32_t nonfinite_samples = 0;
  uint32_t first_loud_sample = UINT32_MAX;
  uint32_t first_clipped_sample = UINT32_MAX;
  uint32_t peak_sample = UINT32_MAX;
  uint32_t longest_loud_run = 0;
  uint32_t output_buffer_bytes = 0;
  uint32_t required_buffer_bytes = 0;
  bool input_accepted = true;
  bool muted = false;
  float loud_threshold = 0.0f;
  float peak = 0.0f;
  float episode_peak = 0.0f;
  std::array<float, kGuestAudioChannels> minimum{};
  std::array<float, kGuestAudioChannels> maximum{};
  std::array<float, kGuestAudioChannels> peak_per_channel{};
  std::array<float, kGuestAudioChannels> rms{};
  std::array<float, kGuestAudioChannels> mean{};
  std::array<float, kGuestAudioChannels> peak_frame{};
  std::array<float, kGuestAudioChannels> first_clipped_frame{};
  std::array<uint64_t, kMaximumCoreAudioClients> client_ring_read_before{};
  std::array<uint64_t, kMaximumCoreAudioClients> client_ring_read_after{};
  std::array<uint64_t, kMaximumCoreAudioClients> client_available_after{};
};

class CoreAudioSignalDiagnosticQueue {
 public:
  bool Push(const CoreAudioSignalDiagnostic& diagnostic) {
    const uint32_t write = write_index_.load(std::memory_order_relaxed);
    uint32_t next = write + 1;
    if (next == kCoreAudioSignalDiagnosticQueueEntries) {
      next = 0;
    }
    if (next == read_index_.load(std::memory_order_acquire)) {
      dropped_.fetch_add(1, std::memory_order_relaxed);
      return false;
    }
    entries_[write] = diagnostic;
    write_index_.store(next, std::memory_order_release);
    return true;
  }

  bool Pop(CoreAudioSignalDiagnostic* diagnostic) {
    if (!diagnostic) {
      return false;
    }
    const uint32_t read = read_index_.load(std::memory_order_relaxed);
    if (read == write_index_.load(std::memory_order_acquire)) {
      return false;
    }
    *diagnostic = entries_[read];
    uint32_t next = read + 1;
    if (next == kCoreAudioSignalDiagnosticQueueEntries) {
      next = 0;
    }
    read_index_.store(next, std::memory_order_release);
    return true;
  }

  uint64_t TakeDroppedCount() {
    return dropped_.exchange(0, std::memory_order_acq_rel);
  }

 private:
  std::array<CoreAudioSignalDiagnostic, kCoreAudioSignalDiagnosticQueueEntries> entries_{};
  alignas(64) std::atomic<uint32_t> write_index_{0};
  alignas(64) std::atomic<uint32_t> read_index_{0};
  std::atomic<uint64_t> dropped_{0};
};

struct CoreAudioSignalEpisodeState {
  bool loud_active = false;
  bool clip_active = false;
  bool nonfinite_active = false;
  uint64_t episode_id = 0;
  uint64_t blocks = 0;
  uint64_t frames = 0;
  uint64_t loud_samples = 0;
  uint64_t clipped_samples = 0;
  uint64_t nonfinite_samples = 0;
  uint32_t blocks_since_log = 0;
  float peak = 0.0f;
};

struct CoreAudioClientState {
  explicit CoreAudioClientState(rex::thread::Semaphore* client_semaphore)
      : semaphore(client_semaphore), ring(kCoreAudioRingFrames, kGuestAudioChannels) {}

  rex::thread::Semaphore* semaphore = nullptr;
  SpscFrameRing ring;
  std::atomic<uint32_t> channels{2};
  std::atomic<bool> accepting{true};
  std::atomic<bool> paused{false};
  std::atomic<uint64_t> submitted_blocks{0};
  std::atomic<uint64_t> retired_blocks_total{0};
  std::atomic<uint64_t> retired_blocks_pending{0};
  std::atomic<uint32_t> credit_depth{1};
  std::atomic<uint32_t> credit_limit{1};
  std::atomic<uint64_t> adaptive_credit_requests{0};
  std::atomic<uint64_t> dropped_blocks{0};
  std::atomic<uint64_t> underrun_frames{0};
  std::atomic<uint64_t> low_water_frames{kCoreAudioRingFrames};
  std::atomic<uint64_t> high_water_frames{0};
  uint32_t diagnostic_client_index = UINT32_MAX;
  CoreAudioSignalDiagnosticQueue signal_diagnostics;
  CoreAudioSignalEpisodeState signal_episode;
};

struct CoreAudioOutputMetrics {
  uint64_t callback_count = 0;
  uint64_t callback_frames = 0;
  uint64_t underrun_frames = 0;
  uint64_t dropped_blocks = 0;
  uint64_t ring_low_frames = 0;
  uint64_t ring_high_frames = 0;
  uint32_t maximum_credit_depth = 0;
  uint64_t maximum_callback_ticks = 0;
  uint64_t callback_buffer_errors = 0;
  uint64_t device_overloads = 0;
  uint64_t rebuffer_events = 0;
  uint64_t credit_release_failures = 0;
  uint64_t loud_events = 0;
  uint64_t clipped_samples = 0;
  uint64_t nonfinite_samples = 0;
  uint64_t dropped_signal_diagnostics = 0;
};

class CoreAudioOutput {
 public:
  CoreAudioOutput();
  ~CoreAudioOutput();

  CoreAudioOutput(const CoreAudioOutput&) = delete;
  CoreAudioOutput& operator=(const CoreAudioOutput&) = delete;

  bool Initialize();
  void Shutdown();

  bool AttachClient(CoreAudioClientState* client);
  void DetachClient(CoreAudioClientState* client);
  void PauseClient(CoreAudioClientState* client);
  void ResumeClient(CoreAudioClientState* client);
  void FlushClient(CoreAudioClientState* client, bool return_credits);
  void NotifyProducerWork();
  void InspectSubmittedBlock(CoreAudioClientState* client, const float* samples,
                             uint32_t frame_count, uint32_t channels,
                             uint32_t guest_frame_ptr, bool input_accepted,
                             uint64_t ring_read_before,
                             uint64_t ring_read_after,
                             uint64_t ring_write_before,
                             uint64_t ring_write_after,
                             uint64_t ring_available_before,
                             uint64_t ring_available_after);

  bool available() const { return available_.load(std::memory_order_acquire); }
  uint32_t channel_count() const { return channel_count_.load(std::memory_order_acquire); }
  uint32_t RecommendedInitialCredits(uint32_t configured_maximum) const;
  CoreAudioOutputMetrics SnapshotMetrics() const;

 private:
  bool ConfigureDevice();
  void DisposeDevice();
  bool StartIfPrerolled();
  void StopOutput();
  void ControlThreadMain();
  void DrainRetiredCredits();
  void ReconfigureDevice();
  void LogMetrics();
  void RefreshSignalDiagnosticSettings();
  void DrainSignalDiagnostics();
  void LogSignalDiagnostic(const CoreAudioSignalDiagnostic& diagnostic);
  void FlushClientLocked(CoreAudioClientState* client, bool return_credits);
  bool AllClientsPaused() const;

  static OSStatus DevicePropertyChanged(AudioObjectID object_id, UInt32 address_count,
                                        const AudioObjectPropertyAddress* addresses, void* context);
  static OSStatus ProcessorOverload(AudioObjectID object_id, UInt32 address_count,
                                    const AudioObjectPropertyAddress* addresses, void* context);
  static OSStatus Render(void* context, AudioUnitRenderActionFlags* action_flags,
                         const AudioTimeStamp* timestamp, UInt32 bus_number, UInt32 frame_count,
                         AudioBufferList* buffer_list);
  OSStatus RenderFrames(UInt32 frame_count, AudioBufferList* buffer_list);

  std::array<std::atomic<CoreAudioClientState*>, kMaximumCoreAudioClients> clients_{};
  std::mutex control_mutex_;
  std::condition_variable control_wake_;
  std::thread control_thread_;
  std::atomic<bool> control_running_{false};
  std::atomic<bool> available_{false};
  std::atomic<bool> output_running_{false};
  std::atomic<bool> reconfigure_requested_{false};
  std::atomic<bool> rebuffer_requested_{false};
  std::atomic<bool> callback_active_{false};
  std::atomic<bool> muted_{false};
  std::atomic<uint32_t> channel_count_{2};
  std::atomic<uint32_t> device_period_frames_{256};
  std::atomic<uint32_t> maximum_frames_per_slice_{4096};
  std::atomic<uint64_t> callback_count_{0};
  std::atomic<uint64_t> callback_frames_{0};
  std::atomic<uint64_t> callback_buffer_errors_{0};
  std::atomic<uint64_t> callback_max_ticks_{0};
  std::atomic<uint64_t> device_overloads_{0};
  std::atomic<uint64_t> rebuffer_events_{0};
  std::atomic<uint64_t> credit_release_failures_{0};
  std::atomic<uint64_t> signal_event_sequence_{0};
  std::atomic<uint64_t> signal_loud_events_{0};
  std::atomic<uint64_t> signal_clipped_samples_{0};
  std::atomic<uint64_t> signal_nonfinite_samples_{0};
  std::atomic<uint64_t> signal_diagnostics_dropped_{0};
  std::atomic<bool> signal_diagnostics_enabled_{true};
  std::atomic<float> signal_loud_threshold_{0.95f};
  std::atomic<uint32_t> signal_log_interval_blocks_{32};
  CoreAudioSignalDiagnosticQueue output_signal_diagnostics_;
  CoreAudioSignalEpisodeState output_signal_episode_;

  AudioComponentInstance audio_unit_ = nullptr;
  AudioDeviceID device_id_ = kAudioDeviceUnknown;
  bool system_listener_installed_ = false;
  bool device_listeners_installed_ = false;
  bool overload_listener_installed_ = false;
  std::chrono::steady_clock::time_point next_metrics_log_{};
  std::chrono::steady_clock::time_point next_reconfigure_retry_{};
};

}  // namespace rex::audio::coreaudio
