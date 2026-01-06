#include "img_loader.h"
#include <os/logger.h>
#include <fstream>
#include <algorithm>
#include <cstring>

#if defined(_WIN32)
#include <windows.h>
#include <bcrypt.h>
#else
#include <CommonCrypto/CommonCrypto.h>
#endif

namespace ImgLoader
{
    static bool g_initialized = false;
    static std::mutex g_mutex;
    static std::unordered_map<std::string, std::shared_ptr<VirtualImg>> g_imgCache;
    static Stats g_stats;

    // Helper: Align size to block boundaries
    static size_t AlignToBlocks(size_t size)
    {
        return ((size + IMG_BLOCK_SIZE - 1) / IMG_BLOCK_SIZE) * IMG_BLOCK_SIZE;
    }

    // Helper: Check if file is RSC resource
    static bool IsResourceFile(const std::vector<uint8_t>& data)
    {
        if (data.size() < sizeof(RscHeader))
            return false;
        const RscHeader* header = reinterpret_cast<const RscHeader*>(data.data());
        return header->magic == RscHeader::MAGIC;
    }

    // Helper: Get RSC info from file data
    static std::pair<uint32_t, uint32_t> GetRscInfo(const std::vector<uint8_t>& data)
    {
        if (data.size() < sizeof(RscHeader))
            return {0, 0};
        const RscHeader* header = reinterpret_cast<const RscHeader*>(data.data());
        if (header->magic != RscHeader::MAGIC)
            return {0, 0};
        return {header->flags, header->type};
    }

    // Helper: Get actual file size from IMG entry
    static uint32_t GetActualFileSize(const ImgEntryV3& entry)
    {
        bool isResource = ((entry.sizeOrFlags & 0xC0000000) != 0) || 
                          ((entry.sizeOrFlags & 0x80000000) == 0x80000000);
        if (!isResource)
        {
            return entry.sizeOrFlags;
        }
        else
        {
            int paddingCount = entry.flags & 0x7FF;
            return entry.usedBlocks * IMG_BLOCK_SIZE - paddingCount;
        }
    }

    void Initialize()
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        if (g_initialized)
            return;
        
        g_initialized = true;
        g_imgCache.clear();
        g_stats = Stats{};
        
