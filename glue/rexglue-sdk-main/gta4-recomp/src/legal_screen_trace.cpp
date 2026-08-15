#include <atomic>
#include <cstddef>
#include <cstdint>

#include <xxhash.h>

#include <rex/cvar.h>
#include <rex/logging.h>

#include "gta4_init.h"

REXCVAR_DEFINE_BOOL(gta4_trace_legal_screen, true, "GTA IV/Diagnostics",
                    "Trace legal-screen state and HUD text submission");

namespace {

// Retail control flow and string xrefs:
//   sub_82144800 -> LEGAL_360 / LEGAL_360_US
//   sub_82144800 -> sub_821F6E38 (HUD text submission)
constexpr uint32_t kLegalTextBuffer = 0x831E4DE0;
constexpr uint32_t kLegalSequenceGlobal = 0x831D5344;
constexpr uint32_t kLegalModeGlobal = 0x831D5349;
constexpr uint32_t kLegalReadyGlobal = 0x831D534B;
constexpr uint32_t kGlobalDevice = 0x831C22A4;
constexpr uint32_t kSubmittedFrameOffset = 16544;
constexpr uint32_t kRenderTargetBase = 3108;
constexpr size_t kMaximumLegalTextBytes = 4096;

thread_local uint32_t g_legal_trace_depth = 0;
thread_local uint32_t g_legal_text_submits = 0;
std::atomic<uint64_t> g_legal_calls{0};

uint8_t* GuestPointer(uint8_t* base, uint32_t address) {
  return base + address + REX_PHYS_HOST_OFFSET(address);
}

uint8_t LoadGuestU8(uint8_t* base, uint32_t address) {
  return *reinterpret_cast<volatile uint8_t*>(GuestPointer(base, address));
}

uint32_t LoadGuestU32(uint8_t* base, uint32_t address) {
  return __builtin_bswap32(
      *reinterpret_cast<volatile uint32_t*>(GuestPointer(base, address)));
}

struct GuestTextFingerprint {
  size_t length = 0;
  uint64_t hash = 0;
};

GuestTextFingerprint FingerprintGuestText(uint8_t* base, uint32_t address) {
  GuestTextFingerprint result;
  if (!base || !address) {
    return result;
  }
  while (result.length < kMaximumLegalTextBytes &&
         LoadGuestU8(base, address + uint32_t(result.length))) {
    ++result.length;
  }
  result.hash = XXH3_64bits(GuestPointer(base, address), result.length);
  return result;
}

uint32_t CurrentDevice(uint8_t* base) {
  return base ? LoadGuestU32(base, kGlobalDevice) : 0;
}

uint32_t CurrentFrame(uint8_t* base, uint32_t device) {
  return device ? LoadGuestU32(base, device + kSubmittedFrameOffset) : 0;
}

}  // namespace

extern "C" void sub_82144800(PPCContext& ctx, uint8_t* base) {
  if (!REXCVAR_GET(gta4_trace_legal_screen)) {
    __imp__sub_82144800(ctx, base);
    return;
  }

  const uint64_t call = g_legal_calls.fetch_add(1, std::memory_order_relaxed) + 1;
  const uint32_t device = CurrentDevice(base);
  const uint32_t frame = CurrentFrame(base, device);
  const uint32_t render_target =
      device ? LoadGuestU32(base, device + kRenderTargetBase) : 0;
  const uint32_t sequence = LoadGuestU32(base, kLegalSequenceGlobal);
  const uint8_t mode = LoadGuestU8(base, kLegalModeGlobal);
  const uint8_t ready = LoadGuestU8(base, kLegalReadyGlobal);

  g_legal_text_submits = 0;
  ++g_legal_trace_depth;
  REXLOG_WARN(
      "gta4-legal-trace: call={} phase=enter frame={} lr={:08X} device={:08X} "
      "rt0={:08X} sequence={} mode={} ready={}",
      call, frame, ctx.lr, device, render_target, sequence, mode, ready);

  __imp__sub_82144800(ctx, base);

  --g_legal_trace_depth;
  const GuestTextFingerprint text = FingerprintGuestText(base, kLegalTextBuffer);
  const uint32_t frame_after = CurrentFrame(base, device);
  REXLOG_WARN(
      "gta4-legal-trace: call={} phase=exit frame={}->{} text={:08X} "
      "text-bytes={} text-hash={:016X} hud-submits={}",
      call, frame, frame_after, kLegalTextBuffer, text.length, text.hash,
      g_legal_text_submits);
}

extern "C" void sub_821F6E38(PPCContext& ctx, uint8_t* base) {
  if (REXCVAR_GET(gta4_trace_legal_screen) && g_legal_trace_depth) {
    ++g_legal_text_submits;
    const uint32_t device = CurrentDevice(base);
    const GuestTextFingerprint text = FingerprintGuestText(base, ctx.r5.u32);
    REXLOG_WARN(
        "gta4-legal-trace: phase=hud-submit frame={} submit={} text={:08X} "
        "text-bytes={} text-hash={:016X} position={:.7g},{:.7g} "
        "color={:08X}/{:08X}",
        CurrentFrame(base, device), g_legal_text_submits, ctx.r5.u32, text.length,
        text.hash, ctx.f1.f64, ctx.f2.f64, ctx.r6.u32, ctx.r7.u32);
  }
  __imp__sub_821F6E38(ctx, base);
}
