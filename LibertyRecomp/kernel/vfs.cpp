#include "vfs.h"
#include "button_prompts.h"
#include <os/logger.h>
#include <user/config.h>
#include <user/paths.h>
#include <hid/hid.h>
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

    // GTA IV file extensions to try when path doesn't include extension
    static const std::vector<std::string> g_gtaExtensions = {
        ".xtd", ".wtd", ".xbd", ".wbd", ".xdr", ".wdr", 
        ".xft", ".wft", ".xad", ".wad", ".xmd", ".wmd",
        ".xcd", ".wcd", ".xpd", ".wpd", ".xnd", ".wnd",
        ".xvd", ".wvd", ".xdd", ".wdd", ".xld", ".wld"
    };

    
    void Initialize(const std::filesystem::path& extractedRoot)
    {
        printf("[VFS] Initialize called with: %s\n", extractedRoot.string().c_str()); fflush(stdout);
        
        std::lock_guard<std::mutex> lock(g_mutex);
        printf("[VFS] Got mutex lock\n"); fflush(stdout);
        
        g_extractedRoot = extractedRoot;
        g_initialized = true;
        
        // Set up default path mappings for GTA IV
        printf("[VFS] Calling ResetPathMappings...\n"); fflush(stdout);
        ResetPathMappings();
        printf("[VFS] ResetPathMappings done\n"); fflush(stdout);
        
        // Build file index
        printf("[VFS] Calling RebuildIndex...\n"); fflush(stdout);
        RebuildIndex();
        printf("[VFS] RebuildIndex done\n"); fflush(stdout);
        
        LOGF_UTILITY("[VFS] Initialized with root: {}", extractedRoot.string());
        LOGF_UTILITY("[VFS] Indexed {} files, {} directories, {} bytes total",
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
    
    // Comprehensive VFS file request logging - logs ALL requests
    static int s_resolveCount = 0;
    
    std::filesystem::path Resolve(const std::string& guestPath)
    {
        if (!g_initialized)
        {
            return {};
        }
        
        std::string normalized = NormalizePath(guestPath);
        std::string stripped = StripDrivePrefix(normalized);
        
        ++s_resolveCount;
        
        // COMPREHENSIVE LOGGING: Log ALL file requests with full details
        // This helps trace exactly what files the game is requesting
        printf("[VFS] Resolve #%d: '%s' -> normalized='%s' stripped='%s'\n", 
               s_resolveCount, guestPath.c_str(), normalized.c_str(), stripped.c_str());
        fflush(stdout);
        
        // DEBUG: Check if this is sounds.dat and trace every step
        bool isDebugPath = (guestPath.find("sounds.dat") != std::string::npos);
        if (isDebugPath) {
            printf("[VFS] DEBUG sounds.dat: Starting path mapping loop, %zu mappings\n", g_pathMappings.size());
            fflush(stdout);
        }
        
        // === BUTTON PROMPT DYNAMIC SWITCHING ===
        // Intercept requests for buttons_360.xtd and redirect to controller-specific version
        // This enables dynamic button prompts based on detected controller type
        if (stripped.find("textures/buttons_360") != std::string::npos ||
            stripped.find("buttons_360.xtd") != std::string::npos)
        {
            std::filesystem::path buttonPromptPath = ButtonPrompts::GetCurrentButtonPromptsPath();
            
            if (!buttonPromptPath.empty())
            {
                std::string platform = ButtonPrompts::GetCurrentPlatformName();
                printf("[VFS] BUTTON_PROMPT: Using %s prompts from '%s'\n", 
                       platform.c_str(), buttonPromptPath.string().c_str());
                fflush(stdout);
                g_stats.cacheHits++;
                return buttonPromptPath;
            }
            
            // If ButtonPrompts module failed, continue to normal resolution (original game file)
            printf("[VFS] BUTTON_PROMPT: Module returned empty path, falling back to default resolution\n");
            fflush(stdout);
        }
        
        // Check path mappings first
        for (const auto& mapping : g_pathMappings)
        {
            std::string mappingNorm = NormalizePath(mapping.guestPrefix);
            
            // Check if mapping matches via stripped path (prefix already removed) or normalized path (full path)
            bool matchedViaStripped = (stripped.find(mappingNorm) == 0);
            bool matchedViaNormalized = (normalized.find(mappingNorm) != std::string::npos);
            
            if (matchedViaStripped || matchedViaNormalized)
            {
                if (isDebugPath) {
                    printf("[VFS] DEBUG sounds.dat: MATCHED mapping '%s' -> '%s' (via %s)\n", 
                           mapping.guestPrefix.c_str(), mapping.hostPrefix.c_str(),
                           matchedViaStripped ? "stripped" : "normalized");
                    fflush(stdout);
                }
                
                // Found a mapping - replace prefix
                // BUG FIX: Handle the two match cases differently
                std::string remainder;
                if (matchedViaStripped)
                {
                    // Match was in stripped path - use stripped for remainder calculation
                    if (stripped.length() > mappingNorm.length())
                    {
                        remainder = stripped.substr(mappingNorm.length());
                    }
                }
                else
                {
                    // Match was in normalized path (e.g., "platform:/stream.ini" matched "platform:")
                    // The stripped path already has the drive prefix removed, so use stripped directly as remainder
                    // For "platform:/stream.ini": normalized="platform:/stream.ini", stripped="stream.ini"
                    // We want remainder="stream.ini" (the full stripped path)
                    remainder = stripped;
                }
                
                // Clean up leading slash if present
                if (!remainder.empty() && remainder.front() == '/')
                {
                    remainder.erase(0, 1);
                }
                
                std::filesystem::path resolved = g_extractedRoot / mapping.hostPrefix;
                if (!remainder.empty())
                {
                    resolved /= remainder;
                }
                
                if (isDebugPath) {
                    printf("[VFS] DEBUG sounds.dat: Checking exists for '%s'\n", resolved.string().c_str());
                    fflush(stdout);
                }
                
                // Check if exists
                std::error_code ec;
                if (std::filesystem::exists(resolved, ec))
                {
                    g_stats.cacheHits++;
                    printf("[VFS] FOUND via mapping: '%s' -> '%s'\n", guestPath.c_str(), resolved.string().c_str());
                    fflush(stdout);
                    return resolved;
                }
                
                if (isDebugPath) {
                    printf("[VFS] DEBUG sounds.dat: exists returned false, ec=%d\n", ec.value());
                    fflush(stdout);
                }
                
                // Try with GTA IV extensions
                for (const auto& ext : g_gtaExtensions)
                {
                    std::filesystem::path withExt = resolved;
                    withExt += ext;
                    if (std::filesystem::exists(withExt, ec))
                    {
                        g_stats.cacheHits++;
                        return withExt;
                    }
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
        
        // Try with GTA IV extensions if path has no extension
        if (stripped.find('.') == std::string::npos || 
            stripped.rfind('.') < stripped.rfind('/'))
        {
            for (const auto& ext : g_gtaExtensions)
            {
                std::filesystem::path withExt = g_extractedRoot / (stripped + ext);
                std::error_code ec;
                if (std::filesystem::exists(withExt, ec))
                {
                    g_stats.cacheHits++;
                    return withExt;
                }
                
                // Also try in file index
                std::string indexKey = stripped + ext;
                std::lock_guard<std::mutex> lock(g_mutex);
                auto it = g_fileIndex.find(indexKey);
                if (it != g_fileIndex.end())
                {
                    g_stats.cacheHits++;
                    return it->second;
                }
            }
        }
        
        g_stats.cacheMisses++;
        printf("[VFS] NOT FOUND: '%s' (stripped='%s')\n", guestPath.c_str(), stripped.c_str());
        fflush(stdout);
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
        // Note: caller (Initialize) already holds g_mutex, don't lock again
        // std::lock_guard<std::mutex> lock(g_mutex);
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
        
        // Common paths - game uses "common:/" prefix for common.rpf contents
        // e.g., "common:/data/version.txt" -> "common/data/version.txt"
        g_pathMappings.push_back({"common:", "common"});
        g_pathMappings.push_back({"common:/", "common/"});
        g_pathMappings.push_back({"common/", "common/"});
        g_pathMappings.push_back({"data/", "common/data/"});
        g_pathMappings.push_back({"text/", "common/text/"});
        
        // Audio paths - audio config files are stored in audio/config/
        // Game requests "platform:/GTA/build/config/..." which strips to "gta/build/config/..."
        // Also requests "platform:/GTA/build/engineSettings.xml" -> "gta/build/enginesettings.xml"
        // These need to map to audio/config/
        g_pathMappings.push_back({"gta/build/config/", "audio/config/"});
        g_pathMappings.push_back({"gta/build/enginesettings.xml", "audio/config/enginesettings.xml"});
        g_pathMappings.push_back({"config/", "audio/config/"});
        g_pathMappings.push_back({"audio/", "audio/"});
        
        // Platform-specific paths (platform: → xbox360/)
        // GTA IV uses "platform:/textures/fonts" etc. for Xbox 360 platform assets
        // Root platform: prefix mappings (must come before subdirectory mappings)
        g_pathMappings.push_back({"platform:", "xbox360"});
        g_pathMappings.push_back({"platform:/", "xbox360/"});
        
        // Subdirectory mappings (more specific paths)
        g_pathMappings.push_back({"textures", "xbox360/textures"});
        g_pathMappings.push_back({"models", "xbox360/models"});
        g_pathMappings.push_back({"anim", "xbox360/anim"});
    }
    
    void RebuildIndex()
    {
        // Note: caller (Initialize) already holds g_mutex, don't lock again
        // std::lock_guard<std::mutex> lock(g_mutex);
        
        g_fileIndex.clear();
        g_stats = Stats{};
        
        if (!g_initialized || g_extractedRoot.empty())
        {
            return;
        }
        
        std::error_code ec;
        if (!std::filesystem::exists(g_extractedRoot, ec))
        {
            LOGF_WARNING("[VFS] Extracted root does not exist: {}", g_extractedRoot.string());
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
