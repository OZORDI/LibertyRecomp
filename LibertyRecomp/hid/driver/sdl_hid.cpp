#include <stdafx.h>
#include <SDL3/SDL.h>
#include <user/config.h>
#include <hid/hid.h>
#include <hid/mouse_camera.h>
#include <hid/dualsense.h>
#include <os/logger.h>
#include <ui/game_window.h>
#include <kernel/xdm.h>
#include <kernel/xam.h>
#include <kernel/button_prompts.h>
#include <app.h>
#include <chrono>
#include <queue>
#include <mutex>
#include <cstring>
#include <cmath>

#define TRANSLATE_INPUT(S, X) SDL_GetGamepadButton(controller, S) << FirstBitLow(X)
#define VIBRATION_TIMEOUT_MS 5000

// Motion sensing state (global for active controller)
static hid::MotionState g_motionState{};
static bool g_motionSensorEnabled = false;

class Controller
{
public:
    SDL_Gamepad* controller{};
    SDL_Joystick* joystick{};
    SDL_JoystickID id{ 0 };
    XAMINPUT_GAMEPAD state{};
    XAMINPUT_VIBRATION vibration{ 0, 0 };
    int index{};
    
    // Motion sensor state
    bool hasGyro{};
    bool hasAccel{};
    bool motionEnabled{};
    
    // For orientation integration
    float integratedPitch{};
    float integratedRoll{};
    float integratedYaw{};
    uint64_t lastMotionTimestamp{};

    Controller() = default;

    explicit Controller(SDL_JoystickID instance_id) : Controller(SDL_OpenGamepad(instance_id))
    {
    }

    Controller(SDL_Gamepad* controller) : controller(controller)
    {
        if (!controller)
            return;

        joystick = SDL_GetGamepadJoystick(controller);
        id = SDL_GetJoystickID(joystick);

        // Check for motion sensor support
        hasGyro = SDL_GamepadHasSensor(controller, SDL_SENSOR_GYRO);
        hasAccel = SDL_GamepadHasSensor(controller, SDL_SENSOR_ACCEL);
        
        if (hasGyro || hasAccel) {
            LOGFN("Motion sensors detected - Gyro: {}, Accel: {}", hasGyro ? "Yes" : "No", hasAccel ? "Yes" : "No");
        }
    }

    SDL_GamepadType GetControllerType() const
    {
        return SDL_GetGamepadType(controller);
    }

    hid::EInputDevice GetInputDevice() const
    {
        switch (GetControllerType())
        {
            case SDL_GAMEPAD_TYPE_PS3:
            case SDL_GAMEPAD_TYPE_PS4:
            case SDL_GAMEPAD_TYPE_PS5:
                return hid::EInputDevice::PlayStation;
            case SDL_GAMEPAD_TYPE_XBOX360:
            case SDL_GAMEPAD_TYPE_XBOXONE:
                return hid::EInputDevice::Xbox;
            default:
                return hid::EInputDevice::Unknown;
        }
    }

    const char* GetControllerName() const
    {
        auto result = SDL_GetGamepadName(controller);

        if (!result)
            return "Unknown Device";

        return result;
    }

    void Close()
    {
        if (!controller)
            return;
        
        // Disable motion sensors before closing
        if (motionEnabled) {
            SetMotionEnabled(false);
        }

        SDL_CloseGamepad(controller);

        controller = nullptr;
        joystick = nullptr;
        id = 0;
        hasGyro = false;
        hasAccel = false;
        motionEnabled = false;
    }

    bool CanPoll()
    {
        return controller;
    }
    
