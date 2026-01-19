/**
 * CRT Initialization - Xbox 360 initterm-style Global Constructor Support
 * 
 * Implementation of runtime constructor table parsing and execution.
 */

#include <stdafx.h>
#include "crt_init.h"
#include <os/logger.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstring>

namespace kernel {

// Global instance
CRTInitializer g_crtInitializer;

CRTInitializer::CRTInitializer() {
    InitializeDefaultSkipList();
}

void CRTInitializer::InitializeDefaultSkipList() {
    // Known problematic addresses from previous debugging:
    // 0x82A08AE4: memcpy helper function incorrectly in constructor table
    //             When called without proper register setup, it corrupts memory
    //             (specifically the critical section at 0x82A97FB4)
    AddToSkipList(0x82A08AE4, "memcpy helper - corrupts CS when called with garbage regs");
}

uint32_t CRTInitializer::ReadBE32(const uint8_t* data) {
    return (static_cast<uint32_t>(data[0]) << 24) |
           (static_cast<uint32_t>(data[1]) << 16) |
           (static_cast<uint32_t>(data[2]) << 8) |
           (static_cast<uint32_t>(data[3]));
}

size_t CRTInitializer::ParseFromBinary(const uint8_t* binary_data, size_t binary_size) {
    if (!binary_data || binary_size < TABLE_FILE_OFFSET + ENTRY_STRIDE) {
        LOG_ERROR("[CRT] Invalid binary data for constructor table parsing");
        return 0;
    }
    
    constructors_.clear();
    stats_ = Stats{};
    
    LOG_WARNING("[CRT] Parsing constructor table from binary...");
    LOGF_WARNING("[CRT]   File offset: 0x{:X}", TABLE_FILE_OFFSET);
    LOGF_WARNING("[CRT]   Virtual address: 0x{:08X}", TABLE_VA);
    LOGF_WARNING("[CRT]   Entry stride: {} bytes", ENTRY_STRIDE);
    
    // Scan the constructor table
    // Table ends when we hit a null pointer or leave the expected address range
    size_t offset = TABLE_FILE_OFFSET;
    size_t max_entries = 256;  // Safety limit
    
    while (offset + ENTRY_STRIDE <= binary_size && constructors_.size() < max_entries) {
        uint32_t func_addr = ReadBE32(binary_data + offset);
        uint32_t priority = ReadBE32(binary_data + offset + 4);
        
        // Check for end of table (null or out of expected range)
        if (func_addr == 0) {
            LOGF_WARNING("[CRT] Found null terminator at offset 0x{:X}", offset);
            break;
        }
        
        // Check if still in constructor address range
        if (func_addr < CTOR_SECTION_START || func_addr >= CTOR_SECTION_END) {
            LOGF_WARNING("[CRT] Address 0x{:08X} outside constructor range, table end at offset 0x{:X}", 
                       func_addr, offset);
            break;
        }
        
        ConstructorEntry entry;
        entry.address = func_addr;
        entry.priority = priority;
        
        // Validate the address
        auto validation = ValidateAddress(func_addr, nullptr);
        entry.valid = validation.valid;
        entry.skip_reason = validation.reason;
        
        constructors_.push_back(entry);
        stats_.total_entries++;
        
        if (!entry.valid) {
            if (func_addr == 0) {
                stats_.null_entries++;
            } else if (func_addr < CODE_SECTION_START || func_addr >= CODE_SECTION_END) {
                stats_.out_of_range_entries++;
            } else {
                stats_.skipped_entries++;
            }
        } else {
            stats_.valid_entries++;
        }
        
        offset += ENTRY_STRIDE;
    }
    
    LOGF_WARNING("[CRT] Parsed {} constructor entries:", constructors_.size());
    LOGF_WARNING("[CRT]   Valid: {}", stats_.valid_entries);
    LOGF_WARNING("[CRT]   Skipped: {}", stats_.skipped_entries);
    LOGF_WARNING("[CRT]   Out of range: {}", stats_.out_of_range_entries);
    
    initialized_ = true;
    return stats_.valid_entries;
}

size_t CRTInitializer::ParseFromMemory(uint8_t* base) {
    if (!base) {
        LOG_ERROR("[CRT] Invalid base address for constructor table parsing");
        return 0;
    }
    
    constructors_.clear();
    stats_ = Stats{};
    
    LOG_WARNING("[CRT] Parsing constructor table from memory...");
    LOGF_WARNING("[CRT]   Table VA: 0x{:08X}", TABLE_VA);
    
    // Calculate memory offset from base
    // The table is at VA 0x82235A38, base is typically 0x82120000
    uint32_t table_offset = TABLE_VA - CODE_SECTION_START;
    uint8_t* table_ptr = base + table_offset;
    
    size_t max_entries = 256;
    size_t entry_index = 0;
    
    while (entry_index < max_entries) {
        uint8_t* entry_ptr = table_ptr + (entry_index * ENTRY_STRIDE);
        
        // Read big-endian addresses from memory
        uint32_t func_addr = ReadBE32(entry_ptr);
        uint32_t priority = ReadBE32(entry_ptr + 4);
        
        // End of table check
        if (func_addr == 0) {
            break;
        }
        
        if (func_addr < CTOR_SECTION_START || func_addr >= CTOR_SECTION_END) {
            break;
        }
        
        ConstructorEntry entry;
        entry.address = func_addr;
        entry.priority = priority;
        
        // Validate with memory access for prologue check
        auto validation = ValidateAddress(func_addr, base);
        entry.valid = validation.valid;
        entry.skip_reason = validation.reason;
        
        constructors_.push_back(entry);
        stats_.total_entries++;
        
        if (entry.valid) {
            stats_.valid_entries++;
        } else {
            stats_.skipped_entries++;
        }
        
        entry_index++;
    }
    
    LOGF_WARNING("[CRT] Parsed {} constructors from memory (valid: {})", 
               constructors_.size(), stats_.valid_entries);
    
    initialized_ = true;
    return stats_.valid_entries;
}

ValidationResult CRTInitializer::ValidateAddress(uint32_t addr, uint8_t* base) {
    ValidationResult result;
    result.valid = true;
    
    // Check 1: Null pointer
    if (addr == 0) {
        result.valid = false;
        result.reason = "null pointer";
        return result;
    }
    
    // Check 2: Within code section
    if (addr < CODE_SECTION_START || addr >= CODE_SECTION_END) {
        result.valid = false;
        result.reason = "outside code section";
        return result;
    }
    
    // Check 3: Skip list
    if (skip_list_.count(addr)) {
        result.valid = false;
        auto it = skip_reasons_.find(addr);
        result.reason = (it != skip_reasons_.end()) ? it->second : "in skip list";
        return result;
    }
    
    // Check 4: Function prologue (if we have memory access)
    if (base && !CheckFunctionPrologue(addr, base)) {
        result.valid = false;
        result.reason = "invalid function prologue";
        return result;
    }
    
    return result;
}

bool CRTInitializer::CheckFunctionPrologue(uint32_t addr, uint8_t* base) {
    // Calculate offset from base
    if (addr < CODE_SECTION_START) return false;
    
    uint32_t offset = addr - CODE_SECTION_START;
    uint8_t* func_ptr = base + offset;
    
    // Read first instruction (big-endian)
    uint32_t first_instr = ReadBE32(func_ptr);
    
    // Common PPC function prologues:
    // - mflr r0 (0x7C0802A6)
    // - stwu r1, -X(r1) (0x9421xxxx)
    // - stw r31, X(r1) (0x93E1xxxx)
    // - mr rX, rY (0x7CxxxxB8)
    // - lis rX, imm (0x3Cxxxxxx)
    // - li rX, imm (0x38xxxxxx)
    
    // Check for common prologue patterns
    uint32_t opcode = (first_instr >> 26) & 0x3F;
    
    switch (opcode) {
        case 31:  // Extended opcodes (mflr, mr, etc.)
            return true;
        case 37:  // stwu
            return true;
        case 36:  // stw
            return true;
        case 15:  // lis
            return true;
        case 14:  // li/addi
            return true;
        case 18:  // b/bl (tail call or trampoline)
            return true;
        case 16:  // bc (conditional branch - unusual for prologue but valid)
            return true;
        case 24:  // ori (sometimes used for nop)
            return true;
        default:
            // Unknown opcode - could still be valid
            // Be permissive to avoid false negatives
            return true;
    }
}

size_t CRTInitializer::CallConstructors(void* ctx, uint8_t* base, 
                                        LookupFunc lookup, CallFunc call) {
    if (!initialized_) {
        LOG_ERROR("[CRT] CallConstructors called before initialization");
        return 0;
    }
    
    LOGF_WARNING("[CRT-INITTERM] Calling {} constructors...", constructors_.size());
    
    size_t called = 0;
    size_t skipped = 0;
    size_t not_found = 0;
    
    for (size_t i = 0; i < constructors_.size(); i++) {
        const auto& entry = constructors_[i];
        
        if (!entry.valid) {
            skipped++;
            continue;
        }
        
        // Look up the recompiled function
        void* func = lookup(base, entry.address);
        if (!func) {
            not_found++;
            continue;
        }
        
        // Call the constructor
        try {
            call(func, ctx, base);
            called++;
        } catch (...) {
            LOGF_ERROR("[CRT] Constructor 0x{:08X} threw exception", entry.address);
            stats_.call_failed_entries++;
        }
    }
    
    stats_.called_entries = called;
    
    LOGF_WARNING("[CRT-INITTERM] Complete: called={}, skipped={}, not_found={}", 
               called, skipped, not_found);
    
    return called;
}

void CRTInitializer::AddToSkipList(uint32_t addr, const std::string& reason) {
    skip_list_.insert(addr);
    skip_reasons_[addr] = reason;
    
    // Update existing entries if already parsed
    for (auto& entry : constructors_) {
        if (entry.address == addr) {
            entry.valid = false;
            entry.skip_reason = reason;
        }
    }
}

size_t CRTInitializer::LoadSkipListFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        LOGF_WARNING("[CRT] Could not open skip list file: {}", path);
        return 0;
    }
    
    size_t loaded = 0;
    std::string line;
    
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') continue;
        
        // Parse: ADDRESS [REASON]
        // e.g., "0x82A08AE4 memcpy helper"
        std::istringstream iss(line);
        std::string addr_str;
        iss >> addr_str;
        
        if (addr_str.empty()) continue;
        
        uint32_t addr = 0;
        try {
            addr = std::stoul(addr_str, nullptr, 0);
        } catch (...) {
            LOGF_WARNING("[CRT] Invalid address in skip list: {}", addr_str);
            continue;
        }
        
        // Get optional reason (rest of line)
        std::string reason;
        std::getline(iss >> std::ws, reason);
        if (reason.empty()) reason = "from config file";
        
        AddToSkipList(addr, reason);
        loaded++;
    }
    
    LOGF_WARNING("[CRT] Loaded {} entries from skip list: {}", loaded, path);
    return loaded;
}

bool CRTInitializer::SaveSkipListToFile(const std::string& path) const {
    std::ofstream file(path);
    if (!file.is_open()) {
        LOGF_ERROR("[CRT] Could not create skip list file: {}", path);
        return false;
    }
    
    file << "# CRT Constructor Skip List\n";
    file << "# Format: ADDRESS REASON\n";
    file << "# Addresses can be in hex (0x...) or decimal\n\n";
    
    for (uint32_t addr : skip_list_) {
        file << "0x" << std::hex << std::uppercase << addr;
        
        auto it = skip_reasons_.find(addr);
        if (it != skip_reasons_.end() && !it->second.empty()) {
            file << " " << it->second;
        }
        file << "\n";
    }
    
    return true;
}

size_t CRTInitializer::GetValidCount() const {
    return std::count_if(constructors_.begin(), constructors_.end(),
                         [](const ConstructorEntry& e) { return e.valid; });
}

size_t CRTInitializer::GetSkippedCount() const {
    return std::count_if(constructors_.begin(), constructors_.end(),
                         [](const ConstructorEntry& e) { return !e.valid; });
}

} // namespace kernel
