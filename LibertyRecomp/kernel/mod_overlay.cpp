#include "mod_overlay.h"
#include "io/img_loader.h"
#include "io/texture_convert.h"
#include <os/logger.h>
#include <fmt/format.h>
#include <algorithm>
#include <cctype>

namespace ModOverlay
{
    static std::filesystem::path g_gameRoot;
    static bool g_initialized = false;
    static std::mutex g_mutex;
    
    // Registered overlay directories (sorted by priority, highest first)
    static std::vector<OverlayEntry> g_overlays;
    
    // File index: normalized path -> FileOverride
    static std::unordered_map<std::string, FileOverride> g_fileIndex;
    
    // Statistics
    static Stats g_stats;

    // Normalize path for comparison (lowercase, forward slashes)
    static std::string NormalizePath(const std::string& path)
    {
        std::string normalized = path;
        std::transform(normalized.begin(), normalized.end(), normalized.begin(),
            [](unsigned char c) { return std::tolower(c); });
        std::replace(normalized.begin(), normalized.end(), '\\', '/');
        
        // Remove leading slashes
        while (!normalized.empty() && normalized.front() == '/')
        {
            normalized.erase(0, 1);
        }
        
        // Remove trailing slashes
        while (!normalized.empty() && normalized.back() == '/')
        {
            normalized.pop_back();
        }
        
        return normalized;
    }

    // Sort overlays by priority (highest first)
    static void SortOverlays()
    {
        std::sort(g_overlays.begin(), g_overlays.end(),
            [](const OverlayEntry& a, const OverlayEntry& b) {
                return a.priority > b.priority;
            });
    }

    void Initialize(const std::filesystem::path& gameRoot)
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        
        g_gameRoot = gameRoot;
        g_initialized = true;
        g_overlays.clear();
        g_fileIndex.clear();
        g_stats = Stats{};
        
        LOGF_UTILITY("[ModOverlay] Initialized with game root: {}", gameRoot.string());
        
        // Scan for FusionFix-style update folders
        ScanForFusionFix(gameRoot);
        
