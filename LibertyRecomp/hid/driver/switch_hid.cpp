// Nintendo Switch native HID driver — replaces sdl_hid.cpp on Switch
// Uses libnx pad.h wrapper for controller input

#include <stdafx.h>

#ifdef LIBERTY_RECOMP_NX

#include <switch/runtime/pad.h>
#include <switch/services/hid.h>
#include <hid/hid.h>
#include <os/logger.h>
#include <cstring>
#include <mutex>
#include <queue>

// ── Internal state ───────────────────────────────────────────────────────────

static PadState g_pad{};
static XAMINPUT_GAMEPAD g_gamepadState{};
static uint32_t g_packetNumber = 0;
static bool g_connected = false;

// Motion sensing state (Switch Pro Controller / Joy-Con have SixAxis)
static hid::MotionState g_motionState{};
static bool g_motionSensorEnabled = false;

// Keystroke queue
static std::queue<hid::KeystrokeEvent> s_keystrokeQueues[4];
static std::mutex s_keystrokeMutex;

// ── Button mapping ───────────────────────────────────────────────────────────
// Positional mapping: Switch B (bottom) → Xbox A (bottom), etc.

static uint16_t MapButtons(uint64_t nxButtons)
{
    uint16_t out = 0;

    // D-pad
    if (nxButtons & HidNpadButton_Up)    out |= XAMINPUT_GAMEPAD_DPAD_UP;
    if (nxButtons & HidNpadButton_Down)  out |= XAMINPUT_GAMEPAD_DPAD_DOWN;
    if (nxButtons & HidNpadButton_Left)  out |= XAMINPUT_GAMEPAD_DPAD_LEFT;
    if (nxButtons & HidNpadButton_Right) out |= XAMINPUT_GAMEPAD_DPAD_RIGHT;

    // Shoulder buttons
    if (nxButtons & HidNpadButton_L) out |= XAMINPUT_GAMEPAD_LEFT_SHOULDER;
    if (nxButtons & HidNpadButton_R) out |= XAMINPUT_GAMEPAD_RIGHT_SHOULDER;

    // Stick clicks
    if (nxButtons & HidNpadButton_StickL) out |= XAMINPUT_GAMEPAD_LEFT_THUMB;
    if (nxButtons & HidNpadButton_StickR) out |= XAMINPUT_GAMEPAD_RIGHT_THUMB;

    // Start / Back
    if (nxButtons & HidNpadButton_Plus)  out |= XAMINPUT_GAMEPAD_START;
    if (nxButtons & HidNpadButton_Minus) out |= XAMINPUT_GAMEPAD_BACK;

    // Face buttons — positional mapping (Nintendo B=bottom=Xbox A, etc.)
    if (nxButtons & HidNpadButton_B) out |= XAMINPUT_GAMEPAD_A;  // bottom
    if (nxButtons & HidNpadButton_A) out |= XAMINPUT_GAMEPAD_B;  // right
    if (nxButtons & HidNpadButton_Y) out |= XAMINPUT_GAMEPAD_X;  // left
    if (nxButtons & HidNpadButton_X) out |= XAMINPUT_GAMEPAD_Y;  // top

    return out;
}

// ── Poll current pad state ───────────────────────────────────────────────────

static void PollPad()
{
    padUpdate(&g_pad);

    g_connected = padIsConnected(&g_pad);
    if (!g_connected) {
        std::memset(&g_gamepadState, 0, sizeof(g_gamepadState));
        return;
    }

    g_packetNumber++;

    uint64_t buttons = padGetButtons(&g_pad);
    auto& pad = g_gamepadState;

    pad.wButtons = MapButtons(buttons);

    // Analog sticks — libnx range is -32768..32767, matches XAMINPUT directly
    HidAnalogStickState stickL = padGetStickPos(&g_pad, 0);
    HidAnalogStickState stickR = padGetStickPos(&g_pad, 1);

    pad.sThumbLX = static_cast<int16_t>(stickL.x);
    pad.sThumbLY = static_cast<int16_t>(stickL.y);
    pad.sThumbRX = static_cast<int16_t>(stickR.x);
    pad.sThumbRY = static_cast<int16_t>(stickR.y);

    // ZL / ZR are digital on Switch — map as full trigger press
    pad.bLeftTrigger  = (buttons & HidNpadButton_ZL) ? 255 : 0;
    pad.bRightTrigger = (buttons & HidNpadButton_ZR) ? 255 : 0;

    // SixAxis motion (if enabled)
    if (g_motionSensorEnabled) {
        HidSixAxisSensorState sixaxis{};
        u64 count = 0;
        // Use the first available SixAxis handle
        HidSixAxisSensorHandle handles[2]{};
        hidGetSixAxisSensorHandles(handles, 2, HidNpadIdType_No1, HidNpadStyleTag_NpadFullKey);
        hidGetSixAxisSensorStates(handles[0], &sixaxis, 1, &count);

        if (count > 0) {
            g_motionState.hasGyro = true;
            g_motionState.hasAccel = true;
            g_motionState.isCalibrated = true;
            g_motionState.timestamp = sixaxis.timestamp;

            g_motionState.gyroX = sixaxis.angular_velocity.x;
            g_motionState.gyroY = sixaxis.angular_velocity.y;
            g_motionState.gyroZ = sixaxis.angular_velocity.z;

            g_motionState.accelX = sixaxis.acceleration.x;
            g_motionState.accelY = sixaxis.acceleration.y;
            g_motionState.accelZ = sixaxis.acceleration.z;

            // Direction from the SixAxis quaternion
            const auto& q = sixaxis.direction;
            // q is a 3x3 rotation matrix (DirectionState), extract Euler angles
            g_motionState.pitch = asinf(-q.direction[2].x);
            g_motionState.roll  = atan2f(q.direction[2].y, q.direction[2].z);
            g_motionState.yaw   = atan2f(q.direction[1].x, q.direction[0].x);
        }
    }
}

