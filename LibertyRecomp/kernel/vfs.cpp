#include "vfs.h"
#include <os/logger.h>
#include <algorithm>
#include <cctype>

namespace VFS
{
    static std::filesystem::path g_extractedRoot;
    static bool g_initialized = false;
    static std::mutex g_mutex;
    
    // Path mappings for special cases
    static std::vector<PathMapping> g_pathMappings;
    
    // File index for fast lookups (lowercase normalized path -> actual path)
    static std::unordered_map<std::string, std::filesystem::path> g_fileIndex;
    
    // Statistics
    static Stats g_stats;
    
    void Initialize(const std::filesystem::path& extractedRoot)
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        
        g_extractedRoot = extractedRoot;
        g_initialized = true;
        
        // Set up default path mappings for GTA IV
        ResetPathMappings();
        
        // Build file index
        RebuildIndex();
        
        LOG_UTILITY("[VFS] Initialized with root: {}", extractedRoot.string());
        LOG_UTILITY("[VFS] Indexed {} files, {} directories, {} bytes total",
            g_stats.totalFiles, g_stats.totalDirectories, g_stats.totalBytes);
    }
    
    bool IsInitialized()
    {
        return g_initialized;
    }
    
    std::filesystem::path GetExtractedRoot()
    {
        return g_extractedRoot;
    }
    
    std::string NormalizePath(const std::string& guestPath)
    {
        std::string normalized = guestPath;
        
        // Convert to lowercase
        std::transform(normalized.begin(), normalized.end(), normalized.begin(),
            [](unsigned char c) { return std::tolower(c); });
        
        // Convert backslashes to forward slashes
        std::replace(normalized.begin(), normalized.end(), '\\', '/');
        
        // Remove trailing slashes
        while (!normalized.empty() && normalized.back() == '/')
        {
            normalized.pop_back();
        }
        
        return normalized;
    }
    
    std::string StripDrivePrefix(const std::string& guestPath)
    {
        std::string result = guestPath;
        
        // Find and remove drive prefix (e.g., "game:", "d:", "c:")
        size_t colonPos = result.find(':');
        if (colonPos != std::string::npos)
        {
            result = result.substr(colonPos + 1);
        }
        
        // Remove leading slashes
        while (!result.empty() && (result.front() == '/' || result.front() == '\\'))
        {
            result.erase(0, 1);
        }
        
        return result;
    }
    
    std::filesystem::path Resolve(const std::string& guestPath)
    {
        if (!g_initialized)
        {
            return {};
        }
        
        std::string normalized = NormalizePath(guestPath);
        std::string stripped = StripDrivePrefix(normalized);
        
        // Check path mappings first
        for (const auto& mapping : g_pathMappings)
        {
            std::string mappingNorm = NormalizePath(mapping.guestPrefix);
            if (stripped.find(mappingNorm) == 0 || normalized.find(mappingNorm) != std::string::npos)
            {
                // Found a mapping - replace prefix
                std::string remainder;
                if (stripped.length() > mappingNorm.length())
                {
                    remainder = stripped.substr(mappingNorm.length());
                    if (!remainder.empty() && remainder.front() == '/')
                    {
                        remainder.erase(0, 1);
                    }
                }
                
                std::filesystem::path resolved = g_extractedRoot / mapping.hostPrefix;
                if (!remainder.empty())
                {
                    resolved /= remainder;
                }
                
                // Check if exists
                std::error_code ec;
                if (std::filesystem::exists(resolved, ec))
                {
                    g_stats.cacheHits++;
                    return resolved;
                }
            }
        }
        
        // Try direct path resolution
        std::filesystem::path directPath = g_extractedRoot / stripped;
        std::error_code ec;
        if (std::filesystem::exists(directPath, ec))
        {
            g_stats.cacheHits++;
            return directPath;
        }
        
        // Try file index lookup (case-insensitive)
        {
            std::lock_guard<std::mutex> lock(g_mutex);
            auto it = g_fileIndex.find(stripped);
            if (it != g_fileIndex.end())
            {
                g_stats.cacheHits++;
                return it->second;
            }
        }
        
        g_stats.cacheMisses++;
        return {};
    }
    
    bool Exists(const std::string& guestPath)
    {
        auto resolved = Resolve(guestPath);
        if (resolved.empty())
        {
            return false;
        }
        
        std::error_code ec;
        return std::filesystem::exists(resolved, ec);
    }
    
    bool IsDirectory(const std::string& guestPath)
    {
        auto resolved = Resolve(guestPath);
        if (resolved.empty())
        {
            return false;
        }
        
        std::error_code ec;
        return std::filesystem::is_directory(resolved, ec);
    }
    
    uint64_t GetFileSize(const std::string& guestPath)
    {
        auto resolved = Resolve(guestPath);
        if (resolved.empty())
        {
            return 0;
        }
        
        std::error_code ec;
        if (std::filesystem::is_regular_file(resolved, ec))
        {
            return std::filesystem::file_size(resolved, ec);
        }
        
        return 0;
    }
    
    void AddPathMapping(const std::string& guestPrefix, const std::string& hostPrefix)
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        g_pathMappings.push_back({guestPrefix, hostPrefix});
    }
    
    void ResetPathMappings()
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        g_pathMappings.clear();
        
        // Default GTA IV path mappings
        // These map special Xbox 360 paths to their extracted locations
        
        // Shader paths - "fxl_final\" is inside common.rpf
        g_pathMappings.push_back({"fxl_final", "common/shaders/fxl_final"});
        g_pathMappings.push_back({"shaders/fxl_final", "common/shaders/fxl_final"});
        
        // RPF roots - when game opens "common.rpf", serve from extracted "common/"
        g_pathMappings.push_back({"common.rpf", "common"});
        g_pathMappings.push_back({"xbox360.rpf", "xbox360"});
        g_pathMappings.push_back({"audio.rpf", "audio"});
        
        // Common paths
        g_pathMappings.push_back({"common/", "common/"});
        g_pathMappings.push_back({"data/", "common/data/"});
        g_pathMappings.push_back({"text/", "common/text/"});
    }
    
    void RebuildIndex()
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        
        g_fileIndex.clear();
        g_stats = Stats{};
        
        if (!g_initialized || g_extractedRoot.empty())
        {
            return;
        }
        
        std::error_code ec;
        if (!std::filesystem::exists(g_extractedRoot, ec))
        {
            LOG_WARNING("[VFS] Extracted root does not exist: {}", g_extractedRoot.string());
            return;
        }
        
        // Recursively scan the extracted directory
        for (const auto& entry : std::filesystem::recursive_directory_iterator(g_extractedRoot, ec))
        {
            if (ec)
            {
                continue;
            }
            
            // Get relative path from extracted root
            std::filesystem::path relativePath = entry.path().lexically_relative(g_extractedRoot);
            std::string normalizedKey = NormalizePath(relativePath.string());
            
            if (entry.is_directory(ec))
            {
                g_stats.totalDirectories++;
            }
            else if (entry.is_regular_file(ec))
            {
                g_stats.totalFiles++;
                g_stats.totalBytes += entry.file_size(ec);
            }
            
            // Add to index
            g_fileIndex[normalizedKey] = entry.path();
        }
    }
    
    Stats GetStats()
    {
        return g_stats;
    }
}
