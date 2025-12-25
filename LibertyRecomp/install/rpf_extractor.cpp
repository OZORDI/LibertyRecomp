#include "rpf_extractor.h"
#include "thread_pool.h"

#include <array>
#include <atomic>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <sstream>
#include <zlib.h>

#if defined(_WIN32)
#include <windows.h>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")
#else
#include <CommonCrypto/CommonCrypto.h>
#endif

namespace RpfExtractor
{
    // RPF2 magic number (little-endian "RPF2")
    constexpr uint32_t RPF2_MAGIC = 0x32465052;
    
    // TOC starts at this offset
    constexpr uint32_t TOC_OFFSET = 0x800;
    
    // Block size for file data alignment
    constexpr uint32_t BLOCK_SIZE = 0x800;
    
    // =========================================================================
    // Internal structures matching SparkIV format
    // =========================================================================
    
    struct TocEntry {
        int32_t nameOffset;
        int32_t field1;
        int32_t field2;
        uint32_t field3;
        
        bool isDirectory;
        
        // Directory fields
        int32_t contentEntryIndex;
        int32_t contentEntryCount;
        
        // File fields
        int32_t size;           // Uncompressed size
        int32_t offset;         // File data offset
        int32_t sizeInArchive;  // Compressed size
        bool isCompressed;
        bool isResourceFile;
        uint8_t resourceType;
        
        std::string name;
        std::string fullPath;   // Built during tree traversal
    };
    
    // =========================================================================
    // AES-256-ECB decryption (16 rounds like SparkIV)
    // =========================================================================
    
    static bool DecryptAES256_16Rounds(
        std::vector<uint8_t>& data,
        const std::vector<uint8_t>& key)
    {
        if (key.size() != 32 || data.empty())
        {
            return false;
        }
        
        // Only decrypt data aligned to 16 bytes
        size_t dataLen = data.size() & ~0x0F;
        if (dataLen == 0)
        {
            return true;  // Nothing to decrypt
        }
        
#if defined(_WIN32)
        BCRYPT_ALG_HANDLE hAlg = nullptr;
        BCRYPT_KEY_HANDLE hKey = nullptr;
        
        if (BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_AES_ALGORITHM, nullptr, 0) != 0)
        {
            return false;
        }
        
        // Set ECB mode
        if (BCryptSetProperty(hAlg, BCRYPT_CHAINING_MODE, 
            (PUCHAR)BCRYPT_CHAIN_MODE_ECB, sizeof(BCRYPT_CHAIN_MODE_ECB), 0) != 0)
        {
            BCryptCloseAlgorithmProvider(hAlg, 0);
            return false;
        }
        
        // Create key
        if (BCryptGenerateSymmetricKey(hAlg, &hKey, nullptr, 0, 
            (PUCHAR)key.data(), (ULONG)key.size(), 0) != 0)
        {
            BCryptCloseAlgorithmProvider(hAlg, 0);
            return false;
        }
        
