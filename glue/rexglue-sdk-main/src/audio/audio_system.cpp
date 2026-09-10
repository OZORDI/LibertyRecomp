/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2022 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 *
 * @modified    Tom Clay, 2026 - Adapted for ReXGlue runtime
 */

#include <algorithm>
#include <array>
#include <optional>
#include <rex/assert.h>
#include <rex/audio/audio_driver.h>
#include <rex/audio/audio_system.h>
#include <rex/audio/flags.h>
#include <rex/audio/xma/decoder.h>
#include <rex/dbg.h>
#include <rex/diagnostics/gta4_transition.h>
#include <rex/logging.h>
#include <rex/math.h>
#include <rex/memory/ring_buffer.h>
#include <rex/stream.h>
#include <rex/string/buffer.h>
#include <rex/system/thread_state.h>
#include <rex/thread.h>
#include <rex/cvar.h>

#if REX_PLATFORM_MAC && !REX_PLATFORM_IOS
#include <pthread/qos.h>
#endif

REXCVAR_DEFINE_INT32(
    audio_maxqframes, 64, "Audio",
    "Maximum buffered guest audio blocks (range 1-64). The backend selects the initial depth.");

// As with normal Microsoft, there are like twelve different ways to access
// the audio APIs. Early games use XMA*() methods almost exclusively to touch
// decoders. Later games use XAudio*() and direct memory writes to the XMA
// structures (as opposed to the XMA* calls), meaning that we have to support
// both.
//
// For ease of implementation, most audio related processing is handled in
// AudioSystem, and the functions here call off to it.
// The XMA*() functions just manipulate the audio system in the guest context
// and let the normal AudioSystem handling take it, to prevent duplicate
// implementations. They can be found in xboxkrnl_audio_xma.cc

#include "handoff_trace.inc"

