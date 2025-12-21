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
    , m_packageType(StfsPackageType::kCon)
    , m_contentType(XContentType::kSavedGame)
    , m_titleId(0)
    , m_version(0)
    , m_baseVersion(0)
    , m_headerSize(0)
    , m_fileTableBlockNum(0)
    , m_fileTableBlockCount(0)
    , m_totalBlockCount(0)
    , m_readOnlyFormat(false)
{
}

StfsParser::~StfsParser()
{
    Close();
}

bool StfsParser::Open(const std::filesystem::path& path)
{
    Close();
    
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
    m_files.clear();
}

bool StfsParser::IsTitleUpdate() const
{
    return m_contentType == XContentType::kInstaller;
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
    uint8_t buf[4];
    m_file.seekg(offset);
    m_file.read(reinterpret_cast<char*>(buf), 4);
    return (uint32_t(buf[0]) << 24) | (uint32_t(buf[1]) << 16) |
           (uint32_t(buf[2]) << 8) | uint32_t(buf[3]);
}

uint16_t StfsParser::ReadUInt16BE(uint64_t offset)
{
    uint8_t buf[2];
    m_file.seekg(offset);
    m_file.read(reinterpret_cast<char*>(buf), 2);
    return (uint16_t(buf[0]) << 8) | uint16_t(buf[1]);
}

uint32_t StfsParser::ReadUInt24LE(uint64_t offset)
{
    uint8_t buf[3];
    m_file.seekg(offset);
    m_file.read(reinterpret_cast<char*>(buf), 3);
    return (uint32_t(buf[2]) << 16) | (uint32_t(buf[1]) << 8) | uint32_t(buf[0]);
}

void StfsParser::ReadBytes(uint64_t offset, void* buffer, size_t size)
{
    m_file.seekg(offset);
    m_file.read(reinterpret_cast<char*>(buffer), size);
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
        return false; // Invalid magic
    }
    
    // Header size depends on package type
    // CON: 0xB000 for female, 0xA000 for male
    // LIVE/PIRS: 0xB000 for female, 0xA000 for male
    // Read the header size from the metadata
    
    // Metadata starts at different offsets depending on signature type
    uint64_t metadataOffset;
    if (m_packageType == StfsPackageType::kCon) {
        metadataOffset = 0x22C; // After CON signature
    } else {
        metadataOffset = 0x22C; // Same for LIVE/PIRS
    }
    
    // Read content type at metadata + 0x00
    m_contentType = static_cast<XContentType>(ReadUInt32BE(metadataOffset));
    
    // Read metadata version at metadata + 0x04
    uint32_t metadataVersion = ReadUInt32BE(metadataOffset + 0x04);
    
    // Read execution info (title ID, version) at metadata + 0x10
    // Execution info structure:
    // +0x00: Media ID (4 bytes)
    // +0x04: Version (4 bytes)
    // +0x08: Base Version (4 bytes)
    // +0x0C: Title ID (4 bytes)
    uint64_t execInfoOffset = metadataOffset + 0x10;
    m_version = ReadUInt32BE(execInfoOffset + 0x04);
    m_baseVersion = ReadUInt32BE(execInfoOffset + 0x08);
    m_titleId = ReadUInt32BE(execInfoOffset + 0x0C);
    
    // Volume descriptor at metadata + 0x24
    uint64_t volumeDescOffset = metadataOffset + 0x24;
    
    // Read volume descriptor
    uint8_t descriptorLength;
    ReadBytes(volumeDescOffset, &descriptorLength, 1);
    
    if (descriptorLength != 0x24) {
        // Not a valid STFS volume descriptor
        return false;
    }
    
    // Read flags byte
    uint8_t flags;
    ReadBytes(volumeDescOffset + 2, &flags, 1);
    m_readOnlyFormat = (flags & 0x01) != 0;
    
    // File table block count (2 bytes, little endian at offset +3)
    uint8_t ftBlockCountBytes[2];
    ReadBytes(volumeDescOffset + 3, ftBlockCountBytes, 2);
    m_fileTableBlockCount = ftBlockCountBytes[0] | (uint16_t(ftBlockCountBytes[1]) << 8);
    
    // File table block number (3 bytes, little endian at offset +5)
    m_fileTableBlockNum = ReadUInt24LE(volumeDescOffset + 5);
    
    // Total block count (4 bytes BE at offset +0x1C)
    m_totalBlockCount = ReadUInt32BE(volumeDescOffset + 0x1C);
    
    // Determine header size based on read-only format
    m_headerSize = m_readOnlyFormat ? 0xA000 : 0xB000;
    
    // Read display name (at metadata + 0x1691 for first language, UTF-16BE)
    uint64_t displayNameOffset = metadataOffset + 0x1691;
    char16_t nameBuffer[128];
    ReadBytes(displayNameOffset, nameBuffer, 256);
    
    // Convert UTF-16BE to string
    m_displayName.clear();
    for (int i = 0; i < 128 && nameBuffer[i] != 0; i++) {
        // Byte swap for BE
        uint16_t ch = (uint16_t(reinterpret_cast<uint8_t*>(&nameBuffer[i])[0]) << 8) |
                      uint16_t(reinterpret_cast<uint8_t*>(&nameBuffer[i])[1]);
        if (ch == 0) break;
        if (ch < 128) {
            m_displayName += static_cast<char>(ch);
        }
    }
    
    return true;
}