        LOGF_UTILITY("[ImgLoader] Initialized");
    }

    bool IsInitialized()
    {
        return g_initialized;
    }

    std::optional<std::vector<FileEntry>> ExtractImg(const std::filesystem::path& imgPath)
    {
        std::error_code ec;
        if (!std::filesystem::exists(imgPath, ec) || !std::filesystem::is_regular_file(imgPath, ec))
        {
            return std::nullopt;
        }

        // Read file into memory
        std::ifstream file(imgPath, std::ios::binary | std::ios::ate);
        if (!file.is_open())
        {
            return std::nullopt;
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        file.seekg(0, std::ios::beg);

        std::vector<uint8_t> imgData(fileSize);
        if (!file.read(reinterpret_cast<char*>(imgData.data()), fileSize))
        {
            return std::nullopt;
        }

        // Check for GTA IV format
        if (fileSize < sizeof(ImgHeaderV3))
        {
            return std::nullopt;
        }

        const ImgHeaderV3* header = reinterpret_cast<const ImgHeaderV3*>(imgData.data());
        if (header->magic != GTAIV_MAGIC || header->version != 3 || header->itemSize != 16)
        {
            LOGF_WARNING("[ImgLoader] Invalid IMG header: magic=0x{:08X} version={}", 
                header->magic, header->version);
            return std::nullopt;
        }

        // Parse entries
        size_t entriesOffset = sizeof(ImgHeaderV3);
        size_t entriesSize = header->numItems * sizeof(ImgEntryV3);

        if (imgData.size() < entriesOffset + entriesSize)
        {
            return std::nullopt;
        }

        const ImgEntryV3* entries = reinterpret_cast<const ImgEntryV3*>(imgData.data() + entriesOffset);

        // Parse filename table
        size_t filenameTableOffset = entriesOffset + entriesSize;
        size_t filenameTableSize = header->tableSize - (header->numItems * header->itemSize);

        if (filenameTableOffset + filenameTableSize > imgData.size())
        {
            return std::nullopt;
        }

        std::vector<std::string> filenames;
        size_t currentOffset = filenameTableOffset;

        for (uint32_t i = 0; i < header->numItems; ++i)
        {
            if (currentOffset >= imgData.size())
                break;

            const char* nameStart = reinterpret_cast<const char*>(imgData.data() + currentOffset);
            size_t maxNameLen = imgData.size() - currentOffset;
            size_t nameLen = strnlen(nameStart, maxNameLen);

            filenames.emplace_back(nameStart, nameLen);
            currentOffset += nameLen + 1;
        }

        // Extract files
        std::vector<FileEntry> files;
        for (uint32_t i = 0; i < header->numItems && i < filenames.size(); ++i)
        {
            const ImgEntryV3& entry = entries[i];
            uint32_t actualSize = GetActualFileSize(entry);
            uint32_t dataOffset = entry.offsetBlock * IMG_BLOCK_SIZE;

            if (dataOffset + actualSize > imgData.size())
            {
                continue;
            }

            FileEntry fileEntry;
            fileEntry.name = filenames[i];
            fileEntry.data.assign(imgData.begin() + dataOffset, imgData.begin() + dataOffset + actualSize);
            fileEntry.resourceType = entry.resourceType;
            files.push_back(std::move(fileEntry));
        }

        g_stats.imgsLoaded++;
        return files;
    }

    std::vector<FileEntry> GetReplacementFiles(const std::filesystem::path& updateFolder)
    {
        std::vector<FileEntry> replacements;
        std::error_code ec;

        if (!std::filesystem::exists(updateFolder, ec) || !std::filesystem::is_directory(updateFolder, ec))
        {
            return replacements;
        }

        constexpr auto perms = std::filesystem::directory_options::skip_permission_denied |
                               std::filesystem::directory_options::follow_directory_symlink;

        for (const auto& entry : std::filesystem::recursive_directory_iterator(updateFolder, perms, ec))
        {
            if (ec) continue;
            if (!entry.is_regular_file(ec)) continue;

            std::ifstream file(entry.path(), std::ios::binary | std::ios::ate);
            if (!file.is_open()) continue;

            auto size = file.tellg();
            if (size > static_cast<std::streamoff>(MAX_FILESIZE)) continue;

            file.seekg(0, std::ios::beg);
            std::vector<uint8_t> fileData(static_cast<size_t>(size));
            if (!file.read(reinterpret_cast<char*>(fileData.data()), size)) continue;

            FileEntry replacement;
            replacement.name = entry.path().filename().string();
            replacement.data = std::move(fileData);
            replacement.isReplacement = true;

            replacements.push_back(std::move(replacement));
        }

        return replacements;
    }

    std::vector<FileEntry> MergeFiles(
        const std::vector<FileEntry>& originalFiles,
        const std::vector<FileEntry>& replacementFiles)
    {
        // Build replacement lookup (case-insensitive)
        std::unordered_map<std::string, const FileEntry*> replacementMap;
        for (const auto& file : replacementFiles)
        {
            std::string lowerName = file.name;
            std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
            replacementMap[lowerName] = &file;
        }

        std::vector<FileEntry> merged;
        std::unordered_set<std::string> processed;

        // Process originals (replace if needed)
        for (const auto& original : originalFiles)
        {
            std::string lowerName = original.name;
            std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

            auto it = replacementMap.find(lowerName);
            if (it != replacementMap.end())
            {
                // Use replacement
                FileEntry mergedFile = *it->second;
                mergedFile.name = original.name; // Keep original casing
                merged.push_back(std::move(mergedFile));
                processed.insert(lowerName);
                g_stats.filesReplaced++;
            }
            else
            {
                // Keep original
                merged.push_back(original);
            }
        }

        // Add new files
        for (const auto& replacement : replacementFiles)
        {
            std::string lowerName = replacement.name;
            std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

            if (processed.find(lowerName) == processed.end())
            {
                merged.push_back(replacement);
                g_stats.filesAdded++;
            }
        }

        return merged;
    }

    std::shared_ptr<std::vector<uint8_t>> CreateImg(
        std::vector<FileEntry>& files,
        ImgVersion version)
    {
        if (files.empty())
            return nullptr;

        // Calculate filename table size
        size_t filenameTableSize = 0;
        for (const auto& file : files)
        {
            filenameTableSize += file.name.length() + 1;
        }

        // Calculate TOC size
        size_t baseTocSize = sizeof(ImgHeaderV3) + (files.size() * sizeof(ImgEntryV3)) + filenameTableSize;
        size_t tocWithPadding = baseTocSize + 32;
        size_t alignedTocSize = AlignToBlocks(tocWithPadding);

        // Calculate file positions and total size
        uint32_t currentPosition = static_cast<uint32_t>(alignedTocSize / IMG_BLOCK_SIZE);
        size_t totalSize = alignedTocSize;

        for (auto& file : files)
        {
            file.position = currentPosition;
            size_t alignedFileSize = AlignToBlocks(file.data.size());
            currentPosition += static_cast<uint32_t>(alignedFileSize / IMG_BLOCK_SIZE);
            totalSize += alignedFileSize;
        }

        // Create IMG data
        auto imgData = std::make_shared<std::vector<uint8_t>>(totalSize, 0);
        uint8_t* data = imgData->data();
        size_t offset = 0;

        // Write header
        ImgHeaderV3 header = {};
        header.magic = GTAIV_MAGIC;
        header.version = 3;
        header.numItems = static_cast<uint32_t>(files.size());
        header.tableSize = static_cast<uint32_t>((files.size() * sizeof(ImgEntryV3)) + filenameTableSize);
        header.itemSize = sizeof(ImgEntryV3);
        header.unknown = 0x00E9;

        std::memcpy(data + offset, &header, sizeof(header));
        offset += sizeof(header);

        // Write entries
        for (const auto& file : files)
        {
            ImgEntryV3 entry = {};
            bool isResource = IsResourceFile(file.data);

            if (isResource)
            {
                auto [rscFlags, resourceType] = GetRscInfo(file.data);
                entry.sizeOrFlags = rscFlags;
                entry.resourceType = resourceType;
                size_t alignedSize = AlignToBlocks(file.data.size());
                uint16_t paddingCount = static_cast<uint16_t>(alignedSize - file.data.size());
                entry.flags = (paddingCount & 0x7FF) | 0x2000;
            }
            else
            {
                entry.sizeOrFlags = static_cast<uint32_t>(file.data.size());
                entry.resourceType = 0;
                entry.flags = 0;
            }

            entry.offsetBlock = file.position;
            entry.usedBlocks = static_cast<uint16_t>(AlignToBlocks(file.data.size()) / IMG_BLOCK_SIZE);

            std::memcpy(data + offset, &entry, sizeof(entry));
            offset += sizeof(entry);
        }

        // Write filename table
        for (const auto& file : files)
        {
            std::memcpy(data + offset, file.name.c_str(), file.name.length());
            offset += file.name.length();
            data[offset++] = 0;
        }

        // Skip padding, write file data
        for (const auto& file : files)
        {
            size_t fileOffset = file.position * IMG_BLOCK_SIZE;
            std::memcpy(data + fileOffset, file.data.data(), file.data.size());
        }

        return imgData;
    }

    std::shared_ptr<VirtualImg> MergeImgWithFolder(
        const std::filesystem::path& originalImgPath,
        const std::filesystem::path& updateFolderPath)
    {
        // Extract original
        auto originalFiles = ExtractImg(originalImgPath);
        if (!originalFiles.has_value())
        {
            LOGF_WARNING("[ImgLoader] Failed to extract: {}", originalImgPath.string());
            return nullptr;
        }

        // Get replacements
        auto replacementFiles = GetReplacementFiles(updateFolderPath);
        if (replacementFiles.empty())
        {
            // No replacements, no need for virtual IMG
            return nullptr;
        }

        // Merge
        auto mergedFiles = MergeFiles(originalFiles.value(), replacementFiles);

        // Create virtual IMG
        auto virtualImg = std::make_shared<VirtualImg>();
        virtualImg->originalPath = originalImgPath.string();
        virtualImg->files = mergedFiles;
        virtualImg->data = CreateImg(virtualImg->files, ImgVersion::V3);
        virtualImg->version = ImgVersion::V3;

        g_stats.imgsMerged++;
        LOGF_UTILITY("[ImgLoader] Merged {} with {} replacements ({} files total)",
            originalImgPath.filename().string(), replacementFiles.size(), mergedFiles.size());

        return virtualImg;
    }

    std::filesystem::path FindUpdateFolder(const std::string& imgPath)
    {
        // TODO: Search through registered mod overlay directories
        // For now, check common locations relative to current working directory
        std::vector<std::filesystem::path> searchPaths = {
            std::filesystem::path("update") / imgPath,
            std::filesystem::path("mods/update") / imgPath,
        };

        for (const auto& searchPath : searchPaths)
        {
            std::error_code ec;
            if (std::filesystem::exists(searchPath, ec) && std::filesystem::is_directory(searchPath, ec))
            {
                return searchPath;
            }
        }

        return {};
    }

    std::shared_ptr<VirtualImg> GetVirtualImg(const std::string& imgPath)
    {
        std::lock_guard<std::mutex> lock(g_mutex);

        // Check cache
        auto it = g_imgCache.find(imgPath);
        if (it != g_imgCache.end())
        {
            g_stats.cacheHits++;
            return it->second;
        }

        g_stats.cacheMisses++;

        // Find update folder
        auto updateFolder = FindUpdateFolder(imgPath);
        if (updateFolder.empty())
        {
            return nullptr;
        }

        // TODO: Get original IMG path from VFS
        // For now, return nullptr - integration needed
        return nullptr;
    }

    void ClearCache()
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        g_imgCache.clear();
    }

    Stats GetStats()
    {
        return g_stats;
    }

    void DumpStatus()
    {
        LOGF_UTILITY("[ImgLoader] Stats: loaded={} merged={} replaced={} added={} hits={} misses={}",
            g_stats.imgsLoaded, g_stats.imgsMerged, g_stats.filesReplaced,
            g_stats.filesAdded, g_stats.cacheHits, g_stats.cacheMisses);
    }
}

