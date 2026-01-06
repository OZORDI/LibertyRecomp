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
