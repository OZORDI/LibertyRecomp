#include "game_window.h"

#if !defined(LIBERTY_RECOMP_PS4) && !defined(LIBERTY_RECOMP_NX)

#include <gpu/video.h>
#include <os/logger.h>
#include <os/user.h>
#include <os/version.h>
#include <app.h>
#include <sdl_listener.h>
// SDL_syswm.h removed in SDL3

#if defined(__APPLE__)
#include <CoreGraphics/CoreGraphics.h>
#endif

#if _WIN32
#include <dwmapi.h>
#include <shellscalingapi.h>
#endif

#ifdef LIBERTY_RECOMP_HAS_RESOURCES
#include <res/images/game_icon.bmp.h>
#endif // LIBERTY_RECOMP_HAS_RESOURCES

bool m_isFullscreenKeyReleased = true;
bool m_isResizing = false;

bool Window_OnSDLEvent(void*, SDL_Event* event)
{
    if (ImGui::GetIO().BackendPlatformUserData != nullptr)
        ImGui_ImplSDL3_ProcessEvent(event);

    for (auto listener : GetEventListeners())
    {
        if (listener->OnSDLEvent(event))
        {
            return false;
        }
    }

    switch (event->type)
    {
        case SDL_EVENT_QUIT:
        {
            if (App::s_isSaving)
                break;

            printf("[SDL_QUIT] App::Exit() triggered from Window_OnSDLEvent! timestamp=%llu\n", (unsigned long long)event->quit.timestamp);
            fflush(stdout);
            App::Exit();

            break;
        }

        case SDL_EVENT_KEY_DOWN:
        {
            switch (event->key.key)
            {
                // Toggle fullscreen on ALT+ENTER.
                case SDLK_RETURN:
                {
                    if (!(event->key.mod & SDL_KMOD_ALT) || !m_isFullscreenKeyReleased)
                        break;

                    Config::Fullscreen = GameWindow::SetFullscreen(!GameWindow::IsFullscreen());

                    if (Config::Fullscreen)
                    {
                        Config::Monitor = GameWindow::GetDisplay();
                    }
                    else
                    {
                        Config::WindowState = GameWindow::SetMaximised(Config::WindowState == EWindowState::Maximised);
                    }

                    // Block holding ALT+ENTER spamming window changes.
                    m_isFullscreenKeyReleased = false;

                    break;
                }

                // Restore original window dimensions on F2.
                case SDLK_F2:
                    Config::Fullscreen = GameWindow::SetFullscreen(false);
                    GameWindow::ResetDimensions();
                    break;

                // Recentre window on F3.
                case SDLK_F3:
                {
                    if (GameWindow::IsFullscreen())
                        break;

                    GameWindow::SetDimensions(GameWindow::s_width, GameWindow::s_height);

                    break;
                }
            }

            break;
        }

        case SDL_EVENT_KEY_UP:
        {
            switch (event->key.key)
            {
                // Allow user to input ALT+ENTER again.
                case SDLK_RETURN:
                    m_isFullscreenKeyReleased = true;
                    break;
            }
            break;
        }

        case SDL_EVENT_WINDOW_FOCUS_LOST:
            GameWindow::s_isFocused = false;
            SDL_ShowCursor();
            break;

        case SDL_EVENT_WINDOW_FOCUS_GAINED:
        {
            GameWindow::s_isFocused = true;

            if (GameWindow::IsFullscreen() && !GameWindow::s_isFullscreenCursorVisible)
                SDL_HideCursor();

            break;
        }

        case SDL_EVENT_WINDOW_RESTORED:
            Config::WindowState = EWindowState::Normal;
            break;

        case SDL_EVENT_WINDOW_MAXIMIZED:
            Config::WindowState = EWindowState::Maximised;
            break;

        case SDL_EVENT_WINDOW_RESIZED:
            m_isResizing = true;
            Config::WindowSize = -1;
            GameWindow::s_width = event->window.data1;
            GameWindow::s_height = event->window.data2;
            GameWindow::SetTitle(fmt::format("{} - [{}x{}]", GameWindow::GetTitle(), GameWindow::s_width, GameWindow::s_height).c_str());
            break;

        case SDL_EVENT_WINDOW_MOVED:
            GameWindow::s_x = event->window.data1;
            GameWindow::s_y = event->window.data2;
            break;

        case SDL_USER_PLAYER_CHAR:
            GameWindow::s_playerCharacter = static_cast<EPlayerCharacter>(event->user.code);
            GameWindow::SetIcon(GameWindow::s_playerCharacter);
            break;
    }

    return false;
}

