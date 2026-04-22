#pragma once

#include <rex/platform.h>
#include <plume_render_interface_types.h>
#include <user/config.h>

#if !REX_PLATFORM_CONSOLE
#include <sdl_events.h>
#endif

// Default to native display resolution (detected at runtime)
// These are fallback values if detection fails
#define DEFAULT_WIDTH 1920
#define DEFAULT_HEIGHT 1080
#define MIN_WIDTH 640
#define MIN_HEIGHT 480

class GameWindow
{
public:
#if !REX_PLATFORM_CONSOLE
    static inline SDL_Window* s_pWindow = nullptr;
#else
    static inline void* s_pWindow = nullptr; // unused on console
#endif
    static inline plume::RenderWindow s_renderWindow;

    static inline int s_x;
    static inline int s_y;
    static inline int s_width = DEFAULT_WIDTH;
    static inline int s_height = DEFAULT_HEIGHT;

    static inline EPlayerCharacter s_playerCharacter;

    static inline bool s_isFocused
#if REX_PLATFORM_CONSOLE
        = true // Console is always focused
#endif
        ;
    static inline bool s_isFullscreenCursorVisible;
    static inline bool s_isChangingDisplay;

#if !REX_PLATFORM_CONSOLE
    static SDL_Surface* GetIconSurface(void* pIconBmp, size_t iconSize);
    static void SetIcon(void* pIconBmp, size_t iconSize);
    static void SetIcon(EPlayerCharacter player = EPlayerCharacter::Sonic);
    static const char* GetTitle();
    static void SetTitle(const char* title = nullptr);
    static void SetTitleBarColour();
    static bool IsFullscreen();
    static bool SetFullscreen(bool isEnabled);
    static void SetFullscreenCursorVisibility(bool isVisible);
    static bool IsMaximised();
    static EWindowState SetMaximised(bool isEnabled);
    static SDL_Rect GetDimensions();
    static void GetSizeInPixels(int *w, int *h);
    static void SetDimensions(int w, int h, int x = SDL_WINDOWPOS_CENTERED, int y = SDL_WINDOWPOS_CENTERED); // SDL3: SDL_WINDOWPOS_CENTERED still valid
    static void ResetDimensions();
    static uint32_t GetWindowFlags();
    static int GetDisplayCount();
    static int GetDisplay();
    static void SetDisplay(int displayIndex);
    static std::vector<SDL_DisplayMode> GetDisplayModes(bool ignoreInvalidModes = true, bool ignoreRefreshRates = true);
    static int FindNearestDisplayMode();
    static bool IsPositionValid();
    static bool Init(const char* sdlVideoDriver = nullptr);
    static void Update();
#else
    // Console stubs — always fullscreen, fixed resolution. Init/Update are
    // implemented out-of-line per-platform in game_window.cpp (PS4 uses
    // sceVideoOut, Switch uses NWindow).
    static const char* GetTitle() { return "LibertyRecomp"; }
    static void SetTitle(const char* = nullptr) {}
    static bool IsFullscreen() { return true; }
    static bool SetFullscreen(bool) { return true; }
    static void GetSizeInPixels(int *w, int *h) { if (w) *w = s_width; if (h) *h = s_height; }
    static bool Init(const char* sdlVideoDriver = nullptr);
    static void Update();
#endif
};
