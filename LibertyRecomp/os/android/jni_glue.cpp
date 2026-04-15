// JNI glue — called from LibertySDLActivity.java before SDL starts
// Sets paths that the native process layer reads.
#include <jni.h>
#include <string>
#include <cstring>
#include <malloc.h>

#include "achievement_bridge_android.h"

const char* g_androidAppInternalPath = nullptr;
const char* g_androidObbPath         = nullptr;

// Global JavaVM captured at library load via JNI_OnLoad. Any native code
// running off a non-Java thread must AttachCurrentThread() to obtain a JNIEnv.
JavaVM*  g_androidJavaVM     = nullptr;
// Global reference to the hosting Activity (Context). Set from Java via
// nativeSetActivity(). Used for Context.getSystemService / getResources calls.
jobject  g_androidActivity   = nullptr;
// Cached API level (Build.VERSION.SDK_INT). 0 until set from Java.
int      g_androidApiLevel   = 0;

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* /*reserved*/)
{
    g_androidJavaVM = vm;
    return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT void JNICALL
Java_com_libertyrecomp_LibertySDLActivity_nativeSetActivity(
    JNIEnv* env,
    jclass  /*clazz*/,
    jobject activity,
    jint    apiLevel)
{
    if (g_androidActivity)
    {
        env->DeleteGlobalRef(g_androidActivity);
        g_androidActivity = nullptr;
    }
    if (activity)
        g_androidActivity = env->NewGlobalRef(activity);
    g_androidApiLevel = static_cast<int>(apiLevel);

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
    auto copyJString = [&](jstring js) -> const char* {
        if (!js) return nullptr;
        const char* tmp = env->GetStringUTFChars(js, nullptr);
        size_t len = strlen(tmp);
        char* buf = static_cast<char*>(malloc(len + 1));
        memcpy(buf, tmp, len + 1);
        env->ReleaseStringUTFChars(js, tmp);
        return buf;
    };

    free(const_cast<char*>(g_androidAppInternalPath));
    free(const_cast<char*>(g_androidObbPath));

    g_androidAppInternalPath = copyJString(internalPath);
    g_androidObbPath         = copyJString(obbPath);
}
