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

// sub_829D4C48 - Frame swap stub (would SIGBUS without GPU context)
extern "C" void __imp__sub_82A46098(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_82A46098) { return; }

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
// DIAGNOSTIC: sub_82A50890 — GPU CreateDevice (top-level GPU init)
// =============================================================================
extern "C" void __imp__sub_82A50890(PPCContext &ctx, uint8_t *base);
PPC_FUNC_HOOK(sub_82A50890) {
    printf("[GPU-CREATE] sub_82A50890 ENTER r3=0x%08X r4=0x%08X\n",
           ctx.r3.u32, ctx.r4.u32);
    fflush(stdout);
    __imp__sub_82A50890(ctx, base);
    printf("[GPU-CREATE] sub_82A50890 EXIT  result=0x%08X\n", ctx.r3.u32);
    fflush(stdout);
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
    __imp__sub_8285B088(ctx, base);
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
