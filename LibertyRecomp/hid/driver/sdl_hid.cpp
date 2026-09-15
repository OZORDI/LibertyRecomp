#include <stdafx.h>
#include <SDL3/SDL.h>
#include <user/config.h>
#include <hid/hid.h>
#include <hid/mouse_camera.h>
#include <os/logger.h>
#include <ui/game_window.h>
#include <kernel/xam.h>
#include <kernel/button_prompts.h>
#include <app.h>
#include <chrono>
#include <queue>
#include <mutex>
#include <cstring>
#include <cmath>

namespace {
// Xbox input status and virtual-key values do not depend on legacy guest memory.
constexpr uint32_t kInputSuccess = 0x0;
constexpr uint32_t kInputBadArguments = 0xA0;
constexpr uint32_t kInputNotConnected = 0x48F;
constexpr uint32_t kKeystrokeDown = 0x0001;
constexpr uint32_t kKeystrokeUp = 0x0002;
constexpr uint32_t kKeystrokeRepeat = 0x0004;
constexpr uint32_t kVirtualBack = 0x08;
constexpr uint32_t kVirtualTab = 0x09;
constexpr uint32_t kVirtualReturn = 0x0D;
constexpr uint32_t kVirtualShift = 0x10;
constexpr uint32_t kVirtualControl = 0x11;
constexpr uint32_t kVirtualEscape = 0x1B;
constexpr uint32_t kVirtualSpace = 0x20;
constexpr uint32_t kVirtualLeft = 0x25;
constexpr uint32_t kVirtualUp = 0x26;
constexpr uint32_t kVirtualRight = 0x27;
constexpr uint32_t kVirtualDown = 0x28;
constexpr uint32_t kVirtualDelete = 0x2E;
} // namespace

#if defined(GTA4_SONY_LEGACY_OWNER)
#include <rex/input/input_driver.h>
#include <rex/input/input_system.h>
#include <rex/input/sony_feedback.h>

namespace sony = rex::input::sony;

namespace {

using rex::X_RESULT;
using rex::X_STATUS;

// SDL handles stay with this file's event/main-thread owner. Guest input only
// copies these values or posts a rumble request under the snapshot mutex.
struct LegacyInputSnapshot {
    bool connected = false;
    bool rumble = false;
    bool wireless = false;
    bool physicalBack = false;
    bool touchClick = false;
    uint64_t generation = 0;
    rex::input::X_INPUT_STATE state{};
    uint16_t nativeLow = 0, nativeHigh = 0;
    uint64_t nativeUntil = 0;
};

std::mutex g_legacySnapshotMutex;
std::recursive_mutex g_legacyControllerMutex;
// SDL event watches run under SDL's event/joystick lock. Always acquire that
// recursive lock before our controller lock, including main-thread output.
struct LegacySDLJoystickLock {
    LegacySDLJoystickLock() { SDL_LockJoysticks(); }
    ~LegacySDLJoystickLock() { SDL_UnlockJoysticks(); }
};
class LegacyControllerLock {
public:
    LegacyControllerLock() : controllerLock_(g_legacyControllerMutex) {}
private:
    LegacySDLJoystickLock joystickLock_;
    std::lock_guard<std::recursive_mutex> controllerLock_;
};
std::array<LegacyInputSnapshot, 4> g_legacySnapshots;
uint64_t g_legacyDeviceGeneration = 0;
bool g_legacyFocused = true;
bool g_legacySonyRunning = true;

struct LegacySonyOutput {
    sony::Device device{};
    sony::EffectPacket packet{};
    std::array<uint8_t, 3> color{};
    uint16_t appliedLow = 0, appliedHigh = 0;
    bool lightSet = false;
    bool triggersSet = false;
    unsigned rumbleFailures = 0, lightFailures = 0, triggerFailures = 0;
    uint64_t nextRumble = 0, rumbleRetryAt = 0, retryAt = 0;
};

// Put the existing default input system behind the real legacy SDL driver.
// Its NOP fallback must not hide the physical controller's capabilities.
class DefaultInputFallback final : public rex::input::InputDriver {
public:
    explicit DefaultInputFallback(bool toolMode)
        : InputDriver(nullptr, 0), input_(rex::input::CreateDefaultInputSystem(toolMode)) {}
    rex::X_STATUS Setup() override { return input_->Setup(); }
    const char* trace_name() const override { return "legacy-default-fallback"; }
    rex::X_RESULT GetCapabilities(uint32_t user, uint32_t flags,
                                  rex::input::X_INPUT_CAPABILITIES* out) override {
        return input_->GetCapabilities(user, flags, out);
    }
    rex::X_RESULT GetState(uint32_t user, rex::input::X_INPUT_STATE* out) override {
        return input_->GetState(user, out);
    }
    rex::X_RESULT SetState(uint32_t user, rex::input::X_INPUT_VIBRATION* value) override {
        return input_->SetState(user, value);
    }
    rex::X_RESULT GetKeystroke(uint32_t user, uint32_t flags,
                               rex::input::X_INPUT_KEYSTROKE* out) override {
        return input_->GetKeystroke(user, flags, out);
    }
    bool TryGetMotionState(uint32_t user, rex::input::MotionState* out) override {
        return input_->TryGetMotionState(user, out);
    }
private:
    std::unique_ptr<rex::input::InputSystem> input_;
};

class LegacySDLInputDriver final : public rex::input::InputDriver {
public:
    LegacySDLInputDriver() : InputDriver(nullptr, 0) {}
    rex::X_STATUS Setup() override { return X_STATUS_SUCCESS; }
    const char* trace_name() const override { return "legacy-sdl-owner"; }

