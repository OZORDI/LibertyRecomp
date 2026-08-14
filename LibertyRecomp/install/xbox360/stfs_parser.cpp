/**
 * STFS Parser for Xbox 360 Title Update files
 * Adapted from Xenia (BSD-3-Clause license)
 * Copyright 2020 Ben Vanik. All rights reserved.
 */

#include "stfs_parser.h"
#include <cstring>
#include <algorithm>

namespace liberty {
namespace install {

StfsParser::StfsParser()
    : m_isOpen(false)
    , m_readFailed(false)
    , m_packageType(StfsPackageType::kCon)
    , m_contentType(XContentType::kSavedGame)
    , m_titleId(0)
    , m_mediaId(0)
    , m_version(0)
    , m_baseVersion(0)
    , m_headerSize(0)
    , m_fileTableBlockNum(0)
    , m_fileTableBlockCount(0)
    , m_totalBlockCount(0)
    , m_readOnlyFormat(false)
    , m_rootActiveIndex(false)
{
}

StfsParser::~StfsParser()
{
    Close();
}

bool StfsParser::Open(const std::filesystem::path& path)
{
    Close();
    m_readFailed = false;
    
    m_path = path;
    m_file.open(path, std::ios::binary);
    
    if (!m_file.is_open()) {
        return false;
    }
    
    m_isOpen = true;
    
    if (!ParseHeader()) {
        Close();
        return false;
    }
    
    if (!ParseFileTable()) {
        Close();
        return false;
    }
    
    return true;
}

void StfsParser::Close()
{
    if (m_file.is_open()) {
        m_file.close();
    }
    m_isOpen = false;
    m_readFailed = false;
    m_files.clear();
}

bool StfsParser::IsTitleUpdate() const
{
    return m_contentType == XContentType::kInstaller;
}

bool StfsParser::IsDLC() const
{
    return m_contentType == XContentType::kMarketplaceContent;
}

TitleUpdateInfo StfsParser::GetTitleUpdateInfo() const
{
    TitleUpdateInfo info;
    info.titleId = m_titleId;
    info.version = m_version;
    info.baseVersion = m_baseVersion;
    info.displayName = m_displayName;
    info.contentType = m_contentType;
    info.isValid = m_isOpen && IsTitleUpdate();
    return info;
}

std::vector<StfsFileEntry> StfsParser::GetFileList() const
{
    return m_files;
}

uint32_t StfsParser::ReadUInt32BE(uint64_t offset)
{
    uint8_t buf[4]{};
    ReadBytes(offset, buf, sizeof(buf));
    return (uint32_t(buf[0]) << 24) | (uint32_t(buf[1]) << 16) |
           (uint32_t(buf[2]) << 8) | uint32_t(buf[3]);
}

uint16_t StfsParser::ReadUInt16BE(uint64_t offset)
{
    uint8_t buf[2]{};
    ReadBytes(offset, buf, sizeof(buf));
    return (uint16_t(buf[0]) << 8) | uint16_t(buf[1]);
}

uint32_t StfsParser::ReadUInt24LE(uint64_t offset)
{
    uint8_t buf[3]{};
    ReadBytes(offset, buf, sizeof(buf));
    return (uint32_t(buf[2]) << 16) | (uint32_t(buf[1]) << 8) | uint32_t(buf[0]);
}

void StfsParser::ReadBytes(uint64_t offset, void* buffer, size_t size)
{
    if (m_readFailed) {
        std::memset(buffer, 0, size);
        return;
    }

    m_file.clear();
    m_file.seekg(offset);
    if (!m_file.good()) {
        std::memset(buffer, 0, size);
        m_readFailed = true;
        return;
    }

    m_file.read(reinterpret_cast<char*>(buffer), size);
    if (m_file.gcount() != static_cast<std::streamsize>(size)) {
        std::memset(buffer, 0, size);
        m_readFailed = true;
    }
}

bool StfsParser::ParseHeader()
{
    // Read magic (first 4 bytes)
    uint32_t magic = ReadUInt32BE(0);

    if (magic == static_cast<uint32_t>(StfsPackageType::kCon)) {
        m_packageType = StfsPackageType::kCon;
    } else if (magic == static_cast<uint32_t>(StfsPackageType::kPirs)) {
        m_packageType = StfsPackageType::kPirs;
    } else if (magic == static_cast<uint32_t>(StfsPackageType::kLive)) {
        m_packageType = StfsPackageType::kLive;
    } else {
        return false;
    }

    // XContentHeader is always 0x344 bytes for ALL package types (CON/LIVE/PIRS).
    // Layout (from Xenia stfs_xbox.h):
    //   0x000: magic (4)
    //   0x004: signature (0x228)
    //   0x22C: licenses[16] (0x100)
    //   0x32C: content_id (0x14)
    //   0x340: header_size (4)
    // Total: 0x344
    //
    // XContentMetadata starts immediately after at offset 0x344.
    // Verified with Python against actual STFS files.
    constexpr uint64_t kMetadataOffset = 0x344;

    // content_type at metadata + 0x00
    m_contentType = static_cast<XContentType>(ReadUInt32BE(kMetadataOffset));

    // metadata_version at metadata + 0x04
    uint32_t metadataVersion = ReadUInt32BE(kMetadataOffset + 0x04);

    // execution_info at metadata + 0x10 (xex2_opt_execution_info, 0x18 bytes)
    //   +0x00: media_id (4 BE)
    //   +0x04: version (4 BE)
    //   +0x08: base_version (4 BE)
    //   +0x0C: title_id (4 BE)
    constexpr uint64_t kExecInfoOffset = kMetadataOffset + 0x10;
    m_mediaId     = ReadUInt32BE(kExecInfoOffset + 0x00);
    m_version     = ReadUInt32BE(kExecInfoOffset + 0x04);
    m_baseVersion = ReadUInt32BE(kExecInfoOffset + 0x08);
    m_titleId     = ReadUInt32BE(kExecInfoOffset + 0x0C);

    // StfsVolumeDescriptor at metadata + 0x35 (0x24 bytes)
    // Layout within XContentMetadata:
    //   +0x00: content_type (4)
    //   +0x04: metadata_version (4)
    //   +0x08: content_size (8)
    //   +0x10: execution_info (0x18)
    //   +0x28: console_id (5)
    //   +0x2D: profile_id (8)
    //   +0x35: volume_descriptor (0x24) ← here
    // Verified: descriptor_length at file offset 0x379 reads 0x24 ✓
    constexpr uint64_t kVolumeDescOffset = kMetadataOffset + 0x35;

    uint8_t descriptorLength;
    ReadBytes(kVolumeDescOffset, &descriptorLength, 1);
    if (descriptorLength != 0x24) {
        return false;
    }

    // flags at +2 within volume descriptor
    uint8_t flags;
    ReadBytes(kVolumeDescOffset + 2, &flags, 1);
    m_readOnlyFormat = (flags & 0x01) != 0;
    m_rootActiveIndex = (flags & 0x02) != 0;

    // file_table_block_count at +3 (2 bytes LE)
    uint8_t ftBlockCountBytes[2];
    ReadBytes(kVolumeDescOffset + 3, ftBlockCountBytes, 2);
    m_fileTableBlockCount = ftBlockCountBytes[0] | (uint16_t(ftBlockCountBytes[1]) << 8);

    // file_table_block_number at +5 (3 bytes LE)
    m_fileTableBlockNum = ReadUInt24LE(kVolumeDescOffset + 5);

    // total_block_count at +0x1C (4 bytes BE)
    m_totalBlockCount = ReadUInt32BE(kVolumeDescOffset + 0x1C);

    // header_size from XContentHeader at offset 0x340
    m_headerSize = ReadUInt32BE(0x340);
    // Round up to 0x1000 boundary (Xenia does this)
    m_headerSize = (m_headerSize + 0xFFF) & ~0xFFFu;
    if (m_headerSize == 0) {
        // Fallback based on read-only format
        m_headerSize = m_readOnlyFormat ? 0xA000 : 0xB000;
    }

    // display_name at metadata + 0xCD (first language slot, UTF-16BE, 128 chars)
    // Layout: after device_id (0x14) at metadata + 0xB9, display_name_raw starts at +0xCD
    constexpr uint64_t kDisplayNameOffset = kMetadataOffset + 0xCD;
    char16_t nameBuffer[128];
    ReadBytes(kDisplayNameOffset, nameBuffer, 256);

    m_displayName.clear();
    for (int i = 0; i < 128; i++) {
        uint16_t ch = (uint16_t(reinterpret_cast<uint8_t*>(&nameBuffer[i])[0]) << 8) |
                      uint16_t(reinterpret_cast<uint8_t*>(&nameBuffer[i])[1]);
        if (ch == 0) break;
        if (ch < 128) {
            m_displayName += static_cast<char>(ch);
        }
    }

    return !m_readFailed;
}

uint64_t StfsParser::BlockToOffset(uint32_t blockNum) const
{
    // STFS inserts a hash table at every level of the 170-way block tree.
    // Read/write packages keep two copies of every table; LIVE/PIRS packages
    // normally use the read-only layout with a single copy.
    const uint64_t blocksPerHashTable = m_readOnlyFormat ? 1 : 2;
    uint64_t levelBase = kHashesPerBlock;
    uint64_t physicalBlock = blockNum;

    for (uint32_t hashLevel = 0; hashLevel < 3; hashLevel++) {
        physicalBlock += ((uint64_t(blockNum) + levelBase) / levelBase) *
                         blocksPerHashTable;
        if (blockNum < levelBase) {
            break;
        }

        levelBase *= kHashesPerBlock;
    }

    return uint64_t(m_headerSize) + (physicalBlock * kBlockSize);
}

uint32_t StfsParser::BlockToHashBlockNumber(uint32_t blockNum,
                                            uint32_t hashLevel) const
{
    const uint32_t blocksPerHashTable = m_readOnlyFormat ? 1 : 2;
    const uint32_t blocksPerLevel[] = {
        kHashesPerBlock,
        kHashesPerBlock * kHashesPerBlock,
        kHashesPerBlock * kHashesPerBlock * kHashesPerBlock,
    };
    const uint32_t blockStep[] = {
        blocksPerLevel[0] + blocksPerHashTable,
        blocksPerLevel[1] + ((blocksPerLevel[0] + 1) * blocksPerHashTable),
    };

    if (hashLevel == 2) {
        return blockStep[1];
    }

    if (blockNum < blocksPerLevel[hashLevel]) {
        return hashLevel == 0 ? 0 : blockStep[hashLevel - 1];
    }

    uint32_t block =
        (blockNum / blocksPerLevel[hashLevel]) * blockStep[hashLevel];
    if (hashLevel == 0) {
        block += ((blockNum / blocksPerLevel[1]) + 1) * blocksPerHashTable;
        if (blockNum < blocksPerLevel[1]) {
            return block;
        }
    }

    return block + blocksPerHashTable;
}

uint64_t StfsParser::BlockToHashBlockOffset(uint32_t blockNum,
                                            uint32_t hashLevel) const
{
    return uint64_t(m_headerSize) +
           (uint64_t(BlockToHashBlockNumber(blockNum, hashLevel)) * kBlockSize);
}

uint32_t StfsParser::GetNextBlock(uint32_t blockNum)
{
    const uint32_t blocksPerLevel[] = {
        kHashesPerBlock,
        kHashesPerBlock * kHashesPerBlock,
        kHashesPerBlock * kHashesPerBlock * kHashesPerBlock,
    };

    uint32_t highestHashLevel = 0;
    while (highestHashLevel < 2 &&
           m_totalBlockCount >= blocksPerLevel[highestHashLevel]) {
        highestHashLevel++;
    }

    uint32_t secondaryTableOffset = m_rootActiveIndex ? kBlockSize : 0;
    if (m_readOnlyFormat) {
        // Read-only packages contain only the primary hash-table copy.
        highestHashLevel = 0;
        secondaryTableOffset = 0;
    }

    uint32_t info = 0;
    for (int32_t hashLevel = int32_t(highestHashLevel); hashLevel >= 0;
         hashLevel--) {
        uint32_t entryIndex = blockNum % kHashesPerBlock;
        if (hashLevel > 0) {
            entryIndex = (blockNum / blocksPerLevel[hashLevel - 1]) %
                         kHashesPerBlock;
        }

        const uint64_t entryOffset =
            BlockToHashBlockOffset(blockNum, uint32_t(hashLevel)) +
            secondaryTableOffset + (uint64_t(entryIndex) * 0x18) + 0x14;
        info = ReadUInt32BE(entryOffset);
        secondaryTableOffset = (info & 0x40000000) ? kBlockSize : 0;
    }

    return info & kEndOfChain;
}

bool StfsParser::ParseFileTable()
{
    m_files.clear();
    
    if (m_fileTableBlockCount == 0) {
        return true; // Empty file table is valid
    }
    
    uint32_t currentBlock = m_fileTableBlockNum;
    uint32_t tableBlocksRead = 0;
    
    for (uint32_t tableIdx = 0; tableIdx < m_fileTableBlockCount; tableIdx++) {
        if (currentBlock == kEndOfChain) {
            return false;
        }

        uint64_t blockOffset = BlockToOffset(currentBlock);
        tableBlocksRead++;
        
        // Each block has 64 directory entries (0x40 bytes each)
        for (uint32_t entryIdx = 0; entryIdx < 64; entryIdx++) {
            uint64_t entryOffset = blockOffset + (entryIdx * 0x40);
            
            // Read entry
            uint8_t entryData[0x40];
            ReadBytes(entryOffset, entryData, 0x40);
            if (m_readFailed) {
                return false;
            }
            
            // Check if entry is valid (name length > 0)
            uint8_t flags = entryData[0x28];
            uint8_t nameLength = flags & 0x3F;
            
            if (nameLength == 0) {
                continue; // Empty entry
            }
            
            StfsFileEntry entry;
            
            // Extract name
            entry.name = std::string(reinterpret_cast<char*>(entryData), nameLength);
            
            // Flags
            entry.isContiguous = (flags & 0x40) != 0;
            entry.isDirectory = (flags & 0x80) != 0;
            
            // Start block (3 bytes LE at offset 0x2F)
            entry.startBlock = entryData[0x2F] | (uint32_t(entryData[0x30]) << 8) | 
                               (uint32_t(entryData[0x31]) << 16);

            // Allocated block count (3 bytes LE at offset 0x2C)
            entry.allocatedBlockCount = entryData[0x2C] |
                                        (uint32_t(entryData[0x2D]) << 8) |
                                        (uint32_t(entryData[0x2E]) << 16);
            
            // File size (4 bytes BE at offset 0x34)
            entry.size = (uint32_t(entryData[0x34]) << 24) | (uint32_t(entryData[0x35]) << 16) |
                         (uint32_t(entryData[0x36]) << 8) | uint32_t(entryData[0x37]);
            
            m_files.push_back(entry);
        }
        
        // Get next file table block
        if (tableIdx + 1 < m_fileTableBlockCount) {
            currentBlock = GetNextBlock(currentBlock);
            if (currentBlock == kEndOfChain) {
                return false;
            }
        }
    }
    
    return !m_readFailed && tableBlocksRead == m_fileTableBlockCount;
}

bool StfsParser::ExtractFile(const std::string& fileName, std::vector<uint8_t>& outData)
{
    m_readFailed = false;
    // Find the file
    auto it = std::find_if(m_files.begin(), m_files.end(),
        [&fileName](const StfsFileEntry& e) { return e.name == fileName; });
    
    if (it == m_files.end()) {
        return false;
    }
    
    const StfsFileEntry& entry = *it;
    
    if (entry.isDirectory) {
        return false; // Can't extract a directory
    }
    
    outData.resize(entry.size);
    
    uint32_t bytesRemaining = entry.size;
    uint32_t currentBlock = entry.startBlock;
    uint32_t outOffset = 0;
    uint32_t blocksRead = 0;
    
    while (bytesRemaining > 0 && currentBlock != kEndOfChain) {
        uint64_t blockOffset = BlockToOffset(currentBlock);
        uint32_t bytesToRead = std::min(bytesRemaining, kBlockSize);
        
        ReadBytes(blockOffset, outData.data() + outOffset, bytesToRead);
        if (m_readFailed) {
            outData.clear();
            return false;
        }
        
        outOffset += bytesToRead;
        bytesRemaining -= bytesToRead;
        blocksRead++;

        // Xenia walks the hash chain even when the contiguous hint is set.
        // This also validates the package metadata instead of assuming that
        // physical blocks remain adjacent across hash-table boundaries.
        if (bytesRemaining > 0) {
            currentBlock = GetNextBlock(currentBlock);
        }
    }
    
    const bool complete = !m_readFailed && bytesRemaining == 0 &&
                          blocksRead == entry.allocatedBlockCount;
    if (!complete) {
        outData.clear();
    }
    return complete;
}

bool StfsParser::ExtractFileToDisk(const std::string& fileName, const std::filesystem::path& outPath)
{
    std::vector<uint8_t> data;
    if (!ExtractFile(fileName, data)) {
        return false;
    }
    
    std::ofstream outFile(outPath, std::ios::binary);
    if (!outFile.is_open()) {
        return false;
    }
    
    outFile.write(reinterpret_cast<const char*>(data.data()), data.size());
    return outFile.good();
}

} // namespace install
} // namespace liberty
