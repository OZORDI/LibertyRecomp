#include "rpf_loader.h"
#include <os/logger.h>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <zlib.h>

#if defined(_WIN32)
#include <windows.h>
#include <bcrypt.h>
#else
#include <CommonCrypto/CommonCrypto.h>
#endif

namespace RpfLoader
{
    // RPF2 constants
    static constexpr uint32_t RPF2_MAGIC = 0x32465052;  // "RPF2" little-endian
    static constexpr uint32_t TOC_OFFSET = 0x800;
    static constexpr uint32_t BLOCK_SIZE = 0x800;

    static bool g_initialized = false;
    static std::mutex g_mutex;
    static std::vector<uint8_t> g_aesKey;
    static std::unordered_map<std::string, RpfCache> g_rpfCaches;  // RPF path -> cache
    static std::unordered_map<std::string, std::string> g_fileToRpf;  // Normalized path -> RPF path
    static std::unordered_map<std::string, std::vector<uint8_t>> g_extractedCache;  // Cached extractions
    static Stats g_stats;

    // =========================================================================
    // AES-256-ECB decryption (16 rounds)
    // =========================================================================
    
    static bool DecryptAES256(std::vector<uint8_t>& data, const std::vector<uint8_t>& key)
    {
        if (key.size() != 32 || data.empty())
            return false;

        size_t dataLen = data.size() & ~0x0F;
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
            (PUCHAR)key.data(), (ULONG)key.size(), 0) != 0)
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
        for (int round = 0; round < 16; round++)
        {
            size_t outLength = 0;
            CCCryptorStatus status = CCCrypt(
                kCCDecrypt, kCCAlgorithmAES, kCCOptionECBMode,
                key.data(), kCCKeySizeAES256,
                nullptr, data.data(), dataLen,
                data.data(), dataLen, &outLength);
            
            if (status != kCCSuccess)
                return false;
        }
        return true;
