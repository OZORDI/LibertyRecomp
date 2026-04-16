/**
 * Android platform hooks for ReXGlue.
 *
 * The host Activity must call SetAndroidJavaContext() with a valid JNIEnv*
 * and a Context (typically the Activity) as early as possible at startup,
 * before any rex::filesystem::GetUserFolder()/GetCachesFolder() call. The
 * implementation caches the absolute paths of Context.getFilesDir() and
 * Context.getCacheDir() so later queries do not need a live JNIEnv.
 *
 * NOTE: Bundle assets (APK .assets/) are NOT reachable via std::filesystem.
 * They must go through an AAssetManager*, which the host is expected to
 * obtain from the Java AssetManager and hand to whichever subsystem needs
 * it (e.g. via AAssetManager_fromJava(env, javaAssetManager)). ReXGlue
 * does not own that pointer; no asset I/O happens through rex::filesystem
 * on Android.
 */
#pragma once

#if defined(REX_PLATFORM_ANDROID) || defined(__ANDROID__)

#include <jni.h>

namespace rex {
namespace platform {
namespace android {

// Called by the host Activity at startup. Caches files/cache dir paths.
// Safe to call multiple times; last call wins. Passing a null env or
// context clears the cache and reverts to the /data/data/<pkg>/files
// fallback.
void SetAndroidJavaContext(JNIEnv* env, jobject context);

}  // namespace android
}  // namespace platform
}  // namespace rex

#endif  // REX_PLATFORM_ANDROID
