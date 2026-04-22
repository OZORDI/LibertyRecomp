/**
 * @file        rex/main_android.h
 * @brief       Minimal Android platform helpers for the ReXGlue runtime.
 *
 * Provides the API-level query and JNI lifecycle hooks referenced by the
 * POSIX/platform threading and memory backends when compiled for Android.
 */

#ifndef REX_MAIN_ANDROID_H_
#define REX_MAIN_ANDROID_H_

#include <cstdint>

#if defined(__ANDROID__)
#include <android/api-level.h>
#endif

namespace rex {

/// Returns the runtime Android API level (e.g. 26 for Android 8.0).
/// On non-Android platforms this returns 0, so callers can keep a single
/// code-path: `if (rex::GetAndroidApiLevel() >= 26) { ... }`.
inline int32_t GetAndroidApiLevel() {
#if defined(__ANDROID__)
  return android_get_device_api_level();
#else
  return 0;
#endif
}

}  // namespace rex

#endif  // REX_MAIN_ANDROID_H_