        // Build file index
        RebuildIndex();
    }

    bool IsInitialized()
    {
        return g_initialized;
    }

    void AddOverlay(const std::filesystem::path& path, int priority, const std::string& name)
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        
        std::error_code ec;
        if (!std::filesystem::exists(path, ec) || !std::filesystem::is_directory(path, ec))
        {
            LOGF_WARNING("[ModOverlay] Overlay path does not exist or is not a directory: {}", path.string());
            return;
        }
        
        // Check if already registered
        for (const auto& overlay : g_overlays)
        {
            if (std::filesystem::equivalent(overlay.path, path, ec))
            {
                LOGF_WARNING("[ModOverlay] Overlay already registered: {}", path.string());
                return;
            }
        }
        
        OverlayEntry entry;
        entry.path = path;
        entry.priority = priority;
        entry.enabled = true;
        entry.name = name.empty() ? path.filename().string() : name;
        
        g_overlays.push_back(entry);
        SortOverlays();
        
        g_stats.totalOverlays++;
        g_stats.enabledOverlays++;
        
        LOGF_UTILITY("[ModOverlay] Added overlay: {} (priority={}, name={})", 
            path.string(), priority, entry.name);
    }

    void RemoveOverlay(const std::filesystem::path& path)
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        
        std::error_code ec;
        auto it = std::remove_if(g_overlays.begin(), g_overlays.end(),
            [&path, &ec](const OverlayEntry& entry) {
                return std::filesystem::equivalent(entry.path, path, ec);
            });
        
        if (it != g_overlays.end())
        {
            g_overlays.erase(it, g_overlays.end());
            g_stats.totalOverlays--;
            LOGF_UTILITY("[ModOverlay] Removed overlay: {}", path.string());
        }
    }

    void ClearOverlays()
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        g_overlays.clear();
        g_fileIndex.clear();
        g_stats = Stats{};
    }

    std::vector<OverlayEntry> GetOverlays()
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        return g_overlays;
    }

    void SetOverlayEnabled(const std::filesystem::path& path, bool enabled)
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        
        std::error_code ec;
        for (auto& overlay : g_overlays)
        {
            if (std::filesystem::equivalent(overlay.path, path, ec))
            {
                if (overlay.enabled != enabled)
                {
                    overlay.enabled = enabled;
                    if (enabled)
                        g_stats.enabledOverlays++;
                    else
                        g_stats.enabledOverlays--;
                    
                    LOGF_UTILITY("[ModOverlay] Overlay {} {}", 
                        overlay.name, enabled ? "enabled" : "disabled");
                }
                return;
            }
        }
    }

    void RebuildIndex()
    {
        // Note: Caller should hold mutex if needed
        g_fileIndex.clear();
        g_stats.totalOverrideFiles = 0;
        
        // Process overlays in priority order (highest first)
        // Later entries in map will NOT override earlier ones
        // So we process highest priority first to ensure they "win"
        for (const auto& overlay : g_overlays)
        {
            if (!overlay.enabled)
                continue;
            
            std::error_code ec;
            if (!std::filesystem::exists(overlay.path, ec))
                continue;
            
            // Recursively scan overlay directory
            for (const auto& entry : std::filesystem::recursive_directory_iterator(overlay.path, ec))
            {
                if (ec || !entry.is_regular_file(ec))
                    continue;
                
                // Get relative path from overlay root
                std::filesystem::path relativePath = entry.path().lexically_relative(overlay.path);
                std::string normalizedKey = NormalizePath(relativePath.string());
                
                // Apply FusionFix path mapping
                normalizedKey = MapFusionFixPath(normalizedKey);
                
                // Only add if not already in index (higher priority overlay already added it)
                if (g_fileIndex.find(normalizedKey) == g_fileIndex.end())
                {
                    FileOverride override;
                    override.hostPath = entry.path();
                    override.normalizedPath = normalizedKey;
                    override.overlayPriority = overlay.priority;
                    
                    g_fileIndex[normalizedKey] = override;
                    g_stats.totalOverrideFiles++;
                }
            }
        }
        
        ScanForImgFolders();
        
        LOGF_UTILITY("[ModOverlay] Index rebuilt: {} override files from {} overlays",
            g_stats.totalOverrideFiles, g_stats.enabledOverlays);
    }

    std::filesystem::path Resolve(const std::string& normalizedPath)
    {
        if (!g_initialized || g_fileIndex.empty())
        {
            return {};
        }
        
        std::string key = NormalizePath(normalizedPath);
        
        std::lock_guard<std::mutex> lock(g_mutex);
        auto it = g_fileIndex.find(key);
        if (it != g_fileIndex.end())
        {
            g_stats.overrideHits++;
            return it->second.hostPath;
        }
        
        // Also try with FusionFix path mapping applied
        std::string mappedKey = MapFusionFixPath(key);
        if (mappedKey != key)
        {
            it = g_fileIndex.find(mappedKey);
            if (it != g_fileIndex.end())
            {
                g_stats.overrideHits++;
                return it->second.hostPath;
            }
        }
        
        // === TEXTURE AUTO-CONVERSION ===
        // If requesting Xbox texture (.xtd) but PC texture (.wtd) exists, convert it
        if (key.size() > 4)
        {
            std::string ext = key.substr(key.size() - 4);
            if (ext == ".xtd" || ext == ".xdr" || ext == ".xft" || ext == ".xbd")
            {
                // Try PC equivalent
                std::string pcKey = key.substr(0, key.size() - 4);
                if (ext == ".xtd") pcKey += ".wtd";
                else if (ext == ".xdr") pcKey += ".wdr";
                else if (ext == ".xft") pcKey += ".wft";
                else if (ext == ".xbd") pcKey += ".wbd";
                
                auto pcIt = g_fileIndex.find(pcKey);
                if (pcIt != g_fileIndex.end())
                {
                    // Found PC texture - convert and cache
                    auto converted = ConvertAndCacheTexture(pcIt->second.hostPath, ext);
                    if (!converted.empty())
                    {
                        g_stats.overrideHits++;
                        return converted;
                    }
                }
            }
        }
        
        g_stats.overrideMisses++;
        return {};
    }

    bool HasOverride(const std::string& normalizedPath)
    {
        return !Resolve(normalizedPath).empty();
    }

    std::vector<FileOverride> GetAllOverrides()
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        
        std::vector<FileOverride> overrides;
        overrides.reserve(g_fileIndex.size());
        
        for (const auto& [key, value] : g_fileIndex)
        {
            overrides.push_back(value);
        }
        
        return overrides;
    }

    Stats GetStats()
    {
        return g_stats;
    }

    void DumpStatus()
    {
        LOGF_UTILITY("=== MOD OVERLAY STATUS ===");
        LOGF_UTILITY("Game Root: {}", g_gameRoot.string());
        LOGF_UTILITY("Initialized: {}", g_initialized ? "YES" : "NO");
        LOGF_UTILITY("Total Overlays: {} ({} enabled)", g_stats.totalOverlays, g_stats.enabledOverlays);
        LOGF_UTILITY("Override Files: {}", g_stats.totalOverrideFiles);
        LOGF_UTILITY("Hits: {}, Misses: {}", g_stats.overrideHits, g_stats.overrideMisses);
        
        LOGF_UTILITY("Registered Overlays:");
        for (const auto& overlay : g_overlays)
        {
            LOGF_UTILITY("  [{}] {} (priority={}, path={})",
                overlay.enabled ? "ON" : "OFF",
                overlay.name,
                overlay.priority,
                overlay.path.string());
        }
        
        if (g_stats.totalOverrideFiles <= 50)
        {
            LOGF_UTILITY("Override Files:");
            for (const auto& [key, value] : g_fileIndex)
            {
                LOGF_UTILITY("  {} -> {}", key, value.hostPath.string());
            }
        }
        else
        {
            LOGF_UTILITY("(Too many override files to list - {} total)", g_stats.totalOverrideFiles);
        }
    }

    void ScanForFusionFix(const std::filesystem::path& gameRoot)
    {
        std::error_code ec;
        
        // FusionFix standard locations (in priority order, highest first)
        std::vector<std::pair<std::filesystem::path, int>> searchPaths = {
            { gameRoot / "mods" / "update", 100 },                    // Highest priority: mods/update/
            { gameRoot / "update", 50 },                               // Standard FusionFix location
            { gameRoot / "GTAIV.EFLC.FusionFix" / "update", 40 },     // Alternative FusionFix location
            { gameRoot / "plugins" / "update", 30 },                   // Plugins folder
            { gameRoot / "mods", 20 },                                 // Generic mods folder
        };
        
        for (const auto& [path, priority] : searchPaths)
        {
            if (std::filesystem::exists(path, ec) && std::filesystem::is_directory(path, ec))
            {
                // Check if this looks like a FusionFix update folder
                // FusionFix update folders typically have: common/, pc/, TLAD/, TBoGT/, etc.
                bool hasFusionFixStructure = 
                    std::filesystem::exists(path / "common", ec) ||
                    std::filesystem::exists(path / "pc", ec) ||
                    std::filesystem::exists(path / "TLAD", ec) ||
                    std::filesystem::exists(path / "TBoGT", ec) ||
                    std::filesystem::exists(path / "GTAIV.EFLC.FusionFix", ec);
                
                // Register regardless, but note if it has FusionFix structure
                std::string name = path.filename().string();
                if (path.parent_path().filename() == "mods")
                    name = "mods/" + name;
                
                if (hasFusionFixStructure)
                    name += " (FusionFix)";
                
                // Don't lock here, we're being called from Initialize which already holds the lock
                OverlayEntry entry;
                entry.path = path;
                entry.priority = priority;
                entry.enabled = true;
                entry.name = name;
                
                g_overlays.push_back(entry);
                g_stats.totalOverlays++;
                g_stats.enabledOverlays++;
                
                LOGF_UTILITY("[ModOverlay] Found FusionFix-style folder: {} (priority={})", 
                    path.string(), priority);
            }
        }
        
        SortOverlays();
    }

    std::string MapFusionFixPath(const std::string& fusionFixPath)
    {
        std::string result = fusionFixPath;
        
        // FusionFix uses specific folder names that map to game paths:
        
        // update/common/* -> common/*
        // (already correct, no change needed)
        
        // update/pc/* -> pc/* (PC-specific assets)
        // For Xbox 360 recomp, we might want to map pc/ to xbox360/ or ignore
        // For now, keep as-is to support PC-style mods
        
        // update/TLAD/* -> dlc/TLAD/*
        if (result.find("tlad/") == 0)
        {
            result = "dlc/" + result;
        }
        else if (result.find("tbogt/") == 0)
        {
            result = "dlc/" + result;
        }
        
        // update/GTAIV.EFLC.FusionFix/* -> various locations
        // This is FusionFix's own data, usually contains:
        //   - plugins/  -> plugins
        //   - shaders/  -> common/shaders
        //   - textures/ -> xbox360/textures
        if (result.find("gtaiv.eflc.fusionfix/") == 0)
        {
            std::string subpath = result.substr(21); // Remove "gtaiv.eflc.fusionfix/"
            
            if (subpath.find("shaders/") == 0)
            {
                result = "common/" + subpath;
            }
            else if (subpath.find("textures/") == 0)
            {
                result = "xbox360/" + subpath;
            }
            else if (subpath.find("models/") == 0)
            {
                result = "xbox360/" + subpath;
            }
            // Otherwise keep as-is
        }
        
        // update/update/* -> (strip redundant update/)
        if (result.find("update/") == 0)
        {
            result = result.substr(7);
        }
        
        return result;
    }
}