    // Enable/disable motion sensors
    void SetMotionEnabled(bool enabled)
    {
        if (!controller) return;
        if (enabled == motionEnabled) return;
        
        if (enabled) {
            if (hasGyro) {
                SDL_SetGamepadSensorEnabled(controller, SDL_SENSOR_GYRO, true);
            }
            if (hasAccel) {
                SDL_SetGamepadSensorEnabled(controller, SDL_SENSOR_ACCEL, true);
            }
            LOG_INFO("Motion sensors enabled");
        } else {
            if (hasGyro) {
                SDL_SetGamepadSensorEnabled(controller, SDL_SENSOR_GYRO, false);
            }
            if (hasAccel) {
                SDL_SetGamepadSensorEnabled(controller, SDL_SENSOR_ACCEL, false);
            }
            LOG_INFO("Motion sensors disabled");
        }
        
        motionEnabled = enabled;
        
        // Reset integration when enabling
        if (enabled) {
            integratedPitch = 0.0f;
            integratedRoll = 0.0f;
            integratedYaw = 0.0f;
            lastMotionTimestamp = SDL_GetTicks() * 1000; // Convert to microseconds
        }
    }
    
    // Poll motion sensors
    void PollMotion(hid::MotionState& outState)
    {
        if (!controller || !motionEnabled) {
            outState = {};
            return;
        }
        
        outState.hasGyro = hasGyro;
        outState.hasAccel = hasAccel;
        outState.isCalibrated = true; // SDL handles calibration
        outState.timestamp = SDL_GetTicks() * 1000; // Microseconds
        
        // Read gyroscope (radians/second)
        if (hasGyro) {
            float gyroData[3] = {0, 0, 0};
            if (SDL_GetGamepadSensorData(controller, SDL_SENSOR_GYRO, gyroData, 3)) {
                outState.gyroX = gyroData[0];
                outState.gyroY = gyroData[1];
                outState.gyroZ = gyroData[2];
            }
        }
        
        // Read accelerometer (m/s² - divide by 9.81 to get g-forces)
        if (hasAccel) {
            float accelData[3] = {0, 0, 0};
            if (SDL_GetGamepadSensorData(controller, SDL_SENSOR_ACCEL, accelData, 3)) {
                // SDL returns m/s², convert to g-forces
                constexpr float GRAVITY = 9.81f;
                outState.accelX = accelData[0] / GRAVITY;
                outState.accelY = accelData[1] / GRAVITY;
                outState.accelZ = accelData[2] / GRAVITY;
            }
        }
        
        // Calculate derived orientation using complementary filter
        // This combines gyro integration (fast but drifts) with accelerometer (noisy but absolute)
        float dt = 0.0f;
        if (lastMotionTimestamp > 0) {
            dt = (outState.timestamp - lastMotionTimestamp) / 1000000.0f; // Convert μs to seconds
        }
        lastMotionTimestamp = outState.timestamp;
        
        if (dt > 0.0f && dt < 0.1f) { // Sanity check on delta time
            // Integrate gyroscope for orientation
            integratedPitch += outState.gyroX * dt;
            integratedRoll += outState.gyroZ * dt;
            integratedYaw += outState.gyroY * dt;
            
            // Calculate pitch/roll from accelerometer (absolute reference)
            float accelPitch = atan2f(-outState.accelY, sqrtf(outState.accelX * outState.accelX + outState.accelZ * outState.accelZ));
            float accelRoll = atan2f(outState.accelX, -outState.accelZ);
            
            // Complementary filter: 98% gyro, 2% accelerometer
            constexpr float ALPHA = 0.98f;
            integratedPitch = ALPHA * integratedPitch + (1.0f - ALPHA) * accelPitch;
            integratedRoll = ALPHA * integratedRoll + (1.0f - ALPHA) * accelRoll;
            // Yaw cannot be corrected without magnetometer, so it will drift
        }
        
        outState.pitch = integratedPitch;
        outState.roll = integratedRoll;
        outState.yaw = integratedYaw;
    }
    
    void ResetMotionOrientation()
    {
        integratedPitch = 0.0f;
        integratedRoll = 0.0f;
        integratedYaw = 0.0f;
    }

