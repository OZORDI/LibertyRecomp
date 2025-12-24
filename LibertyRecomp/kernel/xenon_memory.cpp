#include "xenon_memory.h"
#include <os/logger.h>
#include <cstring>

void InitializeXenonMemoryRegions(uint8_t* base) {
    LOG_WARNING("[Xenon] Initializing Xbox 360 memory regions per Xenon contract");
    
    // Region 1: Stream pool (0x82000000-0x82020000)
    // The game's streaming system assumes this 128 KB region is pre-allocated
    // and zeroed for stream object allocation. sub_822C1A30 will initialize
    // stream structures here if stream.ini parsing succeeds.
    LOG_WARNING("[Xenon] Zeroing stream pool: 0x82000000-0x82020000 (128 KB)");
    memset(base + 0x82000000, 0, 0x20000);
    
    // Region 2: XEX data region (0x82020000-0x82120000)
    // Contains initialization tables, XEX metadata, and other loader data.
    // The game expects this to be zeroed except where the XEX loader sets values.
    LOG_WARNING("[Xenon] Zeroing XEX data region: 0x82020000-0x82120000 (1 MB)");
    memset(base + 0x82020000, 0, 0x100000);
    
    // Region 3: Kernel runtime data (0x82A90000-0x82AA0000)
    // Contains TLS indices, thread pool, callback lists, and other kernel structures.
    // Must be zeroed so the game can properly initialize these structures.
    LOG_WARNING("[Xenon] Zeroing kernel runtime: 0x82A90000-0x82AA0000 (64 KB)");
    memset(base + 0x82A90000, 0, 0x10000);
    
    // Region 4: Static data/BSS (0x83000000-0x831F0000)
    // Global variables and static data. Per C/C++ BSS contract, must be zeroed.
    // Stop at 0x831F0000 (PPC_IMAGE_BASE + PPC_IMAGE_SIZE) to avoid the protected
    // function table region that starts there.
    LOG_WARNING("[Xenon] Zeroing static data (BSS): 0x83000000-0x831F0000 (1.99 MB)");
    memset(base + 0x83000000, 0, 0x1F0000);  // Stop before function table
    
    LOG_WARNING("[Xenon] Memory region initialization complete");
    LOG_WARNING("[Xenon] IMPORTANT: Import region 0x82A00000-0x82B00000 left untouched (system-managed)");
}