// ============================================================================
// IMG Folder Detection for FusionFix-style IMG loading
// ============================================================================

namespace ModOverlay
{
    // Track IMG folders that have replacements
    static std::unordered_map<std::string, std::filesystem::path> g_imgFolders;

    /**
     * Scan overlays for .img folders (FusionFix-style IMG loading).
     * FusionFix allows placing replacement files in folders named like the IMG archive:
     *   update/pc/models/cdimages/vehicles.img/replacement.xtd
     * This overrides files inside the vehicles.img archive.
     */
    void ScanForImgFolders()
    {
        g_imgFolders.clear();

        for (const auto& overlay : g_overlays)
        {
            if (!overlay.enabled)
                continue;

            std::error_code ec;
            if (!std::filesystem::exists(overlay.path, ec))
                continue;

            // Recursively find directories ending with .img
            for (const auto& entry : std::filesystem::recursive_directory_iterator(overlay.path, ec))
            {
                if (ec)
                    continue;

                if (entry.is_directory(ec))
                {
                    std::string dirName = entry.path().filename().string();
                    std::string lowerName = dirName;
                    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

                    // Check if directory ends with .img
                    if (lowerName.size() > 4 && lowerName.substr(lowerName.size() - 4) == ".img")
                    {
                        // Get relative path from overlay
                        std::filesystem::path relativePath = entry.path().lexically_relative(overlay.path);
                        std::string normalizedKey = NormalizePath(relativePath.string());

                        // Only add if not already found (higher priority overlay wins)
                        if (g_imgFolders.find(normalizedKey) == g_imgFolders.end())
                        {
                            g_imgFolders[normalizedKey] = entry.path();
                            LOGF_UTILITY("[ModOverlay] Found IMG folder: {} -> {}",
                                normalizedKey, entry.path().string());
                        }
                    }
                }
            }
        }

        LOGF_UTILITY("[ModOverlay] Scanned {} IMG replacement folders", g_imgFolders.size());
    }