    rex::X_RESULT GetCapabilities(uint32_t user, uint32_t,
                                  rex::input::X_INPUT_CAPABILITIES* out) override {
        std::lock_guard lock(g_legacySnapshotMutex);
        if (user >= g_legacySnapshots.size() || !g_legacySnapshots[user].connected)
            return X_ERROR_DEVICE_NOT_CONNECTED;
        if (out) {
            const auto& pad = g_legacySnapshots[user];
            *out = {};
            out->type = 1;
            out->sub_type = 1;
            out->flags = (pad.rumble ? rex::input::X_INPUT_CAPS_FFB_SUPPORTED : 0) |
                         (pad.wireless ? rex::input::X_INPUT_CAPS_WIRELESS : 0);
            out->gamepad.buttons = 0xF3FF;
            out->gamepad.left_trigger = out->gamepad.right_trigger = 255;
            out->gamepad.thumb_lx = out->gamepad.thumb_ly = 32767;
            out->gamepad.thumb_rx = out->gamepad.thumb_ry = 32767;
            out->vibration.left_motor_speed = pad.rumble ? 65535 : 0;
            out->vibration.right_motor_speed = pad.rumble ? 65535 : 0;
        }
        return X_ERROR_SUCCESS;
    }

    rex::X_RESULT GetState(uint32_t user, rex::input::X_INPUT_STATE* out) override {
        LegacyInputSnapshot snapshot;
        bool focused;
        {
            std::lock_guard lock(g_legacySnapshotMutex);
            if (user >= g_legacySnapshots.size() || !g_legacySnapshots[user].connected)
                return X_ERROR_DEVICE_NOT_CONNECTED;
            snapshot = g_legacySnapshots[user];
            focused = g_legacyFocused;
        }
        if (out) {
            *out = snapshot.state;
            if (!focused) out->gamepad = {};
            else {
                // Re-evaluate the claim at the guest poll so context changes
                // don't require another SDL button event to update Back.
                uint16_t buttons = out->gamepad.buttons;
                buttons &= ~rex::input::X_INPUT_GAMEPAD_BACK;
                if (snapshot.physicalBack ||
                    (snapshot.touchClick && !sony::GetService().ClaimsClick(
                         user, SDL_GetTicks(), sony::GetOptions()))) {
                    buttons |= rex::input::X_INPUT_GAMEPAD_BACK;
                }
                out->gamepad.buttons = buttons;
            }
        }
        return X_ERROR_SUCCESS;
    }

    rex::X_RESULT SetState(uint32_t user, rex::input::X_INPUT_VIBRATION* value) override {
        if (!value) return X_ERROR_BAD_ARGUMENTS;
        std::lock_guard lock(g_legacySnapshotMutex);
        if (user >= g_legacySnapshots.size() || !g_legacySnapshots[user].connected ||
            !g_legacySnapshots[user].rumble) return X_ERROR_DEVICE_NOT_CONNECTED;
        auto& pad = g_legacySnapshots[user];
        pad.nativeLow = g_legacyFocused ? static_cast<uint16_t>(value->left_motor_speed) : 0;
        pad.nativeHigh = g_legacyFocused ? static_cast<uint16_t>(value->right_motor_speed) : 0;
        pad.nativeUntil = SDL_GetTicks() + 5000;
        return X_ERROR_SUCCESS;
    }

    rex::X_RESULT GetKeystroke(uint32_t user, uint32_t,
                               rex::input::X_INPUT_KEYSTROKE* out) override {
        if (user >= g_legacySnapshots.size()) return X_ERROR_DEVICE_NOT_CONNECTED;
        if (!out) return X_ERROR_BAD_ARGUMENTS;
        hid::KeystrokeEvent event{};
        if (!hid::DequeueKeystroke(static_cast<uint8_t>(user), event)) return X_ERROR_EMPTY;
        *out = {};
        out->virtual_key = event.virtualKey;
        out->unicode = event.unicode;
        out->flags = event.flags;
        out->user_index = event.userIndex;
        return X_ERROR_SUCCESS;
    }
};

} // namespace
#endif