#endif
    }

    // =========================================================================
    // Raw deflate decompression
    // =========================================================================
    
    static std::vector<uint8_t> DecompressDeflate(const std::vector<uint8_t>& compressed, size_t expectedSize)
    {
        std::vector<uint8_t> decompressed(expectedSize);
        
        z_stream stream{};
        stream.next_in = const_cast<Bytef*>(compressed.data());
        stream.avail_in = static_cast<uInt>(compressed.size());
        stream.next_out = decompressed.data();
        stream.avail_out = static_cast<uInt>(expectedSize);
        
        if (inflateInit2(&stream, -MAX_WBITS) != Z_OK)
            return {};
        
        int result = inflate(&stream, Z_FINISH);
        inflateEnd(&stream);
        
        if (result != Z_STREAM_END)
            return {};
        
        decompressed.resize(stream.total_out);
        return decompressed;
    }

    // =========================================================================
    // TOC parsing (matches rpf_extractor.cpp format)
    // =========================================================================

    struct TocEntryRaw {
        int32_t nameOffset;
        int32_t field1;
        int32_t field2;
        uint32_t field3;
    };

    static bool LoadRpfCache(const std::filesystem::path& rpfPath, RpfCache& cache)
    {
        std::ifstream file(rpfPath, std::ios::binary);
        if (!file)
            return false;

        // Read header
        uint32_t magic, tocSize, entryCount, unknown, encrypted;
        file.read(reinterpret_cast<char*>(&magic), 4);
        file.read(reinterpret_cast<char*>(&tocSize), 4);
        file.read(reinterpret_cast<char*>(&entryCount), 4);
        file.read(reinterpret_cast<char*>(&unknown), 4);
        file.read(reinterpret_cast<char*>(&encrypted), 4);

        if (magic != RPF2_MAGIC)
            return false;

        cache.rpfPath = rpfPath;
        cache.isEncrypted = (encrypted != 0);
        cache.aesKey = g_aesKey;

        // Read TOC
        file.seekg(TOC_OFFSET);
        std::vector<uint8_t> tocData(tocSize);
        file.read(reinterpret_cast<char*>(tocData.data()), tocSize);

        // Decrypt TOC if needed
        if (cache.isEncrypted && !g_aesKey.empty())
        {
            DecryptAES256(tocData, g_aesKey);
        }

        // Parse entries
        const uint8_t* ptr = tocData.data();
        std::vector<TocEntryRaw> rawEntries(entryCount);

        for (uint32_t i = 0; i < entryCount; i++)
        {
            std::memcpy(&rawEntries[i], ptr, 16);
            ptr += 16;
        }

        // Parse name table
        size_t nameTableSize = tocSize - (entryCount * 16);
        const char* nameTable = reinterpret_cast<const char*>(ptr);

        // Build entries
        cache.entries.clear();
        cache.entries.reserve(entryCount);

        for (uint32_t i = 0; i < entryCount; i++)
        {
            const auto& raw = rawEntries[i];
            RpfFileEntry entry;

            // Get name
            if (raw.nameOffset >= 0 && static_cast<size_t>(raw.nameOffset) < nameTableSize)
            {
                const char* nameStart = nameTable + raw.nameOffset;
                entry.name = std::string(nameStart);
            }

            entry.isDirectory = (raw.field2 < 0);

            if (!entry.isDirectory)
            {
                entry.size = raw.field1;
                entry.offset = raw.field2;
                
                bool isResource = (raw.field3 & 0xC0000000) == 0xC0000000;
                if (isResource)
                {
                    entry.resourceType = static_cast<uint8_t>(entry.offset & 0xFF);
                    entry.offset = entry.offset & 0x7FFFFF00;
                    entry.compressedSize = entry.size;
                    entry.isCompressed = false;
                }
                else
                {
                    entry.compressedSize = raw.field3 & 0xBFFFFFFF;
                    entry.isCompressed = (raw.field3 & 0x40000000) != 0;
                    entry.resourceType = 0;
                }
            }

            cache.entries.push_back(entry);
        }

        // Build paths recursively
        std::function<void(int, const std::string&)> buildPaths;
        buildPaths = [&](int idx, const std::string& parent) {
            if (idx < 0 || static_cast<size_t>(idx) >= cache.entries.size())
                return;

            auto& entry = cache.entries[idx];
            entry.fullPath = parent.empty() ? entry.name : parent + "/" + entry.name;

            if (entry.isDirectory)
            {
                const auto& raw = rawEntries[idx];
                int contentIndex = raw.field2 & 0x7FFFFFFF;
                int contentCount = raw.field3 & 0x0FFFFFFF;

                for (int i = 0; i < contentCount; i++)
                {
                    int childIdx = contentIndex + i;
                    if (childIdx > idx)
                        buildPaths(childIdx, entry.fullPath);
                }
            }
        };

        if (!cache.entries.empty())
            buildPaths(0, "");

        // Build file index
        cache.fileIndex.clear();
        for (size_t i = 0; i < cache.entries.size(); i++)
        {
            if (!cache.entries[i].isDirectory)
            {
                std::string key = cache.entries[i].fullPath;
                std::transform(key.begin(), key.end(), key.begin(), ::tolower);
                std::replace(key.begin(), key.end(), '\\', '/');
                
                // Remove leading slashes
                while (!key.empty() && key[0] == '/')
                    key = key.substr(1);

                cache.fileIndex[key] = i;
            }
        }

        return true;
    }

    // =========================================================================
    // Public API
    // =========================================================================

    void Initialize()
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        if (g_initialized)
            return;

        g_initialized = true;
        g_rpfCaches.clear();
        g_fileToRpf.clear();
        g_extractedCache.clear();
        g_stats = Stats{};

        // Try to load AES key from standard locations
        std::vector<std::filesystem::path> keyPaths = {
            "aes_key.bin",
            "../aes_key.bin",
            "game/aes_key.bin"
        };

        for (const auto& keyPath : keyPaths)
        {
            std::error_code ec;
            if (std::filesystem::exists(keyPath, ec))
            {
                std::ifstream keyFile(keyPath, std::ios::binary);
                if (keyFile)
                {
                    g_aesKey.resize(32);
                    keyFile.read(reinterpret_cast<char*>(g_aesKey.data()), 32);
                    if (keyFile.gcount() == 32)
                    {
                        LOGF_UTILITY("[RpfLoader] Loaded AES key from: {}", keyPath.string());
                        break;
                    }
                }
            }
        }

        LOGF_UTILITY("[RpfLoader] Initialized");
    }

    bool IsInitialized()
    {
        return g_initialized;
    }

    void SetAesKey(const std::vector<uint8_t>& key)
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        g_aesKey = key;
    }

    void ScanForRpfFiles(const std::filesystem::path& overlayRoot)
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        
        std::error_code ec;
        if (!std::filesystem::exists(overlayRoot, ec))
            return;

        for (const auto& entry : std::filesystem::recursive_directory_iterator(overlayRoot, ec))
        {
            if (ec || !entry.is_regular_file())
                continue;

            std::string ext = entry.path().extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

            if (ext == ".rpf")
            {
                std::string rpfKey = entry.path().string();
                
                // Skip if already loaded
                if (g_rpfCaches.find(rpfKey) != g_rpfCaches.end())
                    continue;

                RpfCache cache;
                if (LoadRpfCache(entry.path(), cache))
                {
                    // Register all files from this RPF
                    for (const auto& [normalizedPath, idx] : cache.fileIndex)
                    {
                        g_fileToRpf[normalizedPath] = rpfKey;
                    }

                    g_rpfCaches[rpfKey] = std::move(cache);
                    g_stats.rpfsLoaded++;

                    LOGF_UTILITY("[RpfLoader] Loaded RPF: {} ({} files)",
                        entry.path().filename().string(), cache.fileIndex.size());
                }
            }
        }
    }

    bool HasFile(const std::string& normalizedPath)
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        return g_fileToRpf.find(normalizedPath) != g_fileToRpf.end();
    }

    std::filesystem::path GetContainingRpf(const std::string& normalizedPath)
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        auto it = g_fileToRpf.find(normalizedPath);
        if (it != g_fileToRpf.end())
            return std::filesystem::path(it->second);
        return {};
    }

    std::optional<ExtractedFile> ExtractFile(const std::string& normalizedPath)
    {
        std::lock_guard<std::mutex> lock(g_mutex);

        // Check extraction cache
        auto cacheIt = g_extractedCache.find(normalizedPath);
        if (cacheIt != g_extractedCache.end())
        {
            g_stats.cacheHits++;
            ExtractedFile result;
            result.data = cacheIt->second;
            result.originalPath = normalizedPath;
            result.fromCache = true;
            return result;
        }

        g_stats.cacheMisses++;

        // Find containing RPF
        auto rpfIt = g_fileToRpf.find(normalizedPath);
        if (rpfIt == g_fileToRpf.end())
            return std::nullopt;

        auto cacheIt2 = g_rpfCaches.find(rpfIt->second);
        if (cacheIt2 == g_rpfCaches.end())
            return std::nullopt;

        const RpfCache& cache = cacheIt2->second;
        auto fileIt = cache.fileIndex.find(normalizedPath);
        if (fileIt == cache.fileIndex.end())
            return std::nullopt;

        const RpfFileEntry& entry = cache.entries[fileIt->second];

        // Read from RPF
        std::ifstream file(cache.rpfPath, std::ios::binary);
        if (!file)
            return std::nullopt;

        file.seekg(entry.offset);
        int readSize = entry.isCompressed ? entry.compressedSize : entry.size;
        if (readSize <= 0) readSize = entry.size;

        std::vector<uint8_t> data(readSize);
        file.read(reinterpret_cast<char*>(data.data()), readSize);

        // Decompress if needed
        std::vector<uint8_t> output;
        if (entry.isCompressed && entry.compressedSize < static_cast<uint32_t>(entry.size) && entry.size > 0)
        {
            output = DecompressDeflate(data, entry.size);
            if (output.empty())
                output = std::move(data);
        }
        else
        {
            output = std::move(data);
        }

        // Cache the result
        g_extractedCache[normalizedPath] = output;
        g_stats.filesExtracted++;
        g_stats.bytesExtracted += output.size();

        ExtractedFile result;
        result.data = std::move(output);
        result.originalPath = normalizedPath;
        result.fromCache = false;
        return result;
    }

    std::filesystem::path ExtractToTemp(const std::string& normalizedPath)
    {
        auto extracted = ExtractFile(normalizedPath);
        if (!extracted.has_value())
            return {};

        // Create temp file
        std::filesystem::path tempDir = std::filesystem::temp_directory_path() / "LibertyRecomp" / "rpf_cache";
        std::error_code ec;
        std::filesystem::create_directories(tempDir, ec);

        // Use path hash for filename
        std::hash<std::string> hasher;
        std::string tempName = std::to_string(hasher(normalizedPath));
        
        // Preserve extension
        size_t dotPos = normalizedPath.rfind('.');
        if (dotPos != std::string::npos)
            tempName += normalizedPath.substr(dotPos);

        std::filesystem::path tempPath = tempDir / tempName;

        std::ofstream outFile(tempPath, std::ios::binary);
        if (!outFile)
            return {};

        outFile.write(reinterpret_cast<const char*>(extracted->data.data()), extracted->data.size());
        return tempPath;
    }

    std::vector<RpfFileEntry> ListFiles(const std::filesystem::path& rpfPath)
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        
        std::string key = rpfPath.string();
        auto it = g_rpfCaches.find(key);
        if (it != g_rpfCaches.end())
        {
            std::vector<RpfFileEntry> files;
            for (const auto& entry : it->second.entries)
            {
                if (!entry.isDirectory)
                    files.push_back(entry);
            }
            return files;
        }
        return {};
    }

    std::vector<std::filesystem::path> GetLoadedRpfs()
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        std::vector<std::filesystem::path> result;
        for (const auto& [path, cache] : g_rpfCaches)
        {
            result.push_back(path);
        }
        return result;
    }

    void ClearCache()
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        g_extractedCache.clear();
    }

    Stats GetStats()
    {
        return g_stats;
    }

    void DumpStatus()
    {
        LOGF_UTILITY("[RpfLoader] Stats: rpfs={} extracted={} bytes={} hits={} misses={}",
            g_stats.rpfsLoaded, g_stats.filesExtracted, g_stats.bytesExtracted,
            g_stats.cacheHits, g_stats.cacheMisses);
        
        for (const auto& [path, cache] : g_rpfCaches)
        {
            LOGF_UTILITY("  RPF: {} ({} files)", 
                std::filesystem::path(path).filename().string(),
                cache.fileIndex.size());
        }
    }
}
// ============================================================================
// Multi-Version RPF Support (RPF0, RPF2, RPF3)
// ============================================================================