// ============================================================================
// IMG V3 Encryption Support (AES-256 ECB, 16 rounds)
// ============================================================================

namespace ImgLoader
{
    static std::vector<uint8_t> g_aesKey;

    // AES-256 ECB decryption (16 rounds) - same as RPF
    static bool DecryptAES256_16Rounds(std::vector<uint8_t>& data)
    {
        if (g_aesKey.size() != 32 || data.empty())
            return false;

        size_t dataLen = data.size() & ~0x0F;  // Align to 16 bytes
        if (dataLen == 0)
            return true;

#if defined(_WIN32)
        BCRYPT_ALG_HANDLE hAlg = nullptr;
        BCRYPT_KEY_HANDLE hKey = nullptr;
        
        if (BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_AES_ALGORITHM, nullptr, 0) != 0)
            return false;
        
        if (BCryptSetProperty(hAlg, BCRYPT_CHAINING_MODE, 
            (PUCHAR)BCRYPT_CHAIN_MODE_ECB, sizeof(BCRYPT_CHAIN_MODE_ECB), 0) != 0)
        {
            BCryptCloseAlgorithmProvider(hAlg, 0);
            return false;
        }
        
        if (BCryptGenerateSymmetricKey(hAlg, &hKey, nullptr, 0, 
            (PUCHAR)g_aesKey.data(), (ULONG)g_aesKey.size(), 0) != 0)
        {
            BCryptCloseAlgorithmProvider(hAlg, 0);
            return false;
        }
        
