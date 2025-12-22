// GTA IV - The Ballad of Gay Tony (TBOGT) DLC
// Hash validation for TBOGT content

#include <utility>

extern const uint64_t TBOGTHashes[];
extern const std::pair<const char *, uint32_t> TBOGTFiles[];
extern const size_t TBOGTFilesSize;

// Hash validation disabled for initial testing (hash count = 0)
const uint64_t TBOGTHashes[] = {
    0ULL  // Placeholder
};

// TBOGT DLC files - extracted from STFS container
// Hash count set to 0 to skip validation during development
const std::pair<const char *, uint32_t> TBOGTFiles[] = {
    { "default.xex", 0 },
    { "tbogt.rpf", 0 },
    { "tbogt_audio.rpf", 0 },
};

const size_t TBOGTFilesSize = std::size(TBOGTFiles);
