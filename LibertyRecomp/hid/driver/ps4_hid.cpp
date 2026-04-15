// PS4 native HID driver — replaces sdl_hid.cpp on PlayStation 4
// Uses OpenOrbis scePad API for DualShock 4 input

#include <stdafx.h>

#ifdef LIBERTY_RECOMP_PS4

#include <orbis/Pad.h>
#include <hid/hid.h>
#include <os/logger.h>
#include <kernel/xam.h>
#include <cstring>
#include <cmath>
#include <mutex>
#include <queue>

// ── Internal state ───────────────────────────────────────────────────────────

static int32_t g_padHandle = -1;
static XAMINPUT_GAMEPAD g_gamepadState{};
static uint32_t g_packetNumber = 0;
static bool g_connected = false;

// Motion sensing state
static hid::MotionState g_motionState{};
static bool g_motionSensorEnabled = false;

// Touchpad state
static hid::TouchpadState g_touchpadState{};
static float g_prevTouchX = 0.0f;
static float g_prevTouchY = 0.0f;
static uint64_t g_lastTouchTimestamp = 0;

// Light bar state
static uint8_t g_lightBarR = 0, g_lightBarG = 0, g_lightBarB = 64;

// Keystroke queue
static std::queue<hid::KeystrokeEvent> s_keystrokeQueues[4];
static std::mutex s_keystrokeMutex;

// ── Button mapping: OrbisPadButton → XAMINPUT_GAMEPAD ────────────────────────

static uint16_t MapButtons(uint32_t orbisButtons)
{
    uint16_t out = 0;

    if (orbisButtons & ORBIS_PAD_BUTTON_UP)       out |= XAMINPUT_GAMEPAD_DPAD_UP;
    if (orbisButtons & ORBIS_PAD_BUTTON_DOWN)     out |= XAMINPUT_GAMEPAD_DPAD_DOWN;
    if (orbisButtons & ORBIS_PAD_BUTTON_LEFT)     out |= XAMINPUT_GAMEPAD_DPAD_LEFT;
    if (orbisButtons & ORBIS_PAD_BUTTON_RIGHT)    out |= XAMINPUT_GAMEPAD_DPAD_RIGHT;

    if (orbisButtons & ORBIS_PAD_BUTTON_OPTIONS)  out |= XAMINPUT_GAMEPAD_START;
    if (orbisButtons & ORBIS_PAD_BUTTON_TOUCH_PAD) out |= XAMINPUT_GAMEPAD_BACK;

    if (orbisButtons & ORBIS_PAD_BUTTON_L3)       out |= XAMINPUT_GAMEPAD_LEFT_THUMB;
    if (orbisButtons & ORBIS_PAD_BUTTON_R3)       out |= XAMINPUT_GAMEPAD_RIGHT_THUMB;

    if (orbisButtons & ORBIS_PAD_BUTTON_L1)       out |= XAMINPUT_GAMEPAD_LEFT_SHOULDER;
    if (orbisButtons & ORBIS_PAD_BUTTON_R1)       out |= XAMINPUT_GAMEPAD_RIGHT_SHOULDER;

    // Face buttons — positional mapping (Cross=bottom=A, Circle=right=B, etc.)
    if (orbisButtons & ORBIS_PAD_BUTTON_CROSS)    out |= XAMINPUT_GAMEPAD_A;
    if (orbisButtons & ORBIS_PAD_BUTTON_CIRCLE)   out |= XAMINPUT_GAMEPAD_B;
    if (orbisButtons & ORBIS_PAD_BUTTON_SQUARE)   out |= XAMINPUT_GAMEPAD_X;
    if (orbisButtons & ORBIS_PAD_BUTTON_TRIANGLE) out |= XAMINPUT_GAMEPAD_Y;

    return out;
}

// Convert stick 0-255 range (128=center) to int16 -32768..32767
static int16_t ConvertStick(uint8_t raw)
{
    int val = (static_cast<int>(raw) - 128) * 256;
    if (val > 32767) val = 32767;
    if (val < -32768) val = -32768;
    return static_cast<int16_t>(val);
}

// ── Poll current pad state ───────────────────────────────────────────────────

