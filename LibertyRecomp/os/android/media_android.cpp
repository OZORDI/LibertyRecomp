#include <os/media.h>
#include "jni_glue.h"

// Returns true when another app is currently playing audio through the
// system's AudioManager (AudioManager.isMusicActive()). The game uses this
// to duck its own music/ambient output when e.g. Spotify is playing.
//
// Java equivalent:
//   AudioManager am = (AudioManager)getSystemService(Context.AUDIO_SERVICE);
//   return am.isMusicActive();
bool os::media::IsExternalMediaPlaying()
{
    if (!g_androidActivity)
        return false; // Not yet wired up (pre-Activity) — don't duck.

    JniScopedAttach attach;
    JNIEnv* env = attach.env();
    if (!env)
        return false;

    // Cache jclass / jmethodID on first call. All caches hold *global* refs
    // so they remain valid across JNIEnv attach/detach cycles.
    static jclass    s_contextClass         = nullptr;
    static jmethodID s_getSystemService     = nullptr;
    static jfieldID  s_audioServiceField    = nullptr;
    static jstring   s_audioServiceConst    = nullptr; // Context.AUDIO_SERVICE
    static jclass    s_audioManagerClass    = nullptr;
    static jmethodID s_isMusicActive        = nullptr;
    static bool      s_initOk               = false;
    static bool      s_initTried            = false;

    if (!s_initTried)
    {
        s_initTried = true;

        jclass contextClsLocal = env->FindClass("android/content/Context");
        if (!contextClsLocal) { env->ExceptionClear(); return false; }
        s_contextClass = (jclass)env->NewGlobalRef(contextClsLocal);
        env->DeleteLocalRef(contextClsLocal);

        s_getSystemService = env->GetMethodID(
            s_contextClass,
            "getSystemService",
            "(Ljava/lang/String;)Ljava/lang/Object;");
        if (!s_getSystemService) { env->ExceptionClear(); return false; }

        s_audioServiceField = env->GetStaticFieldID(
            s_contextClass, "AUDIO_SERVICE", "Ljava/lang/String;");
        if (!s_audioServiceField) { env->ExceptionClear(); return false; }

        jstring audioConstLocal = (jstring)env->GetStaticObjectField(
            s_contextClass, s_audioServiceField);
        if (!audioConstLocal) { env->ExceptionClear(); return false; }
        s_audioServiceConst = (jstring)env->NewGlobalRef(audioConstLocal);
        env->DeleteLocalRef(audioConstLocal);

        jclass amClsLocal = env->FindClass("android/media/AudioManager");
        if (!amClsLocal) { env->ExceptionClear(); return false; }
        s_audioManagerClass = (jclass)env->NewGlobalRef(amClsLocal);
        env->DeleteLocalRef(amClsLocal);

        s_isMusicActive = env->GetMethodID(
            s_audioManagerClass, "isMusicActive", "()Z");
        if (!s_isMusicActive) { env->ExceptionClear(); return false; }

        s_initOk = true;
    }

    if (!s_initOk)
        return false;

    jobject audioManager = env->CallObjectMethod(
        g_androidActivity, s_getSystemService, s_audioServiceConst);
    if (env->ExceptionCheck()) { env->ExceptionClear(); return false; }
    if (!audioManager) return false;

    jboolean result = env->CallBooleanMethod(audioManager, s_isMusicActive);
    if (env->ExceptionCheck()) { env->ExceptionClear(); result = JNI_FALSE; }

    env->DeleteLocalRef(audioManager);
    return result == JNI_TRUE;
}
