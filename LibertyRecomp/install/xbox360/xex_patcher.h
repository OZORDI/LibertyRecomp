/**
 * XEX Patcher for applying Xbox 360 Title Update patches (.xexp)
 * Adapted from Xenia (BSD-3-Clause license)
 * Copyright 2020 Ben Vanik. All rights reserved.
 */

#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace liberty {
namespace install {

// XEX header flags
enum XexModuleFlags : uint32_t {
    kXexModuleTitleModule = 0x00000001,
    kXexModuleExportsToTitle = 0x00000002,
    kXexModuleSystemDebugger = 0x00000004,
    kXexModuleDllModule = 0x00000008,
    kXexModulePatch = 0x00000010,
    kXexModulePatchFull = 0x00000020,
    kXexModulePatchDelta = 0x00000040,
    kXexModuleUserMode = 0x00000080,
};

// XEX header keys for optional headers
enum XexHeaderKeys : uint32_t {
    kXexHeaderResourceInfo = 0x000002FF,
    kXexHeaderFileFormatInfo = 0x000003FF,
    kXexHeaderDeltaPatchDescriptor = 0x000005FF,
    kXexHeaderBaseReference = 0x00000405,
    kXexHeaderExecutionInfo = 0x00040006,
    kXexHeaderOriginalBaseAddress = 0x00010001,
    kXexHeaderEntryPoint = 0x00010100,
    kXexHeaderTlsInfo = 0x00020200,
    kXexHeaderDefaultStackSize = 0x00020301,
    kXexHeaderDefaultFilesystemCacheSize = 0x00020302,
    kXexHeaderDefaultHeapSize = 0x00020303,
    kXexHeaderPageHeapSizeAndFlags = 0x00028002,
    kXexHeaderSystemFlags = 0x00030000,
    kXexHeaderPrivileges = 0x00030100,
    kXexHeaderImports = 0x000103FF,
    kXexHeaderImageBaseAddress = 0x00010201,
};

// Encryption types
enum XexEncryptionType : uint16_t {
    kXexEncryptionNone = 0,
    kXexEncryptionNormal = 1,
};

// Compression types
enum XexCompressionType : uint16_t {
    kXexCompressionNone = 0,
    kXexCompressionBasic = 1,
    kXexCompressionNormal = 2,
    kXexCompressionDelta = 3,
};

// XEX patch result
struct XexPatchResult {
    bool success;
    std::string errorMessage;
    uint32_t originalVersion;
    uint32_t patchedVersion;
};

class XexPatcher {
public:
    XexPatcher();
    ~XexPatcher();

    // Load the base XEX file
    bool LoadBaseXex(const std::filesystem::path& xexPath);
    
    // Load the XEXP patch file
    bool LoadPatch(const std::filesystem::path& xexpPath);
    
    // Check if patch is compatible with base XEX
    bool IsPatchCompatible() const;
    
    // Apply the patch to the loaded XEX
    XexPatchResult ApplyPatch();
    
    // Save the patched XEX to disk
    bool SavePatchedXex(const std::filesystem::path& outPath);
    
    // Get version info
    uint32_t GetBaseVersion() const { return m_baseVersion; }
    uint32_t GetPatchVersion() const { return m_patchVersion; }
    uint32_t GetBaseTitleId() const { return m_baseTitleId; }
    uint32_t GetPatchTitleId() const { return m_patchTitleId; }
    
    // Check if files are loaded
    bool IsBaseLoaded() const { return m_baseLoaded; }
    bool IsPatchLoaded() const { return m_patchLoaded; }

private:
    // AES key constants
    static const uint8_t kXex2RetailKey[16];
    static const uint8_t kXex2DevkitKey[16];
    
    // Parse XEX header
    bool ParseXexHeader(const std::vector<uint8_t>& data, bool isPatch);
    
    // Get optional header
    bool GetOptHeader(uint32_t key, uint32_t& offset, uint32_t& size);
    
    // Decrypt data
    void AesDecrypt(const uint8_t* key, const uint8_t* input, 
                    size_t length, uint8_t* output);
    
    // LZX delta decompression
    bool LzxDeltaApplyPatch(const uint8_t* patchData, uint32_t patchSize,
                            uint32_t windowSize, uint8_t* target);
    
    // XEX data
    std::vector<uint8_t> m_baseData;
    std::vector<uint8_t> m_patchData;
    std::vector<uint8_t> m_patchedData;
    
    // Loaded state
    bool m_baseLoaded;
    bool m_patchLoaded;
    bool m_patched;
    
    // Base XEX info
    uint32_t m_baseTitleId;
    uint32_t m_baseVersion;
    uint32_t m_baseHeaderSize;
    uint32_t m_baseModuleFlags;
    uint8_t m_baseSessionKey[16];
    
    // Patch info
    uint32_t m_patchTitleId;
    uint32_t m_patchVersion;
    uint32_t m_patchHeaderSize;
    uint32_t m_patchModuleFlags;
    uint8_t m_patchSessionKey[16];
    
    // Delta patch descriptor offsets
    uint32_t m_deltaHeadersSourceOffset;
    uint32_t m_deltaHeadersSourceSize;
    uint32_t m_deltaHeadersTargetOffset;
    uint32_t m_sizeOfTargetHeaders;
    uint32_t m_deltaImageSourceOffset;
    uint32_t m_deltaImageSourceSize;
    uint32_t m_deltaImageTargetOffset;
};

} // namespace install
} // namespace liberty
