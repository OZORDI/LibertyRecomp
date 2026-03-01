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
// VBLANK TIMER
// =============================================================================
static std::atomic<bool> g_vblankThreadRunning{false};
static std::thread g_vblankThread;

void StartVBlankTimer() {
  if (g_vblankThreadRunning) return;
  g_vblankThreadRunning = true;
  printf("[StartVBlankTimer] Starting VBlank thread...\n");
  g_vblankThread = std::thread([]() {
    using namespace std::chrono;
    auto nextVBlank = steady_clock::now();
    constexpr auto VBLANK_INTERVAL = nanoseconds(16666667);
    uint32_t vblankCount = 0;
    while (g_vblankThreadRunning) {
      nextVBlank += VBLANK_INTERVAL;
      std::this_thread::sleep_until(nextVBlank);
      ++vblankCount;

      // Set frame-ready flag
      constexpr uint32_t GUEST_FRAME_READY_FLAG = 0x83128A80;
      if (g_memory.base) {
        *(g_memory.base + GUEST_FRAME_READY_FLAG) = 1;
      }

      if (g_gpuRingBuffer.interruptCallback != 0) {
        PPCFunc *callback = g_memory.FindFunction(g_gpuRingBuffer.interruptCallback);
        if (callback) {
          static PPCContext vblankCtx{};
          vblankCtx.r3.u32 = 0;
          vblankCtx.r4.u32 = g_gpuRingBuffer.interruptUserData;
          vblankCtx.r1.u64 = 0x80080000;
          vblankCtx.r13.u64 = 0x80000D20;
          SetPPCContext(vblankCtx);
          callback(vblankCtx, g_memory.base);
        }
      }
    }
  });
}