namespace rex::audio {

AudioSystem::AudioSystem(runtime::FunctionDispatcher* function_dispatcher)
    : memory_(function_dispatcher->memory()),
      function_dispatcher_(function_dispatcher),
      worker_running_(false) {
  queued_frames_ = std::min(
      static_cast<uint32_t>(kMaximumQueuedFrames),
      std::max(static_cast<uint32_t>(REXCVAR_GET(audio_maxqframes)),
               static_cast<uint32_t>(1)));

  for (size_t i = 0; i < kMaximumClientCount; ++i) {
    client_semaphores_[i] = rex::thread::Semaphore::Create(0, queued_frames_);
    assert_not_null(client_semaphores_[i]);
    wait_handles_[i] = client_semaphores_[i].get();
  }
  shutdown_event_ = rex::thread::Event::CreateAutoResetEvent(false);
  assert_not_null(shutdown_event_);
  wait_handles_[kMaximumClientCount] = shutdown_event_.get();

  xma_decoder_ = std::make_unique<rex::audio::XmaDecoder>(function_dispatcher_);

  resume_event_ = rex::thread::Event::CreateAutoResetEvent(false);
  assert_not_null(resume_event_);
}

AudioSystem::~AudioSystem() {
  if (xma_decoder_) {
    xma_decoder_->Shutdown();
  }
  handoff::Shutdown();
}

X_STATUS AudioSystem::Setup(system::KernelState* kernel_state) {
  handoff::Initialize();
  X_STATUS result = xma_decoder_->Setup(kernel_state);
  if (result) {
    return result;
  }

  worker_running_ = true;
  worker_thread_ = system::object_ref<system::XHostThread>(
      new system::XHostThread(kernel_state, 128 * 1024, 0, [this]() {
        WorkerThreadMain();
        return 0;
      }));

  worker_thread_->set_name("Audio Worker");
  worker_thread_->Create();

  return X_STATUS_SUCCESS;
}

void AudioSystem::WorkerThreadMain() {
#if REX_PLATFORM_MAC && !REX_PLATFORM_IOS
  // The guest mixer callback is the producer for the native output ring. Give
  // it interactive QoS so graphics and code-generation load cannot starve
  // audio production long enough to drain the reliability buffer.
  pthread_set_qos_class_self_np(QOS_CLASS_USER_INTERACTIVE, 0);
#endif

  // Initialize driver and ringbuffer.
  Initialize();

  // Main run loop.
  uint32_t diag_pump_count = 0;
  while (worker_running_) {
    diagnostics::gta4_transition::RecordSteady(
        diagnostics::gta4_transition::EventSource::kAudio,
        diagnostics::gta4_transition::EventType::kAudioWaitBegin);
    const uint64_t handoff_wait_begin = handoff::Enabled() ? handoff::Clock() : 0;
    auto result = rex::thread::WaitAny(
        wait_handles_, rex::countof(wait_handles_), true,
        std::chrono::milliseconds(500));
    diagnostics::gta4_transition::RecordSteady(
        diagnostics::gta4_transition::EventSource::kAudio,
        diagnostics::gta4_transition::EventType::kAudioWaitEnd, 0, 0, 0,
        result.first == rex::thread::WaitResult::kFailed
            ? diagnostics::gta4_transition::kFlagError
            : diagnostics::gta4_transition::kFlagNone,
        static_cast<uint64_t>(result.first), result.second);
    if (handoff_wait_begin) handoff::Record("audio-wait", result.second, {handoff::Clock()-handoff_wait_begin, uint64_t(result.first)});
    if (result.first == rex::thread::WaitResult::kFailed) {
      REXAPU_WARN("AudioWorker: WaitAny failed");
      continue;
    }

    if (result.first == rex::thread::WaitResult::kTimeout) {
      if (diag_pump_count < 5) {
        REXAPU_NOISY_DEBUG(
            "AudioWorker: WaitAny timed out (no semaphore signals)");
      }
    }

    if (result.first == thread::WaitResult::kSuccess && result.second == kMaximumClientCount) {
      if (paused_) {
        pause_fence_.Signal();
        thread::Wait(resume_event_.get(), false);
      }
      continue;
    }

    bool pumped = false;
    if (result.first == rex::thread::WaitResult::kSuccess) {
      auto index = result.second;

      std::shared_ptr<ClientDispatchState> dispatch;
      auto global_lock = global_critical_region_.Acquire();
      dispatch = clients_[index].dispatch;
      global_lock.unlock();

      // UnregisterClient removes the dispatch state from the table first, then
      // takes this lock exclusively. A state copied just before removal either
      // finishes its callback before unregistration returns, or observes
      // accepting=false and does not enter guest code. This closes the window
      // where a copied callback argument could outlive the guest audio engine.
      std::optional<std::shared_lock<std::shared_mutex>> dispatch_lock;
      if (dispatch) {
        dispatch_lock.emplace(dispatch->mutex);
      }
      const uint32_t client_callback =
          dispatch && dispatch->accepting ? dispatch->callback : 0;
      const uint32_t client_callback_arg =
          dispatch && dispatch->accepting ? dispatch->wrapped_callback_arg : 0;

      if (client_callback) {
        if (diag_pump_count < 10) {
          REXAPU_DEBUG("AudioWorker: dispatching callback {:08X} with arg {:08X} for client {}",
                       client_callback, client_callback_arg, index);
        }
        SCOPE_profile_cpu_i("apu", "rex::audio::AudioSystem->client_callback");
        diagnostics::gta4_transition::RecordSteady(
            diagnostics::gta4_transition::EventSource::kAudio,
            diagnostics::gta4_transition::EventType::kAudioCallbackBegin,
            client_callback, 0, 0,
            diagnostics::gta4_transition::kFlagBefore, index, 0,
            client_callback_arg);
        handoff::Span handoff_callback("guest-mixer", index, client_callback, client_callback_arg);
        uint64_t args[] = {client_callback_arg};
        function_dispatcher_->Execute(worker_thread_->thread_state(), client_callback, args,
                                      rex::countof(args));
        diagnostics::gta4_transition::RecordSteady(
            diagnostics::gta4_transition::EventSource::kAudio,
            diagnostics::gta4_transition::EventType::kAudioCallbackEnd,
            client_callback, 0, 0,
            diagnostics::gta4_transition::kFlagAfter, index, 0,
            client_callback_arg);
        if (diag_pump_count < 10) {
          REXAPU_DEBUG("AudioWorker: callback returned for client {}", index);
        }
        diag_pump_count++;
      } else {
        REXAPU_DEBUG("AudioWorker: semaphore signaled for client {} but callback is 0", index);
      }
      pumped = true;
    }

    if (!worker_running_) {
      break;
    }

    if (!pumped) {
      SCOPE_profile_cpu_i("apu", "Sleep");
      rex::thread::Sleep(std::chrono::milliseconds(500));
    }
  }
  worker_running_ = false;

  // TODO(benvanik): call module API to kill?
}

int AudioSystem::FindFreeClient() {
  for (size_t i = 0; i < kMaximumClientCount; i++) {
    auto& client = clients_[i];
    if (!client.in_use) {
      return i;
    }
  }

  return -1;
}

void AudioSystem::Initialize() {}

void AudioSystem::Shutdown() {
  if (!worker_running_) {
    return;
  }

  // Shut down XMA decoder first - its worker can stall in FFmpeg
  if (xma_decoder_) {
    xma_decoder_->Shutdown();
  }

  worker_running_ = false;
  shutdown_event_->Set();
  if (worker_thread_) {
    // The worker may be stuck inside a guest callback that is itself blocked
    // on guest objects (e.g. KeWaitForMultipleObjects).
    // Terminate the thread to break the deadlock.
    worker_thread_->Terminate(0);
    worker_thread_.reset();
  }

  // Destroy all active client drivers before the semaphores they reference.
  // Move the strong references out so backend shutdown never runs under the
  // global client-table lock.
  std::array<std::shared_ptr<AudioDriver>, kMaximumClientCount> drivers;
  for (size_t i = 0; i < kMaximumClientCount; i++) {
    if (clients_[i].in_use) {
      drivers[i] = std::move(clients_[i].driver);
      if (clients_[i].wrapped_callback_arg) {
        memory()->SystemHeapFree(clients_[i].wrapped_callback_arg);
      }
      clients_[i] = {};
    }
  }
  for (auto& driver : drivers) {
    driver.reset();
  }
}

X_STATUS AudioSystem::RegisterClient(uint32_t callback, uint32_t callback_arg, size_t* out_index) {
  handoff::Record("audio-register", callback, {callback_arg});
  REXAPU_DEBUG("AudioSystem::RegisterClient: callback={:08X} callback_arg={:08X}", callback,
               callback_arg);
  auto global_lock = global_critical_region_.Acquire();

  auto index = FindFreeClient();
  assert_true(index >= 0);
  REXAPU_DEBUG("AudioSystem::RegisterClient: using client index={} queued_frames={}", index,
               queued_frames_);

  auto* client_semaphore = client_semaphores_[index].get();
  AudioDriver* raw_driver = nullptr;
  auto result = CreateDriver(index, client_semaphore, &raw_driver);
  if (XFAILED(result)) {
    return result;
  }
  assert_not_null(raw_driver);
  std::shared_ptr<AudioDriver> driver(raw_driver,
                                      [this](AudioDriver* value) { DestroyDriver(value); });

  uint32_t ptr = memory()->SystemHeapAlloc(0x4);
  memory::store_and_swap<uint32_t>(memory()->TranslateVirtual(ptr), callback_arg);

  clients_[index] = {
      .driver = driver,
      .dispatch = std::make_shared<ClientDispatchState>(callback, ptr),
      .callback = callback,
      .callback_arg = callback_arg,
      .wrapped_callback_arg = ptr,
      .in_use = true,
  };

  const uint32_t initial_credits =
      std::clamp(driver->RecommendedInitialCredits(queued_frames_), 1U, queued_frames_);
  auto ret = client_semaphore->Release(initial_credits, nullptr);
  assert_true(ret);
  REXAPU_DEBUG("AudioSystem::RegisterClient: client {} initialized with {} producer credits", index,
               initial_credits);

  if (out_index) {
    *out_index = index;
  }

  return X_STATUS_SUCCESS;
}

void AudioSystem::SubmitFrame(size_t index, uint32_t samples_ptr) {
  SCOPE_profile_cpu_f("apu");

  static uint32_t submit_count = 0;
  if (submit_count < 10) {
    REXAPU_DEBUG("AudioSystem::SubmitFrame called: index={} samples_ptr={:08X}", index,
                 samples_ptr);
    submit_count++;
  }

  std::shared_ptr<AudioDriver> driver;
  {
    auto global_lock = global_critical_region_.Acquire();
    assert_true(index < kMaximumClientCount);
    driver = clients_[index].driver;
  }
  if (!driver) {
    REXAPU_WARN("AudioSystem::SubmitFrame ignored for inactive client {}", index);
    return;
  }
  diagnostics::gta4_transition::Record(
      diagnostics::gta4_transition::EventSource::kAudio,
      diagnostics::gta4_transition::EventType::kAudioSubmit, 0, 0, 0,
      diagnostics::gta4_transition::kFlagBefore, index, samples_ptr);
  driver->SubmitFrame(samples_ptr);
}

void AudioSystem::UnregisterClient(size_t index) {
  handoff::Span handoff_unregister("audio-unregister", index);
  SCOPE_profile_cpu_f("apu");

  assert_true(index < kMaximumClientCount);
  std::shared_ptr<AudioDriver> driver;
  std::shared_ptr<ClientDispatchState> dispatch;
  uint32_t wrapped_callback_arg = 0;
  {
    auto global_lock = global_critical_region_.Acquire();
    driver = std::move(clients_[index].driver);
    dispatch = std::move(clients_[index].dispatch);
    wrapped_callback_arg = clients_[index].wrapped_callback_arg;
    clients_[index] = {};
  }

  // Do not allow guest teardown to continue until a callback that already
  // left the client-table lock has returned. The callback may submit audio,
  // so waiting under global_critical_region_ would deadlock.
  if (dispatch) {
    std::unique_lock dispatch_lock(dispatch->mutex);
    dispatch->accepting = false;
  }
  driver.reset();
  if (wrapped_callback_arg) {
    memory()->SystemHeapFree(wrapped_callback_arg);
  }

  // Drain the semaphore of its count.
  auto client_semaphore = client_semaphores_[index].get();
  rex::thread::WaitResult wait_result;
  do {
    wait_result = rex::thread::Wait(client_semaphore, false, std::chrono::milliseconds(0));
  } while (wait_result == rex::thread::WaitResult::kSuccess);
  assert_true(wait_result == rex::thread::WaitResult::kTimeout);
}

bool AudioSystem::Save(stream::ByteStream* stream) {
  stream->Write(kAudioSaveSignature);

  // Count the number of used clients first.
  // Any gaps should be handled gracefully.
  uint32_t used_clients = 0;
  for (size_t i = 0; i < kMaximumClientCount; i++) {
    if (clients_[i].in_use) {
      used_clients++;
    }
  }

  stream->Write(used_clients);
  for (uint32_t i = 0; i < kMaximumClientCount; i++) {
    auto& client = clients_[i];
    if (!client.in_use) {
      continue;
    }

    stream->Write(i);
    stream->Write(client.callback);
    stream->Write(client.callback_arg);
    stream->Write(client.wrapped_callback_arg);
  }

  return true;
}

bool AudioSystem::Restore(stream::ByteStream* stream) {
  if (stream->Read<uint32_t>() != kAudioSaveSignature) {
    REXAPU_ERROR("AudioSystem::Restore - Invalid magic value!");
    return false;
  }

  uint32_t num_clients = stream->Read<uint32_t>();
  for (uint32_t i = 0; i < num_clients; i++) {
    auto id = stream->Read<uint32_t>();
    assert_true(id < kMaximumClientCount);

    auto& client = clients_[id];

    // Reset the semaphore and recreate the driver ourselves.
    if (client.driver) {
      UnregisterClient(id);
    }

    client.callback = stream->Read<uint32_t>();
    client.callback_arg = stream->Read<uint32_t>();
    client.wrapped_callback_arg = stream->Read<uint32_t>();
    client.dispatch = std::make_shared<ClientDispatchState>(client.callback,
                                                            client.wrapped_callback_arg);

    client.in_use = true;

    auto* client_semaphore = client_semaphores_[id].get();
    AudioDriver* raw_driver = nullptr;
    auto status = CreateDriver(id, client_semaphore, &raw_driver);
    if (XFAILED(status)) {
      REXAPU_ERROR(
          "AudioSystem::Restore - Call to CreateDriver failed with status "
          "{:08X}",
          status);
      return false;
    }

    assert_not_null(raw_driver);
    client.driver = std::shared_ptr<AudioDriver>(
        raw_driver, [this](AudioDriver* value) { DestroyDriver(value); });
    const uint32_t initial_credits =
        std::clamp(client.driver->RecommendedInitialCredits(queued_frames_), 1U, queued_frames_);
    auto ret = client_semaphore->Release(initial_credits, nullptr);
    assert_true(ret);
  }

  return true;
}

void AudioSystem::Pause() {
  handoff::Record("audio-pause");
  if (paused_) {
    return;
  }
  paused_ = true;

  // Kind of a hack, but it works.
  shutdown_event_->Set();
  pause_fence_.Wait();

  std::array<std::shared_ptr<AudioDriver>, kMaximumClientCount> drivers;
  {
    auto global_lock = global_critical_region_.Acquire();
    for (size_t i = 0; i < kMaximumClientCount; ++i) {
      drivers[i] = clients_[i].driver;
    }
  }
  for (auto& driver : drivers) {
    if (driver) {
      driver->Pause();
    }
  }
  for (size_t i = 0; i < kMaximumClientCount; ++i) {
    while (rex::thread::Wait(client_semaphores_[i].get(), false, std::chrono::milliseconds(0)) ==
           rex::thread::WaitResult::kSuccess) {}
  }

  xma_decoder_->Pause();
}

void AudioSystem::Resume() {
  handoff::Record("audio-resume");
  if (!paused_) {
    return;
  }
  xma_decoder_->Resume();

  std::array<std::shared_ptr<AudioDriver>, kMaximumClientCount> drivers;
  {
    auto global_lock = global_critical_region_.Acquire();
    for (size_t i = 0; i < kMaximumClientCount; ++i) {
      drivers[i] = clients_[i].driver;
    }
  }
  for (size_t i = 0; i < kMaximumClientCount; ++i) {
    if (!drivers[i]) {
      continue;
    }
    drivers[i]->Resume();
    const uint32_t initial_credits =
        std::clamp(drivers[i]->RecommendedInitialCredits(queued_frames_), 1U, queued_frames_);
    const bool released = client_semaphores_[i]->Release(initial_credits, nullptr);
    assert_true(released);
  }

  paused_ = false;
  resume_event_->Set();
}

}  // namespace rex::audio
