#pragma once
#include <cstdint>

/**
 * Initialize Xbox 360 Xenon memory regions that the game assumes exist.
 * Must be called BEFORE any game code executes.
 * 
 * The Xbox 360 has a specific memory contract where certain regions are
 * guaranteed to be allocated and zeroed on boot. The game relies on this
 * contract and will crash if these regions contain garbage data.
 * 
 * Zeros the following regions per Xbox 360 memory contract:
 * - Stream pool: 0x82000000-0x82020000 (128 KB)
 * - XEX data: 0x82020000-0x82120000 (1 MB)
 * - Kernel runtime: 0x82A90000-0x82AA0000 (64 KB)
 * - Static data (BSS): 0x83000000-0x83200000 (2 MB)
 * 
 * Does NOT touch:
 * - Code region: 0x82120000-0x82A13D5C (managed by recompiler)
 * - Import region: 0x82A00000-0x82B00000 (managed by XEX loader)
 * - Heap regions: managed by game allocator
 * 
 * @param base Pointer to the base of PPC memory (g_memory.base)
 */
void InitializeXenonMemoryRegions(uint8_t* base);
