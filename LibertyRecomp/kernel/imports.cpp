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
#include <SDL3/SDL.h>
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
#include <chrono>
#include <ui/game_window.h>
#include <unordered_map>
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

// Kernel sync helpers (shared via kernel_sync.h)
#include "kernel_sync.h"

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
      if (event.type == SDL_EVENT_QUIT) { std::_Exit(0); }
    }
  }
}

// =============================================================================
// REXGLUE SYNC SIGNALING HELPERS
// =============================================================================
void SignalEventByGuestAddr(uint32_t guestAddr) {
  auto* ks = rex::system::kernel_state();
  if (!ks) return;
  void* ptr = g_memory.Translate(guestAddr);
  if (!ptr) return;
  auto ev = rex::system::XObject::GetNativeObject<rex::system::XEvent>(ks, ptr);
  if (ev) { ev->Set(0, false); }
}

void SignalSemaphoreByGuestAddr(uint32_t guestAddr, int32_t count) {
  auto* ks = rex::system::kernel_state();
  if (!ks) return;
  void* ptr = g_memory.Translate(guestAddr);
  if (!ptr) return;
  auto sem = rex::system::XObject::GetNativeObject<rex::system::XSemaphore>(ks, ptr);
  if (sem) { sem->ReleaseSemaphore(count, nullptr); }
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
    SDL_FlushEvents(SDL_EVENT_FIRST, SDL_EVENT_LAST);
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
  if (!ks || !ks->function_dispatcher()) {
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
              ks->function_dispatcher()->ExecuteInterrupt(
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

// sub_827DF248 - pgStreamer::Init
// REMOVED: Old sync-mode force hook superseded by streaming_async_patches.cpp
// which sets sync flag for InitInternal (skip Xbox threads), starts host worker
// pool, then clears runtime sync flag so submissions go through async path.
// See streaming_async_patches.cpp PPC_FUNC_HOOK(sub_827DF248) for the replacement.

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

// sub_82169B00 - Audio thread sync (Xbox worker model not needed on PC)
PPC_FUNC_HOOK(sub_82169B00) { ctx.r3.u32 = 0; }

// sub_82169400 - Audio worker thread (not needed on PC, SDL handles audio)
PPC_FUNC_HOOK(sub_82169400) { ctx.r3.u32 = 0; }

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
// sub_82A10EB0 — RAGE heap allocator. Removed override — let recompiled code
// handle allocation through its own RtlAllocateHeap → rexcrt path.

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


// sub_8285A8B0 — GPU buffer flush called from sub_8285B088.
// Dispatches two GPU device vtable calls (slot 9 and slot 13) to "submit shader bytecode
// to the Xenos command ring". Both spin-wait on GPU fence completion → hang forever.
// Since we bypass the Xenos GPU entirely, this flush is a no-op for us.
extern "C" void __imp__sub_8285A8B0(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_8285A8B0) {
    // Skip GPU shader bytecode submission — no Xenos hardware to submit to.
    // The caller (sub_8285B088) will still do the slot cleanup writes afterwards.
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
extern "C" void __imp__sub_822440F8(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_822440F8) {
    static bool s_logged = false;

    // Ensure player/episode index is set to 0 (base game)
    uint32_t playerIdx = PPC_LOAD_U32(0x82A95478);
    if (playerIdx == 0xFFFFFFFF) {
        PPC_STORE_U32(0x82A95478, 0);
    }

    if (!s_logged) {
        s_logged = true;
        printf("[STATE-4-INNER] BYPASS: returning 2 (no-save success). "
               "playerIdx=0x%08X, profileIdx=0x%08X\n",
               PPC_LOAD_U32(0x82A95478), PPC_LOAD_U32(0x82A95474));
        fflush(stdout);
    }

    // Return 2 = success (outer state machine sets r29=5, advancing to game start)
    ctx.r3.u64 = 2;
}

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

// =============================================================================
// sub_82A00DC0 — ALIGNMENT-AWARE MEMCPY (582 call sites)
// =============================================================================
// This is a byte-level memcpy with alignment optimization: r3=dst, r4=src, r5=count.
// The GPU/rendering function sub_8285AE20 passes a corrupted count of 0xFFFFFFDB
// (~4GB unsigned) due to uninitialized GPU state. The memcpy then writes sequentially
// through memory, hitting stack guard pages in the 0x70000000-0x7F000000 range
// (which the guard handler blindly unprotects), causing what APPEARS to be a stack
// overflow but is actually a runaway memcpy with a corrupt length.
//
// Fix: Replace with native memcpy + size sanity check. Native memcpy is also
// significantly faster than the byte-level recompiled PPC version.
extern "C" void __imp__sub_82A00DC0(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_82A00DC0) {
    uint32_t dst = ctx.r3.u32;
    uint32_t src = ctx.r4.u32;
    uint32_t count = ctx.r5.u32;

    // Sanity check: reject obviously corrupt lengths (> 256MB)
    // No legitimate memcpy in GTA IV should exceed this.
    constexpr uint32_t kMaxReasonableSize = 256 * 1024 * 1024;
    if (count > kMaxReasonableSize) {
        static int s_warnCount = 0;
        if (s_warnCount++ < 10) {
            printf("[MEMCPY-GUARD] sub_82A00DC0: rejecting corrupt count=0x%08X (%u) "
                   "dst=0x%08X src=0x%08X\n", count, count, dst, src);
            fflush(stdout);
        }
        // Return dst (standard memcpy behavior) without copying
        ctx.r3.u64 = dst;
        return;
    }

    // Use native memcpy for speed and correctness
    if (count > 0 && dst != 0 && src != 0) {
        std::memcpy(reinterpret_cast<void*>(base + dst),
                    reinterpret_cast<const void*>(base + src), count);
    }
    // Restore original dst in r3 (memcpy returns dst — saved at -8(r1) by prologue)
    ctx.r3.u64 = PPC_LOAD_U64(ctx.r1.u32 + -8);
}

// =============================================================================
// AUDIO RENDER THREAD SUBSYSTEM — Complete Decompilation
// =============================================================================
//
// Five functions comprise GTA IV's audio render pump on Xbox 360:
//
//   sub_821910D0  Render pump (thread entry, per-frame)
//   sub_82191228  Voice volume update + queue processing + callback dispatch
//   sub_8218FFB0  Double-buffer drain (swap active/free voice queue lists)
//   sub_82191858  Voice dequeue from linked list with spinlock
//   sub_82212EC0  XAudio device connection state machine
//
// On Xbox 360, XAudioRenderDriverInitialize populates driver+64 with a valid
// IXAudioEndpoint* whose vtable[17] (offset 68) triggers DMA to the audio
// hardware. In the recomp, SDL2 handles audio output natively via a callback
// thread started by XAudioRegisterRenderDriverClient. The endpoint pointer at
// driver+64 is never initialized, so:
//
//   1. sub_821910D0's call to endpoint->vtable[17]() reads garbage (0x000F4000)
//   2. sub_82191228 dereferences endpoint->vtable[17]+24 for XAudio volume APIs
//
// Both crash. We hook sub_821910D0 and sub_82191228 with complete replacements
// that preserve all game-internal state (critsec, timestamp, double-buffer swap,
// voice queue processing, callback dispatch) while skipping endpoint-dependent
// code. sub_8218FFB0 and sub_82191858 run as recompiled PPC — they only touch
// voice queue linked lists and spinlocks, which are safe with empty queues and
// valid with game-allocated voice objects. sub_82212EC0 is hooked to force the
// device into "connected" state since the Xbox async enumeration never fires.
//
// Layout of key globals (verified via lis/addi in PPC scaffolds):
//   0x82B2833C  RTL_CRITICAL_SECTION — audio render critsec
//   0x831D53EC  uint32_t* — pointer to audDevice object (indirect)
//   0x831D53F0  uint32_t[7] — frame counter buffer A (accumulated)
//   0x831D540C  uint32_t[7] — frame counter buffer B (swapped snapshot)
//   0x831D52CC  KEVENT — wait object 0 (shutdown)
//   0x831D5310  KEVENT — wait object 1 (render signal)
//   0x831D52DC  KSEMAPHORE — multi-client synchronization
//   0x831D52F0  KEVENT — render wake event (signaled each frame)
//   0x831E4DB8  Recursive spinlock { KSPIN_LOCK, refcount, owner, savedIRQL }
//
// audDevice layout (offsets verified from Hex-Rays pseudocode):
//   +0x040 (64)   IXAudioEndpoint* pEndpoint (INVALID on PC — never dereference)
//   +0x050 (80)   Primary voice queue (44 bytes: linked list + metadata)
//   +0x07C (124)  uint32_t* pExtraVoices (allocated array, 44 bytes each)
//   +0x080 (128)  uint8_t numExtraVoices
//   +0x084 (132)  float categoryVolumes[2] (effects, music)
//   +0x08C (140)  uint32_t volumeChangeMask
//   +0x0A8 (168)  Callback slots: 2 banks x 8 slots x {funcPtr, argPtr} (8B each)
//                 Bank 0: +0x0A8..+0x0E7 (pre-frame, dispatched when flag=0)
//                 Bank 1: +0x0E8..+0x127 (post-frame, dispatched when flag=1)
//   +0x128 (296)  uint32_t currentCallbackPtr (iteration state)
//   +0x12C (300)  uint32_t timestamp (written from TLS r13+256)
//   +0x130 (304)  uint32_t numWorkerThreads (0 = single-client render path)
//   +0x164 (356)  Render status bytes (indexed by TLS r13+268, 8B each)
// =============================================================================

// --- sub_8218FFB0 and sub_82191858 run as recompiled PPC (no hook needed) ---
// sub_8218FFB0: Raises IRQL, acquires recursive spinlock at 0x831E4DB8,
//   calls sub_82190120 on each voice queue (swaps active/free list pointers),
//   releases spinlock.  Safe: only touches voice queue list pointers.
//
// sub_82191858: Acquires same spinlock, dequeues voices from linked list at
//   a2+36, relinks into free list at a2+40, calls voice->vtable[17]() for
//   each dequeued voice.  If queue is empty, returns immediately.  Voice
//   vtables are game-allocated (valid), not hardware endpoint vtables.

// --- Forward declarations for recompiled PPC functions we call ---
extern "C" void __imp__sub_821910D0(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__sub_82191228(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__sub_8218FFB0(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__sub_82191858(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__sub_82191360(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__sub_821916D8(PPCContext &ctx, uint8_t *base);

// =============================================================================
// sub_82191228 — Voice volume + queue processing + callback dispatch
// =============================================================================
// Original flow (pseudocode from IDA):
//   if (flag) {
//     sub_821916D8(driver, 0);                     // dispatch pre-frame callbacks
//     handle = *(**(driver+64)+68)+24;              // <-- CRASHES: endpoint deref
//     XAudioGetVoiceCategoryVolumeChangeMask(handle, &driver+140);
//     for (cat = 0; cat < 2; cat++)
//       if (changeMask & (1<<cat))
//         XAudioGetVoiceCategoryVolume(cat, &driver+132+cat*4);
//     if (driver+304 > 1) KeReleaseSemaphore(sem, 1, count-1, 0);
//   }
//   sub_82191858(driver, driver+80);               // process primary voice queue
//   sub_82191360(driver, driver+356);              // update primary render status
//   for (i = 0; i < driver+128; i++) {             // process additional queues
//     sub_82191858(driver, *(driver+124) + i*44);
//     sub_82191360(driver, driver+356 + 8*(toggle^=1));
//   }
//   if (flag) {
//     driver+140 = 0;                              // clear change mask
//     sub_821916D8(driver, 1);                     // dispatch post-frame callbacks
//   }
//
// Hook: Skip endpoint dereference and XAudio volume API calls.  Keep everything
// else — callback dispatch, voice queue processing, render status updates.
// Volume categories are set to 1.0f (full volume) by the kernel stub anyway.
// =============================================================================
PPC_FUNC_HOOK(sub_82191228) {
    uint32_t driver = ctx.r3.u32;
    uint8_t  flag   = ctx.r4.u8;

    if (driver == 0) return;

    // --- Pre-frame callback dispatch (bank 0) ---
    if (flag) {
        ctx.r3.u32 = driver;
        ctx.r4.s64 = 0;  // bank 0
        __imp__sub_821916D8(ctx, base);

        // SKIP: XAudioGetVoiceCategoryVolumeChangeMask + XAudioGetVoiceCategoryVolume
        // These dereference the endpoint at driver+64 which is uninitialized.
        // The kernel stubs always return volume=1.0f and changeMask=0, so we
        // just ensure the driver's cached volumes are 1.0f (full passthrough).
        // Write 1.0f (big-endian) to voice category volume slots.
        constexpr uint32_t FLOAT_1_0_BE = 0x3F800000;  // 1.0f in IEEE 754
        PPC_STORE_U32(driver + 132, FLOAT_1_0_BE);     // effects volume
        PPC_STORE_U32(driver + 136, FLOAT_1_0_BE);     // music volume

        // SKIP: KeReleaseSemaphore — only needed for multi-client synchronization
        // which doesn't apply when driver+304 is 0 (normal single-client path).
    }

    // --- Process primary voice queue ---
    ctx.r3.u32 = driver;
    ctx.r4.u32 = driver + 80;
    __imp__sub_82191858(ctx, base);

    // --- Update primary render status ---
    ctx.r3.u32 = driver;
    ctx.r4.u32 = driver + 356;
    __imp__sub_82191360(ctx, base);

    // --- Process additional voice queues ---
    uint8_t extraCount = PPC_LOAD_U8(driver + 128);
    if (extraCount > 0) {
        uint32_t extraBase = PPC_LOAD_U32(driver + 124);
        uint32_t toggle = 1;  // starts at 1, XORs with 1 each iteration
        for (uint32_t i = 0; i < extraCount; i++) {
            ctx.r3.u32 = driver;
            ctx.r4.u32 = extraBase + i * 44;
            __imp__sub_82191858(ctx, base);

            ctx.r3.u32 = driver;
            ctx.r4.u32 = driver + 356 + toggle * 8;
            __imp__sub_82191360(ctx, base);

            toggle ^= 1;
        }
    }

    // --- Post-frame callback dispatch (bank 1) + clear change mask ---
    if (flag) {
        PPC_STORE_U32(driver + 140, 0);  // clear volume change mask
        ctx.r3.u32 = driver;
        ctx.r4.s64 = 1;  // bank 1
        __imp__sub_821916D8(ctx, base);
    }
}

// =============================================================================
// sub_821910D0 — Audio Render Thread Pump (per-frame entry point)
// =============================================================================
// Original flow (pseudocode from IDA):
//   RtlEnterCriticalSection(0x82B2833C);
//   driver = *(0x831D53EC);
//   driver+300 = TLS[r13+256];           // store timestamp
//   if (driver+304 != 0) {
//     KeSetEvent(0x831D52F0, 1, 0);
//     KeWaitForMultipleObjects(2, {0x831D52CC, 0x831D5310}, WaitAny, ...);
//     wasShutdown = (result == 1);
//   } else {
//     sub_8218FFB0(driver);              // drain voice queue double-buffers
//     sub_82191228(driver, 1);           // volume + queues + callbacks
//   }
//   if (!wasShutdown) {
//     endpoint = *(driver+64);           // <-- CRASHES: garbage vtable
//     if (endpoint->vtable[17](endpoint) >= 0)
//       atomic_inc(0x831D53F0);          // frame counter
//   }
//   driver+300 = 0;                      // clear timestamp
//   for (i=0; i<7; i++) {                // double-buffer swap
//     bufB[i] = bufA[i]; bufA[i] = 0;
//   }
//   RtlLeaveCriticalSection(0x82B2833C);
//   return 0;
//
// Hook: Complete replacement.  Calls sub_8218FFB0 and our hooked sub_82191228
// for full voice queue processing.  Skips endpoint vtable[17] dispatch (audio
// output is handled by the SDL2 callback thread).  Preserves critsec, timestamp,
// double-buffer swap, and shutdown/multi-client event path.
// =============================================================================
PPC_FUNC_HOOK(sub_821910D0) {
    constexpr uint32_t CRITSEC       = 0x82B2833C;
    constexpr uint32_t DRIVER_PTR    = 0x831D53EC;
    constexpr uint32_t BUFFER_A      = 0x831D53F0;
    constexpr uint32_t BUFFER_B      = 0x831D540C;
    constexpr uint32_t EVENT_RENDER  = 0x831D52F0;
    constexpr uint32_t WAIT_OBJ_0   = 0x831D52CC;
    constexpr uint32_t WAIT_OBJ_1   = 0x831D5310;

    bool wasShutdown = false;

    // 1. Enter critical section
    ctx.r3.u32 = CRITSEC;
    __imp__RtlEnterCriticalSection(ctx, base);

    // 2. Load driver pointer
    uint32_t driver = PPC_LOAD_U32(DRIVER_PTR);
    if (driver == 0) {
        ctx.r3.u32 = CRITSEC;
        __imp__RtlLeaveCriticalSection(ctx, base);
        ctx.r3.s64 = 0;
        return;
    }

    // 3. Store timestamp from TLS
    uint32_t timestamp = PPC_LOAD_U32(ctx.r13.u32 + 256);
    PPC_STORE_U32(driver + 300, timestamp);

    // 4. Check connection/thread count
    uint32_t connCount = PPC_LOAD_U32(driver + 304);
    if (connCount != 0) {
        // Multi-client path: signal render event and wait for shutdown or render
        ctx.r3.u32 = EVENT_RENDER;
        ctx.r4.s64 = 1;    // increment
        ctx.r5.s64 = 0;    // wait = FALSE
        __imp__KeSetEvent(ctx, base);

        // Set up wait block on the stack (original uses sp+100, 44 bytes zeroed)
        // KeWaitForMultipleObjects(2, {obj0, obj1}, WaitAny=1, Executive=3,
        //                          KernelMode=1, Alertable=0, Timeout=0, WaitBlock)
        // We store the two object pointers and a zeroed wait block area.
        uint32_t sp = ctx.r1.u32;
        PPC_STORE_U32(sp + 80, WAIT_OBJ_0);
        PPC_STORE_U32(sp + 84, WAIT_OBJ_1);
        PPC_STORE_U32(sp + 96, 0);  // WaitBlockArray index init

        // Zero 44 bytes of wait block (sp+100..sp+143)
        for (uint32_t i = 0; i < 44; i += 4)
            PPC_STORE_U32(sp + 100 + i, 0);

        ctx.r3.s64  = 2;           // count
        ctx.r4.u32  = sp + 80;     // object array
        ctx.r5.s64  = 1;           // WaitAny
        ctx.r6.s64  = 3;           // Executive
        ctx.r7.s64  = 1;           // KernelMode
        ctx.r8.s64  = 0;           // Alertable = FALSE
        ctx.r9.s64  = 0;           // Timeout = NULL (infinite)
        ctx.r10.u32 = sp + 96;     // WaitBlockArray
        __imp__KeWaitForMultipleObjects(ctx, base);

        wasShutdown = (ctx.r3.s32 == 1);
    } else {
        // Normal single-client path: drain buffers and process voices
        ctx.r3.u32 = driver;
        __imp__sub_8218FFB0(ctx, base);

        ctx.r3.u32 = driver;
        ctx.r4.s64 = 1;  // flag = true (do volume + callbacks)
        sub_82191228(ctx, base);  // calls our hooked version
    }

    // 5. Frame counter increment (skip endpoint vtable[17] dispatch)
    // On Xbox 360, endpoint->vtable[17]() triggers DMA and returns >= 0 on
    // success. We skip the endpoint call entirely (SDL2 handles output) but
    // still increment the frame counter to keep the game's timing correct.
    if (!wasShutdown) {
        // Atomic increment of BUFFER_A[0] — matches original lwarx/stwcx loop
        uint32_t old = PPC_LOAD_U32(BUFFER_A);
        PPC_STORE_U32(BUFFER_A, old + 1);
    }

    // 6. Clear timestamp
    PPC_STORE_U32(driver + 300, 0);

    // 7. Double-buffer swap: copy A[i] -> B[i], zero A[i]
    for (uint32_t i = 0; i < 7; i++) {
        uint32_t off = i * 4;
        uint32_t val = PPC_LOAD_U32(BUFFER_A + off);
        PPC_STORE_U32(BUFFER_A + off, 0);
        PPC_STORE_U32(BUFFER_B + off, val);
    }

    // 8. Leave critical section
    ctx.r3.u32 = CRITSEC;
    __imp__RtlLeaveCriticalSection(ctx, base);

    // 9. Return 0
    ctx.r3.s64 = 0;
}

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

// =============================================================================
// sub_82212EC0 — Audio device connection state machine
// =============================================================================
// Called every 100ms from sub_82212F38's yield loop (the network/audio service
// thread).  Original flow (pseudocode from IDA):
//
//   if (struct+2016 == 1) {           // state: connecting
//     sub_8220FDB8(struct);           // async Xbox device enumeration
//     if (struct+2016 != 1) {         // state changed (connected or error)
//       if (struct+288 > 0)           // endpoint count from enumeration
//         connected = sub_82211B38(struct);  // bind to first endpoint
//       else
//         connected = false;
//       struct+2024 = (connected<<7) | (struct+2024 & 0x7F);  // set bit 7
//     }
//   }
//
// sub_8220FDB8 uses NtDeviceIoControlFile to enumerate XAudio endpoints via
// an Xbox kernel async callback. That callback never fires in the recomp
// (no XAudio hardware), so struct+2016 stays at 1 forever and the caller
// loops.  sub_82211B38 then hashes an endpoint GUID and selects an audio
// format — also Xbox-specific.
//
// Fix: On first call, force struct+2016=0 (connected) and struct+288=1
// (one endpoint found).  Then set the "connected" flag at struct+2024
// bit 7.  Skip the original entirely — both callees depend on Xbox I/O.
// =============================================================================
extern "C" void __imp__sub_82212EC0(PPCContext &ctx, uint8_t *base);
static std::atomic<bool> s_audioDeviceFixed{false};
PPC_FUNC_HOOK(sub_82212EC0) {
    uint32_t structPtr = ctx.r3.u32;
    if (structPtr == 0) return;

    if (!s_audioDeviceFixed.exchange(true, std::memory_order_relaxed)) {
        // Force device into connected state
        PPC_STORE_U32(structPtr + 2016, 0);   // state: connected (not connecting)
        PPC_STORE_U32(structPtr + 288, 1);    // endpoint count: 1

        // Set "connected" flag: bit 7 of byte at struct+2024
        uint8_t flags = PPC_LOAD_U8(structPtr + 2024);
        PPC_STORE_U8(structPtr + 2024, flags | 0x80);
    }
    // Skip original — sub_8220FDB8 and sub_82211B38 require Xbox async I/O
}

// sub_82849918 — Sleep thunk: tail-calls sub_82A12B60 (Sleep(r3 ms))
// 31 callers. Replaced with native sleep to avoid __imp__ stack overflow.
PPC_FUNC_HOOK(sub_82849918) {
    uint32_t ms = ctx.r3.u32;
    if (ms == 0) std::this_thread::yield();
    else std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    ctx.r3.s64 = 0; // STATUS_SUCCESS
}

// sub_82849910 — Sleep(0) thunk: hardcodes r3=0, tail-calls sub_82A12B60
// Replace with native yield to prevent stack overflow from tail-call chain.
PPC_FUNC_HOOK(sub_82849910) {
    std::this_thread::yield();
    ctx.r3.s64 = 0;
}

// sub_82A7A070 — Sleep(0) thunk: sets r3=0, tail-calls sub_82A12B60
PPC_FUNC_HOOK(sub_82A7A070) {
    std::this_thread::yield();
    ctx.r3.s64 = 0;
}


// sub_82A12B60 — Sleep wrapper: r3=ms, sets alertable=false, tail-calls sub_82A1A200
// Replace with native sleep to prevent stack overflow from tail-call chain.
PPC_FUNC_HOOK(sub_82A12B60) {
    uint32_t ms = ctx.r3.u32;
    if (ms == 0) std::this_thread::yield();
    else std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    ctx.r3.s64 = 0;
}


// NOTE: grmSetup dispatch thunks (sub_821B3958/3970/38D8) moved to
// patches/grm_setup_patches.cpp — full decompilation with proper null-guards.
// NOTE: Audio object vtable guards (sub_829158F0/82915840) moved to
// patches/scene_tick_patches.cpp — full decompilation with entity lifecycle.

// =============================================================================
// MAIN ENTRY POINT
// =============================================================================
extern "C" void __imp__sub_8218BEA8(PPCContext &ctx, uint8_t *base);

// __imp__sub_82856F08 was never a real function boundary in the XEX — the
// address 0x82856F08 falls inside another function. Provide a no-op stub so
// the per-frame loop hook below can reference it without a linker error.
extern "C" void __imp__sub_82856F08(PPCContext &ctx, uint8_t *base) {
    (void)ctx; (void)base;
}

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
// XAM functions now handled by rexglue — weak stubs for link-time resolution
GUEST_FUNCTION_WEAK_STUB(__imp__XamUserReadProfileSettings)
GUEST_FUNCTION_WEAK_STUB(__imp__XamSessionCreateHandle)
GUEST_FUNCTION_WEAK_STUB(__imp__XamSessionRefObjByHandle)
GUEST_FUNCTION_WEAK_STUB(__imp__XamUserGetSigninState)
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

// --- Sessions + Profile ---
// Removed: XamSessionCreateHandle, XamSessionRefObjByHandle, XamUserReadProfileSettings
// These are now handled by rexkernel (xam_user.cpp) — let rexglue run uninterrupted.