bool GameWindow::Init(const char* sdlVideoDriver)
{
#if defined(__APPLE__)
    uint32_t displayCount = 0;
    CGError cgErr = CGGetActiveDisplayList(0, nullptr, &displayCount);
    if (cgErr != kCGErrorSuccess || displayCount == 0)
    {
        LOGFN_ERROR("No active display/WindowServer access (CGError={}, count={}). Aborting video init.", (int)cgErr, displayCount);
        return false;
    }
#endif

#ifdef __linux__
    SDL_SetHint("SDL_APP_ID", "io.github.ozordi.libertyrecomp");
#endif

    // SDL3: SDL_VideoInit removed; use SDL_InitSubSystem(SDL_INIT_VIDEO) instead
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
    {
        LOGFN_ERROR("Failed to initialise SDL video subsystem: {}", SDL_GetError());
    }

    auto videoDriverName = SDL_GetCurrentVideoDriver();

    if (videoDriverName)
        LOGFN("SDL video driver: \"{}\"", videoDriverName);
    else
        LOGFN_ERROR("Failed to initialise any SDL video driver: {}", SDL_GetError());

    SDL_AddEventWatch(Window_OnSDLEvent, s_pWindow);

#ifdef _WIN32
    SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
#endif

    s_x = Config::WindowX;
    s_y = Config::WindowY;
    s_width = Config::WindowWidth;
    s_height = Config::WindowHeight;

    // If window size is default (1280x720 from old config), use native display resolution
    if (s_width == 1280 && s_height == 720)
    {
        // SDL3: SDL_GetDesktopDisplayMode returns a pointer
        SDL_DisplayID primaryDisplay = SDL_GetPrimaryDisplay();
        const SDL_DisplayMode* displayMode = SDL_GetDesktopDisplayMode(primaryDisplay);
        if (displayMode)
        {
            int logicalW = displayMode->w;
            int logicalH = displayMode->h;
            
            // Create a small temporary hidden window to detect the HiDPI scale factor
            // SDL3: SDL_WINDOW_ALLOW_HIGHDPI removed (always on), position set separately
            SDL_Window* tempWindow = SDL_CreateWindow(
                "HiDPI Detection",
                100, 100,
                SDL_WINDOW_HIDDEN
            );
            
            float scaleX = 1.0f;
            float scaleY = 1.0f;
            
            if (tempWindow)
            {
                int windowW, windowH;
                int pixelW, pixelH;
                
                SDL_GetWindowSize(tempWindow, &windowW, &windowH);
                SDL_GetWindowSizeInPixels(tempWindow, &pixelW, &pixelH);
                
                if (windowW > 0 && windowH > 0)
                {
                    scaleX = static_cast<float>(pixelW) / static_cast<float>(windowW);
                    scaleY = static_cast<float>(pixelH) / static_cast<float>(windowH);
                }
                
                SDL_DestroyWindow(tempWindow);
            }
            
            // Apply the HiDPI scale factor to get native pixel resolution
            s_width = static_cast<int>(logicalW * scaleX);
            s_height = static_cast<int>(logicalH * scaleY);
            
            if (scaleX > 1.0f || scaleY > 1.0f)
            {
                LOGFN("Detected HiDPI/Retina display (scale: {:.1f}x{:.1f})", scaleX, scaleY);
                LOGFN("Logical resolution: {}x{}, Native pixel resolution: {}x{}", 
                      logicalW, logicalH, s_width, s_height);
            }
            else
            {
                LOGFN("Using native display resolution: {}x{}", s_width, s_height);
            }
        }
        else
        {
            // Fallback to 1920x1080 if detection fails
            s_width = DEFAULT_WIDTH;
            s_height = DEFAULT_HEIGHT;
            LOGFN("Display detection failed, using fallback: {}x{}", s_width, s_height);
        }
    }

    if (s_x == -1 && s_y == -1)
        s_x = s_y = SDL_WINDOWPOS_CENTERED;

    if (!IsPositionValid())
        GameWindow::ResetDimensions();

    // SDL3: position is set after creation
    s_pWindow = SDL_CreateWindow("Liberty Recompiled", s_width, s_height, GetWindowFlags());
    if (s_pWindow)
        SDL_SetWindowPosition(s_pWindow, s_x, s_y);
    if (!s_pWindow)
    {
        LOGFN_ERROR("SDL_CreateWindow failed: {}", SDL_GetError());
        return false;
    }

    if (IsFullscreen())
        SDL_HideCursor();

    SetDisplay(Config::Monitor);
    SetIcon();
    SetTitle();

    SDL_SetWindowMinimumSize(s_pWindow, MIN_WIDTH, MIN_HEIGHT);

    // SDL3: SDL_SysWMinfo removed; use SDL_GetPointerProperty on the window
#if defined(_WIN32)
    s_renderWindow = (HWND)SDL_GetPointerProperty(SDL_GetWindowProperties(s_pWindow), SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);

    if (Config::DisableDWMRoundedCorners)
    {
        DWM_WINDOW_CORNER_PREFERENCE wcp = DWMWCP_DONOTROUND;
        DwmSetWindowAttribute(s_renderWindow, DWMWA_WINDOW_CORNER_PREFERENCE, &wcp, sizeof(wcp));
    }
#elif defined(SDL_VULKAN_ENABLED)
    s_renderWindow = s_pWindow;
#elif defined(__linux__)
    {
        auto* x11Display = SDL_GetPointerProperty(SDL_GetWindowProperties(s_pWindow), SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr);
        auto x11Window = (Window)SDL_GetNumberProperty(SDL_GetWindowProperties(s_pWindow), SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
        s_renderWindow = { (Display*)x11Display, x11Window };
    }
#elif defined(__APPLE__)
    // SDL3: get the native NSWindow* via properties
    s_renderWindow.window = SDL_GetPointerProperty(
        SDL_GetWindowProperties(s_pWindow),
        SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
    // Create Metal layer for rendering
    auto metalView = SDL_Metal_CreateView(s_pWindow);
    s_renderWindow.view = SDL_Metal_GetLayer(metalView);
#else
    static_assert(false, "Unknown platform.");
#endif

    SetTitleBarColour();

    SDL_ShowWindow(s_pWindow);
    return true;
}

void GameWindow::Update()
{
    if (!GameWindow::IsFullscreen() && !GameWindow::IsMaximised() && !s_isChangingDisplay)
    {
        Config::WindowX = GameWindow::s_x;
        Config::WindowY = GameWindow::s_y;
        Config::WindowWidth = GameWindow::s_width;
        Config::WindowHeight = GameWindow::s_height;
    }

    if (m_isResizing)
    {
        SetTitle();
        m_isResizing = false;
    }

    if (g_needsResize)
        s_isChangingDisplay = false;
}

SDL_Surface* GameWindow::GetIconSurface(void* pIconBmp, size_t iconSize)
{
    auto io = SDL_IOFromMem(pIconBmp, iconSize);
    auto surface = SDL_LoadBMP_IO(io, true);

    if (!surface)
        LOGF_ERROR("Failed to load icon: {}", SDL_GetError());

    return surface;
}

void GameWindow::SetIcon(void* pIconBmp, size_t iconSize)
{
    if (auto icon = GetIconSurface(pIconBmp, iconSize))
    {
        SDL_SetWindowIcon(s_pWindow, icon);
        SDL_DestroySurface(icon);
    }
}

void GameWindow::SetIcon(EPlayerCharacter player)
{
    // TODO: Per-character icons
    switch (player) {
        case EPlayerCharacter::Sonic:
            break;
        case EPlayerCharacter::Shadow:
            break;
        case EPlayerCharacter::Silver:
            break;
        case EPlayerCharacter::Blaze:
            break;
        case EPlayerCharacter::Amy:
            break;
        case EPlayerCharacter::Tails:
            break;
        case EPlayerCharacter::Rouge:
            break;
        case EPlayerCharacter::Knuckles:
            break;
    }

#ifdef LIBERTY_RECOMP_HAS_RESOURCES
    SetIcon(g_game_icon, sizeof(g_game_icon));
#endif
}

const char* GameWindow::GetTitle()
{
    if (Config::UseOfficialTitleOnTitleBar)
    {
        return "SONIC THE HEDGEHOG";
    }

    return "Liberty Recompiled";
}

void GameWindow::SetTitle(const char* title)
{
    SDL_SetWindowTitle(s_pWindow, title ? title : GetTitle());
}

void GameWindow::SetTitleBarColour()
{
#if _WIN32
    if (os::user::IsDarkTheme())
    {
        auto version = os::version::GetOSVersion();

        if (version.Major < 10 || version.Build <= 17763)
            return;

        auto flag = version.Build >= 18985
            ? DWMWA_USE_IMMERSIVE_DARK_MODE
            : 19; // DWMWA_USE_IMMERSIVE_DARK_MODE_BEFORE_20H1

        const DWORD useImmersiveDarkMode = 1;
        DwmSetWindowAttribute(s_renderWindow, flag, &useImmersiveDarkMode, sizeof(useImmersiveDarkMode));
    }
#endif
}

bool GameWindow::IsFullscreen()
{
    return SDL_GetWindowFlags(s_pWindow) & SDL_WINDOW_FULLSCREEN;
}

bool GameWindow::SetFullscreen(bool isEnabled)
{
    if (isEnabled)
    {
        SDL_SetWindowFullscreen(s_pWindow, true);
        if (s_isFullscreenCursorVisible) SDL_ShowCursor(); else SDL_HideCursor();
    }
    else
    {
        SDL_SetWindowFullscreen(s_pWindow, false);
        SDL_ShowCursor();

        SetIcon(GameWindow::s_playerCharacter);
        SetDimensions(Config::WindowWidth, Config::WindowHeight, Config::WindowX, Config::WindowY);
    }

    return isEnabled;
}
    
void GameWindow::SetFullscreenCursorVisibility(bool isVisible)
{
    s_isFullscreenCursorVisible = isVisible;

    if (IsFullscreen())
    {
        if (s_isFullscreenCursorVisible) SDL_ShowCursor(); else SDL_HideCursor();
    }
    else
    {
        SDL_ShowCursor();
    }
}

bool GameWindow::IsMaximised()
{
    return SDL_GetWindowFlags(s_pWindow) & SDL_WINDOW_MAXIMIZED;
}

EWindowState GameWindow::SetMaximised(bool isEnabled)
{
    if (isEnabled)
    {
        SDL_MaximizeWindow(s_pWindow);
    }
    else
    {
        SDL_RestoreWindow(s_pWindow);
    }

    return isEnabled
        ? EWindowState::Maximised
        : EWindowState::Normal;
}

SDL_Rect GameWindow::GetDimensions()
{
    SDL_Rect rect{};

    SDL_GetWindowPosition(s_pWindow, &rect.x, &rect.y);
    SDL_GetWindowSize(s_pWindow, &rect.w, &rect.h);

    return rect;
}

void GameWindow::GetSizeInPixels(int *w, int *h)
{
    SDL_GetWindowSizeInPixels(s_pWindow, w, h);
}

void GameWindow::SetDimensions(int w, int h, int x, int y)
{
    s_width = w;
    s_height = h;
    s_x = x;
    s_y = y;

    SDL_SetWindowSize(s_pWindow, w, h);
    SDL_ResizeEvent(s_pWindow, w, h);

    SDL_SetWindowPosition(s_pWindow, x, y);
    SDL_MoveEvent(s_pWindow, x, y);
}

void GameWindow::ResetDimensions()
{
    s_x = SDL_WINDOWPOS_CENTERED;
    s_y = SDL_WINDOWPOS_CENTERED;
    s_width = DEFAULT_WIDTH;
    s_height = DEFAULT_HEIGHT;

    Config::WindowX = s_x;
    Config::WindowY = s_y;
    Config::WindowWidth = s_width;
    Config::WindowHeight = s_height;
}

uint32_t GameWindow::GetWindowFlags()
{
    // SDL3: SDL_WINDOW_ALLOW_HIGHDPI removed (always on)
    uint32_t flags = SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE;

    if (Config::WindowState == EWindowState::Maximised)
        flags |= SDL_WINDOW_MAXIMIZED;

    if (Config::Fullscreen)
        flags |= SDL_WINDOW_FULLSCREEN;

#ifdef SDL_VULKAN_ENABLED
    flags |= SDL_WINDOW_VULKAN;
#endif

    return flags;
}

int GameWindow::GetDisplayCount()
{
    int count = 0;
    SDL_DisplayID* displays = SDL_GetDisplays(&count);
    SDL_free(displays);
    return count > 0 ? count : 1;
}

int GameWindow::GetDisplay()
{
    SDL_DisplayID did = SDL_GetDisplayForWindow(s_pWindow);
    // Convert DisplayID to index for compatibility
    int count = 0;
    SDL_DisplayID* displays = SDL_GetDisplays(&count);
    int idx = 0;
    if (displays) {
        for (int i = 0; i < count; i++) {
            if (displays[i] == did) { idx = i; break; }
        }
        SDL_free(displays);
    }
    return idx;
}

void GameWindow::SetDisplay(int displayIndex)
{
    if (!IsFullscreen())
        return;

    if (GetDisplay() == displayIndex)
        return;

    s_isChangingDisplay = true;

    // SDL3: Get DisplayID from index
    int count = 0;
    SDL_DisplayID* displays = SDL_GetDisplays(&count);
    if (displays && displayIndex < count)
    {
        SDL_Rect bounds;
        if (SDL_GetDisplayBounds(displays[displayIndex], &bounds))
        {
            SetFullscreen(false);
            SetDimensions(bounds.w, bounds.h, bounds.x, bounds.y);
            SetFullscreen(true);
        }
        else
        {
            ResetDimensions();
        }
        SDL_free(displays);
    }
    else
    {
        if (displays) SDL_free(displays);
        ResetDimensions();
    }
}

std::vector<SDL_DisplayMode> GameWindow::GetDisplayModes(bool ignoreInvalidModes, bool ignoreRefreshRates)
{
    auto result = std::vector<SDL_DisplayMode>();
    auto uniqueResolutions = std::set<std::pair<int, int>>();

    // SDL3: Get DisplayID from index
    int displayCount = 0;
    SDL_DisplayID* displays = SDL_GetDisplays(&displayCount);
    int displayIdx = GetDisplay();
    if (!displays || displayIdx >= displayCount) {
        if (displays) SDL_free(displays);
        return result;
    }
    SDL_DisplayID did = displays[displayIdx];
    SDL_free(displays);

    int modeCount = 0;
    const SDL_DisplayMode* const* modes = SDL_GetFullscreenDisplayModes(did, &modeCount);

    if (!modes || modeCount <= 0)
        return result;

    for (int i = modeCount - 1; i >= 0; i--)
    {
        const SDL_DisplayMode* modePtr = modes[i];
        SDL_DisplayMode mode = *modePtr;

        if (ignoreInvalidModes)
        {
            if (mode.w < MIN_WIDTH || mode.h < MIN_HEIGHT)
                continue;

            const SDL_DisplayMode* desktopMode = SDL_GetDesktopDisplayMode(did);

            if (desktopMode)
            {
                if (mode.w >= desktopMode->w || mode.h >= desktopMode->h)
                    continue;
            }
        }

        if (ignoreRefreshRates)
        {
            auto res = std::make_pair(mode.w, mode.h);

            if (uniqueResolutions.find(res) == uniqueResolutions.end())
            {
                uniqueResolutions.insert(res);
                result.push_back(mode);
            }
        }
        else
        {
            result.push_back(mode);
        }
    }

    return result;
}

int GameWindow::FindNearestDisplayMode()
{
    auto result = -1;
    auto displayModes = GetDisplayModes();
    auto currentDiff = std::numeric_limits<int>::max();

    for (int i = 0; i < displayModes.size(); i++)
    {
        auto& mode = displayModes[i];

        auto widthDiff = abs(mode.w - s_width);
        auto heightDiff = abs(mode.h - s_height);
        auto totalDiff = widthDiff + heightDiff;

        if (totalDiff < currentDiff)
        {
            currentDiff = totalDiff;
            result = i;
        }
    }

    return result;
}

bool GameWindow::IsPositionValid()
{
    auto displayCount = GetDisplayCount();

    int numDisplays = 0;
    SDL_DisplayID* displayIds = SDL_GetDisplays(&numDisplays);
    if (!displayIds) return false;

    for (int i = 0; i < numDisplays; i++)
    {
        SDL_Rect bounds;

        if (SDL_GetDisplayBounds(displayIds[i], &bounds))
        {
            auto x = s_x;
            auto y = s_y;

            // Window spans across the entire display in windowed mode, which is invalid.
            if (!Config::Fullscreen && s_width == bounds.w && s_height == bounds.h)
            {
                SDL_free(displayIds);
                return false;
            }

            if (x == SDL_WINDOWPOS_CENTERED_DISPLAY(i))
                x = bounds.w / 2 - s_width / 2;

            if (y == SDL_WINDOWPOS_CENTERED_DISPLAY(i))
                y = bounds.h / 2 - s_height / 2;

            if (x >= bounds.x && x < bounds.x + bounds.w &&
                y >= bounds.y && y < bounds.y + bounds.h)
            {
                SDL_free(displayIds);
                return true;
            }
        }
    }

    SDL_free(displayIds);
    return false;
}

#endif // !LIBERTY_RECOMP_PS4 && !LIBERTY_RECOMP_NX

#if defined(LIBERTY_RECOMP_PS4)

#include "game_window.h"
#include <cstdio>
#include <cstring>

#include <orbis/VideoOut.h>
#include <orbis/SystemService.h>
#include <orbis/_types/video.h>
#include <orbis/_types/user.h>

// PS4 video-out state. On PS4 there is no window system — the TV is the display
// and it is always fullscreen. We open a single video-out handle (the "main"
// bus) at startup and expose that handle as the render surface. plume's PS4
// backend consumes the handle in place of an HWND/NSWindow*/X11 Window.
namespace {
    constexpr int32_t kInvalidVideoOutHandle = -1;
    int32_t g_videoOutHandle = kInvalidVideoOutHandle;

    // Query the TV resolution as reported by the system. Falls back to 1080p
    // if the status call fails for any reason (e.g. no HDMI signal yet).
    void QueryVideoOutResolution(int32_t handle, int& width, int& height)
    {
        width = DEFAULT_WIDTH;
        height = DEFAULT_HEIGHT;

        if (handle < 0)
            return;

        OrbisVideoOutResolutionStatus status{};
        int32_t ret = sceVideoOutGetResolutionStatus(handle, &status);
        if (ret == 0 && status.width > 0 && status.height > 0)
        {
            width = static_cast<int>(status.width);
            height = static_cast<int>(status.height);
        }
        else
        {
            std::printf("[PS4][GameWindow] sceVideoOutGetResolutionStatus failed: 0x%08X\n", ret);
        }
    }
}

bool GameWindow::Init(const char* /*sdlVideoDriver*/)
{
    // Disable the idle / screensaver timer while the game is running.
    sceSystemServiceDisableSuspendConfirmationDialog();
    sceSystemServiceHideSplashScreen();

    if (g_videoOutHandle >= 0)
    {
        // Already opened (Init called twice). Refresh resolution and return.
        QueryVideoOutResolution(g_videoOutHandle, s_width, s_height);
        s_isFocused = true;
        return true;
    }

    int32_t handle = sceVideoOutOpen(
        ORBIS_VIDEO_USER_MAIN,
        ORBIS_VIDEO_OUT_BUS_MAIN,
        0,
        nullptr);

    if (handle < 0)
    {
        std::printf("[PS4][GameWindow] sceVideoOutOpen failed: 0x%08X\n", handle);
        return false;
    }

    g_videoOutHandle = handle;

    QueryVideoOutResolution(g_videoOutHandle, s_width, s_height);
    s_x = 0;
    s_y = 0;

    // Expose the video-out handle to plume as the render "window". plume's PS4
    // backend treats this as an opaque surface identifier, not an HWND.
    s_pWindow = reinterpret_cast<void*>(static_cast<intptr_t>(g_videoOutHandle));
    s_renderWindow.videoOutHandle = g_videoOutHandle;

    s_isFocused = true;
    s_isFullscreenCursorVisible = false;
    s_isChangingDisplay = false;

    std::printf("[PS4][GameWindow] video out opened: handle=%d resolution=%dx%d\n",
                g_videoOutHandle, s_width, s_height);
    return true;
}

void GameWindow::Update()
{
    // PS4 has no window events to pump here — resolution is fixed for the
    // lifetime of the video-out handle, the HID layer handles pad events
    // directly via scePadReadState, and there is no focus/resize/move to
    // track. Left as a no-op on purpose.
}

#endif // LIBERTY_RECOMP_PS4

#if defined(LIBERTY_RECOMP_NX)

// Nintendo Switch windowing backend — libnx NWindow / vi layer.
// The default NWindow created at application startup is used as the surface
// for VK_NN_vi_surface (Vulkan). Switch has no concept of a windowed mode:
// the compositor always presents full-screen, at 1920x1080 in docked mode and
// 1280x720 in handheld mode. The operation mode is re-queried every frame so
// the swapchain can be rebuilt when the user docks/undocks.

#include "game_window.h"
#include <cstdio>

#include <switch.h>

namespace
{
    NWindow*  g_nwindow   = nullptr;
    ViDisplay g_viDisplay = {};
    ViLayer   g_viLayer   = {};
    NWindow   g_ownedNwindow = {};
    bool      g_ownsLayer = false;
    bool      g_viInitOk  = false;

    void QueryOperationMode(int& width, int& height)
    {
        // Default to handheld; upgrade to docked if applet reports it.
        width  = 1280;
        height = 720;

        AppletOperationMode mode = appletGetOperationMode();
        if (mode == AppletOperationMode_Console)
        {
            width  = 1920;
            height = 1080;
        }
    }
}

bool GameWindow::Init(const char* /*sdlVideoDriver*/)
{
    // Prefer the libnx-managed default NWindow (viInit + default layer happen
    // automatically during application startup). Fall back to explicit vi
    // display/layer creation if the default is missing.
    g_nwindow = nwindowGetDefault();

    if (!g_nwindow || !nwindowIsValid(g_nwindow))
    {
        std::printf("[NX][GameWindow] default NWindow unavailable, creating vi layer manually\n");

        Result rc = viInitialize(ViServiceType_Application);
        if (R_FAILED(rc))
        {
            std::printf("[NX][GameWindow] viInitialize failed: 0x%08X\n", rc);
            return false;
        }
        g_viInitOk = true;

        rc = viOpenDefaultDisplay(&g_viDisplay);
        if (R_FAILED(rc))
        {
            // Older/name-based path.
            rc = viOpenDisplay("Default", &g_viDisplay);
        }
        if (R_FAILED(rc))
        {
            std::printf("[NX][GameWindow] viOpenDisplay failed: 0x%08X\n", rc);
            viExit();
            g_viInitOk = false;
            return false;
        }

        rc = viCreateLayer(&g_viDisplay, &g_viLayer);
        if (R_FAILED(rc))
        {
            std::printf("[NX][GameWindow] viCreateLayer failed: 0x%08X\n", rc);
            viCloseDisplay(&g_viDisplay);
            viExit();
            g_viInitOk = false;
            return false;
        }

        viSetLayerScalingMode(&g_viLayer, ViScalingMode_FitToLayer);

        rc = nwindowCreateFromLayer(&g_ownedNwindow, &g_viLayer);
        if (R_FAILED(rc))
        {
            std::printf("[NX][GameWindow] nwindowCreateFromLayer failed: 0x%08X\n", rc);
            viDestroyManagedLayer(&g_viLayer);
            viCloseDisplay(&g_viDisplay);
            viExit();
            g_viInitOk = false;
            return false;
        }

        g_nwindow   = &g_ownedNwindow;
        g_ownsLayer = true;
    }

    int w = 0, h = 0;
    QueryOperationMode(w, h);
    s_width  = w;
    s_height = h;
    s_x      = 0;
    s_y      = 0;

    // Keep NWindow dimensions in sync with the current operation mode before
    // the Vulkan swapchain attaches any buffers.
    nwindowSetDimensions(g_nwindow, static_cast<u32>(w), static_cast<u32>(h));

    // Expose the NWindow* as the native surface handle; plume's Switch backend
    // feeds this to vkCreateViSurfaceNN as VkViSurfaceCreateInfoNN::window.
    s_pWindow = static_cast<void*>(g_nwindow);
    s_renderWindow.window = static_cast<void*>(g_nwindow);

    s_isFocused = true;
    s_isFullscreenCursorVisible = false;
    s_isChangingDisplay = false;

    std::printf("[NX][GameWindow] NWindow ready: %dx%d (%s)\n",
                w, h,
                appletGetOperationMode() == AppletOperationMode_Console ? "docked" : "handheld");
    return true;
}

void GameWindow::Update()
{
    if (!g_nwindow)
        return;

    // Re-query operation mode so dock/undock transitions rebuild the swapchain.
    int w = 0, h = 0;
    QueryOperationMode(w, h);

    if (w != s_width || h != s_height)
    {
        s_width  = w;
        s_height = h;
        nwindowSetDimensions(g_nwindow, static_cast<u32>(w), static_cast<u32>(h));
        s_isChangingDisplay = true;
        std::printf("[NX][GameWindow] operation mode changed -> %dx%d\n", w, h);
    }
}

// ---------- Switch-specific helpers (not in the cross-platform header) ----------
// These mirror the interface sketched in the task:
//   GetHWND, ToggleFullscreen, PumpEvents, GetDisplayCount, GetDisplayBounds,
//   GetResolution. Exposed as free functions under the GameWindowNX namespace
//   so callers that need Switch specifics can reach them without polluting the
//   cross-platform GameWindow API.
namespace GameWindowNX
{
    void* GetHWND()
    {
        // Native handle used for VK_NN_vi_surface (VkViSurfaceCreateInfoNN::window).
        return static_cast<void*>(g_nwindow);
    }

    // Switch is always fullscreen; toggle is a no-op.
    void ToggleFullscreen() { /* no-op on Switch */ }

    // HID (hid/hid.cpp) is pumped separately; nothing to drain here.
    void PumpEvents() { /* no-op on Switch */ }

    int GetDisplayCount()
    {
        return 1;
    }

    void GetDisplayBounds(int /*index*/, int* x, int* y, int* w, int* h)
    {
        int rw = 0, rh = 0;
        QueryOperationMode(rw, rh);
        if (x) *x = 0;
        if (y) *y = 0;
        if (w) *w = rw;
        if (h) *h = rh;
    }

    void GetResolution(int* w, int* h)
    {
        int rw = 0, rh = 0;
        QueryOperationMode(rw, rh);
        if (w) *w = rw;
        if (h) *h = rh;
    }

    void Shutdown()
    {
        if (g_ownsLayer)
        {
            if (g_nwindow)
                nwindowClose(g_nwindow);
            viDestroyManagedLayer(&g_viLayer);
            viCloseDisplay(&g_viDisplay);
            g_ownsLayer = false;
        }
        if (g_viInitOk)
        {
            viExit();
            g_viInitOk = false;
        }
        g_nwindow = nullptr;
        GameWindow::s_pWindow = nullptr;
    }
}

#endif // LIBERTY_RECOMP_NX
