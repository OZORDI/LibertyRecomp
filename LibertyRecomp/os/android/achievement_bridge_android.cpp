// achievement_bridge_android.cpp -- Play Games Services achievement bridge.
//
// Implements os::achievements::android:: via JNI against the v2 Play Games SDK:
//
//   com.google.android.gms.games.PlayGames
//     -> static getAchievementsClient(Activity): AchievementsClient
//   com.google.android.gms.games.AchievementsClient
//     -> unlock(String id)
//     -> increment(String id, int numSteps)
//     -> load(boolean forceReload): Task<AnnotatedData<AchievementBuffer>>
//
// All JNI calls happen on a dedicated worker thread. Play Games itself returns
// Task<Void>, so from native's perspective fire-and-forget is fine; the worker
// thread exists purely so we never block the XAM / audio / render callers.
//
// The id -> string mapping follows the PS4 TROP convention: 1-indexed Xbox ID
// maps to a zero-padded "ach_NNN" identifier. Titles can override by editing
// kAchievementIds[] to match their App Store Connect / Play Console IDs.

#if defined(LIBERTY_RECOMP_ANDROID) || defined(__ANDROID__)

#include "achievement_bridge_android.h"
#include "jni_glue.h"

#include <android/log.h>

#include <atomic>
#include <condition_variable>
#include <cstdio>
#include <cstring>
#include <functional>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

