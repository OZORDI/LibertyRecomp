// Android VibratorManager JNI bridge for dual-motor gamepad rumble.
//
// API levels:
//   * API 31+ (Android 12): InputDevice.getVibratorManager() exposes each
//     motor independently. getVibratorIds() returns an int[]; getVibrator(id)
//     returns a per-motor Vibrator. We map low_freq -> ids[0] (strong) and
//     high_freq -> ids[1] (weak). If only a single id is present we fall back
//     to combined amplitude on that motor.
//   * API 26..30: InputDevice.getVibrator() returns a single Vibrator. We
//     play VibrationEffect.createOneShot with the MAX(low, high) amplitude.
//   * API 21..25: Vibrator.vibrate(long) legacy pattern (no amplitude).
//
// Threading:
//   * JavaVM* is captured in JNI_OnLoad (primary) or via nativeSetContext
//     (fallback when linked alongside an existing JNI_OnLoad). Every call
//     attaches the current thread if needed and detaches on scope exit.
//
// Context:
//   * Java_com_libertyrecomp_LibertySDLActivity_nativeSetContext must be
//     invoked once from the activity's onCreate with `this` (the Activity).
//     The Activity is stored as a GlobalRef and used for getSystemService().
//
// Class/method IDs are lazily resolved on first successful call and cached.

#include "vibration_android.h"

#include <jni.h>
#include <android/log.h>

#include <cstdint>
#include <mutex>

#define LOG_TAG "LibertyRumble"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,  LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace {

// ---- Global JVM + Context ---------------------------------------------------

JavaVM*   g_vm       = nullptr;
jobject   g_context  = nullptr;  // GlobalRef to the Activity
int       g_sdkInt   = 0;

std::once_flag g_idsOnce;

// Cached class / method IDs.
struct JniIds {
    // android.os.Build$VERSION.SDK_INT (int static field)
    // Not strictly cached — read once during init.

    // android.content.Context
    jmethodID ctx_getSystemService = nullptr;  // (String)Object
    jstring   INPUT_SERVICE        = nullptr;  // GlobalRef

    // android.hardware.input.InputManager
    jclass    cls_InputManager     = nullptr;  // GlobalRef
    jmethodID im_getInputDevice    = nullptr;  // (int)InputDevice

    // android.view.InputDevice
    jclass    cls_InputDevice      = nullptr;  // GlobalRef
    jmethodID id_getVibratorManager = nullptr; // ()VibratorManager (API 31)
    jmethodID id_getVibrator        = nullptr; // ()Vibrator        (API 16)

    // android.os.VibratorManager (API 31+)
    jclass    cls_VibratorManager  = nullptr;  // GlobalRef
    jmethodID vm_getVibratorIds    = nullptr;  // ()[I
    jmethodID vm_getVibrator       = nullptr;  // (int)Vibrator
    jmethodID vm_cancel            = nullptr;  // ()V

    // android.os.Vibrator
    jclass    cls_Vibrator         = nullptr;  // GlobalRef
    jmethodID vib_vibrateEffect    = nullptr;  // (VibrationEffect)V
    jmethodID vib_vibrateLong      = nullptr;  // (long)V  legacy
    jmethodID vib_cancel           = nullptr;  // ()V

    // android.os.VibrationEffect
    jclass    cls_VibrationEffect  = nullptr;  // GlobalRef
    jmethodID ve_createOneShot     = nullptr;  // (long,int) static
};
JniIds g_ids;

// ---- Thread attach helper ---------------------------------------------------

struct ScopedEnv {
    JNIEnv* env    = nullptr;
    bool    owned  = false;  // true if we attached this thread

    ScopedEnv() {
        if (!g_vm) return;
        int getEnvStat = g_vm->GetEnv(reinterpret_cast<void**>(&env),
                                      JNI_VERSION_1_6);
        if (getEnvStat == JNI_EDETACHED) {
            if (g_vm->AttachCurrentThread(&env, nullptr) == JNI_OK) {
                owned = true;
            } else {
                env = nullptr;
            }
        } else if (getEnvStat != JNI_OK) {
            env = nullptr;
        }
    }
    ~ScopedEnv() {
        if (owned && g_vm) {
            g_vm->DetachCurrentThread();
        }
    }
    ScopedEnv(const ScopedEnv&) = delete;
    ScopedEnv& operator=(const ScopedEnv&) = delete;

    explicit operator bool() const { return env != nullptr; }
};

// Clear any pending JNI exception and log it.
bool CheckAndClearException(JNIEnv* env, const char* where) {
    if (env->ExceptionCheck()) {
        LOGW("JNI exception in %s", where);
        env->ExceptionDescribe();
        env->ExceptionClear();
        return true;
    }
    return false;
}

