/**
 ******************************************************************************
 * ReXGlue : native macOS CoreAudio logical client                            *
 ******************************************************************************
 */

#include <rex/platform.h>

#if REX_PLATFORM_MAC && !REX_PLATFORM_IOS

#include <algorithm>
#include <thread>

#include <rex/audio/conversion.h>
#include <rex/audio/coreaudio/coreaudio_audio_driver.h>
#include <rex/diagnostics/gta4_transition.h>
#include <rex/logging.h>

namespace rex::audio::coreaudio {

CoreAudioAudioDriver::CoreAudioAudioDriver(memory::Memory* memory,
                                           rex::thread::Semaphore* semaphore,
                                           std::shared_ptr<CoreAudioOutput> output)
    : AudioDriver(memory),
      semaphore_(semaphore),
      output_(std::move(output)),
      state_(std::make_unique<CoreAudioClientState>(semaphore)) {}

CoreAudioAudioDriver::~CoreAudioAudioDriver() {
  Shutdown();
}

bool CoreAudioAudioDriver::Initialize() {
  if (!output_ || !state_ || !output_->AttachClient(state_.get())) {
    return false;
  }
  attached_ = true;
  return true;
}

void CoreAudioAudioDriver::Shutdown() {
  if (attached_ && output_ && state_) {
    output_->DetachClient(state_.get());
    attached_ = false;
  }
}

void CoreAudioAudioDriver::SubmitFrame(uint32_t frame_ptr) {
  if (!attached_ || !state_->accepting.load(std::memory_order_acquire) ||
      state_->paused.load(std::memory_order_acquire) || !output_->available()) {
    SubmitSilent();
    return;
  }

  const float* input_frame = memory_->TranslateVirtual<float*>(frame_ptr);
  const uint32_t channels = state_->channels.load(std::memory_order_acquire);
  if (channels == 6) {
    conversion::sequential_6_BE_to_interleaved_6_LE(converted_frame_.data(), input_frame,
                                                    kGuestAudioFramesPerBlock);
  } else {
    conversion::sequential_6_BE_to_interleaved_2_LE(converted_frame_.data(), input_frame,
                                                    kGuestAudioFramesPerBlock);
  }

  const uint64_t ring_read_before = state_->ring.read_frame();
  const uint64_t ring_write_before = state_->ring.write_frame();
  const uint64_t ring_available_before = state_->ring.available_frames();
  const bool input_accepted =
      state_->ring.Write(converted_frame_.data(), kGuestAudioFramesPerBlock, channels);
  if (input_accepted) {
    state_->submitted_blocks.fetch_add(1, std::memory_order_release);
  }
  const uint64_t ring_read_after = state_->ring.read_frame();
  const uint64_t ring_write_after = state_->ring.write_frame();
  const uint64_t ring_available_after = state_->ring.available_frames();
  output_->InspectSubmittedBlock(state_.get(), converted_frame_.data(),
                                 kGuestAudioFramesPerBlock, channels, frame_ptr,
                                 input_accepted, ring_read_before, ring_read_after,
                                 ring_write_before, ring_write_after, ring_available_before,
                                 ring_available_after);

  if (!input_accepted) {
    state_->dropped_blocks.fetch_add(1, std::memory_order_relaxed);
    // Return this producer credit outside the real-time callback. The frame is
    // dropped as one indivisible unit so ring framing is never corrupted.
    const bool released = semaphore_->Release(1, nullptr);
    if (!released) {
      REXAPU_ERROR("CoreAudio: failed to return credit after ring overflow");
    }
    diagnostics::gta4_transition::Record(
        diagnostics::gta4_transition::EventSource::kAudio,
        diagnostics::gta4_transition::EventType::kAudioSubmit, 0, 0, 0,
        diagnostics::gta4_transition::kFlagError, frame_ptr,
        state_->ring.available_frames(),
        state_->credit_depth.load(std::memory_order_relaxed));
    return;
  }

  const uint64_t available = ring_available_after;
  uint64_t previous = state_->high_water_frames.load(std::memory_order_relaxed);
  while (previous < available && !state_->high_water_frames.compare_exchange_weak(
                                     previous, available, std::memory_order_relaxed)) {}
  output_->NotifyProducerWork();
  diagnostics::gta4_transition::Record(
      diagnostics::gta4_transition::EventSource::kAudio,
      diagnostics::gta4_transition::EventType::kAudioRingState, 0, 0, 0,
      diagnostics::gta4_transition::kFlagAfter,
      state_->submitted_blocks.load(std::memory_order_relaxed), available,
      uint64_t(state_->credit_depth.load(std::memory_order_relaxed)) << 32 |
          (state_->paused.load(std::memory_order_relaxed) ? 1U : 0U));
}

uint32_t CoreAudioAudioDriver::RecommendedInitialCredits(uint32_t configured_maximum) const {
  if (!output_ || !state_) {
    return 1;
  }
  return std::min(state_->credit_depth.load(std::memory_order_acquire), configured_maximum);
}

void CoreAudioAudioDriver::Pause() {
  if (attached_ && output_) {
    output_->PauseClient(state_.get());
  }
  silent_clock_.Reset();
  silent_deadline_ = {};
}

void CoreAudioAudioDriver::Resume() {
  silent_clock_.Reset();
  silent_deadline_ = {};
  if (attached_ && output_) {
    output_->ResumeClient(state_.get());
  }
}

void CoreAudioAudioDriver::Flush() {
  if (attached_ && output_) {
    output_->FlushClient(state_.get(), false);
  }
}

void CoreAudioAudioDriver::SubmitSilent() {
  const auto now = std::chrono::steady_clock::now();
  if (silent_deadline_ == std::chrono::steady_clock::time_point{}) {
    silent_deadline_ = now;
  }
  silent_deadline_ += silent_clock_.NextDuration();
  if (silent_deadline_ < now) {
    silent_deadline_ = now;
    silent_clock_.Reset();
  }
  std::this_thread::sleep_until(silent_deadline_);
  if (semaphore_) {
    const bool released = semaphore_->Release(1, nullptr);
    if (!released) {
      REXAPU_ERROR("CoreAudio: paced-silent credit release failed");
    }
  }
}

}  // namespace rex::audio::coreaudio

#endif  // REX_PLATFORM_MAC && !REX_PLATFORM_IOS
