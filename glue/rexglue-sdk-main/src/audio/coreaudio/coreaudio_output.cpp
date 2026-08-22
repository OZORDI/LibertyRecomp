/**
 ******************************************************************************
 * ReXGlue : native macOS CoreAudio output                                    *
 ******************************************************************************
 */

#include <rex/platform.h>

#if REX_PLATFORM_MAC && !REX_PLATFORM_IOS

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstring>
#include <limits>
#include <string>
#include <vector>

#include <mach/mach_time.h>
#include <pthread/qos.h>

#include <rex/audio/coreaudio/coreaudio_output.h>
#include <rex/audio/flags.h>
#include <rex/cvar.h>
#include <rex/diagnostics/gta4_transition.h>
#include <rex/diagnostics/policy.h>
#include <rex/logging.h>

REXCVAR_DEFINE_INT32(audio_device_period_frames, 256, "Audio/CoreAudio",
                     "Requested CoreAudio device period in sample frames (0 keeps device default)");
REXCVAR_DEFINE_INT32(audio_coreaudio_preroll_blocks, 64, "Audio/CoreAudio",
                     "Guest audio blocks buffered before native playback starts (range 1-64)");
REXCVAR_DEFINE_STRING(audio_surround, "auto", "Audio/CoreAudio",
                      "CoreAudio channel mode: auto, stereo, or 5.1");
REXCVAR_DEFINE_BOOL(audio_coreaudio_metrics, false, "Audio/CoreAudio",
                    "Periodically log bounded CoreAudio queue and callback metrics");
REXCVAR_DEFINE_BOOL(audio_coreaudio_signal_diagnostics, true, "Audio/CoreAudio",
                    "Log bounded producer and pre-clamp signal diagnostics when audio is loud");
REXCVAR_DEFINE_DOUBLE(audio_coreaudio_loud_threshold, 0.95, "Audio/CoreAudio",
                      "Absolute Float32 sample level that starts a loud-audio diagnostic episode");
REXCVAR_DEFINE_INT32(audio_coreaudio_signal_log_interval_blocks, 32, "Audio/CoreAudio",
                     "Blocks between detailed records during a sustained loud-audio episode");

