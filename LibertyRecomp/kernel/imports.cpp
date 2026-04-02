// =============================================================================
// imports.cpp - Liberty hooks for RexGlue runtime
// =============================================================================
// Architecture: RexGlue handles ALL Xbox 360 kernel exports.
// Liberty hooks ONLY: Video/GPU, Audio, Input, Networking, Voice, Sessions,
// Profile (multiplayer), and a few essential GPU game-function hooks.
// =============================================================================

#include "function.h"
#include "heap.h"
#include "memory.h"
#include "user_profile.h"
#include "xam.h"
#include "xbox.h"
#include "xdm.h"
#include "xex.h"
#include <SDL.h>
#ifdef __APPLE__
#include <CommonCrypto/CommonCryptor.h>
#endif
#include <apu/audio.h>
#include <atomic>
#include <cpu/guest_thread.h>
#include <cpu/ppc_context.h>
#include <cstdint>
#include <cstdio>
#include <gpu/video.h>
#include <hid/hid.h>
#include <memory>
#include <mutex>
#include <os/logger.h>
#include <stdafx.h>
#include <thread>
#include <ui/game_window.h>
#include <unordered_map>
#include <unordered_set>
#include <user/config.h>
#include <user/paths.h>

#include "game_init.h"
#include "io/net_session.h"
#include "io/net_socket.h"
#include "io/voice_chat.h"
#include "menu_hooks.h"

// RexGlue Integration
#include "../runtime/rex_adapter.h"
#include <rex/kernel/kernel_state.h>
#include <rex/kernel/xevent.h>
#include <rex/kernel/xsemaphore.h>
#include <rex/kernel/xobject.h>
#include <rex/kernel/util/object_table.h>
#include <rex/kernel/xmemory.h>
#include <rex/system/xthread.h>
#include <rex/system/processor.h>

// Forward declarations
static void SignalEventByGuestAddr(uint32_t guestAddr);
static void SignalSemaphoreByGuestAddr(uint32_t guestAddr, int32_t count = 1);

// =============================================================================
// KERNEL PHASE SYSTEM
// =============================================================================
enum class KernelPhase { Boot, Init, Runtime };
std::atomic<KernelPhase> g_kernelPhase{KernelPhase::Boot};

inline bool ShouldFailOpenWait() {
  return g_kernelPhase.load(std::memory_order_acquire) != KernelPhase::Runtime;
}

void KernelPhase_EnterInit() {
  auto expected = KernelPhase::Boot;
  if (g_kernelPhase.compare_exchange_strong(expected, KernelPhase::Init)) {
    LOG_WARNING("[KERNEL_PHASE] Boot -> Init");
  }
}

void KernelPhase_EnterRuntime() {
  auto phase = g_kernelPhase.load();
  if (phase != KernelPhase::Runtime) {
    g_kernelPhase.store(KernelPhase::Runtime, std::memory_order_release);
    LOGF_WARNING("[KERNEL_PHASE] {} -> Runtime",
                 phase == KernelPhase::Boot ? "Boot" : "Init");
    /* g_headless_wait_cap_enabled removed in SDK v0.2.1 */
    printf("[KERNEL_PHASE] Disabled headless wait cap (GPU active)\n");
  }
}

// Global device context address (shared across threads)
static std::atomic<uint32_t> g_guestDeviceAddr{0};
extern "C" uint32_t GetGuestDeviceAddr() { return g_guestDeviceAddr.load(std::memory_order_acquire); }
extern "C" void SetGuestDeviceAddr(uint32_t addr) { g_guestDeviceAddr.store(addr, std::memory_order_release); }

// PHASE 2: Global flag for storage init tracing
static std::atomic<bool> g_inStorageInit{false};

// =============================================================================
// SDL EVENT PUMPING
// =============================================================================
static std::chrono::steady_clock::time_point g_lastSdlPumpTime;
static constexpr auto SDL_PUMP_INTERVAL = std::chrono::milliseconds(16);
static std::thread::id g_kernelMainThreadId;
static std::atomic<bool> g_kernelMainThreadIdSet{false};

void InitKernelMainThread() {
  g_kernelMainThreadId = std::this_thread::get_id();
  g_kernelMainThreadIdSet = true;
}

bool IsMainThread() {
  if (!g_kernelMainThreadIdSet) return false;
  return std::this_thread::get_id() == g_kernelMainThreadId;
}

void PumpSdlEventsIfNeeded() {
  if (!IsMainThread()) return;
  auto now = std::chrono::steady_clock::now();
  if (now - g_lastSdlPumpTime >= SDL_PUMP_INTERVAL) {
    g_lastSdlPumpTime = now;
    SDL_PumpEvents();
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) { std::_Exit(0); }
    }
  }
}

// =============================================================================
// REXGLUE SYNC SIGNALING HELPERS
// =============================================================================
static void SignalEventByGuestAddr(uint32_t guestAddr) {
  auto* ks = rex::system::kernel_state();
  if (!ks) return;
  void* ptr = g_memory.Translate(guestAddr);
  if (!ptr) return;
  auto ev = rex::system::XObject::GetNativeObject<rex::system::XEvent>(ks, ptr);
  if (ev) { ev->Set(0, false); }
}

static void SignalSemaphoreByGuestAddr(uint32_t guestAddr, int32_t count) {
  auto* ks = rex::system::kernel_state();
  if (!ks) return;
  void* ptr = g_memory.Translate(guestAddr);
  if (!ptr) return;
  auto sem = rex::system::XObject::GetNativeObject<rex::system::XSemaphore>(ks, ptr);
  if (sem) { sem->ReleaseSemaphore(count); }
}

// =============================================================================
// GPU RING BUFFER STATE
// =============================================================================
struct GpuRingBufferState {
  uint32_t ringBufferBase = 0;
  uint32_t ringBufferSize = 0;
  uint32_t readPtrWritebackAddr = 0;
  uint32_t blockSize = 0;
  uint32_t interruptCallback = 0;
  uint32_t interruptUserData = 0;
  bool initialized = false;
  bool writebackEnabled = false;
  bool interruptFired = false;
  bool enginesInitialized = false;
  bool edramTrainingComplete = false;
  bool interruptSeen = false;
  uint32_t lastKnownWritePtr = 0;
  uint32_t processedReadPtr = 0;
  uint32_t pm4DrawCount = 0;
  uint32_t pm4ShaderLoadCount = 0;
  uint32_t pm4SetConstantCount = 0;
  uint32_t pm4SwapCount = 0;
  uint32_t pm4OtherCount = 0;
};
static GpuRingBufferState g_gpuRingBuffer;

// Read GPU context from global variable
static uint32_t ReadGpuContextFromGlobal() {
  constexpr uint32_t GOT_ENTRY_GUEST = 0x82000768;
  if (!g_memory.base) return 0;
  uint32_t *gotEntryHost = reinterpret_cast<uint32_t *>(g_memory.base + GOT_ENTRY_GUEST);
  uint32_t globalAddr = __builtin_bswap32(*gotEntryHost);
  if (globalAddr == 0 || globalAddr >= 0xF0000000) return 0;
  uint32_t *globalHost = reinterpret_cast<uint32_t *>(g_memory.base + globalAddr);
  uint32_t gpuContext = __builtin_bswap32(*globalHost);
  return gpuContext;
}

// =============================================================================
// VIDEO / GPU FUNCTIONS (Liberty's Metal/Vulkan rendering pipeline)
// =============================================================================

void VdSwap() {
  KernelPhase_EnterRuntime();
  static uint32_t s_frameCount = 0;
  if (s_frameCount < 10 || (s_frameCount % 60 == 0 && s_frameCount < 600)) {
    LOGF_UTILITY("VdSwap frame {} - presenting!", s_frameCount);
  }
  ++s_frameCount;

  if (IsMainThread()) {
    SDL_PumpEvents();
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
    GameWindow::Update();
  }
  Video::Present();

  // Signal audio/IO events each frame (simulates Xbox hardware interrupts)
  SignalEventByGuestAddr(0x83137B80);   // Audio worker event
  SignalSemaphoreByGuestAddr(0x83130008); // Audio work queue semaphore
  SignalEventByGuestAddr(0x82A9800C);   // File I/O events
  SignalEventByGuestAddr(0x82AA0010);
  SignalEventByGuestAddr(0x82AA0014);
  SignalEventByGuestAddr(0x82AA0018);
  SignalEventByGuestAddr(0x83131E10);   // Streaming I/O
  SignalEventByGuestAddr(0x83131B34);   // XamTask completion
  SignalEventByGuestAddr(0x82000768);   // GPU sync
}

void VdGetSystemCommandBuffer() { LOG_UTILITY("!!! STUB !!!"); }

void VdEnableRingBufferRPtrWriteBack(uint32_t writebackAddr, uint32_t blockSizeLog2) {
  LOG_UTILITY("!!! STUB !!!");
}

void VdInitializeRingBuffer(uint32_t physAddr, uint32_t sizeLog2) {
  LOG_UTILITY("!!! STUB !!!");
}

uint32_t MmGetPhysicalAddress(uint32_t address) {
  LOGF_UTILITY("0x{:x}", address);
  return address;
}

void VdSetSystemCommandBufferGpuIdentifierAddress() { LOG_UTILITY("!!! STUB !!!"); }

void VdShutdownEngines() { LOG_UTILITY("!!! STUB !!!"); }

void VdQueryVideoMode(XVIDEO_MODE *vm) {
  memset(vm, 0, sizeof(XVIDEO_MODE));
  vm->DisplayWidth = 1280;
  vm->DisplayHeight = 720;
  vm->IsInterlaced = false;
  vm->IsWidescreen = true;
  vm->IsHighDefinition = true;
  vm->RefreshRate = 0x42700000;
  vm->VideoStandard = 1;
  vm->Unknown4A = 0x4A;
  vm->Unknown01 = 0x01;
}

void VdGetCurrentDisplayInformation() { LOG_UTILITY("!!! STUB !!!"); }
void VdSetDisplayMode() { LOG_UTILITY("!!! STUB !!!"); }

// =============================================================================
// VBLANK TIMER — uses RexGlue XHostThread for proper kernel context
// =============================================================================
// The VBlank callback dispatches into PPC code that uses kernel primitives
// (critical sections, events, semaphores). This REQUIRES a proper XHostThread
// with TLS, kernel state, and CPU context — a bare std::thread will crash
// because the PPC code tries to use RexGlue threading infrastructure that
// doesn't exist on a raw thread.
// =============================================================================
static std::atomic<bool> g_vblankThreadRunning{false};
static rex::system::object_ref<rex::system::XHostThread> g_vblankThread;

void StartVBlankTimer() {
  if (g_vblankThreadRunning) return;

  auto* ks = rex::system::kernel_state();
  if (!ks || !ks->processor()) {
    printf("[StartVBlankTimer] ERROR: kernel_state or processor not ready\n");
    return;
  }

  g_vblankThreadRunning = true;
  printf("[StartVBlankTimer] Starting VBlank XHostThread...\n");

  g_vblankThread = rex::system::object_ref<rex::system::XHostThread>(
      new rex::system::XHostThread(ks, 128 * 1024, 0, [ks]() {
        using namespace std::chrono;
        auto nextVBlank = steady_clock::now();
        constexpr auto VBLANK_INTERVAL = nanoseconds(16666667); // 60Hz
        uint32_t vblankCount = 0;

        while (g_vblankThreadRunning) {
          nextVBlank += VBLANK_INTERVAL;
          std::this_thread::sleep_until(nextVBlank);
          ++vblankCount;

          // Set frame-ready flag in guest memory
          constexpr uint32_t GUEST_FRAME_READY_FLAG = 0x83128A80;
          if (g_memory.base) {
            *(g_memory.base + GUEST_FRAME_READY_FLAG) = 1;
          }

          // Dispatch interrupt callback through RexGlue's processor
          // This properly saves/restores TLS, acquires the global interrupt
          // lock, and sets up r3/r4 for the PPC callback — exactly like
          // GraphicsSystem::DispatchInterruptCallback() does.
          if (g_gpuRingBuffer.interruptCallback != 0) {
            auto* thread = rex::system::XThread::GetCurrentThread();
            if (thread) {
              thread->SetActiveCpu(2); // GPU interrupts go to CPU 2

              // r3=0: VBlank/frame-done path (sub_82A46098 increments
              //   FrameSubmitted counter and clears frame-done fence).
              // r3=1: CPU interrupt/error path — NOT what we want.
              uint64_t args[] = {0, g_gpuRingBuffer.interruptUserData};
              ks->processor()->ExecuteInterrupt(
                  thread->thread_state(),
                  g_gpuRingBuffer.interruptCallback,
                  args, 2);
            }
          }
        }
        return 0;
      }));

  g_vblankThread->set_name("GPU VSync");
  g_vblankThread->Create();
}

void StopVBlankTimer() {
  if (g_vblankThreadRunning) {
    g_vblankThreadRunning = false;
    // XHostThread is ref-counted, will clean up when released
  }
}

void VdSetGraphicsInterruptCallback(uint32_t callback, uint32_t userData) {
  g_gpuRingBuffer.interruptCallback = callback;
  g_gpuRingBuffer.interruptUserData = userData;
  StartVBlankTimer();
  printf("[VdSetGraphicsInterruptCallback] callback=0x%08X userData=0x%08X\n", callback, userData);
}

// GPU MMIO read callback — minimal register emulation
// Matches RexGlue's GraphicsSystem::ReadRegister for critical registers.
// Without this, PPC_MM_LOAD_U32(0x7FC86544) returns 0 and the VBlank
// dispatch in sub_829D7368 never calls sub_829D4C48, starving all worker threads.
static uint32_t GpuMmioRead(void* /*ppc_ctx*/, void* /*ctx*/, uint32_t addr) {
    uint32_t reg = (addr & 0xFFFF) / 4;
    switch (reg) {
        case 0x1951: return 1;          // interrupt status — vblank active
        case 0x0F00: return 0x08100748; // RB_EDRAM_TIMING
        case 0x0F01: return 0x0000200E; // RB_BC_CONTROL
        case 0x194C: return 0x000002D0; // D1MODE_V_COUNTER
        case 0x1961: return 0x050002D0; // AVIVO_D1MODE_VIEWPORT_SIZE (1280x720)
        default:     return 0;
    }
}

static void GpuMmioWrite(void* /*ppc_ctx*/, void* /*ctx*/, uint32_t /*addr*/, uint32_t /*val*/) {
    // Ignore GPU register writes for now
}

uint32_t VdInitializeEngines() {
    // Register GPU MMIO range so PPC_MM_LOAD_U32 reads return valid values.
    // Without this, the VBlank interrupt status register (0x7FC86544) reads 0
    // and the VBlank dispatch never fires, causing all worker threads to hang.
    auto* ks = rex::system::kernel_state();
    if (ks && ks->memory()) {
        ks->memory()->AddVirtualMappedRange(
            0x7FC80000, 0xFFFF0000, 0x10000,
            nullptr, GpuMmioRead, GpuMmioWrite);
        printf("[VdInitializeEngines] GPU MMIO range 0x7FC80000 registered\n");
    } else {
        printf("[VdInitializeEngines] WARNING: Could not register GPU MMIO range\n");
    }
    return 1;
}
uint32_t VdIsHSIOTrainingSucceeded() { return 1; }
void VdGetCurrentDisplayGamma() { LOG_UTILITY("!!! STUB !!!"); }
void VdQueryVideoFlags() { LOG_UTILITY("!!! STUB !!!"); }
void VdCallGraphicsNotificationRoutines() { LOG_UTILITY("!!! STUB !!!"); }
void VdInitializeScalerCommandBuffer() { LOG_UTILITY("!!! STUB !!!"); }

uint32_t VdRetrainEDRAM() {
  g_gpuRingBuffer.edramTrainingComplete = true;
  return 0;
}

uint32_t VdRetrainEDRAMWorker(uint32_t unk0) {
  g_gpuRingBuffer.edramTrainingComplete = true;
  return 0;
}

void VdEnableDisableClockGating() { LOG_UTILITY("!!! STUB !!!"); }
uint32_t VdGetGpuMemoryUsage() { return 0; }
void VdSetGpuMemoryMode() { LOG_UTILITY("!!! STUB !!!"); }
void VdGetSystemCommandBuffer2() { LOG_UTILITY("!!! STUB !!!"); }
void VdSetSystemCommandBuffer2() { LOG_UTILITY("!!! STUB !!!"); }
void VdGetDisplayInformation() { LOG_UTILITY("!!! STUB !!!"); }
void VdSetDisplayConfiguration() { LOG_UTILITY("!!! STUB !!!"); }
uint32_t VdPerformHardwareTest() { return 1; }
uint32_t VdGetHardwareStatus() { return 1; }
void VdSetOverlayMode() { LOG_UTILITY("!!! STUB !!!"); }
void VdGetOverlayInformation() { LOG_UTILITY("!!! STUB !!!"); }

// Networking stub (referenced by GUEST_FUNCTION_HOOK but impl in net_socket.cpp)
void NetDll___WSAFDIsSet() { LOG_UTILITY("!!! STUB !!!"); }

// =============================================================================
// AUDIO — handled by RexGlue SDL AudioSystem (hooks removed, RexGlue owns these)
// =============================================================================
// XAudioRegisterRenderDriverClient, XAudioSubmitRenderDriverFrame, etc. are
// exported by RexGlue's XBOXKRNL layer with full implementations.
//
// XMACreateContext / XMAReleaseContext: on macOS, xboxkrnl_audio_xma.cpp is
// compiled out (REX_NO_XMA_DECODER=1) because FFmpeg XMA support is Apple-only.
// The generated PPCFuncMappings still references these __imp__ symbols, so we
// provide minimal forwarding stubs here that satisfy the linker.  The game's
// XMA path would be a no-op on macOS anyway without a real XMA decoder.
extern "C" void __imp__XMACreateContext(PPCContext& ctx, uint8_t* base) {
    // macOS: XMA decoder not available — return null context pointer.
    if (ctx.r3.u32 != 0) {
        PPC_STORE_U32(ctx.r3.u32, 0);
    }
    ctx.r3.u32 = 0x8007000E; // STATUS_NOT_IMPLEMENTED
}
extern "C" void __imp__XMAReleaseContext(PPCContext& ctx, uint8_t* base) {
    // macOS: no-op release.
    ctx.r3.u32 = 0;
}

// =============================================================================
// VOICE CHAT (routes to VoiceChatManager)
// =============================================================================
uint32_t XamVoiceCreate(uint32_t userIndex, uint32_t flags, void **voice) {
  return Net::VoiceChatManager::Instance().CreateVoiceChannel(userIndex, flags, voice);
}

