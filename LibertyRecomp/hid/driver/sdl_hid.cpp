#include <stdafx.h>
#include <SDL.h>
#include <user/config.h>
#include <hid/hid.h>
#include <hid/mouse_camera.h>
#include <os/logger.h>
#include <ui/game_window.h>
#include <kernel/xdm.h>
#include <kernel/xam.h>
#include <app.h>
#include <chrono>
#include <queue>
#include <mutex>

#define TRANSLATE_INPUT(S, X) SDL_GameControllerGetButton(controller, S) << FirstBitLow(X)
#define VIBRATION_TIMEOUT_MS 5000

class Controller
{
public:
    SDL_GameController* controller{};
    SDL_Joystick* joystick{};
    SDL_JoystickID id{ -1 };
    XAMINPUT_GAMEPAD state{};
    XAMINPUT_VIBRATION vibration{ 0, 0 };
    int index{};

    Controller() = default;

    explicit Controller(int index) : Controller(SDL_GameControllerOpen(index))
    {
        this->index = index;
    }

    Controller(SDL_GameController* controller) : controller(controller)
    {
        if (!controller)
            return;

        joystick = SDL_GameControllerGetJoystick(controller);
        id = SDL_JoystickInstanceID(joystick);
    }

    SDL_GameControllerType GetControllerType() const
    {
        return SDL_GameControllerGetType(controller);
    }

    hid::EInputDevice GetInputDevice() const
    {
        switch (GetControllerType())
        {
            case SDL_CONTROLLER_TYPE_PS3:
            case SDL_CONTROLLER_TYPE_PS4:
            case SDL_CONTROLLER_TYPE_PS5:
                return hid::EInputDevice::PlayStation;
            case SDL_CONTROLLER_TYPE_XBOX360:
            case SDL_CONTROLLER_TYPE_XBOXONE:
                return hid::EInputDevice::Xbox;
            default:
                return hid::EInputDevice::Unknown;
        }
    }

    const char* GetControllerName() const
    {
        auto result = SDL_GameControllerName(controller);

        if (!result)
            return "Unknown Device";

        return result;
    }

    void Close()
    {
        if (!controller)
            return;

        SDL_GameControllerClose(controller);

        controller = nullptr;
        joystick = nullptr;
        id = -1;
    }

    bool CanPoll()
    {
        return controller;
    }

    void PollAxis()
    {
        if (!CanPoll())
            return;

        auto& pad = state;

        pad.sThumbLX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
        pad.sThumbLY = ~SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);

        pad.sThumbRX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);
        pad.sThumbRY = ~SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);

        pad.bLeftTrigger = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT) >> 7;
        pad.bRightTrigger = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) >> 7;
    }

    void Poll()
    {
        if (!CanPoll())
            return;

        auto& pad = state;

        pad.wButtons = 0;

        pad.wButtons |= TRANSLATE_INPUT(SDL_CONTROLLER_BUTTON_DPAD_UP, XAMINPUT_GAMEPAD_DPAD_UP);
        pad.wButtons |= TRANSLATE_INPUT(SDL_CONTROLLER_BUTTON_DPAD_DOWN, XAMINPUT_GAMEPAD_DPAD_DOWN);
        pad.wButtons |= TRANSLATE_INPUT(SDL_CONTROLLER_BUTTON_DPAD_LEFT, XAMINPUT_GAMEPAD_DPAD_LEFT);
        pad.wButtons |= TRANSLATE_INPUT(SDL_CONTROLLER_BUTTON_DPAD_RIGHT, XAMINPUT_GAMEPAD_DPAD_RIGHT);

        pad.wButtons |= TRANSLATE_INPUT(SDL_CONTROLLER_BUTTON_START, XAMINPUT_GAMEPAD_START);
        pad.wButtons |= TRANSLATE_INPUT(SDL_CONTROLLER_BUTTON_BACK, XAMINPUT_GAMEPAD_BACK);
        pad.wButtons |= TRANSLATE_INPUT(SDL_CONTROLLER_BUTTON_TOUCHPAD, XAMINPUT_GAMEPAD_BACK);

        pad.wButtons |= TRANSLATE_INPUT(SDL_CONTROLLER_BUTTON_LEFTSTICK, XAMINPUT_GAMEPAD_LEFT_THUMB);
        pad.wButtons |= TRANSLATE_INPUT(SDL_CONTROLLER_BUTTON_RIGHTSTICK, XAMINPUT_GAMEPAD_RIGHT_THUMB);

        pad.wButtons |= TRANSLATE_INPUT(SDL_CONTROLLER_BUTTON_LEFTSHOULDER, XAMINPUT_GAMEPAD_LEFT_SHOULDER);
        pad.wButtons |= TRANSLATE_INPUT(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, XAMINPUT_GAMEPAD_RIGHT_SHOULDER);

        pad.wButtons |= TRANSLATE_INPUT(SDL_CONTROLLER_BUTTON_A, XAMINPUT_GAMEPAD_A);
        pad.wButtons |= TRANSLATE_INPUT(SDL_CONTROLLER_BUTTON_B, XAMINPUT_GAMEPAD_B);
        pad.wButtons |= TRANSLATE_INPUT(SDL_CONTROLLER_BUTTON_X, XAMINPUT_GAMEPAD_X);
        pad.wButtons |= TRANSLATE_INPUT(SDL_CONTROLLER_BUTTON_Y, XAMINPUT_GAMEPAD_Y);
    }

    void SetVibration(const XAMINPUT_VIBRATION& vibration)
    {
        if (!CanPoll())
            return;

        this->vibration = vibration;

        SDL_GameControllerRumble(controller, vibration.wLeftMotorSpeed * 256, vibration.wRightMotorSpeed * 256, VIBRATION_TIMEOUT_MS);
    }

    void SetLED(const uint8_t r, const uint8_t g, const uint8_t b) const
    {
        SDL_GameControllerSetLED(controller, r, g, b);
    }
};