namespace RpfLoader
{
    // RPF Version magic numbers
    static constexpr uint32_t RPF0_MAGIC = 0x30465052;  // "RPF0" - Table Tennis
    static constexpr uint32_t RPF2_MAGIC = 0x32465052;  // "RPF2" - GTA IV
    static constexpr uint32_t RPF3_MAGIC = 0x33465052;  // "RPF3" - GTA IV Audio, Midnight Club LA

    enum class RpfVersion
    {
        Unknown = -1,
        V0 = 0,  // Table Tennis
        V2 = 2,  // GTA IV
        V3 = 3   // GTA IV Audio
    };

    // RPF0 Header (12 bytes)
    #pragma pack(push, 1)
    struct RpfHeaderV0
    {
        uint32_t magic;       // RPF0
        uint32_t tocSize;     // TOC size in bytes
        uint32_t entryCount;  // Number of entries
    };

    // RPF2/3 Header (20 bytes)
    struct RpfHeaderV23
    {
        uint32_t magic;       // RPF2 or RPF3
        uint32_t tocSize;     // TOC size in bytes
        int32_t entryCount;   // Number of entries
        int32_t unknown;      // Unknown field
        uint32_t encrypted;   // Encryption flag (0 = unencrypted)
    };

    // RPF0 Directory Entry (16 bytes)
    struct RpfDirEntryV0
    {
        uint32_t nameOffsetAndType;  // 3 bytes name offset, 1 byte type (0x80 = dir)
        uint32_t contentIndex;       // First entry index in directory
        uint32_t contentCount;       // Count of entries
        uint32_t contentCount2;      // Same as contentCount (padding)
    };

