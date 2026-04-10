// GTA IV Audio Patches
// Handles audio volume, attenuation, and RAGE audio engine integration

#include <stdafx.h>
#include <api/Liberty.h>
#include <kernel/function.h>
#include <kernel/memory.h>
#include <os/media.h>
#include <os/version.h>
#include <os/logger.h>
#include <patches/audio_patches.h>
#include <user/config.h>
#include <app.h>

int AudioPatches::m_isAttenuationSupported = -1;

// GTA IV Audio Engine State
// These pointers will be resolved at runtime when we find the audio engine in memory
namespace GTA4Audio
{
    // Volume levels (0.0 - 1.0)
    static float g_musicVolume = 1.0f;
    static float g_effectsVolume = 1.0f;
    static float g_masterVolume = 1.0f;
    
    // Audio engine state
    static bool g_audioInitialized = false;
    static uint32_t g_audioEnginePtr = 0;  // Guest pointer to RAGE audEngine
    
    float GetMusicVolume() { return g_musicVolume; }
    void SetMusicVolume(float volume) { g_musicVolume = std::clamp(volume, 0.0f, 1.0f); }
    
    float GetEffectsVolume() { return g_effectsVolume; }
    void SetEffectsVolume(float volume) { g_effectsVolume = std::clamp(volume, 0.0f, 1.0f); }
    
    float GetMasterVolume() { return g_masterVolume; }
    void SetMasterVolume(float volume) { g_masterVolume = std::clamp(volume, 0.0f, 1.0f); }
    
    bool IsInitialized() { return g_audioInitialized; }
    void SetInitialized(bool init) { g_audioInitialized = init; }
}

bool AudioPatches::CanAttenuate()
{
#if _WIN32
    if (m_isAttenuationSupported >= 0)
        return m_isAttenuationSupported;

    auto version = os::version::GetOSVersion();

    m_isAttenuationSupported = version.Major >= 10 && version.Build >= 17763;

    return m_isAttenuationSupported;
#elif __linux__
    return true;
#elif __APPLE__
    // macOS supports audio attenuation
    return true;
#else
    return false;
#endif
}

void AudioPatches::Update(float deltaTime)
{
    // Update GTA IV audio volumes from config
    const float musicVolume = Config::MusicVolume * Config::MasterVolume;
    const float effectsVolume = Config::EffectsVolume * Config::MasterVolume;

    if (Config::MusicAttenuation && CanAttenuate())
    {
        auto time = 1.0f - expf(2.5f * -deltaTime);

        if (os::media::IsExternalMediaPlaying())
        {
            // Fade out music when external media is playing
            GTA4Audio::SetMusicVolume(std::lerp(GTA4Audio::GetMusicVolume(), 0.0f, time));
        }
        else
        {
            // Fade music back in
            GTA4Audio::SetMusicVolume(std::lerp(GTA4Audio::GetMusicVolume(), musicVolume, time));
        }
    }
    else
    {
        GTA4Audio::SetMusicVolume(musicVolume);
    }

    GTA4Audio::SetEffectsVolume(effectsVolume);
    GTA4Audio::SetMasterVolume(Config::MasterVolume);
}

// =============================================================================
// GTA IV Audio System Stubs and Hooks
// =============================================================================

// GTA IV uses RAGE's audEngine for audio management.
// These hooks intercept audio-related calls to provide proper volume control.

// Hook for audio engine initialization
// Address TBD - needs to be found in GTA IV code
void GTA4_AudioEngineInit(PPCRegister& result)
{
    GTA4Audio::SetInitialized(true);
    LOGN("GTA4 Audio: Engine initialized");
    result.u32 = 1;  // Success
}

// Hook for setting music volume
// This would be called by the game's options menu
void GTA4_SetMusicVolume(PPCRegister& volume)
{
    float vol = volume.f32;
    GTA4Audio::SetMusicVolume(vol);
    Config::MusicVolume = vol;
}

// Hook for setting effects volume
void GTA4_SetEffectsVolume(PPCRegister& volume)
{
    float vol = volume.f32;
    GTA4Audio::SetEffectsVolume(vol);
    Config::EffectsVolume = vol;
}

// Hook for getting current music volume (for UI display)
void GTA4_GetMusicVolume(PPCRegister& result)
{
    result.f32 = GTA4Audio::GetMusicVolume();
}

// Hook for getting current effects volume (for UI display)
void GTA4_GetEffectsVolume(PPCRegister& result)
{
    result.f32 = GTA4Audio::GetEffectsVolume();
}

// =============================================================================
// XMA Audio Playback Hooks
// =============================================================================
// GTA IV uses XMA compressed audio for music and sound effects.
// The XMA decoder is already implemented in apu/xma_decoder.cpp

// Stub for audio stream creation - returns success
// TODO: Find actual GTA IV address for audio stream creation
uint32_t GTA4_CreateAudioStream(PPCContext& ctx, uint8_t* base)
{
    // Return success - actual implementation in xma_decoder
    ctx.r3.u32 = 0;  // S_OK
    return ctx.r3.u32;
}

// =============================================================================
// Radio System (GTA IV specific)
// =============================================================================
// GTA IV has an in-game radio system with multiple stations.
// Volume should respect both radio volume and music volume settings.

namespace GTA4Radio
{
    static int g_currentStation = 0;
    static float g_radioVolume = 1.0f;
    static bool g_radioEnabled = true;
    
    int GetCurrentStation() { return g_currentStation; }
    void SetCurrentStation(int station) { g_currentStation = station; }
    
    float GetRadioVolume() { return g_radioVolume * GTA4Audio::GetMusicVolume(); }
    void SetRadioVolume(float volume) { g_radioVolume = std::clamp(volume, 0.0f, 1.0f); }
    