std::array<Controller, 4> g_controllers;
Controller* g_activeController;

// Mouse state tracking
static bool s_isMouseCaptured = false;
static std::chrono::steady_clock::time_point s_lastMouseMovement;
static constexpr auto MOUSE_HIDE_DELAY = std::chrono::milliseconds(2000);

// Mouse wheel state for weapon switching (GTA IV)
static int32_t s_mouseWheelDelta = 0;

// Keystroke queue for XamInputGetKeystrokeEx
static std::queue<hid::KeystrokeEvent> s_keystrokeQueues[4];
static std::mutex s_keystrokeMutex;

// Forward declarations for keystroke processing
static uint16_t SDLScancodeToVirtualKey(SDL_Scancode scancode, uint16_t mod);
static uint16_t SDLScancodeToUnicode(SDL_Scancode scancode, uint16_t mod);
static void ProcessKeyboardEvent(const SDL_KeyboardEvent& key);

inline Controller* EnsureController(uint32_t dwUserIndex)
{
    if (!g_controllers[dwUserIndex].controller)
        return nullptr;

    return &g_controllers[dwUserIndex];
}

inline size_t FindFreeController()
{
    for (size_t i = 0; i < g_controllers.size(); i++)
    {
        if (!g_controllers[i].controller)
            return i;
    }

    return -1;
}

inline Controller* FindController(int which)
{
    for (auto& controller : g_controllers)
    {
        if (controller.id == which)
            return &controller;
    }

    return nullptr;
}

static void SetControllerInputDevice(Controller* controller)
{
    g_activeController = controller;

    if (App::s_isLoading)
        return;

    hid::EInputDevice previousDevice = hid::g_inputDevice;
    hid::g_inputDevice = controller->GetInputDevice();
    hid::g_inputDeviceController = hid::g_inputDevice;

    auto controllerType = (hid::EInputDeviceExplicit)controller->GetControllerType();
    auto controllerName = controller->GetControllerName();

    // Only proceed if the controller type changes.
    if (hid::g_inputDeviceExplicit != controllerType)
    {
        hid::g_inputDeviceExplicit = controllerType;

        if (controllerType == hid::EInputDeviceExplicit::Unknown)
        {
            LOGFN("Detected controller: {} (Unknown Controller Type)", controllerName);
        }
        else
        {
            LOGFN("Detected controller: {}", controllerName);
        }
    }
    
    // Notify game of input device change
    if (previousDevice != hid::g_inputDevice)
    {
        XamNotifyEnqueueEvent(0x00000009, (uint32_t)hid::g_inputDevice);
    }
}

static void UpdateMouseCursorVisibility()
{
    auto now = std::chrono::steady_clock::now();
    
    // In fullscreen, hide cursor after delay when using mouse for camera
    if (GameWindow::IsFullscreen() && !GameWindow::s_isFullscreenCursorVisible)
    {
        if (hid::g_inputDevice == hid::EInputDevice::Mouse)
        {
            // Show cursor briefly when mouse moves, then hide
            if (now - s_lastMouseMovement < MOUSE_HIDE_DELAY)
            {
                SDL_ShowCursor(SDL_ENABLE);
            }
            else
            {
                SDL_ShowCursor(SDL_DISABLE);
            }
        }
        else
        {
            SDL_ShowCursor(SDL_DISABLE);
        }
    }
    else
    {
        // Windowed mode or cursor forced visible
        SDL_ShowCursor(SDL_ENABLE);
    }
}

