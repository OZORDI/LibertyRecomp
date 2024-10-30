#include "window.h"
#include <config.h>
#include <kernel/function.h>
#include <SDL_syswm.h>

bool m_isFullscreenKeyReleased = true;

int Window_OnSDLEvent(void*, SDL_Event* event)
{
    switch (event->type)
    {
        case SDL_QUIT:
            Config::Save();
            ExitProcess(0);
            break;

        case SDL_KEYDOWN:
        {
            switch (event->key.keysym.sym)
            {
                // Toggle fullscreen on ALT+ENTER.
                case SDLK_RETURN:
                {
                    if (!(event->key.keysym.mod & KMOD_ALT) || !m_isFullscreenKeyReleased)
                        break;

                    Config::Fullscreen = Window::SetFullscreen(!Window::IsFullscreen());

                    if (!Config::Fullscreen)
                        Config::WindowState = Window::SetMaximised(Config::WindowState == EWindowState::Maximised);

                    // Block holding ALT+ENTER spamming window changes.
                    m_isFullscreenKeyReleased = false;

                    break;
                }

                // Restore original window dimensions on F2.
                case SDLK_F2:
                    Config::Fullscreen = Window::SetFullscreen(false);
                    Window::SetDimensions(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DEFAULT_WIDTH, DEFAULT_HEIGHT);
                    break;
            }

            break;
        }

        case SDL_KEYUP:
        {
            switch (event->key.keysym.sym)
            {
                // Allow user to input ALT+ENTER again.
                case SDLK_RETURN:
                    m_isFullscreenKeyReleased = true;
                    break;
            }
        }

        case SDL_WINDOWEVENT:
        {
            switch (event->window.event)
            {
                case SDL_WINDOWEVENT_FOCUS_LOST:
                    Window::s_isFocused = false;
                    SDL_ShowCursor(SDL_ENABLE);
                    break;

                case SDL_WINDOWEVENT_FOCUS_GAINED:
                    Window::s_isFocused = true;
                    SDL_ShowCursor(Window::IsFullscreen() ? SDL_DISABLE : SDL_ENABLE);
                    break;

                case SDL_WINDOWEVENT_RESTORED:
                case SDL_WINDOWEVENT_MAXIMIZED:
                {
                    Config::WindowState = Window::IsMaximised()
                        ? EWindowState::Maximised
                        : EWindowState::Normal;

                    break;
                }

                case SDL_WINDOWEVENT_RESIZED:
                {
                    Window::s_width = event->window.data1;
                    Window::s_height = event->window.data2;

                    if (!Window::IsFullscreen())
                    {
                        Config::WindowWidth = Window::s_width;
                        Config::WindowHeight = Window::s_height;
                    }

                    break;
                }

                case SDL_WINDOWEVENT_MOVED:
                {
                    Window::s_x = event->window.data1;
                    Window::s_y = event->window.data2;

                    if (!Window::IsFullscreen())
                    {
                        Config::WindowX = Window::s_x;
                        Config::WindowY = Window::s_y;
                    }

                    break;
                }
            }

            break;
        }
    }

    return 0;
}

void Window::Init()
{
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
    SDL_AddEventWatch(Window_OnSDLEvent, s_pWindow);
    SetProcessDPIAware();

    s_x = Config::WindowX;
    s_y = Config::WindowY;
    s_width = Config::WindowWidth;
    s_height = Config::WindowHeight;

    auto isPositionValid = IsPositionValid();

    if (!isPositionValid)
    {
        s_x = SDL_WINDOWPOS_CENTERED;
        s_y = SDL_WINDOWPOS_CENTERED;
        s_width = DEFAULT_WIDTH;
        s_height = DEFAULT_HEIGHT;
    }

    s_pWindow = SDL_CreateWindow("SWA", s_x, s_y, s_width, s_height, GetWindowFlags());

    if (!isPositionValid)
    {
        auto rect = Window::GetDimensions();

        Config::WindowX = rect.x;
        Config::WindowY = rect.y;
        Config::WindowWidth = rect.w;
        Config::WindowHeight = rect.h;
    }

    SetIcon();
    SetTitle();
    SDL_SetWindowMinimumSize(s_pWindow, 640, 480);

    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    SDL_GetWindowWMInfo(s_pWindow, &info);

    s_handle = info.info.win.window;
}

// CApplication::Update
PPC_FUNC_IMPL(__imp__sub_822C1130);
PPC_FUNC(sub_822C1130)
{
    SDL_PumpEvents();
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);

    __imp__sub_822C1130(ctx, base);
}