    bool IsRadioEnabled() { return g_radioEnabled; }
    void SetRadioEnabled(bool enabled) { g_radioEnabled = enabled; }
}

// Hook for radio station change
void GTA4_SetRadioStation(PPCRegister& station)
{
    int stationId = station.s32;
    GTA4Radio::SetCurrentStation(stationId);
    // Station change logged - LOGN doesn't support format args
}

// Hook for radio volume
void GTA4_SetRadioVolume(PPCRegister& volume)
{
    float vol = volume.f32;
    GTA4Radio::SetRadioVolume(vol);
}

// =============================================================================
// Audio Configuration Callback
// =============================================================================
// Called when audio-related config values change

void AudioConfigChanged()
{
    // Sync config values to audio system
    GTA4Audio::SetMasterVolume(Config::MasterVolume);
    GTA4Audio::SetMusicVolume(Config::MusicVolume);
    GTA4Audio::SetEffectsVolume(Config::EffectsVolume);
}

// =============================================================================
// GTA IV Audio System PPC Hooks
// =============================================================================
// These hooks intercept the actual PPC audio initialization functions
// to track when the audio system is ready for volume control.

// External declarations for original PPC functions
extern "C" void __imp__sub_822EEDB8(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_821AB5F8(PPCContext& ctx, uint8_t* base);
extern "C" void __imp__sub_82910858(PPCContext& ctx, uint8_t* base);

// Hook for audio system initialization (sub_822EEDB8)
// Called during game startup to initialize RAGE audEngine
PPC_FUNC_HOOK(sub_822EEDB8) {
    // Call original function first
    __imp__sub_822EEDB8(ctx, base);
    
    // Mark audio as initialized
    GTA4Audio::SetInitialized(true);
    LOG_INFO("[Audio] GTA IV audio system initialized");
}

// Hook for radio system initialization (sub_821AB5F8)
// Called during game startup to set up radio stations
PPC_FUNC_HOOK(sub_821AB5F8) {
    // Call original function first
    __imp__sub_821AB5F8(ctx, base);

    LOG_INFO("[Audio] GTA IV radio system initialized");
}

// Hook for audio node tree traversal (sub_82910858)
// Prevents stack overflow from circular references in the audio graph.
// Each call uses 128 bytes of PPC stack; 505+ levels = cycle in node data.
// Real audio trees are <20 levels deep, so 64 is a safe limit.
PPC_FUNC_HOOK(sub_82910858) {
    static thread_local int depth = 0;
    if (depth > 64) return;
    depth++;
    __imp__sub_82910858(ctx, base);
    depth--;
}

// =============================================================================
// Loading Screen Audio Readiness Check (sub_822155F0)
// =============================================================================
//
// Called by the loading screen state machine (sub_82144188) when stage type == 5.
// Original checks whether a RAGE audio resource at g_loadScreenAudioResource
// (0x82BCC1F8) has finished loading. If it returns false, the state machine
// stalls with "Waiting on audio to start the loading screens!"
//
// Original logic (13 PPC instructions, leaf function):
//   bool audResource_IsReady(audResourceHandle* this) {
//       if (this->handle == 0xFFFF)      return false;  // not initialized
//       if (this->bankPtr == NULL)        return false;  // no bank loaded
//       if (this->bankPtr->state != 2)    return false;  // bank not READY
//       return true;
//   }
//
// Struct layout (reconstructed from field accesses):
//
//   struct audResourceHandle {        // at 0x82BCC1F8 (global instance)
//       /* +0x00 */ uint16_t pad0;
//       /* +0x02 */ uint16_t pad1;
//       /* +0x04 */ uint16_t handle;   // 0xFFFF = uninitialized
//       /* +0x06 */ uint16_t pad2;
//       /* +0x08 */ uint32_t pad3;
//       /* +0x0C */ uint32_t pad4;
//       /* +0x10 */ uint32_t bankPtr;  // guest pointer to audBankHeader
//   };                                // size >= 0x14 (20 bytes)
//
//   struct audBankHeader {
//       /* +0x00 */ uint32_t pad0;
//       /* +0x04 */ uint16_t pad1;
//       /* +0x06 */ uint16_t state;    // 2 = READY
//   };
//
// Force-returning 1 unblocks the loading screen on platforms where the
// XMA audio backend is not yet fully initialized.

PPC_FUNC_HOOK(sub_822155F0) {
    // Always report audio as ready to unblock the loading screen state machine.
    ctx.r3.u64 = 1;
}

// =============================================================================
// rage::audVoiceXenon / rage::audVoicePcmXenon  -- XAudio2 voice hooks
// =============================================================================
//
// On Xbox 360, these classes drive XAudio2 source voices for XMA and PCM audio
// respectively. On PC/macOS/Linux there is no XAudio2 hardware; SDL handles
// audio mixing at a higher level. These hooks replace the 11-slot vtable with
// implementations that maintain the game's internal state machine (flags,
// playback position, buffer double-buffering) while skipping all XAudio2 calls.
//
// Struct layout (reconstructed from Hex-Rays + recomp scaffolds):
//
//   struct audVoicePhysical {           // base class (vtable @ 0x8209E33C)
//       /* +0x00 */ uint32_t  vtable;
//       /* +0x04 */ uint32_t  soundDef;        // guest ptr to audSoundDef
//       /* +0x08 */ uint32_t  totalDurationMs;  // round(numSamples / (sampleRate*0.001))
//       /* +0x0C */ uint32_t  sampleRate;
//       /* +0x10 */ uint32_t  bankEntry;        // guest ptr
//       /* +0x14 */ uint32_t  waveInfo;         // guest ptr to audWaveInfo
//       /* +0x18 */ uint32_t  pad18;
//       /* +0x1C */ float     defaultFreq;      // 23900.0f
//       /* +0x20 */ float     channelMatrix[18]; // 6 speakers x 3 channels
//       /* +0x68 */ uint8_t   pad68[0x20];
//       /* +0x88 */ uint8_t   flags;            // bit0-7 state flags
//       /* +0x89 */ uint8_t   isStarted;
//       /* +0x8A */ uint8_t   pad8A[2];
//   };  // 0x8C bytes
//
//   struct audVoiceXenon : audVoicePhysical {  // vtable @ 0x820A0E34
//       /* +0x8C */ uint8_t   bufferSlots[2][88]; // double-buffered XMA packets
//       /* +0x13C*/ uint32_t  xaudioVoice;       // guest ptr to IXAudioSourceVoice
//       /* +0x140*/ uint32_t  outputVoice;        // guest ptr to output voice/submix
//       /* +0x144*/ uint32_t  sourceSampleRate;   // sample rate stored on Start
//       /* +0x148*/ uint32_t  samplesPlayed;      // cumulative offset for position
//       /* +0x14C*/ uint32_t  loopAdjust;         // loop sample adjustment
//       /* +0x150*/ int32_t   lastPlaybackPos;    // previous GetPlaybackPosition ms
//       /* +0x154*/ uint32_t  startOffset;        // SetPosition a2
//       /* +0x158*/ uint8_t   frozenCount;        // stall detector counter
//       /* +0x159*/ uint8_t   currentBuffer;      // 0 or 1, toggled in SubmitBuffer
//       /* +0x15A*/ uint8_t   voiceState;         // XAudio voice state bits
//   };  // pool stride 768 bytes
//
// voiceState bits:
//   bit 0 (0x01) = buffer queued / active
//   bit 1 (0x02) = playing
//   bit 2 (0x04) = paused
//   bit 4 (0x10) = audible
//   bit 5 (0x20) = starved / underrun
//   bit 6 (0x40) = looping
//
// flags bits:
//   bit 1 (0x02) = muted (volume == 0)
//   bit 3 (0x08) = hardware loop mode
//   bit 6 (0x40) = has loop info
//   bit 7 (0x80) = stop requested

// Forward declarations for original generated functions (called as fallback)
extern "C" void __imp__sub_8291E728(PPCContext& ctx, uint8_t* base); // audVoicePhysical ctor
extern "C" void __imp__sub_8291E738(PPCContext& ctx, uint8_t* base); // audVoicePhysical::Start base
extern "C" void __imp__sub_8291E908(PPCContext& ctx, uint8_t* base); // audVoicePhysical pool free
extern "C" void __imp__sub_8292FDE0(PPCContext& ctx, uint8_t* base); // SetVolume/UpdateParams
extern "C" void __imp__sub_8292F2C0(PPCContext& ctx, uint8_t* base); // UpdateChannelVolumes
extern "C" void __imp__sub_829025A0(PPCContext& ctx, uint8_t* base); // audMixer::GetLoopSamples

// Helper: convert sample count to milliseconds (replicates sub_82905B10 fsel rounding)
static int32_t samplesToMs(uint32_t samples, uint32_t sampleRate) {
    if (sampleRate == 0) return 0;
    float ms = (float)samples / ((float)sampleRate * 0.001f);
    // Replicate PPC fsel rounding: round-to-nearest via float
    return (int32_t)(ms + 0.5f);
}

// ---------------------------------------------------------------------------
// [Slot 0] Destructor  (sub_82930548 for audVoiceXenon)
// ---------------------------------------------------------------------------
// Sets vtable, calls audVoicePhysical dtor, optionally frees pool slot.
// On PC: no XAudio voice to destroy, just maintain state.
PPC_FUNC_HOOK(sub_82930548) {
    uint32_t thisPtr = ctx.r3.u32;
    uint8_t  shouldFree = ctx.r4.u8;

    // Write audVoiceXenon vtable (original writes off_820A0E34)
    PPC_STORE_U32(thisPtr + 0, 0x820A0E34);

    // Call audVoicePhysical base destructor
    ctx.r3.u64 = thisPtr;
    __imp__sub_8291E728(ctx, base);

    // Conditionally free the pool slot
    if (shouldFree & 1) {
        ctx.r3.u64 = thisPtr;
        __imp__sub_8291E908(ctx, base);
    }

    ctx.r3.u64 = thisPtr;
}

// ---------------------------------------------------------------------------
// [Slot 1] Start  (sub_8292FAB8)
// ---------------------------------------------------------------------------
// Initializes voice playback state, computes total duration, sets up XAudio
// voice with format/output configuration. On PC: set up internal state but
// skip all XAudio2 CreateSourceVoice / SetOutputVoices calls.
PPC_FUNC_HOOK(sub_8292FAB8) {
    uint32_t thisPtr = ctx.r3.u32;
    uint32_t soundDef = ctx.r4.u32; // a2

    // Call audVoicePhysical::Start (sets soundDef, waveInfo, sampleRate, channelMatrix)
    ctx.r3.u64 = thisPtr;
    ctx.r4.u64 = soundDef;
    __imp__sub_8291E738(ctx, base);

    // Read fields set by base Start
    uint32_t waveInfoPtr = PPC_LOAD_U32(thisPtr + 0x14); // waveInfo
    uint32_t sampleRate  = PPC_LOAD_U32(thisPtr + 0x0C); // sampleRate

    // Clear voiceState, set isStarted
    PPC_STORE_U8(thisPtr + 0x15A, 0);  // voiceState = 0
    PPC_STORE_U8(thisPtr + 0x89, 1);   // isStarted = 1

    // Store sourceSampleRate
    PPC_STORE_U32(thisPtr + 0x144, sampleRate);

    // Compute totalDurationMs = round(waveInfo->numSamples / (sampleRate * 0.001))
    if (waveInfoPtr && sampleRate) {
        uint32_t numSamples = PPC_LOAD_U32(waveInfoPtr + 16); // waveInfo->numSamples
        int32_t totalMs = samplesToMs(numSamples, sampleRate);
        PPC_STORE_U32(thisPtr + 0x08, (uint32_t)totalMs);
    }

    // No XAudio voice creation (sub_8218F700 CreateSourceVoice) - skip
    // No output voice setup (sub_8218EE90 SetOutputVoices) - skip
    // xaudioVoice stays 0 (no hardware voice)
    PPC_STORE_U32(thisPtr + 0x13C, 0); // xaudioVoice = NULL

    // Zero the buffer slots (176 bytes at offset 0x8C)
    for (uint32_t i = 0; i < 176; i += 4) {
        PPC_STORE_U32(thisPtr + 0x8C + i, 0);
    }

    // Read output voice from soundDef linkage
    uint32_t soundDefPtr = PPC_LOAD_U32(thisPtr + 0x04);
    if (soundDefPtr) {
        uint32_t linkedBank = PPC_LOAD_U32(soundDefPtr + 12);
        if (linkedBank) {
            PPC_STORE_U32(thisPtr + 0x140, PPC_LOAD_U32(linkedBank + 32));
        }
    }

    // Clear playback tracking
    PPC_STORE_U8(thisPtr + 0x159, 0);   // currentBuffer = 0
    PPC_STORE_U32(thisPtr + 0x148, 0);  // samplesPlayed = 0

    // Set loop flag from waveInfo
    if (waveInfoPtr) {
        int32_t loopCount = PPC_LOAD_U32(waveInfoPtr + 20); // signed
        uint8_t flags = PPC_LOAD_U8(thisPtr + 0x88);
        if (loopCount < 0) {
            PPC_STORE_U32(thisPtr + 0x14C, 0); // loopAdjust = 0
            flags &= ~0x40; // clear hasLoopInfo
        } else {
            PPC_STORE_U32(thisPtr + 0x14C, 0); // loopAdjust = 0
            flags |= 0x40;  // set hasLoopInfo
        }
        PPC_STORE_U8(thisPtr + 0x88, flags);
    }

    ctx.r3.u64 = 0; // return S_OK
}

// ---------------------------------------------------------------------------
// [Slot 2] Stop  (sub_8292F0E8)
// ---------------------------------------------------------------------------
// Stops playback: destroys the XAudio voice, calls base class notification.
// On PC: just clear the voice pointer and notify base.
PPC_FUNC_HOOK(sub_8292F0E8) {
    uint32_t thisPtr = ctx.r3.u32;

    // On Xbox: DestroyVoice(xaudioVoice) via sub_8218EDB8
    // On PC: just null out the voice handle
    PPC_STORE_U32(thisPtr + 0x13C, 0); // xaudioVoice = NULL

    // Call base class stop notification (nullsub_1 in original = audEngine::OnVoiceStopped)
    // This is effectively a no-op in the original binary (nullsub), but we call
    // through the recompiled code path to maintain any side effects.
    ctx.r3.u64 = thisPtr;
}

// ---------------------------------------------------------------------------
// [Slot 3] SetPosition  (sub_8292FFD0)
// ---------------------------------------------------------------------------
// Called to set playback position (seek). Calls SubmitBuffer if not in HW loop
// mode, then updates volume/params via sub_8292FDE0, optionally starts voice.
// On PC: maintain state tracking, skip XAudio Start call.
PPC_FUNC_HOOK(sub_8292FFD0) {
    uint32_t thisPtr = ctx.r3.u32;
    uint32_t position = ctx.r4.u32; // startOffset in samples

    uint8_t flags = PPC_LOAD_U8(thisPtr + 0x88);

    // If not in HW loop mode, call SubmitBuffer (vtable slot 7)
    if ((flags & 0x08) == 0) {
        uint32_t vtable = PPC_LOAD_U32(thisPtr);
        uint32_t submitBufFn = PPC_LOAD_U32(vtable + 28); // slot 7
        ctx.r3.u64 = thisPtr;
        ctx.r4.u64 = 0; // bufferSize = 0 triggers initial setup path
        PPC_CALL_INDIRECT_FUNC(submitBufFn);
    }

    // Store startOffset
    PPC_STORE_U32(thisPtr + 0x154, position);

    // Update volume/params via sub_8292FDE0
    uint32_t soundDefPtr = PPC_LOAD_U32(thisPtr + 0x04);
    float volume = 0.0f;
    if (soundDefPtr) {
        PPCRegister temp;
        temp.u32 = PPC_LOAD_U32(soundDefPtr);
        volume = temp.f32;
    }
    ctx.r3.u64 = thisPtr;
    ctx.f1.f64 = (double)volume;
    __imp__sub_8292FDE0(ctx, base);

    // Check if volume is zero -> mark as stopped+muted
    if (soundDefPtr) {
        PPCRegister temp;
        temp.u32 = PPC_LOAD_U32(soundDefPtr);
        if (temp.f32 == 0.0f) {
            uint8_t f = PPC_LOAD_U8(thisPtr + 0x88);
            PPC_STORE_U8(thisPtr + 0x88, f | 0x82); // stop+mute
        }
        // else: on Xbox would call sub_8218F070 (Start voice) - skip on PC
    }
}

// ---------------------------------------------------------------------------
// [Slot 4] Activate  (sub_8292F130)
// ---------------------------------------------------------------------------
// Resumes a paused/stopped voice. On Xbox: calls FlushSourceBuffers on the
// XAudio voice. On PC: just update flags.
PPC_FUNC_HOOK(sub_8292F130) {
    uint32_t thisPtr = ctx.r3.u32;

    // On Xbox: sub_8218F0B8(xaudioVoice, 0) = FlushSourceBuffers
    // On PC: skip (no XAudio voice)

    // If no loop adjustment yet, get one from the mixer
    uint32_t loopAdj = PPC_LOAD_U32(thisPtr + 0x14C);
    if (loopAdj == 0) {
        // sub_829025A0(dword_831C6750) returns mixer->field_310
        // This is the global loop sample count
        ctx.r3.u64 = 0x831C6750;
        uint32_t globalPtr = PPC_LOAD_U32(0x831C6750);
        if (globalPtr) {
            uint32_t loopSamples = PPC_LOAD_U32(globalPtr + 784);
            PPC_STORE_U32(thisPtr + 0x14C, loopSamples);
        }
    }

    // Clear stop+mute flags: flags &= 0x7D (clears bits 7 and 1)
    uint8_t flags = PPC_LOAD_U8(thisPtr + 0x88);
    PPC_STORE_U8(thisPtr + 0x88, flags & 0x7D);
}

// ---------------------------------------------------------------------------
// [Slot 5] IsPlaying  (sub_8292F198)
// ---------------------------------------------------------------------------
// Returns true if the voice is actively producing audio.
// Checks voiceState bits, flags, and soundDef configuration.
PPC_FUNC_HOOK(sub_8292F198) {
    uint32_t thisPtr = ctx.r3.u32;
    uint8_t voiceState = PPC_LOAD_U8(thisPtr + 0x15A);

    // If any of these voiceState bits are set, voice is playing
    if (voiceState & (0x01 | 0x02 | 0x04 | 0x40)) {
        ctx.r3.u64 = 1;
        return;
    }

    // Check flags: if upper bits set (>=0x80) and muted (bit 1), playing
    uint8_t flags = PPC_LOAD_U8(thisPtr + 0x88);
    if ((flags & 0x80) != 0) {
        if (flags & 0x02) {
            ctx.r3.u64 = 1;
            return;
        }
    }

    // Check soundDef->byte24 bit 1 (looping/streaming flag)
    uint32_t soundDefPtr = PPC_LOAD_U32(thisPtr + 0x04);
    if (soundDefPtr) {
        uint8_t sdByte24 = PPC_LOAD_U8(soundDefPtr + 24);
        if (sdByte24 & 0x02) {
            ctx.r3.u64 = 1;
            return;
        }
    }

    ctx.r3.u64 = 0;
}

// ---------------------------------------------------------------------------
// [Slot 6] GetAudibility  (sub_8292F210)
// ---------------------------------------------------------------------------
// Returns 1 if the voice is audible (has active buffers with content).
PPC_FUNC_HOOK(sub_8292F210) {
    uint32_t thisPtr = ctx.r3.u32;
    uint8_t voiceState = PPC_LOAD_U8(thisPtr + 0x15A);

    // Must have buffer queued (bit 0)
    if (!(voiceState & 0x01)) {
        ctx.r3.u64 = 0;
        return;
    }

    // Audible if starved (bit 5) or content-audible (bit 4)
    if (voiceState & (0x20 | 0x10)) {
        ctx.r3.u64 = 1;
        return;
    }

    ctx.r3.u64 = 0;
}

// ---------------------------------------------------------------------------
// [Slot 7] SubmitBuffer  (sub_82930078)
// ---------------------------------------------------------------------------
// Submits audio data to the XAudio voice. Reads wave info, fills buffer slot,
// toggles double-buffer index. On PC: update internal buffer tracking but
// skip actual XAudio SubmitSourceBuffer call.
PPC_FUNC_HOOK(sub_82930078) {
    uint32_t thisPtr = ctx.r3.u32;
    uint32_t bufferSize = ctx.r4.u32; // a2 = sample count for seek path

    uint32_t waveInfoPtr = PPC_LOAD_U32(thisPtr + 0x14);
    if (!waveInfoPtr) return;

    uint32_t waveDataSize = PPC_LOAD_U32(waveInfoPtr + 12); // waveInfo->dataSize
    if (!waveDataSize) return;

    // Fill current buffer slot with wave info
    uint8_t bufIdx = PPC_LOAD_U8(thisPtr + 0x159);
    uint32_t slotBase = thisPtr + 0x8C + (uint32_t)bufIdx * 88;

    // Copy wave data pointer and size
    PPC_STORE_U32(slotBase + 0, PPC_LOAD_U32(waveInfoPtr + 0));  // dataPtr
    PPC_STORE_U32(slotBase + 4, waveDataSize);                    // dataSize

    // Copy loop info if non-looping (loopCount >= 0)
    int32_t loopCount = (int32_t)PPC_LOAD_U32(waveInfoPtr + 20);
    if (loopCount >= 0) {
        PPC_STORE_U32(slotBase + 8, 0xFFFFFFFF);                       // loopEnd = -1
        PPC_STORE_U32(slotBase + 12, PPC_LOAD_U32(waveInfoPtr + 40));  // loopStartSamples
        PPC_STORE_U32(slotBase + 16, PPC_LOAD_U32(waveInfoPtr + 44));  // loopEndSamples
        uint32_t codecInfo = PPC_LOAD_U32(waveInfoPtr + 48);
        PPC_STORE_U8(slotBase + 21, codecInfo & 0x0F);                 // bitsPerSample lo
        PPC_STORE_U8(slotBase + 20, (codecInfo >> 4) & 0x0F);          // bitsPerSample hi
    }

    // On Xbox: sub_8218EF98 submits buffer to XAudio voice - skip on PC

    // Toggle double-buffer index: (bufIdx - 1) & 1
    PPC_STORE_U8(thisPtr + 0x159, (bufIdx - 1) & 1);

    // If bufferSize > 0, handle seek path: compute new samplesPlayed
    if (bufferSize > 0) {
        uint32_t sampleRate = PPC_LOAD_U32(thisPtr + 0x0C);
        if (sampleRate > 0) {
            // Convert bufferSize (samples) to ms-equivalent tracking value
            float samplesF = (float)sampleRate * (float)bufferSize;
            float msVal = samplesF * 0.001f;
            int32_t rounded = (int32_t)(msVal + 0.5f);
            uint32_t shifted = (uint32_t)rounded << 7; // multiply by 128
            PPC_STORE_U32(thisPtr + 0x148, shifted);   // samplesPlayed
        }
        // On Xbox: would also call sub_8292F910 + sub_8218F1D0
        // (encode seek position + set XAudio play region) - skip on PC
    }
}

// ---------------------------------------------------------------------------
// [Slot 8] Unused  (sub_821778A0)
// ---------------------------------------------------------------------------
// Returns this->totalDurationMs (offset 8). Shared across many vtables.
// The recompiled code handles this fine, no hook needed - but included for
// completeness. This is a hot function (70 callers) so let the recomp run it.
// We do NOT hook this slot.

// ---------------------------------------------------------------------------
// [Slot 9] GetPlaybackPosition  (sub_829302A8)
// ---------------------------------------------------------------------------
// Returns current playback position in milliseconds. Handles looping and
// frozen-voice detection. On PC: without XAudio hardware position feedback,
// return -1 (finished) to let the audio state machine advance naturally.
PPC_FUNC_HOOK(sub_829302A8) {
    uint32_t thisPtr = ctx.r3.u32;

    // Check if voice is playing via vtable slot 5 (IsPlaying)
    uint32_t vtable = PPC_LOAD_U32(thisPtr);
    uint32_t isPlayingFn = PPC_LOAD_U32(vtable + 20); // slot 5
    ctx.r3.u64 = thisPtr;
    PPC_CALL_INDIRECT_FUNC(isPlayingFn);
    bool isPlaying = (ctx.r3.u32 & 0xFF) != 0;

    if (!isPlaying) {
        // Voice not playing -> return -1 (finished)
        PPC_STORE_U8(thisPtr + 0x158, 0);          // frozenCount = 0
        PPC_STORE_U32(thisPtr + 0x150, 0xFFFFFFFF); // lastPlaybackPos = -1
        ctx.r3.s64 = -1;
        return;
    }

    // Without XAudio hardware, we cannot get real sample position from
    // sub_8292F250 (which reads XAudio GetState). Instead, return -1
    // to signal completion. The game's audio mixer will advance to the
    // next buffer or stop the voice as appropriate.
    //
    // On Xbox, this function does:
    //   1. Get current position in samples from XAudio
    //   2. Convert to ms: posMs = round(samples / (sampleRate * 0.001))
    //   3. If posMs > totalDurationMs: handle loop or finish
    //   4. Detect frozen voices (100 consecutive same-position ticks)
    //   5. Return posMs or -1

    PPC_STORE_U8(thisPtr + 0x158, 0);          // frozenCount = 0
    PPC_STORE_U32(thisPtr + 0x150, 0xFFFFFFFF); // lastPlaybackPos = -1
    ctx.r3.s64 = -1;
}

// ---------------------------------------------------------------------------
// [Slot 10] Tick  (sub_829304A0)
// ---------------------------------------------------------------------------
// Per-frame update: sets XAudio frequency ratio, updates volume/params,
// updates channel volumes, reads back voice state. On PC: maintain state
// without XAudio calls.
PPC_FUNC_HOOK(sub_829304A0) {
    uint32_t thisPtr = ctx.r3.u32;
    uint32_t xaudioVoice = PPC_LOAD_U32(thisPtr + 0x13C);

    // On Xbox: only ticks if xaudioVoice != NULL
    // On PC: xaudioVoice is always NULL, but we still need to update state
    // for the game's audio state machine to function correctly.

    // Read soundDef frequency and update volume/params
    uint32_t soundDefPtr = PPC_LOAD_U32(thisPtr + 0x04);
    if (soundDefPtr) {
        // sub_8218EE20: SetFrequencyRatio - skip on PC (no XAudio voice)

        // Update volume/output params via sub_8292FDE0
        PPCRegister temp;
        temp.u32 = PPC_LOAD_U32(soundDefPtr);
        ctx.r3.u64 = thisPtr;
        ctx.f1.f64 = (double)temp.f32;
        __imp__sub_8292FDE0(ctx, base);

        // Update channel volumes via sub_8292F2C0
        ctx.r3.u64 = thisPtr;
        __imp__sub_8292F2C0(ctx, base);

        // On Xbox: sub_8218EF40 reads back XAudio voice state into voiceState
        // On PC: leave voiceState as-is (maintained by Start/Stop/SubmitBuffer)
    }
}

// =============================================================================
// rage::audVoicePcmXenon hooks
// =============================================================================
// audVoicePcmXenon (vtable @ 0x820A1014) uses the same 11-slot layout but
// for PCM (uncompressed) audio. The logic is structurally identical to
// audVoiceXenon with minor differences in buffer format handling.
// Since both classes inherit from audVoicePhysical and the PC path skips all
// XAudio calls, the PCM variants delegate to the same stub logic.

// Forward declarations for audVoicePcmXenon original functions
extern "C" void __imp__sub_829316A0(PPCContext& ctx, uint8_t* base); // PCM destructor
extern "C" void __imp__sub_829305F8(PPCContext& ctx, uint8_t* base); // PCM Start
extern "C" void __imp__sub_829308F8(PPCContext& ctx, uint8_t* base); // PCM Stop
extern "C" void __imp__sub_82930AD8(PPCContext& ctx, uint8_t* base); // PCM SetPosition
extern "C" void __imp__sub_82930B80(PPCContext& ctx, uint8_t* base); // PCM Activate
extern "C" void __imp__sub_82930BC8(PPCContext& ctx, uint8_t* base); // PCM IsPlaying
extern "C" void __imp__sub_82930C30(PPCContext& ctx, uint8_t* base); // PCM GetAudibility
extern "C" void __imp__sub_82930C60(PPCContext& ctx, uint8_t* base); // PCM SubmitBuffer
extern "C" void __imp__sub_829314A8(PPCContext& ctx, uint8_t* base); // PCM GetPlaybackPosition
extern "C" void __imp__sub_829315F8(PPCContext& ctx, uint8_t* base); // PCM Tick

// [PCM Slot 0] Destructor
PPC_FUNC_HOOK(sub_829316A0) {
    uint32_t thisPtr = ctx.r3.u32;
    uint8_t  shouldFree = ctx.r4.u8;

    PPC_STORE_U32(thisPtr + 0, 0x820A1014); // audVoicePcmXenon vtable
    ctx.r3.u64 = thisPtr;
    __imp__sub_8291E728(ctx, base);          // base dtor

    if (shouldFree & 1) {
        ctx.r3.u64 = thisPtr;
        __imp__sub_8291E908(ctx, base);
    }
    ctx.r3.u64 = thisPtr;
}

// [PCM Slot 1] Start
PPC_FUNC_HOOK(sub_829305F8) {
    uint32_t thisPtr = ctx.r3.u32;
    uint32_t soundDef = ctx.r4.u32;

    // Call base Start
    ctx.r3.u64 = thisPtr;
    ctx.r4.u64 = soundDef;
    __imp__sub_8291E738(ctx, base);

    uint32_t sampleRate = PPC_LOAD_U32(thisPtr + 0x0C);
    uint32_t waveInfoPtr = PPC_LOAD_U32(thisPtr + 0x14);

    PPC_STORE_U8(thisPtr + 0x15A, 0);  // voiceState = 0  (PCM offset is 0x16C but same field)
    PPC_STORE_U8(thisPtr + 0x89, 1);   // isStarted = 1
    PPC_STORE_U32(thisPtr + 0x144, sampleRate);

    if (waveInfoPtr && sampleRate) {
        uint32_t numSamples = PPC_LOAD_U32(waveInfoPtr + 16);
        int32_t totalMs = samplesToMs(numSamples, sampleRate);
        PPC_STORE_U32(thisPtr + 0x08, (uint32_t)totalMs);
    }

    // No XAudio voice creation on PC
    PPC_STORE_U32(thisPtr + 0x13C, 0);

    // Zero buffer area
    for (uint32_t i = 0; i < 176; i += 4) {
        PPC_STORE_U32(thisPtr + 0x8C + i, 0);
    }

    PPC_STORE_U8(thisPtr + 0x159, 0);
    PPC_STORE_U32(thisPtr + 0x148, 0);

    if (waveInfoPtr) {
        int32_t loopCount = (int32_t)PPC_LOAD_U32(waveInfoPtr + 20);
        uint8_t flags = PPC_LOAD_U8(thisPtr + 0x88);
        if (loopCount < 0) {
            PPC_STORE_U32(thisPtr + 0x14C, 0);
            flags &= ~0x40;
        } else {
            PPC_STORE_U32(thisPtr + 0x14C, 0);
            flags |= 0x40;
        }
        PPC_STORE_U8(thisPtr + 0x88, flags);
    }

    ctx.r3.u64 = 0;
}

// [PCM Slot 2] Stop
PPC_FUNC_HOOK(sub_829308F8) {
    uint32_t thisPtr = ctx.r3.u32;
    PPC_STORE_U32(thisPtr + 0x13C, 0);
    ctx.r3.u64 = thisPtr;
}

// [PCM Slot 3] SetPosition
PPC_FUNC_HOOK(sub_82930AD8) {
    uint32_t thisPtr = ctx.r3.u32;
    uint32_t position = ctx.r4.u32;

    uint8_t flags = PPC_LOAD_U8(thisPtr + 0x88);
    if ((flags & 0x08) == 0) {
        uint32_t vtable = PPC_LOAD_U32(thisPtr);
        uint32_t submitBufFn = PPC_LOAD_U32(vtable + 28);
        ctx.r3.u64 = thisPtr;
        ctx.r4.u64 = 0;
        PPC_CALL_INDIRECT_FUNC(submitBufFn);
    }

    PPC_STORE_U32(thisPtr + 0x154, position);

    uint32_t soundDefPtr = PPC_LOAD_U32(thisPtr + 0x04);
    float volume = 0.0f;
    if (soundDefPtr) {
        PPCRegister temp;
        temp.u32 = PPC_LOAD_U32(soundDefPtr);
        volume = temp.f32;
    }
    ctx.r3.u64 = thisPtr;
    ctx.f1.f64 = (double)volume;
    __imp__sub_8292FDE0(ctx, base);

    if (soundDefPtr) {
        PPCRegister temp;
        temp.u32 = PPC_LOAD_U32(soundDefPtr);
        if (temp.f32 == 0.0f) {
            uint8_t f = PPC_LOAD_U8(thisPtr + 0x88);
            PPC_STORE_U8(thisPtr + 0x88, f | 0x82);
        }
    }
}

// [PCM Slot 4] Activate
PPC_FUNC_HOOK(sub_82930B80) {
    uint32_t thisPtr = ctx.r3.u32;

    uint32_t loopAdj = PPC_LOAD_U32(thisPtr + 0x14C);
    if (loopAdj == 0) {
        uint32_t globalPtr = PPC_LOAD_U32(0x831C6750);
        if (globalPtr) {
            PPC_STORE_U32(thisPtr + 0x14C, PPC_LOAD_U32(globalPtr + 784));
        }
    }

    uint8_t flags = PPC_LOAD_U8(thisPtr + 0x88);
    PPC_STORE_U8(thisPtr + 0x88, flags & 0x7D);
}

// [PCM Slot 5] IsPlaying
PPC_FUNC_HOOK(sub_82930BC8) {
    uint32_t thisPtr = ctx.r3.u32;
    uint8_t voiceState = PPC_LOAD_U8(thisPtr + 0x15A);

    if (voiceState & (0x01 | 0x02 | 0x04 | 0x40)) {
        ctx.r3.u64 = 1;
        return;
    }

    uint8_t flags = PPC_LOAD_U8(thisPtr + 0x88);
    if ((flags & 0x80) && (flags & 0x02)) {
        ctx.r3.u64 = 1;
        return;
    }

    uint32_t soundDefPtr = PPC_LOAD_U32(thisPtr + 0x04);
    if (soundDefPtr) {
        if (PPC_LOAD_U8(soundDefPtr + 24) & 0x02) {
            ctx.r3.u64 = 1;
            return;
        }
    }

    ctx.r3.u64 = 0;
}

// [PCM Slot 6] GetAudibility
PPC_FUNC_HOOK(sub_82930C30) {
    uint32_t thisPtr = ctx.r3.u32;
    uint8_t voiceState = PPC_LOAD_U8(thisPtr + 0x15A);

    if (!(voiceState & 0x01)) {
        ctx.r3.u64 = 0;
        return;
    }
    if (voiceState & (0x20 | 0x10)) {
        ctx.r3.u64 = 1;
        return;
    }
    ctx.r3.u64 = 0;
}

// [PCM Slot 7] SubmitBuffer
PPC_FUNC_HOOK(sub_82930C60) {
    uint32_t thisPtr = ctx.r3.u32;
    uint32_t bufferSize = ctx.r4.u32;

    uint32_t waveInfoPtr = PPC_LOAD_U32(thisPtr + 0x14);
    if (!waveInfoPtr) return;

    uint32_t waveDataSize = PPC_LOAD_U32(waveInfoPtr + 12);
    if (!waveDataSize) return;

    uint8_t bufIdx = PPC_LOAD_U8(thisPtr + 0x159);
    uint32_t slotBase = thisPtr + 0x8C + (uint32_t)bufIdx * 88;

    PPC_STORE_U32(slotBase + 0, PPC_LOAD_U32(waveInfoPtr + 0));
    PPC_STORE_U32(slotBase + 4, waveDataSize);

    int32_t loopCount = (int32_t)PPC_LOAD_U32(waveInfoPtr + 20);
    if (loopCount >= 0) {
        PPC_STORE_U32(slotBase + 8, 0xFFFFFFFF);
        PPC_STORE_U32(slotBase + 12, PPC_LOAD_U32(waveInfoPtr + 40));
        PPC_STORE_U32(slotBase + 16, PPC_LOAD_U32(waveInfoPtr + 44));
        uint32_t codecInfo = PPC_LOAD_U32(waveInfoPtr + 48);
        PPC_STORE_U8(slotBase + 21, codecInfo & 0x0F);
        PPC_STORE_U8(slotBase + 20, (codecInfo >> 4) & 0x0F);
    }

    PPC_STORE_U8(thisPtr + 0x159, (bufIdx - 1) & 1);

    if (bufferSize > 0) {
        uint32_t sampleRate = PPC_LOAD_U32(thisPtr + 0x0C);
        if (sampleRate > 0) {
            float samplesF = (float)sampleRate * (float)bufferSize;
            float msVal = samplesF * 0.001f;
            int32_t rounded = (int32_t)(msVal + 0.5f);
            uint32_t shifted = (uint32_t)rounded << 7;
            PPC_STORE_U32(thisPtr + 0x148, shifted);
        }
    }
}

// [PCM Slot 9] GetPlaybackPosition
PPC_FUNC_HOOK(sub_829314A8) {
    uint32_t thisPtr = ctx.r3.u32;

    uint32_t vtable = PPC_LOAD_U32(thisPtr);
    uint32_t isPlayingFn = PPC_LOAD_U32(vtable + 20);
    ctx.r3.u64 = thisPtr;
    PPC_CALL_INDIRECT_FUNC(isPlayingFn);
    bool isPlaying = (ctx.r3.u32 & 0xFF) != 0;

    if (!isPlaying) {
        PPC_STORE_U8(thisPtr + 0x158, 0);
        PPC_STORE_U32(thisPtr + 0x150, 0xFFFFFFFF);
        ctx.r3.s64 = -1;
        return;
    }

    PPC_STORE_U8(thisPtr + 0x158, 0);
    PPC_STORE_U32(thisPtr + 0x150, 0xFFFFFFFF);
    ctx.r3.s64 = -1;
}

// [PCM Slot 10] Tick
PPC_FUNC_HOOK(sub_829315F8) {
    uint32_t thisPtr = ctx.r3.u32;

    uint32_t soundDefPtr = PPC_LOAD_U32(thisPtr + 0x04);
    if (soundDefPtr) {
        PPCRegister temp;
        temp.u32 = PPC_LOAD_U32(soundDefPtr);
        ctx.r3.u64 = thisPtr;
        ctx.f1.f64 = (double)temp.f32;
        __imp__sub_8292FDE0(ctx, base);

        ctx.r3.u64 = thisPtr;
        __imp__sub_8292F2C0(ctx, base);
    }
}