uint64_t StfsParser::BlockToOffset(uint32_t blockNum)
{
    // Calculate the offset for a given block number
    // This accounts for hash tables interspersed with data blocks
    
    uint64_t baseOffset = m_headerSize;
    
    if (m_readOnlyFormat) {
        // Read-only: single hash table backing
        uint32_t hashBlockNum = blockNum / kHashesPerBlock;
        uint32_t hashBlockOffset = (hashBlockNum + 1) * kBlockSize; // +1 for each hash table
        return baseOffset + (blockNum * kBlockSize) + hashBlockOffset;
    } else {
        // Read-write: dual hash table backing
        uint32_t hashBlockNum = blockNum / kHashesPerBlock;
        uint32_t hashBlockOffset = (hashBlockNum + 1) * 2 * kBlockSize; // +2 for each hash table pair
        return baseOffset + (blockNum * kBlockSize) + hashBlockOffset;
    }
}

uint32_t StfsParser::GetNextBlock(uint32_t blockNum)
{
    // Read the next block number from the hash table
    uint32_t hashTableBlock = ComputeHashTableBlockNumber(blockNum);
    uint32_t entryIndex = blockNum % kHashesPerBlock;
    
    uint64_t hashTableOffset = BlockToOffset(hashTableBlock) - kBlockSize;
    uint64_t entryOffset = hashTableOffset + (entryIndex * 0x18) + 0x14;
    
    // Read the info field (4 bytes BE)
    uint32_t info = ReadUInt32BE(entryOffset);
    
    // Extract next block (lower 24 bits)
    return info & 0xFFFFFF;
}

uint32_t StfsParser::ComputeHashTableBlockNumber(uint32_t blockNum)
{
    // Compute which hash table block contains the entry for this data block
    return (blockNum / kHashesPerBlock) * kHashesPerBlock;
}

bool StfsParser::ParseFileTable()
{
    m_files.clear();
    
    if (m_fileTableBlockCount == 0) {
        return true; // Empty file table is valid
    }
    
    uint32_t currentBlock = m_fileTableBlockNum;
    
    for (uint32_t tableIdx = 0; tableIdx < m_fileTableBlockCount; tableIdx++) {
        uint64_t blockOffset = BlockToOffset(currentBlock);
        
        // Each block has 64 directory entries (0x40 bytes each)
        for (uint32_t entryIdx = 0; entryIdx < 64; entryIdx++) {
            uint64_t entryOffset = blockOffset + (entryIdx * 0x40);
            
            // Read entry
            uint8_t entryData[0x40];
            ReadBytes(entryOffset, entryData, 0x40);
            
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
            
            // File size (4 bytes BE at offset 0x34)
            entry.size = (uint32_t(entryData[0x34]) << 24) | (uint32_t(entryData[0x35]) << 16) |
                         (uint32_t(entryData[0x36]) << 8) | uint32_t(entryData[0x37]);
            
            m_files.push_back(entry);
        }
        
        // Get next file table block
        if (tableIdx + 1 < m_fileTableBlockCount) {
            currentBlock = GetNextBlock(currentBlock);
            if (currentBlock == 0xFFFFFF) {
                break; // End of chain
            }
        }
    }
    
    return true;
}

bool StfsParser::ExtractFile(const std::string& fileName, std::vector<uint8_t>& outData)
{
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
    
    while (bytesRemaining > 0 && currentBlock != 0xFFFFFF) {
        uint64_t blockOffset = BlockToOffset(currentBlock);
        uint32_t bytesToRead = std::min(bytesRemaining, kBlockSize);
        
        ReadBytes(blockOffset, outData.data() + outOffset, bytesToRead);
        
        outOffset += bytesToRead;
        bytesRemaining -= bytesToRead;
        
        if (entry.isContiguous) {
            currentBlock++;
        } else {
            currentBlock = GetNextBlock(currentBlock);
        }
    }
    
    return bytesRemaining == 0;
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