namespace rex::audio::coreaudio {
namespace {

constexpr auto kControlPollInterval = std::chrono::milliseconds(1);
constexpr auto kMetricsInterval = std::chrono::seconds(5);
constexpr auto kDeviceRetryInterval = std::chrono::seconds(2);

void CopyDiagnosticFrame(const float* samples, uint32_t frame_count, uint32_t channels,
                         uint32_t sample_index,
                         std::array<float, kGuestAudioChannels>* destination) {
  if (!samples || !destination || !channels || sample_index == UINT32_MAX) {
    return;
  }
  const uint32_t frame = sample_index / channels;
  if (frame >= frame_count) {
    return;
  }
  for (uint32_t channel = 0; channel < channels; ++channel) {
    (*destination)[channel] = samples[size_t(frame) * channels + channel];
  }
}

bool AnalyzeSignal(const float* samples, uint32_t frame_count, uint32_t channels,
                   float loud_threshold, CoreAudioSignalDiagnostic* diagnostic) {
  if (!samples || !diagnostic || !frame_count || !channels ||
      channels > kGuestAudioChannels) {
    return false;
  }

  diagnostic->frame_count = frame_count;
  diagnostic->channels = channels;
  diagnostic->loud_threshold = loud_threshold;
  std::array<double, kGuestAudioChannels> sums{};
  std::array<double, kGuestAudioChannels> square_sums{};
  std::array<uint32_t, kGuestAudioChannels> finite_counts{};
  for (uint32_t channel = 0; channel < channels; ++channel) {
    diagnostic->minimum[channel] = std::numeric_limits<float>::infinity();
    diagnostic->maximum[channel] = -std::numeric_limits<float>::infinity();
  }

  uint32_t current_loud_run = 0;
  const size_t sample_count = size_t(frame_count) * channels;
  for (size_t index = 0; index < sample_count; ++index) {
    const uint32_t channel = static_cast<uint32_t>(index % channels);
    const float sample = samples[index];
    if (!std::isfinite(sample)) {
      if (diagnostic->first_clipped_sample == UINT32_MAX) {
        diagnostic->first_clipped_sample = static_cast<uint32_t>(index);
      }
      ++diagnostic->nonfinite_samples;
      ++current_loud_run;
      diagnostic->longest_loud_run =
          std::max(diagnostic->longest_loud_run, current_loud_run);
      continue;
    }

    diagnostic->minimum[channel] = std::min(diagnostic->minimum[channel], sample);
    diagnostic->maximum[channel] = std::max(diagnostic->maximum[channel], sample);
    sums[channel] += sample;
    square_sums[channel] += double(sample) * sample;
    ++finite_counts[channel];

    const float absolute = std::abs(sample);
    diagnostic->peak_per_channel[channel] =
        std::max(diagnostic->peak_per_channel[channel], absolute);
    if (absolute > diagnostic->peak || diagnostic->peak_sample == UINT32_MAX) {
      diagnostic->peak = absolute;
      diagnostic->peak_sample = static_cast<uint32_t>(index);
    }
    if (absolute >= loud_threshold) {
      if (diagnostic->first_loud_sample == UINT32_MAX) {
        diagnostic->first_loud_sample = static_cast<uint32_t>(index);
      }
      ++diagnostic->loud_samples;
      ++current_loud_run;
      diagnostic->longest_loud_run =
          std::max(diagnostic->longest_loud_run, current_loud_run);
    } else {
      current_loud_run = 0;
    }
    if (absolute > 1.0f) {
      if (diagnostic->first_clipped_sample == UINT32_MAX) {
        diagnostic->first_clipped_sample = static_cast<uint32_t>(index);
      }
      ++diagnostic->clipped_samples;
      if (sample > 0.0f) {
        ++diagnostic->positive_clipped_samples;
      } else {
        ++diagnostic->negative_clipped_samples;
      }
    }
  }

  for (uint32_t channel = 0; channel < channels; ++channel) {
    if (!finite_counts[channel]) {
      diagnostic->minimum[channel] = 0.0f;
      diagnostic->maximum[channel] = 0.0f;
      continue;
    }
    diagnostic->mean[channel] =
        static_cast<float>(sums[channel] / finite_counts[channel]);
    diagnostic->rms[channel] =
        static_cast<float>(std::sqrt(square_sums[channel] / finite_counts[channel]));
  }
  CopyDiagnosticFrame(samples, frame_count, channels, diagnostic->peak_sample,
                      &diagnostic->peak_frame);
  CopyDiagnosticFrame(samples, frame_count, channels, diagnostic->first_clipped_sample,
                      &diagnostic->first_clipped_frame);
  return true;
}

uint32_t UpdateSignalEpisode(CoreAudioSignalEpisodeState* episode,
                             CoreAudioSignalDiagnostic* diagnostic,
                             uint32_t periodic_interval) {
  const bool loud_now = diagnostic->loud_samples || diagnostic->clipped_samples ||
                        diagnostic->nonfinite_samples;
  const bool clip_now = diagnostic->clipped_samples != 0;
  const bool nonfinite_now = diagnostic->nonfinite_samples != 0;
  uint32_t reasons = kCoreAudioSignalReasonNone;

  if (loud_now && !episode->loud_active) {
    ++episode->episode_id;
    episode->blocks = 0;
    episode->frames = 0;
    episode->loud_samples = 0;
    episode->clipped_samples = 0;
    episode->nonfinite_samples = 0;
    episode->blocks_since_log = 0;
    episode->peak = 0.0f;
    reasons |= kCoreAudioSignalReasonLoudStart;
  }
  if (clip_now && !episode->clip_active) {
    reasons |= kCoreAudioSignalReasonClipStart;
  }
  if (nonfinite_now && !episode->nonfinite_active) {
    reasons |= kCoreAudioSignalReasonNonfinite;
  }

  if (loud_now) {
    ++episode->blocks;
    episode->frames += diagnostic->frame_count;
    episode->loud_samples += diagnostic->loud_samples;
    episode->clipped_samples += diagnostic->clipped_samples;
    episode->nonfinite_samples += diagnostic->nonfinite_samples;
    episode->peak = std::max(episode->peak, diagnostic->peak);
    ++episode->blocks_since_log;
    if (reasons == kCoreAudioSignalReasonNone &&
        episode->blocks_since_log >= std::max(periodic_interval, 1U)) {
      reasons |= kCoreAudioSignalReasonPeriodic;
    }
  } else if (episode->loud_active) {
    reasons |= kCoreAudioSignalReasonEpisodeEnd;
  }

  diagnostic->episode_id = episode->episode_id;
  diagnostic->episode_blocks = episode->blocks;
  diagnostic->episode_frames = episode->frames;
  diagnostic->episode_loud_samples = episode->loud_samples;
  diagnostic->episode_clipped_samples = episode->clipped_samples;
  diagnostic->episode_nonfinite_samples = episode->nonfinite_samples;
  diagnostic->episode_peak = episode->peak;

  if (reasons != kCoreAudioSignalReasonNone) {
    episode->blocks_since_log = 0;
  }
  episode->loud_active = loud_now;
  episode->clip_active = clip_now;
  episode->nonfinite_active = nonfinite_now;
  return reasons;
}

const char* SignalStageName(CoreAudioSignalStage stage) {
  switch (stage) {
    case CoreAudioSignalStage::kGuestProducer:
      return "guest-producer";
    case CoreAudioSignalStage::kPreClampMix:
      return "pre-clamp-mix";
  }
  return "unknown";
}

template <typename T>
bool ReadProperty(AudioObjectID object, AudioObjectPropertySelector selector,
                  AudioObjectPropertyScope scope, T* value) {
  AudioObjectPropertyAddress address{selector, scope, kAudioObjectPropertyElementMain};
  UInt32 size = sizeof(T);
  return AudioObjectGetPropertyData(object, &address, 0, nullptr, &size, value) == noErr;
}

uint32_t QueryOutputChannelCount(AudioDeviceID device) {
  AudioObjectPropertyAddress address{kAudioDevicePropertyStreamConfiguration,
                                     kAudioDevicePropertyScopeOutput,
                                     kAudioObjectPropertyElementMain};
  UInt32 size = 0;
  if (AudioObjectGetPropertyDataSize(device, &address, 0, nullptr, &size) != noErr || !size) {
    return 0;
  }
  std::vector<uint8_t> storage(size);
  auto* buffers = reinterpret_cast<AudioBufferList*>(storage.data());
  if (AudioObjectGetPropertyData(device, &address, 0, nullptr, &size, buffers) != noErr) {
    return 0;
  }
  uint32_t channels = 0;
  for (UInt32 i = 0; i < buffers->mNumberBuffers; ++i) {
    channels += buffers->mBuffers[i].mNumberChannels;
  }
  return channels;
}

void UpdateMaximum(std::atomic<uint64_t>& target, uint64_t value) {
  uint64_t previous = target.load(std::memory_order_relaxed);
  while (previous < value &&
         !target.compare_exchange_weak(previous, value, std::memory_order_relaxed)) {}
}

void UpdateMinimum(std::atomic<uint64_t>& target, uint64_t value) {
  uint64_t previous = target.load(std::memory_order_relaxed);
  while (previous > value &&
         !target.compare_exchange_weak(previous, value, std::memory_order_relaxed)) {}
}

std::array<AudioObjectPropertyAddress, 5> DeviceListenerAddresses() {
  return {{{kAudioDevicePropertyDeviceIsAlive, kAudioObjectPropertyScopeGlobal,
            kAudioObjectPropertyElementMain},
           {kAudioDevicePropertyNominalSampleRate, kAudioObjectPropertyScopeGlobal,
            kAudioObjectPropertyElementMain},
           {kAudioDevicePropertyBufferFrameSize, kAudioObjectPropertyScopeGlobal,
            kAudioObjectPropertyElementMain},
           {kAudioDevicePropertyStreamConfiguration, kAudioDevicePropertyScopeOutput,
            kAudioObjectPropertyElementMain},
           {kAudioDevicePropertyIOStoppedAbnormally, kAudioObjectPropertyScopeGlobal,
            kAudioObjectPropertyElementMain}}};
}

AudioObjectPropertyAddress ProcessorOverloadAddress() {
  return {kAudioDeviceProcessorOverload, kAudioObjectPropertyScopeGlobal,
          kAudioObjectPropertyElementMain};
}

}  // namespace

CoreAudioOutput::CoreAudioOutput() {
  for (auto& client : clients_) {
    client.store(nullptr, std::memory_order_relaxed);
  }
}

CoreAudioOutput::~CoreAudioOutput() {
  Shutdown();
}

bool CoreAudioOutput::Initialize() {
  if (control_running_.exchange(true)) {
    return available();
  }

  RefreshSignalDiagnosticSettings();

  AudioObjectPropertyAddress default_device_address{kAudioHardwarePropertyDefaultOutputDevice,
                                                    kAudioObjectPropertyScopeGlobal,
                                                    kAudioObjectPropertyElementMain};
  if (AudioObjectAddPropertyListener(kAudioObjectSystemObject, &default_device_address,
                                     DevicePropertyChanged, this) == noErr) {
    system_listener_installed_ = true;
  } else {
    REXAPU_WARN("CoreAudio: unable to install default-output listener");
  }

  const bool configured = ConfigureDevice();
  available_.store(configured, std::memory_order_release);
  next_reconfigure_retry_ = std::chrono::steady_clock::now() + kDeviceRetryInterval;
  next_metrics_log_ = std::chrono::steady_clock::now() + kMetricsInterval;
  control_thread_ = std::thread([this]() { ControlThreadMain(); });
  return configured;
}

void CoreAudioOutput::Shutdown() {
  if (!control_running_.exchange(false)) {
    return;
  }
  control_wake_.notify_all();
  if (control_thread_.joinable()) {
    control_thread_.join();
  }

  StopOutput();
  DisposeDevice();

  if (system_listener_installed_) {
    AudioObjectPropertyAddress default_device_address{kAudioHardwarePropertyDefaultOutputDevice,
                                                      kAudioObjectPropertyScopeGlobal,
                                                      kAudioObjectPropertyElementMain};
    AudioObjectRemovePropertyListener(kAudioObjectSystemObject, &default_device_address,
                                      DevicePropertyChanged, this);
    system_listener_installed_ = false;
  }
}

bool CoreAudioOutput::AttachClient(CoreAudioClientState* client) {
  if (!client || !client->ring.valid()) {
    return false;
  }
  std::lock_guard<std::mutex> lock(control_mutex_);
  client->channels.store(channel_count(), std::memory_order_release);
  const uint32_t credit_limit = static_cast<uint32_t>(std::clamp(
      REXCVAR_GET(audio_maxqframes), 1, static_cast<int32_t>(kMaximumCoreAudioGuestBlocks)));
  client->credit_limit.store(credit_limit, std::memory_order_release);
  client->credit_depth.store(RecommendedInitialCredits(credit_limit),
                             std::memory_order_release);
  client->accepting.store(true, std::memory_order_release);
  client->paused.store(false, std::memory_order_release);
  for (uint32_t client_index = 0; client_index < clients_.size(); ++client_index) {
    auto& slot = clients_[client_index];
    CoreAudioClientState* expected = nullptr;
    if (slot.compare_exchange_strong(expected, client, std::memory_order_release,
                                     std::memory_order_relaxed)) {
      client->diagnostic_client_index = client_index;
      control_wake_.notify_all();
      return true;
    }
  }
  return false;
}

void CoreAudioOutput::DetachClient(CoreAudioClientState* client) {
  if (!client) {
    return;
  }
  client->accepting.store(false, std::memory_order_release);
  for (auto& slot : clients_) {
    CoreAudioClientState* expected = client;
    slot.compare_exchange_strong(expected, nullptr, std::memory_order_acq_rel,
                                 std::memory_order_relaxed);
  }
  while (callback_active_.load(std::memory_order_acquire)) {
    std::this_thread::yield();
  }
  FlushClient(client, false);
}

void CoreAudioOutput::PauseClient(CoreAudioClientState* client) {
  if (!client) {
    return;
  }
  client->paused.store(true, std::memory_order_release);
  while (callback_active_.load(std::memory_order_acquire)) {
    std::this_thread::yield();
  }
  FlushClient(client, false);
  if (AllClientsPaused()) {
    StopOutput();
  }
}

void CoreAudioOutput::ResumeClient(CoreAudioClientState* client) {
  if (!client) {
    return;
  }
  client->paused.store(false, std::memory_order_release);
  control_wake_.notify_all();
}

void CoreAudioOutput::FlushClient(CoreAudioClientState* client, bool return_credits) {
  if (!client) {
    return;
  }
  std::lock_guard<std::mutex> lock(control_mutex_);
  FlushClientLocked(client, return_credits);
}

void CoreAudioOutput::FlushClientLocked(CoreAudioClientState* client, bool return_credits) {
  if (!client) {
    return;
  }
  client->ring.DiscardAll();
  const uint64_t submitted = client->submitted_blocks.load(std::memory_order_acquire);
  const uint64_t retired = client->retired_blocks_total.load(std::memory_order_acquire);
  if (submitted > retired) {
    const uint64_t discarded = submitted - retired;
    client->retired_blocks_total.store(submitted, std::memory_order_release);
    if (return_credits) {
      client->retired_blocks_pending.fetch_add(discarded, std::memory_order_release);
    } else {
      client->retired_blocks_pending.store(0, std::memory_order_release);
      client->adaptive_credit_requests.store(0, std::memory_order_release);
      client->credit_depth.store(
          RecommendedInitialCredits(client->credit_limit.load(std::memory_order_acquire)),
          std::memory_order_release);
    }
  } else if (!return_credits) {
    client->retired_blocks_pending.store(0, std::memory_order_release);
    client->adaptive_credit_requests.store(0, std::memory_order_release);
    client->credit_depth.store(
        RecommendedInitialCredits(client->credit_limit.load(std::memory_order_acquire)),
        std::memory_order_release);
  }
}

void CoreAudioOutput::NotifyProducerWork() {
  control_wake_.notify_all();
}

void CoreAudioOutput::InspectSubmittedBlock(CoreAudioClientState* client,
                                            const float* samples,
                                            uint32_t frame_count,
                                            uint32_t channels,
                                            uint32_t guest_frame_ptr,
                                            bool input_accepted,
                                            uint64_t ring_read_before,
                                            uint64_t ring_read_after,
                                            uint64_t ring_write_before,
                                            uint64_t ring_write_after,
                                            uint64_t ring_available_before,
                                            uint64_t ring_available_after) {
  if (!client || !signal_diagnostics_enabled_.load(std::memory_order_relaxed)) {
    return;
  }

  CoreAudioSignalDiagnostic diagnostic;
  diagnostic.stage = CoreAudioSignalStage::kGuestProducer;
  diagnostic.client_index = client->diagnostic_client_index;
  diagnostic.guest_frame_ptr = guest_frame_ptr;
  diagnostic.input_accepted = input_accepted;
  diagnostic.ring_available_before = ring_available_before;
  diagnostic.ring_available_after = ring_available_after;
  diagnostic.ring_read_before = ring_read_before;
  diagnostic.ring_read_after = ring_read_after;
  diagnostic.ring_write_before = ring_write_before;
  diagnostic.ring_write_after = ring_write_after;
  diagnostic.submitted_blocks =
      client->submitted_blocks.load(std::memory_order_acquire);
  diagnostic.source_sequence = diagnostic.submitted_blocks;
  if (!AnalyzeSignal(samples, frame_count, channels,
                     signal_loud_threshold_.load(std::memory_order_relaxed),
                     &diagnostic)) {
    return;
  }

  diagnostic.reason_flags = UpdateSignalEpisode(
      &client->signal_episode, &diagnostic,
      signal_log_interval_blocks_.load(std::memory_order_relaxed));
  if (!input_accepted) {
    diagnostic.reason_flags |= kCoreAudioSignalReasonDroppedInput;
  }
  if (diagnostic.reason_flags == kCoreAudioSignalReasonNone) {
    return;
  }
  diagnostic.event_sequence =
      signal_event_sequence_.fetch_add(1, std::memory_order_relaxed) + 1;
  client->signal_diagnostics.Push(diagnostic);
  control_wake_.notify_all();
}

uint32_t CoreAudioOutput::RecommendedInitialCredits(uint32_t configured_maximum) const {
  const uint32_t period =
      std::max(device_period_frames_.load(std::memory_order_acquire), 1U);
  const uint32_t period_blocks =
      (period + kGuestAudioFramesPerBlock - 1) / kGuestAudioFramesPerBlock;
  const uint32_t reliability_preroll = static_cast<uint32_t>(std::clamp(
      REXCVAR_GET(audio_coreaudio_preroll_blocks), 1,
      static_cast<int32_t>(kMaximumCoreAudioGuestBlocks)));
  return std::clamp(std::max(period_blocks + 4U, reliability_preroll), 1U,
                    std::max(configured_maximum, 1U));
}

bool CoreAudioOutput::ConfigureDevice() {
  available_.store(false, std::memory_order_release);

  if (!ReadProperty(kAudioObjectSystemObject, kAudioHardwarePropertyDefaultOutputDevice,
                    kAudioObjectPropertyScopeGlobal, &device_id_) ||
      device_id_ == kAudioDeviceUnknown) {
    REXAPU_ERROR("CoreAudio: no default output device; using paced silent output");
    return false;
  }

  UInt32 alive = 0;
  if (!ReadProperty(device_id_, kAudioDevicePropertyDeviceIsAlive, kAudioObjectPropertyScopeGlobal,
                    &alive) ||
      !alive) {
    REXAPU_ERROR("CoreAudio: default output device is not alive");
    return false;
  }

  const uint32_t physical_channels = QueryOutputChannelCount(device_id_);
  const std::string surround_mode = REXCVAR_GET(audio_surround);
  const bool request_surround = surround_mode == "5.1" || surround_mode == "surround" ||
                                (surround_mode == "auto" && physical_channels >= 6);
  const uint32_t channels = request_surround && physical_channels >= 6 ? 6U : 2U;

  const int32_t requested_period = REXCVAR_GET(audio_device_period_frames);
  if (requested_period > 0) {
    AudioObjectPropertyAddress period_address{kAudioDevicePropertyBufferFrameSize,
                                              kAudioObjectPropertyScopeGlobal,
                                              kAudioObjectPropertyElementMain};
    Boolean settable = false;
    if (AudioObjectIsPropertySettable(device_id_, &period_address, &settable) == noErr &&
        settable) {
      AudioValueRange range{};
      if (ReadProperty(device_id_, kAudioDevicePropertyBufferFrameSizeRange,
                       kAudioObjectPropertyScopeGlobal, &range) &&
          requested_period >= range.mMinimum && requested_period <= range.mMaximum) {
        UInt32 requested = static_cast<UInt32>(requested_period);
        const UInt32 size = sizeof(requested);
        const OSStatus status =
            AudioObjectSetPropertyData(device_id_, &period_address, 0, nullptr, size, &requested);
        if (status != noErr) {
          REXAPU_WARN("CoreAudio: device rejected requested {}-frame period (status {})", requested,
                      status);
        }
      }
    }
  }

  UInt32 device_period = 0;
  if (ReadProperty(device_id_, kAudioDevicePropertyBufferFrameSize, kAudioObjectPropertyScopeGlobal,
                   &device_period) &&
      device_period) {
    device_period_frames_.store(device_period, std::memory_order_release);
  }

  Float64 nominal_rate = 0;
  ReadProperty(device_id_, kAudioDevicePropertyNominalSampleRate, kAudioObjectPropertyScopeGlobal,
               &nominal_rate);

  AudioComponentDescription description{};
  description.componentType = kAudioUnitType_Output;
  description.componentSubType = kAudioUnitSubType_DefaultOutput;
  description.componentManufacturer = kAudioUnitManufacturer_Apple;
  const AudioComponent component = AudioComponentFindNext(nullptr, &description);
  if (!component || AudioComponentInstanceNew(component, &audio_unit_) != noErr || !audio_unit_) {
    REXAPU_ERROR("CoreAudio: unable to create Default Output Audio Unit");
    audio_unit_ = nullptr;
    return false;
  }

  AudioStreamBasicDescription format{};
  format.mSampleRate = kGuestAudioSampleRate;
  format.mFormatID = kAudioFormatLinearPCM;
  format.mFormatFlags =
      kAudioFormatFlagIsFloat | kAudioFormatFlagIsPacked | kAudioFormatFlagsNativeEndian;
  format.mBytesPerPacket = channels * sizeof(float);
  format.mFramesPerPacket = 1;
  format.mBytesPerFrame = channels * sizeof(float);
  format.mChannelsPerFrame = channels;
  format.mBitsPerChannel = sizeof(float) * 8;
  OSStatus status = AudioUnitSetProperty(audio_unit_, kAudioUnitProperty_StreamFormat,
                                         kAudioUnitScope_Input, 0, &format, sizeof(format));
  if (status != noErr) {
    REXAPU_ERROR("CoreAudio: unable to set {}-channel 48 kHz Float32 client format (status {})",
                 channels, status);
    DisposeDevice();
    return false;
  }

  if (channels == 6) {
    AudioChannelLayout layout{};
    layout.mChannelLayoutTag = kAudioChannelLayoutTag_MPEG_5_1_A;
    status = AudioUnitSetProperty(audio_unit_, kAudioUnitProperty_AudioChannelLayout,
                                  kAudioUnitScope_Input, 0, &layout, sizeof(layout));
    if (status != noErr) {
      REXAPU_WARN("CoreAudio: unable to set 5.1 channel layout (status {}); retrying stereo",
                  status);
      DisposeDevice();
      channel_count_.store(2, std::memory_order_release);
      REXCVAR_SET(audio_surround, "stereo");
      return ConfigureDevice();
    }
  }

  AURenderCallbackStruct callback{Render, this};
  status = AudioUnitSetProperty(audio_unit_, kAudioUnitProperty_SetRenderCallback,
                                kAudioUnitScope_Input, 0, &callback, sizeof(callback));
  if (status != noErr) {
    REXAPU_ERROR("CoreAudio: unable to install output callback (status {})", status);
    DisposeDevice();
    return false;
  }

  UInt32 maximum_frames = 0;
  UInt32 maximum_frames_size = sizeof(maximum_frames);
  if (AudioUnitGetProperty(audio_unit_, kAudioUnitProperty_MaximumFramesPerSlice,
                           kAudioUnitScope_Global, 0, &maximum_frames,
                           &maximum_frames_size) == noErr &&
      maximum_frames) {
    maximum_frames_per_slice_.store(maximum_frames, std::memory_order_release);
  }

  status = AudioUnitInitialize(audio_unit_);
  if (status != noErr) {
    REXAPU_ERROR("CoreAudio: AudioUnitInitialize failed (status {})", status);
    DisposeDevice();
    return false;
  }

  for (const auto& address : DeviceListenerAddresses()) {
    AudioObjectAddPropertyListener(device_id_, &address, DevicePropertyChanged, this);
  }
  device_listeners_installed_ = true;
  const AudioObjectPropertyAddress overload_address = ProcessorOverloadAddress();
  overload_listener_installed_ = AudioObjectAddPropertyListener(device_id_, &overload_address,
                                                                ProcessorOverload, this) == noErr;
  channel_count_.store(channels, std::memory_order_release);
  REXAPU_INFO(
      "CoreAudio: initialized default output device {} (physical_channels={}, client_channels={}, "
      "device_rate={}, period={}, max_slice={})",
      device_id_, physical_channels, channels, nominal_rate,
      device_period_frames_.load(std::memory_order_relaxed),
      maximum_frames_per_slice_.load(std::memory_order_relaxed));
  return true;
}

void CoreAudioOutput::DisposeDevice() {
  available_.store(false, std::memory_order_release);
  if (overload_listener_installed_ && device_id_ != kAudioDeviceUnknown) {
    const AudioObjectPropertyAddress overload_address = ProcessorOverloadAddress();
    AudioObjectRemovePropertyListener(device_id_, &overload_address, ProcessorOverload, this);
    overload_listener_installed_ = false;
  }
  if (device_listeners_installed_ && device_id_ != kAudioDeviceUnknown) {
    for (const auto& address : DeviceListenerAddresses()) {
      AudioObjectRemovePropertyListener(device_id_, &address, DevicePropertyChanged, this);
    }
    device_listeners_installed_ = false;
  }
  if (audio_unit_) {
    AudioUnitUninitialize(audio_unit_);
    AudioComponentInstanceDispose(audio_unit_);
    audio_unit_ = nullptr;
  }
  device_id_ = kAudioDeviceUnknown;
}

bool CoreAudioOutput::StartIfPrerolled() {
  if (!available() || output_running_.load(std::memory_order_acquire) || !audio_unit_) {
    return false;
  }
  bool has_client = false;
  for (const auto& slot : clients_) {
    CoreAudioClientState* client = slot.load(std::memory_order_acquire);
    if (!client || client->paused.load(std::memory_order_acquire)) {
      continue;
    }
    has_client = true;
    const uint64_t target_frames =
        uint64_t(client->credit_depth.load(std::memory_order_acquire)) *
        kGuestAudioFramesPerBlock;
    if (client->ring.available_frames() < target_frames) {
      return false;
    }
  }
  if (!has_client) {
    return false;
  }

  const OSStatus status = AudioOutputUnitStart(audio_unit_);
  if (status != noErr) {
    REXAPU_ERROR("CoreAudio: AudioOutputUnitStart failed (status {})", status);
    reconfigure_requested_.store(true, std::memory_order_release);
    return false;
  }
  output_running_.store(true, std::memory_order_release);
  REXAPU_INFO("CoreAudio: output started after reliability preroll");
  return true;
}

void CoreAudioOutput::StopOutput() {
  if (output_running_.exchange(false) && audio_unit_) {
    AudioOutputUnitStop(audio_unit_);
  }
  while (callback_active_.load(std::memory_order_acquire)) {
    std::this_thread::yield();
  }
}

void CoreAudioOutput::ControlThreadMain() {
  rex::thread::set_current_thread_name("CoreAudio Control");
  pthread_set_qos_class_self_np(QOS_CLASS_USER_INTERACTIVE, 0);
  while (control_running_.load(std::memory_order_acquire)) {
    RefreshSignalDiagnosticSettings();
    const auto now = std::chrono::steady_clock::now();
    const bool retry_unavailable = !available() && now >= next_reconfigure_retry_;
    const bool reconfigure =
        reconfigure_requested_.exchange(false, std::memory_order_acq_rel) || retry_unavailable;
    if (reconfigure) {
      rebuffer_requested_.store(false, std::memory_order_release);
      ReconfigureDevice();
    } else if (rebuffer_requested_.exchange(false, std::memory_order_acq_rel)) {
      StopOutput();
      rebuffer_events_.fetch_add(1, std::memory_order_relaxed);
      REXAPU_WARN("CoreAudio: output starved; rebuilding reliability preroll");
    }
    muted_.store(REXCVAR_GET(audio_mute), std::memory_order_release);
    DrainRetiredCredits();
    DrainSignalDiagnostics();
    StartIfPrerolled();
    if (std::chrono::steady_clock::now() >= next_metrics_log_) {
      LogMetrics();
      next_metrics_log_ = std::chrono::steady_clock::now() + kMetricsInterval;
    }

    std::unique_lock<std::mutex> lock(control_mutex_);
    control_wake_.wait_for(lock, kControlPollInterval);
  }
  DrainRetiredCredits();
  DrainSignalDiagnostics();
}

void CoreAudioOutput::DrainRetiredCredits() {
  std::lock_guard<std::mutex> lock(control_mutex_);
  for (const auto& slot : clients_) {
    CoreAudioClientState* client = slot.load(std::memory_order_acquire);
    if (!client || !client->semaphore) {
      continue;
    }
    uint64_t retired = client->retired_blocks_pending.exchange(0, std::memory_order_acq_rel);
    while (retired) {
      if (!client->semaphore->Release(1, nullptr)) {
        const uint64_t failure_index =
            credit_release_failures_.fetch_add(1, std::memory_order_relaxed);
        client->retired_blocks_pending.fetch_add(retired, std::memory_order_release);
        if (failure_index < 5) {
          REXAPU_WARN("CoreAudio: producer-credit semaphore full; deferring retired credits");
        }
        break;
      }
      --retired;
    }

    uint64_t adaptive =
        client->adaptive_credit_requests.exchange(0, std::memory_order_acq_rel);
    while (adaptive) {
      if (!client->semaphore->Release(1, nullptr)) {
        const uint64_t failure_index =
            credit_release_failures_.fetch_add(1, std::memory_order_relaxed);
        client->credit_depth.fetch_sub(static_cast<uint32_t>(adaptive),
                                       std::memory_order_acq_rel);
        if (failure_index < 5) {
          REXAPU_WARN("CoreAudio: adaptive credit reached producer semaphore capacity");
        }
        break;
      }
      --adaptive;
    }
  }
}

void CoreAudioOutput::ReconfigureDevice() {
  diagnostics::gta4_transition::Record(
      diagnostics::gta4_transition::EventSource::kCoreAudio,
      diagnostics::gta4_transition::EventType::kAudioDeviceChange, 0, 0, 0,
      diagnostics::gta4_transition::kFlagBefore, device_id_, channel_count(),
      device_period_frames_.load(std::memory_order_relaxed));
  REXAPU_INFO("CoreAudio: output-device change detected; rebuilding native output");
  StopOutput();
  DisposeDevice();
  {
    std::lock_guard<std::mutex> lock(control_mutex_);
    for (const auto& slot : clients_) {
      if (CoreAudioClientState* client = slot.load(std::memory_order_acquire)) {
        FlushClientLocked(client, true);
      }
    }
  }
  const bool configured = ConfigureDevice();
  next_reconfigure_retry_ = std::chrono::steady_clock::now() + kDeviceRetryInterval;
  if (!configured) {
    REXAPU_WARN("CoreAudio: device rebuild failed; clients will use paced silence until retry");
  }
  {
    std::lock_guard<std::mutex> lock(control_mutex_);
    for (const auto& slot : clients_) {
      if (CoreAudioClientState* client = slot.load(std::memory_order_acquire)) {
        client->channels.store(channel_count(), std::memory_order_release);
      }
    }
  }
  available_.store(configured, std::memory_order_release);
  diagnostics::gta4_transition::Record(
      diagnostics::gta4_transition::EventSource::kCoreAudio,
      diagnostics::gta4_transition::EventType::kAudioDeviceChange, 0, 0, 0,
      configured ? diagnostics::gta4_transition::kFlagAfter
                 : diagnostics::gta4_transition::kFlagError,
      device_id_, channel_count(), device_period_frames_.load(std::memory_order_relaxed));
}

void CoreAudioOutput::LogMetrics() {
  if (!rex::diagnostics::IsEnabled(rex::diagnostics::Category::kAudio) ||
      !REXCVAR_GET(audio_coreaudio_metrics)) {
    return;
  }
  const CoreAudioOutputMetrics metrics = SnapshotMetrics();
  REXAPU_INFO(
      "CoreAudioMetrics: callbacks={} frames={} underrun_frames={} dropped_blocks={} "
      "ring_low={} ring_high={} credit_depth={} max_callback_ticks={} "
      "callback_buffer_errors={} device_overloads={} rebuffer_events={} "
      "credit_release_failures={} loud_events={} clipped_samples={} "
      "nonfinite_samples={} dropped_signal_diagnostics={}",
      metrics.callback_count, metrics.callback_frames, metrics.underrun_frames,
      metrics.dropped_blocks, metrics.ring_low_frames, metrics.ring_high_frames,
      metrics.maximum_credit_depth, metrics.maximum_callback_ticks, metrics.callback_buffer_errors,
      metrics.device_overloads, metrics.rebuffer_events, metrics.credit_release_failures,
      metrics.loud_events, metrics.clipped_samples, metrics.nonfinite_samples,
      metrics.dropped_signal_diagnostics);
}

void CoreAudioOutput::RefreshSignalDiagnosticSettings() {
  const bool diagnostics_enabled =
      rex::diagnostics::IsEnabled(rex::diagnostics::Category::kAudio) &&
      REXCVAR_GET(audio_coreaudio_signal_diagnostics);
  signal_diagnostics_enabled_.store(diagnostics_enabled, std::memory_order_relaxed);
  if (!diagnostics_enabled) {
    return;
  }
  signal_loud_threshold_.store(
      static_cast<float>(std::clamp(REXCVAR_GET(audio_coreaudio_loud_threshold), 0.0, 1.0)),
      std::memory_order_relaxed);
  signal_log_interval_blocks_.store(
      static_cast<uint32_t>(std::max(
          REXCVAR_GET(audio_coreaudio_signal_log_interval_blocks), 1)),
      std::memory_order_relaxed);
}

void CoreAudioOutput::DrainSignalDiagnostics() {
  if (!signal_diagnostics_enabled_.load(std::memory_order_relaxed)) {
    return;
  }
  CoreAudioSignalDiagnostic diagnostic;
  for (const auto& slot : clients_) {
    CoreAudioClientState* client = slot.load(std::memory_order_acquire);
    if (!client) {
      continue;
    }
    while (client->signal_diagnostics.Pop(&diagnostic)) {
      LogSignalDiagnostic(diagnostic);
    }
    const uint64_t dropped = client->signal_diagnostics.TakeDroppedCount();
    if (dropped) {
      signal_diagnostics_dropped_.fetch_add(dropped, std::memory_order_relaxed);
      REXAPU_WARN(
          "CoreAudioSignal: stage=guest-producer point=diagnostic-queue-overflow "
          "client={} dropped_records={}",
          client->diagnostic_client_index, dropped);
    }
  }
  while (output_signal_diagnostics_.Pop(&diagnostic)) {
    LogSignalDiagnostic(diagnostic);
  }
  const uint64_t dropped = output_signal_diagnostics_.TakeDroppedCount();
  if (dropped) {
    signal_diagnostics_dropped_.fetch_add(dropped, std::memory_order_relaxed);
    REXAPU_WARN(
        "CoreAudioSignal: stage=pre-clamp-mix point=diagnostic-queue-overflow "
        "dropped_records={}",
        dropped);
  }
}

void CoreAudioOutput::LogSignalDiagnostic(
    const CoreAudioSignalDiagnostic& diagnostic) {
  const uint32_t first_loud_frame =
      diagnostic.first_loud_sample == UINT32_MAX || !diagnostic.channels
          ? UINT32_MAX
          : diagnostic.first_loud_sample / diagnostic.channels;
  const uint32_t first_loud_channel =
      diagnostic.first_loud_sample == UINT32_MAX || !diagnostic.channels
          ? UINT32_MAX
          : diagnostic.first_loud_sample % diagnostic.channels;
  const uint32_t first_clipped_frame =
      diagnostic.first_clipped_sample == UINT32_MAX || !diagnostic.channels
          ? UINT32_MAX
          : diagnostic.first_clipped_sample / diagnostic.channels;
  const uint32_t first_clipped_channel =
      diagnostic.first_clipped_sample == UINT32_MAX || !diagnostic.channels
          ? UINT32_MAX
          : diagnostic.first_clipped_sample % diagnostic.channels;
  const uint32_t peak_frame =
      diagnostic.peak_sample == UINT32_MAX || !diagnostic.channels
          ? UINT32_MAX
          : diagnostic.peak_sample / diagnostic.channels;
  const uint32_t peak_channel =
      diagnostic.peak_sample == UINT32_MAX || !diagnostic.channels
          ? UINT32_MAX
          : diagnostic.peak_sample % diagnostic.channels;
  const float peak_dbfs = diagnostic.peak > 0.0f
                              ? 20.0f * std::log10(diagnostic.peak)
                              : -std::numeric_limits<float>::infinity();

  REXAPU_WARN(
      "CoreAudioSignal: point=signal-event event={} stage={} reasons=0x{:02X} "
      "source_sequence={} episode={} episode_blocks={} episode_frames={} "
      "episode_peak={:.9f} episode_loud_samples={} episode_clipped_samples={} "
      "episode_nonfinite_samples={} loud_start={} clip_start={} periodic={} "
      "episode_end={} nonfinite_start={} dropped_input={}",
      diagnostic.event_sequence, SignalStageName(diagnostic.stage),
      diagnostic.reason_flags, diagnostic.source_sequence, diagnostic.episode_id,
      diagnostic.episode_blocks, diagnostic.episode_frames, diagnostic.episode_peak,
      diagnostic.episode_loud_samples, diagnostic.episode_clipped_samples,
      diagnostic.episode_nonfinite_samples,
      bool(diagnostic.reason_flags & kCoreAudioSignalReasonLoudStart),
      bool(diagnostic.reason_flags & kCoreAudioSignalReasonClipStart),
      bool(diagnostic.reason_flags & kCoreAudioSignalReasonPeriodic),
      bool(diagnostic.reason_flags & kCoreAudioSignalReasonEpisodeEnd),
      bool(diagnostic.reason_flags & kCoreAudioSignalReasonNonfinite),
      bool(diagnostic.reason_flags & kCoreAudioSignalReasonDroppedInput));
  REXAPU_WARN(
      "CoreAudioSignal: point=signal-level event={} threshold={:.9f} peak={:.9f} "
      "peak_dbfs={:.4f} loud_samples={} clipped_samples={} positive_clipped={} "
      "negative_clipped={} nonfinite_samples={} longest_loud_run={} "
      "first_loud_sample={} first_loud_frame={} first_loud_channel={} "
      "first_clipped_sample={} first_clipped_frame={} first_clipped_channel={} "
      "peak_sample={} peak_frame={} peak_channel={}",
      diagnostic.event_sequence, diagnostic.loud_threshold, diagnostic.peak,
      peak_dbfs, diagnostic.loud_samples, diagnostic.clipped_samples,
      diagnostic.positive_clipped_samples, diagnostic.negative_clipped_samples,
      diagnostic.nonfinite_samples, diagnostic.longest_loud_run,
      diagnostic.first_loud_sample, first_loud_frame, first_loud_channel,
      diagnostic.first_clipped_sample, first_clipped_frame, first_clipped_channel,
      diagnostic.peak_sample, peak_frame, peak_channel);
  REXAPU_WARN(
      "CoreAudioSignal: point=signal-context event={} client={} client_mask=0x{:02X} "
      "guest_frame_ptr=0x{:08X} accepted={} frames={} channels={} mixed_clients={} "
      "muted={} submitted_blocks={} ring_read_before={} ring_read_after={} "
      "ring_write_before={} ring_write_after={} "
      "ring_available_before={} ring_available_after={} callback_ticks={} "
      "device_overloads={} underrun_frames={} output_buffer_bytes={} "
      "required_buffer_bytes={} output_frame_start={}",
      diagnostic.event_sequence, diagnostic.client_index, diagnostic.client_mask,
      diagnostic.guest_frame_ptr, diagnostic.input_accepted, diagnostic.frame_count,
      diagnostic.channels, diagnostic.mixed_clients, diagnostic.muted,
      diagnostic.submitted_blocks, diagnostic.ring_read_before,
      diagnostic.ring_read_after, diagnostic.ring_write_before,
      diagnostic.ring_write_after, diagnostic.ring_available_before,
      diagnostic.ring_available_after, diagnostic.callback_ticks,
      diagnostic.device_overloads, diagnostic.underrun_frames,
      diagnostic.output_buffer_bytes, diagnostic.required_buffer_bytes,
      diagnostic.output_frame_start);
  for (uint32_t channel = 0; channel < diagnostic.channels; ++channel) {
    const float channel_peak_dbfs = diagnostic.peak_per_channel[channel] > 0.0f
                                        ? 20.0f * std::log10(
                                              diagnostic.peak_per_channel[channel])
                                        : -std::numeric_limits<float>::infinity();
    REXAPU_WARN(
        "CoreAudioSignal: point=signal-channel event={} channel={} min={:.9f} "
        "max={:.9f} peak={:.9f} peak_dbfs={:.4f} rms={:.9f} mean={:.9f} "
        "peak_frame_value={:.9f} first_clipped_frame_value={:.9f}",
        diagnostic.event_sequence, channel, diagnostic.minimum[channel],
        diagnostic.maximum[channel], diagnostic.peak_per_channel[channel],
        channel_peak_dbfs, diagnostic.rms[channel], diagnostic.mean[channel],
        diagnostic.peak_frame[channel], diagnostic.first_clipped_frame[channel]);
  }
  if (diagnostic.stage == CoreAudioSignalStage::kPreClampMix) {
    for (uint32_t client = 0; client < kMaximumCoreAudioClients; ++client) {
      if (!(diagnostic.client_mask & (1U << client))) {
        continue;
      }
      REXAPU_WARN(
          "CoreAudioSignal: point=signal-client-ring event={} client={} "
          "read_before={} read_after={} consumed={} available_after={}",
          diagnostic.event_sequence, client,
          diagnostic.client_ring_read_before[client],
          diagnostic.client_ring_read_after[client],
          diagnostic.client_ring_read_after[client] -
              diagnostic.client_ring_read_before[client],
          diagnostic.client_available_after[client]);
    }
  }
}

CoreAudioOutputMetrics CoreAudioOutput::SnapshotMetrics() const {
  CoreAudioOutputMetrics metrics;
  metrics.callback_count = callback_count_.load(std::memory_order_relaxed);
  metrics.callback_frames = callback_frames_.load(std::memory_order_relaxed);
  metrics.ring_low_frames = kCoreAudioRingFrames;
  for (const auto& slot : clients_) {
    if (CoreAudioClientState* client = slot.load(std::memory_order_acquire)) {
      metrics.underrun_frames += client->underrun_frames.load(std::memory_order_relaxed);
      metrics.dropped_blocks += client->dropped_blocks.load(std::memory_order_relaxed);
      metrics.ring_low_frames = std::min(metrics.ring_low_frames,
                                         client->low_water_frames.load(std::memory_order_relaxed));
      metrics.ring_high_frames = std::max(
          metrics.ring_high_frames, client->high_water_frames.load(std::memory_order_relaxed));
      metrics.maximum_credit_depth = std::max(
          metrics.maximum_credit_depth,
          client->credit_depth.load(std::memory_order_relaxed));
    }
  }
  metrics.maximum_callback_ticks = callback_max_ticks_.load(std::memory_order_relaxed);
  metrics.callback_buffer_errors = callback_buffer_errors_.load(std::memory_order_relaxed);
  metrics.device_overloads = device_overloads_.load(std::memory_order_relaxed);
  metrics.rebuffer_events = rebuffer_events_.load(std::memory_order_relaxed);
  metrics.credit_release_failures = credit_release_failures_.load(std::memory_order_relaxed);
  metrics.loud_events = signal_loud_events_.load(std::memory_order_relaxed);
  metrics.clipped_samples = signal_clipped_samples_.load(std::memory_order_relaxed);
  metrics.nonfinite_samples = signal_nonfinite_samples_.load(std::memory_order_relaxed);
  metrics.dropped_signal_diagnostics =
      signal_diagnostics_dropped_.load(std::memory_order_relaxed);
  return metrics;
}

bool CoreAudioOutput::AllClientsPaused() const {
  bool found = false;
  for (const auto& slot : clients_) {
    CoreAudioClientState* client = slot.load(std::memory_order_acquire);
    if (!client) {
      continue;
    }
    found = true;
    if (!client->paused.load(std::memory_order_acquire)) {
      return false;
    }
  }
  return found;
}

OSStatus CoreAudioOutput::DevicePropertyChanged(AudioObjectID, UInt32,
                                                const AudioObjectPropertyAddress*, void* context) {
  auto* output = static_cast<CoreAudioOutput*>(context);
  if (output) {
    output->reconfigure_requested_.store(true, std::memory_order_release);
    diagnostics::gta4_transition::Record(
        diagnostics::gta4_transition::EventSource::kCoreAudio,
        diagnostics::gta4_transition::EventType::kAudioDeviceChange, 0, 0, 0,
        diagnostics::gta4_transition::kFlagStateChanged);
  }
  return noErr;
}

OSStatus CoreAudioOutput::ProcessorOverload(AudioObjectID, UInt32,
                                            const AudioObjectPropertyAddress*, void* context) {
  auto* output = static_cast<CoreAudioOutput*>(context);
  if (output) {
    output->device_overloads_.fetch_add(1, std::memory_order_relaxed);
  }
  return noErr;
}

OSStatus CoreAudioOutput::Render(void* context, AudioUnitRenderActionFlags*, const AudioTimeStamp*,
                                 UInt32, UInt32 frame_count, AudioBufferList* buffer_list) {
  auto* output = static_cast<CoreAudioOutput*>(context);
  return output ? output->RenderFrames(frame_count, buffer_list) : kAudio_ParamError;
}

OSStatus CoreAudioOutput::RenderFrames(UInt32 frame_count, AudioBufferList* buffer_list) {
  const uint64_t start_ticks = mach_absolute_time();
  callback_active_.store(true, std::memory_order_release);
  const uint64_t callback_index =
      callback_count_.fetch_add(1, std::memory_order_relaxed) + 1;
  const uint64_t output_frame_start =
      callback_frames_.fetch_add(frame_count, std::memory_order_relaxed);
  diagnostics::gta4_transition::RecordSteady(
      diagnostics::gta4_transition::EventSource::kCoreAudio,
      diagnostics::gta4_transition::EventType::kAudioCallbackBegin, 0, 0, 0,
      diagnostics::gta4_transition::kFlagBefore, frame_count, channel_count(),
      maximum_frames_per_slice_.load(std::memory_order_relaxed));

  if (!buffer_list || !buffer_list->mNumberBuffers) {
    callback_active_.store(false, std::memory_order_release);
    diagnostics::gta4_transition::Record(
        diagnostics::gta4_transition::EventSource::kCoreAudio,
        diagnostics::gta4_transition::EventType::kAudioCallbackEnd, 0, 0, 0,
        diagnostics::gta4_transition::kFlagError, frame_count, kAudio_ParamError);
    return kAudio_ParamError;
  }
  for (UInt32 i = 0; i < buffer_list->mNumberBuffers; ++i) {
    AudioBuffer& buffer = buffer_list->mBuffers[i];
    if (buffer.mData && buffer.mDataByteSize) {
      std::memset(buffer.mData, 0, buffer.mDataByteSize);
    }
  }

  const uint32_t channels = channel_count();
  AudioBuffer& output_buffer = buffer_list->mBuffers[0];
  const UInt32 output_buffer_bytes = output_buffer.mDataByteSize;
  const uint64_t required_bytes = uint64_t(frame_count) * channels * sizeof(float);
  if (!output_buffer.mData || output_buffer.mDataByteSize < required_bytes ||
      frame_count > maximum_frames_per_slice_.load(std::memory_order_relaxed)) {
    callback_buffer_errors_.fetch_add(1, std::memory_order_relaxed);
    callback_active_.store(false, std::memory_order_release);
    diagnostics::gta4_transition::Record(
        diagnostics::gta4_transition::EventSource::kCoreAudio,
        diagnostics::gta4_transition::EventType::kAudioCallbackEnd, 0, 0, 0,
        diagnostics::gta4_transition::kFlagError, frame_count, output_buffer.mDataByteSize,
        required_bytes);
    return noErr;
  }

  auto* destination = static_cast<float*>(output_buffer.mData);
  bool replace = true;
  uint32_t mixed_clients = 0;
  uint64_t callback_underrun_frames = 0;
  CoreAudioSignalDiagnostic output_diagnostic;
  output_diagnostic.stage = CoreAudioSignalStage::kPreClampMix;
  output_diagnostic.source_sequence = callback_index;
  output_diagnostic.output_frame_start = output_frame_start;
  output_diagnostic.frame_count = frame_count;
  output_diagnostic.channels = channels;
  output_diagnostic.output_buffer_bytes = output_buffer_bytes;
  output_diagnostic.required_buffer_bytes = static_cast<uint32_t>(required_bytes);
  output_diagnostic.device_overloads =
      device_overloads_.load(std::memory_order_relaxed);
  output_diagnostic.muted = muted_.load(std::memory_order_relaxed);
  for (uint32_t client_index = 0; client_index < clients_.size(); ++client_index) {
    const auto& slot = clients_[client_index];
    CoreAudioClientState* client = slot.load(std::memory_order_acquire);
    if (!client || client->paused.load(std::memory_order_acquire) ||
        client->channels.load(std::memory_order_acquire) != channels) {
      continue;
    }

    const uint64_t before = client->ring.read_frame();
    const uint64_t available_before = client->ring.available_frames();
    const uint32_t consumed = client->ring.Read(destination, frame_count, channels, !replace);
    const uint64_t after = client->ring.read_frame();
    const uint64_t available_after = client->ring.available_frames();
    output_diagnostic.client_ring_read_before[client_index] = before;
    output_diagnostic.client_ring_read_after[client_index] = after;
    output_diagnostic.client_available_after[client_index] = available_after;
    output_diagnostic.ring_available_before += available_before;
    output_diagnostic.ring_available_after += available_after;
    output_diagnostic.submitted_blocks +=
        client->submitted_blocks.load(std::memory_order_relaxed);
    if (consumed) {
      replace = false;
      ++mixed_clients;
      output_diagnostic.client_mask |= 1U << client_index;
    }
    if (consumed < frame_count) {
      const uint32_t missing_frames = frame_count - consumed;
      callback_underrun_frames += missing_frames;
      client->underrun_frames.fetch_add(missing_frames, std::memory_order_relaxed);
      rebuffer_requested_.store(true, std::memory_order_release);
      uint32_t depth = client->credit_depth.load(std::memory_order_acquire);
      const uint32_t limit = client->credit_limit.load(std::memory_order_acquire);
      while (depth < limit) {
        if (client->credit_depth.compare_exchange_weak(
                depth, depth + 1, std::memory_order_acq_rel,
                std::memory_order_acquire)) {
          client->adaptive_credit_requests.fetch_add(1, std::memory_order_release);
          break;
        }
      }
      diagnostics::gta4_transition::Record(
          diagnostics::gta4_transition::EventSource::kCoreAudio,
          diagnostics::gta4_transition::EventType::kAudioUnderrun, 0, 0, 0,
          diagnostics::gta4_transition::kFlagError, frame_count, consumed,
          uint64_t(client->credit_depth.load(std::memory_order_relaxed)) << 32 |
              client->credit_limit.load(std::memory_order_relaxed));
    }
    UpdateMinimum(client->low_water_frames, available_after);
    UpdateMaximum(client->high_water_frames, available_after);
    diagnostics::gta4_transition::RecordSteady(
        diagnostics::gta4_transition::EventSource::kCoreAudio,
        diagnostics::gta4_transition::EventType::kAudioRingState, 0, 0, 0,
        diagnostics::gta4_transition::kFlagPeriodic, available_after,
        client->credit_depth.load(std::memory_order_relaxed),
        client->paused.load(std::memory_order_relaxed) ? 1 : 0);

    const uint64_t retired = after / kGuestAudioFramesPerBlock -
                             before / kGuestAudioFramesPerBlock;
    if (retired) {
      client->retired_blocks_total.fetch_add(retired, std::memory_order_relaxed);
      client->retired_blocks_pending.fetch_add(retired, std::memory_order_release);
    }
  }

  bool publish_output_diagnostic = false;
  if (signal_diagnostics_enabled_.load(std::memory_order_relaxed) &&
      AnalyzeSignal(destination, frame_count, channels,
                    signal_loud_threshold_.load(std::memory_order_relaxed),
                    &output_diagnostic)) {
    output_diagnostic.mixed_clients = mixed_clients;
    output_diagnostic.underrun_frames = callback_underrun_frames;
    output_diagnostic.reason_flags = UpdateSignalEpisode(
        &output_signal_episode_, &output_diagnostic,
        signal_log_interval_blocks_.load(std::memory_order_relaxed));
    if (output_diagnostic.reason_flags & kCoreAudioSignalReasonLoudStart) {
      signal_loud_events_.fetch_add(1, std::memory_order_relaxed);
    }
    signal_clipped_samples_.fetch_add(output_diagnostic.clipped_samples,
                                      std::memory_order_relaxed);
    signal_nonfinite_samples_.fetch_add(output_diagnostic.nonfinite_samples,
                                        std::memory_order_relaxed);
    publish_output_diagnostic =
        output_diagnostic.reason_flags != kCoreAudioSignalReasonNone;
    if (publish_output_diagnostic) {
      output_diagnostic.event_sequence =
          signal_event_sequence_.fetch_add(1, std::memory_order_relaxed) + 1;
    }
  }

  if (mixed_clients) {
    const size_t sample_count = size_t(frame_count) * channels;
    for (size_t i = 0; i < sample_count; ++i) {
      const float sample = destination[i];
      destination[i] = std::isfinite(sample) ? std::clamp(sample, -1.0f, 1.0f) : 0.0f;
    }
  }
  if (muted_.load(std::memory_order_relaxed)) {
    std::memset(destination, 0, static_cast<size_t>(required_bytes));
  }
  diagnostics::gta4_transition::CapturePcmFloatInterleaved(
      destination, frame_count, channels, kGuestAudioSampleRate);
  output_buffer.mDataByteSize = static_cast<UInt32>(required_bytes);

  callback_active_.store(false, std::memory_order_release);
  const uint64_t callback_ticks = mach_absolute_time() - start_ticks;
  if (publish_output_diagnostic) {
    output_diagnostic.callback_ticks = callback_ticks;
    output_signal_diagnostics_.Push(output_diagnostic);
  }
  UpdateMaximum(callback_max_ticks_, callback_ticks);
  diagnostics::gta4_transition::RecordSteady(
      diagnostics::gta4_transition::EventSource::kCoreAudio,
      diagnostics::gta4_transition::EventType::kAudioCallbackEnd, 0, 0, 0,
      diagnostics::gta4_transition::kFlagAfter, frame_count, mixed_clients, callback_ticks);
  return noErr;
}

}  // namespace rex::audio::coreaudio

#endif  // REX_PLATFORM_MAC && !REX_PLATFORM_IOS