static void SetControllerTimeOfDayLED(Controller& controller, EPlayerCharacter player)
{
    uint8_t r, g, b;

    // TODO: Per-character colors

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

    r = 0;
    g = 37;
    b = 184;

    controller.SetLED(r, g, b);
}

int HID_OnSDLEvent(void*, SDL_Event* event)
{
    switch (event->type)
    {
        case SDL_CONTROLLERDEVICEADDED:
        {
            const auto freeIndex = FindFreeController();

            if (freeIndex != -1)
            {
                auto controller = Controller(event->cdevice.which);

                g_controllers[freeIndex] = controller;

                SetControllerTimeOfDayLED(controller, App::s_playerCharacter);
            }

            break;
        }

        case SDL_CONTROLLERDEVICEREMOVED:
        {
            auto* controller = FindController(event->cdevice.which);

            if (controller)
                controller->Close();

            break;
        }

        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:
        case SDL_CONTROLLERAXISMOTION:
        case SDL_CONTROLLERTOUCHPADDOWN:
        {
            auto* controller = FindController(event->cdevice.which);

            if (!controller)
                break;

            if (event->type == SDL_CONTROLLERAXISMOTION)
            {
                if (abs(event->caxis.value) > 8000)
                {
                    SDL_ShowCursor(SDL_DISABLE);
                    SetControllerInputDevice(controller);
                }

                controller->PollAxis();
            }
            else
            {
                SDL_ShowCursor(SDL_DISABLE);
                SetControllerInputDevice(controller);

                controller->Poll();
            }

            break;
        }

        case SDL_KEYDOWN:
        case SDL_KEYUP:
        {
            // Enqueue keystroke for XamInputGetKeystrokeEx
            ProcessKeyboardEvent(event->key);
            
            if (!App::s_isLoading)
            {
                hid::EInputDevice previousDevice = hid::g_inputDevice;
                hid::g_inputDevice = hid::EInputDevice::Keyboard;
                
                // Notify on device change
                if (previousDevice != hid::g_inputDevice)
                {
                    XamNotifyEnqueueEvent(0x00000009, (uint32_t)hid::g_inputDevice);
                }
            }
            break;
        }


        case SDL_MOUSEMOTION:
        {
            // Only switch to mouse on significant movement (> 5 pixels)
            if (abs(event->motion.xrel) > 5 || abs(event->motion.yrel) > 5)
            {
                if (!App::s_isLoading)
                {
                    hid::EInputDevice previousDevice = hid::g_inputDevice;
                    hid::g_inputDevice = hid::EInputDevice::Mouse;
                    
                    // Notify on device change
                    if (previousDevice != hid::g_inputDevice)
                    {
                        XamNotifyEnqueueEvent(0x00000009, (uint32_t)hid::g_inputDevice);
                    }
                }
                
                // Update mouse camera with delta
                MouseCamera::Update(event->motion.xrel, event->motion.yrel, 1.0f / 60.0f);
                
                // Track last movement for cursor visibility
                s_lastMouseMovement = std::chrono::steady_clock::now();
            }
            
            UpdateMouseCursorVisibility();
            break;
        }
        
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        {
            if (!App::s_isLoading)
            {
                hid::EInputDevice previousDevice = hid::g_inputDevice;
                hid::g_inputDevice = hid::EInputDevice::Mouse;
                
                // Notify on device change
                if (previousDevice != hid::g_inputDevice)
                {
                    XamNotifyEnqueueEvent(0x00000009, (uint32_t)hid::g_inputDevice);
                }
            }
            
            s_lastMouseMovement = std::chrono::steady_clock::now();
            UpdateMouseCursorVisibility();
            break;
        }
        
        case SDL_MOUSEWHEEL:
        {
            if (!App::s_isLoading)
            {
                hid::EInputDevice previousDevice = hid::g_inputDevice;
                hid::g_inputDevice = hid::EInputDevice::Mouse;
                
                // Notify on device change
                if (previousDevice != hid::g_inputDevice)
                {
                    XamNotifyEnqueueEvent(0x00000009, (uint32_t)hid::g_inputDevice);
                }
            }
            
            // Accumulate wheel delta (Y axis for vertical scrolling)
            // Positive = scroll up (next weapon), Negative = scroll down (previous weapon)
            s_mouseWheelDelta += event->wheel.y;
            
            s_lastMouseMovement = std::chrono::steady_clock::now();
            UpdateMouseCursorVisibility();
            break;
        }

        case SDL_WINDOWEVENT:
        {
            if (event->window.event == SDL_WINDOWEVENT_FOCUS_LOST)
            {
                // Stop vibrating controllers on focus lost.
                for (auto& controller : g_controllers)
                    controller.SetVibration({ 0, 0 });
                
                // Reset mouse camera
                MouseCamera::Reset();
            }
            else if (event->window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
            {
                // Reset mouse state on focus gain
                s_lastMouseMovement = std::chrono::steady_clock::now();
            }

            break;
        }

        case SDL_USER_PLAYER_CHAR:
        {
            for (auto& controller : g_controllers)
                SetControllerTimeOfDayLED(controller, static_cast<EPlayerCharacter>(event->user.code));

            break;
        }
    }

    return 0;
}

int32_t hid::GetMouseWheelDelta()
{
    return s_mouseWheelDelta;
}

void hid::ResetMouseWheelDelta()
{
    s_mouseWheelDelta = 0;
}

void hid::Init()
{
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_GAMECUBE, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS3, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS4, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS4_RUMBLE, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS5, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS5_PLAYER_LED, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS5_RUMBLE, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_WII, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_STEAM, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_STEAMDECK, "1");
    SDL_SetHint(SDL_HINT_XINPUT_ENABLED, "1");
    
    SDL_SetHint(SDL_HINT_GAMECONTROLLER_USE_BUTTON_LABELS, "0"); // Uses Button Labels. This hint is disabled for Nintendo Controllers.

    SDL_InitSubSystem(SDL_INIT_EVENTS);
    SDL_AddEventWatch(HID_OnSDLEvent, nullptr);

    SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

    // Load controller mappings from SDL_GameControllerDB
    if (int mappings = SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt"); mappings > 0) {
        LOGFN("Loaded {} controller mapping(s) from SDL_GameControllerDB ({})", mappings, "gamecontrollerdb.txt");
    }
    
    // Initialize mouse camera system
    MouseCamera::Initialize();
    
    // Initialize mouse state
    s_lastMouseMovement = std::chrono::steady_clock::now();
}