// ---- One-time JNI ID resolution --------------------------------------------

jclass FindClassGlobal(JNIEnv* env, const char* name) {
    jclass local = env->FindClass(name);
    if (CheckAndClearException(env, name) || !local) return nullptr;
    jclass global = static_cast<jclass>(env->NewGlobalRef(local));
    env->DeleteLocalRef(local);
    return global;
}

bool ResolveIds(JNIEnv* env) {
    // android.os.Build$VERSION.SDK_INT
    jclass versionCls = env->FindClass("android/os/Build$VERSION");
    if (!versionCls) { CheckAndClearException(env, "Build$VERSION"); return false; }
    jfieldID sdkField = env->GetStaticFieldID(versionCls, "SDK_INT", "I");
    if (!sdkField) { CheckAndClearException(env, "SDK_INT"); return false; }
    g_sdkInt = env->GetStaticIntField(versionCls, sdkField);
    env->DeleteLocalRef(versionCls);

    // Context.getSystemService
    jclass ctxCls = env->FindClass("android/content/Context");
    if (!ctxCls) { CheckAndClearException(env, "Context"); return false; }
    g_ids.ctx_getSystemService = env->GetMethodID(
        ctxCls, "getSystemService",
        "(Ljava/lang/String;)Ljava/lang/Object;");
    jfieldID inputServiceField = env->GetStaticFieldID(
        ctxCls, "INPUT_SERVICE", "Ljava/lang/String;");
    jstring inputServiceLocal = static_cast<jstring>(
        env->GetStaticObjectField(ctxCls, inputServiceField));
    g_ids.INPUT_SERVICE = static_cast<jstring>(
        env->NewGlobalRef(inputServiceLocal));
    env->DeleteLocalRef(inputServiceLocal);
    env->DeleteLocalRef(ctxCls);

    // InputManager
    g_ids.cls_InputManager = FindClassGlobal(env,
        "android/hardware/input/InputManager");
    if (!g_ids.cls_InputManager) return false;
    g_ids.im_getInputDevice = env->GetMethodID(
        g_ids.cls_InputManager, "getInputDevice",
        "(I)Landroid/view/InputDevice;");

    // InputDevice
    g_ids.cls_InputDevice = FindClassGlobal(env, "android/view/InputDevice");
    if (!g_ids.cls_InputDevice) return false;
    if (g_sdkInt >= 31) {
        g_ids.id_getVibratorManager = env->GetMethodID(
            g_ids.cls_InputDevice, "getVibratorManager",
            "()Landroid/os/VibratorManager;");
        CheckAndClearException(env, "InputDevice.getVibratorManager");
    }
    g_ids.id_getVibrator = env->GetMethodID(
        g_ids.cls_InputDevice, "getVibrator",
        "()Landroid/os/Vibrator;");
    CheckAndClearException(env, "InputDevice.getVibrator");

    // VibratorManager (API 31+)
    if (g_sdkInt >= 31) {
        g_ids.cls_VibratorManager = FindClassGlobal(env,
            "android/os/VibratorManager");
        if (g_ids.cls_VibratorManager) {
            g_ids.vm_getVibratorIds = env->GetMethodID(
                g_ids.cls_VibratorManager, "getVibratorIds", "()[I");
            g_ids.vm_getVibrator = env->GetMethodID(
                g_ids.cls_VibratorManager, "getVibrator",
                "(I)Landroid/os/Vibrator;");
            g_ids.vm_cancel = env->GetMethodID(
                g_ids.cls_VibratorManager, "cancel", "()V");
            CheckAndClearException(env, "VibratorManager methods");
        }
    }

    // Vibrator
    g_ids.cls_Vibrator = FindClassGlobal(env, "android/os/Vibrator");
    if (!g_ids.cls_Vibrator) return false;
    g_ids.vib_vibrateEffect = env->GetMethodID(
        g_ids.cls_Vibrator, "vibrate",
        "(Landroid/os/VibrationEffect;)V");
    CheckAndClearException(env, "Vibrator.vibrate(effect)");
    g_ids.vib_vibrateLong = env->GetMethodID(
        g_ids.cls_Vibrator, "vibrate", "(J)V");
    CheckAndClearException(env, "Vibrator.vibrate(long)");
    g_ids.vib_cancel = env->GetMethodID(
        g_ids.cls_Vibrator, "cancel", "()V");

    // VibrationEffect (API 26+)
    if (g_sdkInt >= 26) {
        g_ids.cls_VibrationEffect = FindClassGlobal(env,
            "android/os/VibrationEffect");
        if (g_ids.cls_VibrationEffect) {
            g_ids.ve_createOneShot = env->GetStaticMethodID(
                g_ids.cls_VibrationEffect, "createOneShot",
                "(JI)Landroid/os/VibrationEffect;");
            CheckAndClearException(env, "VibrationEffect.createOneShot");
        }
    }

    LOGI("Vibration JNI initialised: SDK_INT=%d, VibratorManager=%s",
         g_sdkInt, g_ids.cls_VibratorManager ? "yes" : "no");
    return true;
}