void XamVoiceClose(void *voice) {
  Net::VoiceChatManager::Instance().CloseVoiceChannel(voice);
}

uint32_t XamVoiceHeadsetPresent(uint32_t userIndex) {
  auto &voiceChat = Net::VoiceChatManager::Instance();
  if (!voiceChat.IsInitialized()) voiceChat.Initialize();
  return voiceChat.IsHeadsetPresent() ? 1 : 0;
}

uint32_t XamVoiceSubmitPacket(void *voice, uint32_t size, void *data) {
  return Net::VoiceChatManager::Instance().SubmitVoicePacket(voice, size, data);
}

// =============================================================================
// INPUT (XamInputGetKeystrokeEx — SDL keystroke queue)
// =============================================================================
uint32_t XamInputGetKeystrokeEx(uint32_t userIndex, uint32_t flags, void *keystroke) {
  if (userIndex >= 4) return ERROR_DEVICE_NOT_CONNECTED;
  hid::KeystrokeEvent event;
  if (!hid::DequeueKeystroke(userIndex, event)) return ERROR_EMPTY;
  if (keystroke) {
    XINPUT_KEYSTROKE *pKeystroke = reinterpret_cast<XINPUT_KEYSTROKE *>(keystroke);
    pKeystroke->VirtualKey = event.virtualKey;
    pKeystroke->Unicode = event.unicode;
    pKeystroke->Flags = event.flags;
    pKeystroke->UserIndex = event.userIndex;
    pKeystroke->HidCode = 0;
  }
  return ERROR_SUCCESS;
}

// VdPersistDisplay — same as VdSwap for our purposes
void VdPersistDisplay() { VdSwap(); }

// =============================================================================
// PROFILE (needed for multiplayer)
// =============================================================================
uint32_t XamUserReadProfileSettings(uint32_t titleId, uint32_t userIndex,
                                    uint32_t xuidCount, uint64_t *xuids,
                                    uint32_t settingCount, uint32_t *settingIds,
                                    be<uint32_t> *bufferSize, void *buffer,
                                    void *overlapped) {
  constexpr uint32_t ERROR_INSUFFICIENT_BUFFER = 122;
  constexpr uint32_t HEADER_SIZE = 8;
  constexpr uint32_t SETTING_SIZE = 40;

  if (settingCount < 1 || settingCount > 32) return 0x80070057;
  if (!bufferSize) return 0x80070057;

  uint32_t neededSize = HEADER_SIZE + (SETTING_SIZE * settingCount) + 256;
  if (!buffer || *bufferSize < neededSize) {
    if (*bufferSize == 0) *bufferSize = neededSize;
    return ERROR_INSUFFICIENT_BUFFER;
  }

  auto &profile = Liberty::GetUserProfile();
  memset(buffer, 0, *bufferSize);
  uint8_t *bufPtr = reinterpret_cast<uint8_t *>(buffer);
  *reinterpret_cast<be<uint32_t> *>(bufPtr + 0) = settingCount;
  *reinterpret_cast<be<uint32_t> *>(bufPtr + 4) = HEADER_SIZE;

  uint8_t *settingPtr = bufPtr + HEADER_SIZE;
  for (uint32_t i = 0; i < settingCount; ++i) {
    uint32_t settingId = settingIds[i];
    auto *setting = profile.getSetting(settingId);
    if (setting && setting->isSet) {
      *reinterpret_cast<be<uint32_t> *>(settingPtr + 0) = setting->isTitleSpecific() ? 2 : 1;
    }
    *reinterpret_cast<be<uint32_t> *>(settingPtr + 4) = 0;
    if (xuidCount > 0 && xuids) {
      *reinterpret_cast<be<uint64_t> *>(settingPtr + 8) = profile.xuid();
    } else {
      *reinterpret_cast<be<uint32_t> *>(settingPtr + 8) = userIndex;
    }
    *reinterpret_cast<be<uint32_t> *>(settingPtr + 16) = settingId;
    *reinterpret_cast<be<uint32_t> *>(settingPtr + 20) = 0;
    if (setting && setting->isSet) {
      settingPtr[24] = static_cast<uint8_t>(setting->type);
      switch (setting->type) {
      case Liberty::ProfileSettingType::Int32:
        *reinterpret_cast<be<int32_t> *>(settingPtr + 32) = static_cast<Liberty::Int32Setting *>(setting)->value;
        break;
      case Liberty::ProfileSettingType::Float:
        *reinterpret_cast<be<float> *>(settingPtr + 32) = static_cast<Liberty::FloatSetting *>(setting)->value;
        break;
      case Liberty::ProfileSettingType::Int64:
        *reinterpret_cast<be<int64_t> *>(settingPtr + 32) = static_cast<Liberty::Int64Setting *>(setting)->value;
        break;
      default: break;
      }
    }
    settingPtr += SETTING_SIZE;
  }

  if (overlapped) {
    XXOVERLAPPED *pOverlapped = reinterpret_cast<XXOVERLAPPED *>(overlapped);
    pOverlapped->dwCompletionContext = GuestThread::GetCurrentThreadId();
    pOverlapped->Error = 0;
    pOverlapped->Length = neededSize;
  }
  return 0;
}

// =============================================================================
// GPU GAME-FUNCTION HOOKS (essential for PC — not old patches)
// =============================================================================

// sub_829DD978 - GPU Command Buffer drain (no Xbox GPU hardware)
PPC_FUNC_HOOK(sub_82A4EDC8) {
  uint32_t ctx_addr = ctx.r3.u32;
  uint32_t write_ptr = PPC_LOAD_U32(ctx_addr + 56);
  PPC_STORE_U32(ctx_addr + 60, write_ptr);
  ctx.r3.u32 = 0;
}

// sub_829D72A0 - GPU Atomic Sync (no Xbox GPU hardware)
PPC_FUNC_HOOK(sub_82A486F0) { /* r3 already contains result */ }

// sub_829D87E8 - GPU Sync bypass (no Xbox GPU spin loop)
PPC_FUNC_HOOK(sub_82A49C38) {
  uint32_t deviceCtx = ctx.r3.u32;
  if (deviceCtx != 0) { PPC_STORE_U32(deviceCtx + 11000, 0); }
}

// sub_829CFED0 - GPU fence completion (force done after tight loop)
extern "C" void __imp__sub_82A41320(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_82A41320) {
  static int s_consecutiveCalls = 0;
  ++s_consecutiveCalls;
  if (s_consecutiveCalls > 50) {
    s_consecutiveCalls = 0;
    ctx.r3.u32 = 0;
    return;
  }
  __imp__sub_82A41320(ctx, base);
  if (ctx.r3.u32 == 0) s_consecutiveCalls = 0;
}

// sub_828507F8 - Frame presentation (fix throttle check)
extern "C" void __imp__sub_828507F8(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_828507F8) {
  // Do NOT zero 0x83124CCC here. That wiped the submitted-frame counter before the
  // pacing check, causing submitted(0) - presented(N) < 2 to never trigger and
  // blocking VdSwap after frame 2. The sync is now done in sub_82A467D8 (video.cpp).
  __imp__sub_828507F8(ctx, base);
}

// sub_829A0678 - HDCP bypass (PC has no HDCP)
extern "C" void __imp__sub_829A0678(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_829A0678) { ctx.r3.u32 = 0; }

// sub_828C9980 - grcEffect::SetTextureSlot
// Accounts for ~31% of MISSING-FUNC hits (89,952). The original function does a
// virtual call via vtable[13] on the current texture object to get its underlying
// resource pointer for an identity check ("is new texture same as current?").
// That vtable dispatch targets a function not in the recomp table, triggering
// MISSING-FUNC on every call. Fix: skip the vtable identity check entirely and
// always take the "different texture" path. This causes redundant texture rebinds
// but is correct — the identity check was only a GPU state-change optimization
// on Xbox 360 hardware. The net effect is a large performance win: eliminating
// 89K fprintf+fflush calls far outweighs redundant emulated-GPU texture binds.
extern "C" void sub_82147AA8(PPCContext &ctx, uint8_t *base);
extern "C" void sub_828E0F88(PPCContext &ctx, uint8_t *base);
extern "C" void sub_828C97E0(PPCContext &ctx, uint8_t *base);

PPC_FUNC_HOOK(sub_828C9980) {
  // Args: r3=this(effect), r4=slotArray, r5=slotIndex, r6=newTexture
  uint32_t effect     = ctx.r3.u32;
  uint32_t slotArray  = ctx.r4.u32;
  int32_t  slotIndex  = ctx.r5.s32;
  uint32_t newTexture = ctx.r6.u32;

  if (slotIndex == 0) {
    // Clear global flag and return (matches epilogue at loc_828C9A48/loc_828C9A54)
    PPC_STORE_U32(0x831C3DE4, 0);
    return;
  }

  uint32_t adjustedIdx = (uint32_t)(slotIndex - 1);
  uint32_t slotArrayBase = PPC_LOAD_U32(slotArray);
  uint32_t offset = adjustedIdx * 4;
  uint32_t currentTex = PPC_LOAD_U32(slotArrayBase + offset);

  if (currentTex == 0) {
    // Slot is empty — jump to assignment check (loc_828C9A0C path)
    goto check_assign;
  }

  // --- ORIGINAL CODE DOES VTABLE[13] DISPATCH HERE FOR IDENTITY CHECK ---
  // We skip it entirely: always assume textures are different (never early-out).
  // This eliminates the MISSING-FUNC from the unresolved virtual call.

  {
    // Reload current texture from slot (mirrors lwz r11,0(r29); lwzx r3,r11,r30)
    uint32_t curTex = PPC_LOAD_U32(PPC_LOAD_U32(slotArray) + offset);
    uint8_t texType = PPC_LOAD_U8(curTex + 8);
    if (texType == 2) {
      // Special path: sub_828E0F88(currentTexture, newTexture)
      ctx.r3.u32 = curTex;
      ctx.r4.u32 = newTexture;
      sub_828E0F88(ctx, base);
      goto finalize;
    }
    // Normal unbind: sub_82147AA8(currentTexture)
    ctx.r3.u32 = curTex;
    sub_82147AA8(ctx, base);
    // Store new texture in slot
    PPC_STORE_U32(PPC_LOAD_U32(slotArray) + offset, newTexture);
    if (newTexture == 0)
      goto finalize;
    goto inc_refcount;
  }

check_assign:
  {
    // loc_828C9A0C: slot was empty or identity matched (we only reach here if empty)
    uint32_t existing = PPC_LOAD_U32(PPC_LOAD_U32(slotArray) + offset);
    if (existing != 0) goto epilogue;  // slot occupied, same texture
    if (newTexture == 0) goto epilogue; // both null
    // Assign new texture to empty slot
    PPC_STORE_U32(PPC_LOAD_U32(slotArray) + offset, newTexture);
  }

inc_refcount:
  {
    // Increment refcount: *(uint16_t*)(newTexture + 10) += 1
    uint16_t refCount = PPC_LOAD_U16(newTexture + 10);
    PPC_STORE_U16(newTexture + 10, (uint16_t)(refCount + 1));
  }

finalize:
  // Tail call: sub_828C97E0(effect, slotArray, newTexture, adjustedIdx)
  ctx.r3.u32 = effect;
  ctx.r4.u32 = slotArray;
  ctx.r5.u32 = newTexture;
  ctx.r6.u32 = adjustedIdx;
  sub_828C97E0(ctx, base);

epilogue:
  // Clear global flag at 0x831C3DE4
  PPC_STORE_U32(0x831C3DE4, 0);
}

// sub_82A46098 - VBlank frame completion dispatch
// Increments FrameSubmitted (device+16532) and FrameCompleted (device+16552)
// counters, timestamps frames, and dispatches the frame-done callback
// (device+16528). The indirect callback signals the frame sync event that
// sub_821B5890 waits on — without it, the game thread blocks forever after
// frame 5. The one MMIO write (0x7FC86110) is safely handled by GpuMmioWrite.
extern "C" void __imp__sub_82A46098(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_82A46098) { __imp__sub_82A46098(ctx, base); }

// sub_829D7368 - VBlank callback (skip Xbox GPU fence queue)
extern "C" void __imp__sub_82A487B8(PPCContext &ctx, uint8_t *base);
//
// The VBlank callback reads device[+10900] and dereferences *(device[+10900]+16)
// to find the frame_done callback. If device[+10900]==0, this is a null deref.
// The recomp code gracefully handles frame_done==0 (skips call) but runs the
// spinlock-clear at loc_82A48810 which signals the main game thread.
//
// FIX: When device[+10900]==0, allocate a zero-filled 32-byte guest stub so the
// recomp code can safely read *(stub+16)=0 (frame_done=null, gracefully skipped)
// and still execute the spinlock interrupt-bit clear. This unblocks the game thread.
//
static uint32_t s_vblankStubAddr = 0;  // guest addr of the zero-filled stub

PPC_FUNC_HOOK(sub_82A487B8) {
  // Ensure device[+10900] is non-zero so the recomp code doesn't crash
  uint32_t userData = ctx.r4.u32;
  if (userData >= 0x10000 && userData < 0x83300000 && base) {
    uint32_t slot_val = __builtin_bswap32(
        *reinterpret_cast<const uint32_t*>(base + userData + 10900));
    if (slot_val == 0) {
      // Allocate a zero-filled stub once
      if (s_vblankStubAddr == 0) {
        auto* ks = rex::system::kernel_state();
        auto* mem = ks ? ks->memory() : nullptr;
        if (mem) {
          s_vblankStubAddr = mem->SystemHeapAlloc(64);
          if (s_vblankStubAddr) {
            memset(base + s_vblankStubAddr, 0, 64);
            printf("[VBlank-FIX] Allocated stub at guest 0x%08X for device[+10900]\n",
                   s_vblankStubAddr);
            fflush(stdout);
          }
        }
      }
      if (s_vblankStubAddr != 0) {
        // Write the stub address into device[+10900] (big-endian)
        *reinterpret_cast<uint32_t*>(base + userData + 10900) =
            __builtin_bswap32(s_vblankStubAddr);
      }
    }
  }
  // Let the recomp code run — it will find frame_done=0 and skip the call
  // but still execute the spinlock operations at loc_82A48810
  __imp__sub_82A487B8(ctx, base);
}

// sub_82871180 - GPU render state submission (accesses D3D device context at
// dword_83124AF4 which contains uncommitted GPU command buffer pointers in the
// 0x70xxxxxx range, causing SIGBUS). Pure hardware-bound Xbox 360 D3D code.
PPC_FUNC_HOOK(sub_82871180) { ctx.r3.u32 = 0; }

// sub_8284CFD8 — Streaming ring-buffer worker pool init
//
// This function loops over 2 streaming workers, allocating ring-buffer structs
// at unk_8319F2F8 + i*24944.  Each struct has a semaphore handle slot at +24940
// that workers block on via NtWaitForSingleObjectEx.  In BSS the slot is zero,
// so the wait returns INVALID_HANDLE immediately — workers spin and never sleep,
// the ring-buffer drain loop never yields, and the main thread's completion
// event H_c is never released → deadlock in sub_8285D018 wait chain.
//
// Previous attempts patched inside the generated code and called sub_82849778
// (a RAGE utility, not NtCreateSemaphore) — producing PPC code addresses as
// "handles" which are silently wrong.
//
// Fix: override here, call the original __imp__ to let workers be created
// normally, then post-seed the semaphore handles via rex::system::XSemaphore
// C++ API — no guest call needed, guaranteed valid kernel object handles.
extern "C" void __imp__sub_8284CFD8(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_8284CFD8) {
    __imp__sub_8284CFD8(ctx, base);  // run original worker init

    constexpr uint32_t RINGBUF_BASE  = 0x8319F2F8;
    constexpr uint32_t WORKER_STRIDE = 24944;
    constexpr uint32_t SEMA_OFFSET   = 24940;
    constexpr uint32_t NUM_WORKERS   = 2;

    auto* ks = rex::system::kernel_state();
    if (!ks) {
        printf("[SEMA-SEED] ERROR: kernel_state() null\n");
        fflush(stdout);
        return;
    }

    for (uint32_t i = 0; i < NUM_WORKERS; i++) {
        uint32_t ringbuf_addr = RINGBUF_BASE + i * WORKER_STRIDE;

        uint32_t existing = PPC_LOAD_U32(ringbuf_addr + SEMA_OFFSET);
        // Valid kernel handles are 0xF8xxxxxx; anything else (code ptrs, BSS garbage) needs replacement
        if ((existing & 0xFF000000u) == 0xF8000000u) {
            printf("[SEMA-SEED] worker=%u already seeded handle=0x%08X\n", i, existing);
            fflush(stdout);
            continue;
        }

        auto sem = rex::system::object_ref<rex::system::XSemaphore>(
            new rex::system::XSemaphore(ks));
        if (!sem->Initialize(0, 0x7FFF)) {
            printf("[SEMA-SEED] ERROR: XSemaphore::Initialize failed worker=%u\n", i);
            fflush(stdout);
            continue;
        }

        uint32_t handle = sem->handle();
        PPC_STORE_U32(ringbuf_addr + SEMA_OFFSET, handle);
        printf("[SEMA-SEED] worker=%u  ringbuf=0x%08X+%u  handle=0x%08X  OK\n",
               i, ringbuf_addr, SEMA_OFFSET, handle);
        fflush(stdout);
    }
}

// sub_827DF248 - pgStreamer::Init — creates pgStreamer table and worker threads.
//
// Problem: pgStreamer worker threads (sub_827DE858) die immediately after
// creation. The worker structs at guest BSS (unk_83101BA0) have their
// semaphore/CS initialized by an earlier call, but a race condition exists:
// the worker starts running before its queue is fully set up, dequeues from
// a zeroed slot where v4[387]==0 (the shutdown sentinel), and exits its loop.
// With dead workers, the atomic refcount v8[3] on pgStreamer entries is never
// decremented, causing pgStreamer::Close (sub_827DF0A8) to busy-wait forever.
// This deadlocks the Main XThread during CGame::Initialise when loading
// "platform:/textures/fonts".
//
// Fix: Force synchronous streaming by setting dword_830F589C = 1 before
// calling the original init. In sync mode, sub_827DF248 skips worker thread
// creation, and all streaming work is processed inline on the calling thread
// via sub_827DE1C0, which correctly decrements v8[3].
extern "C" void __imp__sub_827DF248(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_827DF248) {
    // Force synchronous streaming mode — no worker threads
    constexpr uint32_t SYNC_FLAG_ADDR = 0x830F589C;
    *reinterpret_cast<be<uint32_t>*>(g_memory.Translate(SYNC_FLAG_ADDR)) = 1;

    static bool logged = false;
    if (!logged) {
        printf("[STREAMING] pgStreamer::Init — forced sync mode (dword_830F589C=1)\n");
        fflush(stdout);
        logged = true;
    }

    __imp__sub_827DF248(ctx, base);
}