    /**
     * Check if an IMG file has a replacement folder.
     * 
     * @param imgPath Normalized path to IMG (e.g., "pc/models/cdimages/vehicles.img")
     * @return Path to replacement folder, or empty if none
     */
    std::filesystem::path GetImgReplacementFolder(const std::string& imgPath)
    {
        std::string key = NormalizePath(imgPath);
        
        auto it = g_imgFolders.find(key);
        if (it != g_imgFolders.end())
        {
            return it->second;
        }

        // Also try with FusionFix path mapping
        std::string mappedKey = MapFusionFixPath(key);
        if (mappedKey != key)
        {
            it = g_imgFolders.find(mappedKey);
            if (it != g_imgFolders.end())
            {
                return it->second;
            }
        }

        return {};
    }

    /**
     * Get all IMG folders with replacements.
     */
    std::vector<std::pair<std::string, std::filesystem::path>> GetAllImgFolders()
    {
        std::vector<std::pair<std::string, std::filesystem::path>> result;
        result.reserve(g_imgFolders.size());

        for (const auto& [key, path] : g_imgFolders)
        {
            result.emplace_back(key, path);
        }

        return result;
    }
}

// ============================================================================
// Texture Auto-Conversion (PC .wtd -> Xbox .xtd)
// ============================================================================