    void PollAxis()
    {
        if (!CanPoll())
            return;

        auto& pad = state;

        pad.sThumbLX = SDL_GetGamepadAxis(controller, SDL_GAMEPAD_AXIS_LEFTX);
        pad.sThumbLY = ~SDL_GetGamepadAxis(controller, SDL_GAMEPAD_AXIS_LEFTY);

        pad.sThumbRX = SDL_GetGamepadAxis(controller, SDL_GAMEPAD_AXIS_RIGHTX);
        pad.sThumbRY = ~SDL_GetGamepadAxis(controller, SDL_GAMEPAD_AXIS_RIGHTY);

        pad.bLeftTrigger = SDL_GetGamepadAxis(controller, SDL_GAMEPAD_AXIS_LEFT_TRIGGER) >> 7;
        pad.bRightTrigger = SDL_GetGamepadAxis(controller, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) >> 7;
    }

    void Poll()
    {
        if (!CanPoll())
            return;

        auto& pad = state;

        pad.wButtons = 0;

        pad.wButtons |= TRANSLATE_INPUT(SDL_GAMEPAD_BUTTON_DPAD_UP, XAMINPUT_GAMEPAD_DPAD_UP);
        pad.wButtons |= TRANSLATE_INPUT(SDL_GAMEPAD_BUTTON_DPAD_DOWN, XAMINPUT_GAMEPAD_DPAD_DOWN);
        pad.wButtons |= TRANSLATE_INPUT(SDL_GAMEPAD_BUTTON_DPAD_LEFT, XAMINPUT_GAMEPAD_DPAD_LEFT);
        pad.wButtons |= TRANSLATE_INPUT(SDL_GAMEPAD_BUTTON_DPAD_RIGHT, XAMINPUT_GAMEPAD_DPAD_RIGHT);

        pad.wButtons |= TRANSLATE_INPUT(SDL_GAMEPAD_BUTTON_START, XAMINPUT_GAMEPAD_START);
        pad.wButtons |= TRANSLATE_INPUT(SDL_GAMEPAD_BUTTON_BACK, XAMINPUT_GAMEPAD_BACK);
        pad.wButtons |= TRANSLATE_INPUT(SDL_GAMEPAD_BUTTON_TOUCHPAD, XAMINPUT_GAMEPAD_BACK);

        pad.wButtons |= TRANSLATE_INPUT(SDL_GAMEPAD_BUTTON_LEFT_STICK, XAMINPUT_GAMEPAD_LEFT_THUMB);
        pad.wButtons |= TRANSLATE_INPUT(SDL_GAMEPAD_BUTTON_RIGHT_STICK, XAMINPUT_GAMEPAD_RIGHT_THUMB);

        pad.wButtons |= TRANSLATE_INPUT(SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, XAMINPUT_GAMEPAD_LEFT_SHOULDER);
        pad.wButtons |= TRANSLATE_INPUT(SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, XAMINPUT_GAMEPAD_RIGHT_SHOULDER);

        pad.wButtons |= TRANSLATE_INPUT(SDL_GAMEPAD_BUTTON_SOUTH, XAMINPUT_GAMEPAD_A);
        pad.wButtons |= TRANSLATE_INPUT(SDL_GAMEPAD_BUTTON_EAST, XAMINPUT_GAMEPAD_B);
        pad.wButtons |= TRANSLATE_INPUT(SDL_GAMEPAD_BUTTON_WEST, XAMINPUT_GAMEPAD_X);
        pad.wButtons |= TRANSLATE_INPUT(SDL_GAMEPAD_BUTTON_NORTH, XAMINPUT_GAMEPAD_Y);
    }

    void SetVibration(const XAMINPUT_VIBRATION& vibration)
    {
        if (!CanPoll())
            return;

        this->vibration = vibration;

        SDL_RumbleGamepad(controller, vibration.wLeftMotorSpeed * 256, vibration.wRightMotorSpeed * 256, VIBRATION_TIMEOUT_MS);
    }