    // RPF0 File Entry (16 bytes)
    struct RpfFileEntryV0
    {
        uint32_t nameOffsetAndType;  // 3 bytes name offset, 1 byte type (0 = file)
        uint32_t offset;             // File data offset
        uint32_t size;               // Compressed size
        uint32_t uncompressedSize;   // Uncompressed size
    };

    // RPF2/3 Entry (16 bytes) - unified structure
    struct RpfEntryV23
    {
        uint32_t field1;  // nameOffset (V2) or nameHash (V3)
        uint32_t field2;  // size for files, flags for directories
        uint32_t field3;  // offset for files, contentIndex for directories
        uint32_t field4;  // flags/sizeInArchive for files, contentCount for directories
    };
    #pragma pack(pop)

    /**
     * Detect RPF version from magic number.
     */
    static RpfVersion DetectRpfVersion(uint32_t magic)
    {
        switch (magic)
        {
            case RPF0_MAGIC: return RpfVersion::V0;
            case RPF2_MAGIC: return RpfVersion::V2;
            case RPF3_MAGIC: return RpfVersion::V3;
            default: return RpfVersion::Unknown;
        }
    }

    /**
     * Parse RPF0 format (Table Tennis style).
     * Simpler format without encryption.
     */
    static bool ParseRpfV0(std::ifstream& file, RpfCache& cache)
    {
        file.seekg(0);
        RpfHeaderV0 header;
        file.read(reinterpret_cast<char*>(&header), sizeof(header));

        if (header.magic != RPF0_MAGIC)
            return false;

        cache.isEncrypted = false;

        // Read TOC (starts at offset 2048)
        file.seekg(TOC_OFFSET);
        std::vector<uint8_t> tocData(header.tocSize);
        file.read(reinterpret_cast<char*>(tocData.data()), header.tocSize);

        // Parse entries
        const uint8_t* ptr = tocData.data();
        size_t entryDataSize = header.entryCount * 16;
        
        // Name table starts after entries
        const char* nameTable = reinterpret_cast<const char*>(tocData.data() + entryDataSize);
        size_t nameTableSize = header.tocSize - entryDataSize;

        cache.entries.clear();
        cache.entries.reserve(header.entryCount);

        for (uint32_t i = 0; i < header.entryCount; i++)
        {
            uint32_t nameOffsetAndType = *reinterpret_cast<const uint32_t*>(ptr);
            uint32_t nameOffset = nameOffsetAndType & 0x00FFFFFF;
            bool isDirectory = (nameOffsetAndType & 0x80000000) != 0;

            RpfFileEntry entry;
            entry.isDirectory = isDirectory;

            // Get name
            if (nameOffset < nameTableSize)
            {
                entry.name = std::string(nameTable + nameOffset);
            }

            if (isDirectory)
            {
                const RpfDirEntryV0* dirEntry = reinterpret_cast<const RpfDirEntryV0*>(ptr);
                // Directory - no file data
                entry.offset = 0;
                entry.size = 0;
            }
            else
            {
                const RpfFileEntryV0* fileEntry = reinterpret_cast<const RpfFileEntryV0*>(ptr);
                entry.offset = fileEntry->offset;
                entry.size = fileEntry->uncompressedSize;
                entry.compressedSize = fileEntry->size;
                entry.isCompressed = (fileEntry->size != fileEntry->uncompressedSize);
            }

            cache.entries.push_back(entry);
            ptr += 16;
        }

        return true;
    }