namespace ModOverlay
{
    // Cache for converted textures
    static std::unordered_map<std::string, std::filesystem::path> g_convertedTextureCache;

    /**
     * Convert a PC texture to Xbox 360 format and cache it.
     * 
     * @param pcTexturePath Path to PC texture file
     * @param targetExt Target extension (.xtd, .xdr, etc.)
     * @return Path to converted file, or empty on failure
     */
    std::filesystem::path ConvertAndCacheTexture(
        const std::filesystem::path& pcTexturePath,
        const std::string& targetExt)
    {
        // Check cache first
        std::string cacheKey = pcTexturePath.string() + targetExt;
        auto cacheIt = g_convertedTextureCache.find(cacheKey);
        if (cacheIt != g_convertedTextureCache.end())
        {
            std::error_code ec;
            if (std::filesystem::exists(cacheIt->second, ec))
            {
                return cacheIt->second;
            }
        }

        // Read PC texture
        std::ifstream file(pcTexturePath, std::ios::binary | std::ios::ate);
        if (!file)
        {
            return {};
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        file.seekg(0, std::ios::beg);

        std::vector<uint8_t> pcData(fileSize);
        if (!file.read(reinterpret_cast<char*>(pcData.data()), fileSize))
        {
            return {};
        }

        // Convert to Xbox 360 format
        auto xboxData = TextureConvert::ConvertPCToXbox360(pcData);
        if (!xboxData.has_value() || xboxData->empty())
        {
            // Conversion failed - try to use as-is (some files may be compatible)
            xboxData = pcData;
        }

        // Write to cache directory
        std::filesystem::path cacheDir = std::filesystem::temp_directory_path() / 
                                         "LibertyRecomp" / "texture_cache";
        std::error_code ec;
        std::filesystem::create_directories(cacheDir, ec);

        // Create unique filename based on original path
        std::hash<std::string> hasher;
        std::string cacheName = std::to_string(hasher(pcTexturePath.string())) + targetExt;
        std::filesystem::path cachePath = cacheDir / cacheName;

        std::ofstream outFile(cachePath, std::ios::binary);
        if (!outFile)
        {
            return {};
        }

        outFile.write(reinterpret_cast<const char*>(xboxData->data()), xboxData->size());
        
        // Add to cache
        g_convertedTextureCache[cacheKey] = cachePath;

        LOGF_UTILITY("[ModOverlay] Converted PC texture: {} -> {}", 
            pcTexturePath.filename().string(), cachePath.filename().string());

        return cachePath;
    }

    /**
     * Clear the texture conversion cache.
     */
    void ClearTextureCache()
    {
        g_convertedTextureCache.clear();
        
        std::filesystem::path cacheDir = std::filesystem::temp_directory_path() / 
                                         "LibertyRecomp" / "texture_cache";
        std::error_code ec;
        std::filesystem::remove_all(cacheDir, ec);
    }
}