// ── Public hid:: interface ───────────────────────────────────────────────────

namespace hid {

EInputDevice g_inputDevice = EInputDevice::Xbox; // Shows Xbox prompts by default
EInputDevice g_inputDeviceController = EInputDevice::Xbox;
EInputDeviceExplicit g_inputDeviceExplicit = EInputDeviceExplicit::SwitchPro;

uint16_t g_prohibitedButtons = 0;
bool g_isLeftStickProhibited = false;
bool g_isRightStickProhibited = false;

void Init()
{
    // Configure for 1 player, accepting Pro Controller and dual Joy-Con styles
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    padInitializeDefault(&g_pad);

    g_connected = true;
    LOG_INFO("Switch pad initialized via libnx");
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
    if (dwUserIndex != 0)
        return 1;

    // Switch Pro Controller supports HD Rumble via hidSendVibrationValue
    HidVibrationDeviceHandle vibDevHandles[2]{};
    hidInitializeVibrationDevices(vibDevHandles, 2, HidNpadIdType_No1, HidNpadStyleTag_NpadFullKey);

    HidVibrationValue vibL{}, vibR{};
    float leftAmp  = static_cast<float>(pVibration->wLeftMotorSpeed) / 65535.0f;
    float rightAmp = static_cast<float>(pVibration->wRightMotorSpeed) / 65535.0f;

    // Low-frequency motor (left)
    vibL.amp_low  = leftAmp;
    vibL.freq_low = 160.0f;
    vibL.amp_high = leftAmp * 0.5f;
    vibL.freq_high = 320.0f;

    // High-frequency motor (right)
    vibR.amp_low  = rightAmp * 0.5f;
    vibR.freq_low = 160.0f;
    vibR.amp_high = rightAmp;
    vibR.freq_high = 320.0f;

    hidSendVibrationValues(vibDevHandles, (HidVibrationValue[]){vibL, vibR}, 2);
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

bool HasMotionSensor()
{
    // Pro Controller and Joy-Con both have SixAxis
    return true;
}

void SetMotionSensorEnabled(bool enabled)
{
    g_motionSensorEnabled = enabled;
    HidSixAxisSensorHandle handles[2]{};
    hidGetSixAxisSensorHandles(handles, 2, HidNpadIdType_No1, HidNpadStyleTag_NpadFullKey);
    if (enabled) {
        hidStartSixAxisSensor(handles[0]);
        hidStartSixAxisSensor(handles[1]);
    } else {
        hidStopSixAxisSensor(handles[0]);
        hidStopSixAxisSensor(handles[1]);
    }
}

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
}

// ── Light Bar (Switch has no light bar) ──────────────────────────────────────

bool HasLightBar() { return false; }
void SetLightBarColor(uint8_t, uint8_t, uint8_t) {}

// ── Touchpad (Switch has no touchpad) ────────────────────────────────────────

static hid::TouchpadState g_emptyTouchpad{};

bool HasTouchpad() { return false; }
const TouchpadState& GetTouchpadState() { return g_emptyTouchpad; }
void UpdateTouchpadState() {}

// ── Adaptive Triggers (Switch has no adaptive triggers) ──────────────────────

bool HasAdaptiveTriggers() { return false; }

void InitDualSense() {}
void UpdateDualSense() {}
void ShutdownDualSense() {}

} // namespace hid

#endif // LIBERTY_RECOMP_NX
