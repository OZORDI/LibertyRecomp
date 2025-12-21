/**
 * XEX Patcher for applying Xbox 360 Title Update patches (.xexp)
 * Adapted from Xenia (BSD-3-Clause license)
 * Copyright 2020 Ben Vanik. All rights reserved.
 * 
 * Note: Full XEXP patching requires LZX delta decompression which is complex.
 * This implementation provides the framework and falls back to xextool for
 * actual patch application if available.
 */

#include "xex_patcher.h"
#include <cstring>
#include <fstream>
#include <algorithm>

namespace liberty {
namespace install {

// AES keys for XEX decryption
const uint8_t XexPatcher::kXex2RetailKey[16] = {
    0x20, 0xB1, 0x85, 0xA5, 0x9D, 0x28, 0xFD, 0xC3,
    0x40, 0x58, 0x3F, 0xBB, 0x08, 0x96, 0xBF, 0x91
};

const uint8_t XexPatcher::kXex2DevkitKey[16] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

XexPatcher::XexPatcher()
    : m_baseLoaded(false)
    , m_patchLoaded(false)
    , m_patched(false)
    , m_baseTitleId(0)
    , m_baseVersion(0)
    , m_baseHeaderSize(0)
    , m_baseModuleFlags(0)
    , m_patchTitleId(0)
    , m_patchVersion(0)
    , m_patchHeaderSize(0)
    , m_patchModuleFlags(0)
    , m_deltaHeadersSourceOffset(0)
    , m_deltaHeadersSourceSize(0)
    , m_deltaHeadersTargetOffset(0)
    , m_sizeOfTargetHeaders(0)
    , m_deltaImageSourceOffset(0)
    , m_deltaImageSourceSize(0)
    , m_deltaImageTargetOffset(0)
{
    std::memset(m_baseSessionKey, 0, 16);
    std::memset(m_patchSessionKey, 0, 16);
}

XexPatcher::~XexPatcher()
{
}

bool XexPatcher::LoadBaseXex(const std::filesystem::path& xexPath)
{
    std::ifstream file(xexPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return false;
    }
    
    size_t fileSize = file.tellg();
    file.seekg(0);
    
    m_baseData.resize(fileSize);
    file.read(reinterpret_cast<char*>(m_baseData.data()), fileSize);
    
    if (!ParseXexHeader(m_baseData, false)) {
        m_baseData.clear();
        return false;
    }
    
    m_baseLoaded = true;
    return true;
}

bool XexPatcher::LoadPatch(const std::filesystem::path& xexpPath)
{
    std::ifstream file(xexpPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return false;
    }
    
    size_t fileSize = file.tellg();
    file.seekg(0);
    
    m_patchData.resize(fileSize);
    file.read(reinterpret_cast<char*>(m_patchData.data()), fileSize);
    
    if (!ParseXexHeader(m_patchData, true)) {
        m_patchData.clear();
        return false;
    }
    
    m_patchLoaded = true;
    return true;
}

bool XexPatcher::ParseXexHeader(const std::vector<uint8_t>& data, bool isPatch)
{
    if (data.size() < 24) {
        return false;
    }
    
    // Check magic (XEX2)
    if (data[0] != 'X' || data[1] != 'E' || data[2] != 'X' || data[3] != '2') {
        return false;
    }
    
    // Read module flags (offset 0x04, 4 bytes BE)
    uint32_t moduleFlags = (uint32_t(data[4]) << 24) | (uint32_t(data[5]) << 16) |
                           (uint32_t(data[6]) << 8) | uint32_t(data[7]);
    
    // Read header size (offset 0x10, 4 bytes BE)
    uint32_t headerSize = (uint32_t(data[0x10]) << 24) | (uint32_t(data[0x11]) << 16) |
                          (uint32_t(data[0x12]) << 8) | uint32_t(data[0x13]);
    
    // Read optional header count (offset 0x14, 4 bytes BE)
    uint32_t optHeaderCount = (uint32_t(data[0x14]) << 24) | (uint32_t(data[0x15]) << 16) |
                              (uint32_t(data[0x16]) << 8) | uint32_t(data[0x17]);
    
    // Parse optional headers to find execution info
    uint32_t titleId = 0;
    uint32_t version = 0;
    
    uint64_t optHeaderOffset = 0x18;
    for (uint32_t i = 0; i < optHeaderCount && optHeaderOffset + 8 <= data.size(); i++) {
        uint32_t key = (uint32_t(data[optHeaderOffset]) << 24) | 
                       (uint32_t(data[optHeaderOffset + 1]) << 16) |
                       (uint32_t(data[optHeaderOffset + 2]) << 8) | 
                       uint32_t(data[optHeaderOffset + 3]);
        
        uint32_t value = (uint32_t(data[optHeaderOffset + 4]) << 24) | 
                         (uint32_t(data[optHeaderOffset + 5]) << 16) |
                         (uint32_t(data[optHeaderOffset + 6]) << 8) | 
                         uint32_t(data[optHeaderOffset + 7]);
        
        // Check if this is execution info (key 0x00040006)
        if (key == kXexHeaderExecutionInfo) {
            // value is offset to execution info structure
            if (value + 0x18 <= data.size()) {
                // Execution info structure:
                // +0x00: Media ID (4 bytes)
                // +0x04: Version (4 bytes)
                // +0x08: Base Version (4 bytes)
                // +0x0C: Title ID (4 bytes)
                // +0x10: Platform (1 byte)
                // +0x11: Executable Type (1 byte)
                // +0x12: Disc Number (1 byte)
                // +0x13: Disc Count (1 byte)
                version = (uint32_t(data[value + 4]) << 24) | 
                          (uint32_t(data[value + 5]) << 16) |
                          (uint32_t(data[value + 6]) << 8) | 
                          uint32_t(data[value + 7]);
                
                titleId = (uint32_t(data[value + 0x0C]) << 24) | 
                          (uint32_t(data[value + 0x0D]) << 16) |
                          (uint32_t(data[value + 0x0E]) << 8) | 
                          uint32_t(data[value + 0x0F]);
            }
        }
        
        // Check for delta patch descriptor (key 0x000005FF) for patches
        if (isPatch && key == kXexHeaderDeltaPatchDescriptor) {
            if (value + 0x40 <= data.size()) {
                // Delta patch descriptor structure (partial)
                m_deltaHeadersSourceOffset = (uint32_t(data[value + 0x04]) << 24) |
                                              (uint32_t(data[value + 0x05]) << 16) |
                                              (uint32_t(data[value + 0x06]) << 8) |
                                              uint32_t(data[value + 0x07]);
                
                m_deltaHeadersSourceSize = (uint32_t(data[value + 0x08]) << 24) |
                                            (uint32_t(data[value + 0x09]) << 16) |
                                            (uint32_t(data[value + 0x0A]) << 8) |
                                            uint32_t(data[value + 0x0B]);
                
                m_deltaHeadersTargetOffset = (uint32_t(data[value + 0x0C]) << 24) |
                                              (uint32_t(data[value + 0x0D]) << 16) |
                                              (uint32_t(data[value + 0x0E]) << 8) |
                                              uint32_t(data[value + 0x0F]);
                
                m_sizeOfTargetHeaders = (uint32_t(data[value + 0x10]) << 24) |
                                         (uint32_t(data[value + 0x11]) << 16) |
                                         (uint32_t(data[value + 0x12]) << 8) |
                                         uint32_t(data[value + 0x13]);
            }
        }
        
        optHeaderOffset += 8;
    }
    
    if (isPatch) {
        m_patchTitleId = titleId;
        m_patchVersion = version;
        m_patchHeaderSize = headerSize;
        m_patchModuleFlags = moduleFlags;
    } else {
        m_baseTitleId = titleId;
        m_baseVersion = version;
        m_baseHeaderSize = headerSize;
        m_baseModuleFlags = moduleFlags;
    }
    
    return true;
}

bool XexPatcher::IsPatchCompatible() const
{
    if (!m_baseLoaded || !m_patchLoaded) {
        return false;
    }
    
    // Check if patch has the patch flag set
    if (!(m_patchModuleFlags & (kXexModulePatch | kXexModulePatchDelta | kXexModulePatchFull))) {
        return false;
    }
    
    // Title IDs should match
    if (m_baseTitleId != m_patchTitleId) {
        return false;
    }
    
    return true;
}

XexPatchResult XexPatcher::ApplyPatch()
{
    XexPatchResult result;
    result.success = false;
    result.originalVersion = m_baseVersion;
    result.patchedVersion = m_patchVersion;
    
    if (!m_baseLoaded) {
        result.errorMessage = "Base XEX not loaded";
        return result;
    }
    
    if (!m_patchLoaded) {
        result.errorMessage = "Patch not loaded";
        return result;
    }
    
    if (!IsPatchCompatible()) {
        result.errorMessage = "Patch is not compatible with base XEX";
        return result;
    }
    
    // For full patches (not delta), simply replace the entire XEX
    if (m_patchModuleFlags & kXexModulePatchFull) {
        // Full patch - the XEXP IS the new XEX
        m_patchedData = m_patchData;
        m_patched = true;
        result.success = true;
        return result;
    }
    
    // Delta patches require LZX delta decompression
    // This is complex and typically requires the MS LZX implementation
    // For now, we'll indicate that external tools are needed
    
    result.errorMessage = "Delta patches require xextool for application. "
                          "Please use: xextool -p patch.xexp -o output.xex input.xex";
    
    // TODO: Implement full LZX delta decompression from Xenia's code
    // This would require porting lzx.cc and the ApplyPatch logic
    
    return result;
}

bool XexPatcher::SavePatchedXex(const std::filesystem::path& outPath)
{
    if (!m_patched || m_patchedData.empty()) {
        return false;
    }
    
    std::ofstream file(outPath, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    file.write(reinterpret_cast<const char*>(m_patchedData.data()), m_patchedData.size());
    return file.good();
}

void XexPatcher::AesDecrypt(const uint8_t* key, const uint8_t* input, 
                            size_t length, uint8_t* output)
{
    // Placeholder for AES decryption
    // In production, this would use a proper AES implementation
    // For now, we just copy the data
    std::memcpy(output, input, length);
}

bool XexPatcher::LzxDeltaApplyPatch(const uint8_t* patchData, uint32_t patchSize,
                                     uint32_t windowSize, uint8_t* target)
{
    // Placeholder for LZX delta decompression
    // This requires porting Xenia's lzx.cc which is significant work
    // For MVP, we rely on xextool binary
    return false;
}

} // namespace install
} // namespace liberty