#include <rex/platform.h>
#if REX_PLATFORM_IOS
#include <os/ios/haptics_ios.h>
#define LR_USE_IOS_HAPTICS 1
#endif

#ifndef LR_USE_IOS_HAPTICS
#define LR_USE_IOS_HAPTICS 0
#endif

#if defined(__ANDROID__)
#include <android/api-level.h>
#include <os/android/vibration_android.h>
#define LR_USE_ANDROID_VIBRATION 1
// Runtime SDL3 property exposing the Android InputDevice ID for a joystick.
#ifndef SDL_PROP_JOYSTICK_ANDROID_DEVICE_ID_NUMBER
#define SDL_PROP_JOYSTICK_ANDROID_DEVICE_ID_NUMBER "SDL.joystick.android.device_id"
#endif
#endif

#ifndef LR_USE_ANDROID_VIBRATION
#define LR_USE_ANDROID_VIBRATION 0
#endif

#define TRANSLATE_INPUT(S, X) SDL_GetGamepadButton(controller, S) << FirstBitLow(X)
#define VIBRATION_TIMEOUT_MS 5000

// Motion sensing state (global for active controller)
static hid::MotionState g_motionState{};
static bool g_motionSensorEnabled = false;

class Controller;
#if defined(GTA4_SONY_LEGACY_OWNER)
static void NeutralizeSonyController(Controller& controller, bool disconnect);
#endif

class Controller
{
public:
    SDL_Gamepad* controller{};
    SDL_Joystick* joystick{};
    SDL_JoystickID id{ 0 };
    XAMINPUT_GAMEPAD state{};
    XAMINPUT_VIBRATION vibration{ 0, 0 };
    int index{};
#if defined(GTA4_SONY_LEGACY_OWNER)
    LegacySonyOutput sonyOutput{};
#endif
    
    // Motion sensor state
    bool hasGyro{};
    bool hasAccel{};
    bool motionEnabled{};
    
    // For orientation integration
    float integratedPitch{};
    float integratedRoll{};
    float integratedYaw{};
    uint64_t lastMotionTimestamp{};

#if LR_USE_IOS_HAPTICS
    // True when the native Core Haptics engine came up successfully for this pad.
    // When false we fall back to SDL_RumbleGamepad.
    bool iosHapticsReady{};
#endif

#if LR_USE_ANDROID_VIBRATION
    // Android InputDevice ID (from SDL joystick properties).
    // -1 means unknown — fall back to SDL_RumbleGamepad.
    int32_t androidDeviceId{ -1 };
    // True when the platform API supports VibratorManager (API 31+).
    bool androidVibratorReady{};
#endif

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

#if LR_USE_IOS_HAPTICS
        // Bring up the native Core Haptics engine for this pad.
        // The wrapper sets a flag internally on failure; we re-query by attempting
        // a no-op rumble — simpler: treat init as best-effort and let the wrapper
        // silently no-op if the underlying GCController has no haptics.
        ios_haptics_init(controller);
        iosHapticsReady = true;
#endif

#if LR_USE_ANDROID_VIBRATION
        // Grab the Android InputDevice ID SDL attached to this joystick so we can
        // route rumble through Android's VibratorManager (API 31+). If the
        // property is missing (e.g. SDL couldn't resolve it), fall back to SDL.
        if (joystick) {
            SDL_PropertiesID props = SDL_GetJoystickProperties(joystick);
            if (props != 0) {
                Sint64 devId = SDL_GetNumberProperty(props, SDL_PROP_JOYSTICK_ANDROID_DEVICE_ID_NUMBER, -1);
                androidDeviceId = static_cast<int32_t>(devId);
            }
        }
        // VibratorManager.getVibration(int) arrived in API 31 (Android 12).
        androidVibratorReady = (android_get_device_api_level() >= 31) && (androidDeviceId >= 0);
        if (androidVibratorReady) {
            LOGFN("Android vibration routing enabled for InputDevice {}", androidDeviceId);
        }
#endif
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
#if defined(GTA4_SONY_LEGACY_OWNER)
        NeutralizeSonyController(*this, true);
#endif
        
