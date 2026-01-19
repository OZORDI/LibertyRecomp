/**
 * CRT Initialization - Xbox 360 initterm-style Global Constructor Support
 * 
 * Parses constructor table at runtime from the binary instead of hardcoded addresses.
 * Properly validates each entry before calling and supports configurable skip lists.
 * 
 * Based on Xbox 360 CRT $XCA/$XCZ section layout.
 */

#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <functional>
#include <unordered_set>

namespace kernel {

// Constructor entry from the binary
struct ConstructorEntry {
    uint32_t address;       // Function pointer (big-endian in binary)
    uint32_t priority;      // Priority/padding byte (second 4 bytes of 8-byte stride)
    bool valid;             // Passed validation
    std::string skip_reason; // If skipped, why
};

// Validation result
struct ValidationResult {
    bool valid;
    std::string reason;
};

/**
 * CRTInitializer - Parses and calls Xbox 360 global constructors
 * 
 * Key features:
 * - Runtime parsing from binary (no hardcoded addresses)
 * - Proper validation (null check, code section check, function prologue check)
 * - Configurable skip list (loaded from config file or defaults)
 * - Destructor registration (atexit table) - TODO
 * - Thread-safe initialization guard
 */
class CRTInitializer {
public:
    // Binary layout constants
    static constexpr uint32_t CODE_SECTION_START = 0x82120000;
    static constexpr uint32_t CODE_SECTION_END = 0x82C00000;
    static constexpr uint32_t CTOR_SECTION_START = 0x82A00000;
    static constexpr uint32_t CTOR_SECTION_END = 0x82B00000;
    
    // Table location in .pdata section
    // File offset 0x115A38, VA 0x82235A38, 8-byte stride entries
    static constexpr uint32_t TABLE_FILE_OFFSET = 0x115A38;
    static constexpr uint32_t TABLE_VA = 0x82235A38;
    static constexpr size_t ENTRY_STRIDE = 8;
    
    CRTInitializer();
    ~CRTInitializer() = default;
    
    /**
     * Parse constructor table from binary data
     * @param binary_data Pointer to the loaded default.bin
     * @param binary_size Size of the binary
     * @return Number of valid constructors found
     */
    size_t ParseFromBinary(const uint8_t* binary_data, size_t binary_size);
    
    /**
     * Parse constructor table from memory-mapped image
     * @param base Base address of mapped image
     * @return Number of valid constructors found
     */
    size_t ParseFromMemory(uint8_t* base);
    
    /**
     * Call all constructors in order
     * @param ctx PPC context
     * @param base Base address for function lookup
     * @param lookup_func Function to lookup recompiled function by address
     * @return Number of constructors successfully called
     */
    using LookupFunc = std::function<void*(uint8_t* base, uint32_t addr)>;
    using CallFunc = std::function<void(void* func, void* ctx, uint8_t* base)>;
    
    size_t CallConstructors(void* ctx, uint8_t* base, 
                           LookupFunc lookup, CallFunc call);
    
    /**
     * Add address to skip list
     * @param addr Address to skip
     * @param reason Human-readable reason for skipping
     */
    void AddToSkipList(uint32_t addr, const std::string& reason);
    
    /**
     * Load skip list from config file
     * @param path Path to skip list file (one address per line, # for comments)
     * @return Number of entries loaded
     */
    size_t LoadSkipListFromFile(const std::string& path);
    
    /**
     * Save skip list to config file
     * @param path Path to save to
     * @return Success
     */
    bool SaveSkipListToFile(const std::string& path) const;
    
    // Accessors
    const std::vector<ConstructorEntry>& GetConstructors() const { return constructors_; }
    size_t GetTotalCount() const { return constructors_.size(); }
    size_t GetValidCount() const;
    size_t GetSkippedCount() const;
    bool IsInitialized() const { return initialized_; }
    
    // Statistics
    struct Stats {
        size_t total_entries;
        size_t valid_entries;
        size_t skipped_entries;
        size_t null_entries;
        size_t out_of_range_entries;
        size_t invalid_prologue_entries;
        size_t called_entries;
        size_t call_failed_entries;
    };
    const Stats& GetStats() const { return stats_; }

private:
    /**
     * Validate a constructor address
     * @param addr Address to validate
     * @param base Base address for memory access (optional)
     * @return Validation result with reason
     */
    ValidationResult ValidateAddress(uint32_t addr, uint8_t* base = nullptr);
    
    /**
     * Check if address points to valid function prologue
     * @param addr Address to check
     * @param base Base address for memory access
     * @return True if looks like valid function start
     */
    bool CheckFunctionPrologue(uint32_t addr, uint8_t* base);
    
    /**
     * Read big-endian uint32 from data
     */
    static uint32_t ReadBE32(const uint8_t* data);
    
    std::vector<ConstructorEntry> constructors_;
    std::unordered_set<uint32_t> skip_list_;
    std::unordered_map<uint32_t, std::string> skip_reasons_;
    Stats stats_{};
    bool initialized_ = false;
    
    // Default skip list entries (known bad addresses)
    void InitializeDefaultSkipList();
};

// Global instance
extern CRTInitializer g_crtInitializer;

} // namespace kernel