// sub_829A2540 - NtSetEvent wrapper called by RPF streaming workers (sub_827EE568)
// after processing each work item. The completion event handle at work item
// offset 156 is never set by the producer (sub_827EEBD8 only writes 156 bytes
// into 160-byte slots), so it retains guest memory debug fill (0xCDCDCDCD).
// NtSetEvent(0xCDCDCDCD) returns STATUS_INVALID_HANDLE on every iteration,
// flooding the log via RtlSetLastNTError -> RtlNtStatusToDosError on 3 worker
// threads. Guard: skip when handle is clearly invalid.
extern "C" void __imp__sub_829A2540(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_829A2540) {
  uint32_t handle = ctx.r3.u32;
  if (handle == 0 || handle == 0xCDCDCDCD) {
    ctx.r3.u32 = 0;
    return;
  }
  __imp__sub_829A2540(ctx, base);
}

// =============================================================================
// AUDIO HOOKS
// =============================================================================

// sub_82168C08 - Audio init: signal events to unblock workers
extern "C" void __imp__sub_82168C08(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_82168C08) {
  __imp__sub_82168C08(ctx, base);
  SignalEventByGuestAddr(0x83130044);
  SignalEventByGuestAddr(0x83137B80);
  SignalSemaphoreByGuestAddr(0x83130008, 6);
}

// sub_82169B00 - Audio thread sync — REMOVED: was preventing audio worker
// threads from constructing sound objects. Without workers running, pool
// slots stay uninitialized with Xbox kernel addresses (0x85000000 etc.)
// in vtable fields, causing 543 MISSING-FUNCs in the audio mixer.
// Let rexglue's recompiled code handle audio threading.

// sub_82169400 - Audio worker thread — REMOVED: same reason as above.
// SDL handles audio output, but the game's audio object construction
// still needs these workers to initialize sound pool entries.