        // Disable motion sensors before closing
        if (motionEnabled) {
            SetMotionEnabled(false);
        }

#if LR_USE_IOS_HAPTICS
        if (iosHapticsReady) {
            ios_haptics_shutdown(controller);
            iosHapticsReady = false;
        }
#endif

#if LR_USE_ANDROID_VIBRATION
        // Make sure we leave the native vibrator silent when the pad goes away.
        if (androidVibratorReady && androidDeviceId >= 0) {
            android_vibration_stop(androidDeviceId);
        }
        androidVibratorReady = false;
        androidDeviceId = -1;
#endif

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
#if defined(GTA4_SONY_LEGACY_OWNER)
        if (!sony::GetService().ClaimsClick(static_cast<uint32_t>(index), SDL_GetTicks(),
                                             sony::GetOptions()))
#endif
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

#if defined(GTA4_SONY_LEGACY_OWNER)
        // Legacy callers share the main-thread output composer as well.
        std::lock_guard lock(g_legacySnapshotMutex);
        auto& snapshot = g_legacySnapshots[index];
        snapshot.nativeLow = g_legacyFocused ? vibration.wLeftMotorSpeed : 0;
        snapshot.nativeHigh = g_legacyFocused ? vibration.wRightMotorSpeed : 0;
        snapshot.nativeUntil = SDL_GetTicks() + VIBRATION_TIMEOUT_MS;
        return;
#endif

        const uint16_t low  = static_cast<uint16_t>(vibration.wLeftMotorSpeed  * 256);
        const uint16_t high = static_cast<uint16_t>(vibration.wRightMotorSpeed * 256);

#if LR_USE_IOS_HAPTICS
        if (iosHapticsReady)
        {
            ios_haptics_set_rumble(controller, low, high, VIBRATION_TIMEOUT_MS);
            return;
        }
#endif

#if LR_USE_ANDROID_VIBRATION
        if (androidVibratorReady)
        {
            // Route through Android VibratorManager (API 31+). Forward the 16-bit
            // XInput speeds directly; the native side scales as needed. Zero speed
            // on both motors means stop.
            if (vibration.wLeftMotorSpeed == 0 && vibration.wRightMotorSpeed == 0)
                android_vibration_stop(androidDeviceId);
            else
                android_vibration_set_rumble(androidDeviceId,
                                             vibration.wLeftMotorSpeed,
                                             vibration.wRightMotorSpeed,
                                             VIBRATION_TIMEOUT_MS);
            return;
        }
#endif

        SDL_RumbleGamepad(controller, low, high, VIBRATION_TIMEOUT_MS);
    }

    void SetLED(const uint8_t r, const uint8_t g, const uint8_t b) const
    {
        SDL_SetGamepadLED(controller, r, g, b);
    }
};

std::array<Controller, 4> g_controllers;
Controller* g_activeController;

#if defined(GTA4_SONY_LEGACY_OWNER)
static void RegisterSonyController(Controller& pad)
{
    const auto properties = SDL_GetGamepadProperties(pad.controller);
    const auto type = SDL_GetRealGamepadType(pad.controller);
    auto& output = pad.sonyOutput;
    auto& device = output.device;
    device.instance_id = pad.id;
    if (++g_legacyDeviceGeneration == 0) ++g_legacyDeviceGeneration;
    device.generation = g_legacyDeviceGeneration;
    device.model = type == SDL_GAMEPAD_TYPE_PS4 ? sony::Model::kDualShock4 :
                   type == SDL_GAMEPAD_TYPE_PS5 ? sony::Model::kDualSense : sony::Model::kNone;
    device.rumble = SDL_GetBooleanProperty(properties, SDL_PROP_GAMEPAD_CAP_RUMBLE_BOOLEAN, false);
    device.light = SDL_GetBooleanProperty(properties, SDL_PROP_GAMEPAD_CAP_RGB_LED_BOOLEAN, false);
    device.touchpad = SDL_GetNumGamepadTouchpads(pad.controller) > 0;
    if (device.model == sony::Model::kDualSense) {
        output.packet = sony::EncodeTriggers(sony::Resistance(0, 0), sony::Resistance(0, 0));
        device.triggers = SDL_SendGamepadEffect(pad.controller, output.packet.data(),
                                                static_cast<int>(output.packet.size()));
        output.triggersSet = device.triggers;
    }
    if (device.model != sony::Model::kNone) {
        sony::GetService().Connect(static_cast<uint32_t>(pad.index), device);
        LOGFN("sony-controller: legacy user={} instance={} generation={} model={} touch={} light={} rumble={} triggers={}",
              pad.index, pad.id, device.generation, static_cast<int>(device.model),
              device.touchpad, device.light, device.rumble, device.triggers);
    }
    std::lock_guard lock(g_legacySnapshotMutex);
    auto& snapshot = g_legacySnapshots[pad.index];
    snapshot = {};
    snapshot.connected = true;
    snapshot.generation = device.generation;
    snapshot.rumble = device.rumble;
    snapshot.wireless = SDL_GetGamepadConnectionState(pad.controller) == SDL_JOYSTICK_CONNECTION_WIRELESS;
}

