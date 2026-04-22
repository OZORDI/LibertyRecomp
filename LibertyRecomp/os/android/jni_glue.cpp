// JNI glue — called from LibertySDLActivity.java before SDL starts
// Sets paths that the native process layer reads.
#include <jni.h>
#include <string>
#include <cstring>
#include <malloc.h>

#include <rex/filesystem.h>
#include <rex/platform/android.h>

#include "achievement_bridge_android.h"

const char* g_androidAppInternalPath = nullptr;
const char* g_androidObbPath         = nullptr;
// Set from LibertySDLActivity's folder-picker flow. Declared in jni_glue.h
// and consumed by install/embedded_assets.cpp (::GetGameRoot) so the native
// runtime points at the user-selected game directory instead of the
// CMake-baked LIBERTY_RECOMP_EMBEDDED_GAME_PATH.
const char* g_androidGameRoot        = nullptr;

namespace {
// Shared UTF-8 copy helper: allocates a NUL-terminated buffer via malloc so
// free() is always valid on the replaced pointer. Returns nullptr for null
// input or empty strings (treated as "unset").
const char* CopyJString(JNIEnv* env, jstring js)
{
    if (!js) return nullptr;
    const char* tmp = env->GetStringUTFChars(js, nullptr);
    if (!tmp) return nullptr;
    size_t len = strlen(tmp);
    char* buf = nullptr;
    if (len > 0) {
        buf = static_cast<char*>(malloc(len + 1));
        if (buf) memcpy(buf, tmp, len + 1);
    }
    env->ReleaseStringUTFChars(js, tmp);
    return buf;
}
} // namespace

// Global JavaVM captured at library load via JNI_OnLoad. Any native code
// running off a non-Java thread must AttachCurrentThread() to obtain a JNIEnv.
JavaVM*  g_androidJavaVM     = nullptr;
// Global reference to the hosting Activity (Context). Set from Java via
// nativeSetActivity(). Used for Context.getSystemService / getResources calls.
jobject  g_androidActivity   = nullptr;
// Cached API level (Build.VERSION.SDK_INT). 0 until set from Java.
int      g_androidApiLevel   = 0;

// SDL3 already provides JNI_OnLoad in SDL_android.c, so we can't define our
// own.  Instead we lazily capture the JavaVM the first time it's needed,
// using SDL_GetAndroidJNIEnv() (available once SDL_Init / NativeActivity has run).
#include <SDL3/SDL.h>

static JavaVM* EnsureJavaVM()
{
    if (!g_androidJavaVM) {
        JNIEnv* env = (JNIEnv*)SDL_GetAndroidJNIEnv();
        if (env) env->GetJavaVM(&g_androidJavaVM);
    }
    return g_androidJavaVM;
}

// Published to ReXGlue so its Android-specific subsystems (filesystem
// content-URI resolver, keyboard dialog, etc.) can obtain a JNIEnv
// on arbitrary worker threads via vm->AttachCurrentThread().
extern "C" JavaVM* rex_android_get_jvm()
{
    return EnsureJavaVM();
}

extern "C" JNIEXPORT void JNICALL
Java_com_libertyrecomp_LibertySDLActivity_nativeSetActivity(
    JNIEnv* env,
    jclass  /*clazz*/,
    jobject activity,
    jint    apiLevel)
{
    // Capture the JavaVM from the JNIEnv if not already set.
    if (!g_androidJavaVM) env->GetJavaVM(&g_androidJavaVM);

    if (g_androidActivity)
    {
        env->DeleteGlobalRef(g_androidActivity);
        g_androidActivity = nullptr;
    }
    if (activity)
        g_androidActivity = env->NewGlobalRef(activity);
    g_androidApiLevel = static_cast<int>(apiLevel);

    // Publish the Java Context to ReXGlue so filesystem queries
    // (GetUserFolder / GetCachesFolder) resolve to the real app-private
    // paths instead of the /data/data/<progname>/files fallback.
    rex::platform::android::SetAndroidJavaContext(env, g_androidActivity);

    // Initialize the ReXGlue Android filesystem backend (content://
    // resolver refs + ParcelFileDescriptor method IDs). Must run after
    // SetAndroidJavaContext so the cached context is available.
    if (g_androidActivity) {
        rex::filesystem::AndroidInitialize();
    } else {
        rex::filesystem::AndroidShutdown();
    }

    // Bring up the Play Games achievement bridge now that we have an Activity.
    if (g_androidActivity)
        os::achievements::android::Initialize(env, g_androidActivity);
}

extern "C" JNIEXPORT void JNICALL
Java_com_libertyrecomp_LibertySDLActivity_nativeSetPaths(
    JNIEnv* env,
    jclass  /*clazz*/,
    jstring internalPath,
    jstring obbPath)
{
    free(const_cast<char*>(g_androidAppInternalPath));
    free(const_cast<char*>(g_androidObbPath));

    g_androidAppInternalPath = CopyJString(env, internalPath);
    g_androidObbPath         = CopyJString(env, obbPath);
}

// Called by LibertySDLActivity.loadLibraries() once the user has picked a
// game directory. Overrides the CMake-baked LIBERTY_RECOMP_EMBEDDED_GAME_PATH
// inside EmbeddedAssets::GetGameRoot() so the native VFS/installer path is
// rooted at the user-selected folder.
extern "C" JNIEXPORT void JNICALL
Java_com_libertyrecomp_LibertySDLActivity_nativeSetGameRoot(
    JNIEnv* env,
    jclass  /*clazz*/,
    jstring gameRoot)
{
    free(const_cast<char*>(g_androidGameRoot));
    g_androidGameRoot = CopyJString(env, gameRoot);
}