        ULONG cbResult = 0;
        for (int round = 0; round < 16; round++)
        {
            if (BCryptDecrypt(hKey, data.data(), (ULONG)dataLen, nullptr, 
                nullptr, 0, data.data(), (ULONG)dataLen, &cbResult, 0) != 0)
            {
                BCryptDestroyKey(hKey);
                BCryptCloseAlgorithmProvider(hAlg, 0);
                return false;
            }
        }
        
        BCryptDestroyKey(hKey);
        BCryptCloseAlgorithmProvider(hAlg, 0);
        return true;
#else
        // macOS implementation using CommonCrypto
        for (int round = 0; round < 16; round++)
        {
            size_t outLength = 0;
            CCCryptorStatus status = CCCrypt(
                kCCDecrypt, kCCAlgorithmAES, kCCOptionECBMode,
                g_aesKey.data(), kCCKeySizeAES256,
                nullptr, data.data(), dataLen,
                data.data(), dataLen, &outLength);
            
            if (status != kCCSuccess)
                return false;
        }
        return true;
#endif
    }

    void SetAesKey(const std::vector<uint8_t>& key)
    {
        g_aesKey = key;
    }

    bool IsEncrypted(const std::filesystem::path& imgPath)
    {
        std::ifstream file(imgPath, std::ios::binary);
        if (!file)
            return false;

        uint32_t magic;
        file.read(reinterpret_cast<char*>(&magic), sizeof(magic));
        
        // If magic is not valid GTAIV_MAGIC, it's likely encrypted
        return magic != GTAIV_MAGIC;
    }

    bool DecryptImgHeader(std::vector<uint8_t>& data)
    {
        if (g_aesKey.empty())
        {
            LOGF_WARNING("[ImgLoader] Cannot decrypt IMG: no AES key set");
            return false;
        }

        return DecryptAES256_16Rounds(data);
    }

    /**
     * Extract IMG with encryption support.
     */
    std::optional<std::vector<FileEntry>> ExtractImgEncrypted(const std::filesystem::path& imgPath)
    {
        std::error_code ec;
        if (!std::filesystem::exists(imgPath, ec))
            return std::nullopt;

        std::ifstream file(imgPath, std::ios::binary | std::ios::ate);
        if (!file)
            return std::nullopt;

        size_t fileSize = static_cast<size_t>(file.tellg());
        file.seekg(0);

        // Read first 20 bytes for header
        std::vector<uint8_t> headerData(sizeof(ImgHeaderV3));
        file.read(reinterpret_cast<char*>(headerData.data()), sizeof(ImgHeaderV3));

        // Check if encrypted
        const ImgHeaderV3* header = reinterpret_cast<const ImgHeaderV3*>(headerData.data());
        bool encrypted = (header->magic != GTAIV_MAGIC);

        if (encrypted)
        {
            LOGF_UTILITY("[ImgLoader] IMG is encrypted, attempting decryption: {}", 
                imgPath.filename().string());

            // Need to decrypt header + table together
            // Read enough data for header + potential table
            file.seekg(0);
            size_t headerAndTableSize = std::min(fileSize, size_t(0x10000)); // Read up to 64KB
            std::vector<uint8_t> encryptedData(headerAndTableSize);
            file.read(reinterpret_cast<char*>(encryptedData.data()), headerAndTableSize);

            if (!DecryptImgHeader(encryptedData))
            {
                LOGF_WARNING("[ImgLoader] Failed to decrypt IMG: {}", imgPath.string());
                return std::nullopt;
            }

            // Re-parse header after decryption
            header = reinterpret_cast<const ImgHeaderV3*>(encryptedData.data());
            if (header->magic != GTAIV_MAGIC || header->version != 3)
            {
                LOGF_WARNING("[ImgLoader] Invalid decrypted IMG header: {}", imgPath.string());
                return std::nullopt;
            }

            // Parse entries from decrypted data
            size_t entriesOffset = sizeof(ImgHeaderV3);
            size_t entriesSize = header->numItems * sizeof(ImgEntryV3);
            
            if (encryptedData.size() < entriesOffset + entriesSize)
            {
                return std::nullopt;
            }

            const ImgEntryV3* entries = reinterpret_cast<const ImgEntryV3*>(
                encryptedData.data() + entriesOffset);

            // Parse filenames
            size_t filenameTableOffset = entriesOffset + entriesSize;
            std::vector<std::string> filenames;
            size_t currentOffset = filenameTableOffset;
            
            for (uint32_t i = 0; i < header->numItems; ++i)
            {
                if (currentOffset >= encryptedData.size())
                    break;
                const char* nameStart = reinterpret_cast<const char*>(
                    encryptedData.data() + currentOffset);
                size_t maxLen = encryptedData.size() - currentOffset;
                size_t nameLen = strnlen(nameStart, maxLen);
                filenames.emplace_back(nameStart, nameLen);
                currentOffset += nameLen + 1;
            }

            // Extract files (file data is NOT encrypted, only header/table)
            std::vector<FileEntry> files;
            for (uint32_t i = 0; i < header->numItems && i < filenames.size(); ++i)
            {
                const ImgEntryV3& entry = entries[i];
                uint32_t dataOffset = entry.offsetBlock * IMG_BLOCK_SIZE;
                
                // Calculate actual size
                bool isResource = ((entry.sizeOrFlags & 0xC0000000) != 0);
                uint32_t actualSize;
                if (!isResource)
                {
                    actualSize = entry.sizeOrFlags;
                }
                else
                {
                    int paddingCount = entry.flags & 0x7FF;
                    actualSize = entry.usedBlocks * IMG_BLOCK_SIZE - paddingCount;
                }

                if (dataOffset + actualSize > fileSize)
                    continue;

                // Read file data from original file (not encrypted)
                file.seekg(dataOffset);
                FileEntry fileEntry;
                fileEntry.name = filenames[i];
                fileEntry.data.resize(actualSize);
                file.read(reinterpret_cast<char*>(fileEntry.data.data()), actualSize);
                fileEntry.resourceType = entry.resourceType;
                files.push_back(std::move(fileEntry));
            }

            g_stats.imgsLoaded++;
            LOGF_UTILITY("[ImgLoader] Extracted {} files from encrypted IMG: {}", 
                files.size(), imgPath.filename().string());
            return files;
        }

        // Not encrypted - use standard extraction
        return ExtractImg(imgPath);
    }
}