static void PollPad()
{
    OrbisPadData data{};
    int32_t ret = scePadReadState(g_padHandle, &data);

    if (ret < 0 || !data.connected) {
        g_connected = false;
        std::memset(&g_gamepadState, 0, sizeof(g_gamepadState));
        return;
    }

    g_connected = true;
    g_packetNumber++;

    auto& pad = g_gamepadState;
    pad.wButtons = MapButtons(data.buttons);

    // Analog sticks (0-255, 128=center) → int16 (-32768..32767)
    pad.sThumbLX = ConvertStick(data.leftStick.x);
    pad.sThumbLY = static_cast<int16_t>(~ConvertStick(data.leftStick.y)); // Y inverted
    pad.sThumbRX = ConvertStick(data.rightStick.x);
    pad.sThumbRY = static_cast<int16_t>(~ConvertStick(data.rightStick.y));

    // Analog triggers (0-255 direct mapping)
    pad.bLeftTrigger  = data.analogButtons.l2;
    pad.bRightTrigger = data.analogButtons.r2;

    // Motion sensors — always available on DS4
    if (g_motionSensorEnabled) {
        g_motionState.hasGyro = true;
        g_motionState.hasAccel = true;
        g_motionState.isCalibrated = true;
        g_motionState.timestamp = data.timestamp;

        // vel is angular velocity in rad/s
        g_motionState.gyroX = data.vel.x;
        g_motionState.gyroY = data.vel.y;
        g_motionState.gyroZ = data.vel.z;

        // acell is acceleration in g-forces
        g_motionState.accelX = data.acell.x;
        g_motionState.accelY = data.acell.y;
        g_motionState.accelZ = data.acell.z;

        // Orientation from quaternion (quat)
        g_motionState.pitch = asinf(2.0f * (data.quat.w * data.quat.x
                              - data.quat.y * data.quat.z));
        g_motionState.roll  = atan2f(2.0f * (data.quat.w * data.quat.z
                              + data.quat.x * data.quat.y),
                              1.0f - 2.0f * (data.quat.z * data.quat.z
                              + data.quat.x * data.quat.x));
        g_motionState.yaw   = atan2f(2.0f * (data.quat.w * data.quat.y
                              + data.quat.z * data.quat.x),
                              1.0f - 2.0f * (data.quat.x * data.quat.x
                              + data.quat.y * data.quat.y));
    }

    // Touchpad
    if (data.touch.fingers > 0) {
        const auto& t0 = data.touch.touch[0];
        g_touchpadState.finger0Down = true;
        g_touchpadState.finger0X = static_cast<float>(t0.x) / 1920.0f;
        g_touchpadState.finger0Y = static_cast<float>(t0.y) / 943.0f;
    } else {
        g_touchpadState.finger0Down = false;
    }

    if (data.touch.fingers > 1) {
        const auto& t1 = data.touch.touch[1];
        g_touchpadState.finger1Down = true;
        g_touchpadState.finger1X = static_cast<float>(t1.x) / 1920.0f;
        g_touchpadState.finger1Y = static_cast<float>(t1.y) / 943.0f;
    } else {
        g_touchpadState.finger1Down = false;
    }

    // Swipe detection
    if (g_touchpadState.finger0Down && g_lastTouchTimestamp > 0) {
        float dt = static_cast<float>(data.timestamp - g_lastTouchTimestamp) / 1000000.0f;
        if (dt > 0.0f && dt < 0.1f) {
            g_touchpadState.swipeVelocityX = (g_touchpadState.finger0X - g_prevTouchX) / dt;
            g_touchpadState.swipeVelocityY = (g_touchpadState.finger0Y - g_prevTouchY) / dt;
            g_touchpadState.isSwiping = (fabsf(g_touchpadState.swipeVelocityX) > 2.0f
                                      || fabsf(g_touchpadState.swipeVelocityY) > 2.0f);
        }
    } else {
        g_touchpadState.swipeVelocityX = 0.0f;
        g_touchpadState.swipeVelocityY = 0.0f;
        g_touchpadState.isSwiping = false;
    }
    g_prevTouchX = g_touchpadState.finger0X;
    g_prevTouchY = g_touchpadState.finger0Y;
    g_lastTouchTimestamp = data.timestamp;
}

// ── Public hid:: interface ───────────────────────────────────────────────────

