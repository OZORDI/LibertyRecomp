// GTA IV Title Update hash definitions

#pragma once

#include <utility>

// Title Update version info
struct TitleUpdateInfo {
    const char* version;
    uint32_t expectedSize;
    uint64_t hash;
};

extern const TitleUpdateInfo TitleUpdates[];
extern const size_t TitleUpdatesSize;