static void NeutralizeSonyController(Controller& pad, bool disconnect)
{
    auto& output = pad.sonyOutput;
    if (pad.controller) {
        if (SDL_RumbleGamepad(pad.controller, 0, 0, 0))
            output.appliedLow = output.appliedHigh = 0;
        if (output.device.model == sony::Model::kDualSense && output.triggersSet) {
            const auto neutral = sony::EncodeTriggers(sony::Resistance(0, 0), sony::Resistance(0, 0));
            const unsigned attempts = disconnect ? 3 : 1;
            for (unsigned attempt = 0; attempt < attempts; ++attempt) {
                if (SDL_SendGamepadEffect(pad.controller, neutral.data(), static_cast<int>(neutral.size()))) {
                    output.packet = neutral;
                    break;
                }
            }
        }
        if (output.lightSet && SDL_SetGamepadLED(pad.controller, 0, 0, 0)) {
            output.color = {};
            output.lightSet = false;
        }
    }
    output.nextRumble = 0;
    if (disconnect && output.device.model != sony::Model::kNone)
        sony::GetService().Disconnect(pad.id);
    std::lock_guard lock(g_legacySnapshotMutex);
    auto& snapshot = g_legacySnapshots[pad.index];
    snapshot.nativeLow = snapshot.nativeHigh = 0;
    snapshot.nativeUntil = 0;
    if (disconnect) snapshot = {};
}

static void SnapshotLegacyController(Controller& pad, bool initial = false)
{
    // The legacy event watch updates ordinary axes/buttons. Poll once when a
    // device is discovered; keep copying input at every main-loop iteration.
    if (initial) {
        pad.PollAxis();
        pad.Poll();
    }
    rex::input::X_INPUT_GAMEPAD state{};
    state.buttons = pad.state.wButtons;
    state.left_trigger = pad.state.bLeftTrigger;
    state.right_trigger = pad.state.bRightTrigger;
    state.thumb_lx = pad.state.sThumbLX;
    state.thumb_ly = pad.state.sThumbLY;
    state.thumb_rx = pad.state.sThumbRX;
    state.thumb_ry = pad.state.sThumbRY;
    sony::GetService().UpdateAxes(static_cast<uint32_t>(pad.index), state.left_trigger, state.right_trigger);
    const bool back = SDL_GetGamepadButton(pad.controller, SDL_GAMEPAD_BUTTON_BACK);
    const bool touch = SDL_GetGamepadButton(pad.controller, SDL_GAMEPAD_BUTTON_TOUCHPAD);
    std::lock_guard lock(g_legacySnapshotMutex);
    auto& snapshot = g_legacySnapshots[pad.index];
    if (memcmp(&snapshot.state.gamepad, &state, sizeof(state)) != 0)
        snapshot.state.packet_number = static_cast<uint32_t>(snapshot.state.packet_number) + 1;
    snapshot.state.gamepad = state;
    snapshot.physicalBack = back;
    snapshot.touchClick = touch;
}