uint32_t hid::GetState(uint32_t dwUserIndex, XAMINPUT_STATE* pState)
{
    static uint32_t packet;

    if (!pState)
        return ERROR_BAD_ARGUMENTS;

    memset(pState, 0, sizeof(*pState));

    pState->dwPacketNumber = packet++;

    if (!g_activeController)
        return ERROR_DEVICE_NOT_CONNECTED;

    pState->Gamepad = g_activeController->state;

    return ERROR_SUCCESS;
}

uint32_t hid::SetState(uint32_t dwUserIndex, XAMINPUT_VIBRATION* pVibration)
{
    if (!pVibration)
        return ERROR_BAD_ARGUMENTS;

    if (!g_activeController)
        return ERROR_DEVICE_NOT_CONNECTED;

    g_activeController->SetVibration(*pVibration);

    return ERROR_SUCCESS;
}

uint32_t hid::GetCapabilities(uint32_t dwUserIndex, XAMINPUT_CAPABILITIES* pCaps)
{
    if (!pCaps)
        return ERROR_BAD_ARGUMENTS;

    if (!g_activeController)
        return ERROR_DEVICE_NOT_CONNECTED;

    memset(pCaps, 0, sizeof(*pCaps));

    pCaps->Type = XAMINPUT_DEVTYPE_GAMEPAD;
    pCaps->SubType = XAMINPUT_DEVSUBTYPE_GAMEPAD; // TODO: other types?
    pCaps->Flags = 0;
    pCaps->Gamepad = g_activeController->state;
    pCaps->Vibration = g_activeController->vibration;

    return ERROR_SUCCESS;
}

