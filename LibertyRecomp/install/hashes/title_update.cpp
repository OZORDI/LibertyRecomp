// GTA IV Title Updates
// Version information and hash validation

#include "title_update.h"

// Known GTA IV Title Update versions with file sizes
// Hash validation can be enabled later
const TitleUpdateInfo TitleUpdates[] = {
    { "v4", 3399920, 0ULL },   // Grand Theft Auto IV (USA) (v4).zip
    { "v5", 3537299, 0ULL },   // Grand Theft Auto IV (USA) (v5).zip
    { "v6", 3475604, 0ULL },   // Grand Theft Auto IV (USA) (v6).zip
    { "v8", 3461723, 0ULL },   // Grand Theft Auto IV (USA) (v8).zip - latest
};

const size_t TitleUpdatesSize = std::size(TitleUpdates);
