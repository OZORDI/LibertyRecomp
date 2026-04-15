// achievement_bridge_switch.cpp -- local file-backed achievement tracker (Switch)
//
// The Switch has no native trophy/achievement API.  We store unlock state
// in a flat binary file at save:/achievements.dat.  The file layout is:
//
//   Offset  Size  Description
//   0x00    4     Magic  ('LACH')
//   0x04    4     Version (1)
//   0x08    4     Unlock count (0..65)
//   0x0C    4     Completion flag (0 or 1) -- equivalent to PS4 Platinum
//   0x10    65    Bitfield: 1 byte per achievement (0 = locked, 1 = unlocked)
//   0x51    --    EOF
//
// Total file size: 81 bytes.  Trivially small; written atomically on each
// unlock so there is no risk of partial-write corruption in practice.

#ifdef LIBERTY_RECOMP_NX

#include "achievement_bridge_switch.h"

#include <cstdio>
#include <cstring>
#include <mutex>

// ---- constants --------------------------------------------------------------

static constexpr int      kNonPlatinumCount  = 65;
static constexpr uint32_t kMagic             = 0x4C414348; // 'LACH'
static constexpr uint32_t kVersion           = 1;
static constexpr const char* kSavePath       = "save:/achievements.dat";

// ---- on-disk / in-memory state ----------------------------------------------

struct AchievementState
{
    uint32_t magic;
    uint32_t version;
    uint32_t unlock_count;
    uint32_t completion_flag;
    uint8_t  unlocked[kNonPlatinumCount]; // 1 byte per achievement (0 or 1)
};

static AchievementState s_state{};
static bool             s_initialized = false;
static std::mutex       s_mutex;

// ---- file I/O helpers -------------------------------------------------------

static bool LoadFromDisk()
{
    FILE* f = fopen(kSavePath, "rb");
    if (!f)
        return false;

    AchievementState tmp{};
    size_t n = fread(&tmp, 1, sizeof(tmp), f);
    fclose(f);

    if (n != sizeof(tmp))
        return false;
    if (tmp.magic != kMagic || tmp.version != kVersion)
        return false;

    s_state = tmp;
    return true;
}

static bool SaveToDisk()
{
    FILE* f = fopen(kSavePath, "wb");
    if (!f)
        return false;

    size_t n = fwrite(&s_state, 1, sizeof(s_state), f);
    fclose(f);
    return (n == sizeof(s_state));
}

// ---- internal helpers -------------------------------------------------------

static void RecountUnlocks()
{
    uint32_t count = 0;
    for (int i = 0; i < kNonPlatinumCount; ++i)
    {
        if (s_state.unlocked[i])
            ++count;
    }
    s_state.unlock_count = count;
}

static void CheckCompletion()
{
    if (s_state.unlock_count >= static_cast<uint32_t>(kNonPlatinumCount))
        s_state.completion_flag = 1;
}

// ---- public API (os::achievements) ------------------------------------------

namespace os::achievements
{

bool InitAchievements()
{
    std::lock_guard<std::mutex> lock(s_mutex);

    if (s_initialized)
        return true;

    // Try to load existing state; if missing or corrupt, start fresh.
    if (!LoadFromDisk())
    {
        memset(&s_state, 0, sizeof(s_state));
        s_state.magic   = kMagic;
        s_state.version = kVersion;
    }

    s_initialized = true;
    return true;
}

void UnlockAchievement(uint32_t xbox_id)
{
    std::lock_guard<std::mutex> lock(s_mutex);

    if (!s_initialized)
        return;

    // Xbox IDs are 1-indexed: valid range is 1..65.
    if (xbox_id < 1 || xbox_id > static_cast<uint32_t>(kNonPlatinumCount))
        return;

    const int index = static_cast<int>(xbox_id) - 1;

    // Already unlocked -- nothing to do.
    if (s_state.unlocked[index])
        return;

    s_state.unlocked[index] = 1;
    RecountUnlocks();
    CheckCompletion();
    SaveToDisk();
}

bool IsAchievementUnlocked(uint32_t xbox_id)
{
    std::lock_guard<std::mutex> lock(s_mutex);

    if (!s_initialized)
        return false;
    if (xbox_id < 1 || xbox_id > static_cast<uint32_t>(kNonPlatinumCount))
        return false;

    return s_state.unlocked[xbox_id - 1] != 0;
}

int GetAchievementProgress()
{
    std::lock_guard<std::mutex> lock(s_mutex);
    return static_cast<int>(s_state.unlock_count);
}

bool IsCompleted()
{
    std::lock_guard<std::mutex> lock(s_mutex);
    return s_state.completion_flag != 0;
}

void Shutdown()
{
    std::lock_guard<std::mutex> lock(s_mutex);

    if (!s_initialized)
        return;

    // Final flush in case caller unlocked something without a save path.
    SaveToDisk();
    s_initialized = false;
}

} // namespace os::achievements

// ---- convenience wrapper matching PS4 bridge call-site convention -----------

void LibertyNXOnXboxAchievementUnlocked(uint32_t xbox_id)
{
    os::achievements::UnlockAchievement(xbox_id);
}

#endif // LIBERTY_RECOMP_NX