#define LOG_TAG "LibertyAchv"
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN,  LOG_TAG, __VA_ARGS__)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace {

// ---- id -> Play Games string mapping ----------------------------------------
// GTA IV has 65 scoreable achievements (1..65). Mirror the PS4 bridge: build
// "ach_NNN" on demand. A title that wants to override specific IDs should
// populate kAchievementIdOverrides[xbox_id] with the Play Console string.
static constexpr int kNonPlatinumCount = 65;
static const char* const kAchievementIdOverrides[kNonPlatinumCount + 1] = {};

static std::string GCIdentifierForXboxId(uint32_t xbox_id) {
    if (xbox_id >= 1 && xbox_id <= kNonPlatinumCount) {
        const char* override_id = kAchievementIdOverrides[xbox_id];
        if (override_id && override_id[0]) return std::string(override_id);
    }
    char buf[16];
    std::snprintf(buf, sizeof(buf), "ach_%03u", xbox_id);
    return std::string(buf);
}

// ---- Cached JNI handles -----------------------------------------------------

struct JniIds {
    jclass    cls_PlayGames            = nullptr; // global
    jmethodID mid_getAchievementsClient = nullptr; // static (Activity)AchievementsClient

    jclass    cls_AchievementsClient   = nullptr; // global
    jmethodID mid_unlock               = nullptr; // (String)V
    jmethodID mid_increment            = nullptr; // (String,int)V
    jmethodID mid_load                 = nullptr; // (boolean)Task
};

JavaVM*   g_vm         = nullptr;   // copy of g_androidJavaVM; captured at Initialize
jobject   g_activity   = nullptr;   // global ref to host Activity
jobject   g_client     = nullptr;   // global ref to AchievementsClient
JniIds    g_ids{};
std::atomic<bool> g_initialised{false};

// ---- Worker thread ----------------------------------------------------------
// All JNI work is queued onto this thread. Play Games is asynchronous already,
// but we still run off-thread to avoid any per-call JNI overhead on the caller.
std::mutex              g_queue_mutex;
std::condition_variable g_queue_cv;
std::queue<std::function<void(JNIEnv*)>> g_queue;
std::thread             g_worker;
std::atomic<bool>       g_worker_stop{false};

void EnqueueOnWorker(std::function<void(JNIEnv*)> fn) {
    std::lock_guard<std::mutex> lock(g_queue_mutex);
    g_queue.push(std::move(fn));
    g_queue_cv.notify_one();
}

bool CheckAndClearException(JNIEnv* env, const char* where) {
    if (env && env->ExceptionCheck()) {
        ALOGW("JNI exception in %s", where);
        env->ExceptionDescribe();
        env->ExceptionClear();
        return true;
    }
    return false;
}

jclass FindClassGlobal(JNIEnv* env, const char* name) {
    jclass local = env->FindClass(name);
    if (CheckAndClearException(env, name) || !local) return nullptr;
    jclass global = static_cast<jclass>(env->NewGlobalRef(local));
    env->DeleteLocalRef(local);
    return global;
}

// Resolve classes/methods and cache the AchievementsClient. Must run on a
// JNI-attached thread.
bool ResolveIdsAndClient(JNIEnv* env) {
    if (!g_activity) return false;

    g_ids.cls_PlayGames = FindClassGlobal(env,
        "com/google/android/gms/games/PlayGames");
    if (!g_ids.cls_PlayGames) { ALOGW("PlayGames class not found"); return false; }

    g_ids.mid_getAchievementsClient = env->GetStaticMethodID(
        g_ids.cls_PlayGames, "getAchievementsClient",
        "(Landroid/app/Activity;)Lcom/google/android/gms/games/AchievementsClient;");
    if (CheckAndClearException(env, "getAchievementsClient mid") ||
        !g_ids.mid_getAchievementsClient) return false;

    g_ids.cls_AchievementsClient = FindClassGlobal(env,
        "com/google/android/gms/games/AchievementsClient");
    if (!g_ids.cls_AchievementsClient) { ALOGW("AchievementsClient class not found"); return false; }

    g_ids.mid_unlock = env->GetMethodID(
        g_ids.cls_AchievementsClient, "unlock", "(Ljava/lang/String;)V");
    CheckAndClearException(env, "unlock mid");

    g_ids.mid_increment = env->GetMethodID(
        g_ids.cls_AchievementsClient, "increment", "(Ljava/lang/String;I)V");
    CheckAndClearException(env, "increment mid");

    g_ids.mid_load = env->GetMethodID(
        g_ids.cls_AchievementsClient, "load",
        "(Z)Lcom/google/android/gms/tasks/Task;");
    CheckAndClearException(env, "load mid");

    // Cache the client for the lifetime of the app. getAchievementsClient is
    // cheap but caching avoids a JNI round-trip per unlock.
    jobject local_client = env->CallStaticObjectMethod(
        g_ids.cls_PlayGames, g_ids.mid_getAchievementsClient, g_activity);
    if (CheckAndClearException(env, "getAchievementsClient call") || !local_client) {
        ALOGW("getAchievementsClient returned null");
        return false;
    }
    g_client = env->NewGlobalRef(local_client);
    env->DeleteLocalRef(local_client);
    return g_client != nullptr;
}

void WorkerMain() {
    // Attach for the lifetime of the worker.
    JNIEnv* env = nullptr;
    if (!g_vm) return;
    JavaVMAttachArgs args{ JNI_VERSION_1_6, "LibertyAchvWorker", nullptr };
    if (g_vm->AttachCurrentThread(&env, &args) != JNI_OK) {
        ALOGE("Worker: AttachCurrentThread failed");
        return;
    }

    if (!ResolveIdsAndClient(env)) {
        ALOGW("Worker: JNI resolution failed; queued tasks will be dropped");
    } else {
        g_initialised = true;
        ALOGI("Play Games AchievementsClient initialised");
    }

    while (!g_worker_stop.load(std::memory_order_acquire)) {
        std::function<void(JNIEnv*)> fn;
        {
            std::unique_lock<std::mutex> lock(g_queue_mutex);
            g_queue_cv.wait(lock, [] {
                return g_worker_stop.load(std::memory_order_acquire) ||
                       !g_queue.empty();
            });
            if (g_worker_stop.load(std::memory_order_acquire) && g_queue.empty())
                break;
            fn = std::move(g_queue.front());
            g_queue.pop();
        }
        if (fn) fn(env);
    }

    // Release cached JNI globals before detach.
    if (g_client)                    env->DeleteGlobalRef(g_client);                    g_client = nullptr;
    if (g_ids.cls_PlayGames)         env->DeleteGlobalRef(g_ids.cls_PlayGames);
    if (g_ids.cls_AchievementsClient) env->DeleteGlobalRef(g_ids.cls_AchievementsClient);
    g_ids = JniIds{};

    g_vm->DetachCurrentThread();
}

} // namespace

// ---- Public API -------------------------------------------------------------