namespace hid {

EInputDevice g_inputDevice = EInputDevice::PlayStation;
EInputDevice g_inputDeviceController = EInputDevice::PlayStation;
EInputDeviceExplicit g_inputDeviceExplicit = EInputDeviceExplicit::DualShock4;

uint16_t g_prohibitedButtons = 0;
bool g_isLeftStickProhibited = false;
bool g_isRightStickProhibited = false;

void Init()
{
    int32_t ret = scePadInit();
    if (ret < 0) {
        LOGFN_ERROR("scePadInit failed: {:#x}", static_cast<unsigned>(ret));
        return;
    }

    // PS4 user ID 1 (primary), standard pad, index 0
    g_padHandle = scePadOpen(0x01, ORBIS_PAD_PORT_TYPE_STANDARD, 0, nullptr);
    if (g_padHandle < 0) {
        LOGFN_ERROR("scePadOpen failed: {:#x}", static_cast<unsigned>(g_padHandle));
        return;
    }

    g_connected = true;
    LOG_INFO("PS4 pad initialized");
}

uint32_t GetState(uint32_t dwUserIndex, XAMINPUT_STATE* pState)
{
    if (dwUserIndex != 0 || !g_connected)
        return 1; // ERROR_DEVICE_NOT_CONNECTED

    PollPad();

    pState->dwPacketNumber = g_packetNumber;
    pState->Gamepad = g_gamepadState;

    // Apply prohibited inputs
    pState->Gamepad.wButtons &= ~g_prohibitedButtons;
    if (g_isLeftStickProhibited) {
        pState->Gamepad.sThumbLX = 0;
        pState->Gamepad.sThumbLY = 0;
    }
    if (g_isRightStickProhibited) {
        pState->Gamepad.sThumbRX = 0;
        pState->Gamepad.sThumbRY = 0;
    }

    return 0;
}

uint32_t SetState(uint32_t dwUserIndex, XAMINPUT_VIBRATION* pVibration)
{
    if (dwUserIndex != 0 || g_padHandle < 0)
        return 1;

    OrbisPadVibeParam vibe{};
    vibe.lgMotor = static_cast<uint8_t>(pVibration->wLeftMotorSpeed >> 8);
    vibe.smMotor = static_cast<uint8_t>(pVibration->wRightMotorSpeed >> 8);
    scePadSetVibration(g_padHandle, &vibe);
    return 0;
}

uint32_t GetCapabilities(uint32_t dwUserIndex, XAMINPUT_CAPABILITIES* pCaps)
{
    if (dwUserIndex != 0)
        return 1;

    std::memset(pCaps, 0, sizeof(*pCaps));
    pCaps->Type = XAMINPUT_DEVTYPE_GAMEPAD;
    pCaps->SubType = XAMINPUT_DEVSUBTYPE_GAMEPAD;
    pCaps->Gamepad = g_gamepadState;
    return 0;
}

void SetProhibitedInputs(uint16_t wButtons, bool leftStick, bool rightStick)
{
    g_prohibitedButtons = wButtons;
    g_isLeftStickProhibited = leftStick;
    g_isRightStickProhibited = rightStick;
}

bool IsInputAllowed() { return true; }
bool IsInputDeviceController() { return true; }

int32_t GetMouseWheelDelta() { return 0; }
void ResetMouseWheelDelta() {}

void EnqueueKeystroke(const KeystrokeEvent& event)
{
    std::lock_guard<std::mutex> lock(s_keystrokeMutex);
    if (s_keystrokeQueues[event.userIndex].size() < 64)
        s_keystrokeQueues[event.userIndex].push(event);
}

bool DequeueKeystroke(uint8_t userIndex, KeystrokeEvent& outEvent)
{
    std::lock_guard<std::mutex> lock(s_keystrokeMutex);
    if (userIndex >= 4 || s_keystrokeQueues[userIndex].empty())
        return false;
    outEvent = s_keystrokeQueues[userIndex].front();
    s_keystrokeQueues[userIndex].pop();
    return true;
}

void ClearKeystrokeQueue(uint8_t userIndex)
{
    std::lock_guard<std::mutex> lock(s_keystrokeMutex);
    if (userIndex < 4)
        while (!s_keystrokeQueues[userIndex].empty())
            s_keystrokeQueues[userIndex].pop();
}

// ── Motion ───────────────────────────────────────────────────────────────────

bool HasMotionSensor() { return true; } // DS4 always has motion

void SetMotionSensorEnabled(bool enabled) { g_motionSensorEnabled = enabled; }
bool IsMotionSensorEnabled() { return g_motionSensorEnabled; }

const MotionState& GetMotionState() { return g_motionState; }

void UpdateMotionState()
{
    // Motion is already updated in PollPad()
}

void ResetMotionOrientation()
{
    g_motionState.pitch = 0.0f;
    g_motionState.roll = 0.0f;
    g_motionState.yaw = 0.0f;
    scePadResetOrientation(g_padHandle);
}

// ── Light Bar ────────────────────────────────────────────────────────────────

bool HasLightBar() { return true; } // DS4 always has light bar

void SetLightBarColor(uint8_t r, uint8_t g, uint8_t b)
{
    if (g_padHandle < 0) return;
    OrbisPadColor color{r, g, b, 255};
    scePadSetLightBar(g_padHandle, &color);
    g_lightBarR = r; g_lightBarG = g; g_lightBarB = b;
}

// ── Touchpad ─────────────────────────────────────────────────────────────────

bool HasTouchpad() { return true; } // DS4 always has touchpad

const TouchpadState& GetTouchpadState() { return g_touchpadState; }

void UpdateTouchpadState()
{
    // Touchpad is already updated in PollPad()
}

// ── Adaptive Triggers (DS4 does NOT have adaptive triggers) ──────────────────

bool HasAdaptiveTriggers() { return false; }

void InitDualSense() {}
void UpdateDualSense() {}
void ShutdownDualSense() {}

} // namespace hid

#endif // LIBERTY_RECOMP_PS4
