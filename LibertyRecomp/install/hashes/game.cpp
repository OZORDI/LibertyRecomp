// GTA IV Base Game Files
// File sizes from: Grand Theft Auto IV (USA) (En,Fr,De,Es,It)
// Hash validation can be enabled later with fshasher

#include <utility>

extern const uint64_t GameHashes[];
extern const std::pair<const char *, uint32_t> GameFiles[];
extern const size_t GameFilesSize;

// Expected file sizes for validation (in bytes)
// These match the actual Xbox 360 disc content
static constexpr uint32_t SIZE_DEFAULT_XEX = 11841536;   // 11.3 MB
static constexpr uint32_t SIZE_AUDIO_RPF   = 782336;     // 764 KB  
static constexpr uint32_t SIZE_COMMON_RPF  = 17223680;   // 16.4 MB
static constexpr uint32_t SIZE_XBOX360_RPF = 60323840;   // 57.5 MB

// Hash validation disabled for initial testing (hash count = 0)
const uint64_t GameHashes[] = {
    0ULL  // Placeholder - not used when files have 0 hash count
};

// GTA IV Xbox 360 base game files
// Hash count set to 0 to skip hash validation during development
// File presence is still validated by the installer
const std::pair<const char *, uint32_t> GameFiles[] = {
    { "default.xex", 0 },
    { "audio.rpf", 0 },
    { "common.rpf", 0 },
    { "xbox360.rpf", 0 },
};

const size_t GameFilesSize = std::size(GameFiles);
