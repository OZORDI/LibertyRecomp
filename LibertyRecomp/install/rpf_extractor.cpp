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
#include <wincrypt.h>
#define popen _popen
#define pclose _pclose
#else
#include <CommonCrypto/CommonCrypto.h>
#endif

namespace RpfExtractor
{
    // RPF2 magic number (little-endian "RPF2")
    constexpr uint32_t RPF2_MAGIC = 0x32465052;
    
    // TOC starts at this offset
    constexpr uint32_t TOC_OFFSET = 0x800;
    
    static std::string ExecuteCommand(const std::string& cmd, int& exitCode)
    {
        std::array<char, 128> buffer;
        std::string result;
        
#if defined(_WIN32)
        FILE* pipe = _popen(cmd.c_str(), "r");
#else
        FILE* pipe = popen(cmd.c_str(), "r");
#endif
        
        if (!pipe)
        {
            exitCode = -1;
            return "Failed to execute command";
        }
        
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
        {
            result += buffer.data();
        }
        
#if defined(_WIN32)
        exitCode = _pclose(pipe);
#else
        exitCode = pclose(pipe);
#endif
        
        return result;
    }
    
    bool IsRpfFile(const std::filesystem::path& path)
    {
        if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path))
        {
            return false;
        }
        
        // Check extension
        std::string ext = path.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        if (ext != ".rpf")
        {
            return false;
        }
        
        // Validate magic
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
        
        // Read header (20 bytes, little-endian on disk)
        file.read(reinterpret_cast<char*>(&header.magic), 4);
        file.read(reinterpret_cast<char*>(&header.tocSize), 4);
        file.read(reinterpret_cast<char*>(&header.entryCount), 4);
        file.read(reinterpret_cast<char*>(&header.unknown), 4);
        file.read(reinterpret_cast<char*>(&header.encrypted), 4);
        
        if (!file)
        {
            return false;
        }
        
        // Check magic - RPF2 for GTA IV
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
        
        // AES-256 key is 32 bytes
        key.resize(32);
        file.read(reinterpret_cast<char*>(key.data()), 32);
        
        return file.gcount() == 32;
    }
    
    bool ExtractAesKeyFromExecutable(
        const std::filesystem::path& exePath,
        std::vector<uint8_t>& key)
    {
        // This is a placeholder - actual implementation would need to:
        // 1. Parse the XEX/PE format
        // 2. Find the embedded AES key at a known offset
        // For now, return false and rely on pre-extracted key file
        return false;
    }
    
    std::vector<RpfFileEntry> ListEntries(
        const std::filesystem::path& rpfPath,
        const std::vector<uint8_t>& aesKey)
    {
        std::vector<RpfFileEntry> entries;
        
        RpfHeader header;
        if (!ReadHeader(rpfPath, header))
        {
            return entries;
        }
        
        std::ifstream file(rpfPath, std::ios::binary);
        if (!file)
        {
            return entries;
        }
        
        // Seek to TOC
        file.seekg(TOC_OFFSET);
        if (!file)
        {
            return entries;
        }
        
        // Read TOC entries
        // Each entry is 16 bytes
        // Format depends on whether it's a directory or file entry
        for (uint32_t i = 0; i < header.entryCount; i++)
        {
            uint32_t nameOffset, field1, field2, field3;
            file.read(reinterpret_cast<char*>(&nameOffset), 4);
            file.read(reinterpret_cast<char*>(&field1), 4);
            file.read(reinterpret_cast<char*>(&field2), 4);
            file.read(reinterpret_cast<char*>(&field3), 4);
            
            if (!file)
            {
                break;
            }
            
            RpfFileEntry entry;
            
            // Determine if directory or file based on flags
            // High bit of field1 typically indicates directory
            entry.isDirectory = (field1 & 0x80000000) != 0;
            
            if (entry.isDirectory)
            {
                entry.offset = 0;
                entry.size = 0;
                entry.compressedSize = 0;
                entry.resourceType = 0;
                entry.isCompressed = false;
                entry.isEncrypted = false;
            }
            else
            {
                entry.size = field1;
                // Offset is 3 bytes + resource type is 1 byte
                entry.offset = field2 & 0x00FFFFFF;
                entry.resourceType = (field2 >> 24) & 0xFF;
                // Flags in field3
                entry.isCompressed = (field3 & 0x01) != 0;
                entry.isEncrypted = (field3 & 0x02) != 0;
                entry.compressedSize = entry.isCompressed ? (field3 >> 8) : entry.size;
            }
            
            entries.push_back(entry);
        }
        
        // Read name table and populate entry names
        // Names are stored after the TOC entries
        size_t nameTableOffset = TOC_OFFSET + (header.entryCount * 16);
        for (auto& entry : entries)
        {
            // This is simplified - actual implementation needs proper name offset handling
            entry.name = "entry_" + std::to_string(&entry - entries.data());
        }
        
        return entries;
    }
    
    ExtractionResult Extract(
        const std::filesystem::path& rpfPath,
        const std::filesystem::path& outputDir,
        const std::vector<uint8_t>& aesKey,
        const std::function<void(float)>& progressCallback)
    {
        ExtractionResult result;
        
        // For complex RPF extraction, fall back to SparkCLI
        if (IsSparkCliAvailable())
        {
            return ExtractWithSparkCli(rpfPath, outputDir, {}, progressCallback);
        }
        
        result.errorMessage = "Native RPF extraction not fully implemented. Please use SparkCLI or pre-extract RPF files.";
        return result;
    }
    
    ExtractionResult ExtractShaders(
        const std::filesystem::path& rpfPath,
        const std::filesystem::path& outputDir,
        const std::vector<uint8_t>& aesKey,
        const std::function<void(float)>& progressCallback)
    {
        // Use full extraction and then filter for shaders
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
        
        // If we found shaders directly, we're done
        if (!result.fxcFiles.empty())
        {
            return result;
        }
        
        // Otherwise, look for RPF files and try to extract
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
        
        // Extract each RPF
        float progressPerRpf = rpfFiles.empty() ? 1.0f : 1.0f / rpfFiles.size();
        for (size_t i = 0; i < rpfFiles.size(); i++)
        {
            if (progressCallback)
            {
                progressCallback(static_cast<float>(i) * progressPerRpf);
            }
            
            auto rpfOutput = outputDir / rpfFiles[i].stem();
            auto extractResult = ExtractShaders(rpfFiles[i], rpfOutput, aesKey, nullptr);
            
            if (extractResult.success)
            {
                // Scan extracted directory for .fxc files
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
    
    // AES-256 decryption helper
    static bool DecryptAES256(
        std::vector<uint8_t>& data,
        const std::vector<uint8_t>& key)
    {
        if (key.size() != 32 || data.empty())
        {
            return false;
        }
        
#if defined(_WIN32)
        HCRYPTPROV hProv = 0;
        HCRYPTKEY hKey = 0;
        
        if (!CryptAcquireContext(&hProv, nullptr, nullptr, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
        {
            return false;
        }
        
        // Create key blob
        struct {
            BLOBHEADER header;
            DWORD keySize;
            BYTE keyData[32];
        } keyBlob;
        
        keyBlob.header.bType = PLAINTEXTKEYBLOB;
        keyBlob.header.bVersion = CUR_BLOB_VERSION;
        keyBlob.header.reserved = 0;
        keyBlob.header.aiKeyAlg = CALG_AES_256;
        keyBlob.keySize = 32;
        std::memcpy(keyBlob.keyData, key.data(), 32);
        
        if (!CryptImportKey(hProv, (BYTE*)&keyBlob, sizeof(keyBlob), 0, 0, &hKey))
        {
            CryptReleaseContext(hProv, 0);
            return false;
        }
        
        DWORD mode = CRYPT_MODE_ECB;
        CryptSetKeyParam(hKey, KP_MODE, (BYTE*)&mode, 0);
        
        DWORD dataLen = static_cast<DWORD>(data.size());
        if (!CryptDecrypt(hKey, 0, TRUE, 0, data.data(), &dataLen))
        {
            CryptDestroyKey(hKey);
            CryptReleaseContext(hProv, 0);
            return false;
        }
        
        CryptDestroyKey(hKey);
        CryptReleaseContext(hProv, 0);
        return true;
#else
        // macOS/iOS CommonCrypto
        size_t outLength = 0;
        CCCryptorStatus status = CCCrypt(
            kCCDecrypt,
            kCCAlgorithmAES,
            kCCOptionECBMode,
            key.data(), kCCKeySizeAES256,
            nullptr,  // No IV for ECB
            data.data(), data.size(),
            data.data(), data.size(),
            &outLength);
        
        return status == kCCSuccess;
#endif
    }
    
    // Zlib decompression helper
    static std::vector<uint8_t> DecompressZlib(
        const std::vector<uint8_t>& compressed,
        size_t expectedSize)
    {
        std::vector<uint8_t> decompressed(expectedSize);
        
        z_stream stream{};
        stream.next_in = const_cast<Bytef*>(compressed.data());
        stream.avail_in = static_cast<uInt>(compressed.size());
        stream.next_out = decompressed.data();
        stream.avail_out = static_cast<uInt>(expectedSize);
        
        if (inflateInit(&stream) != Z_OK)
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
    
    ExtractionResult ExtractParallel(
        const std::filesystem::path& rpfPath,
        const std::filesystem::path& outputDir,
        const std::vector<uint8_t>& aesKey,
        const ParallelExtractionOptions& options,
        const std::function<void(float)>& progressCallback)
    {
        ExtractionResult result;
        
        // Read header
        RpfHeader header;
        if (!ReadHeader(rpfPath, header))
        {
            result.errorMessage = "Failed to read RPF header";
            return result;
        }
        
        // Get entries (this is fast, single-threaded is fine)
        auto entries = ListEntries(rpfPath, aesKey);
        if (entries.empty())
        {
            result.errorMessage = "No entries found in RPF";
            return result;
        }
        
        // Create output directory
        std::error_code ec;
        std::filesystem::create_directories(outputDir, ec);
        
        // Pre-create directory structure
        for (const auto& entry : entries)
        {
            if (entry.isDirectory)
            {
                std::filesystem::create_directories(outputDir / entry.name, ec);
            }
        }
        
        // Filter to file entries only
        std::vector<RpfFileEntry> fileEntries;
        for (const auto& entry : entries)
        {
            if (!entry.isDirectory)
            {
                fileEntries.push_back(entry);
            }
        }
        
        if (fileEntries.empty())
        {
            result.success = true;
            result.extractedPath = outputDir;
            return result;
        }
        
        // Open RPF file with shared access
        std::ifstream rpfFile(rpfPath, std::ios::binary);
        if (!rpfFile)
        {
            result.errorMessage = "Failed to open RPF file";
            return result;
        }
        std::mutex rpfMutex;
        
        // Create thread pool and memory budget
        size_t numThreads = options.numThreads;
        if (numThreads == 0)
        {
            numThreads = std::thread::hardware_concurrency();
            if (numThreads == 0) numThreads = 4;
        }
        
        Install::ThreadPool pool(numThreads);
        Install::MemoryBudget memBudget(options.memoryBudgetMB * 1024 * 1024);
        
        // Progress tracking
        std::atomic<uint64_t> filesExtracted{0};
        std::atomic<uint64_t> bytesExtracted{0};
        std::atomic<uint64_t> errors{0};
        const size_t totalFiles = fileEntries.size();
        
        // Process files in parallel
        std::vector<std::future<void>> futures;
        
        for (const auto& entry : fileEntries)
        {
            futures.push_back(pool.Enqueue([&, entry]()
            {
                // Acquire memory budget
                size_t bufferSize = std::max(entry.size, entry.compressedSize);
                memBudget.Acquire(bufferSize * 2);  // Input + output buffers
                
                try
                {
                    // Read compressed/encrypted data (locked)
                    std::vector<uint8_t> data(entry.compressedSize > 0 ? 
                        entry.compressedSize : entry.size);
                    
                    {
                        std::lock_guard<std::mutex> lock(rpfMutex);
                        rpfFile.seekg(entry.offset);
                        rpfFile.read(reinterpret_cast<char*>(data.data()), data.size());
                    }
                    
                    // Decrypt if needed (unlocked, CPU-bound)
                    if (entry.isEncrypted && !aesKey.empty())
                    {
                        DecryptAES256(data, aesKey);
                    }
                    
                    // Decompress if needed (unlocked, CPU-bound)
                    std::vector<uint8_t> output;
                    if (entry.isCompressed && entry.compressedSize < entry.size)
                    {
                        output = DecompressZlib(data, entry.size);
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
                    
                    // Write output (no lock needed, different files)
                    std::filesystem::path outPath = outputDir / entry.name;
                    std::filesystem::create_directories(outPath.parent_path(), ec);
                    
                    std::ofstream out(outPath, std::ios::binary);
                    out.write(reinterpret_cast<const char*>(output.data()), output.size());
                    
                    filesExtracted++;
                    bytesExtracted += output.size();
                    
                    // Update progress
                    if (progressCallback)
                    {
                        float progress = static_cast<float>(filesExtracted.load()) / 
                                        static_cast<float>(totalFiles);
                        progressCallback(progress);
                    }
                }
                catch (...)
                {
                    errors++;
                }
                
                memBudget.Release(bufferSize * 2);
            }));
        }
        
        // Wait for all tasks
        for (auto& f : futures)
        {
            f.wait();
        }
        
        pool.Wait();
        
        if (progressCallback)
        {
            progressCallback(1.0f);
        }
        
        result.success = errors == 0;
        result.extractedPath = outputDir;
        result.filesExtracted = filesExtracted.load();
        result.bytesExtracted = bytesExtracted.load();
        
        if (errors > 0)
        {
            result.errorMessage = "Failed to extract " + std::to_string(errors.load()) + " files";
        }
        
        return result;
    }
    
    bool IsSparkCliAvailable()
    {
#if defined(_WIN32)
        // Check for SparkCLI.exe in common locations
        std::vector<std::filesystem::path> paths = {
            "SparkCLI.exe",
            "tools/SparkCLI.exe",
            "../SparkIV-master/SRC/SparkCLI/bin/Release/SparkCLI.exe",
        };
#else
        // Check for mono and SparkCLI
        int exitCode = 0;
        std::string monoPath = ExecuteCommand("which mono 2>/dev/null", exitCode);
        if (exitCode != 0 || monoPath.empty())
        {
            return false;
        }
        
        std::vector<std::filesystem::path> paths = {
            "SparkCLI.exe",
            "tools/SparkCLI.exe",
            "../SparkIV-master/SRC/SparkCLI/bin/Release/SparkCLI.exe",
        };
#endif
        
        for (const auto& p : paths)
        {
            if (std::filesystem::exists(p))
            {
                return true;
            }
        }
        
        return false;
    }
    
    ExtractionResult ExtractWithSparkCli(
        const std::filesystem::path& rpfPath,
        const std::filesystem::path& outputDir,
        const std::filesystem::path& keyPath,
        const std::function<void(float)>& progressCallback)
    {
        ExtractionResult result;
        
        // Find SparkCLI
        std::filesystem::path sparkCliPath;
        std::vector<std::filesystem::path> searchPaths = {
            "SparkCLI.exe",
            "tools/SparkCLI.exe",
            "../SparkIV-master/SRC/SparkCLI/bin/Release/SparkCLI.exe",
        };
        
        for (const auto& p : searchPaths)
        {
            if (std::filesystem::exists(p))
            {
                sparkCliPath = p;
                break;
            }
        }
        
        if (sparkCliPath.empty())
        {
            result.errorMessage = "SparkCLI not found";
            return result;
        }
        
        // Build command
        std::ostringstream cmd;
        
#if defined(_WIN32)
        cmd << "\"" << sparkCliPath.string() << "\" ";
#else
        cmd << "mono \"" << sparkCliPath.string() << "\" ";
#endif
        
        cmd << "extract \"" << rpfPath.string() << "\" \"" << outputDir.string() << "\"";
        
        if (!keyPath.empty())
        {
            cmd << " --key \"" << keyPath.string() << "\"";
        }
        
        cmd << " 2>&1";
        
        if (progressCallback)
        {
            progressCallback(0.0f);
        }
        
        // Create output directory
        std::error_code ec;
        std::filesystem::create_directories(outputDir, ec);
        
        // Execute
        int exitCode = 0;
        std::string output = ExecuteCommand(cmd.str(), exitCode);
        
        if (progressCallback)
        {
            progressCallback(1.0f);
        }
        
        if (exitCode != 0)
        {
            result.errorMessage = "SparkCLI failed: " + output;
            return result;
        }
        
        // Count extracted files
        uint64_t fileCount = 0;
        uint64_t byteCount = 0;
        for (const auto& entry : std::filesystem::recursive_directory_iterator(outputDir, ec))
        {
            if (entry.is_regular_file())
            {
                fileCount++;
                byteCount += entry.file_size();
            }
        }
        
        result.success = true;
        result.extractedPath = outputDir;
        result.filesExtracted = fileCount;
        result.bytesExtracted = byteCount;
        
        return result;
    }
}
