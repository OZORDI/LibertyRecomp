#include "sdl2_driver.h"
#include <cpu/code_cache.h>
#include <cpu/guest_thread.h>
#include <cpu/guest_code.h>
#include <queue>

#define SDLAUDIO_DRIVER_KEY (uint32_t)('SDLA')
constexpr uint32_t g_semaphoreCount = 16;
constexpr uint32_t g_audioFrameSize = XAUDIO_NUM_SAMPLES * XAUDIO_NUM_CHANNELS;

PPCFunc* g_clientCallback{};
SDL_AudioDeviceID g_audioDevice{};
SDL_sem* g_audioSemaphore{ SDL_CreateSemaphore(g_semaphoreCount) };
uint32_t g_audioFrames[g_audioFrameSize * g_semaphoreCount];
uint32_t g_audioFrameIndex = 0;
Mutex g_framesMutex{};
std::queue<float*> g_queuedFrames{};
std::queue<std::unique_ptr<float[]>> g_frames{};

static void SDLAudioCallback(void*, uint8_t* frames, int len)
{
    std::lock_guard g{ g_framesMutex };
    if (g_queuedFrames.empty())
    {
        memset(frames, 0, len);
    }
    else
    {
        memcpy(frames, g_queuedFrames.front(), g_audioFrameSize * sizeof(float));
        g_queuedFrames.pop();
    }
    SDL_SemPost(g_audioSemaphore);
}

static PPC_FUNC(DriverLoop)
{
    GuestThread::SetThreadName(GetCurrentThreadId(), "Audio Driver");

    while (true)
    {
        if (!g_clientCallback)
        {
            // NOTE: This if statement doesn't get compiled in without this barrier. What?
            _ReadBarrier();
            continue;
        }

        SDL_SemWait(g_audioSemaphore);
        GuestCode::Run((void*)g_clientCallback, &ctx);
    }
}

void XAudioInitializeSystem()
{
    assert(g_audioSemaphore);

    SDL_SetHint(SDL_HINT_AUDIO_CATEGORY, "playback");
    SDL_SetHint(SDL_HINT_AUDIO_DEVICE_APP_NAME, "SWA");
    auto err = SDL_InitSubSystem(SDL_INIT_AUDIO);
    SDL_AudioSpec spec{};
    spec.freq = XAUDIO_SAMPLES_HZ;
    spec.format = AUDIO_F32SYS;
    spec.channels = XAUDIO_NUM_CHANNELS;
    spec.samples = XAUDIO_NUM_SAMPLES;
    spec.callback = SDLAudioCallback;
    g_audioDevice = SDL_OpenAudioDevice(nullptr, false, &spec, &spec, 0);
    assert(g_audioDevice);

    SDL_PauseAudioDevice(g_audioDevice, 0);
    KeInsertHostFunction(SDLAUDIO_DRIVER_KEY, DriverLoop);
    GuestThread::Start(SDLAUDIO_DRIVER_KEY, 0, 0, nullptr);
}

void XAudioRegisterClient(PPCFunc* callback, uint32_t param)
{
    g_clientCallback = callback;
}

void XAudioSubmitFrame(void* samples)
{
    uint32_t* audioFrame = &g_audioFrames[g_audioFrameSize * g_audioFrameIndex];
    g_audioFrameIndex = (g_audioFrameIndex + 1) % g_semaphoreCount;

    for (size_t i = 0; i < XAUDIO_NUM_SAMPLES; i++)
    {
        for (size_t j = 0; j < XAUDIO_NUM_CHANNELS; j++)
            audioFrame[i * XAUDIO_NUM_CHANNELS + j] = std::byteswap(((uint32_t*)samples)[j * XAUDIO_NUM_SAMPLES + i]);
    }

    std::lock_guard g{ g_framesMutex };
    g_queuedFrames.emplace(reinterpret_cast<float*>(audioFrame));
}