// Convert SDL scancode to Xbox virtual key
static uint16_t SDLScancodeToVirtualKey(SDL_Scancode scancode, uint16_t mod) {
    // Letters A-Z
    if (scancode >= SDL_SCANCODE_A && scancode <= SDL_SCANCODE_Z) {
        return 0x41 + (scancode - SDL_SCANCODE_A);
    }
    
    // Numbers 0-9
    if (scancode >= SDL_SCANCODE_1 && scancode <= SDL_SCANCODE_9) {
        return 0x31 + (scancode - SDL_SCANCODE_1);
    }
    if (scancode == SDL_SCANCODE_0) {
        return 0x30;
    }
    
    // Special keys
    switch (scancode) {
        case SDL_SCANCODE_RETURN:    return VK_RETURN;
        case SDL_SCANCODE_ESCAPE:    return VK_ESCAPE;
        case SDL_SCANCODE_BACKSPACE: return VK_BACK;
        case SDL_SCANCODE_TAB:       return VK_TAB;
        case SDL_SCANCODE_SPACE:     return VK_SPACE;
        case SDL_SCANCODE_DELETE:    return VK_DELETE;
        case SDL_SCANCODE_LEFT:      return VK_LEFT;
        case SDL_SCANCODE_RIGHT:     return VK_RIGHT;
        case SDL_SCANCODE_UP:        return VK_UP;
        case SDL_SCANCODE_DOWN:      return VK_DOWN;
        case SDL_SCANCODE_LSHIFT:
        case SDL_SCANCODE_RSHIFT:    return VK_SHIFT;
        case SDL_SCANCODE_LCTRL:
        case SDL_SCANCODE_RCTRL:     return VK_CONTROL;
        default: return 0;
    }
}

// Get unicode character for the key
static uint16_t SDLScancodeToUnicode(SDL_Scancode scancode, uint16_t mod) {
    bool shift = (mod & KMOD_SHIFT) != 0;
    
    // Letters A-Z
    if (scancode >= SDL_SCANCODE_A && scancode <= SDL_SCANCODE_Z) {
        char base = shift ? 'A' : 'a';
        return base + (scancode - SDL_SCANCODE_A);
    }
    
    // Numbers 0-9 (with shift for symbols)
    if (scancode >= SDL_SCANCODE_1 && scancode <= SDL_SCANCODE_9) {
        if (shift) {
            const char* symbols = "!@#$%^&*(";
            return symbols[scancode - SDL_SCANCODE_1];
        }
        return '1' + (scancode - SDL_SCANCODE_1);
    }
    if (scancode == SDL_SCANCODE_0) {
        return shift ? ')' : '0';
    }
    
    // Special keys
    switch (scancode) {
        case SDL_SCANCODE_RETURN:    return '\r';
        case SDL_SCANCODE_BACKSPACE: return '\b';
        case SDL_SCANCODE_TAB:       return '\t';
        case SDL_SCANCODE_SPACE:     return ' ';
        default: return 0;
    }
}

// Process SDL keyboard event into keystroke queue
static void ProcessKeyboardEvent(const SDL_KeyboardEvent& key) {
    uint16_t virtualKey = SDLScancodeToVirtualKey(key.keysym.scancode, key.keysym.mod);
    if (virtualKey == 0) return;
    
    hid::KeystrokeEvent event;
    event.virtualKey = virtualKey;
    event.unicode = SDLScancodeToUnicode(key.keysym.scancode, key.keysym.mod);
    event.userIndex = 0;
    
    if (key.type == SDL_KEYDOWN) {
        event.flags = key.repeat ? XINPUT_KEYSTROKE_REPEAT : XINPUT_KEYSTROKE_KEYDOWN;
    } else {
        event.flags = XINPUT_KEYSTROKE_KEYUP;
    }
    
    hid::EnqueueKeystroke(event);
}

void hid::EnqueueKeystroke(const KeystrokeEvent& event) {
    std::lock_guard<std::mutex> lock(s_keystrokeMutex);
    if (event.userIndex < 4) {
        // Limit queue size to prevent memory issues
        if (s_keystrokeQueues[event.userIndex].size() < 64) {
            s_keystrokeQueues[event.userIndex].push(event);
        }
    }
}

bool hid::DequeueKeystroke(uint8_t userIndex, KeystrokeEvent& outEvent) {
    std::lock_guard<std::mutex> lock(s_keystrokeMutex);
    if (userIndex >= 4 || s_keystrokeQueues[userIndex].empty()) {
        return false;
    }
    
    outEvent = s_keystrokeQueues[userIndex].front();
    s_keystrokeQueues[userIndex].pop();
    return true;
}

void hid::ClearKeystrokeQueue(uint8_t userIndex) {
    std::lock_guard<std::mutex> lock(s_keystrokeMutex);
    if (userIndex < 4) {
        while (!s_keystrokeQueues[userIndex].empty()) {
            s_keystrokeQueues[userIndex].pop();
        }
    }
}