        // Decrypt 16 times (like SparkIV)
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
        // macOS/iOS CommonCrypto - decrypt 16 times like SparkIV
        for (int round = 0; round < 16; round++)
        {
            size_t outLength = 0;
            CCCryptorStatus status = CCCrypt(
                kCCDecrypt,
                kCCAlgorithmAES,
                kCCOptionECBMode,
                key.data(), kCCKeySizeAES256,
                nullptr,  // No IV for ECB
                data.data(), dataLen,
                data.data(), dataLen,
                &outLength);
            
            if (status != kCCSuccess)
            {
                return false;
            }
        }
        return true;
#endif
    }
    
    // =========================================================================
    // Raw deflate decompression (no zlib header)
    // =========================================================================
    
    static std::vector<uint8_t> DecompressRawDeflate(
        const std::vector<uint8_t>& compressed,
        size_t expectedSize)
    {
        std::vector<uint8_t> decompressed(expectedSize);
        
        z_stream stream{};
        stream.next_in = const_cast<Bytef*>(compressed.data());
        stream.avail_in = static_cast<uInt>(compressed.size());
        stream.next_out = decompressed.data();
        stream.avail_out = static_cast<uInt>(expectedSize);
        
        // Use -MAX_WBITS for raw deflate (no zlib/gzip header)
        if (inflateInit2(&stream, -MAX_WBITS) != Z_OK)
        {
            return {};
        }
        
        int result = inflate(&stream, Z_FINISH);
        inflateEnd(&stream);
        
        if (result != Z_STREAM_END)
        {
            return {};
        }
        
        decompressed.resize(stream.total_out);
        return decompressed;
    }
    
    // =========================================================================
    // RPF header and TOC reading
    // =========================================================================
    
    bool IsRpfFile(const std::filesystem::path& path)
    {
        if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path))
        {
            return false;
        }
        
        std::string ext = path.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        if (ext != ".rpf")
        {
            return false;
        }
        
        RpfHeader header;
        return ReadHeader(path, header);
    }
    
    bool ReadHeader(const std::filesystem::path& path, RpfHeader& header)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file)
        {
            return false;
        }
        
        file.read(reinterpret_cast<char*>(&header.magic), 4);
        file.read(reinterpret_cast<char*>(&header.tocSize), 4);
        file.read(reinterpret_cast<char*>(&header.entryCount), 4);
        file.read(reinterpret_cast<char*>(&header.unknown), 4);
        file.read(reinterpret_cast<char*>(&header.encrypted), 4);
        
        if (!file)
        {
            return false;
        }
        
        if (header.magic != RPF2_MAGIC)
        {
            return false;
        }
        
        return true;
    }
    
    bool HasEncryptedFiles(const std::filesystem::path& path)
    {
        RpfHeader header;
        if (!ReadHeader(path, header))
        {
            return false;
        }
        return header.encrypted != 0;
    }
    
    bool LoadAesKey(const std::filesystem::path& keyPath, std::vector<uint8_t>& key)
    {
        std::ifstream file(keyPath, std::ios::binary);
        if (!file)
        {
            return false;
        }
        
        key.resize(32);
        file.read(reinterpret_cast<char*>(key.data()), 32);
        
        return file.gcount() == 32;
    }
    
    bool ExtractAesKeyFromExecutable(
        const std::filesystem::path& exePath,
        std::vector<uint8_t>& key)
    {
        // Placeholder - rely on shipped aes_key.bin
        return false;
    }
    
    // =========================================================================
    // Read TOC entries with proper name table parsing (SparkIV format)
    // =========================================================================
    
    static std::vector<TocEntry> ReadTocEntries(
        std::ifstream& file,
        const RpfHeader& header,
        const std::vector<uint8_t>& aesKey)
    {
        std::vector<TocEntry> entries;
        
        // Read entire TOC
        file.seekg(TOC_OFFSET);
        std::vector<uint8_t> tocData(header.tocSize);
        file.read(reinterpret_cast<char*>(tocData.data()), header.tocSize);
        
        if (!file)
        {
            return entries;
        }
        
        // Decrypt TOC if needed
        if (header.encrypted != 0 && !aesKey.empty())
        {
            DecryptAES256_16Rounds(tocData, aesKey);
        }
        
        // Parse entries from TOC data
        const uint8_t* ptr = tocData.data();
        
        for (uint32_t i = 0; i < header.entryCount; i++)
        {
            TocEntry entry;
            
            // Read 16 bytes
            std::memcpy(&entry.nameOffset, ptr + 0, 4);
            std::memcpy(&entry.field1, ptr + 4, 4);
            std::memcpy(&entry.field2, ptr + 8, 4);
            std::memcpy(&entry.field3, ptr + 12, 4);
            ptr += 16;
            
            // Directory detection: high bit of field2 (byte 8) per SparkIV
            entry.isDirectory = (entry.field2 < 0);  // Signed negative = high bit set
            
            if (entry.isDirectory)
            {
                // DirectoryEntry format
                entry.contentEntryIndex = entry.field2 & 0x7FFFFFFF;
                entry.contentEntryCount = entry.field3 & 0x0FFFFFFF;
                entry.size = 0;
                entry.offset = 0;
                entry.sizeInArchive = 0;
                entry.isCompressed = false;
                entry.isResourceFile = false;
                entry.resourceType = 0;
            }
            else
            {
                // FileEntry format
                entry.size = entry.field1;
                entry.offset = entry.field2;
                
                // Check if resource file
                entry.isResourceFile = (entry.field3 & 0xC0000000) == 0xC0000000;
                
                if (entry.isResourceFile)
                {
                    entry.resourceType = static_cast<uint8_t>(entry.offset & 0xFF);
                    entry.offset = entry.offset & 0x7FFFFF00;
                    entry.sizeInArchive = entry.size;
                    entry.isCompressed = false;
                }
                else
                {
                    entry.sizeInArchive = entry.field3 & 0xBFFFFFFF;
                    entry.isCompressed = (entry.field3 & 0x40000000) != 0;
                    entry.resourceType = 0;
                }
                
                entry.contentEntryIndex = 0;
                entry.contentEntryCount = 0;
            }
            
            entries.push_back(entry);
        }
        
        // Read name string table (follows TOC entries)
        size_t nameTableSize = header.tocSize - (header.entryCount * 16);
        const char* nameTable = reinterpret_cast<const char*>(ptr);
        
        // Populate entry names from name table
        for (auto& entry : entries)
        {
            if (entry.nameOffset >= 0 && static_cast<size_t>(entry.nameOffset) < nameTableSize)
            {
                // Read null-terminated string
                const char* nameStart = nameTable + entry.nameOffset;
                const char* nameEnd = nameStart;
                while (nameEnd < nameTable + nameTableSize && *nameEnd != '\0')
                {
                    nameEnd++;
                }
                entry.name = std::string(nameStart, nameEnd);
            }
            else
            {
                entry.name = "unknown_" + std::to_string(&entry - entries.data());
            }
        }
        
        return entries;
    }
    
    // =========================================================================
    // Build full paths by traversing directory tree
    // =========================================================================
    
    static void BuildPaths(
        std::vector<TocEntry>& entries,
        int entryIndex,
        const std::string& parentPath)
    {
        if (entryIndex < 0 || static_cast<size_t>(entryIndex) >= entries.size())
        {
            return;
        }
        
        TocEntry& entry = entries[entryIndex];
        
        // Build full path
        if (parentPath.empty() || entry.name == "/" || entry.name.empty())
        {
            entry.fullPath = entry.name;
        }
        else
        {
            entry.fullPath = parentPath + "/" + entry.name;
        }
        
        // If directory, recursively process children
        if (entry.isDirectory)
        {
            for (int i = 0; i < entry.contentEntryCount; i++)
            {
                int childIndex = entry.contentEntryIndex + i;
                if (childIndex > entryIndex && static_cast<size_t>(childIndex) < entries.size())
                {
                    BuildPaths(entries, childIndex, entry.fullPath);
                }
            }
        }
    }
    
    // =========================================================================
    // Public ListEntries function
    // =========================================================================
    
    std::vector<RpfFileEntry> ListEntries(
        const std::filesystem::path& rpfPath,
        const std::vector<uint8_t>& aesKey)
    {
        std::vector<RpfFileEntry> result;
        
        RpfHeader header;
        if (!ReadHeader(rpfPath, header))
        {
            return result;
        }
        
        std::ifstream file(rpfPath, std::ios::binary);
        if (!file)
        {
            return result;
        }
        
        auto tocEntries = ReadTocEntries(file, header, aesKey);
        
        // Build paths starting from root (entry 0)
        if (!tocEntries.empty())
        {
            BuildPaths(tocEntries, 0, "");
        }
        
        // Convert to public format
        for (const auto& entry : tocEntries)
        {
            RpfFileEntry pub;
            pub.name = entry.fullPath;
            pub.isDirectory = entry.isDirectory;
            pub.offset = entry.offset;
            pub.size = entry.size;
            pub.compressedSize = entry.sizeInArchive;
            pub.isCompressed = entry.isCompressed;
            pub.isEncrypted = false;  // Encryption is at TOC level
            pub.resourceType = entry.resourceType;
            result.push_back(pub);
        }
        
        return result;
    }
    
    // =========================================================================
    // Native RPF extraction implementation
    // =========================================================================
    
    ExtractionResult Extract(
        const std::filesystem::path& rpfPath,
        const std::filesystem::path& outputDir,
        const std::vector<uint8_t>& aesKey,
        const std::function<void(float)>& progressCallback)
    {
        ExtractionResult result;
        
        // Read header
        RpfHeader header;
        if (!ReadHeader(rpfPath, header))
        {
            result.errorMessage = "Failed to read RPF header from: " + rpfPath.string();
            return result;
        }
        
        std::ifstream file(rpfPath, std::ios::binary);
        if (!file)
        {
            result.errorMessage = "Failed to open RPF file: " + rpfPath.string();
            return result;
        }
        
        // Read and parse TOC
        auto tocEntries = ReadTocEntries(file, header, aesKey);
        if (tocEntries.empty())
        {
            result.errorMessage = "No entries found in RPF: " + rpfPath.string();
            return result;
        }
        
        // Build full paths
        BuildPaths(tocEntries, 0, "");
        
        // Create output directory
        std::error_code ec;
        std::filesystem::create_directories(outputDir, ec);
        
        // Count files for progress
        size_t totalFiles = 0;
        for (const auto& entry : tocEntries)
        {
            if (!entry.isDirectory)
            {
                totalFiles++;
            }
        }
        
        size_t filesProcessed = 0;
        uint64_t bytesExtracted = 0;
        
        // Extract all entries
        for (const auto& entry : tocEntries)
        {
            // Clean path (remove leading "/" or "root" markers)
            std::string cleanPath = entry.fullPath;
            while (!cleanPath.empty() && (cleanPath[0] == '/' || cleanPath[0] == '\\'))
            {
                cleanPath = cleanPath.substr(1);
            }
            if (cleanPath.empty() || cleanPath == "/" || cleanPath == "\\")
            {
                continue;  // Skip root entry
            }
            
            std::filesystem::path entryPath = outputDir / cleanPath;
            
            if (entry.isDirectory)
            {
                // Create directory
                std::filesystem::create_directories(entryPath, ec);
            }
            else
            {
                // Extract file
                std::filesystem::create_directories(entryPath.parent_path(), ec);
                
                // Read file data
                file.seekg(entry.offset);
                int readSize = entry.isCompressed ? entry.sizeInArchive : entry.size;
                if (readSize <= 0) readSize = entry.size;
                
                std::vector<uint8_t> data(readSize);
                file.read(reinterpret_cast<char*>(data.data()), readSize);
                
                // Decompress if needed
                std::vector<uint8_t> output;
                if (entry.isCompressed && entry.sizeInArchive < entry.size && entry.size > 0)
                {
                    output = DecompressRawDeflate(data, entry.size);
                    if (output.empty())
                    {
                        // Decompression failed, use raw data
                        output = std::move(data);
                    }
                }
                else
                {
                    output = std::move(data);
                }
                
                // Write to disk
                std::ofstream outFile(entryPath, std::ios::binary);
                if (outFile)
                {
                    outFile.write(reinterpret_cast<const char*>(output.data()), output.size());
                    bytesExtracted += output.size();
                }
                
                filesProcessed++;
                
                if (progressCallback && totalFiles > 0)
                {
                    progressCallback(static_cast<float>(filesProcessed) / static_cast<float>(totalFiles));
                }
            }
        }
        
        if (progressCallback)
        {
            progressCallback(1.0f);
        }
        
        result.success = true;
        result.extractedPath = outputDir;
        result.filesExtracted = filesProcessed;
        result.bytesExtracted = bytesExtracted;
        
        return result;
    }
    
    // =========================================================================
    // Other functions
    // =========================================================================
    
    ExtractionResult ExtractShaders(
        const std::filesystem::path& rpfPath,
        const std::filesystem::path& outputDir,
        const std::vector<uint8_t>& aesKey,
        const std::function<void(float)>& progressCallback)
    {
        return Extract(rpfPath, outputDir, aesKey, progressCallback);
    }
    
    ShaderScanResult ScanAndExtractShaders(
        const std::filesystem::path& gameDir,
        const std::filesystem::path& outputDir,
        const std::vector<uint8_t>& aesKey,
        const std::function<void(float)>& progressCallback)
    {
        ShaderScanResult result;
        
        std::error_code ec;
        std::filesystem::create_directories(outputDir, ec);
        
        // First, look for already-extracted .fxc files
        for (const auto& entry : std::filesystem::recursive_directory_iterator(gameDir, ec))
        {
            if (!entry.is_regular_file())
            {
                continue;
            }
            
            std::string ext = entry.path().extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            
            if (ext == ".fxc")
            {
                result.fxcFiles.push_back(entry.path());
                result.totalShaderBytes += entry.file_size();
            }
        }
        
        if (!result.fxcFiles.empty())
        {
            return result;
        }
        
        // Look for RPF files and extract
        std::vector<std::filesystem::path> rpfFiles;
        for (const auto& entry : std::filesystem::recursive_directory_iterator(gameDir, ec))
        {
            if (!entry.is_regular_file())
            {
                continue;
            }
            
            if (IsRpfFile(entry.path()))
            {
                rpfFiles.push_back(entry.path());
            }
        }
        
        float progressPerRpf = rpfFiles.empty() ? 1.0f : 1.0f / rpfFiles.size();
        for (size_t i = 0; i < rpfFiles.size(); i++)
        {
            if (progressCallback)
            {
                progressCallback(static_cast<float>(i) * progressPerRpf);
            }
            
            auto rpfOutput = outputDir / rpfFiles[i].stem();
            auto extractResult = Extract(rpfFiles[i], rpfOutput, aesKey, nullptr);
            
            if (extractResult.success)
            {
                for (const auto& entry : std::filesystem::recursive_directory_iterator(rpfOutput, ec))
                {
                    if (!entry.is_regular_file())
                    {
                        continue;
                    }
                    
                    std::string ext = entry.path().extension().string();
                    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
                    
                    if (ext == ".fxc")
                    {
                        result.fxcFiles.push_back(entry.path());
                        result.totalShaderBytes += entry.file_size();
                    }
                }
            }
        }
        
        if (progressCallback)
        {
            progressCallback(1.0f);
        }
        
        return result;
    }
    
    ExtractionResult ExtractParallel(
        const std::filesystem::path& rpfPath,
        const std::filesystem::path& outputDir,
        const std::vector<uint8_t>& aesKey,
        const ParallelExtractionOptions& options,
        const std::function<void(float)>& progressCallback)
    {
        // For now, use single-threaded extraction
        return Extract(rpfPath, outputDir, aesKey, progressCallback);
    }
    
    // SparkCLI functions - deprecated, we use native extraction now
    bool IsSparkCliAvailable()
    {
        return false;
    }
    
    ExtractionResult ExtractWithSparkCli(
        const std::filesystem::path& rpfPath,
        const std::filesystem::path& outputDir,
        const std::filesystem::path& keyPath,
        const std::function<void(float)>& progressCallback)
    {
        ExtractionResult result;
        result.errorMessage = "SparkCLI is deprecated. Using native extraction.";
        return result;
    }
}
