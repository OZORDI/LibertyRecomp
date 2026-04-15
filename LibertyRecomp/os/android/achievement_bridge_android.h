// achievement_bridge_android.h -- Xbox 360 achievement -> Google Play Games bridge
//
// Mirrors the public call-site convention of the PS4/Switch/GameCenter bridges.
// All JNI work runs on a detached worker thread because Play Games returns
// Task<Void> and blocking the caller (potentially the XAM / UI thread) is
// undesirable. Fire-and-forget semantics match the PS4 bridge.
//
// Xbox achievement IDs are 1-indexed (1..65, base + DLC).
// The Play Games string ID is looked up from a static mapping table
// (mirrors the PS4 id -> trop string pattern) so the game can continue using
// its native integer IDs without knowing the Play Games console string.
//
// Wire-up:
//   - LibertyAndroidAchievementBridge::Initialize() is called from
//     Java_com_libertyrecomp_LibertySDLActivity_nativeSetContext() in
//     jni_glue.cpp once the Activity global ref is available.
//   - LibertyAndroidOnXboxAchievementUnlocked() is called from the
//     achievement manager / XAM message hook on every AWARD_ACHIEVEMENT write.
//   - LibertyAndroidAchievementBridge::Shutdown() is called at app teardown.

#pragma once

#if defined(LIBERTY_RECOMP_ANDROID) || defined(__ANDROID__)

#include <cstdint>
#include <jni.h>

namespace os::achievements::android
{
    // One-shot init. Captures the Activity global ref, resolves JNI class/method
    // IDs, and caches the AchievementsClient. Safe to call multiple times;
    // subsequent calls replace the cached references.
    bool Initialize(JNIEnv* env, jobject activity);

    // Tear down cached JNI globals. Safe to call even if Initialize() failed.
    void Shutdown();

    // Unlock an achievement by its 1-indexed Xbox ID (1..65).
    // Looks the ID up in the static string-ID mapping table.
    // Dispatched to a background thread; returns immediately.
    void AwardAchievement(uint32_t xbox_id);

    // Report progress on an incremental achievement. For non-incremental
    // achievements, calls unlock() once `current >= max`. Otherwise calls
    // AchievementsClient.increment(id, current).
    void UpdateProgress(uint32_t xbox_id, int32_t current, int32_t max);

    // Query the unlock status of an achievement. Queues a load on the worker
    // thread and writes the result via `callback` (optional). Returns false
    // immediately if not initialised or the ID is out of range.
    //
    // callback may be nullptr — in that case the query is issued for its
    // cache-refresh side-effect only. Invoked on the worker thread.
    bool QueryStatus(uint32_t xbox_id, void (*callback)(uint32_t xbox_id, bool unlocked, int32_t current_steps));
}

// Convenience wrapper matching the PS4/Switch bridges' call-site convention.
// Delegates to os::achievements::android::AwardAchievement().
void LibertyAndroidOnXboxAchievementUnlocked(uint32_t xbox_id);

#endif // LIBERTY_RECOMP_ANDROID || __ANDROID__
