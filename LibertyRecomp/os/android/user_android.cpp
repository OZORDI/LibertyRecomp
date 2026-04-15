#include <os/user.h>
#include "jni_glue.h"

// Returns true when the system is currently in dark-mode (night theme).
// Java equivalent:
//   Configuration c = getResources().getConfiguration();
//   return (c.uiMode & Configuration.UI_MODE_NIGHT_MASK)
//            == Configuration.UI_MODE_NIGHT_YES;
//
// The uiMode field exists on every API level, but the system only reliably
// toggles UI_MODE_NIGHT_YES in response to system dark-mode on API 29+.
// Older versions always report UI_MODE_NIGHT_UNDEFINED, so we return false.
bool os::user::IsDarkTheme()
{
    if (g_androidApiLevel > 0 && g_androidApiLevel < 29)
        return false;
    if (!g_androidActivity)
        return false;

    JniScopedAttach attach;
    JNIEnv* env = attach.env();
    if (!env)
        return false;

    static constexpr jint UI_MODE_NIGHT_MASK = 0x30;
    static constexpr jint UI_MODE_NIGHT_YES  = 0x20;

    static jmethodID s_getResources        = nullptr;
    static jmethodID s_getConfiguration    = nullptr;
    static jfieldID  s_uiModeField         = nullptr;
    static bool      s_initOk              = false;
    static bool      s_initTried           = false;

    if (!s_initTried)
    {
        s_initTried = true;

        jclass contextCls = env->FindClass("android/content/Context");
        if (!contextCls) { env->ExceptionClear(); return false; }
        s_getResources = env->GetMethodID(
            contextCls, "getResources", "()Landroid/content/res/Resources;");
        env->DeleteLocalRef(contextCls);
        if (!s_getResources) { env->ExceptionClear(); return false; }

        jclass resourcesCls = env->FindClass("android/content/res/Resources");
        if (!resourcesCls) { env->ExceptionClear(); return false; }
        s_getConfiguration = env->GetMethodID(
            resourcesCls,
            "getConfiguration",
            "()Landroid/content/res/Configuration;");
        env->DeleteLocalRef(resourcesCls);
        if (!s_getConfiguration) { env->ExceptionClear(); return false; }

        jclass configCls = env->FindClass("android/content/res/Configuration");
        if (!configCls) { env->ExceptionClear(); return false; }
        s_uiModeField = env->GetFieldID(configCls, "uiMode", "I");
        env->DeleteLocalRef(configCls);
        if (!s_uiModeField) { env->ExceptionClear(); return false; }

        s_initOk = true;
    }

    if (!s_initOk)
        return false;

    jobject resources = env->CallObjectMethod(g_androidActivity, s_getResources);
    if (env->ExceptionCheck() || !resources)
    {
        env->ExceptionClear();
        if (resources) env->DeleteLocalRef(resources);
        return false;
    }

    jobject configuration = env->CallObjectMethod(resources, s_getConfiguration);
    env->DeleteLocalRef(resources);
    if (env->ExceptionCheck() || !configuration)
    {
        env->ExceptionClear();
        if (configuration) env->DeleteLocalRef(configuration);
        return false;
    }

    jint uiMode = env->GetIntField(configuration, s_uiModeField);
    env->DeleteLocalRef(configuration);

    return (uiMode & UI_MODE_NIGHT_MASK) == UI_MODE_NIGHT_YES;
}