namespace os::achievements::android {

bool Initialize(JNIEnv* env, jobject activity) {
    if (!env || !activity) return false;
    // Replace any prior registration (hot-reload scenarios).
    Shutdown();

    if (env->GetJavaVM(&g_vm) != JNI_OK || !g_vm) {
        ALOGE("Initialize: GetJavaVM failed");
        return false;
    }
    g_activity = env->NewGlobalRef(activity);
    if (!g_activity) return false;

    g_worker_stop = false;
    g_worker = std::thread(WorkerMain);
    return true;
}

void Shutdown() {
    if (g_worker.joinable()) {
        g_worker_stop = true;
        g_queue_cv.notify_all();
        g_worker.join();
    }
    // Clear the queue for a clean Initialize() on the next call.
    {
        std::lock_guard<std::mutex> lock(g_queue_mutex);
        std::queue<std::function<void(JNIEnv*)>> empty;
        std::swap(g_queue, empty);
    }

    if (g_activity && g_vm) {
        JNIEnv* env = nullptr;
        if (g_vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) == JNI_OK && env) {
            env->DeleteGlobalRef(g_activity);
        }
    }
    g_activity = nullptr;
    g_initialised = false;
}

void AwardAchievement(uint32_t xbox_id) {
    if (xbox_id < 1 || xbox_id > static_cast<uint32_t>(kNonPlatinumCount)) return;
    if (!g_vm) return;

    std::string ident = GCIdentifierForXboxId(xbox_id);
    EnqueueOnWorker([ident = std::move(ident)](JNIEnv* env) {
        if (!g_initialised.load() || !g_client || !g_ids.mid_unlock) return;
        jstring jid = env->NewStringUTF(ident.c_str());
        if (!jid) { CheckAndClearException(env, "NewStringUTF"); return; }
        env->CallVoidMethod(g_client, g_ids.mid_unlock, jid);
        CheckAndClearException(env, "AchievementsClient.unlock");
        env->DeleteLocalRef(jid);
    });
}

void UpdateProgress(uint32_t xbox_id, int32_t current, int32_t max) {
    if (xbox_id < 1 || xbox_id > static_cast<uint32_t>(kNonPlatinumCount)) return;
    if (!g_vm) return;

    std::string ident = GCIdentifierForXboxId(xbox_id);
    // If fully complete, dispatch a plain unlock() for definiteness.
    if (max > 0 && current >= max) {
        AwardAchievement(xbox_id);
        return;
    }
    const int32_t steps = current > 0 ? current : 0;
    if (steps == 0) return;

    EnqueueOnWorker([ident = std::move(ident), steps](JNIEnv* env) {
        if (!g_initialised.load() || !g_client || !g_ids.mid_increment) return;
        jstring jid = env->NewStringUTF(ident.c_str());
        if (!jid) { CheckAndClearException(env, "NewStringUTF"); return; }
        env->CallVoidMethod(g_client, g_ids.mid_increment, jid, static_cast<jint>(steps));
        CheckAndClearException(env, "AchievementsClient.increment");
        env->DeleteLocalRef(jid);
    });
}

bool QueryStatus(uint32_t xbox_id,
                 void (*callback)(uint32_t, bool, int32_t)) {
    if (xbox_id < 1 || xbox_id > static_cast<uint32_t>(kNonPlatinumCount)) return false;
    if (!g_vm) return false;

    // Kick a load(forceReload=true) so Play Games refreshes its local cache.
    // The actual per-achievement status is read off the returned AnnotatedData
    // asynchronously via Task listeners registered from the Java side (out of
    // scope for this bridge); here we just issue the refresh and report the
    // cached state we don't have natively, falling back to (false, 0).
    EnqueueOnWorker([xbox_id, callback](JNIEnv* env) {
        if (!g_initialised.load() || !g_client || !g_ids.mid_load) {
            if (callback) callback(xbox_id, false, 0);
            return;
        }
        jobject task = env->CallObjectMethod(g_client, g_ids.mid_load, JNI_TRUE);
        CheckAndClearException(env, "AchievementsClient.load");
        if (task) env->DeleteLocalRef(task);
        if (callback) callback(xbox_id, false, 0);
    });
    return true;
}

} // namespace os::achievements::android

void LibertyAndroidOnXboxAchievementUnlocked(uint32_t xbox_id) {
    os::achievements::android::AwardAchievement(xbox_id);
}

#endif // LIBERTY_RECOMP_ANDROID || __ANDROID__