    void SetLED(const uint8_t r, const uint8_t g, const uint8_t b) const
    {
        SDL_SetGamepadLED(controller, r, g, b);
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

// Maps SDL_GameControllerType to EInputDeviceExplicit with name-based detection for special controllers
static hid::EInputDeviceExplicit MapControllerType(SDL_GamepadType sdlType, const char* controllerName)
{
    // First check name for special controllers that SDL doesn't have dedicated types for
    if (controllerName)
    {
        // Steam Deck detection
        if (strstr(controllerName, "Steam Deck") != nullptr ||
            strstr(controllerName, "Deck Controller") != nullptr)
        {
            return hid::EInputDeviceExplicit::SteamDeck;
        }
        
        // Steam Controller detection
        if (strstr(controllerName, "Steam Controller") != nullptr ||
            strstr(controllerName, "Steam Virtual Gamepad") != nullptr)
        {
            return hid::EInputDeviceExplicit::SteamController;
        }
        
        // Xbox Series X detection (check name since SDL reports as XboxOne)
        if (strstr(controllerName, "Xbox Series") != nullptr ||
            strstr(controllerName, "Xbox Wireless Controller") != nullptr)
        {
            return hid::EInputDeviceExplicit::XboxSeriesX;
        }
    }
    
    // Fall back to SDL type mapping
    switch (sdlType)
    {
        case SDL_GAMEPAD_TYPE_XBOX360:
            return hid::EInputDeviceExplicit::Xbox360;
        case SDL_GAMEPAD_TYPE_XBOXONE:
            return hid::EInputDeviceExplicit::XboxOne;
        case SDL_GAMEPAD_TYPE_PS3:
            return hid::EInputDeviceExplicit::DualShock3;
        case SDL_GAMEPAD_TYPE_PS4:
            return hid::EInputDeviceExplicit::DualShock4;
        case SDL_GAMEPAD_TYPE_PS5:
            return hid::EInputDeviceExplicit::DualSense;
        case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO:
            return hid::EInputDeviceExplicit::SwitchPro;
        case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_LEFT:
            return hid::EInputDeviceExplicit::SwitchJCLeft;
        case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT:
            return hid::EInputDeviceExplicit::SwitchJCRight;
        case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
            return hid::EInputDeviceExplicit::SwitchJCPair;
        case SDL_GAMEPAD_TYPE_STANDARD:
            return hid::EInputDeviceExplicit::Unknown;
        default:
            return hid::EInputDeviceExplicit::Unknown;
    }
}

static void SetControllerInputDevice(Controller* controller)
{
    g_activeController = controller;

    if (App::s_isLoading)
        return;

    hid::EInputDevice previousDevice = hid::g_inputDevice;
    hid::g_inputDevice = controller->GetInputDevice();
    hid::g_inputDeviceController = hid::g_inputDevice;

    auto controllerName = controller->GetControllerName();
    auto controllerType = MapControllerType(controller->GetControllerType(), controllerName);

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
        
        // Notify button prompts system to refresh cache for the new controller type
        ButtonPrompts::RefreshCache();
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
                SDL_ShowCursor();
            }
            else
            {
                SDL_HideCursor();
            }
        }
        else
        {
            SDL_HideCursor();
        }
    }
    else
    {
        // Windowed mode or cursor forced visible
        SDL_ShowCursor();
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

bool HID_OnSDLEvent(void*, SDL_Event* event)
{
    switch (event->type)
    {
        case SDL_EVENT_GAMEPAD_ADDED:
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

        case SDL_EVENT_GAMEPAD_REMOVED:
        {
            auto* controller = FindController(event->cdevice.which);

            if (controller)
                controller->Close();

            break;
        }

        case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
        case SDL_EVENT_GAMEPAD_BUTTON_UP:
        case SDL_EVENT_GAMEPAD_AXIS_MOTION:
        case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN:
        {
            auto* controller = FindController(event->cdevice.which);

            if (!controller)
                break;

            if (event->type == SDL_EVENT_GAMEPAD_AXIS_MOTION)
            {
                if (abs(event->gaxis.value) > 8000)
                {
                    SDL_HideCursor();
                    SetControllerInputDevice(controller);
                }

                controller->PollAxis();
            }
            else
            {
                SDL_HideCursor();
                SetControllerInputDevice(controller);

                controller->Poll();
            }

            break;
        }

        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
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


        case SDL_EVENT_MOUSE_MOTION:
        {
            // Only switch to mouse on significant movement (> 5 pixels)
            // SDL3: motion.xrel/yrel are float
    if (fabsf(event->motion.xrel) > 5.0f || fabsf(event->motion.yrel) > 5.0f)
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
                MouseCamera::Update((int32_t)event->motion.xrel, (int32_t)event->motion.yrel, 1.0f / 60.0f);
                
                // Track last movement for cursor visibility
                s_lastMouseMovement = std::chrono::steady_clock::now();
            }
            
            UpdateMouseCursorVisibility();
            break;
        }
        
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        case SDL_EVENT_MOUSE_BUTTON_UP:
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
        
        case SDL_EVENT_MOUSE_WHEEL:
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
            // SDL3: wheel.y is float
            s_mouseWheelDelta += (int32_t)event->wheel.y;
            
            s_lastMouseMovement = std::chrono::steady_clock::now();
            UpdateMouseCursorVisibility();
            break;
        }

        case SDL_EVENT_WINDOW_FOCUS_LOST:
        {
            // Stop vibrating controllers on focus lost.
            for (auto& controller : g_controllers)
                controller.SetVibration({ 0, 0 });

            // Reset mouse camera
            MouseCamera::Reset();
            break;
        }

        case SDL_EVENT_WINDOW_FOCUS_GAINED:
        {
            // Reset mouse state on focus gain
            s_lastMouseMovement = std::chrono::steady_clock::now();
            break;
        }

        case SDL_USER_PLAYER_CHAR:
        {
            for (auto& controller : g_controllers)
                SetControllerTimeOfDayLED(controller, static_cast<EPlayerCharacter>(event->user.code));

            break;
        }
    }

    return true;
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
    SDL_SetHint(SDL_HINT_JOYSTICK_ENHANCED_REPORTS, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS5, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS5_PLAYER_LED, "1");
    // PS5 rumble hint merged into SDL_HINT_JOYSTICK_ENHANCED_REPORTS above
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_WII, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_STEAM, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_STEAMDECK, "1");
    SDL_SetHint(SDL_HINT_XINPUT_ENABLED, "1");
    
    // SDL3: SDL_HINT_GAMECONTROLLER_USE_BUTTON_LABELS removed; button labels are always positional

    SDL_InitSubSystem(SDL_INIT_EVENTS);
    SDL_AddEventWatch(HID_OnSDLEvent, nullptr);

    SDL_InitSubSystem(SDL_INIT_GAMEPAD);

    // Load controller mappings from SDL_GameControllerDB
    if (int mappings = SDL_AddGamepadMappingsFromFile("gamecontrollerdb.txt"); mappings > 0) {
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
    bool shift = (mod & SDL_KMOD_SHIFT) != 0;
    
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
    uint16_t virtualKey = SDLScancodeToVirtualKey(key.scancode, key.mod);
    if (virtualKey == 0) return;
    
    hid::KeystrokeEvent event;
    event.virtualKey = virtualKey;
    event.unicode = SDLScancodeToUnicode(key.scancode, key.mod);
    event.userIndex = 0;
    
    if (key.type == SDL_EVENT_KEY_DOWN) {
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

// ============================================================================
// Motion Sensing API Implementation
// ============================================================================

bool hid::HasMotionSensor()
{
    if (!g_activeController)
        return false;
    
    return g_activeController->hasGyro || g_activeController->hasAccel;
}

void hid::SetMotionSensorEnabled(bool enabled)
{
    g_motionSensorEnabled = enabled;
    
    if (g_activeController) {
        g_activeController->SetMotionEnabled(enabled);
    }
}

bool hid::IsMotionSensorEnabled()
{
    return g_motionSensorEnabled;
}

const hid::MotionState& hid::GetMotionState()
{
    return g_motionState;
}

void hid::UpdateMotionState()
{
    if (!g_activeController || !g_motionSensorEnabled) {
        g_motionState = {};
        return;
    }
    
    g_activeController->PollMotion(g_motionState);
}

void hid::ResetMotionOrientation()
{
    if (g_activeController) {
        g_activeController->ResetMotionOrientation();
    }
    
    g_motionState.pitch = 0.0f;
    g_motionState.roll = 0.0f;
    g_motionState.yaw = 0.0f;
}

// ============================================================================
// Light Bar API Implementation (DualShock 4 / DualSense)
// ============================================================================

bool hid::HasLightBar()
{
    if (!g_activeController || !g_activeController->controller)
        return false;
    
    // DualShock 4 and DualSense have light bars
    auto type = SDL_GetGamepadType(g_activeController->controller);
    return type == SDL_GAMEPAD_TYPE_PS4 || type == SDL_GAMEPAD_TYPE_PS5;
}

void hid::SetLightBarColor(uint8_t r, uint8_t g, uint8_t b)
{
    if (!g_activeController || !g_activeController->controller)
        return;
    
    g_activeController->SetLED(r, g, b);
}

// ============================================================================
// Touchpad API Implementation (DualShock 4 / DualSense)
// ============================================================================

static hid::TouchpadState g_touchpadState{};
static float g_lastTouchX = 0.0f;
static float g_lastTouchY = 0.0f;
static uint64_t g_lastTouchTime = 0;

bool hid::HasTouchpad()
{
    if (!g_activeController || !g_activeController->controller)
        return false;
    
    // DualShock 4 and DualSense have touchpads
    auto type = SDL_GetGamepadType(g_activeController->controller);
    return type == SDL_GAMEPAD_TYPE_PS4 || type == SDL_GAMEPAD_TYPE_PS5;
}

const hid::TouchpadState& hid::GetTouchpadState()
{
    return g_touchpadState;
}

void hid::UpdateTouchpadState()
{
    if (!g_activeController || !g_activeController->controller || !HasTouchpad()) {
        g_touchpadState = {};
        return;
    }
    
    // SDL3: touchpad API always available
    bool state;
    float x, y, pressure;
    
    // Finger 0
    if (SDL_GetGamepadTouchpadFinger(g_activeController->controller, 0, 0, &state, &x, &y, &pressure)) {
        g_touchpadState.finger0Down = state;
        g_touchpadState.finger0X = x;
        g_touchpadState.finger0Y = y;
        
        // Calculate swipe velocity
        if (g_touchpadState.finger0Down) {
            uint64_t now = SDL_GetTicks();
            float dt = (now - g_lastTouchTime) / 1000.0f;
            
            if (dt > 0.0f && dt < 0.1f && g_lastTouchTime > 0) {
                g_touchpadState.swipeVelocityX = (x - g_lastTouchX) / dt;
                g_touchpadState.swipeVelocityY = (y - g_lastTouchY) / dt;
                
                // Detect swipe gesture (velocity threshold)
                float swipeSpeed = sqrtf(g_touchpadState.swipeVelocityX * g_touchpadState.swipeVelocityX +
                                         g_touchpadState.swipeVelocityY * g_touchpadState.swipeVelocityY);
                g_touchpadState.isSwiping = swipeSpeed > 2.0f;
            }
            
            g_lastTouchX = x;
            g_lastTouchY = y;
            g_lastTouchTime = now;
        } else {
            g_touchpadState.swipeVelocityX = 0.0f;
            g_touchpadState.swipeVelocityY = 0.0f;
            g_touchpadState.isSwiping = false;
            g_lastTouchTime = 0;
        }
    }
    
    // Finger 1 (multi-touch)
    if (SDL_GetGamepadTouchpadFinger(g_activeController->controller, 0, 1, &state, &x, &y, &pressure)) {
        g_touchpadState.finger1Down = state;
        g_touchpadState.finger1X = x;
        g_touchpadState.finger1Y = y;
    }
}

// ============================================================================
// DualSense Adaptive Trigger API Implementation
// ============================================================================

bool hid::HasAdaptiveTriggers()
{
    if (!g_activeController || !g_activeController->controller)
        return false;
    
    // Only DualSense has adaptive triggers
    auto type = SDL_GetGamepadType(g_activeController->controller);
    return type == SDL_GAMEPAD_TYPE_PS5;
}

void hid::InitDualSense()
{
    DualSense::Initialize();
}

void hid::UpdateDualSense()
{
    DualSense::Update();
}

void hid::ShutdownDualSense()
{
    DualSense::Shutdown();
}