bool EnsureInitialised(JNIEnv* env) {
    bool ok = true;
    std::call_once(g_idsOnce, [&] { ok = ResolveIds(env); });
    return ok && g_ids.cls_InputDevice != nullptr;
}

// ---- Core helpers -----------------------------------------------------------

// Fetches the InputManager via Context.getSystemService(INPUT_SERVICE).
// Returns a local ref that the caller must release.
jobject GetInputManager(JNIEnv* env) {
    if (!g_context) return nullptr;
    jobject svc = env->CallObjectMethod(
        g_context, g_ids.ctx_getSystemService, g_ids.INPUT_SERVICE);
    if (CheckAndClearException(env, "Context.getSystemService")) return nullptr;
    return svc;
}

// Returns a local ref to an InputDevice, or nullptr.
jobject GetInputDevice(JNIEnv* env, int32_t deviceId) {
    jobject im = GetInputManager(env);
    if (!im) return nullptr;
    jobject dev = env->CallObjectMethod(
        im, g_ids.im_getInputDevice, static_cast<jint>(deviceId));
    CheckAndClearException(env, "InputManager.getInputDevice");
    env->DeleteLocalRef(im);
    return dev;
}

// Plays a oneshot effect on the given Vibrator local ref at `amp` (0..255).
// Uses the legacy (long) form if VibrationEffect is unavailable or amp<=0
// but duration>0. Silently no-ops on duration==0.
void PlayOnVibrator(JNIEnv* env, jobject vibrator,
                    uint32_t duration_ms, int amp /*0..255*/) {
    if (!vibrator || duration_ms == 0) return;

    if (g_sdkInt >= 26 && g_ids.ve_createOneShot && g_ids.vib_vibrateEffect) {
        // amplitude must be in [1,255] or DEFAULT_AMPLITUDE(-1). Clamp.
        if (amp < 1)   amp = 1;
        if (amp > 255) amp = 255;
        jobject effect = env->CallStaticObjectMethod(
            g_ids.cls_VibrationEffect, g_ids.ve_createOneShot,
            static_cast<jlong>(duration_ms), static_cast<jint>(amp));
        if (CheckAndClearException(env, "createOneShot") || !effect) return;
        env->CallVoidMethod(vibrator, g_ids.vib_vibrateEffect, effect);
        CheckAndClearException(env, "Vibrator.vibrate(effect)");
        env->DeleteLocalRef(effect);
    } else if (g_ids.vib_vibrateLong) {
        env->CallVoidMethod(vibrator, g_ids.vib_vibrateLong,
                            static_cast<jlong>(duration_ms));
        CheckAndClearException(env, "Vibrator.vibrate(long)");
    }
}

void CancelOnVibrator(JNIEnv* env, jobject vibrator) {
    if (!vibrator || !g_ids.vib_cancel) return;
    env->CallVoidMethod(vibrator, g_ids.vib_cancel);
    CheckAndClearException(env, "Vibrator.cancel");
}

} // namespace

// ---- Public C API -----------------------------------------------------------

