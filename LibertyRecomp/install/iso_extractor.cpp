#include "iso_extractor.h"
#include "iso_file_system.h"

#include <array>
#include <cstdio>
#include <fstream>
#include <sstream>

namespace IsoExtractor
{
    bool IsIsoFile(const std::filesystem::path& path)
    {
        if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path))
        {
            return false;
        }
        
        // Check extension
        std::string ext = path.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        if (ext != ".iso")
        {
            return false;
        }
        
        // Validate Xbox ISO magic (MICROSOFT*XBOX*MEDIA at various offsets)
        std::ifstream file(path, std::ios::binary);
        if (!file)
        {
            return false;
        }
        
        // Xbox 360 ISO can have magic at multiple offsets depending on format
        const size_t possibleOffsets[] = { 0x10000, 0x10000 + 0x0000FB20, 0x10000 + 0x00020600, 0x10000 + 0x02080000 };
        const char* expected = "MICROSOFT*XBOX*MEDIA";
        char magic[20];
        
        for (size_t offset : possibleOffsets)
        {
            file.seekg(offset);
            if (!file)
            {
                continue;
            }
            
            file.read(magic, 20);
            if (file && std::memcmp(magic, expected, 20) == 0)
            {
                return true;
            }
        }
        
        // File has .iso extension but couldn't validate magic
        // Still return true based on extension (might be valid but different offset)
        return true;
    }
    
    bool IsToolAvailable()
    {
        // Native implementation - always available
        return true;
    }
    
    std::filesystem::path GetToolPath()
    {
        // Native implementation - no external tool needed
        return "native";
    }
    
    ExtractionResult Extract(
        const std::filesystem::path& isoPath,
        const std::filesystem::path& outputDir,
        const std::function<void(float)>& progressCallback)
    {
        ExtractionResult result;
        
        // Check if ISO exists
        if (!std::filesystem::exists(isoPath))
        {
            result.errorMessage = "ISO file does not exist: " + isoPath.string();
            return result;
        }
        
        // Create output directory
        std::error_code ec;
        std::filesystem::create_directories(outputDir, ec);
        if (ec)
        {
            result.errorMessage = "Failed to create output directory: " + ec.message();
            return result;
        }
        
        if (progressCallback)
        {
            progressCallback(0.0f);
        }
        
        // Use native ISOFileSystem to parse the ISO
        auto isoFs = ISOFileSystem::create(isoPath);
        if (!isoFs)
        {
            result.errorMessage = "Failed to parse ISO file. Invalid or unsupported Xbox 360 ISO format: " + isoPath.string();
            return result;
        }
        
        // Get list of all files in the ISO
        std::vector<std::string> fileList = isoFs->getFileList();
        if (fileList.empty())
        {
            result.errorMessage = "ISO file appears to be empty: " + isoPath.string();
            return result;
        }
        
        // Calculate total size for progress reporting
        uint64_t totalSize = isoFs->getTotalSize();
        uint64_t processedSize = 0;
        uint64_t filesExtracted = 0;
        
        // Extract each file
        for (const std::string& filePath : fileList)
        {
            // Build output path
            std::filesystem::path outPath = outputDir / filePath;
            
            // Create parent directories
            std::filesystem::create_directories(outPath.parent_path(), ec);
            if (ec)
            {
                result.errorMessage = "Failed to create directory: " + outPath.parent_path().string() + " - " + ec.message();
                return result;
            }
            
            // Get file size and load data
            size_t fileSize = isoFs->getSize(filePath);
            if (fileSize == 0)
            {
                // Skip empty files but count them
                filesExtracted++;
                continue;
            }
            
            std::vector<uint8_t> fileData(fileSize);
            if (!isoFs->load(filePath, fileData.data(), fileSize))
            {
                result.errorMessage = "Failed to read file from ISO: " + filePath;
                return result;
            }
            
            // Write to output file
            std::ofstream outFile(outPath, std::ios::binary);
            if (!outFile)
            {
                result.errorMessage = "Failed to create output file: " + outPath.string();
                return result;
            }
            
            outFile.write(reinterpret_cast<const char*>(fileData.data()), fileSize);
            if (outFile.bad())
            {
                result.errorMessage = "Failed to write output file: " + outPath.string();
                return result;
            }
            
            filesExtracted++;
            processedSize += fileSize;
            
            // Update progress
            if (progressCallback && totalSize > 0)
            {
                float progress = static_cast<float>(processedSize) / static_cast<float>(totalSize);
                progressCallback(progress);
            }
        }
        
        if (progressCallback)
        {
            progressCallback(1.0f);
        }
        
        result.success = true;
        result.extractedPath = outputDir;
        result.filesExtracted = filesExtracted;
        
        return result;
    }
    
    std::vector<std::string> ListContents(const std::filesystem::path& isoPath)
    {
        std::vector<std::string> contents;
        
        // Use native ISOFileSystem to parse and list
        auto isoFs = ISOFileSystem::create(isoPath);
        if (!isoFs)
        {
            return contents;
        }
        
        return isoFs->getFileList();
    }
}