void StopVBlankTimer() {
  if (g_vblankThreadRunning) {
    g_vblankThreadRunning = false;
    if (g_vblankThread.joinable()) g_vblankThread.join();
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
// AUDIO STUBS (Liberty's audio pipeline handles these)
// =============================================================================
void XAudioGetVoiceCategoryVolume() { LOG_UTILITY("!!! STUB !!!"); }

uint32_t XAudioGetVoiceCategoryVolumeChangeMask(uint32_t Driver, be<uint32_t> *Mask) {
  *Mask = 0;
  return 0;
}

void XMAReleaseContext() { LOG_UTILITY("!!! STUB !!!"); }
void XMACreateContext() { LOG_UTILITY("!!! STUB !!!"); }

uint32_t XAudioGetSpeakerConfig(void *config) {
  LOG_UTILITY("!!! STUB !!!");
  return 0;
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
PPC_FUNC(sub_829DD978) {
  uint32_t ctx_addr = ctx.r3.u32;
  uint32_t write_ptr = PPC_LOAD_U32(ctx_addr + 56);
  PPC_STORE_U32(ctx_addr + 60, write_ptr);
  ctx.r3.u32 = 0;
}

// sub_829D72A0 - GPU Atomic Sync (no Xbox GPU hardware)
PPC_FUNC(sub_829D72A0) { /* r3 already contains result */ }

// sub_829D87E8 - GPU Sync bypass (no Xbox GPU spin loop)
PPC_FUNC(sub_829D87E8) {
  uint32_t deviceCtx = ctx.r3.u32;
  if (deviceCtx != 0) { PPC_STORE_U32(deviceCtx + 11000, 0); }
}

// sub_829CFED0 - GPU fence completion (force done after tight loop)
extern "C" void __imp__sub_829CFED0(PPCContext &ctx, uint8_t *base);
PPC_FUNC(sub_829CFED0) {
  static int s_consecutiveCalls = 0;
  ++s_consecutiveCalls;
  if (s_consecutiveCalls > 50) {
    s_consecutiveCalls = 0;
    ctx.r3.u32 = 0;
    return;
  }
  __imp__sub_829CFED0(ctx, base);
  if (ctx.r3.u32 == 0) s_consecutiveCalls = 0;
}

// sub_828507F8 - Frame presentation (fix throttle check)
extern "C" void __imp__sub_828507F8(PPCContext &ctx, uint8_t *base);
PPC_FUNC(sub_828507F8) {
  PPC_STORE_U32(0x83124CCC, 0);
  __imp__sub_828507F8(ctx, base);
}

// sub_829A0678 - HDCP bypass (PC has no HDCP)
extern "C" void __imp__sub_829A0678(PPCContext &ctx, uint8_t *base);
PPC_FUNC(sub_829A0678) { ctx.r3.u32 = 0; }

// sub_829D4C48 - Frame swap stub (would SIGBUS without GPU context)
extern "C" void __imp__sub_829D4C48(PPCContext &ctx, uint8_t *base);
PPC_FUNC(sub_829D4C48) { return; }

// sub_829D7368 - VBlank callback (skip Xbox GPU fence queue)
extern "C" void __imp__sub_829D7368(PPCContext &ctx, uint8_t *base);
PPC_FUNC(sub_829D7368) {
  uint32_t gpuContext = ReadGpuContextFromGlobal();
  if (gpuContext == 0) return;
  if (ctx.r3.u32 == 0) return; // Skip Xbox VBlank queue - PC uses Video::Present
  ctx.r4.u32 = gpuContext;
  __imp__sub_829D7368(ctx, base);
}

// sub_82871180 - GPU render state submission (accesses D3D device context at
// dword_83124AF4 which contains uncommitted GPU command buffer pointers in the
// 0x70xxxxxx range, causing SIGBUS). Pure hardware-bound Xbox 360 D3D code.
PPC_FUNC(sub_82871180) { ctx.r3.u32 = 0; }

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
PPC_FUNC(sub_827DF248) {
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
PPC_FUNC(sub_829A2540) {
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
PPC_FUNC(sub_82168C08) {
  __imp__sub_82168C08(ctx, base);
  SignalEventByGuestAddr(0x83130044);
  SignalEventByGuestAddr(0x83137B80);
  SignalSemaphoreByGuestAddr(0x83130008, 6);
}

// sub_82169B00 - Audio thread sync (Xbox worker model not needed on PC)
PPC_FUNC(sub_82169B00) { ctx.r3.u32 = 0; }

// sub_82169400 - Audio worker thread (not needed on PC, SDL handles audio)
PPC_FUNC(sub_82169400) { ctx.r3.u32 = 0; }

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
PPC_FUNC(sub_827D85E0) {
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
PPC_FUNC(sub_827D8620) {
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
PPC_FUNC(sub_8218BE28) {
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
PPC_FUNC(sub_8218BE50) {
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
extern "C" void __imp__sub_827DAE40(PPCContext &ctx, uint8_t *base);
static std::atomic<int> s_tlsGuardCount{0};

PPC_FUNC(sub_827DAE40) {
    __imp__sub_827DAE40(ctx, base);
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

PPC_FUNC(sub_825EE000) {
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

PPC_FUNC(sub_822054F8) {
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

PPC_FUNC(sub_827FC7F0) {
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

PPC_FUNC(sub_827E0898) {
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
// MAIN ENTRY POINT
// =============================================================================
extern "C" void __imp__sub_8218BEA8(PPCContext &ctx, uint8_t *base);
extern "C" void __imp__sub_82856F08(PPCContext &ctx, uint8_t *base);

PPC_FUNC(sub_8218BEA8) {
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

// sub_821966D0_hook — worker thread gate (suspend during init, pass-through at runtime)
extern "C" void __imp__sub_821966D0(PPCContext &ctx, uint8_t *base);
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

// --- Audio ---
GUEST_FUNCTION_HOOK(__imp__XAudioGetVoiceCategoryVolume, XAudioGetVoiceCategoryVolume);
GUEST_FUNCTION_HOOK(__imp__XAudioGetVoiceCategoryVolumeChangeMask, XAudioGetVoiceCategoryVolumeChangeMask);
GUEST_FUNCTION_HOOK(__imp__XMAReleaseContext, XMAReleaseContext);
GUEST_FUNCTION_HOOK(__imp__XMACreateContext, XMACreateContext);
GUEST_FUNCTION_HOOK(__imp__XAudioRegisterRenderDriverClient, XAudioRegisterRenderDriverClient);
GUEST_FUNCTION_HOOK(__imp__XAudioUnregisterRenderDriverClient, XAudioUnregisterRenderDriverClient);
GUEST_FUNCTION_HOOK(__imp__XAudioSubmitRenderDriverFrame, XAudioSubmitRenderDriverFrame);
GUEST_FUNCTION_HOOK(__imp__XAudioGetSpeakerConfig, XAudioGetSpeakerConfig);

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
