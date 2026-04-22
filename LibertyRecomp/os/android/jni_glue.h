// Shared JNI glue state for the Android backend.
// The JavaVM* is captured in JNI_OnLoad; the Activity (Context) global ref
// is set from Java via LibertySDLActivity.nativeSetActivity().
#pragma once

#include <jni.h>

extern JavaVM*  g_androidJavaVM;
extern jobject  g_androidActivity;
extern int      g_androidApiLevel;

// Game-root directory supplied by LibertySDLActivity's folder-picker UI.
// Takes precedence over the CMake-baked LIBERTY_RECOMP_EMBEDDED_GAME_PATH when
// set; remains nullptr until the user commits a selection. Owning pointer —
// allocated with malloc(), replaced at most once per process via
// nativeSetGameRoot().
extern "C" const char* g_androidGameRoot;

// RAII helper: attaches the current thread to the VM if needed, and detaches
// on destruction (only if this object performed the attach).
class JniScopedAttach
{
public:
    JniScopedAttach()
    {
        if (!g_androidJavaVM)
            return;
        if (g_androidJavaVM->GetEnv(reinterpret_cast<void**>(&m_env), JNI_VERSION_1_6) == JNI_OK)
            return;
        JavaVMAttachArgs args{ JNI_VERSION_1_6, "LibertyRecomp", nullptr };
        if (g_androidJavaVM->AttachCurrentThread(&m_env, &args) == JNI_OK)
            m_ownsAttach = true;
        else
            m_env = nullptr;
    }

    ~JniScopedAttach()
    {
        if (m_ownsAttach && g_androidJavaVM)
            g_androidJavaVM->DetachCurrentThread();
    }

    JniScopedAttach(const JniScopedAttach&)            = delete;
    JniScopedAttach& operator=(const JniScopedAttach&) = delete;

    JNIEnv* env() const { return m_env; }

private:
    JNIEnv* m_env        = nullptr;
    bool    m_ownsAttach = false;
};
