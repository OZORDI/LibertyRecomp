#include "sdl2_driver.h"
#include <cpu/code_cache.h>
#include <cpu/guest_thread.h>
#include <cpu/guest_code.h>
#include <kernel/heap.h>

#define SDLAUDIO_DRIVER_KEY (uint32_t)('SDLA')
constexpr uint32_t g_semaphoreCount = 16;
constexpr uint32_t g_audioFrameSize = XAUDIO_NUM_SAMPLES * XAUDIO_NUM_CHANNELS;

PPCFunc* volatile g_clientCallback{};
DWORD g_clientCallbackParam{}; // pointer in guest memory

SDL_AudioDeviceID g_audioDevice{};
SDL_sem* g_audioSemaphore{ SDL_CreateSemaphore(g_semaphoreCount) };
uint32_t g_audioFrames[g_audioFrameSize * g_semaphoreCount];
uint32_t g_audioFrameIndex = 0;
uint32_t g_renderFrameIndex = 0; // Index of frame that's being rendered
uint32_t g_numRenderFrames = 0; // Number of frames to render
Mutex g_framesMutex{};

static void SDLAudioCallback(void*, uint8_t* frames, int len)
{
    std::lock_guard guard{ g_framesMutex };
    if (g_numRenderFrames == 0)
    {
        memset(frames, 0, len);
    }
    else
    {
        g_renderFrameIndex = (g_renderFrameIndex + 1) % g_semaphoreCount;
        auto* srcFrames = &g_audioFrames[g_renderFrameIndex * g_audioFrameSize];
        memcpy(frames, srcFrames, g_audioFrameSize * sizeof(float));
        --g_numRenderFrames;
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
            continue;
        }

        SDL_SemWait(g_audioSemaphore);
        ctx.r3.u64 = g_clientCallbackParam;
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
    auto* pClientParam = static_cast<uint32_t*>(g_userHeap.Alloc(sizeof(param)));
    ByteSwap(param);
    *pClientParam = param;
    g_clientCallbackParam = g_memory.MapVirtual(pClientParam);

    g_clientCallback = callback;
}

void XAudioSubmitFrame(void* samples)
{
    std::lock_guard guard{ g_framesMutex };
    uint32_t* audioFrame = &g_audioFrames[g_audioFrameSize * g_audioFrameIndex];
    g_audioFrameIndex = (g_audioFrameIndex + 1) % g_semaphoreCount;

    for (size_t i = 0; i < XAUDIO_NUM_SAMPLES; i++)
    {
        for (size_t j = 0; j < XAUDIO_NUM_CHANNELS; j++)
            audioFrame[i * XAUDIO_NUM_CHANNELS + j] = std::byteswap(((uint32_t*)samples)[j * XAUDIO_NUM_SAMPLES + i]);
    }

    ++g_numRenderFrames;
}
