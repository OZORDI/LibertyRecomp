#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace rex::diagnostics::gta4_transition {

enum class Mode : uint8_t {
  kOff,
  kMetadata,
  kPcm,
};

enum class EventSource : uint8_t {
  kCoordinator,
  kGuest,
  kAudio,
  kXma,
  kCoreAudio,
  kRenderer,
  kPresenter,
};

enum class EventType : uint16_t {
  kLoadingBegin,
  kLoadingTick,
  kLoadingStateChange,
  kLoadingEnd,
  kStateDispatchBegin,
  kStateDispatchEnd,
  kWorldActivationBegin,
  kLoadingTeardownBegin,
  kLoadingTeardownEnd,
  kWorldActivationEnd,
  kFirstIndexedDraw,
  kPresent,
  kTraceComplete,
  kTraceCancelled,
  kAudioWaitBegin,
  kAudioWaitEnd,
  kAudioCallbackBegin,
  kAudioCallbackEnd,
  kAudioSubmit,
  kAudioRingState,
  kAudioUnderrun,
  kAudioDeviceChange,
  kXmaKick,
  kXmaWorkerBegin,
  kXmaContextBegin,
  kXmaContextEnd,
  kXmaKickComplete,
  kXmaLock,
  kXmaClear,
  kFirstDrawState,
  kDrawMetadata,
  kVertexConstants,
  kVertexRange,
  kResourceGeneration,
  kCommandArenaAllocateBegin,
  kCommandArenaAllocateEnd,
  kCommandArenaConsumeBegin,
  kCommandArenaConsumeEnd,
  kPipelineMissBegin,
  kPipelineMissEnd,
  kFrameRecordBegin,
  kFrameRecordEnd,
  kQueueSubmitBegin,
  kQueueSubmitEnd,
  kPresentBegin,
  kPresentEnd,
  kCount,
};

enum EventFlags : uint8_t {
  kFlagNone = 0,
  kFlagPeriodic = 1 << 0,
  kFlagStateChanged = 1 << 1,
  kFlagBefore = 1 << 2,
  kFlagAfter = 1 << 3,
  kFlagError = 1 << 4,
};

// Binary trace ABI. Keep this trivially copyable and exactly 64 bytes so
// producers can publish a record without allocation, formatting, or locks.
struct alignas(8) TransitionEvent {
  uint64_t host_tick;
  uint64_t sequence;
  uint64_t transition_id;
  uint64_t value0;
  uint64_t value1;
  uint64_t value2;
  uint32_t guest_pc;
  uint32_t guest_lr;
  uint32_t submitted_frame;
  EventType type;
  EventSource source;
  uint8_t flags;
};

static_assert(sizeof(TransitionEvent) == 64);
static_assert(std::is_trivially_copyable_v<TransitionEvent>);

Mode GetMode();
bool IsEnabled();
uint64_t ActiveTransitionId();
void Initialize();

// Safe for hot and realtime producer paths. Returns false when capture is off,
// inactive, finalizing, or full. This function performs no allocation, file
// I/O, locking, formatting, or normal logging.
bool Record(EventSource source, EventType type, uint32_t guest_pc = 0,
            uint32_t guest_lr = 0, uint32_t submitted_frame = 0,
            uint8_t flags = kFlagNone, uint64_t value0 = 0,
            uint64_t value1 = 0, uint64_t value2 = 0);

// Samples high-rate steady-state events before the confirmed teardown while
// retaining full density at and after the world boundary. Errors and state
// changes should continue to use Record directly.
bool RecordSteady(EventSource source, EventType type, uint32_t guest_pc = 0,
                  uint32_t guest_lr = 0, uint32_t submitted_frame = 0,
                  uint8_t flags = kFlagNone, uint64_t value0 = 0,
                  uint64_t value1 = 0, uint64_t value2 = 0);

// Optional final-device PCM capture. The caller must pass native interleaved
// float output after mixing/muting. The realtime path only copies into the
// preallocated circular buffer and never performs file I/O.
void CapturePcmFloatInterleaved(const float* samples, uint32_t frame_count,
                                uint32_t channel_count,
                                uint32_t sample_rate);

// GTA IV boundary observations. LoadingTick begins the bounded trace while the
// retail loading controller is active. State changes are always retained;
// unchanged ticks are sampled periodically.
void NoteLoadingTick(uint32_t guest_pc, uint32_t guest_lr, bool loading_active,
                     uint32_t loading_state, uint64_t state_bits);
void NoteStateDispatch(bool entering, uint32_t guest_pc, uint32_t guest_lr,
                       uint32_t stored_state, uint64_t dispatch_flags = 0);
void NoteWorldActivationBegin(uint32_t guest_pc, uint32_t guest_lr,
                              uint64_t argument_bits);
void NoteLoadingTeardown(bool entering, uint32_t guest_pc, uint32_t guest_lr,
                         uint64_t argument_bits);
void NoteWorldActivationEnd(uint32_t guest_pc, uint32_t guest_lr,
                            uint64_t result_bits);
void NoteFirstIndexedDraw(uint32_t guest_pc, uint32_t guest_lr,
                          uint32_t submitted_frame, uint64_t draw_id);
void NotePresent(uint32_t submitted_frame, uint64_t present_id);

// Explicit shutdown hook for tests/tools. Runtime capture normally flushes
// asynchronously after the third Present following the first indexed draw.
void Shutdown();

}  // namespace rex::diagnostics::gta4_transition
