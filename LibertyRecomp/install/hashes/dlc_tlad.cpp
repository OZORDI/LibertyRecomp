// GTA IV - The Lost and Damned (TLAD) DLC
// Hash validation for TLAD content

#include <utility>

extern const uint64_t TLADHashes[];
extern const std::pair<const char *, uint32_t> TLADFiles[];
extern const size_t TLADFilesSize;

// Hash validation disabled for initial testing (hash count = 0)
const uint64_t TLADHashes[] = {
    0ULL  // Placeholder
};

// TLAD DLC files - extracted from STFS container
// Hash count set to 0 to skip validation during development
const std::pair<const char *, uint32_t> TLADFiles[] = {
    { "default.xex", 0 },
    { "tlad.rpf", 0 },
    { "tlad_audio.rpf", 0 },
};

const size_t TLADFilesSize = std::size(TLADFiles);