void hid::UpdateSonyFeedback()
{
    LegacyControllerLock controllerLock;
    if (!g_legacySonyRunning) return;
    const uint64_t now = SDL_GetTicks();
    static uint64_t nextOutput = 0;
    for (auto& pad : g_controllers) {
        if (pad.controller && SDL_GamepadConnected(pad.controller)) SnapshotLegacyController(pad);
    }
    if (now < nextOutput) return;
    nextOutput = now + 10;
    const auto options = sony::GetOptions();
    for (auto& pad : g_controllers) {
        if (!pad.controller || !SDL_GamepadConnected(pad.controller)) continue;
        auto& cached = pad.sonyOutput;
        const uint32_t user = static_cast<uint32_t>(pad.index);
        const bool sonyPad = cached.device.model != sony::Model::kNone;
        const auto effect = sonyPad ? sony::GetService().Compose(user, now, options) : sony::Output{};
        uint16_t nativeLow = 0, nativeHigh = 0;
        {
            std::lock_guard lock(g_legacySnapshotMutex);
            auto& snapshot = g_legacySnapshots[user];
            if (!g_legacyFocused || effect.suppress_native || now >= snapshot.nativeUntil)
                snapshot.nativeLow = snapshot.nativeHigh = 0;
            nativeLow = snapshot.nativeLow;
            nativeHigh = snapshot.nativeHigh;
        }
        const uint16_t low = effect.suppress_native ? 0 : std::max(nativeLow, effect.low_motor);
        const uint16_t high = effect.suppress_native ? 0 : std::max(nativeHigh, effect.high_motor);
        if (cached.device.rumble && cached.rumbleFailures < 3 && now >= cached.rumbleRetryAt &&
            (low != cached.appliedLow || high != cached.appliedHigh ||
             ((low || high) && now >= cached.nextRumble))) {
            // Finite packets silence the motors if the UI owner stops pumping.
            if (SDL_RumbleGamepad(pad.controller, low, high, 100)) {
                cached.appliedLow = low;
                cached.appliedHigh = high;
                cached.rumbleFailures = 0;
            } else {
                ++cached.rumbleFailures;
                cached.rumbleRetryAt = now + 100;
                if (cached.rumbleFailures == 3) SDL_RumbleGamepad(pad.controller, 0, 0, 0);
            }
            cached.nextRumble = now + 50;
        }
        if (!sonyPad || now < cached.retryAt) continue;
        const auto device = sony::GetService().ReadDevice(user);
        if (device.generation != cached.device.generation) continue;
        const auto color = effect.lighting ? effect.color : std::array<uint8_t, 3>{};
        if (device.light && (cached.lightFailures < 3 || !effect.lighting) &&
            (cached.lightSet || effect.lighting) && color != cached.color) {
            if (SDL_SetGamepadLED(pad.controller, color[0], color[1], color[2])) {
                cached.color = color;
                cached.lightSet = effect.lighting;
                cached.lightFailures = 0;
            } else {
                ++cached.lightFailures;
                cached.retryAt = now + 100;
                if (cached.lightFailures <= 3)
                    LOGFN("sony-controller: legacy LED output failed user={} attempt={}: {}",
                          user, cached.lightFailures, SDL_GetError());
            }
        }
        const auto neutral = sony::EncodeTriggers(sony::Resistance(0, 0), sony::Resistance(0, 0));
        const bool neutralPending = cached.triggersSet && cached.packet != neutral;
        if ((device.triggers && cached.triggerFailures < 3) || neutralPending) {
            const auto packet = device.triggers && cached.triggerFailures < 3
                ? sony::EncodeTriggers(effect.left, effect.right) : neutral;
            if (!cached.triggersSet || packet != cached.packet) {
                if (SDL_SendGamepadEffect(pad.controller, packet.data(), static_cast<int>(packet.size()))) {
                    cached.packet = packet;
                    cached.triggersSet = true;
                    cached.triggerFailures = 0;
                } else {
                    ++cached.triggerFailures;
                    cached.retryAt = now + 100;
                    if (cached.triggerFailures <= 3)
                        LOGFN("sony-controller: legacy trigger output failed user={} attempt={}: {}",
                              user, cached.triggerFailures, SDL_GetError());
                    if (cached.triggerFailures == 3) {
                        if (SDL_SendGamepadEffect(pad.controller, neutral.data(), static_cast<int>(neutral.size())))
                            cached.packet = neutral;
                        sony::GetService().DisableTriggers(user, device.generation);
                    }
                }
            }
        }
    }
}

void hid::ShutdownSonyFeedback()
{
    LegacyControllerLock controllerLock;
    if (!g_legacySonyRunning) return;
    g_legacySonyRunning = false;
    sony::GetService().SetFocused(false);
    {
        std::lock_guard lock(g_legacySnapshotMutex);
        g_legacyFocused = false;
    }
    for (auto& pad : g_controllers) {
        if (pad.controller) NeutralizeSonyController(pad, true);
    }
}

std::unique_ptr<rex::input::InputSystem> hid::CreateRexInputSystem(bool toolMode)
{
    auto input = std::make_unique<rex::input::InputSystem>(nullptr);
    if (!toolMode) input->AddDriver(std::make_unique<LegacySDLInputDriver>());
    input->AddDriver(std::make_unique<DefaultInputFallback>(toolMode));
    return input;
}
#endif

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
#if defined(GTA4_SONY_LEGACY_OWNER)
    LegacyControllerLock controllerLock;
    if (!g_legacySonyRunning) return true;
    if (event->type == SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN ||
        event->type == SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION ||
        event->type == SDL_EVENT_GAMEPAD_TOUCHPAD_UP) {
        if (event->gtouchpad.touchpad == 0) {
            const auto phase = event->type == SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN ? sony::ContactPhase::kDown :
                               event->type == SDL_EVENT_GAMEPAD_TOUCHPAD_UP ? sony::ContactPhase::kUp :
                                                                                 sony::ContactPhase::kMove;
            sony::GetService().ObserveTouch(event->gtouchpad.which, event->gtouchpad.finger,
                phase, event->gtouchpad.x, event->gtouchpad.y, SDL_GetTicks());
        }
    } else if ((event->type == SDL_EVENT_GAMEPAD_BUTTON_DOWN ||
                event->type == SDL_EVENT_GAMEPAD_BUTTON_UP) &&
               event->gbutton.button == SDL_GAMEPAD_BUTTON_TOUCHPAD) {
        sony::GetService().ObserveClick(event->gbutton.which,
            event->type == SDL_EVENT_GAMEPAD_BUTTON_DOWN, SDL_GetTicks());
    }
