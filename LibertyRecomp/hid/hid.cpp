#include "hid.h"
#if !REX_PLATFORM_CONSOLE
#include <ui/game_window.h>
#include <user/config.h>
#endif

// Globals are defined in ps4_hid.cpp / switch_hid.cpp Init() on consoles
#if !REX_PLATFORM_CONSOLE
hid::EInputDevice hid::g_inputDevice;
hid::EInputDevice hid::g_inputDeviceController;
hid::EInputDeviceExplicit hid::g_inputDeviceExplicit;

uint16_t hid::g_prohibitedButtons;
bool hid::g_isLeftStickProhibited;
bool hid::g_isRightStickProhibited;

void hid::SetProhibitedInputs(uint16_t wButtons, bool leftStick, bool rightStick)
{
    hid::g_prohibitedButtons = wButtons;
    hid::g_isLeftStickProhibited = leftStick;
    hid::g_isRightStickProhibited = rightStick;
}

bool hid::IsInputAllowed()
{
    return GameWindow::s_isFocused || Config::AllowBackgroundInput;
}

bool hid::IsInputDeviceController()
{
    return hid::g_inputDevice != hid::EInputDevice::Keyboard &&
        hid::g_inputDevice != hid::EInputDevice::Mouse;
}
#endif
