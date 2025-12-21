/**
 * STFS Parser for Xbox 360 Title Update files
 * Adapted from Xenia (BSD-3-Clause license)
 * Copyright 2020 Ben Vanik. All rights reserved.
 */

#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace liberty {
namespace install {

// STFS Package types
enum class StfsPackageType : uint32_t {
    kCon = 0x434F4E20,   // "CON "
    kPirs = 0x50495253,  // "PIRS"
    kLive = 0x4C495645,  // "LIVE"
};

// Content types
enum class XContentType : uint32_t {
    kSavedGame = 0x00000001,
    kMarketplaceContent = 0x00000002,
    kPublisher = 0x00000003,
    kXbox360Title = 0x00001000,
    kIptvPauseBuffer = 0x00002000,
    kInstalledGame = 0x00004000,
    kXboxTitle = 0x00005000,
    kSocialTitle = 0x00006000,
    kGamesOnDemand = 0x00007000,
    kSUStoragePack = 0x00008000,
    kAvatarItem = 0x00009000,
    kProfile = 0x00010000,
    kGamerPicture = 0x00020000,
    kTheme = 0x00030000,
    kCacheFile = 0x00040000,
    kStorageDownload = 0x00050000,
    kXboxSavedGame = 0x00060000,
    kXboxDownload = 0x00070000,
    kGameDemo = 0x00080000,
    kVideo = 0x00090000,
    kGameTitle = 0x000A0000,
    kInstaller = 0x000B0000,  // Title Update!
    kGameTrailer = 0x000C0000,
    kArcadeTitle = 0x000D0000,
    kXNA = 0x000E0000,
    kLicenseStore = 0x000F0000,
    kMovie = 0x00100000,
    kTV = 0x00200000,
    kMusicVideo = 0x00300000,
    kGameVideo = 0x00400000,
    kPodcastVideo = 0x00500000,
    kViralVideo = 0x00600000,
    kCommunityGame = 0x02000000,
};

// File entry in STFS package
struct StfsFileEntry {
    std::string name;
    uint32_t size;
    uint32_t startBlock;
    bool isDirectory;
    bool isContiguous;
};

// Title Update metadata
struct TitleUpdateInfo {
    uint32_t titleId;
    uint32_t version;
    uint32_t baseVersion;
    std::string displayName;
    XContentType contentType;
    bool isValid;
};

class StfsParser {
public:
    StfsParser();
    ~StfsParser();

    // Open an STFS package file
    bool Open(const std::filesystem::path& path);
    
    // Close the package
    void Close();
    
    // Check if this is a valid Title Update package
    bool IsTitleUpdate() const;
    
    // Get metadata
    TitleUpdateInfo GetTitleUpdateInfo() const;
    uint32_t GetTitleId() const { return m_titleId; }
    uint32_t GetVersion() const { return m_version; }
    StfsPackageType GetPackageType() const { return m_packageType; }
    
    // List all files in the package
    std::vector<StfsFileEntry> GetFileList() const;
    
    // Extract a file by name
    bool ExtractFile(const std::string& fileName, std::vector<uint8_t>& outData);
    
    // Extract a file to disk
    bool ExtractFileToDisk(const std::string& fileName, const std::filesystem::path& outPath);

private:
    static const uint32_t kBlockSize = 0x1000;
    static const uint32_t kHashesPerBlock = 170;
    
    // Read helpers
    uint32_t ReadUInt32BE(uint64_t offset);
    uint16_t ReadUInt16BE(uint64_t offset);
    uint32_t ReadUInt24LE(uint64_t offset);
    void ReadBytes(uint64_t offset, void* buffer, size_t size);
    
    // Block navigation
    uint64_t BlockToOffset(uint32_t blockNum);
    uint32_t GetNextBlock(uint32_t blockNum);
    uint32_t ComputeHashTableBlockNumber(uint32_t blockNum);
    
    // Parse functions
    bool ParseHeader();
    bool ParseFileTable();
    
    std::ifstream m_file;
    std::filesystem::path m_path;
    bool m_isOpen;
    
    // Package info
    StfsPackageType m_packageType;
    XContentType m_contentType;
    uint32_t m_titleId;
    uint32_t m_version;
    uint32_t m_baseVersion;
    std::string m_displayName;
    
    // Volume descriptor
    uint32_t m_headerSize;
    uint32_t m_fileTableBlockNum;
    uint32_t m_fileTableBlockCount;
    uint32_t m_totalBlockCount;
    bool m_readOnlyFormat;
    
    // File entries
    std::vector<StfsFileEntry> m_files;
};

} // namespace install
} // namespace liberty