#endif
    switch (event->type)
    {
        case SDL_EVENT_GAMEPAD_ADDED:
        {
#if defined(GTA4_SONY_LEGACY_OWNER)
            if (FindController(event->gdevice.which)) break;
#endif
            const auto freeIndex = FindFreeController();

            if (freeIndex != -1)
            {
                auto controller = Controller(event->gdevice.which);

#if defined(GTA4_SONY_LEGACY_OWNER)
                if (!controller.controller) break;
                controller.index = static_cast<int>(freeIndex);
#endif

                g_controllers[freeIndex] = controller;

#if defined(GTA4_SONY_LEGACY_OWNER)
                auto& connected = g_controllers[freeIndex];
                RegisterSonyController(connected);
                SnapshotLegacyController(connected, true);
                if (connected.sonyOutput.device.model == sony::Model::kNone)
#endif
                    SetControllerTimeOfDayLED(controller, App::s_playerCharacter);
            }

            break;
        }

        case SDL_EVENT_GAMEPAD_REMOVED:
        {
            auto* controller = FindController(event->gdevice.which);

            if (controller)
                controller->Close();
#if defined(GTA4_SONY_LEGACY_OWNER)
            if (controller == g_activeController) g_activeController = nullptr;
#endif

            break;
        }

        case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
        case SDL_EVENT_GAMEPAD_BUTTON_UP:
        case SDL_EVENT_GAMEPAD_AXIS_MOTION:
        case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN:
        {
            const auto instance = event->type == SDL_EVENT_GAMEPAD_AXIS_MOTION ? event->gaxis.which :
                                  event->type == SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN ? event->gtouchpad.which :
                                                                                event->gbutton.which;
            auto* controller = FindController(instance);

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
#if defined(GTA4_SONY_LEGACY_OWNER)
            sony::GetService().SetFocused(false);
            {
                std::lock_guard lock(g_legacySnapshotMutex);
                g_legacyFocused = false;
            }
            for (auto& controller : g_controllers) {
                if (controller.controller) NeutralizeSonyController(controller, false);
            }
#else
            // Stop vibrating controllers on focus lost.
            for (auto& controller : g_controllers)
                controller.SetVibration({ 0, 0 });
#endif

            // Reset mouse camera
            MouseCamera::Reset();
            break;
        }

        case SDL_EVENT_WINDOW_FOCUS_GAINED:
        {
#if defined(GTA4_SONY_LEGACY_OWNER)
            sony::GetService().SetFocused(true);
            {
                std::lock_guard lock(g_legacySnapshotMutex);
                g_legacyFocused = true;
            }
#endif
            // Reset mouse state on focus gain
            s_lastMouseMovement = std::chrono::steady_clock::now();
            break;
        }

        case SDL_USER_PLAYER_CHAR:
        {
            for (auto& controller : g_controllers) {
#if defined(GTA4_SONY_LEGACY_OWNER)
                if (controller.sonyOutput.device.model != sony::Model::kNone) continue;
#endif
                SetControllerTimeOfDayLED(controller, static_cast<EPlayerCharacter>(event->user.code));
            }

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

#if defined(GTA4_SONY_LEGACY_OWNER)
    const bool focused = GameWindow::s_pWindow &&
        (SDL_GetWindowFlags(GameWindow::s_pWindow) & SDL_WINDOW_INPUT_FOCUS);
    {
        std::lock_guard lock(g_legacySnapshotMutex);
        g_legacyFocused = focused;
    }
    sony::GetService().SetFocused(focused);
    // SDL may have been initialized by the installer before our event watch.
    int count = 0;
    if (auto* ids = SDL_GetGamepads(&count)) {
        for (int i = 0; i < count; ++i) {
            SDL_Event added{};
            added.type = SDL_EVENT_GAMEPAD_ADDED;
            added.gdevice.which = ids[i];
            HID_OnSDLEvent(nullptr, &added);
        }
        SDL_free(ids);
    }
#endif

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
#if defined(GTA4_SONY_LEGACY_OWNER)
    LegacyControllerLock controllerLock;
#endif
    static uint32_t packet;

    if (!pState)
        return kInputBadArguments;

    memset(pState, 0, sizeof(*pState));

    pState->dwPacketNumber = packet++;

    if (!g_activeController)
        return kInputNotConnected;

    pState->Gamepad = g_activeController->state;

    return kInputSuccess;
}

uint32_t hid::SetState(uint32_t dwUserIndex, XAMINPUT_VIBRATION* pVibration)
{
#if defined(GTA4_SONY_LEGACY_OWNER)
    LegacyControllerLock controllerLock;
#endif
    if (!pVibration)
        return kInputBadArguments;

    if (!g_activeController)
        return kInputNotConnected;

    g_activeController->SetVibration(*pVibration);

    return kInputSuccess;
}

uint32_t hid::GetCapabilities(uint32_t dwUserIndex, XAMINPUT_CAPABILITIES* pCaps)
{
#if defined(GTA4_SONY_LEGACY_OWNER)
    LegacyControllerLock controllerLock;
#endif
    if (!pCaps)
        return kInputBadArguments;

    if (!g_activeController)
        return kInputNotConnected;

    memset(pCaps, 0, sizeof(*pCaps));

    pCaps->Type = XAMINPUT_DEVTYPE_GAMEPAD;
    pCaps->SubType = XAMINPUT_DEVSUBTYPE_GAMEPAD; // TODO: other types?
    pCaps->Flags = 0;
    pCaps->Gamepad = g_activeController->state;
    pCaps->Vibration = g_activeController->vibration;

    return kInputSuccess;
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
        case SDL_SCANCODE_RETURN:    return kVirtualReturn;
        case SDL_SCANCODE_ESCAPE:    return kVirtualEscape;
        case SDL_SCANCODE_BACKSPACE: return kVirtualBack;
        case SDL_SCANCODE_TAB:       return kVirtualTab;
        case SDL_SCANCODE_SPACE:     return kVirtualSpace;
        case SDL_SCANCODE_DELETE:    return kVirtualDelete;
        case SDL_SCANCODE_LEFT:      return kVirtualLeft;
        case SDL_SCANCODE_RIGHT:     return kVirtualRight;
        case SDL_SCANCODE_UP:        return kVirtualUp;
        case SDL_SCANCODE_DOWN:      return kVirtualDown;
        case SDL_SCANCODE_LSHIFT:
        case SDL_SCANCODE_RSHIFT:    return kVirtualShift;
        case SDL_SCANCODE_LCTRL:
        case SDL_SCANCODE_RCTRL:     return kVirtualControl;
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
        event.flags = key.repeat ? kKeystrokeRepeat : kKeystrokeDown;
    } else {
        event.flags = kKeystrokeUp;
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
#if defined(GTA4_SONY_LEGACY_OWNER)
    LegacyControllerLock controllerLock;
#endif
    if (!g_activeController)
        return false;
    
    return g_activeController->hasGyro || g_activeController->hasAccel;
}

void hid::SetMotionSensorEnabled(bool enabled)
{
#if defined(GTA4_SONY_LEGACY_OWNER)
    LegacyControllerLock controllerLock;
#endif
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
#if defined(GTA4_SONY_LEGACY_OWNER)
    LegacyControllerLock controllerLock;
#endif
    if (!g_activeController || !g_motionSensorEnabled) {
        g_motionState = {};
        return;
    }
    
    g_activeController->PollMotion(g_motionState);
}

void hid::ResetMotionOrientation()
{
#if defined(GTA4_SONY_LEGACY_OWNER)
    LegacyControllerLock controllerLock;
#endif
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
#if defined(GTA4_SONY_LEGACY_OWNER)
    LegacyControllerLock controllerLock;
#endif
    if (!g_activeController || !g_activeController->controller)
        return false;
    
    // DualShock 4 and DualSense have light bars
    auto type = SDL_GetGamepadType(g_activeController->controller);
    return type == SDL_GAMEPAD_TYPE_PS4 || type == SDL_GAMEPAD_TYPE_PS5;
}

void hid::SetLightBarColor(uint8_t r, uint8_t g, uint8_t b)
{
#if defined(GTA4_SONY_LEGACY_OWNER)
    LegacyControllerLock controllerLock;
#endif
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
#if defined(GTA4_SONY_LEGACY_OWNER)
    LegacyControllerLock controllerLock;
#endif
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
#if defined(GTA4_SONY_LEGACY_OWNER)
    LegacyControllerLock controllerLock;
#endif
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
#if defined(GTA4_SONY_LEGACY_OWNER)
    LegacyControllerLock controllerLock;
#endif
    if (!g_activeController || !g_activeController->controller)
        return false;
    
    // Only DualSense has adaptive triggers
    auto type = SDL_GetGamepadType(g_activeController->controller);
    return type == SDL_GAMEPAD_TYPE_PS5;
}
