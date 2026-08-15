#include <cstdint>
#include <atomic>

#include <rex/chrono/clock.h>
#include <rex/diagnostics/gta4_transition.h>
#include <rex/thread.h>

#include "gta4_init.h"

namespace {

constexpr uint32_t kLoadingActiveGlobal = 0x831D5335;
constexpr uint32_t kLoadingReadyGlobal = 0x831D5336;
constexpr uint32_t kLoadingCompleteGlobal = 0x831D5337;
constexpr uint32_t kLoadingScreenIndexGlobal = 0x831D5340;
constexpr uint32_t kLoadingAudioGateGlobal = 0x831D5348;
constexpr uint32_t kFrontendStoredStateGlobal = 0x82BFA13C;
// Python-derived from `lis r11,-32076; addi r11,r11,-29864` in the generated
// sub_821BB3D8 body. Selector and cursor are fields +20 and +28.
constexpr uint32_t kCommandArenaStateGlobal = 0x82B38B58;
constexpr uint32_t kCommandArenaSelectorOffset = 20;
constexpr uint32_t kCommandArenaCursorOffset = 28;

thread_local uint32_t g_world_activation_depth = 0;
std::atomic<uint64_t> g_command_arena_generation{0};

uint64_t ReadLoadingStateBits(uint8_t* base) {
  uint64_t bits = REX_LOAD_U8(kLoadingReadyGlobal);
  bits |= uint64_t{REX_LOAD_U8(kLoadingCompleteGlobal)} << 8;
  bits |= uint64_t{REX_LOAD_U8(kLoadingAudioGateGlobal)} << 16;
  return bits;
}

}  // namespace

extern "C" void sub_82144188(PPCContext& ctx, uint8_t* base) {
  rex::diagnostics::gta4_transition::NoteLoadingTick(
      0x82144188, static_cast<uint32_t>(ctx.lr),
      REX_LOAD_U8(kLoadingActiveGlobal) != 0,
      REX_LOAD_U32(kLoadingScreenIndexGlobal), ReadLoadingStateBits(base));
  __imp__sub_82144188(ctx, base);
  rex::diagnostics::gta4_transition::NoteLoadingTick(
      0x82144188, static_cast<uint32_t>(ctx.lr),
      REX_LOAD_U8(kLoadingActiveGlobal) != 0,
      REX_LOAD_U32(kLoadingScreenIndexGlobal), ReadLoadingStateBits(base));
}

extern "C" void sub_8214B640(PPCContext& ctx, uint8_t* base) {
  const uint32_t caller = static_cast<uint32_t>(ctx.lr);
  const uint32_t stored_state = REX_LOAD_U32(kFrontendStoredStateGlobal);
  rex::diagnostics::gta4_transition::NoteStateDispatch(
      true, 0x8214B640, caller, stored_state);
  __imp__sub_8214B640(ctx, base);
  rex::diagnostics::gta4_transition::NoteStateDispatch(
      false, 0x8214B640, caller, REX_LOAD_U32(kFrontendStoredStateGlobal),
      stored_state);
}

extern "C" void sub_82141F00(PPCContext& ctx, uint8_t* base) {
  const uint32_t caller = static_cast<uint32_t>(ctx.lr);
  const uint64_t arguments = uint64_t{ctx.r3.u32} |
                             (uint64_t{ctx.r4.u32} << 32);
  rex::diagnostics::gta4_transition::NoteWorldActivationBegin(
      0x82141F00, caller, arguments);
  ++g_world_activation_depth;
  __imp__sub_82141F00(ctx, base);
  --g_world_activation_depth;
  rex::diagnostics::gta4_transition::NoteWorldActivationEnd(
      0x82141F00, caller, ctx.r3.u64);
}

extern "C" void sub_82145770(PPCContext& ctx, uint8_t* base) {
  const uint32_t caller = static_cast<uint32_t>(ctx.lr);
  const uint64_t argument = ctx.r3.u64;
  const bool is_world_entry_teardown =
      g_world_activation_depth != 0 && (ctx.r3.u32 & 0xFF) == 0;
  if (is_world_entry_teardown) {
    rex::diagnostics::gta4_transition::NoteLoadingTeardown(
        true, 0x82145770, caller, argument);
  }
  __imp__sub_82145770(ctx, base);
  if (is_world_entry_teardown) {
    rex::diagnostics::gta4_transition::NoteLoadingTeardown(
        false, 0x82145770, caller, argument);
  }
}