    /**
     * Parse RPF2 format (GTA IV main format).
     * Uses name offsets in TOC.
     */
    static bool ParseRpfV2(std::ifstream& file, RpfCache& cache, const std::vector<uint8_t>& tocData, uint32_t entryCount)
    {
        const uint8_t* ptr = tocData.data();
        size_t entryDataSize = entryCount * 16;
        
        // Name table starts after entries
        const char* nameTable = reinterpret_cast<const char*>(tocData.data() + entryDataSize);
        size_t nameTableSize = tocData.size() - entryDataSize;

        cache.entries.clear();
        cache.entries.reserve(entryCount);

        for (uint32_t i = 0; i < entryCount; i++)
        {
            const RpfEntryV23* raw = reinterpret_cast<const RpfEntryV23*>(ptr);
            RpfFileEntry entry;

            // V2 uses name offset in field1
            uint32_t nameOffset = raw->field1;
            if (nameOffset < nameTableSize)
            {
                entry.name = std::string(nameTable + nameOffset);
            }

            // Check if directory (MSB of field3 set)
            entry.isDirectory = (static_cast<int32_t>(raw->field3) < 0);

            if (!entry.isDirectory)
            {
                entry.size = raw->field2;
                entry.offset = raw->field3;
                
                // Resource type in low byte of offset for resources
                bool isResource = (raw->field4 & 0xC0000000) == 0xC0000000;
                if (isResource)
                {
                    entry.resourceType = static_cast<uint8_t>(entry.offset & 0xFF);
                    entry.offset = entry.offset & 0x7FFFFF00;
                    entry.compressedSize = entry.size;
                    entry.isCompressed = false;
                }
                else
                {
                    entry.compressedSize = raw->field4 & 0xBFFFFFFF;
                    entry.isCompressed = (raw->field4 & 0x40000000) != 0;
                    entry.resourceType = 0;
                }
            }

            cache.entries.push_back(entry);
            ptr += 16;
        }

        return true;
    }