extern "C" void android_vibration_set_rumble(int32_t device_id,
                                             uint16_t low_freq,
                                             uint16_t high_freq,
                                             uint32_t duration_ms)
{
    if (!g_vm || !g_context) return;

    ScopedEnv scope;
    if (!scope) return;
    JNIEnv* env = scope.env;

    if (!EnsureInitialised(env)) return;

    if (duration_ms == 0 || (low_freq == 0 && high_freq == 0)) {
        android_vibration_stop(device_id);
        return;
    }

    const int amp_low  = static_cast<int>(low_freq  >> 8);
    const int amp_high = static_cast<int>(high_freq >> 8);

    jobject dev = GetInputDevice(env, device_id);
    if (!dev) { LOGW("InputDevice %d not found", device_id); return; }

    bool played = false;

    // Preferred path: per-motor VibratorManager on API 31+.
    if (g_sdkInt >= 31 && g_ids.id_getVibratorManager &&
        g_ids.cls_VibratorManager) {
        jobject vman = env->CallObjectMethod(dev, g_ids.id_getVibratorManager);
        CheckAndClearException(env, "getVibratorManager");
        if (vman) {
            jintArray idsArr = static_cast<jintArray>(
                env->CallObjectMethod(vman, g_ids.vm_getVibratorIds));
            CheckAndClearException(env, "getVibratorIds");
            if (idsArr) {
                jsize n = env->GetArrayLength(idsArr);
                jint* ids = env->GetIntArrayElements(idsArr, nullptr);
                if (ids && n >= 1) {
                    // Motor 0 = strong/low, motor 1 = weak/high.
                    jobject motor0 = env->CallObjectMethod(
                        vman, g_ids.vm_getVibrator, ids[0]);
                    CheckAndClearException(env, "getVibrator[0]");
                    PlayOnVibrator(env, motor0, duration_ms, amp_low);
                    if (motor0) env->DeleteLocalRef(motor0);

                    if (n >= 2) {
                        jobject motor1 = env->CallObjectMethod(
                            vman, g_ids.vm_getVibrator, ids[1]);
                        CheckAndClearException(env, "getVibrator[1]");
                        PlayOnVibrator(env, motor1, duration_ms, amp_high);
                        if (motor1) env->DeleteLocalRef(motor1);
                    } else {
                        // Single motor: combine.
                        int combined = amp_low > amp_high ? amp_low : amp_high;
                        jobject only = env->CallObjectMethod(
                            vman, g_ids.vm_getVibrator, ids[0]);
                        // Already played above; re-play with combined if it
                        // differs materially. Cheap no-op otherwise.
                        if (only && combined != amp_low) {
                            PlayOnVibrator(env, only, duration_ms, combined);
                        }
                        if (only) env->DeleteLocalRef(only);
                    }
                    played = n >= 1;
                }
                if (ids) env->ReleaseIntArrayElements(idsArr, ids, JNI_ABORT);
                env->DeleteLocalRef(idsArr);
            }
            env->DeleteLocalRef(vman);
        }
    }

    // Fallback: legacy single Vibrator on API <31 or if manager missing.
    if (!played && g_ids.id_getVibrator) {
        jobject vib = env->CallObjectMethod(dev, g_ids.id_getVibrator);
        CheckAndClearException(env, "InputDevice.getVibrator");
        if (vib) {
            int combined = amp_low > amp_high ? amp_low : amp_high;
            PlayOnVibrator(env, vib, duration_ms, combined);
            env->DeleteLocalRef(vib);
        }
    }

    env->DeleteLocalRef(dev);
}

extern "C" void android_vibration_stop(int32_t device_id)
{
    if (!g_vm || !g_context) return;

    ScopedEnv scope;
    if (!scope) return;
    JNIEnv* env = scope.env;

    if (!EnsureInitialised(env)) return;

    jobject dev = GetInputDevice(env, device_id);
    if (!dev) return;

    if (g_sdkInt >= 31 && g_ids.id_getVibratorManager &&
        g_ids.cls_VibratorManager) {
        jobject vman = env->CallObjectMethod(dev, g_ids.id_getVibratorManager);
        CheckAndClearException(env, "getVibratorManager");
        if (vman) {
            if (g_ids.vm_cancel) {
                env->CallVoidMethod(vman, g_ids.vm_cancel);
                CheckAndClearException(env, "VibratorManager.cancel");
            }
            env->DeleteLocalRef(vman);
            env->DeleteLocalRef(dev);
            return;
        }
    }

    if (g_ids.id_getVibrator) {
        jobject vib = env->CallObjectMethod(dev, g_ids.id_getVibrator);
        CheckAndClearException(env, "InputDevice.getVibrator");
        if (vib) {
            CancelOnVibrator(env, vib);
            env->DeleteLocalRef(vib);
        }
    }

    env->DeleteLocalRef(dev);
}

// ---- JNI entry points -------------------------------------------------------

// Called by LibertySDLActivity.onCreate(this) to hand us the Activity/Context.
// Safe to call multiple times; replaces the previous GlobalRef.
extern "C" JNIEXPORT void JNICALL
Java_com_libertyrecomp_LibertySDLActivity_nativeSetContext(
    JNIEnv* env, jclass /*clazz*/, jobject context)
{
    if (!g_vm) {
        env->GetJavaVM(&g_vm);
    }
    if (g_context) {
        env->DeleteGlobalRef(g_context);
        g_context = nullptr;
    }
    if (context) {
        g_context = env->NewGlobalRef(context);
    }
    LOGI("nativeSetContext: ctx=%p vm=%p", g_context, g_vm);
}

// NOTE: JNI_OnLoad lives in jni_glue.cpp (single source of truth for the
// JavaVM* handle on Android). This TU captures the VM lazily via
// nativeSetContext() above, or via an env->GetJavaVM() call the first time
// any public API is invoked with a live JNIEnv.