// =============================================================================
// RAGE ALLOCATOR FIX — Phases 2 & 3
//
// Root cause: Liberty stubs several shader/GPU init functions that participate
// in RAGE's push/pop allocator-swap protocol.  When a stubbed function skips
// setting TLS[1680] (target allocator) before a push, or skips a push that a
// later pop depends on, TLS[1676] (active allocator) gets zeroed — killing
// ALL subsequent allocations.
//
// Phase 2: Protective push/pop hooks — prevent TLS[1676] from being zeroed.
// Phase 3: Fallback allocator — if TLS[1676] is still 0, route through
//          RexGlue's SystemHeapAlloc so allocations never silently fail.
// =============================================================================
extern "C" void __imp__sub_8218BE28(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__sub_8218BE50(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__sub_827D85E0(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__sub_827D8620(PPCContext &ctx, uint8_t *base);

// Safe big-endian read from guest memory
static uint32_t ReadGuestU32(uint32_t guestAddr) {
    if (guestAddr == 0 || guestAddr >= 0xFFFF0000u) return 0;
    return *reinterpret_cast<be<uint32_t>*>(g_memory.Translate(guestAddr));
}

// Write a big-endian uint32 to guest memory
static void WriteGuestU32(uint32_t guestAddr, uint32_t value) {
    if (guestAddr == 0 || guestAddr >= 0xFFFF0000u) return;
    *reinterpret_cast<be<uint32_t>*>(g_memory.Translate(guestAddr)) = value;
}

// Read TLS[offset] for the current thread context
static uint32_t ReadTLS(uint32_t r13, uint32_t offset) {
    uint32_t tlsBase = ReadGuestU32(r13);
    if (tlsBase == 0) return 0;
    return ReadGuestU32(tlsBase + offset);
}

// Write TLS[offset] for the current thread context
static void WriteTLS(uint32_t r13, uint32_t offset, uint32_t value) {
    uint32_t tlsBase = ReadGuestU32(r13);
    if (tlsBase == 0) return;
    WriteGuestU32(tlsBase + offset, value);
}

static std::atomic<int> s_allocFallbackCount{0};
static std::atomic<int> s_pushGuardCount{0};
static std::atomic<int> s_popGuardCount{0};

// Validate that a TLS[1676] value points to a real RAGE allocator object.
// A valid allocator has a vtable pointer at offset 0 in the XEX code range.
static bool IsValidAllocator(uint32_t memMgr) {
    if (memMgr == 0) return false;
    uint32_t vtable = ReadGuestU32(memMgr);
    return (vtable >= 0x82000000u && vtable < 0x84000000u);
}

// ---------------------------------------------------------------------------
// Phase 2: Protective PUSH hook (sub_827D85E0)
//
// Original behavior: if cur != target, sets TLS[1676] = TLS[1680].
// Problem: if TLS[1680] is 0 (target not set by a stubbed function),
//          this writes 0 into TLS[1676], killing the allocator.
// Fix: if TLS[1680] == 0 AND cur != 0, skip the push entirely.
//      The allocator stays alive with its current value.
// ---------------------------------------------------------------------------
PPC_FUNC_HOOK(sub_827D85E0) {
    uint32_t r13 = ctx.r13.u32;
    uint32_t cur    = ReadTLS(r13, 1676);
    uint32_t target = ReadTLS(r13, 1680);

    if (!IsValidAllocator(target) && IsValidAllocator(cur)) {
        // Target allocator wasn't set (likely a stubbed function skipped it).
        // Don't let the push zero out the active allocator.
        // Pretend the push happened with cur == target (increment refcount).
        uint32_t refcnt = ReadTLS(r13, 1668);
        WriteTLS(r13, 1668, refcnt + 1);
        int n = s_pushGuardCount.fetch_add(1, std::memory_order_relaxed);
        if (n < 5) {
            printf("[PUSH GUARD] #%d blocked push with target=0, kept cur=0x%08X refcnt=%u->%u caller=0x%08X\n",
                   n, cur, refcnt, refcnt + 1, static_cast<uint32_t>(ctx.lr));
            fflush(stdout);
        }
        return;
    }

    __imp__sub_827D85E0(ctx, base);
}

// ---------------------------------------------------------------------------
// Phase 2: Protective POP hook (sub_827D8620)
//
// Original behavior: if refcount == 0, restores TLS[1676] from TLS[1672],
//                    then zeros TLS[1672].
// Problem: if TLS[1672] is 0 (a push was skipped by a stubbed function),
//          this writes 0 into TLS[1676], killing the allocator.
// Fix: if refcount == 0 AND TLS[1672] == 0 AND TLS[1676] != 0,
//      skip the pop entirely.  The allocator stays alive.
// ---------------------------------------------------------------------------
PPC_FUNC_HOOK(sub_827D8620) {
    uint32_t r13 = ctx.r13.u32;
    uint32_t refcnt = ReadTLS(r13, 1668);
    uint32_t saved  = ReadTLS(r13, 1672);
    uint32_t cur    = ReadTLS(r13, 1676);

    if (refcnt == 0 && saved == 0 && cur != 0) {
        // The saved allocator is 0 — restoring it would kill TLS[1676].
        // This means a matching push was skipped (Liberty stub).
        // Skip the pop to keep the allocator alive.
        int n = s_popGuardCount.fetch_add(1, std::memory_order_relaxed);
        if (n < 5) {
            printf("[POP GUARD] #%d blocked pop with saved=0, kept cur=0x%08X caller=0x%08X\n",
                   n, cur, static_cast<uint32_t>(ctx.lr));
            fflush(stdout);
        }
        return;
    }

    // If our push guard incremented refcount, the pop's refcount > 0 path
    // will simply decrement it and return — which is correct.
    __imp__sub_827D8620(ctx, base);
}

// ---------------------------------------------------------------------------
// Phase 3: Fallback allocator (sub_8218BE28)
//
// If TLS[1676] is 0 despite the Phase 2 guards (e.g. allocator was never
// initialized on this thread, or a code path we didn't anticipate zeroed it),
// route through RexGlue's SystemHeapAlloc so the allocation succeeds.
// ---------------------------------------------------------------------------
PPC_FUNC_HOOK(sub_8218BE28) {
    uint32_t r13 = ctx.r13.u32;
    uint32_t memMgr = ReadTLS(r13, 1676);

    if (!IsValidAllocator(memMgr)) {
        // Allocator chain is broken — use RexGlue's system heap as fallback
        uint32_t size = ctx.r3.u32;
        auto* ks = rex::system::kernel_state();
        auto* mem = ks ? ks->memory() : nullptr;
        if (mem) {
            uint32_t guest = mem->SystemHeapAlloc(size);
            ctx.r3.u32 = guest;
            int n = s_allocFallbackCount.fetch_add(1, std::memory_order_relaxed);
            if (n < 200 || (n & 0xFF) == 0) {
                printf("[ALLOC FALLBACK] #%d size=%u -> 0x%08X..0x%08X caller=0x%08X\n",
                       n, size, guest, guest + size, static_cast<uint32_t>(ctx.lr));
                fflush(stdout);
            }
        } else {
            printf("[ALLOC FALLBACK] CRITICAL: RexGlue memory not available, size=%u\n", size);
            fflush(stdout);
            ctx.r3.u32 = 0;
        }
        return;
    }

    // Normal path — allocator chain is healthy
    __imp__sub_8218BE28(ctx, base);
}

// ---------------------------------------------------------------------------
// Phase 3b: Fallback allocator (sub_8218BE50) — aligned variant
//
// Same as sub_8218BE28 above, but sub_8218BE50 takes an explicit alignment
// parameter in r4 instead of hardcoding 16.  The RPF2 TOC allocation
// (sub_827EF2F8) calls sub_8218BE50(tocSize, 128) and hangs when this
// returns 0 because dev+8 becomes NULL.
// ---------------------------------------------------------------------------
PPC_FUNC_HOOK(sub_8218BE50) {
    uint32_t r13 = ctx.r13.u32;
    uint32_t memMgr = ReadTLS(r13, 1676);

    if (!IsValidAllocator(memMgr)) {
        uint32_t size  = ctx.r3.u32;
        uint32_t align = ctx.r4.u32;
        if (align < 16) align = 16;
        // Round size up to alignment so the block satisfies the request
        uint32_t allocSize = (size + align - 1) & ~(align - 1);
        auto* ks = rex::system::kernel_state();
        auto* mem = ks ? ks->memory() : nullptr;
        if (mem) {
            uint32_t guest = mem->SystemHeapAlloc(allocSize);
            ctx.r3.u32 = guest;
            int n = s_allocFallbackCount.fetch_add(1, std::memory_order_relaxed);
            if (n < 200 || (n & 0xFF) == 0) {
                printf("[ALLOC FALLBACK] #%d size=%u align=%u -> 0x%08X..0x%08X caller=0x%08X\n",
                       n, size, align, guest, guest + allocSize,
                       static_cast<uint32_t>(ctx.lr));
                fflush(stdout);
            }
        } else {
            printf("[ALLOC FALLBACK] CRITICAL: RexGlue memory not available, size=%u align=%u\n", size, align);
            fflush(stdout);
            ctx.r3.u32 = 0;
        }
        return;
    }

    // Normal path — allocator chain is healthy
    __imp__sub_8218BE50(ctx, base);
}

// ---------------------------------------------------------------------------
// Phase 4: Guard sub_827DAE40 — direct TLS[1676/1680] writer
//
// This function copies a 44-byte struct from its parameter and writes
// input[8] directly to BOTH TLS[1676] and TLS[1680], bypassing push/pop.
// If the value is corrupt (e.g. 0xBEBEBEBE from RexGlue stack fill),
// clear it to 0 so the Phase 3 fallback handles allocations.
// ---------------------------------------------------------------------------
extern "C" void __imp__sub_8284C290(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_tlsGuardCount{0};

PPC_FUNC_HOOK(sub_8284C290) {
    __imp__sub_8284C290(ctx, base);
    uint32_t r13 = ctx.r13.u32;
    uint32_t val = ReadTLS(r13, 1676);
    if (val != 0 && !IsValidAllocator(val)) {
        WriteTLS(r13, 1676, 0);
        WriteTLS(r13, 1680, 0);
        int n = s_tlsGuardCount.fetch_add(1, std::memory_order_relaxed);
        if (n < 10) {
            printf("[ALLOC GUARD] sub_827DAE40 wrote corrupt TLS[1676]=0x%08X, cleared to 0 caller=0x%08X\n",
                   val, static_cast<uint32_t>(ctx.lr));
            fflush(stdout);
        }
    }
}

// =============================================================================
// DIAGNOSTIC: Double-free detector for heap free function (sub_82848B68)
//
// The game's heap free function.  Track recently-freed addresses and flag
// double-frees, which corrupt the free-list (next = self → infinite loop).
// =============================================================================
extern "C" void __imp__sub_82848B68(PPCContext &ctx, uint8_t *base);

static std::mutex s_freeLogMutex;
static std::unordered_set<uint32_t> s_freedAddrs;
static std::atomic<int> s_freeCount{0};
static std::atomic<int> s_doubleFreeCount{0};

PPC_FUNC_HOOK(sub_82848B68) {
    // r4 = pointer being freed (the user-data pointer, node = r4 - 16)
    uint32_t ptr = ctx.r4.u32;
    uint32_t node = ptr - 16;
    int n = s_freeCount.fetch_add(1, std::memory_order_relaxed);

    // Stack unwinding: sub_82848B68 ← sub_828494D8(112B) ← sub_82847618(128B) ← ???
    // sub_828494D8 LR at r1+104, sub_82847618 LR at r1+232
    uint32_t caller = static_cast<uint32_t>(ctx.lr);
    uint32_t outerCaller = 0;
    uint32_t outerCaller2 = 0;
    if (caller == 0x82849560) {
        outerCaller = ReadGuestU32(ctx.r1.u32 + 104);
        if (outerCaller == 0x828476EC) {
            outerCaller2 = ReadGuestU32(ctx.r1.u32 + 232);
        }
    }

    // Read first 8 bytes of object (potential next/prev pointers) BEFORE free
    uint32_t obj_word0 = ReadGuestU32(ptr);
    uint32_t obj_word1 = ReadGuestU32(ptr + 4);

    {
        std::lock_guard<std::mutex> lk(s_freeLogMutex);
        if (s_freedAddrs.count(ptr)) {
            int df = s_doubleFreeCount.fetch_add(1, std::memory_order_relaxed);
            if (df < 20) {
                printf("[DOUBLE-FREE] #%d ptr=0x%08X caller=0x%08X outer1=0x%08X outer2=0x%08X obj[0]=0x%08X obj[4]=0x%08X frees=%d\n",
                       df, ptr, caller, outerCaller, outerCaller2, obj_word0, obj_word1, n);
                fflush(stdout);
            } else if (df == 20) {
                printf("[DOUBLE-FREE] ... suppressing further output (20 logged). total_frees=%d\n", n);
                fflush(stdout);
            }
        }
        s_freedAddrs.insert(ptr);
    }

    // Log first 20 frees with full detail
    if (n < 20) {
        printf("[HEAP-FREE] #%d ptr=0x%08X heap=0x%08X caller=0x%08X outer1=0x%08X outer2=0x%08X obj[0]=0x%08X obj[4]=0x%08X\n",
               n, ptr, ctx.r3.u32, caller, outerCaller, outerCaller2, obj_word0, obj_word1);
        fflush(stdout);
    }

    __imp__sub_82848B68(ctx, base);
}

// =============================================================================
// DIAGNOSTIC: TLS[1684] debug flag checker
//
// TLS[1684] controls debug assertions/memset in the heap allocator.
// On retail Xbox 360 this is 0.  Log its value on the first allocation
// so we know if debug paths are active.
// =============================================================================
static std::atomic<bool> s_tls1684Checked{false};

extern "C" void __imp__sub_82848750(PPCContext &ctx, uint8_t *base);

PPC_FUNC_HOOK(sub_82848750) {
    if (!s_tls1684Checked.exchange(true)) {
        uint32_t r13 = ctx.r13.u32;
        uint32_t tls1684 = ReadTLS(r13, 1684);
        printf("[TLS-DEBUG] TLS[1684] = 0x%08X on first sub_82848750 call (0 = retail, nonzero = debug paths active)\n",
               tls1684);
        fflush(stdout);
    }
    __imp__sub_82848750(ctx, base);
}

// =============================================================================
// DIAGNOSTIC: sub_82A50890 — GPU CreateDevice (top-level GPU init)
// =============================================================================
// =============================================================================
// SetRenderState no-op stub — called by the device's function pointer table
// when sub_828E02E8 dispatches render state changes. On Xbox 360, the Xenos
// driver fills these slots with GPU register writers. In the recomp, we use
// this no-op so the dispatch doesn't hit NULL.
// Signature: (GuestDevice* device, uint32_t value)
// =============================================================================
static void SetRenderStateStub(PPCContext& ctx, uint8_t* base) {
    // No-op: silently discard render state changes.
    // TODO: Implement real state tracking like Unleashed's SetRenderState<>.
}

extern "C" void __imp__sub_82A50890(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_82A50890) {
    uint32_t deviceAddr = ctx.r3.u32;  // Save device address BEFORE __imp__ call
    printf("[GPU-CREATE] sub_82A50890 ENTER r3=0x%08X r4=0x%08X\n",
           deviceAddr, ctx.r4.u32);
    fflush(stdout);
    __imp__sub_82A50890(ctx, base);
    printf("[GPU-CREATE] sub_82A50890 EXIT  result=0x%08X\n", ctx.r3.u32);
    fflush(stdout);

    // =========================================================================
    // Populate render state function pointer table in the device struct
    // (Replicating Unleashed's CreateDevice approach)
    //
    // The dispatch function sub_828E02E8 reads:
    //   table_value = MEM_BE[0x82B0D8F8 + stateIdx * 4]
    //   device_ptr  = MEM_BE[0x831C22A4]
    //   func_ptr    = MEM_BE[device_ptr + table_value + 64]
    //   call func_ptr
    //
    // table_value ranges 0x46-0x6D, so func_ptr offsets are 0x86-0xAD.
    // =========================================================================
    if (deviceAddr != 0 && deviceAddr > 0x10000) {
        // Register the no-op stub at a synthetic guest address
        static bool s_populated = false;
        if (!s_populated) {
            s_populated = true;

            // Use an address past the end of code for our stub
            constexpr uint32_t STUB_GUEST_ADDR = 0x82A90000 - 4;
            g_memory.InsertFunction(STUB_GUEST_ADDR, SetRenderStateStub);

            // Write the stub address at each of the 18 unique byte offsets
            // that the index table at 0x82B0D8F8 points to.
            // The dispatch does: func_ptr = MEM_BE[device_ptr + table_val + 64]
            // where table_val is one of these 18 values.
            static const uint32_t kTableOffsets[] = {
                0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E,
                0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D
            };
            uint8_t* devicePtr = static_cast<uint8_t*>(g_memory.Translate(deviceAddr));
            for (uint32_t off : kTableOffsets) {
                // func_ptr location = device + off + 64
                uint32_t funcOff = off + 64;
                *reinterpret_cast<uint32_t*>(devicePtr + funcOff) = __builtin_bswap32(STUB_GUEST_ADDR);
            }

            printf("[GPU-CREATE] Populated render state function table at device+0x86-0xB0 "
                   "with stub 0x%08X\n", STUB_GUEST_ADDR);
            fflush(stdout);
        }
    }
}

// DIAGNOSTIC: sub_82A416B8 — D3D device setup (caller of sub_82A50890)
extern "C" void __imp__sub_82A416B8(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_82A416B8) {
    printf("[D3D-SETUP] sub_82A416B8 ENTER r3=0x%08X r4=0x%08X r5=0x%08X\n",
           ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
    fflush(stdout);
    __imp__sub_82A416B8(ctx, base);
    printf("[D3D-SETUP] sub_82A416B8 EXIT  result=0x%08X\n", ctx.r3.u32);
    fflush(stdout);
}

// =============================================================================
// DIAGNOSTIC: sub_82A49D08 — GPU ring buffer / device init
//   r3 = device ptr, r4 = config ptr (if 0 → cleanup path, skips allocations)
// =============================================================================
extern "C" void __imp__sub_82A49D08(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_82A49D08) {
    uint32_t device = ctx.r3.u32;
    uint32_t config = ctx.r4.u32;
    printf("[GPU-INIT] sub_82A49D08 ENTER device=0x%08X config=0x%08X\n",
           device, config);
    fflush(stdout);
    __imp__sub_82A49D08(ctx, base);
    printf("[GPU-INIT] sub_82A49D08 EXIT  result=0x%08X\n", ctx.r3.u32);
    // Read device+10896 and device+10900 after call
    if (device >= 0x10000 && device < 0x83300000 && base) {
        uint32_t val_10896 = __builtin_bswap32(
            *reinterpret_cast<const uint32_t*>(base + device + 10896));
        uint32_t val_10900 = __builtin_bswap32(
            *reinterpret_cast<const uint32_t*>(base + device + 10900));
        printf("[GPU-INIT]   device[+10896]=0x%08X  device[+10900]=0x%08X\n",
               val_10896, val_10900);
    }
    fflush(stdout);
}

// DIAGNOSTIC: sub_821B3608 — RAGE memory allocator dispatch
extern "C" void __imp__sub_821B3608(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_allocDispatchCount{0};
PPC_FUNC_HOOK(sub_821B3608) {
    uint32_t size  = ctx.r3.u32;
    uint32_t flags = ctx.r4.u32;
    int n = s_allocDispatchCount.fetch_add(1, std::memory_order_relaxed);
    if (n < 30) {
        printf("[ALLOC-DISPATCH] sub_821B3608 #%d size=0x%X flags=0x%08X\n",
               n, size, flags);
        fflush(stdout);
    }
    __imp__sub_821B3608(ctx, base);
    if (n < 30) {
        printf("[ALLOC-DISPATCH] sub_821B3608 #%d -> result=0x%08X\n",
               n, ctx.r3.u32);
        fflush(stdout);
    }
}

// =============================================================================
// RAGE HEAP ALLOCATOR HOOK — sub_82A10EB0 (v8)
//
// sub_821B3608 routes allocations with flags bit-31=1 through sub_82A10EB0.
// sub_82A10EB0 calls sub_82A18920 which calls XMemAlloc — a stub returning 0.
// This causes device[+10896] and device[+10900] to remain NULL, which prevents
// the frame_done vtable entry from ever being written, deadlocking the VBlank
// interrupt loop (sub_82A487B8 reads device[+10900]+16 = NULL = 0x00000000).
//
// Fix: intercept sub_82A10EB0 and route through RexGlue SystemHeapAlloc.
// Parameters: r3 = size, r4 = flags (bit-31 path = physical/GPU alloc)
// Returns:    r3 = guest pointer to allocated block (non-zero on success)
// =============================================================================
extern "C" void __imp__sub_82A10EB0(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_rageAllocCount{0};

PPC_FUNC_HOOK(sub_82A10EB0) {
    uint32_t size  = ctx.r3.u32;
    uint32_t flags = ctx.r4.u32;
    if (size == 0 || size >= 0x4000000u) {
        // Out-of-range — fall through to original (will likely fail too)
        __imp__sub_82A10EB0(ctx, base);
        return;
    }
    auto* ks = rex::system::kernel_state();
    auto* mem = ks ? ks->memory() : nullptr;
    if (mem) {
        uint32_t guest = mem->SystemHeapAlloc(size);
        ctx.r3.u32 = guest;
        int n = s_rageAllocCount.fetch_add(1, std::memory_order_relaxed);
        if (n < 50 || (n & 0xFF) == 0) {
            printf("[RAGE-HEAP] sub_82A10EB0 #%d size=0x%X flags=0x%08X -> 0x%08X\n",
                   n, size, flags, guest);
            fflush(stdout);
        }
    } else {
        printf("[RAGE-HEAP] CRITICAL: kernel_state() null, size=0x%X flags=0x%08X\n",
               size, flags);
        fflush(stdout);
        ctx.r3.u32 = 0;
    }
}

// =============================================================================
// GPU RING BUFFER SUBMIT + FENCE WAIT STUBS
//
// The game's RAGE renderer accumulates Xbox 360 ring buffer commands, then
// calls sub_8285D018 to "kick" the ring buffer and wait for GPU completion.
// sub_8285D018 -> sub_8285CF98 -> sub_8285CEA8 -> sub_8285C648 -> sub_828497D8
//   -> sub_82A13040 -> NtWaitForSingleObjectEx (BLOCKS FOREVER — no real GPU)
//
// Since we render via high-level D3D hooks (DrawPrimitive, SetVertexShader etc.
// intercepted in video.cpp), the ring buffer never has real commands to execute.
// We stub the entire submit+wait chain so the render loop can tick at full speed:
//
//   sub_8285D018 : "submit command buffer + wait" → skip entirely, return 0
//   sub_8285C648 : "GPU fence wait"               → return 1 (signaled)
//   sub_8285CF98 : "fence create + wait wrapper"  → return 1 (signaled)
//   sub_828497D8 : "NtWait dispatcher"             → return 1 (success)
// =============================================================================

// sub_827A9A20 — binary search in a resource dictionary (hash map).
// r3 = dict pointer (struct with +16=array, +20=count, +8=next),  r4 = key.
// 15+ callers load the dict pointer from global 0x831C2EF8 which is null when
// GPU init is stubbed.  First instruction reads r3+20 → SIGBUS on null.
// Fix: return 0 (not found) when dict is null.  This covers all callers at once.
extern "C" void __imp__sub_827A9A20(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_827A9A20_nullDict{0};
PPC_FUNC_HOOK(sub_827A9A20) {
    if (ctx.r3.u32 == 0) {
        int n = s_827A9A20_nullDict.fetch_add(1, std::memory_order_relaxed);
        if (n < 10) {
            printf("[GPU-STUB] sub_827A9A20 null dict lookup (key=0x%08X) #%d\n",
                   ctx.r4.u32, n);
            fflush(stdout);
        }
        ctx.r3.u32 = 0; // not found
        return;
    }
    __imp__sub_827A9A20(ctx, base);
}

extern "C" void __imp__sub_8285D018(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_gpuSubmitCount{0};
PPC_FUNC_HOOK(sub_8285D018) {
    // Ring buffer submit+wait: skip all GPU command submission and fence machinery.
    // High-level D3D hooks in video.cpp capture actual rendering.
    int n = s_gpuSubmitCount.fetch_add(1, std::memory_order_relaxed);
    if (n < 5 || (n & 0x3FF) == 0) {
        printf("[GPU-SUBMIT] sub_8285D018 #%d — stubbed (ring buffer skip)\n", n);
        fflush(stdout);
    }
    ctx.r3.u32 = 0; // fence value = 0 (already complete)
}

extern "C" void __imp__sub_8285C648(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_8285C648) {
    // GPU fence wait: immediately signal completion — no GPU hardware to wait on.
    ctx.r3.u32 = 1; // return 1 = fence signaled
}

extern "C" void __imp__sub_8285CF98(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_8285CF98) {
    // Fence create+wait wrapper: return 1 (success) without waiting.
    ctx.r3.u32 = 1;
}

extern "C" void __imp__sub_828497D8(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_828497D8) {
    // NtWait dispatcher: pass through to original so non-GPU waits still work.
    // Only the higher-level sub_8285D018/sub_8285C648 stubs short-circuit GPU fences.
    __imp__sub_828497D8(ctx, base);
}


// =============================================================================
// GPU SHADER COMPILER / DEVICE INIT STUBS
//
// sub_82852FB0 — "ShaderFinalise": called after each shader is registered.
//   Internally calls sub_8285B088 which dispatches through a GPU device vtable
//   slot 40 (some Xenos "compile/link shader" command). Without a real GPU this
//   vtable call routes into the PM4 command buffer and blocks forever.
//   Since we handle shaders via our own cache (CreateShaderFromBytecode), the
//   game-side finalise step is a no-op for us — just return r3=1 (success).
//
// sub_82852B78 — "ShaderBind": binds a shader name → shader object pair.
//   Also invokes sub_8285B088 after sub_82852A50. Same rationale: no-op OK.
//   Returns the bound shader object ptr in r3; callers check for null.
//   We return 1 (truthy/non-null) so callers proceed.
//
// sub_82299500 — "sub_82299500 renderer subsystem init" — calls sub_82902AF8 →
//   sub_829029A0 → sub_82920060 → sub_8291DC80 → sub_82852FB0/B78 chain above.
//   The entire subsystem init is the Xenos GPU renderer pipeline setup.
//   We stub it to return 1 (success) — our high-level D3D hooks in video.cpp
//   already handle all actual rendering.
// =============================================================================

extern "C" void __imp__sub_8285B088(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_B088_count{0};
PPC_FUNC_HOOK(sub_8285B088) {
    // "Consume pending shader registration request."
    // r3 = shader-slot struct { ptr@+0, status@+4, cond1@+16(unused), cond2@+20 }
    //
    // Original flow:
    //   1. if (*(r3+20)==0 && *(r3+16)!=0) → call sub_8285A8B0 (GPU buffer flush — HANGS)
    //   2. vtable slot 10 → sub_8285F428 → sub_8285EC98 (shader slot registration, pure memory)
    //   3. stw -1 → slot+4  (mark consumed)
    //   4. stw  0 → slot+0  (clear ptr)
    //
    // We let the original run BUT sub_8285A8B0 is now stubbed (see below) so the GPU
    // buffer flush is skipped. The vtable slot 10 path is pure memory and runs fine.
    int n = s_B088_count.fetch_add(1, std::memory_order_relaxed) + 1;
    if (n <= 20 || (n % 100) == 0) {
        printf("[DIAG-B088] sub_8285B088 ENTER #%d r3=0x%08X caller=0x%08X\n", n, ctx.r3.u32, (uint32_t)ctx.lr);
        fflush(stdout);
    }
    __imp__sub_8285B088(ctx, base);
    if (n <= 20 || (n % 100) == 0) {
        printf("[DIAG-B088] sub_8285B088 RETURN #%d\n", n);
        fflush(stdout);
    }
}

// sub_8285A8B0 — GPU buffer flush called from sub_8285B088.
// Dispatches two GPU device vtable calls (slot 9 and slot 13) to "submit shader bytecode
// to the Xenos command ring". Both spin-wait on GPU fence completion → hang forever.
// Since we bypass the Xenos GPU entirely, this flush is a no-op for us.
extern "C" void __imp__sub_8285A8B0(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_8285A8B0) {
    // Skip GPU shader bytecode submission — no Xenos hardware to submit to.
    // The caller (sub_8285B088) will still do the slot cleanup writes afterwards.
}

extern "C" void __imp__sub_82852B78(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_82852B78) {
    // ShaderBind(context, shaderNamePtr, flags) — resolves name to shader object.
    // We let the original run (it does cache lookup + file path resolution which
    // are pure host memory ops). Only sub_8285B088 inside it is the GPU blocker,
    // and that's now stubbed above — so the original completes without blocking.
    __imp__sub_82852B78(ctx, base);
}

// =============================================================================
// POOL STRIDE CRASH DIAGNOSTICS
//
// sub_822054F8 crashes at twllei r9,0 — pool+12 (stride) is 0 despite
// sub_825EE000 storing 16 there.  These hooks trace every allocation and
// field value to find if addresses overlap or memory is corrupted.
// =============================================================================
extern "C" void __imp__sub_822054F8(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__sub_825EE000(PPCContext &ctx, uint8_t *base);

// Pool pointer global address used by sub_822054F8
static constexpr uint32_t POOL_PTR_GLOBAL = 0x82B9C1B0;

PPC_FUNC_HOOK(sub_825EE000) {
    // r3=pool_ptr, r4=count, r5=name_str, r6=stride
    uint32_t poolPtr  = ctx.r3.u32;
    uint32_t count    = ctx.r4.u32;
    uint32_t nameAddr = ctx.r5.u32;
    uint32_t stride   = ctx.r6.u32;

    printf("[POOL-INIT] sub_825EE000 ENTER pool=0x%08X count=%u stride=%u name=0x%08X caller=0x%08X\n",
           poolPtr, count, stride, nameAddr, static_cast<uint32_t>(ctx.lr));
    fflush(stdout);

    __imp__sub_825EE000(ctx, base);

    // After init — dump all pool fields
    uint32_t f0  = PPC_LOAD_U32(poolPtr + 0);
    uint32_t f4  = PPC_LOAD_U32(poolPtr + 4);
    uint32_t f8  = PPC_LOAD_U32(poolPtr + 8);
    uint32_t f12 = PPC_LOAD_U32(poolPtr + 12);
    uint32_t f16 = PPC_LOAD_U32(poolPtr + 16);
    uint32_t f20 = PPC_LOAD_U32(poolPtr + 20);
    printf("[POOL-INIT] sub_825EE000 EXIT  pool=0x%08X\n"
           "  +0  data_buf = 0x%08X\n"
           "  +4  status   = 0x%08X\n"
           "  +8  count    = %u\n"
           "  +12 stride   = %u (0x%08X)  %s\n"
           "  +16 last_idx = %d\n"
           "  +20 used_cnt = %u\n",
           poolPtr, f0, f4, f8, f12, f12,
           (f12 == 0 ? "*** ZERO — WILL CRASH ***" : "ok"),
           static_cast<int32_t>(f16), f20);

    // Check for overlapping allocations
    if (f0 != 0 && f0 <= poolPtr && (f0 + count * stride) > poolPtr) {
        printf("[POOL-INIT] *** OVERLAP: data_buf [0x%08X..0x%08X] contains pool struct at 0x%08X ***\n",
               f0, f0 + count * stride, poolPtr);
    }
    if (f4 != 0 && f4 <= poolPtr && (f4 + count) > poolPtr) {
        printf("[POOL-INIT] *** OVERLAP: status [0x%08X..0x%08X] contains pool struct at 0x%08X ***\n",
               f4, f4 + count, poolPtr);
    }
    if (f0 != 0 && f0 == poolPtr) {
        printf("[POOL-INIT] *** CRITICAL: data_buf == pool_ptr (same address 0x%08X) ***\n", f0);
    }
    fflush(stdout);
}

PPC_FUNC_HOOK(sub_822054F8) {
    uint32_t tls1676 = ReadTLS(ctx.r13.u32, 1676);
    printf("[POOL-DIAG] sub_822054F8 ENTER TLS[1676]=0x%08X caller=0x%08X\n",
           tls1676, static_cast<uint32_t>(ctx.lr));
    fflush(stdout);

    // Read pool ptr global BEFORE the call
    uint32_t poolBefore = PPC_LOAD_U32(POOL_PTR_GLOBAL);
    printf("[POOL-DIAG] global 0x%08X before = 0x%08X\n", POOL_PTR_GLOBAL, poolBefore);
    fflush(stdout);

    __imp__sub_822054F8(ctx, base);

    // Read pool ptr global AFTER the call
    uint32_t poolAfter = PPC_LOAD_U32(POOL_PTR_GLOBAL);
    printf("[POOL-DIAG] sub_822054F8 EXIT  global 0x%08X after = 0x%08X\n",
           POOL_PTR_GLOBAL, poolAfter);
    if (poolAfter != 0) {
        uint32_t strideVal = PPC_LOAD_U32(poolAfter + 12);
        printf("[POOL-DIAG] pool+12 stride = %u (0x%08X)\n", strideVal, strideVal);
    }
    fflush(stdout);
}

// =============================================================================
// NATIVE AES DECRYPTION — Replace recompiled RAGE AES with hardware-accelerated
// =============================================================================
// sub_827FC7F0 is RAGE's AES decrypt wrapper. Original PPC code calls
// sub_827FC738 which applies sub_827FC190 (AES-256-ECB block cipher) 16 times
// per 16-byte block. The recompiled table-based AES with PPC byte-swap
// emulation is ~1000x slower than native. Replace with CommonCrypto AES.
//
// Calling convention:
//   r3 = keySchedule (guest ptr — ignored, we use the extracted key)
//   r4 = dataPtr (guest ptr to data, decrypted IN-PLACE)
//   r5 = size in bytes (masked to 16-byte alignment internally)
//   Returns r3 = 1 (success)
// =============================================================================

#ifdef __APPLE__
static const uint8_t s_rageAesKey[32] = {
    0x1a, 0xb5, 0x6f, 0xed, 0x7e, 0xc3, 0xff, 0x01,
    0x22, 0x7b, 0x69, 0x15, 0x33, 0x97, 0x5d, 0xce,
    0x47, 0xd7, 0x69, 0x65, 0x3f, 0xf7, 0x75, 0x42,
    0x6a, 0x96, 0xcd, 0x6d, 0x53, 0x07, 0x56, 0x5d
};
#endif

PPC_FUNC_HOOK(sub_827FC7F0) {
    uint32_t dataAddr = ctx.r4.u32;
    uint32_t size = ctx.r5.u32 & 0xFFFFFFF0u;

    if (dataAddr == 0 || size == 0) {
        ctx.r3.u32 = 1;
        return;
    }

    uint8_t* data = static_cast<uint8_t*>(g_memory.Translate(dataAddr));

#ifdef __APPLE__
    // Apply AES-256-ECB decrypt 16 times per block (matches RAGE cipher)
    for (uint32_t offset = 0; offset < size; offset += 16) {
        for (int pass = 0; pass < 16; ++pass) {
            size_t outLen = 0;
            CCCrypt(kCCDecrypt, kCCAlgorithmAES, kCCOptionECBMode,
                    s_rageAesKey, sizeof(s_rageAesKey),
                    nullptr,
                    data + offset, 16,
                    data + offset, 16, &outLen);
        }
    }
#else
    // Non-Apple: fall through to original PPC code
    extern "C" void __imp__sub_827FC7F0(PPCContext &ctx, uint8_t *base);
    __imp__sub_827FC7F0(ctx, base);
    return;
#endif

    ctx.r3.u32 = 1;
}

// =============================================================================
// RAGE VFS DIAGNOSTIC — log what file path the game tries to open
// =============================================================================
extern "C" void __imp__sub_827E0898(PPCContext &ctx, uint8_t *base);

PPC_FUNC_HOOK(sub_827E0898) {
    // r3 = device manager, r4 = path string (guest addr), r5 = mode string
    uint32_t pathAddr = ctx.r4.u32;
    if (pathAddr != 0) {
        const char* path = reinterpret_cast<const char*>(base + pathAddr);
        static int s_openCount = 0;
        if (++s_openCount <= 50) {
            printf("[RAGE-VFS] sub_827E0898 open #%d path='%s' device=0x%08X\n",
                   s_openCount, path, ctx.r3.u32);
            fflush(stdout);
        }
    }
    __imp__sub_827E0898(ctx, base);
}
// =============================================================================
// DIAGNOSTIC HOOKS — trace game lifecycle (RESEARCH ONLY, no behavior changes)
// =============================================================================

// xstart — COMPLETE MANUAL OVERRIDE with per-step tracing
// Replicates the full xstart + sub_828474B8 flow using __imp__ calls
extern "C" void __imp__xstart(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__sub_82A18BE0(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__sub_82A18620(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__sub_82A110A8(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__sub_82847340(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__sub_828708D8(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__sub_8285DBE0(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__sub_822BCA90(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__sub_8285D420(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__sub_821B3598(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__sub_821B3CE8(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__sub_828474B8(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__sub_8285DAE8(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__sub_828708E0(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__XamLoaderTerminateTitle(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__DbgPrint(PPCContext &ctx, uint8_t *base);

extern "C" PPC_FUNC(xstart) {
    printf("[XSTART] ENTERED Thread=%p\n", (void*)pthread_self());
    fflush(stdout);
    
    // Defer to the original MSVC __tmainCRTStartup (renamed xstart in recomp)
    // This allows C++ global constructors and KeTlsAlloc to properly initialize
    // the global memory allocator in TLS slot 1676 before game code runs.
    __imp__xstart(ctx, base);

    // === TERMINATE ===
    printf("[XSTART] Game complete. Returning.\n"); fflush(stdout);
}

// sub_82A18BE0 — first function called by xstart (firmware/init check)
extern "C" void __imp__sub_82A18BE0(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_82A18BE0) {
    printf("[DIAG] sub_82A18BE0 ENTER (firmware check) caller=0x%08X\n",
           static_cast<uint32_t>(ctx.lr));
    fflush(stdout);
    __imp__sub_82A18BE0(ctx, base);
    printf("[DIAG] sub_82A18BE0 RETURN\n");
    fflush(stdout);
}

// sub_82A18B08 — called by sub_82A18BE0, determines if HalReturnToFirmware fires
extern "C" void __imp__sub_82A18B08(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_82A18B08) {
    printf("[DIAG] sub_82A18B08 ENTER (firmware init check)\n");
    fflush(stdout);
    __imp__sub_82A18B08(ctx, base);
    uint32_t result = ctx.r3.u32;
    printf("[DIAG] sub_82A18B08 RETURN = %u (%s)\n",
           result, result ? "OK" : "FAIL → HalReturnToFirmware will be called!");
    fflush(stdout);
}

// sub_82A18620 — second function in xstart (notification callbacks with critical section)
extern "C" void __imp__sub_82A18620(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_82A18620) {
    printf("[DIAG] sub_82A18620 ENTER (notification callbacks) arg=0x%08X\n", ctx.r3.u32);
    fflush(stdout);
    __imp__sub_82A18620(ctx, base);
    printf("[DIAG] sub_82A18620 RETURN\n");
    fflush(stdout);
}

// sub_82A110A8 — XEX privilege/AV pack check (called from xstart before anything)
extern "C" void __imp__sub_82A110A8(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_82A110A8) {
    printf("[DIAG] sub_82A110A8 ENTER (XEX privilege check) caller=0x%08X\n",
           static_cast<uint32_t>(ctx.lr));
    fflush(stdout);
    __imp__sub_82A110A8(ctx, base);
    uint32_t result = ctx.r3.u32 & 0xFF;
    printf("[DIAG] sub_82A110A8 RETURN = %u (%s)\n",
           result, result ? "FAIL → XamLoaderTerminateTitle" : "OK → continue");
    fflush(stdout);
}



// sub_82140000 — RAGE init gate.
// Calls sub_821B3CE8 (full RAGE engine init) and returns 1 on success, 0 on failure.
// CRITICAL: sub_821B3CE8 is NOT idempotent — calling it a second time hangs inside
// sub_821B49E8/sub_8285DD10 waiting on a streaming event that's already consumed.
// This function gets called TWICE:
//   1st call: from the main init path (sub_821B3CE8 runs OK, sets up all subsystems)
//   2nd call: from sub_821B3598 (per-frame tick) — must short-circuit to avoid re-init hang
// Fix: guard with a static flag; on subsequent calls return 1 immediately.
extern "C" void __imp__sub_82140000(PPCContext &ctx, uint8_t *base);
static std::atomic<bool> s_rageInitDone{false};
PPC_FUNC_HOOK(sub_82140000) {
    printf("[DIAG] sub_82140000 ENTER (RAGE init gate) caller=0x%08X\n",
           static_cast<uint32_t>(ctx.lr));
    fflush(stdout);

    if (s_rageInitDone.load(std::memory_order_acquire)) {
        // Already initialized — return 1 without re-running sub_821B3CE8
        printf("[DIAG] sub_82140000: already initialized, returning 1 (skip re-init)\n");
        fflush(stdout);
        ctx.r3.u32 = 1;
        return;
    }

    __imp__sub_82140000(ctx, base);
    uint32_t result = ctx.r3.u32 & 0xFF;
    printf("[DIAG] sub_82140000 RETURN = %u (%s)\n",
           result, result ? "SUCCESS" : "FAIL");
    fflush(stdout);

    if (result) {
        s_rageInitDone.store(true, std::memory_order_release);
    }
}

// sub_82140088 — MAIN GAME LOOP
extern "C" void __imp__sub_82140088(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_82140088) {
    printf("[DIAG] sub_82140088 ENTER (main game loop!) caller=0x%08X\n",
           static_cast<uint32_t>(ctx.lr));
    fflush(stdout);
    __imp__sub_82140088(ctx, base);
    printf("[DIAG] sub_82140088 RETURN (main game loop exited!)\n");
    fflush(stdout);
}

// sub_821458B8 — INIT GATE: returns 0 = "not ready", non-zero = "ready"
extern "C" void __imp__sub_821458B8(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_initGateCount{0};
PPC_FUNC_HOOK(sub_821458B8) {
    __imp__sub_821458B8(ctx, base);
    int n = s_initGateCount.fetch_add(1, std::memory_order_relaxed);
    uint32_t result = ctx.r3.u32 & 0xFF;
    if (n < 10 || (n & 0xFF) == 0) {
        printf("[DIAG] sub_821458B8 (init gate) #%d = %u (%s)\n",
               n, result, result ? "READY" : "not ready");
        fflush(stdout);
    }
}

// sub_821B39A8 — QUIT FLAG: returns 0 = "keep running", non-zero = "exit"
extern "C" void __imp__sub_821B39A8(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_quitCheckCount{0};
PPC_FUNC_HOOK(sub_821B39A8) {
    __imp__sub_821B39A8(ctx, base);
    int n = s_quitCheckCount.fetch_add(1, std::memory_order_relaxed);
    uint32_t result = ctx.r3.u32 & 0xFF;
    if (n < 10 || result != 0 || (n & 0xFF) == 0) {
        printf("[DIAG] sub_821B39A8 (quit flag) #%d = %u (%s)\n",
               n, result, result ? "EXIT REQUESTED" : "keep running");
        fflush(stdout);
    }
}

// sub_821B3CE8 — RAGE ENGINE INIT (the big one)
extern "C" void __imp__sub_821B3CE8(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_821B3CE8) {
    printf("[DIAG] sub_821B3CE8 ENTER (RAGE engine init) arg=0x%08X caller=0x%08X\n",
           ctx.r3.u32, static_cast<uint32_t>(ctx.lr));
    fflush(stdout);
    __imp__sub_821B3CE8(ctx, base);
    uint32_t result = ctx.r3.u32 & 0xFF;
    printf("[DIAG] sub_821B3CE8 RETURN = %u (%s)\n",
           result, result ? "INIT SUCCESS" : "INIT FAILED");
    fflush(stdout);
}

// sub_821411D8 — game systems init (only called if sub_821B3CE8 succeeds)
extern "C" void __imp__sub_821411D8(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_821411D8) {
    printf("[DIAG] sub_821411D8 ENTER (game systems init) caller=0x%08X\n",
           static_cast<uint32_t>(ctx.lr));
    fflush(stdout);
    __imp__sub_821411D8(ctx, base);
    printf("[DIAG] sub_821411D8 RETURN (game systems init done)\n");
    fflush(stdout);
}

// sub_82145420 — first-frame setup (called from sub_82140000 after sub_821411D8)
extern "C" void __imp__sub_82145420(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_82145420) {
    printf("[DIAG] sub_82145420 ENTER (first-frame setup) r3=%u r4=%u caller=0x%08X\n",
           ctx.r3.u32, ctx.r4.u32, static_cast<uint32_t>(ctx.lr));
    fflush(stdout);
    __imp__sub_82145420(ctx, base);
    printf("[DIAG] sub_82145420 RETURN\n");
    fflush(stdout);
}

// sub_821412B8 — subsystem init chain (~50 init calls, called from sub_82140000)
extern "C" void __imp__sub_821412B8(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_821412B8) {
    printf("[DIAG] sub_821412B8 ENTER (subsystem init chain) caller=0x%08X\n",
           static_cast<uint32_t>(ctx.lr));
    fflush(stdout);
    __imp__sub_821412B8(ctx, base);
    printf("[DIAG] sub_821412B8 RETURN\n");
    fflush(stdout);
}

// =============================================================================
// sub_821412B8 BINARY SEARCH — diagnostic probes on callees
// Each probe prints on entry so we know which calls complete before the hang.
// Calls are numbered per the 68-call list from research.
// =============================================================================
#define INIT_PROBE(name, num, desc) \
    extern "C" void __imp__##name(PPCContext &ctx, uint8_t *base); \
    PPC_FUNC_HOOK(name) { \
        static int _n = 0; \
        if (++_n <= 3) { printf("[INIT-PROBE] #" #num " " #name " (" desc ") call=%d\n", _n); fflush(stdout); } \
        __imp__##name(ctx, base); \
        if (_n <= 3) { printf("[INIT-PROBE] #" #num " " #name " RETURN\n"); fflush(stdout); } \
    }
INIT_PROBE(sub_82302308,  2,  "early init")
INIT_PROBE(sub_826CA440,  3,  "engine mid-level")
INIT_PROBE(sub_82299500,  4,  "renderer init")
INIT_PROBE(sub_821B4768,  7,  "player/controller")
INIT_PROBE(sub_82214E00, 10,  "game systems")
INIT_PROBE(sub_82266EA8, 11,  "subsystem")
INIT_PROBE(sub_8233C480, 13,  "subsystem r3=2000")
INIT_PROBE(sub_82140F38, 15,  "subsystem")
INIT_PROBE(sub_821E34C0, 20,  "subsystem")
INIT_PROBE(sub_82206BB8, 25,  "subsystem")
INIT_PROBE(sub_8223F458, 26,  "between 25-35")
INIT_PROBE(sub_8223C848, 27,  "between 25-35")
INIT_PROBE(sub_821FC1F8, 28,  "between 25-35")
INIT_PROBE(sub_82267948, 31,  "between 25-35")
// sub_822BCA90 — called many times, print all with caller LR
extern "C" void __imp__sub_822BCA90(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_822BCA90) {
    static int _bca_n = 0;
    ++_bca_n;
    // Suppress spam — only log first 5 and then every 10000
    if (_bca_n <= 5 || (_bca_n % 10000) == 0) {
        printf("[SYNC] sub_822BCA90 call=%d caller=0x%08X\n", _bca_n, static_cast<uint32_t>(ctx.lr));
        fflush(stdout);
    }
    __imp__sub_822BCA90(ctx, base);
}
INIT_PROBE(sub_822446A8, 35,  "subsystem")
INIT_PROBE(sub_821C61D8, 40,  "subsystem")
INIT_PROBE(sub_82220FC8, 46,  "subsystem")
INIT_PROBE(sub_822A1028, 50,  "subsystem")
INIT_PROBE(sub_82146A68, 59,  "late init")
INIT_PROBE(sub_82227D50, 63,  "late init")
INIT_PROBE(sub_82329C90, 68,  "final init")

// sub_821FC1F8 internal binary search probes (47 calls total)
// Probing at calls: 1, 5, 10, 15, 20, 25, 30, 35, 40, 45
INIT_PROBE(sub_8251BA08, 2801, "821FC1F8 call 1")
INIT_PROBE(sub_82504318, 2805, "821FC1F8 call 5")
INIT_PROBE(sub_82446BA8, 2810, "821FC1F8 call 10")
INIT_PROBE(sub_82446DB0, 2815, "821FC1F8 call 15")
INIT_PROBE(sub_8254A610, 2820, "821FC1F8 call 20")
INIT_PROBE(sub_82163F38, 2825, "821FC1F8 call 25")
INIT_PROBE(sub_82478AF8, 2830, "821FC1F8 call 30")
INIT_PROBE(sub_822B2010, 2835, "821FC1F8 call 35")
INIT_PROBE(sub_823A2108, 2840, "821FC1F8 call 40")
INIT_PROBE(sub_825030B8, 2845, "821FC1F8 call 45")

// sub_82478AF8 internal probes — trace which phase hangs
INIT_PROBE(sub_826225E0, 3001, "82478AF8 phase1 env-A")
INIT_PROBE(sub_826226B0, 3003, "82478AF8 phase1 env-C")
INIT_PROBE(sub_8294BD68, 3005, "82478AF8 phase2 audio-mgr-ctor")
INIT_PROBE(sub_8294E208, 3010, "82478AF8 phase7 finalize-audio-mgr")
INIT_PROBE(sub_82956718, 3012, "82478AF8 phase7 xaudio-obj-ctor")
INIT_PROBE(sub_82953088, 3013, "82478AF8 phase8 audio-graph-init")
INIT_PROBE(sub_82955838, 3019, "82478AF8 phase8 CreateSourceVoices")
INIT_PROBE(sub_827ADB48, 3026, "82478AF8 phase12 create-voice-block")
INIT_PROBE(sub_8287AC38, 3036, "82478AF8 phase16 streaming-graph")
INIT_PROBE(sub_8261C7C8, 3039, "82478AF8 phase16 audio3D-init")
INIT_PROBE(sub_8228A1E0, 3049, "82478AF8 phase21 start-stream-thread")
INIT_PROBE(sub_825FD6B8, 3050, "82478AF8 phase21 start-RPF-stream")
INIT_PROBE(sub_82955BE0, 3051, "82478AF8 phase21 xaudio-stream-HANG")
INIT_PROBE(sub_82477670, 3054, "82478AF8 phase22 streaming-tick")
INIT_PROBE(sub_827C2420, 3055, "82478AF8 tail activate-streaming")
INIT_PROBE(sub_8284F310, 30551, "827C2420 start-streaming-mgr")
// sub_82852DD0 replaced with custom detailed hook below (after #undef)
INIT_PROBE(sub_827ACC98, 3060, "82478AF8 tail bind-voice-ptrs")
INIT_PROBE(sub_827ACCA0, 3061, "82478AF8 tail pool-config")
INIT_PROBE(sub_8287A408, 3065, "82478AF8 tail 3D-param-A")
INIT_PROBE(sub_823B33F8, 3072, "82478AF8 tail construct-stream-block")
INIT_PROBE(sub_82478A80, 3074, "82478AF8 tail unknown-A80")
INIT_PROBE(sub_8261FBA0, 3075, "82478AF8 tail unknown-FBA0")
INIT_PROBE(sub_8287A6A8, 3076, "82478AF8 tail audio-cfg-A6A8")
INIT_PROBE(sub_8284F468, 30554, "82852DD0 alloc-resource")
INIT_PROBE(sub_82852A50, 30556, "82852D18 get-resource-ptr")
INIT_PROBE(sub_82851A10, 30557, "82852D18 pre-vtable-call")
#undef INIT_PROBE

// ---------------------------------------------------------------------------
// DIAGNOSTIC: sub_828493E0 — the ACTUAL allocator function called by operator new
// Traces entry/exit to identify if the hang is inside the allocator body
// (after the scoped lock is acquired), e.g., infinite loop in free-list walk.
// ---------------------------------------------------------------------------
extern "C" void __imp__sub_828493E0(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_alloc93E0Count{0};

PPC_FUNC_HOOK(sub_828493E0) {
    int n = s_alloc93E0Count.fetch_add(1, std::memory_order_relaxed);
    uint32_t size = ctx.r4.u32;
    uint32_t caller = static_cast<uint32_t>(ctx.lr);

    // Only trace when called from the engine init range
    bool fromInit = (caller >= 0x82478000 && caller < 0x8247A000) ||
                    (caller >= 0x82847500 && caller < 0x82847700);

    if (fromInit || n < 5) {
        printf("[ALLOC-93E0] ENTER #%d size=%u caller=0x%08X this=0x%08X\n",
               n, size, caller, ctx.r3.u32);
        fflush(stdout);
    }

    __imp__sub_828493E0(ctx, base);

    if (fromInit || n < 5) {
        printf("[ALLOC-93E0] RETURN #%d r3=0x%08X\n", n, ctx.r3.u32);
        fflush(stdout);
    }
}

// ---------------------------------------------------------------------------
// sub_821B3510 — operator new (diagnostic hook)
// ---------------------------------------------------------------------------
extern "C" void __imp__sub_821B3510(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_allocTailCount{0};
PPC_FUNC_HOOK(sub_821B3510) {
    uint32_t lr = (uint32_t)ctx.lr;
    bool fromTail = (lr >= 0x82478000 && lr < 0x8247A000);
    if (fromTail) {
        int n = s_allocTailCount.fetch_add(1, std::memory_order_relaxed) + 1;
        uint32_t size = ctx.r3.u32;
        // sub_821B3510 always sets flags=0 (li r6,0)
        constexpr uint32_t flags = 0;

        // Trace the vtable dispatch chain (Level 1: multi-allocator)
        uint32_t tlsBase = PPC_LOAD_U32(ctx.r13.u32 + 0);
        uint32_t allocator = (tlsBase != 0) ? PPC_LOAD_U32(tlsBase + 1676) : 0;
        uint32_t vtable = (allocator != 0) ? PPC_LOAD_U32(allocator + 0) : 0;
        uint32_t target = (vtable != 0) ? PPC_LOAD_U32(vtable + 8) : 0;

        // Level 2: sub_828475F8 loads sub-allocator at this[(flags+1)*4]
        uint32_t subAllocIdx = (flags + 1) * 4;
        uint32_t subAlloc = (allocator != 0) ? PPC_LOAD_U32(allocator + subAllocIdx) : 0;
        uint32_t subVtable = (subAlloc != 0) ? PPC_LOAD_U32(subAlloc + 0) : 0;
        uint32_t subTarget = (subVtable != 0) ? PPC_LOAD_U32(subVtable + 8) : 0;

        printf("[TAIL-AF8] ENTER #%d caller=0x%08X size=%d "
               "alloc=0x%08X->0x%08X "
               "subAlloc[+%u]=0x%08X->vtable=0x%08X->Alloc=0x%08X\n",
               n, lr, size,
               allocator, target,
               subAllocIdx, subAlloc, subVtable, subTarget);
        fflush(stdout);
    }
    __imp__sub_821B3510(ctx, base);
    if (fromTail) {
        printf("[TAIL-AF8] sub_821B3510(alloc) RETURN r3=0x%08X\n", ctx.r3.u32);
        fflush(stdout);
    }
}

// High-limit probes filtered to only log calls from sub_82478AF8 (0x82478xxx-0x82479xxx)
extern "C" void __imp__sub_8284E830(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_8284E830) {
    uint32_t lr = (uint32_t)ctx.lr;
    bool fromTail = (lr >= 0x82478000 && lr < 0x8247A000);
    if (fromTail) { printf("[TAIL-AF8] sub_8284E830 ENTER caller=0x%08X r3=0x%08X\n", lr, ctx.r3.u32); fflush(stdout); }
    __imp__sub_8284E830(ctx, base);
    if (fromTail) { printf("[TAIL-AF8] sub_8284E830 RETURN\n"); fflush(stdout); }
}

extern "C" void __imp__sub_8284D220(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_8284D220) {
    uint32_t lr = (uint32_t)ctx.lr;
    bool fromTail = (lr >= 0x82478000 && lr < 0x8247A000);
    if (fromTail) { printf("[TAIL-AF8] sub_8284D220 ENTER caller=0x%08X\n", lr); fflush(stdout); }
    __imp__sub_8284D220(ctx, base);
    if (fromTail) { printf("[TAIL-AF8] sub_8284D220 RETURN\n"); fflush(stdout); }
}

extern "C" void __imp__sub_8299B4A8(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_8299B4A8) {
    uint32_t lr = (uint32_t)ctx.lr;
    bool fromTail = (lr >= 0x82478000 && lr < 0x8247A000);
    if (fromTail) { printf("[TAIL-AF8] sub_8299B4A8 ENTER caller=0x%08X\n", lr); fflush(stdout); }
    __imp__sub_8299B4A8(ctx, base);
    if (fromTail) { printf("[TAIL-AF8] sub_8299B4A8 RETURN\n"); fflush(stdout); }
}

// sub_82852DD0 — "OpenAndProcess" streaming resource loader.
// Custom hook replacing INIT_PROBE — traces every internal step with thread ID.
// Flow: sub_8284F468 (find) → sub_82852D18 (process) → sub_8285B088 (flush) → return
extern "C" void __imp__sub_82852DD0(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_DD0_count{0};
PPC_FUNC_HOOK(sub_82852DD0) {
    int n = s_DD0_count.fetch_add(1, std::memory_order_relaxed) + 1;
    uint64_t tid = 0;
    pthread_threadid_np(nullptr, &tid);
    printf("[TRACE-DD0] ENTER #%d tid=%llu r3=0x%08X r4=0x%08X r5=0x%08X r6=0x%08X r7=0x%08X r8=0x%08X caller=0x%08X\n",
           n, tid, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r8.u32, (uint32_t)ctx.lr);
    fflush(stdout);
    __imp__sub_82852DD0(ctx, base);
    printf("[TRACE-DD0] RETURN #%d tid=%llu r3=0x%08X\n", n, tid, ctx.r3.u32);
    fflush(stdout);
}

// sub_82852D18 — Resource processor that hangs at vtable[2] dispatch.
// Diagnostic hook: prints the resolved vtable target before calling original.
extern "C" void __imp__sub_82852D18(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_852D18_count{0};
PPC_FUNC_HOOK(sub_82852D18) {
    int n = s_852D18_count.fetch_add(1, std::memory_order_relaxed) + 1;
    uint64_t tid = 0;
    pthread_threadid_np(nullptr, &tid);
    uint32_t r5_obj = ctx.r5.u32;
    uint32_t vtable_ptr = r5_obj ? PPC_LOAD_U32(r5_obj) : 0;
    uint32_t vtable_slot2 = vtable_ptr ? PPC_LOAD_U32(vtable_ptr + 8) : 0;
    printf("[TRACE-D18] ENTER #%d tid=%llu r3=0x%08X r4=0x%08X r5=0x%08X vt=0x%08X vt[2]=0x%08X r6=0x%08X r7=0x%08X r8=0x%08X caller=0x%08X\n",
           n, tid, ctx.r3.u32, ctx.r4.u32, r5_obj, vtable_ptr, vtable_slot2,
           ctx.r6.u32, ctx.r7.u32, ctx.r8.u32, (uint32_t)ctx.lr);
    fflush(stdout);
    __imp__sub_82852D18(ctx, base);
    printf("[TRACE-D18] RETURN #%d tid=%llu r3=0x%08X\n", n, tid, ctx.r3.u32);
    fflush(stdout);
}

// =============================================================================
// SCENE CREATION PATH DIAGNOSTICS
// =============================================================================
// The front-end state machine (sub_82142230) controls scene creation.
// States 0-2 are XAM dialog states, state 3 is resource loading, states 4-6
// are scene creation. The scene pointer at 0x831C2458 remains NULL if the
// state machine never reaches state 3+. These hooks trace the path.

// sub_82142230 — FRONT-END STATE MACHINE (states 0-6)
// This is the main loop that controls sign-in → storage → save → scene creation.
// r29 tracks the current state.
extern "C" void __imp__sub_82142230(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_82142230) {
    printf("[STATE-MACHINE] sub_82142230 ENTER caller=0x%08X\n",
           static_cast<uint32_t>(ctx.lr));
    fflush(stdout);
    __imp__sub_82142230(ctx, base);
    printf("[STATE-MACHINE] sub_82142230 RETURN\n");
    fflush(stdout);
}

// sub_822414E8 — STATE 0: Sign-in check
// Returns: 0 = not signed in (loop), 1 = signed in (→ state 1), 2 = skip to state 3+
extern "C" void __imp__sub_822414E8(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_state0Count{0};
PPC_FUNC_HOOK(sub_822414E8) {
    __imp__sub_822414E8(ctx, base);
    int n = s_state0Count.fetch_add(1, std::memory_order_relaxed);
    if (n < 20 || (n % 200) == 0) {
        printf("[STATE-0] sub_822414E8 (sign-in check) #%d = %d\n", n, ctx.r3.s32);
        fflush(stdout);
    }
}

// sub_8223DDA8 — STATE 1: Storage device selection
// Returns: 1 or 2 = advance to state 2, other = loop
extern "C" void __imp__sub_8223DDA8(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_state1Count{0};
PPC_FUNC_HOOK(sub_8223DDA8) {
    __imp__sub_8223DDA8(ctx, base);
    int n = s_state1Count.fetch_add(1, std::memory_order_relaxed);
    if (n < 20 || (n % 200) == 0) {
        printf("[STATE-1] sub_8223DDA8 (storage device) #%d = %d\n", n, ctx.r3.s32);
        fflush(stdout);
    }
}

// sub_8223DEE8 — STATE 2: Save/load check
// Returns: 1 = goto loc_821422FC (advance), 2 = state 8 (jump), other = loop
extern "C" void __imp__sub_8223DEE8(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_state2Count{0};
PPC_FUNC_HOOK(sub_8223DEE8) {
    __imp__sub_8223DEE8(ctx, base);
    int n = s_state2Count.fetch_add(1, std::memory_order_relaxed);
    if (n < 20 || (n % 200) == 0) {
        printf("[STATE-2] sub_8223DEE8 (save/load) #%d = %d\n", n, ctx.r3.s32);
        fflush(stdout);
    }
}

// sub_821406C8 — PLAYER ACCESSOR (state 3 gate)
// Reads active player index from 0x82A9172C. Returns NULL if -1 (no player).
// Returns pointer to 188-byte player slot struct.
// State 3 checks "newly set" transitions on slot fields to detect content readiness.
extern "C" void __imp__sub_821406C8(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_playerAccessCount{0};
PPC_FUNC_HOOK(sub_821406C8) {
    __imp__sub_821406C8(ctx, base);
    uint32_t slotPtr = ctx.r3.u32;
    int n = s_playerAccessCount.fetch_add(1, std::memory_order_relaxed);

    // Populate player slot content-readiness fields so state 3's "newly set"
    // transition detectors fire. On Xbox 360, XAM notification callbacks
    // write these. In the recomp, we set them here on first call.
    // The detector pattern: triggers when current != 0 AND shadow == 0.
    // We write current fields; shadows stay 0 until the game copies them.
    if (slotPtr != 0 && n == 0) {
        // slot[56] = DLC/title update content ready (check 4 → sets r29=4)
        PPC_STORE_U32(slotPtr + 56, 1);
        // slot[68] = Sign-in completion
        PPC_STORE_U32(slotPtr + 68, 1);
        // slot[72] = Storage device selected
        PPC_STORE_U32(slotPtr + 72, 1);
        // slot[4] = Profile data loaded
        PPC_STORE_U32(slotPtr + 4, 1);
        printf("[STATE-3] Populated player slot 0x%08X content fields (56,68,72,4)\n", slotPtr);
        fflush(stdout);
    }

    if (n < 20 || (n % 200) == 0) {
        uint32_t activeIdx = PPC_LOAD_U32(0x82A9172C);
        printf("[STATE-3] sub_821406C8 #%d = 0x%08X activeIdx=0x%08X "
               "s56=0x%X s68=0x%X s72=0x%X s4=0x%X\n",
               n, slotPtr, activeIdx,
               slotPtr ? PPC_LOAD_U32(slotPtr + 56) : 0,
               slotPtr ? PPC_LOAD_U32(slotPtr + 68) : 0,
               slotPtr ? PPC_LOAD_U32(slotPtr + 72) : 0,
               slotPtr ? PPC_LOAD_U32(slotPtr + 4) : 0);
        fflush(stdout);
    }
}

// sub_82142F90 — MAIN FRAME UPDATE (called every frame)
// This drives sub_82142230 and the scene render.
extern "C" void __imp__sub_82142F90(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_frameUpdateCount{0};
PPC_FUNC_HOOK(sub_82142F90) {
    int n = s_frameUpdateCount.fetch_add(1, std::memory_order_relaxed);
    if (n < 5 || (n % 500) == 0) {
        uint32_t scenePtr = PPC_LOAD_U32(0x831C2458);
        uint32_t innerState4 = PPC_LOAD_U32(0x82BF99D4); // sub_822440F8 inner state
        uint32_t innerState6 = PPC_LOAD_U32(0x82BF9838); // sub_822438B0 inner state
        uint32_t playerIdx = PPC_LOAD_U32(0x82A95478);   // active player/episode index
        uint32_t profileIdx = PPC_LOAD_U32(0x82A95474);  // active profile index
        printf("[FRAME-UPDATE] #%d scene=0x%08X s4inner=%d s6inner=%d "
               "playerIdx=0x%08X profileIdx=0x%08X\n",
               n, scenePtr, innerState4, innerState6, playerIdx, profileIdx);
        fflush(stdout);
    }
    __imp__sub_82142F90(ctx, base);
}

// sub_822440F8 — STATE 4 INNER STATE MACHINE (7 states, save/content)
// Called by outer state 4. Returns: 0=working, 1=done/error(→r29=7), 2=advance(→r29=5)
//
// BYPASS: This function's entire purpose is Xbox 360 save device selection:
//   State 0: sub_82241428 scans controllers for one WITHOUT storage mapped
//   State 1: Validates storage device via content enumeration
//   State 2: Polls content loading (sub_82243F00)
//   State 3-6: Save slot enumeration and read/write
//
// None of this works in recomp (no controllers, no storage devices).
// sub_82241428 always returns -1 (no controller found) → state 0 returns 1 (error).
//
// Fix: Return 2 (success, no-save path) directly. This is what inner state 6
// returns naturally when no save data exists. The game proceeds to state 5
// (level selection → scene load dispatch) and then state 6 (scene creation).
//
// Side effects reproduced:
//   - 0x82A95478 (playerIdx) set to 0 (base game, player 0)
//   - Profile index left at default (game sets it during state 5)
// sub_822440F8 — REMOVED: was bypassing Xbox save device selection.
// Let rexglue's recompiled code handle the save device flow.

// sub_822438B0 — STATE 6 INNER STATE MACHINE (8 states, scene/world loading)
// Inner state at 0x82BF9838 (lis -32064 + offset -26568)
// Calls sub_82242910 (15-state scene creation sub-machine) in state 2
extern "C" void __imp__sub_822438B0(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_state6InnerCount{0};
PPC_FUNC_HOOK(sub_822438B0) {
    uint32_t stateBefore = PPC_LOAD_U32(0x82BF9838);
    __imp__sub_822438B0(ctx, base);
    uint32_t stateAfter = PPC_LOAD_U32(0x82BF9838);
    int n = s_state6InnerCount.fetch_add(1, std::memory_order_relaxed);
    if (n < 30 || stateBefore != stateAfter || (n % 500) == 0) {
        uint32_t sceneState = PPC_LOAD_U32(0x82BF9848); // sub_82242910 state (0-14)
        uint32_t scenePtr = PPC_LOAD_U32(0x82BF3A88);   // scene object pointer
        uint32_t errorCode = PPC_LOAD_U32(0x82A9546C);  // error code
        printf("[STATE-6-INNER] sub_822438B0 #%d ret=%d state=%d→%d "
               "sceneCreation=%d sceneObj=0x%08X err=%d\n",
               n, ctx.r3.s32, stateBefore, stateAfter,
               sceneState, scenePtr, errorCode);
        fflush(stdout);
    }
}

// sub_822422E0 — STATE 5: GAME START (level selection + scene load dispatch)
// Reads episode index from 0x82B39504 to pick level 12/13/14
// State variable at 0x82BF9834 (NOT 0x829F9834)
//
// IMPORTANT: After this runs, it writes 2 to 0x82BF9834 ("done").
// sub_82242910 (scene creation, called from state 6) reads the SAME address
// at its internal state 4. Value 2 triggers error 34 (case 2 in platform mode switch).
// On Xbox 360, sub_8223DAA0 returns 0 in the scene creation context (not yet ready),
// so sub_82242910 takes the normal path (state 0→1) and never reads 0x82BF9834.
// In the recomp, sub_8223DAA0 returns 1 (ready, because sign-in emulation succeeds),
// causing the fast path (state 0→4) which hits the stale value 2 → error 34.
//
// Fix: Reset 0x82BF9834 to 0 after state 5 completes, so scene creation can proceed.
extern "C" void __imp__sub_822422E0(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_822422E0) {
    uint32_t stateBefore = PPC_LOAD_U32(0x82BF9834);
    uint32_t episode = PPC_LOAD_U32(0x82B39504);
    printf("[STATE-5-START] sub_822422E0 ENTER state=%d episode=%d\n",
           stateBefore, episode);
    fflush(stdout);
    __imp__sub_822422E0(ctx, base);
    uint32_t stateAfter = PPC_LOAD_U32(0x82BF9834);
    printf("[STATE-5-START] sub_822422E0 ret=%d state=%d→%d\n",
           ctx.r3.s32, stateBefore, stateAfter);
    fflush(stdout);

    // Reset the state variable so sub_82242910 (scene creation) doesn't
    // see value 2 at its internal state 4 and trigger error 34.
    if (stateAfter == 2) {
        PPC_STORE_U32(0x82BF9834, 0);
        printf("[STATE-5-START] Reset 0x82BF9834: 2 → 0 (prevent scene creation error 34)\n");
        fflush(stdout);
    }
}

// =============================================================================
// SCENE STATE MACHINE — PLATFORM ADAPTATION HOOKS
// These hooks replace Xbox 360-specific blocking patterns with immediate-
// completion equivalents, preserving all game logic. See docs/rewrite/ for
// the full research documentation (27 files).
// =============================================================================

// sub_8223DB20 — SIGN-IN NOTIFICATION GUARD
// Called by ~30 states across sub_82242910. Polls XNotifyGetNext(handle, 10)
// for XN_SYS_SIGNINCHANGED. If a notification arrives, triggers error 33.
// In the recomp, RexGlue broadcasts 0x0A at startup which can cause spurious
// error 33. Safe to stub: no game-state side effects, pure notification check.
PPC_FUNC_HOOK(sub_8223DB20) {
    ctx.r3.u64 = 0;  // No sign-in change detected
}

// sub_82240B78 — STORAGE DEVICE NOTIFICATION GUARD
// Called by ~17 states. Polls XNotifyGetNext(handle, 11) for
// XN_SYS_STORAGEDEVICESCHANGED. Triggers error 34 if device removed.
// RexGlue never broadcasts 0x0B, so this already returns 0 in practice.
// Stub makes it explicit and eliminates XNotifyGetNext overhead.
PPC_FUNC_HOOK(sub_82240B78) {
    ctx.r3.u64 = 0;  // No storage device change detected
}

// sub_82240B08 — CONTENT DEVICE READINESS CHECK
// Called by state 4 of sub_82242910. Checks if save device handle is valid
// via XamContentGetDeviceData. On Xbox 360, an async content creation
// populates the handle; in the recomp, it stays null because no async op runs.
// Fix: Set readiness flags and return 1 (device ready). This makes state 4
// jump directly to state 9 (scene loading), which is the normal path when
// a valid save device already exists.
PPC_FUNC_HOOK(sub_82240B08) {
    PPC_STORE_U8(0x82BF3A77, 1);   // g_sceneReady = 1
    PPC_STORE_U8(0x82BF3CDA, 1);   // g_contentReady = 1
    ctx.r3.u64 = 1;                // Device is ready
}

// sub_8224FFC8 — XAM DIALOG RESULT PROCESSOR
// Called 68 times across the codebase. Checks if an Xbox Guide dialog was
// accepted (r3=8) or cancelled (r3=11). On Xbox 360, this reads the Guide
// overlay result. In the recomp, no Guide exists, so it always returns 0,
// preventing the step counter at 0x82BFA13C from advancing. This causes
// the ready-signal at 0x82BF9B70 to oscillate 1→2→1→2 forever.
// Fix: Auto-accept dialogs (return 1 for accept, 0 for cancel).
PPC_FUNC_HOOK(sub_8224FFC8) {
    uint32_t queryType = ctx.r3.u32;
    if (queryType == 8) {
        ctx.r3.u64 = 1;  // Accept — simulate "user pressed A"
    } else {
        ctx.r3.u64 = 0;  // Cancel / other — not pressed
    }
}

// sub_8284A7E8 — CONTENT CREATION INITIATOR (populates slot[136], calls XamContentCreateEx)
// After XamContentCreateEx, this function checks if return == 997 (IO_PENDING).
// If 997: sets slot state = 17 (async path — correct, measures file size later).
// If not 997: sets slot state = 16 (sync path — skips file size → size mismatch).
// RexGlue SHOULD return 997 when overlapped is non-null, but we need to verify.
extern "C" void __imp__sub_8284A7E8(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_contentCreateCount{0};
PPC_FUNC_HOOK(sub_8284A7E8) {
    uint32_t slotIdx = ctx.r4.u32;
    int n = s_contentCreateCount.fetch_add(1, std::memory_order_relaxed);
    printf("[CONTENT-CREATE] sub_8284A7E8 #%d ENTER slotIdx=%d r5=0x%08X r6=0x%08X\n",
           n, slotIdx, ctx.r5.u32, ctx.r6.u32);
    fflush(stdout);

    __imp__sub_8284A7E8(ctx, base);

    // After the function runs, check what slot state was set.
    // Table at 0x83192C58, stride 160 bytes. State is at slot+0 (first dword).
    uint32_t tableBase = 0x83192C58;
    uint32_t slotAddr = tableBase + (slotIdx * 160);
    uint32_t slotState = PPC_LOAD_U32(slotAddr);
    uint32_t slot136 = PPC_LOAD_U32(slotAddr + 136);
    uint32_t slot144 = PPC_LOAD_U32(slotAddr + 144);
    printf("[CONTENT-CREATE] sub_8284A7E8 #%d RETURN ret=%d slotState=%d "
           "slot[136]=%u slot[144]=%u (state 16=sync/BAD, 17=async/GOOD)\n",
           n, ctx.r3.s32, slotState, slot136, slot144);
    fflush(stdout);
}

// sub_822417B0 — TWO-PHASE CONTENT SIZE CHECKER
// Phase 1 (r4=1): initiates async content open.
// Phase 2 (r4=0): polls for completion, writes size delta to 0x82BF99C8.
// If delta < 0, state 14 transitions to state 13 (error restart loop).
// Fix: After poll completes, clamp delta to >= 0 (no storage deficit on PC).
// Only 2 callers (states 12 and 14 of sub_82242910). Safe and contained.
extern "C" void __imp__sub_822417B0(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_twoPhaseCount{0};
PPC_FUNC_HOOK(sub_822417B0) {
    uint32_t phase = ctx.r4.u32;
    int n = s_twoPhaseCount.fetch_add(1, std::memory_order_relaxed);

    __imp__sub_822417B0(ctx, base);

    int32_t delta = static_cast<int32_t>(PPC_LOAD_U32(0x82BF99C8));
    if (delta < 0) {
        PPC_STORE_U32(0x82BF99C8, 0);
        printf("[TWO-PHASE] sub_822417B0 #%d phase=%d ret=%d CLAMPED delta %d→0 "
               "(no storage deficit on PC)\n",
               n, phase, ctx.r3.s32, delta);
        fflush(stdout);
    } else if (n < 20 || (n % 100) == 0) {
        printf("[TWO-PHASE] sub_822417B0 #%d phase=%d ret=%d delta=%d\n",
               n, phase, ctx.r3.s32, delta);
        fflush(stdout);
    }
}

// sub_82A00DC0 — REMOVED: was native memcpy replacement.
// Let rexglue's recompiled PPC memcpy run. Note: the rexcrt memcpy hook at
// 0x82A11940 (in gta4_config.toml [rexcrt]) already provides a native memcpy
// for the main CRT memcpy. sub_82A00DC0 is a separate alignment-aware variant.

// =============================================================================
// sub_821910D0 — XAUDIO RENDER THREAD PUMP
// =============================================================================
// This function is the audio render thread's per-frame worker. It enters a
// critical section, processes audio buffers, then calls vtable[17] on the
// XAudioRenderDriverEndpoint (XAudioRenderDriverEndpoint::Present / DMA
// trigger). On Xbox 360, vtable[17] points into xaudio2.xex code. In the
// recomp, audio is handled natively via SDL2 and XAudioRenderDriverInitialize
// is intentionally not emulated (we're not emulating hardware), so the vtable
// contains garbage (0x000F4000). The MISSING-FUNC handler silently skips the
// call and execution continues correctly — the function handles critical
// section, buffer swap, frame counter, and event signaling properly.
//
// Key addresses (computed from lis/addi sequences in the recomp):
//   Critical section:   0x82B2834C (r28+4)
//   Audio device global: PPC_LOAD_U32(0x831D53EC) → r30
//   Frame counter:      0x831D53F0 (atomic increment)
//   Buffer swap base:   0x831D53F0 / 0x831D540C (7 u32 double-buffer)
//
// No hook needed — the default PPC_WEAK_FUNC passthrough is correct.
// The MISSING-FUNC handler silently skips the broken vtable call (42 fprintf
// to stderr per run — negligible noise). All function logic including critical
// section, audio buffer drain, frame submission, frame counter increment, and
// buffer swap operates correctly with the skip.

// =============================================================================

// sub_82242910 — SCENE CREATION SUB-MACHINE (15 states, 0-14)
// Called from sub_822438B0 state 2. Creates the game world.
// State counter at 0x82BF9848 (lis r26=-32064 → 0x82C00000, offset -26552 → -0x67B8).
// NOTE: sub_822438B0 uses 0x82BF9838 (offset -26568). They are DIFFERENT state variables!
// Scene object written to 0x82BF3A88.
//
// FIX: Intercept fast path (state 0→4) and force normal path (state 0→1).
// On Xbox 360, sub_8223DAA0 returns 0 at state 0 after a device enumeration
// delay, taking the normal path 0→1→2→3→4. In the recomp, RexGlue reports
// devices as immediately ready, so sub_8223DAA0 returns 1 (not ready / fast
// path) on the first call, jumping directly to state 4. This skips states
// 1-3 which write value 6 to 0x82A9546C. State 4 then reads stale data
// and triggers error 34 in the platform mode switch.
//
// Fix: If state jumped 0→4, force it back to 1 (normal path entry).
// State 1 has no Xbox hardware dependencies. States 1-3 set up the
// prerequisites that state 4 expects. sub_8223DB20 (used by state 4+)
// cannot fail — both return paths are valid.
//
// Defense-in-depth: The sub_822422E0 hook (state 5) also resets 0x82BF9834
// from 2→0 as a safety net in case the fast path is still somehow taken.
extern "C" void __imp__sub_82242910(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_sceneCreateCount{0};
PPC_FUNC_HOOK(sub_82242910) {
    uint32_t stateBefore = PPC_LOAD_U32(0x82BF9848);  // sub_82242910's own state
    uint32_t modeVal = PPC_LOAD_U32(0x82BF9844);  // platform mode switch (offset -26556)
    uint32_t oldModeVal = PPC_LOAD_U32(0x82BF9834); // legacy diagnostic addr

    // Intercept fast path 0→4: force normal path entry at state 1
    if (stateBefore == 0) {
        printf("[SCENE-CREATE] PRE state=0: platformMode@0x82BF9844=%d "
               "legacy@0x82BF9834=%d\n", modeVal, oldModeVal);
        fflush(stdout);
    }

    // PRE-CALL FIX: Ensure platformMode is 3 BEFORE the function runs.
    // State 4's scene gate requires (val-3) unsigned ≤ 1 → only 3 or 4
    // call sub_8223F308 (scene creation). After state 4, we reset to 0.
    if (stateBefore <= 4) {
        uint32_t platMode = PPC_LOAD_U32(0x82BF9844);
        if (platMode != 3 && platMode != 4) {
            PPC_STORE_U32(0x82BF9844, 3);
            printf("[SCENE-CREATE] PRE-FIX platformMode@0x82BF9844: %d→3 "
                   "(for scene creation gate)\n", platMode);
            fflush(stdout);
        }
    }

    __imp__sub_82242910(ctx, base);
    uint32_t stateAfter = PPC_LOAD_U32(0x82BF9848);

    if (stateBefore == 0 && stateAfter == 4) {
        PPC_STORE_U32(0x82BF9848, 1);
        stateAfter = 1;
        printf("[SCENE-CREATE] INTERCEPTED fast path 0→4, forced to 0→1 "
               "(normal path entry)\n");
        fflush(stdout);
    }

    // NOTE: platformMode stays 3 through states 10/11/12/14.
    // State 11 with {3,4} routes to state 12 (save overwrite check).
    // State 12's content size comparison is the current blocker — see docs/rewrite/28-38.

    int n = s_sceneCreateCount.fetch_add(1, std::memory_order_relaxed);
    if (n < 50 || stateBefore != stateAfter || (n % 200) == 0) {
        uint32_t sceneObj = PPC_LOAD_U32(0x82BF3A88);
        uint32_t errorCode = PPC_LOAD_U32(0x82A9546C);
        uint32_t platMode = PPC_LOAD_U32(0x82BF9844);
        printf("[SCENE-CREATE] sub_82242910 #%d ret=%d state=%d→%d "
               "platMode=%d sceneObj=0x%08X err=%d\n",
               n, ctx.r3.s32, stateBefore, stateAfter, platMode, sceneObj, errorCode);
        fflush(stdout);
    }
}

// sub_8223E028 — STATE MACHINE EXIT (writes completion bytes)
// Called when r29 > 6 — writes to 0x831D5348 and 0x831D5337.
// If this is never called, the state machine is stuck.
extern "C" void __imp__sub_8223E028(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_8223E028) {
    printf("[STATE-EXIT] sub_8223E028 ENTER (state machine completion!) caller=0x%08X\n",
           static_cast<uint32_t>(ctx.lr));
    fflush(stdout);
    __imp__sub_8223E028(ctx, base);
    printf("[STATE-EXIT] sub_8223E028 RETURN\n");
    fflush(stdout);
}

// sub_821B4108 — ACTIVE PLAYER COUNT (for state machine gate)
// Uses player pool at 0x82B29F18. Returns count of active players.
extern "C" void __imp__sub_821B4108(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_activeCountCalls{0};
PPC_FUNC_HOOK(sub_821B4108) {
    __imp__sub_821B4108(ctx, base);
    int n = s_activeCountCalls.fetch_add(1, std::memory_order_relaxed);
    if (n < 20 || (n % 200) == 0) {
        printf("[PLAYER-COUNT] sub_821B4108 #%d = %d\n", n, ctx.r3.s32);
        fflush(stdout);
    }
}

// sub_82241370 — Called at top of sub_82142230 (before state switch)
extern "C" void __imp__sub_82241370(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_preStateCount{0};
PPC_FUNC_HOOK(sub_82241370) {
    __imp__sub_82241370(ctx, base);
    int n = s_preStateCount.fetch_add(1, std::memory_order_relaxed);
    if (n < 5) {
        printf("[PRE-STATE] sub_82241370 #%d = %d\n", n, ctx.r3.s32);
        fflush(stdout);
    }
}

// sub_821428C8 — Called each iteration of state machine loop (per-frame update within states)
extern "C" void __imp__sub_821428C8(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_perIterCount{0};
PPC_FUNC_HOOK(sub_821428C8) {
    __imp__sub_821428C8(ctx, base);
    int n = s_perIterCount.fetch_add(1, std::memory_order_relaxed);
    if (n < 5 || (n % 500) == 0) {
        printf("[ITER] sub_821428C8 #%d\n", n);
        fflush(stdout);
    }
}

// =============================================================================
// sub_821B5890 — Frame Sync Wait (VBlank event)
//
// Called at the end of each frame in the main game loop (sub_82140088).
// Original PPC code:
//   if (byte[r3+4040] == 0)
//       sub_828497D8(dword[r3+4024]);  // NtWaitForSingleObjectEx(event, INFINITE)
//
// On Xbox 360, the GPU VBlank interrupt signals this event each frame.
// In the recomp there is no VBlank interrupt — the event is never signaled,
// so the game thread blocks forever after frame 5.
//
// Fix: set the skip-flag byte (r3+4040) to 1 so the wait is bypassed,
// then call the original to execute the rest of the function (timer updates,
// sub_821B7EB8, sub_821B7D28, etc.).
// =============================================================================
extern "C" void __imp__sub_821B5890(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_821B5890) {
    uint32_t timer_obj = ctx.r3.u32;
    // Force the "skip wait" flag so the NtWaitForSingleObjectEx call is bypassed.
    // Offset 4040 (0xFC8) is the VBlank-ready flag: 0 = wait, non-zero = skip.
    PPC_STORE_U8(timer_obj + 4040, 1);
    __imp__sub_821B5890(ctx, base);
}

// sub_82212EC0 — Audio device connection state machine step.
// Called every 100ms from sub_82212F38's yield loop. On Xbox 360, an async kernel
// callback writes struct+2016 = 0 (connected) after XAudio2 device arrival.
// On PC/macOS that callback never fires, so the loop spins forever.
// Fix: write state=0 (connected) and endpoint_count=1 on first call.
extern "C" void __imp__sub_82212EC0(PPCContext &ctx, uint8_t *base);
static std::atomic<bool> s_audioDeviceFixed{false};
PPC_FUNC_HOOK(sub_82212EC0) {
    if (!s_audioDeviceFixed.exchange(true, std::memory_order_relaxed)) {
        uint32_t structPtr = ctx.r3.u32;
        // struct+2016 = connection state: 1=connecting, 0=connected
        PPC_STORE_U32(structPtr + 2016, 0);
        // struct+288 = endpoint count: must be >= 1 for downstream init
        PPC_STORE_U32(structPtr + 288, 1);
        printf("[AUDIO-FIX] sub_82212EC0: forced device state=connected (struct+2016=0, struct+288=1) at struct=0x%08X\n",
               structPtr);
        fflush(stdout);
    }
    // Skip the original — it calls sub_8220FDB8 which tries Xbox async I/O
    return;
}

// =============================================================================
// MISSING-FUNC ELIMINATION HOOKS (3 independent fixes)
// These eliminate 288K MISSING-FUNC dispatches that cause stack overflow.
// =============================================================================

// Fix 1: sub_821B3970 / sub_821B3990 — grmSetup vtable dispatch thunks
// These load the grmSetup singleton from 0x82B29EEC and dispatch vtable[4]/[5].
// When the global is null (before RAGE init creates the object), they dispatch
// to address 0 → MISSING-FUNC. Null-guard eliminates ~180K hits.
extern "C" void __imp__sub_821B3970(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_821B3970) {
    uint32_t obj = PPC_LOAD_U32(0x82B29EEC);
    if (obj == 0) { ctx.r3.u64 = 0; return; }
    uint32_t vtable_ptr = PPC_LOAD_U32(obj);
    static bool s_dumped = false;
    if (!s_dumped) {
        s_dumped = true;
        uint32_t slot4 = PPC_LOAD_U32(vtable_ptr + 16);
        printf("[VTABLE-DIAG] sub_821B3970 first call:\n");
        printf("  obj_ptr    = 0x%08X\n", obj);
        printf("  vtable_ptr = 0x%08X (expect 0x82000990)\n", vtable_ptr);
        printf("  slot4      = 0x%08X (expect 0x828C5B08)\n", slot4);
        printf("  obj dump:");
        for (int i = 0; i < 8; i++) printf(" %08X", PPC_LOAD_U32(obj + i * 4));
        printf("\n");
        fflush(stdout);
    }
    if (vtable_ptr < 0x82000400 || vtable_ptr >= 0x82107068) {
        ctx.r3.u64 = 0; return;
    }
    __imp__sub_821B3970(ctx, base);
}

extern "C" void __imp__sub_821B3990(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_821B3990) {
    uint32_t obj = PPC_LOAD_U32(0x82B29EEC);
    if (obj == 0) { ctx.r3.u64 = 0; return; }
    uint32_t vtable_ptr = PPC_LOAD_U32(obj);
    if (vtable_ptr < 0x82000400 || vtable_ptr >= 0x82107068) {
        ctx.r3.u64 = 0; return;
    }
    __imp__sub_821B3990(ctx, base);
}

// Fix 2: sub_82191858 — XAudio voice queue drain (use-after-free)
// Freed voice objects still in queue have 0x3F000000 (0.5f volume) in vtable
// field. SDL handles audio — this Xbox XAudio path is dead code. ~18K hits.
PPC_FUNC_HOOK(sub_82191858) {
    // No-op: XAudio voice queue processing not needed on PC/macOS (SDL audio).
}

// Fix 3: sub_828C9980 — already hooked at line 598 (Agent 11 added it)
// grcEffect::SetTextureSlot with null vtable identity check skipped.

// sub_82849918 — Yield/sleep function called each state machine iteration
extern "C" void __imp__sub_82849918(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_yieldCount{0};
PPC_FUNC_HOOK(sub_82849918) {
    uint32_t caller = (uint32_t)ctx.lr;
    __imp__sub_82849918(ctx, base);
    int n = s_yieldCount.fetch_add(1, std::memory_order_relaxed);
    if (n < 10 || (n % 1000) == 0) {
        printf("[YIELD] sub_82849918 #%d caller=0x%08X\n", n, caller);
        fflush(stdout);
    }
}

// sub_8224FA48 — Resource readiness check (called in state 3 and every iteration)
// Reads 0x82BF9B70: returns 0 when value is -1 (no dialog pending), 1 when >= 0.
// State 3 logic: return 0 → proceed through offset checks → state 4.
//                return 1 → jump to loc_82142504 → stays in state 3.
// Natural default of -1 is CORRECT — means "no XAM dialog pending, advance."
extern "C" void __imp__sub_8224FA48(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_resCheckCount{0};
PPC_FUNC_HOOK(sub_8224FA48) {
    __imp__sub_8224FA48(ctx, base);
    int n = s_resCheckCount.fetch_add(1, std::memory_order_relaxed);
    if (n < 20 || (n % 200) == 0) {
        printf("[RES-CHECK] sub_8224FA48 #%d = %d (0x82BF9B70=0x%08X)\n",
               n, ctx.r3.s32, PPC_LOAD_U32(0x82BF9B70));
        fflush(stdout);
    }
}

// sub_821B6FD0 — Multiplayer notification check (state 3 gate 3)
// Must return 0 for state 3 to proceed to offset checks.
extern "C" void __imp__sub_821B6FD0(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_mpNotifyCount{0};
PPC_FUNC_HOOK(sub_821B6FD0) {
    __imp__sub_821B6FD0(ctx, base);
    int n = s_mpNotifyCount.fetch_add(1, std::memory_order_relaxed);
    if (n < 20 || (n % 200) == 0) {
        printf("[MP-NOTIFY] sub_821B6FD0 #%d = %d (need 0 for state4)\n", n, ctx.r3.s32);
        fflush(stdout);
    }
}

// sub_8223CAD8 — Called at start of state 3
extern "C" void __imp__sub_8223CAD8(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_state3InitCount{0};
PPC_FUNC_HOOK(sub_8223CAD8) {
    int n = s_state3InitCount.fetch_add(1, std::memory_order_relaxed);
    if (n < 5) {
        printf("[STATE-3-INIT] sub_8223CAD8 #%d ENTER\n", n);
        fflush(stdout);
    }
    __imp__sub_8223CAD8(ctx, base);
    if (n < 5) {
        printf("[STATE-3-INIT] sub_8223CAD8 #%d RETURN\n", n);
        fflush(stdout);
    }
}

// sub_82219AC0 — Called with player struct in state 3 (checks byte 361)
extern "C" void __imp__sub_82219AC0(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_playerCheckCount{0};
PPC_FUNC_HOOK(sub_82219AC0) {
    int n = s_playerCheckCount.fetch_add(1, std::memory_order_relaxed);
    __imp__sub_82219AC0(ctx, base);
    if (n < 5 || (n % 500) == 0) {
        printf("[PLAYER-CHECK] sub_82219AC0 #%d r3=0x%08X\n", n, ctx.r3.u32);
        fflush(stdout);
    }
}

// sub_82254FE0 — XAM DIALOG COMPLETION (writes 1 to 0x82BF9B70 = "ready")
// This is the ONLY function that sets the resource readiness dword to a non-negative value.
// If this is never called, sub_8224FA48 always returns 0 and state 3 loops forever.
extern "C" void __imp__sub_82254FE0(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_82254FE0) {
    printf("[READY-SIGNAL] sub_82254FE0 ENTER — writing 1 to 0x82BF9B70! caller=0x%08X\n",
           static_cast<uint32_t>(ctx.lr));
    printf("[READY-SIGNAL] args: r3=0x%08X r4=0x%08X r5=0x%08X r6=0x%08X r7=0x%08X r8=0x%08X r9=0x%08X r10=0x%08X\n",
           ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r8.u32, ctx.r9.u32, ctx.r10.u32);
    fflush(stdout);
    __imp__sub_82254FE0(ctx, base);
    printf("[READY-SIGNAL] sub_82254FE0 RETURN — 0x82BF9B70 now = 0x%08X\n",
           PPC_LOAD_U32(0x82BF9B70));
    fflush(stdout);
}

// sub_8224FA38 — READY RESET (writes -1 to 0x82BF9B70 = "not ready")
extern "C" void __imp__sub_8224FA38(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_resetCount{0};
PPC_FUNC_HOOK(sub_8224FA38) {
    int n = s_resetCount.fetch_add(1, std::memory_order_relaxed);
    if (n < 10) {
        printf("[READY-RESET] sub_8224FA38 #%d — resetting 0x82BF9B70 to -1. caller=0x%08X\n",
               n, static_cast<uint32_t>(ctx.lr));
        fflush(stdout);
    }
    __imp__sub_8224FA38(ctx, base);
}

// sub_8214C8C8 — READY COUNTER (increments 0x82BF9B70 toward 4)
extern "C" void __imp__sub_8214C8C8(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_counterCount{0};
PPC_FUNC_HOOK(sub_8214C8C8) {
    uint32_t before = PPC_LOAD_U32(0x82BF9B70);
    __imp__sub_8214C8C8(ctx, base);
    uint32_t after = PPC_LOAD_U32(0x82BF9B70);
    int n = s_counterCount.fetch_add(1, std::memory_order_relaxed);
    if (n < 10 || before != after || (n % 500) == 0) {
        printf("[READY-COUNTER] sub_8214C8C8 #%d — 0x82BF9B70: 0x%08X -> 0x%08X\n",
               n, before, after);
        fflush(stdout);
    }
}

// sub_8223F9F0 — XAM DIALOG FLOW (big function that calls sub_82254FE0 many times)
extern "C" void __imp__sub_8223F9F0(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_xamFlowCount{0};
PPC_FUNC_HOOK(sub_8223F9F0) {
    int n = s_xamFlowCount.fetch_add(1, std::memory_order_relaxed);
    if (n < 5) {
        printf("[XAM-FLOW] sub_8223F9F0 #%d ENTER caller=0x%08X\n",
               n, static_cast<uint32_t>(ctx.lr));
        fflush(stdout);
    }
    __imp__sub_8223F9F0(ctx, base);
    if (n < 5) {
        printf("[XAM-FLOW] sub_8223F9F0 #%d RETURN r3=0x%08X\n", n, ctx.r3.u32);
        fflush(stdout);
    }
}

// sub_8214B168 — Called in sub_8214C8C8 after readiness check
extern "C" void __imp__sub_8214B168(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_postReadyCount{0};
PPC_FUNC_HOOK(sub_8214B168) {
    __imp__sub_8214B168(ctx, base);
    int n = s_postReadyCount.fetch_add(1, std::memory_order_relaxed);
    if (n < 5 || (n % 500) == 0) {
        printf("[POST-READY] sub_8214B168 #%d = %d\n", n, ctx.r3.s32);
        fflush(stdout);
    }
}

// =============================================================================
// MAIN ENTRY POINT
// =============================================================================
extern "C" void __imp__sub_8218BEA8(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__sub_82856F08(PPCContext &ctx, uint8_t *base);

PPC_FUNC_HOOK(sub_8218BEA8) {
  static bool s_initDone = false;

  if (!s_initDone) {
    LOG_WARNING("[MAIN] Running full game initialization...");
    __imp__sub_8218BEA8(ctx, base);
    s_initDone = true;
    LOG_WARNING("[MAIN] Initialization complete, entering render loop");
  }

  static int s_loopCount = 0;
  while (true) {
    ++s_loopCount;
    if (s_loopCount <= 20 || (s_loopCount % 500) == 0) {
        printf("[MAIN_LOOP] Iteration #%d\n", s_loopCount);
        fflush(stdout);
    }
    __imp__sub_82856F08(ctx, base);
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  }
}

// =============================================================================
// LINKER STUBS — Referenced by PPCFuncMappings in recompiled code.
// These symbols MUST exist at link time because recompiled code references
// them.  RexGlue's static libraries also define them via GUEST_FUNCTION_HOOK
// in its kernel .cpp files.  On macOS, -force_load pulls ALL symbols from
// LibertyRecompLib, so we use __attribute__((weak)) so the RexGlue strong
// definitions win at link time.
// =============================================================================
#define GUEST_FUNCTION_WEAK_STUB(subroutine) \
    extern "C" __attribute__((weak)) void subroutine(PPCContext& ctx, uint8_t* base) { }

GUEST_FUNCTION_WEAK_STUB(__imp__IoCheckShareAccess)
GUEST_FUNCTION_WEAK_STUB(__imp__IoCompleteRequest)
GUEST_FUNCTION_WEAK_STUB(__imp__IoDeleteDevice)
GUEST_FUNCTION_WEAK_STUB(__imp__IoDismountVolume)
GUEST_FUNCTION_WEAK_STUB(__imp__IoInvalidDeviceRequest)
GUEST_FUNCTION_WEAK_STUB(__imp__IoRemoveShareAccess)
GUEST_FUNCTION_WEAK_STUB(__imp__IoSetShareAccess)
GUEST_FUNCTION_WEAK_STUB(__imp__ObIsTitleObject)
GUEST_FUNCTION_WEAK_STUB(__imp__ObReferenceObject)
GUEST_FUNCTION_WEAK_STUB(__imp__RtlUpcaseUnicodeChar)
GUEST_FUNCTION_WEAK_STUB(__imp__XamEnableInactivityProcessing)
GUEST_FUNCTION_WEAK_STUB(__imp__XamResetInactivity)
GUEST_FUNCTION_WEAK_STUB(__imp__XamShowGamerCardUIForXUID)
GUEST_FUNCTION_WEAK_STUB(__imp__XamShowPlayerReviewUI)
GUEST_FUNCTION_WEAK_STUB(__imp__XamUserCreateStatsEnumerator)
// v8 new imports - not yet implemented in kernel lib
GUEST_FUNCTION_WEAK_STUB(__imp__XamShowMarketplaceUI)
GUEST_FUNCTION_WEAK_STUB(__imp__XamShowMarketplaceDownloadItemsUI)
GUEST_FUNCTION_WEAK_STUB(__imp__XNetLogonGetMachineID)
GUEST_FUNCTION_WEAK_STUB(__imp__XNetLogonGetTitleID)
GUEST_FUNCTION_WEAK_STUB(__imp__XamUserGetMembershipTierFromXUID)
GUEST_FUNCTION_WEAK_STUB(__imp__XamUserGetOnlineCountryFromXUID)

// sub_821966D0_hook — worker thread gate (suspend during init).
// In v8 the guest function at 0x821966D0 is mid-body (not a discrete entry),
// so InsertFunction() is a no-op and the pass-through is unreachable.
// Provide an inline stub to satisfy the linker.
extern "C" void __imp__sub_821966D0(PPCContext& /*ctx*/, uint8_t* /*base*/) { /* v8: mid-body addr, unreachable */ }
extern "C" void sub_821966D0_hook(PPCContext &ctx, uint8_t *base) {
  if (ShouldFailOpenWait()) return;
  __imp__sub_821966D0(ctx, base);
}

// =============================================================================
// GUEST_FUNCTION_HOOK REGISTRATIONS
// =============================================================================
// RexGlue handles ALL kernel exports. Liberty hooks ONLY frontend systems.
// =============================================================================

// --- Video / GPU ---
GUEST_FUNCTION_HOOK(__imp__XGetVideoMode, VdQueryVideoMode);
GUEST_FUNCTION_HOOK(__imp__VdPersistDisplay, VdPersistDisplay);
GUEST_FUNCTION_HOOK(__imp__VdSwap, VdSwap);
GUEST_FUNCTION_HOOK(__imp__VdGetSystemCommandBuffer, VdGetSystemCommandBuffer);
GUEST_FUNCTION_HOOK(__imp__VdEnableRingBufferRPtrWriteBack, VdEnableRingBufferRPtrWriteBack);
GUEST_FUNCTION_HOOK(__imp__VdInitializeRingBuffer, VdInitializeRingBuffer);
GUEST_FUNCTION_HOOK(__imp__VdSetSystemCommandBufferGpuIdentifierAddress, VdSetSystemCommandBufferGpuIdentifierAddress);
GUEST_FUNCTION_HOOK(__imp__VdShutdownEngines, VdShutdownEngines);
GUEST_FUNCTION_HOOK(__imp__VdQueryVideoMode, VdQueryVideoMode);
GUEST_FUNCTION_HOOK(__imp__VdGetCurrentDisplayInformation, VdGetCurrentDisplayInformation);
GUEST_FUNCTION_HOOK(__imp__VdSetDisplayMode, VdSetDisplayMode);
GUEST_FUNCTION_HOOK(__imp__VdSetGraphicsInterruptCallback, VdSetGraphicsInterruptCallback);
GUEST_FUNCTION_HOOK(__imp__VdInitializeEngines, VdInitializeEngines);
GUEST_FUNCTION_HOOK(__imp__VdIsHSIOTrainingSucceeded, VdIsHSIOTrainingSucceeded);
GUEST_FUNCTION_HOOK(__imp__VdGetCurrentDisplayGamma, VdGetCurrentDisplayGamma);
GUEST_FUNCTION_HOOK(__imp__VdQueryVideoFlags, VdQueryVideoFlags);
GUEST_FUNCTION_HOOK(__imp__VdCallGraphicsNotificationRoutines, VdCallGraphicsNotificationRoutines);
GUEST_FUNCTION_HOOK(__imp__VdInitializeScalerCommandBuffer, VdInitializeScalerCommandBuffer);
GUEST_FUNCTION_HOOK(__imp__VdRetrainEDRAM, VdRetrainEDRAM);
GUEST_FUNCTION_HOOK(__imp__VdRetrainEDRAMWorker, VdRetrainEDRAMWorker);
GUEST_FUNCTION_HOOK(__imp__VdEnableDisableClockGating, VdEnableDisableClockGating);
GUEST_FUNCTION_HOOK(__imp__VdGetGpuMemoryUsage, VdGetGpuMemoryUsage);
GUEST_FUNCTION_HOOK(__imp__VdSetGpuMemoryMode, VdSetGpuMemoryMode);
GUEST_FUNCTION_HOOK(__imp__VdGetSystemCommandBuffer2, VdGetSystemCommandBuffer2);
GUEST_FUNCTION_HOOK(__imp__VdSetSystemCommandBuffer2, VdSetSystemCommandBuffer2);
GUEST_FUNCTION_HOOK(__imp__VdGetDisplayInformation, VdGetDisplayInformation);
GUEST_FUNCTION_HOOK(__imp__VdSetDisplayConfiguration, VdSetDisplayConfiguration);
GUEST_FUNCTION_HOOK(__imp__VdPerformHardwareTest, VdPerformHardwareTest);
GUEST_FUNCTION_HOOK(__imp__VdGetHardwareStatus, VdGetHardwareStatus);
GUEST_FUNCTION_HOOK(__imp__VdSetOverlayMode, VdSetOverlayMode);
GUEST_FUNCTION_HOOK(__imp__VdGetOverlayInformation, VdGetOverlayInformation);

// --- Input ---
GUEST_FUNCTION_HOOK(__imp__XamInputGetCapabilities, XamInputGetCapabilities);
GUEST_FUNCTION_HOOK(__imp__XamInputGetState, XamInputGetState);
GUEST_FUNCTION_HOOK(__imp__XamInputSetState, XamInputSetState);
GUEST_FUNCTION_HOOK(__imp__XamInputGetKeystrokeEx, XamInputGetKeystrokeEx);

// --- Audio --- (RexGlue SDL AudioSystem handles all audio exports natively)

// --- Networking ---
GUEST_FUNCTION_HOOK(__imp__NetDll_WSAStartup, Net::WSAStartup);
GUEST_FUNCTION_HOOK(__imp__NetDll_WSACleanup, Net::WSACleanup);
GUEST_FUNCTION_HOOK(__imp__NetDll_socket, Net::Socket);
GUEST_FUNCTION_HOOK(__imp__NetDll_closesocket, Net::CloseSocket);
GUEST_FUNCTION_HOOK(__imp__NetDll_setsockopt, Net::SetSockOpt);
GUEST_FUNCTION_HOOK(__imp__NetDll_bind, Net::Bind);
GUEST_FUNCTION_HOOK(__imp__NetDll_connect, Net::Connect);
GUEST_FUNCTION_HOOK(__imp__NetDll_listen, Net::Listen);
GUEST_FUNCTION_HOOK(__imp__NetDll_accept, Net::Accept);
GUEST_FUNCTION_HOOK(__imp__NetDll_select, Net::Select);
GUEST_FUNCTION_HOOK(__imp__NetDll_recv, Net::Recv);
GUEST_FUNCTION_HOOK(__imp__NetDll_send, Net::Send);
GUEST_FUNCTION_HOOK(__imp__NetDll_inet_addr, Net::InetAddr);
GUEST_FUNCTION_HOOK(__imp__NetDll___WSAFDIsSet, NetDll___WSAFDIsSet);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetStartup, Net::XNetStartup);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetGetTitleXnAddr, Net::XNetGetTitleXnAddr);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetGetEthernetLinkStatus, Net::XNetGetEthernetLinkStatus);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetCleanup, Net::XNetCleanup);
GUEST_FUNCTION_HOOK(__imp__NetDll_getsockname, Net::GetSockName);
GUEST_FUNCTION_HOOK(__imp__NetDll_ioctlsocket, Net::IOCtlSocket);
GUEST_FUNCTION_HOOK(__imp__NetDll_sendto, Net::SendTo);
GUEST_FUNCTION_HOOK(__imp__NetDll_recvfrom, Net::RecvFrom);
GUEST_FUNCTION_HOOK(__imp__NetDll_shutdown, Net::Shutdown);
GUEST_FUNCTION_HOOK(__imp__NetDll_WSAGetLastError, Net::WSAGetLastError);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetQosListen, Net::XNetQosListen);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetQosLookup, Net::XNetQosLookup);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetQosRelease, Net::XNetQosRelease);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetServerToInAddr, Net::XNetServerToInAddr);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetXnAddrToInAddr, Net::XNetXnAddrToInAddr);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetGetConnectStatus, Net::XNetGetConnectStatus);
GUEST_FUNCTION_HOOK(__imp__NetDll_XNetUnregisterInAddr, Net::XNetUnregisterInAddr);
GUEST_FUNCTION_HOOK(__imp__XLiveBaseGetNatType, Net::XLiveBaseGetNatType);

// --- Voice ---
GUEST_FUNCTION_HOOK(__imp__XamVoiceCreate, XamVoiceCreate);
GUEST_FUNCTION_HOOK(__imp__XamVoiceClose, XamVoiceClose);
GUEST_FUNCTION_HOOK(__imp__XamVoiceHeadsetPresent, XamVoiceHeadsetPresent);
GUEST_FUNCTION_HOOK(__imp__XamVoiceSubmitPacket, XamVoiceSubmitPacket);

// --- Sessions ---
GUEST_FUNCTION_HOOK(__imp__XamSessionCreateHandle, Net::XamSessionCreateHandle);
GUEST_FUNCTION_HOOK(__imp__XamSessionRefObjByHandle, Net::XamSessionRefObjByHandle);

// --- Profile ---
GUEST_FUNCTION_HOOK(__imp__XamUserReadProfileSettings, XamUserReadProfileSettings);