    /**
     * Parse RPF3 format (GTA IV Audio).
     * Uses name hashes instead of offsets.
     */
    static bool ParseRpfV3(std::ifstream& file, RpfCache& cache, const std::vector<uint8_t>& tocData, uint32_t entryCount)
    {
        // RPF3 uses Jenkins hash for names - we can't recover original names
        // But we can still extract files by hash
        
        const uint8_t* ptr = tocData.data();
        
        cache.entries.clear();
        cache.entries.reserve(entryCount);

        for (uint32_t i = 0; i < entryCount; i++)
        {
            const RpfEntryV23* raw = reinterpret_cast<const RpfEntryV23*>(ptr);
            RpfFileEntry entry;

            // V3 uses name hash in field1 - generate placeholder name
            uint32_t nameHash = raw->field1;
            char hashName[16];
            snprintf(hashName, sizeof(hashName), "0x%08X", nameHash);
            entry.name = hashName;

            // Check if directory
            entry.isDirectory = (static_cast<int32_t>(raw->field3) < 0);

            if (!entry.isDirectory)
            {
                entry.size = raw->field2;
                entry.offset = raw->field3 & 0x7FFFFFFF;
                entry.compressedSize = raw->field4 & 0x00FFFFFF;
                entry.isCompressed = (raw->field4 & 0x01000000) != 0;
                entry.resourceType = static_cast<uint8_t>((raw->field4 >> 24) & 0x7F);
            }

            cache.entries.push_back(entry);
            ptr += 16;
        }

        return true;
    }

    /**
     * Load RPF cache with multi-version support.
     */
    static bool LoadRpfCacheMultiVersion(const std::filesystem::path& rpfPath, RpfCache& cache)
    {
        std::ifstream file(rpfPath, std::ios::binary);
        if (!file)
            return false;

        // Read magic to detect version
        uint32_t magic;
        file.read(reinterpret_cast<char*>(&magic), 4);
        
        RpfVersion version = DetectRpfVersion(magic);
        if (version == RpfVersion::Unknown)
        {
            LOGF_WARNING("[RpfLoader] Unknown RPF version: 0x{:08X}", magic);
            return false;
        }

        cache.rpfPath = rpfPath;

        if (version == RpfVersion::V0)
        {
            return ParseRpfV0(file, cache);
        }

        // V2/V3 have same header structure
        file.seekg(0);
        RpfHeaderV23 header;
        file.read(reinterpret_cast<char*>(&header), sizeof(header));

        cache.isEncrypted = (header.encrypted != 0);

        // Read TOC
        file.seekg(TOC_OFFSET);
        std::vector<uint8_t> tocData(header.tocSize);
        file.read(reinterpret_cast<char*>(tocData.data()), header.tocSize);

        // Decrypt TOC if needed
        if (cache.isEncrypted && !g_aesKey.empty())
        {
            if (!DecryptAES256(tocData, g_aesKey))
            {
                LOGF_WARNING("[RpfLoader] Failed to decrypt TOC for: {}", rpfPath.string());
                return false;
            }
        }

        bool success = false;
        if (version == RpfVersion::V2)
        {
            success = ParseRpfV2(file, cache, tocData, header.entryCount);
        }
        else if (version == RpfVersion::V3)
        {
            success = ParseRpfV3(file, cache, tocData, header.entryCount);
        }

        if (!success)
            return false;

        // Build file paths recursively
        std::function<void(int, const std::string&)> buildPaths;
        buildPaths = [&](int idx, const std::string& parent) {
            if (idx < 0 || static_cast<size_t>(idx) >= cache.entries.size())
                return;

            auto& entry = cache.entries[idx];
            entry.fullPath = parent.empty() ? entry.name : parent + "/" + entry.name;
        };

        // Build simple paths for now
        for (size_t i = 0; i < cache.entries.size(); i++)
        {
            cache.entries[i].fullPath = cache.entries[i].name;
        }

        // Build file index
        cache.fileIndex.clear();
        for (size_t i = 0; i < cache.entries.size(); i++)
        {
            if (!cache.entries[i].isDirectory)
            {
                std::string key = cache.entries[i].fullPath;
                std::transform(key.begin(), key.end(), key.begin(), ::tolower);
                std::replace(key.begin(), key.end(), '\\', '/');
                
                while (!key.empty() && key[0] == '/')
                    key = key.substr(1);

                cache.fileIndex[key] = i;
            }
        }

        LOGF_UTILITY("[RpfLoader] Loaded RPF{}: {} ({} entries)", 
            static_cast<int>(version), rpfPath.filename().string(), cache.entries.size());

        return true;
    }
}