extern "C" void sub_821BB3D8(PPCContext& ctx, uint8_t* base) {
  const uint32_t requested_size = ctx.r3.u32;
  const uint32_t caller = static_cast<uint32_t>(ctx.lr);
  const uint32_t selector_before =
      REX_LOAD_U32(kCommandArenaStateGlobal + kCommandArenaSelectorOffset);
  const uint32_t cursor_before =
      REX_LOAD_U32(kCommandArenaStateGlobal + kCommandArenaCursorOffset);
  const uint32_t base_before = selector_before < 2
                                   ? REX_LOAD_U32(kCommandArenaStateGlobal +
                                                  selector_before * sizeof(uint32_t))
                                   : 0;
  const uint64_t begin_tick = rex::chrono::Clock::QueryHostTickCount();
  rex::diagnostics::gta4_transition::Record(
      rex::diagnostics::gta4_transition::EventSource::kGuest,
      rex::diagnostics::gta4_transition::EventType::kCommandArenaAllocateBegin,
      0x821BB3D8, caller, 0,
      rex::diagnostics::gta4_transition::kFlagBefore,
      (uint64_t{selector_before} << 32) | requested_size,
      (uint64_t{base_before} << 32) | cursor_before,
      g_command_arena_generation.load(std::memory_order_relaxed));

  __imp__sub_821BB3D8(ctx, base);

  const uint32_t returned_pointer = ctx.r3.u32;
  const uint32_t selector_after =
      REX_LOAD_U32(kCommandArenaStateGlobal + kCommandArenaSelectorOffset);
  const uint32_t cursor_after =
      REX_LOAD_U32(kCommandArenaStateGlobal + kCommandArenaCursorOffset);
  const bool wrapped = selector_after != selector_before || cursor_after < cursor_before;
  const uint64_t generation =
      wrapped ? g_command_arena_generation.fetch_add(1, std::memory_order_relaxed) + 1
              : g_command_arena_generation.load(std::memory_order_relaxed);
  const uint64_t elapsed =
      rex::chrono::Clock::QueryHostTickCount() - begin_tick;
  rex::diagnostics::gta4_transition::Record(
      rex::diagnostics::gta4_transition::EventSource::kGuest,
      rex::diagnostics::gta4_transition::EventType::kCommandArenaAllocateEnd,
      0x821BB3D8, caller, 0,
      wrapped ? rex::diagnostics::gta4_transition::kFlagStateChanged
              : rex::diagnostics::gta4_transition::kFlagAfter,
      (uint64_t{selector_after} << 32) | returned_pointer,
      (uint64_t{cursor_before} << 32) | cursor_after,
      (generation << 32) | uint32_t(elapsed));
}

extern "C" void sub_821BB2D0(PPCContext& ctx, uint8_t* base) {
  const uint32_t caller = static_cast<uint32_t>(ctx.lr);
  const uint32_t argument3 = ctx.r3.u32;
  const uint32_t argument4 = ctx.r4.u32;
  const uint32_t argument5 = ctx.r5.u32;
  const uint32_t argument6 = ctx.r6.u32;
  const uint64_t generation =
      g_command_arena_generation.load(std::memory_order_acquire);
  const uint64_t begin_tick = rex::chrono::Clock::QueryHostTickCount();
  rex::diagnostics::gta4_transition::Record(
      rex::diagnostics::gta4_transition::EventSource::kGuest,
      rex::diagnostics::gta4_transition::EventType::kCommandArenaConsumeBegin,
      0x821BB2D0, caller, 0,
      rex::diagnostics::gta4_transition::kFlagBefore,
      (uint64_t{argument3} << 32) | argument4,
      (uint64_t{argument5} << 32) | argument6,
      (generation << 32) | rex::thread::current_thread_system_id());

  __imp__sub_821BB2D0(ctx, base);

  const uint64_t elapsed =
      rex::chrono::Clock::QueryHostTickCount() - begin_tick;
  rex::diagnostics::gta4_transition::Record(
      rex::diagnostics::gta4_transition::EventSource::kGuest,
      rex::diagnostics::gta4_transition::EventType::kCommandArenaConsumeEnd,
      0x821BB2D0, caller, 0,
      rex::diagnostics::gta4_transition::kFlagAfter, ctx.r3.u32, elapsed,
      (g_command_arena_generation.load(std::memory_order_acquire) << 32) |
          rex::thread::current_thread_system_id());
}
