#include "xenon_memory.h"
#include <os/logger.h>
#include <cstring>

void InitializeXenonMemoryRegions(uint8_t* base) {
    LOG_WARNING("[Xenon] Initializing Xbox 360 memory regions per Xenon contract");
    
    // Zero ENTIRE PPC data region in one contiguous call (0x82000000-0x831F0000)
    // This covers all game data regions:
    // - Stream pool (0x82000000-0x82020000)
    // - XEX data (0x82020000-0x82120000)
    // - Code data (0x82120000-0x82A90000)
    // - Kernel runtime (0x82A90000-0x82AA0000)
    // - Game globals (0x82AA0000-0x83000000)
    // - Static data/BSS (0x83000000-0x831F0000)
    //
    // CRITICAL: Previous piecemeal approach had gaps that caused garbage pointer crashes.
    // The crash address 0x3f1aa1403e0 appeared repeatedly due to unzeroed memory.
    // This single contiguous memset eliminates all gaps.
    //
    // Size: 0x831F0000 - 0x82000000 = 0x11F0000 = 18,808,832 bytes (~17.9 MB)
    LOG_WARNING("[Xenon] Zeroing entire PPC data region: 0x82000000-0x831F0000 (17.9 MB)");
    memset(base + 0x82000000, 0, 0x11F0000);
    
    LOG_WARNING("[Xenon] Memory region initialization complete");
    LOG_WARNING("[Xenon] Total zeroed: 17.9 MB contiguous (0x82000000-0x831F0000)");
}
